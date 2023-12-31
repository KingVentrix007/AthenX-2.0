#include "TFTP.h"

#include "printf.h"
#include "Ethernet.h"

// TODO: Support multiple transactions
uint16_t transactionID;
volatile bool transferInProgress = false;
volatile bool transferError = false;
bool determiningFileSize = false;   // if determiningFileSize is true, don't store the file in a buffer

uint8_t *tftpFileBuffer = NULL;
uint32_t tftpFileBufferSize = 0;
uint8_t *nextFilePointer;
uint16_t currentBlockNumber;
uint16_t virtualBoxBlockNumber; // workaround for VirtualBox's bullshit
uint32_t tftpFileSize;
uint32_t tftpTaskIndex; // TEMPTEMP HACKHACK

uint32_t tftpServerIP = 0xC0A80001; // TODO, maybe, support more than one server

bool tftpHideErrors = false;

bool TFTP_TransactionComplete()
{
    return !transferInProgress;
}

// return true on success
// will block until the file is transacted
// TODO: Implement timeout, error-checking
// TODO: Fix hanging if an invalid serverIP is given
// TODO: We'll need mutexes or some other sync method to support multiple tasks
// And / Or we'll need to replace globals with per-task variables
// actualFileSize can be NULL if the caller doesn't care about the size
bool TFTP_GetFile(uint32_t serverIP, const char *filename, uint8_t *destinationBuffer, uint32_t maxFileSize, uint32_t *actualFileSize)
{
    // TEMPTEMP HACKHACK block until any existing transfers have completed
    while (transferInProgress)
    {
    }

    if(actualFileSize)
        *actualFileSize = 0;

    if (!NIC_Present)
        return false;

    tftpFileBuffer = destinationBuffer;
    tftpFileBufferSize = maxFileSize;
    // tftpTaskIndex = currentTask;

    if (0)
    {
        printf("TFTP requesting ");
        printf(filename);
        printf("\n");
    }

    TFTP_RequestFile(serverIP, filename, TFTP_TYPE_BINARY, NIC_MAC);

    // wait until the transfer has completed
    while (transferInProgress)
    { }

    if (transferError)
    {
        if(!tftpHideErrors)
            printf("An error occurred with the TFTP transfer\n");
        return false;
    }

    if (0)
    {
        printf("TFTP File Size: ");
        printf("%d",tftpFileSize);
        printf("\n");
    }

    if(actualFileSize)
        *actualFileSize = tftpFileSize;

    return true;
}

// TODO: Same as above
// Retrieves the size of a file. This can take a while for large files, because we actually have to retrieve the entire file to know its size.
bool TFTP_GetFileSize(uint32_t serverIP, const char *filename, uint32_t *pActualFileSize)
{
    // TEMPTEMP HACKHACK block until any existing transfers have completed
    while (transferInProgress)
    {
    }

    // Ensure pActualFileSize is a valid pointer
    if (!pActualFileSize)
        return false;
    
    if (!NIC_Present)
        return false;

    // TFTP doesn't support retreiving a file's size, so we request the file from the server, but don't store the file returned
    determiningFileSize = true;
    // tftpTaskIndex = currentTask;

    // Request the file from the server
    TFTP_RequestFile(serverIP, filename, TFTP_TYPE_BINARY, NIC_MAC);

    // wait until the transfer has completed
    while (transferInProgress)
    { }

    determiningFileSize = false;

    if (transferError)
    {
        if (!tftpHideErrors)
            printf("An error occurred with the TFTP transfer\n");
        return false;
    }

    *pActualFileSize = tftpFileSize;

    return true;
}

