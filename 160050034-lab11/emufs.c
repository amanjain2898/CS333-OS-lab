#include "emufs.h"

/**************************** File system helper functions ****************************/

//	Function to encrypt a block of data 
int encrypt(char* input, char* encrypted)
{
	for(int i=0; i<BLOCKSIZE; i++)
	{
		encrypted[i] = ~input[i];
	}
}

//	Function to decrypt a block of data 
int decrypt(char* input, char* decrypted)
{
	for(int i=0; i<BLOCKSIZE; i++)
	{
		decrypted[i] = ~input[i];
	}
}

//	The following helper functions are used to read and write 
//	superblock and metadata block. 
//	Change the function definitions with required arguments
struct superblock_t* readSuperblock(struct mount_t* mt)
{
	/*
		* Read 0th block from the device into a blocksize buffer
		* Create superblock_t variable and fill it using reader buffer
		* Return the superblock_t variable
	*/
	char* buf = malloc(512*sizeof(char));
	readblock(mt->device_fd,0,buf);
	struct superblock_t* superblock = (struct superblock_t*)malloc(sizeof(struct superblock_t));
	memcpy(superblock, buf, sizeof(struct superblock_t));
	return superblock;
}

int writeSuperblock(struct mount_t* mt,struct superblock_t* superblock)
{
	/*
		* Read the 0th block from device into a buffer
		* Write the superblock into the buffer
		* Write back the buffer into block 0
	*/
	char* buf = malloc(512*sizeof(char));
	memcpy(buf, superblock, sizeof(struct superblock_t));
	writeblock(mt->device_fd,0,buf);
	return 1;
}

struct metadata_t* readMetadata(struct mount_t* mt)
{
	char* buf = malloc(512*sizeof(char));
	int x = readblock(mt->device_fd,1,buf);

	if(x < 0){
		return NULL;
	}

	struct metadata_t* metablock = (struct metadata_t*)malloc(sizeof(struct metadata_t));
	if(mt->fs_number == 1){
		decrypt(buf,buf);
	}
	memcpy(metablock, buf, sizeof(struct metadata_t));
	return metablock;
	// Same as readSuperBlock(), but it is stored on block 1
	// Need to decrypt if emufs-encrypted is used  

}

int writeMetadata(struct mount_t* mt,struct metadata_t* metablock)
{
	// Same as writeSuperblock(), but it is stored on block 1
	// Need to decrypt/encrypt if emufs-encrypted is used  
	char* buf = malloc(512*sizeof(char));
	memcpy(buf, metablock, sizeof(struct metadata_t));
	if(mt->fs_number == 1){
		encrypt(buf,buf);
	}
	int x = writeblock(mt->device_fd,1,buf);
	if(x < 0) return -1;
	return 1;


}

/**************************** File system API ****************************/

int create_file_system(struct mount_t *mount_point, int fs_number)
{
	/*
	   	* Read the superblock.
	    * Set file system number on superblock
		* Clear the bitmaps.  values on the bitmap will be either '0', or '1', or'x'. 
		* Create metadata block in disk
		* Write superblock and metadata block back to disk.

		* Return value: -1,		error
						 1, 	success
	*/
	struct superblock_t* superblock = readSuperblock(mount_point);
	mount_point->fs_number = fs_number;
	superblock->fs_number = fs_number;
	for(int i=0;i<MAX_BLOCKS;i++){
		if(i< superblock->disk_size)
			superblock->bitmap[i] = 0;
		else
			superblock->bitmap[i] = 'x';
	}

	struct metadata_t* metablock = (struct metadata_t*)malloc(sizeof(struct metadata_t));

	if(writeSuperblock(mount_point,superblock) < 0){
		return -1;
	}

	for(int i=0;i<MAX_FILES;i++){
		for(int j=0;j<4;j++){
			metablock->inodes[i].blocks[j] = -1;
		}
	}	

	if(writeMetadata(mount_point,metablock) < 0){
		return -1;
	}

	superblock->bitmap[0] = 1;
	superblock->bitmap[1] = 1;

	writeSuperblock(mount_point,superblock);
	writeMetadata(mount_point,metablock);

	printf("File system created.\n");
	return 1;
}


struct file_t* eopen(struct mount_t* mount_point, char* filename)
{
	/* 
		* If file exist, get the inode number. inode number is the index of inode in the metadata.
		* If file does not exist, 
			* find free inode.
			* allocate the free inode as USED
			* if free id not found, print the error and return -1
		* Create the file hander (struct file_t)
		* Initialize offset in the file hander
		* Return file handler.

		* Return NULL on error.
	*/
	struct metadata_t* metablock = readMetadata(mount_point);
	int flag= 0,inode_num=0;
	for(int i=0;i<10;i++){
		if(metablock->inodes[i].status == 1){
			if(strcmp(metablock->inodes[i].name,filename) == 0){
				flag=1;
				inode_num = i;
			}
		}
	}

