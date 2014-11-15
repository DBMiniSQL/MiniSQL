/***********************************************
**对MiniSQL的修改：
**1.修改了接口！！！！！
**
**疑问：
**1.不需要使用using namespace std？
**2.结构体声明为类会不会好一点？
**3.findBlock传dbName有什么用？
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

#include <fstream>


//最好能够在BlockInfo里面加上recordAmount!

class fileHandle{
public:
	fileInfo* firstFile;		//the first file in the list of files
	int fileNum;				//the number of files in the buffer
	int blockAmout;				//the number of blocks in the buffer

	fileHandle():firstFile(NULL),fileNum(0),blockAmount(0){};
	~fileHandle(){};
};

class Buffer_Manager{
public:
	fileHandle* fileHead;

	Buffer_Manager(){
		fileHead = new fileHandle;
	}

	~Buffer_Manager(){
		delete fileHead;
	}
	//find the data block required
	blockInfo* getBlock(string dbName,string name,int blockNum,int type);

	//find an available block required
	blockInfo* getAvaBlock(string dbName,string name); 

	//find the file required or available
	fileInfo* findFile(string dbName,string fileName,int fileType);

	//find an available block
	blockInfo* findBlock(string dbName);

	//find the required block from the Files
	void readBlock(string dbName,string name,int fileType,int blockNum,blockInfo* tempBlock);

	//write back the block to the Files
	//可能可以把这个和writeBack合并！
	void writeBlock(string dbName,blockInfo* tempBlock);

	//add the block to the file
	void addBlock(string DB_Name,string name,fileInfo* tempFile,blockInfo* tempBlock);

	//find an empty Block for index
	//只有Index会调用！所以不用类型
	blockInfo* getEmptyBlock(string dbName,string name);

	//write the root back
	//写回之前先判断是删除根还是写回新根，通过charNum
	void writeRootBlock(string dbName,string name,blockInfo* tempBlock); 

	//delete a block for index
	//只有index会调用，所以不用类型
	void deleteBlock(string dbName,string name,blockInfo* tempBlock);

	//initial an indexFile
	void getIndexInfo(string dbName,string Index_Name,fileInfo* tempFile);

	//initial an dataFile
	void getTableInfo(string dbName,string name,fileInfo* tempFile);

	//close the file
	void closeFile(string dbName,string fileName,int fileType);

	//close the database
	void closeDatabase(string dbName);

	//quit the process
	void quitProc(string dbName);

};

#endif