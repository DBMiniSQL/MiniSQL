#ifndef _FILEINFO_H_
#define _FILEINFO_H_

#include "Definition.h"

using namespace std;

class BlockInfo;

class FileInfo{
public:
	int type;					//0 for Data File
	//1 for Index File
	string fileName;			//the name of the file
	int recordAmount;			//the number of the record in th file
	int freeNum;				//the free block number which could be used for the file
	FileInfo* next;				//the pointer points to the next file
	BlockInfo* firstBlock;		//point to the first blcok within the file

	FileInfo(){};
	FileInfo(int fileType, string name){
		type = fileType;
		fileName = name;
		recordAmount = 0;
		freeNum = 0;
		next = NULL;
		firstBlock = NULL;
	}
	~FileInfo(){};
};

#endif