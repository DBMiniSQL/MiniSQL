/***********************************************
**Buffer Module
************************************************/

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "Definition.h"
#include "FileInfo.h"
#include "BlockInfo.h"
#include "Catalog.h"

using namespace std;

extern CatalogManager myCatalog;

class fileHandle{
public:
	FileInfo* firstFile;		//the first file in the list of files
	int fileNum;				//the number of files in the buffer
	int blockAmount;				//the number of blocks in the buffer

	fileHandle():firstFile(NULL),fileNum(0),blockAmount(0){};
	~fileHandle(){};
};

class Buffer{
public:
	fileHandle* fileHead;

	Buffer(){
		fileHead = new fileHandle;
	}

	~Buffer(){
		delete fileHead;
	}
	//get the block amount
	int getBlockAmount(string dbName,string name,int fileType);

	//find the data block required
	BlockInfo* getBlock(string dbName,string name,int blockNum,int type);

	//find an available block required
	BlockInfo* getAvaBlock(string dbName,string name); 

	//find an empty Block for index
	BlockInfo* getEmptyBlock(string dbName,string name);

	//find the file required or available
	FileInfo* findFile(string dbName,string fileName,int fileType);

	//find an available block
	BlockInfo* findBlock(string dbName);

	//find the required block from the Files
	void readBlock(string dbName,string name,int fileType,int blockNum,BlockInfo* tempBlock);

	//find an empty block form the Files
	void readEmptyBlock(string dbName,string name,int fileType,BlockInfo* tempBlock);
	
	//write back the block to the Files
	void writeBlock(string dbName,BlockInfo* tempBlock);

	//add the block to the file
	void addBlock(string DB_Name,string name,FileInfo* tempFile,BlockInfo* tempBlock);

	//delete a block for index
	void deleteBlock(string dbName,string name,BlockInfo* tempBlock);

	//close the file
	void closeFile(string dbName,string fileName,int fileType);

	//close the database
	void closeDatabase(string dbName);

	//quit the process
	bool quitProc(string dbName);

};

#endif