	int free_inode=0;

	if(flag == 1){
		free_inode = inode_num;
	}
	else{
		while(free_inode < MAX_FILES){
			if(metablock->inodes[free_inode].status == 0)
				break;
			free_inode++;
		}

		if(free_inode == MAX_FILES){
			printf("No free inode found\n");
			return NULL;
		}
		strcpy(metablock->inodes[free_inode].name,filename);
		metablock->inodes[free_inode].status = 1;
		time_t t;
		metablock->inodes[free_inode].modtime = time(&t);
	}

	struct file_t* ft = (struct file_t*)malloc(sizeof(struct file_t));

	if(ft < 0) return NULL;
	ft->offset = 0;
	ft->inode_number = free_inode;
	ft->mount_point = mount_point;

	writeMetadata(mount_point,metablock);

	printf("File opened\n");
	return ft;
}

int ewrite(struct file_t* file, char* data, int size)
{
	// You do not need to implement partial writes in case file exceeds 4 blocks
	// or no free block is available in the disk. 

	// Return value: 	-1,  error
	//					Number of bytes written

	int init = size;
	struct superblock_t* superblock = readSuperblock(file->mount_point);
	struct metadata_t* metablock = readMetadata(file->mount_point);

	int inode_number = file->inode_number;

	if(file->offset+size > 4*BLOCKSIZE) {
		return -1;
	}

	int unalloc=0,unalloc2=0;

	for(int i=metablock->inodes[inode_number].blocks[file->offset/512];i<metablock->inodes[inode_number].blocks[(file->offset+size)/512];i++){
		if(superblock->bitmap[i] == 0){
			unalloc2++;
		}
	}

	for(int j=0;j<MAX_BLOCKS;j++){
		if(superblock->bitmap[j] == 0){
			unalloc++;
		}
	}

	if(unalloc < unalloc2){
		printf("Error: Out of diskspace\n");
		return -1;
	}

	while(size!=0){
		int block=0;
		if(metablock->inodes[inode_number].blocks[file->offset/512] == -1){
			for(int j=2;j<MAX_BLOCKS;j++){
				if(superblock->bitmap[j] == 0){
					block=j;
					superblock->bitmap[j] = 1;
					metablock->inodes[inode_number].file_size += 512;
					metablock->inodes[inode_number].blocks[file->offset/512] = block;
					time_t t;
					metablock->inodes[inode_number].modtime = time(&t);
					break;
				}
			}
		}
		else
			block = metablock->inodes[inode_number].blocks[file->offset/512];

		writeblock(file->mount_point->device_fd,block,data);
		data+=512;
		size-=512;
		file->offset += 512;

	}

	writeSuperblock(file->mount_point,superblock);
	writeMetadata(file->mount_point,metablock);


	// if(file->mount_point->fs_number == 1){
	// 	encrypt(data,data);
	// }
	
	return init;
}



int eread(struct file_t* file, char* data, int size)
{
	// NO partial READS.

	// Return value: 	-1,  error
	//					Number of bytes read
	struct superblock_t* superblock = readSuperblock(file->mount_point);
	struct metadata_t* metablock = readMetadata(file->mount_point);

	int init = size;

	struct inode_t inode = metablock->inodes[file->inode_number];
	// if(file->offset+size > 4*BLOCKSIZE) return -1;
	if(file->offset+size > inode.file_size){
		printf("Error: Invalid offset\n");
		return -1;
	}

	
	while(size>0){
		int block = inode.blocks[file->offset/512];
		readblock(file->mount_point->device_fd,block,data);
		data+=512;
		file->offset += 512;
		size-=512;
	}

	// if(file->mount_point->fs_number == 1){
	// 	decrypt(data,data);
	// }
	return init;
}

void eclose(struct file_t* file)
{
	// free the memory allocated for the file handler 'file'
	if(file ==NULL) return;
	printf("File closed\n");
	free(file);
}

int eseek(struct file_t *file, int offset)
{
	// Change the offset in file hanlder 'file'
	if(offset > 4*BLOCKSIZE) return -1;
	file->offset = offset;

	return 1;
}

void fsdump(struct mount_t* mount_point)
{

	printf("\n[%s] fsdump \n", mount_point->device_name);
	printf("%-10s %6s \t[%s] \t%s\n", "  NAME", "SIZE", "BLOCKS", "LAST MODIFIED");

	struct superblock_t* superblock = readSuperblock(mount_point);
	struct metadata_t* metablock = readMetadata(mount_point);

	for(int i=0;i<MAX_FILES;i++){
		struct inode_t inode = metablock->inodes[i];
		if(inode.status == 0) continue;

		printf("%-10s %6d \t[%d %d %d %d] \t%s", 
			inode.name, 
			inode.file_size,
			inode.blocks[0],
			inode.blocks[1],
			inode.blocks[2],
			inode.blocks[3],
			asctime(localtime(&(inode.modtime))));
	}
	

}
