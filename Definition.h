#ifndef __DEFINITION_H__
#define __DEFINITION_H__

#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>

#define DATAFILE 0
#define INT 1
#define FLOAT 2
#define CHAR 3

#define FILLEMPTY "0"
#define BLOCKSIZE 4096			//the size of the block
	
#define MAX_FILE_ACTIVE	5		//limit the active files in the buffer
#define MAX_BLOCK		50 		//the max number of blocks

using namespace std;

vector<string> values;

class attr_info
{
public:
    string name;
	int type;
	int length;
	bool isPrimeryKey;
	bool isUnique;
	string indexName;
	attr_info():isPrimeryKey(false),isUnique(false){};
	attr_info(string a_name, int a_type, int a_length, bool isP, bool isU):name(a_name), type(a_type), length(a_length), isPrimeryKey(isP), isUnique(isU){};
};

class table_info
{
public:
	string name;
	int attrNum;	//表中含有的属性数
	int blockNum;
	int totalLength;	//值为sum(attributes[i].length)
	vector<attr_info> attributes;
	table_info():attrNum(0), blockNum(0), totalLength(0){};
};

class index_info
{
public:
	string name;
	string tableName;
	string attrName;
	int blockNum;
	index_info():blockNum(0){};
	~index_info(){};
};

class blockInfo
{
public:
	int blockNum;
	bool dirtyBit;
	blockInfo* next;
	fileInfo* file;
	int charNum;
	char* cBlock;
	int iTime;
	int lock;
	int father;
	bool isFull;
	blockInfo();
	~blockInfo();
	void clearBlock();
};

class fileInfo{
public:
	int type;					//0 for Data File
								//1 for Index File
	string fileName;			//the name of the file
	int recordAmount;			//the number of the record in th file
	int freeNum;				//the free block number which could be used for the file
	fileInfo* next;				//the pointer points to the next file
	blockInfo* firstBlock;		//point to the first blcok within the file
	fileInfo();
	~fileInfo();
};


#endif
