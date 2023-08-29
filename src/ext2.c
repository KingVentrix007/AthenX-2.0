#include "debug.h"
#include "ext2.h"
#include "console.h"
#include "ide.h"
#include "string.h"
#include "kheap.h"

#define DRIVE 0 // We are using drive 0, the harddisk

void read_superblock(){
    uint32* superblock = kmalloc(sizeof(*superblock));
	ide_read_sectors(DRIVE, 1, 2, esb);      
}

static int lba_to_ext2_block(int block_num_LBA)
{
	FUNC_ADDR_NAME(&lba_to_ext2_block);
	 int ext2block = block_num_LBA - 1;
	 return ext2block;
}

uint32 determine_blk_group(uint32 inode){
	FUNC_ADDR_NAME(&determine_blk_group);
	printf("\nINODES PER GROUP: %d\n", esb->inodes_per_group);
	printf("INODES-1: %d\n", inode-1);
    uint32 block_group = (inode - 1) / esb->inodes_per_group;
	//DEBUG("");
    return block_group;
}

uint32 get_inode_index(uint32 inode){
	FUNC_ADDR_NAME(&get_inode_index);
    uint32 index = (inode - 1) % esb->inodes_per_group;
    return index;
}

ext2_bgdt* parse_bgdt(uint32 block_group) {
	FUNC_ADDR_NAME(&parse_bgdt);
	ext2_bgdt* bgdt; 
	uint32 blk_group_start_block = (block_group - 1) * esb->blocks_per_group + 1;
	uint32 bgdt_start = blk_group_start_block + 2;
	uint32 *bgdt_tmp = kmalloc(sizeof(bgdt));
	ide_read_sectors(DRIVE, 1, bgdt_start + 1, bgdt);
	printf("\nBlock bitmap: %d\ninode bmap: %d\ninode table start: %d\nUnallocated Blocks: %d\nUnallocated Inodes: %d\nNumber of dirs: %d", bgdt->blk_bmap, bgdt->inode_bmap, bgdt->inode_table_start, bgdt->unalloc_blocks, bgdt->unalloc_inodes, bgdt->num_of_dirs);
	return bgdt;
}

void get_inode_type(uint32 type){
	FUNC_ADDR_NAME(&get_inode_type);
	switch(type) {
			default:
				printf("\nUnknown Inode Type");	
				printf("%x", type);	
				break;
			case INODE_TYPE_FIFO:
				printf("\nInode Type: FIFO");
				break;
			case INODE_TYPE_CHAR_DEV:
				printf("\nInode Type: Character Device");
				break;
			case INODE_TYPE_DIR:
				printf("\nInode Type: Directory");
				break;
			case INODE_TYPE_BLK_DEV:
				printf("\nInode Type: Block Device");
				break;
			case INODE_TYPE_FILE:
				printf("\nInode Type: Regular File");
				break;
			case INODE_TYPE_SYMB_LINK:
				printf("\nInode Type: Symbolic Link");
				break;
			case INODE_TYPE_UNIX_SOCKET:
				printf("\nInode Type: Unix Socket");
				break;	
		}
}

ext2_inode* read_inode(uint32 inode) {
	//DEBUG("");
	FUNC_ADDR_NAME(&read_inode);
	//DEBUG("");
	uint32 group = determine_blk_group(inode) + 1;
	//DEBUG("");
	uint32 index = get_inode_index(inode);
	//DEBUG("");
	ext2_bgdt* inode_bgdt;
	//DEBUG("");
	inode_bgdt = parse_bgdt(group);
	//DEBUG("");
	uint32 inode_table_start = inode_bgdt->inode_table_start;
	//DEBUG("");
	printf("\nReading Inode: %d", inode);
	//printf("\nInode Table Start: %d", inode_table_start);
	uint32 containing_block = (index * INODE_SIZE) / (1024 << esb->log_block_size);
	//DEBUG("containing_block");
	uint32* inode_tmp = malloc(sizeof(inode_tmp));
	ide_read_sectors(DRIVE, 2, 2 * (inode_table_start + containing_block), inode_tmp);
	//DEBUG("read_sectors");
	ext2_inode* inode_info;
	inode_info = (ext2_inode*)((uint32) inode_tmp + (index % (1024/INODE_SIZE))*INODE_SIZE);
	printf("\nInode Type: %x\nInode UID: %x\nInode Block 0: %d\n", inode_info->type_perm, inode_info->user_id, inode_info->direct_block_pointers[0]);
	uint32 index_in_block = inode % 4;
	//DEBUG("END");
	return inode_info;
}

