#include "ARP.h"
#include "DHCP.h"
#include "Ethernet.h"
#include "UDP.h"
#include "e1000.h"
#include "printf.h"
#include "IPv4.h"
#include "TFTP.h"

void DHCP_Send_Discovery(uint8_t *sourceMAC)
{

    
    // uint8_t sourceMAC[6];
    // printf("sourceMAC ->");
    // for (size_t i = 0; i < sizeof(sourceMAC)/sizeof(sourceMAC[0]); i++)
    // {
    //         sourceMAC[i] = mac.bytes[i];
    //         printf("%0X:",sourceMAC[i]);
    // }
    // printf("\n");
    DHCP_HEADER *dhcpData;
    uint16_t packetSize = sizeof(DHCP_HEADER) - 76 + 8;
    Ethernet_Header *packet = UDP_Create_Packet(0xFFffFFff, 68, 67, &packetSize, sourceMAC, &dhcpData);

    dhcpData->opcode = DHCP_OPCODE_DISCOVER;
    dhcpData->hardwareType = DHCP_HARDWARE_ETHERNET;
    dhcpData->hardwareAddressLength = DHCP_ETHERNET_LENGTH;
    dhcpData->hops = 0;
    dhcpData->transactionID = 0x1234567; // TODO: SHould be random
    dhcpData->seconds = 0;
    dhcpData->flags = 0;
    dhcpData->clientIP = 0;
    dhcpData->yourIP = 0;
    dhcpData->serverIP = 0xFFffFFff;
    dhcpData->relayIP = 0;
    memcpy(dhcpData->clientHardwareAddress, sourceMAC, 6);
    memset(dhcpData->sname, 0, 64);
    memset(dhcpData->file, 0, 128);
    uint32_t magicCookie = SwapBytes32(DHCP_MAGIC_COOKIE);
    memcpy(dhcpData->options, &magicCookie, 4);
    // add the "DHCP Discover option - code - size (1) - value
    dhcpData->options[4] = DHCP_OPTION_MESSAGE_TYPE;
    dhcpData->options[5] = 1;
    dhcpData->options[6] = DHCP_MSG_TYPE_DISCOVER;
    dhcpData->options[7] = DHCP_OPTION_END;

    //printf("%d",packetSize);
    //printf("\n");

    // now send the packet
    printf("%0X:%0X:%0X:%0X:%0X:%0X\n",sourceMAC[0],sourceMAC[1],sourceMAC[2],sourceMAC[3],sourceMAC[4],sourceMAC[5]);
    EthernetSendPacket(packet, packetSize);
}

/*void DHCP_Send_Request(uint8_t *sourceMAC)
{
    DHCP_HEADER *dhcpData;
    uint16_t packetSize = sizeof(DHCP_HEADER) - 72 + 8; // size of DHCP_HEADER plus size of options
    Ethernet_Header *packet = UDP_Create_Packet(0xFFffFFff, 68, 67, &packetSize, sourceMAC, &dhcpData);

    dhcpData->opcode = DHCP_OPCODE_REQUEST;
    dhcpData->hardwareType = DHCP_HARDWARE_ETHERNET;
    dhcpData->hardwareAddressLength = DHCP_ETHERNET_LENGTH;
    dhcpData->hops = 0;
    dhcpData->transactionID = 0x1234567; // TODO: SHould be random
    dhcpData->seconds = 0;
    dhcpData->flags = 0;
    dhcpData->clientIP = 0;
    dhcpData->yourIP = 0;
    dhcpData->serverIP = 0xFFffFFff;
    dhcpData->relayIP = 0;
    memcpy(dhcpData->clientHardwareAddress, sourceMAC, 6);
    memset(dhcpData->sname, 0, 64);
    memset(dhcpData->file, 0, 128);
    uint32_t magicCookie = SwapBytes32(DHCP_MAGIC_COOKIE);
    memcpy(dhcpData->options, &magicCookie, 4);
    // add the "DHCP Discover option - code - size (1) - value
    dhcpData->options[4] = DHCP_OPTION_MESSAGE_TYPE;
    dhcpData->options[5] = 1;
    dhcpData->options[6] = DHCP_MSG_TYPE_DISCOVER;
    dhcpData->options[7] = DHCP_OPTION_END;

    printf("%d",packetSize);
    printf("\n");

    // now send the packet
    EthernetSendPacket(packet, packetSize);
}*/

uint8_t DHCP_GetMessageType(DHCP_HEADER *reply)
{
    // parse the options field (skip the 4-byte magic cookie)
    uint8_t *currentOption = reply->options + 4;
    
    while (*currentOption != DHCP_OPTION_END)
    {
        /*for (int i = 0; i < 20; ++i)
        {
            printf("0x%x",((uint8_t*)currentOption)[i]);
            printf(' ');
        }
        printf("\n");*/

        if (*currentOption == DHCP_OPTION_MESSAGE_TYPE)
        {
            return currentOption[2];
        }
        if (currentOption[1] == 0)
            break;
        currentOption += currentOption[1];
    }

    return 0;
}