// source port is used to keep track of different transactions
uint16_t TFTP_RequestFile(uint32_t serverIP, const char *filename, const char *transferMode, uint8_t *sourceMAC)
{
    TFTP_RequestHeader *tftpData;
    size_t filenameLength = strlen(filename) + 1; // length of filename plus null terminator
    size_t transferModeLength = strlen(transferMode) + 1;

    // packet size is 2 bytes (for the opcode) plus length of filename plus length of transferMode (both strings are zero terminated)
    uint16_t packetSize = (uint16_t)(sizeof(tftpData->opcode) + filenameLength + transferModeLength);
    uint16_t dataSize = packetSize;
    uint16_t sourcePort = 1234; // TODO: should be a random number (I think)
    
    // set some globals that will keep track of the transaction
    transactionID = sourcePort;
    transferInProgress = true;
    nextFilePointer = tftpFileBuffer;
    currentBlockNumber = 1; // TFTP Starts with block number 1
    tftpFileSize = 0;
    tftpServerIP = serverIP;
    transferError = false;

    // create the packet to send
    Ethernet_Header *packet = UDP_Create_Packet(serverIP, sourcePort, TFTP_PORT, &packetSize, sourceMAC, &tftpData);

    memset(tftpData, 0, dataSize);
    tftpData->opcode = TFTP_OP_READ_REQUEST;

    strncpy(tftpData->filename, filename, filenameLength - 1);
    char *transferModePtr = (char *)((uint32_t)tftpData->filename + filenameLength);
    strncpy(transferModePtr, transferMode, transferModeLength);

    EthernetSendPacket(packet, packetSize);

    return sourcePort;
}

// source port is used to keep track of different transactions
// TODO: Seems like Qemu and VirtualBox don't support writing files via TFTP so I can't finish this :(
uint16_t TFTP_WriteFile(uint32_t serverIP, const char *filename, const char *transferMode, uint8_t *sourceMAC)
{
    TFTP_RequestHeader *tftpData;
    size_t filenameLength = strlen(filename) + 1; // length of filename plus null terminator
    size_t transferModeLength = strlen(transferMode) + 1;

    // packet size is 2 bytes (for the opcode) plus length of filename plus length of transferMode (both strings are zero terminated)
    uint16_t packetSize = (uint16_t)(sizeof(tftpData->opcode) + filenameLength + transferModeLength);
    uint16_t dataSize = packetSize;
    uint16_t sourcePort = 1235; // TODO: should be a random number (I think)

                                // set some globals that will keep track of the transaction
    transactionID = sourcePort;
    transferInProgress = true;
    nextFilePointer = tftpFileBuffer;
    currentBlockNumber = 0; // TFTP Starts with block number 1
    tftpFileSize = 0;
    tftpServerIP = serverIP;
    transferError = false;

    // create the packet to send
    Ethernet_Header *packet = UDP_Create_Packet(serverIP, sourcePort, TFTP_PORT, &packetSize, sourceMAC, &tftpData);

    memset(tftpData, 0, dataSize);
    tftpData->opcode = TFTP_OP_WRITE_REQUEST;

    strncpy(tftpData->filename, filename, filenameLength - 1);
    char *transferModePtr = (char *)((uint32_t)tftpData->filename + filenameLength);
    strncpy(transferModePtr, transferMode, transferModeLength);

    EthernetSendPacket(packet, packetSize);

    return sourcePort;
}

void TFTP_SendAck(uint16_t blockNumber, uint16_t transID, uint16_t destinationPort, uint8_t *sourceMAC)
{
    TFTP_Ack *tftpAck;
    uint16_t packetSize = sizeof(TFTP_Ack);
    uint16_t sourcePort = transID;

    if (0)
    {
        printf("TFTP sending ack for block ");
        printf("%d",SwapBytes16(blockNumber));
        printf("\n");
    }

    // create the packet to send
    Ethernet_Header *packet = UDP_Create_Packet(tftpServerIP, sourcePort, destinationPort, &packetSize, sourceMAC, &tftpAck);
    tftpAck->opcode = TFTP_OP_ACK;
    tftpAck->blockNumber = blockNumber;

    EthernetSendPacket(packet, packetSize);
}

