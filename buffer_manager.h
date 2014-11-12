/***********************************************
**对MiniSQL的修改：
**1.修改了接口！！！！！
**
**疑问：
**1.不需要使用using namespace std？
**2.结构体声明为类会不会好一点？
**3.findBlock传DB_Name有什么用？
**4.我如何确定我的blockNum
**
**Tips:
**1.最常用的放在链表的最后，LRU的直接比较文件头
**2.iTime越大表示被用得越多
**4.对于文件系统的假定：
**	1.使用catalog文件存储所有数据库的结构信息
**	2.使用DB_File.db文件存储所有的数据文件信息
**	3.使用DB_File.index文件存储所有的索引文件信息
**	4.文件的最前面先写三个int的参数，依次是：blockAmount，recordLength，recordAmout
**	5.所有的文件都放在根目录下面，之后再考虑添加文件夹的操作
**	6.writeBlock的范围：
**		1.上层插入数据之后，上层修改数据之后，关闭File或者Block之前写回
**		2.文件的创建貌似是catalog的事情，块的插入貌似是record的事情
**5.多出口的goto语句
**
**接口说明
************************************************/

#if !defined(_BUFFER_MANAGER_H_)
#define _BUFFER_MANAGER_H_

#include "MiniSQL.h"
#include <fstream>

#define BLOCK_LEN		4096	//the size of the block
#define MAX_FILE_ACTIVE	5		//limit the active files in the buffer
#define MAX_BLOCK		50 		//the max number of blocks

typedef struct fileInfo{
	int type;					//0 for Data File
								//1 for Index File
	string fileName;			//the name of the file
	int recordAmount;			//the number of the record in th file
	int freeNum;				//the free block number which could be used for the file
								//暂时不使用，用到的时候再分配块到文件
	fileInfo* next;				//the pointer points to the next file
	blockInfo* firstBlock;		//point to the first blcok within the file
} fileInfo;

//最好能够在BlockInfo里面加上recordAmount!
typedef struct blockInfo{
	int blockNum;				//the block number of the block，which is indicated when it be newed
	bool dirtyBit;				//1->need to be write back
	blockInfo*	 next;			//the pointer points to the next block
	fileInfo* file;				//the pointer points to the file,which the block belongs to
	int charNum;				//the number of chars in the block
	char* cBlock;				//the array space for storing the records in the block in buffer
	int iTime;					//indicate the age of the block in use
	int lock;					//prevent the block from replacing
} blockInfo;

// 不清楚如何使用
// typedef struct indexInfo{
// 	string index_name;			//the name of the index file
// 	int length;					//the length of the value
// 	char type;					//the type of the value
// 								//0 for int,1 for float,2 for char(n)
// 	long offset;				//the record offset in the table file
// 	string value;				//the value
// } indexInfo;

typedef struct attr_info		
{
	string attr_name;
	int offset;					
	int length;
	char type;
} attr_info;

typedef struct fileHandle{
	fileInfo* firstFile;		//the first file in the list of files
	int fileNum;				//the number of files in the buffer
	int blockAmout;				//the number of blocks in the buffer
} fileHandle;

//find the block required
blockInfo* getBlock(string DB_Name,string Table_Name,int fileType,int blockNum);

//find the file required or available
fileInfo* findFile(string DB_Name,string fileName,int fileType);

//find an available block
blockInfo* findBlock(string DB_Name);

//find the required block from the Files
void readBlock(string DB_Name,string Table_Name,int fileType,int blockNum,blockInfo* tempBlock);

//write back the block to the Files
void writeBlock(string DB_Name,blockInfo* tempBlock);

//add the block to the file
void addBlock(string DB_Name,string Table_Name,fileInfo* tempFile,blockInfo* tempBlock);

//initial a block
void getBlockInfo(blockInfo* tempBlock);

//initial an indexFile
void getIndexInfo(string DB_Name,string Index_Name,fileInfo* tempFile);

//initial an dataFile
void getTableInfo(string DB_Name,string Table_Name,fileInfo* tempFile);

//close the file
void closeFile(string DB_Name,string fileName,int fileType);

//close the database
void closeDatabase(string DB_Name);

//quit the process
void quitProc(string DB_Name);

#endif