void getdata(char* data, uint32 len, uint32 start, uint32* from){
	FUNC_ADDR_NAME(&getdata);
	for(int i = 0; i < len; i++){
		uint32 index = i % 4;		
		if(index == 0)
			data[i] = from[i + start - 3 * (i / 4)] & 0xFF;
		else if(index == 1)
			data[i] = (from[i + start - 1 - 3 * (i / 4)] >> 8) & 0xFF;
		else if(index == 2)
			data[i] = (from[i + start - 2 - 3 * (i / 4)] >> 16) & 0xFF;
		else if(index == 3)
			data[i] = (from[i + start - 3 - 3 * (i / 4)] >> 24) & 0xFF;						
	}
}

ext2_dirent ext2_read_dirent(uint32* data, uint32 index){
	FUNC_ADDR_NAME(&ext2_read_dirent);
	ext2_dirent dirent;
	dirent.inode = data[index] & 0xFF;
	//printf("\nInode: %d", dirent.inode);
	dirent.dirent_size = data[index + 1] & 0xFF;
	//printf("\nDirent Size: %d", dirent.dirent_size);
	dirent.name_len = (data[index + 1] & 0x0FF000) >> 16;
	//printf("\nName Size: %d", (uint32)dirent.name_len);
	getdata(dirent.name, dirent.name_len, index + 2, data);
	char * tmp = kcalloc(dirent.name_len + 1, 1);
	memcpy(tmp, dirent.name, dirent.name_len);
	dirent.name = tmp;
	return dirent;
}

char **ext2_ls(uint32 inode_num){
	FUNC_ADDR_NAME(&ext2_ls);
	char **return_names = kmalloc(sizeof(**return_names));
	ext2_inode *inode = read_inode(inode_num);
	uint32* dir = kmalloc(sizeof(*dir));
	uint32 datablock0 = inode->direct_block_pointers[0];
	ide_read_sectors(DRIVE, 6, 2 * datablock0, dir);
	int curr = 0;
	int next;
	int i;
	while(1){
		ext2_dirent curr_dirent;
		curr_dirent = ext2_read_dirent(dir, curr);
		if(curr_dirent.inode == 0){
			break;
		}
		//printf("\nInode: %d ", curr_dirent.inode);
		//printf("\nDirent Size: %d", curr_dirent.dirent_size);
		next = curr_dirent.dirent_size / 4;
		//printf("\nName Size: %d", curr_dirent.name_len);
		printf(" %s", curr_dirent.name);
		return_names[i] = curr_dirent.name;
		curr = curr + next;
		i++;
	}
	return return_names;
}

uint32 ext2_find_in_dir(uint32 inode_num, char* dirent_name){
	FUNC_ADDR_NAME(&ext2_find_in_dir);
	uint32 return_inode = FILE_NOT_FOUND;
	ext2_inode* inode = read_inode(inode_num);
	//DEBUG("HERE");
	uint32* dir = kmalloc(sizeof(*dir));
	uint32 datablock0 = inode->direct_block_pointers[0];
	ide_read_sectors(DRIVE, 6, 2 * datablock0, dir);
	int curr = 0;
	int next;
	int i = 0;
	while(1){
		ext2_dirent curr_dirent;
		curr_dirent = ext2_read_dirent(dir, curr);
		if(curr_dirent.inode == 0){
			break;
		}
		//printf("\nInode: %d ", curr_dirent.inode);
		//printf("\nDirent Size: %d", curr_dirent.dirent_size);
		next = curr_dirent.dirent_size / 4;
		//printf("\nName Size: %d", curr_dirent.name_len);
		if(!strcmp(curr_dirent.name, dirent_name)){
			return_inode = curr_dirent.inode;
		}
		curr = curr + next;
		i++;
	}
	return return_inode;
}