// transactionID will be the destination port of the UDP packet
void TFTP_ProcessDataPacket(TFTP_DataHeader *dataPacket, uint16_t sourcePort, uint16_t transID, uint16_t packetLength, uint8_t *sourceMAC)
{
    // data packet will use four bytes for opcode and block number
    uint16_t dataSize = packetLength - 4;

    if(0)
        printf("TFTP Data received\n");

    // Ensure file isn't too big for the destination buffer
    if (!determiningFileSize && tftpFileSize + dataSize > tftpFileBufferSize)
    {
        // Display an error, but not more than once
        if(!transferError)
            printf("Error: requested tftp file exceeds buffer!\n");
        transferInProgress = false;
        transferError = true;

        // send acknowledgement to the server
        TFTP_SendAck(dataPacket->blockNumber, transID, sourcePort, sourceMAC);
        return;
    }
    
    // Ensure block number matches expected number
    uint16_t serverBlockNumber = SwapBytes16(dataPacket->blockNumber);
    if (currentBlockNumber != serverBlockNumber)
    {
        // Workaround for VirtualBox which will always send an extra last packet
        if (virtualBoxBlockNumber != serverBlockNumber)
        {
            printf("Error: tftp packet block number, ");
            printf("%d",serverBlockNumber);
            printf(" doesn't match expected value of ");
            printf("%d",currentBlockNumber);
            printf("\n");
            transferInProgress = false;
            transferError = true;
        }
        return;
    }   

    // if (!determiningFileSize)
    // {
	// 	// TEMPTEMP HACKHACK
    //     // Now we've received some data, but we have to be careful because the task that's currently active
    //     // may not have the same virtual memory space as the task that gave us the destination buffer.
    //     // We need to ensure interrupts are disabled because we can't allow a context switch while we play with page tables.
    //     __asm
    //     {
    //         // TODO: Are interrupts already disabled when we reach this code? Need to check on that.
    //         pushf
    //         cli
    //     }

    //     // Ensure we have the proper page directory loaded for nextFilePointer for the task which requested the file
    //     if (currentTask == tftpTaskIndex || tasks[currentTask].cr3 == tasks[tftpTaskIndex].cr3)
    //     {
    //         // We don't need to do anything with paging

    //         // copy received data to the file buffer
    //         memcpy(nextFilePointer, dataPacket->data, dataSize);
    //         nextFilePointer += dataSize;
    //     }
    //     else
    //     {
    //         // We need to swap out the page directory with the one for the task that requested the file
    //         __writecr3(tasks[tftpTaskIndex].cr3);

    //         // copy received data to the file buffer
    //         memcpy(nextFilePointer, dataPacket->data, dataSize);
    //         nextFilePointer += dataSize;

    //         // restore the page directory of the current task
    //         __writecr3(tasks[currentTask].cr3);
    //     }

    //     // Restore interrupt state
    //     __asm
    //     {
    //         popf
    //     }
    // }

    tftpFileSize += dataSize;
    ++currentBlockNumber;
    
    // send acknowledgement to the server
    TFTP_SendAck(dataPacket->blockNumber, transID, sourcePort, sourceMAC);

    // see if this was the last block
    if (dataSize < TFTP_MAX_BLOCK_SIZE)
        transferInProgress = false;

    // workaround VirtualBox, which will always send an extra 0-byte-length last data packet
    virtualBoxBlockNumber = currentBlockNumber;

    return;
}

void TFTP_ProcessPacket(TFTP_Header *packet, uint16_t sourcePort, uint16_t destinationPort, uint16_t packetLength, uint8_t *sourceMAC)
{
    // TODO: Dynamic memory allocation
    if(0)
        printf("TFTP Packet received\n");

    if (packet->opcode == TFTP_OP_DATA)
    {
        TFTP_ProcessDataPacket((TFTP_DataHeader *)packet, sourcePort, destinationPort, packetLength, sourceMAC);
        return;
    }
    
    if (packet->opcode == TFTP_OP_ERROR)
    {
        TFTP_ErrorHeader *errorPacket = (TFTP_ErrorHeader *)packet;

        if (!tftpHideErrors)
        {
            printf("TFTP Error packet received: ");
            printf(errorPacket->errorMessage);
            printf("\n");
        }

        transferError = true;
        transferInProgress = false;
        return;
    }

    if(packet->opcode == TFTP_OP_OPTION_ACK)
    {
        // send acknowledgement to the server
        TFTP_SendAck(0, destinationPort, sourcePort, sourceMAC);
        return;
    }
    
    printf("Unhandled TFTP Opcode!\n");
}