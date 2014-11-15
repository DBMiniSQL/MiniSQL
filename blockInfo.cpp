#include "Definition.h"

blockInfo::blockInfo(){
	blockNum = -1;
	dirtyBit = 0;
	next = NULL;
	file = NULL;
	charNum = 0;
	cBlock = new char[BLOCKSIZE];
	father = -1;
	iTime = 0;
	lock = 0;	
}

blockInfo::~blockInfo(){
	delete [] cBlock;
}

void blockInfo::clearBlock(){
	blockNum = -1;
	dirtyBit = 0;
	next = NULL;
	file = NULL;
	charNum = 0;
	delete [] cBlock;
	cBlock = new char[BLOCKSIZE];
	father = -1;
	iTime = 0;
	lock = 0;
}