char* ext2_get_filedata(uint32 inode_num){
	FUNC_ADDR_NAME(&ext2_get_filedata);
	char* file_buf = kmalloc(sizeof(*file_buf));
	ext2_inode* inode = read_inode(inode_num);
	uint32* filedata = kmalloc(sizeof(filedata));
	uint32 datablock0 = inode->direct_block_pointers[0];
	ide_read_sectors(DRIVE, 1, 2 * datablock0, filedata);
	getdata(file_buf, 1024, 0, filedata);
	//printf("\n%s", file_buf);
	return file_buf;
}

/* Could improve this impl, but it'll do for now.
*Essentially, we look for a '/' or the end of a string, and parse accordingly
*/

uint32 ext2_path_to_inode(char* path){
	//printf("\nParsing Path: %s", path);
	FUNC_ADDR_NAME(&ext2_path_to_inode);
	int i = 0;
	uint32 last_inode = 2;
	BOOL found = FALSE;
	int start = 0;
	int end = 0;
	if(!strcmp(path, "/"))
		return 2;

	while(path[i]){
		if(path[i] == '/' && found == FALSE){
			start = i;
			found = TRUE;	
		}
		// Parsing Path
		else if(path[i] == '/' && found == TRUE){
			end = i;
			char* lookup_name = kcalloc(end - start, 1);
			int y = 0;
			for(int x = start + 1; x < end; x++){
				lookup_name[y] = path[x];
				y++;
			}
			start = end;
			last_inode = ext2_find_in_dir(last_inode, lookup_name);
		}
		// This is the last name
		else if(i == strlen(path) - 1 && found == TRUE){
			end = i;
			char* lookup_name = kcalloc(end - start, 1);
			int y = 0;
			for(int x = start + 1; x <= end; x++){
				lookup_name[y] = path[x];
				y++;
			}
			start = end - 1;
			last_inode = ext2_find_in_dir(last_inode, lookup_name);
		}
		i++;
	}
	return last_inode;
}

char* ext2_read_file(char* fpath){
	FUNC_ADDR_NAME(&ext2_read_file);
	char* file_buf = kmalloc(sizeof(*file_buf));
	uint32 file_inode = ext2_path_to_inode(fpath);
	printf("\nFile Inode: %d", file_inode);
	file_buf = ext2_get_filedata(file_inode);
	printf("%c\n", file_buf);
	return file_buf;
}

void rewrite_bgds(uint32 group, ext2_bgdt new_bgdt){
	FUNC_ADDR_NAME(&rewrite_bgds);
	uint32* data = kmalloc(sizeof(*data));
	memcpy(data, &new_bgdt, sizeof(new_bgdt));
	uint32 blk_group_start_block = (group - 1) * esb->blocks_per_group + 1;
	uint32 bgdt_start = blk_group_start_block + 2;
	ide_write_sectors(DRIVE, 1, bgdt_start + 1, data);
}
void ext2_write_sector(int sector, uint32* data)
{
	FUNC_ADDR_NAME(&ext2_write_sector);
	
	ide_write_sectors(DRIVE, 1, sector, data);
}
char* ext2_read_sector(int sector)
{
	FUNC_ADDR_NAME(&ext2_read_sector);
	char* file_buf = kmalloc(sizeof(*file_buf));
	
	uint32* filedata = kmalloc(sizeof(filedata));
	ide_read_sectors(DRIVE, 1, sector, filedata);
	getdata(file_buf, 1024, 0, filedata);
	return file_buf;
}

int ext2_init(){
	FUNC_ADDR_NAME(&ext2_init);
	if(esb->magic != 0xEF53){
		printf("Filesystem is Not EXT2!\n");
		return -1;
	}
	else {
		printf("Filesystem is EXT2!\n");
		return 0;
	}
}


int read_root_directory_inode()
{
	ext2_inode* root_inode;
	root_inode = read_inode(2);
	
	for (size_t i = 0; i < 12; i++)
	{
		int block = root_inode->direct_block_pointers[i];
		if(block > 0)
		{
			
			printf("%d: ", block);

		}
	}
	
	
}