// TODO: Support multiple NIC's
void DHCP_ProcessAck(DHCP_HEADER *reply)//, uint8_t *sourceMAC)
{
    printf("     DHCP Acknowledge received! Our IP is ");
    IPv4_PrintIP(reply->yourIP);
    printf("\n");
    
    IPv4_SetClientIP(reply->yourIP);

    uint32_t serverIP = reply->serverIP;
    IPv4_SetGateway(serverIP);  // may be overridden by DHCP_OPTION_ROUTER option

    // tftpServerIP = serverIP;

    // process DHCP options (skip 4 bytes for the "magic cookie")
    uint8_t *currentPointer = reply->options + 4;
    uint32_t optionIP;
    while (*currentPointer != DHCP_OPTION_END)
    {
        // ensure option size isn't 0
        if (currentPointer[1] == 0)
            break;

        switch (currentPointer[0])
        {
            case DHCP_OPTION_SUBNET_MASK:
                memcpy(&optionIP, &currentPointer[2], 4);
                IPv4_SetSubnetMask(optionIP);
                break;
            case DHCP_OPTION_ROUTER:
                memcpy(&optionIP, &currentPointer[2], 4);
                IPv4_SetGateway(optionIP);
                break;
            case DHCP_OPTION_DNS_SERVER:
                memcpy(&optionIP, &currentPointer[2], 4);
                IPv4_SetDNS_Servers(optionIP);
                break;
        }

        // advance to the next option
        // currentPointer[1] holds the size of the data, not counting the first two bytes
        currentPointer += currentPointer[1] + 2;
    }

    //printf("(TODO)\n");
}

void DHCP_ProcessOffer(DHCP_HEADER *reply, uint8_t *sourceMAC)
{
    //printf("DHCP offer received!\n");
    uint32_t myIP = reply->yourIP;
    uint32_t serverIP = reply->serverIP;

    if (1==1)
    {
        printf("     Server ");
        IPv4_PrintIP(serverIP);
        printf(" offered us ");
        IPv4_PrintIP(myIP);
        printf("\n");
    }

    // now we need to send the request
    DHCP_HEADER *dhcpData;
    uint16_t packetSize = sizeof(DHCP_HEADER) - 72 + 20; // size of DHCP_HEADER plus size of options
    Ethernet_Header *packet = UDP_Create_Packet(0xFFffFFff, 68, 67, &packetSize, sourceMAC, &dhcpData);

    dhcpData->opcode = DHCP_OPCODE_REQUEST;
    dhcpData->hardwareType = DHCP_HARDWARE_ETHERNET;
    dhcpData->hardwareAddressLength = DHCP_ETHERNET_LENGTH;
    dhcpData->hops = 0;
    dhcpData->transactionID = reply->transactionID;
    dhcpData->seconds = 0;
    dhcpData->flags = 0;
    dhcpData->clientIP = 0;
    dhcpData->yourIP = 0;
    dhcpData->serverIP = reply->serverIP;
    dhcpData->relayIP = 0;
    memcpy(dhcpData->clientHardwareAddress, sourceMAC, 6);
    memset(dhcpData->sname, 0, 64);
    memset(dhcpData->file, 0, 128);

    // add the DHCP cookie "option"
    uint32_t magicCookie = SwapBytes32(DHCP_MAGIC_COOKIE);
    memcpy(dhcpData->options, &magicCookie, 4);

    // add the "DHCP Request option - code - size (1) - value
    dhcpData->options[4] = DHCP_OPTION_MESSAGE_TYPE;
    dhcpData->options[5] = 1; // size of message
    dhcpData->options[6] = DHCP_MSG_TYPE_REQUEST;

    // add the IP requested option - code - size - IP
    dhcpData->options[7] = DHCP_OPTION_REQUEST_IP;
    dhcpData->options[8] = sizeof(uint32_t); // size of message
    memcpy(&dhcpData->options[9], &reply->yourIP, 4);

    // add the server IP option - code - size - IP
    dhcpData->options[13] = DHCP_OPTION_SERVER_IP;
    dhcpData->options[14] = sizeof(uint32_t); // size of message
    memcpy(&dhcpData->options[15], &reply->serverIP, 4);
    // add the end
    dhcpData->options[19] = DHCP_OPTION_END;

    //printf("%d",packetSize);
    //printf("\n");

    // now send the packet
    EthernetSendPacket(packet, packetSize);
}

void DHCP_ProcessReply(DHCP_HEADER *reply, uint8_t *sourceMAC)
{
    if (reply->opcode == DHCP_OPCODE_OFFER)
    {
        uint8_t messageType = DHCP_GetMessageType(reply);
        if (messageType == DHCP_MSG_TYPE_OFFER)
        {
            DHCP_ProcessOffer(reply, sourceMAC);
            return;
        }

        if (messageType == DHCP_MSG_TYPE_ACK)
        {
            DHCP_ProcessAck(reply);// , sourceMAC);
            return;
        }

        printf("TODO: Unhandled DHCP message type ");
        printf("0x%x",messageType);
        printf("\n");
        /*for (;;)
            __halt();*/
    }
    else
    {
        // NOTE: DHCP uses the same opcode for acknowledge or ACK
        /*if (reply->opcode == DHCP_OPCODE_ACKNOWLEDGE)
            DHCP_ProcessAck(reply, sourceMAC);*/
        //else
            printf("\n\nunkown DHCP opcode received\n\n");
    }

}