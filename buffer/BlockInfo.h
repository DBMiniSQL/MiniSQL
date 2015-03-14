#ifndef _BLOCKINFO_H_
#define _BLOCKINFO_H_

#include "Definition.h"
#include "FileInfo.h"

using namespace std;

class BlockInfo
{
public:
	int blockNum;
	bool dirtyBit;
	BlockInfo* next;
	FileInfo* file;
	int charNum;
	char* cBlock;
	int iTime;
	int lock;
	int father;
	bool isFull;

	BlockInfo(){
		blockNum = -1;
		dirtyBit = 0;
		next = NULL;
		file = NULL;
		charNum = 0;
		cBlock = new char[BLOCKSIZE];
		father = -1;
		iTime = 0;
		lock = 0;
		isFull = 0;
	}
	~BlockInfo(){
		delete[] cBlock;
	}
	void clearBlock(){
		blockNum = -1;
		dirtyBit = 0;
		next = NULL;
		file = NULL;
		charNum = 0;
		father = -1;
		iTime = 0;
		lock = 0;
		isFull = 0;
	}

	void deleteBlock(){
		dirtyBit = 1;
		charNum = 0;
		delete[] cBlock;
		cBlock = new char[BLOCKSIZE];
		father = -1;
		iTime = 0;
		lock = 0;
		isFull = 0;
	}

};

#endif
