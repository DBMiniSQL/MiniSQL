#include "Buffer.h"

int Buffer::getBlockAmount(string& dbName, string& name, int fileType){
	int blockAmount;
	string fileName;

	if (fileType)
		fileName = dbName + "_" + name + ".index";
	else
		fileName = dbName + "_" + name + ".db";
	ifstream infile(fileName, ios::binary);
	if (!infile.is_open()){
		cout << "when read:file open failed" << endl;
		return -1;
	}

	infile.seekg(0, ios::beg);
	infile.read((char*)(&blockAmount), sizeof(blockAmount));
	infile.close();

	return blockAmount;
}

BlockInfo* Buffer::getBlock(string& dbName, string& name, int blockNum, int type){
	string fileName = dbName + '_' + name;
	FileInfo* tempFile = findFile(dbName, fileName, type);

	BlockInfo* tempBlock = tempFile->firstBlock;
	BlockInfo* oldBlock = NULL;

	while (tempBlock){								//search the block
		if (tempBlock->blockNum == blockNum){		//move the block to the tail
			if (tempBlock->next){					//not tail
				if (oldBlock)						//not head
					oldBlock->next = tempBlock->next;
				else								//head
					tempFile->firstBlock = tempBlock->next;
				BlockInfo* tempBlock2 = tempBlock;
				while (tempBlock2->next)
					tempBlock2 = tempBlock2->next;
				tempBlock2->next = tempBlock;
				tempBlock->next = NULL;
			}

			tempBlock->iTime++;						//tail
			tempBlock->lock = 1;
			return tempBlock;
		}
		else{										//search the next block
			oldBlock = tempBlock;
			tempBlock = tempBlock->next;
		}
	}
	//do not find the block
	BlockInfo* tempBlock2 = findBlock(dbName);		//find an available block from buffer
	//load a block from file into buffer
	readBlock(dbName, name, type, blockNum, tempBlock2);
	if (tempBlock2->blockNum != -2)
		addBlock(dbName, name, tempFile, tempBlock2);
	tempBlock2->iTime++;	
	tempBlock2->lock = 1;

	return tempBlock2;
}

BlockInfo* Buffer::getAvaBlock(string& dbName, string& name){
	string fileName = dbName + "_" + name;
	FileInfo* tempFile = findFile(dbName, fileName, 0);

	BlockInfo* tempBlock = tempFile->firstBlock;
	BlockInfo* oldBlock = NULL;
	while (tempBlock){								//search the block
		if (!tempBlock->isFull){						//move the block to the tail
			if (tempBlock->next){					//not tail
				if (oldBlock)						//not head
					oldBlock->next = tempBlock->next;
				else								//head
					tempFile->firstBlock = tempBlock->next;
				BlockInfo* tempBlock2 = tempBlock;
				while (tempBlock2->next)
					tempBlock2 = tempBlock2->next;
				tempBlock2->next = tempBlock;
				tempBlock->next = NULL;
			}

			tempBlock->iTime++;
			tempBlock->lock = 1;
			return tempBlock;
		}
		else{										//search the next block
			oldBlock = tempBlock;
			tempBlock = tempBlock->next;
		}
	}

	BlockInfo* tempBlock2 = findBlock(dbName);

	readEmptyBlock(dbName, name, 0, tempBlock2);
	addBlock(dbName, name, tempFile, tempBlock2);
	tempBlock2->iTime++;
	tempBlock2->lock = 1;

	return tempBlock2;
}

BlockInfo* Buffer::getEmptyBlock(string& dbName, string& name){
	string fileName = dbName + "_" + name;
	FileInfo* tempFile = findFile(dbName, fileName, 1);

	BlockInfo* tempBlock = tempFile->firstBlock;
	BlockInfo* oldBlock = NULL;
	while (tempBlock){										//search the block
		if (tempBlock->charNum == 0){						//move the block to the tail
			if (tempBlock->next){							//not tail
				if (oldBlock)								//not head
					oldBlock->next = tempBlock->next;
				else										//head
					tempFile->firstBlock = tempBlock->next;
				BlockInfo* tempBlock2 = tempBlock;
				while (tempBlock2->next)
					tempBlock2 = tempBlock2->next;
				tempBlock2->next = tempBlock;
				tempBlock->next = NULL;
			}

			tempBlock->iTime++;						//tail
			tempBlock->lock = 1;
			return tempBlock;
		}
		else{										//search the next block
			oldBlock = tempBlock;
			tempBlock = tempBlock->next;
		}
	}

	BlockInfo* tempBlock2 = findBlock(dbName);				//find an available block from buffer
	//load a block from file into buffer
	readEmptyBlock(dbName, name, 1, tempBlock2);
	addBlock(dbName, name, tempFile, tempBlock2);
	tempBlock2->iTime++;
	tempBlock2->lock = 1;

	return tempBlock2;
}

FileInfo* Buffer::findFile(string& dbName, string& fileName, int fileType){
	FileInfo* tempFile = fileHead->firstFile;
	FileInfo* oldFile = NULL;
	while (tempFile){										//search the file
		if ((tempFile->fileName == fileName) && (tempFile->type == fileType)){
			//move the file to the tail
			if (tempFile->next){								//not tail
				if (oldFile)									//not head
					oldFile->next = tempFile->next;
				else										//head
					fileHead->firstFile = tempFile->next;
				FileInfo* tempFile2 = tempFile;
				while (tempFile2->next)
					tempFile2 = tempFile2->next;
				tempFile2->next = tempFile;
				tempFile->next = NULL;
			}
			return tempFile;
		}
		else{
			oldFile = tempFile;
			tempFile = tempFile->next;
		}
	}

	if (fileHead->fileNum >= MAX_FILE_ACTIVE)
		closeFile(dbName, fileHead->firstFile->fileName, fileHead->firstFile->type);
	FileInfo* tempFile2 = new FileInfo(fileType, fileName);

	fileHead->fileNum++;
	if (oldFile)
		oldFile->next = tempFile2;
	else
		fileHead->firstFile = tempFile2;
	tempFile2->next = NULL;

	return tempFile2;
}

BlockInfo* Buffer::findBlock(string& dbName){
	if (fileHead->blockAmount < MAX_BLOCK){
		BlockInfo* tempBlock = new BlockInfo();
		fileHead->blockAmount++;
		return tempBlock;
	}

	else{												//LRU algorithm
		FileInfo* tempFile = fileHead->firstFile;
		BlockInfo* tempBlock = NULL;
		BlockInfo* minDirtyBlock = NULL;
		BlockInfo* minBlock = NULL;

		while (tempFile){
			tempBlock = tempFile->firstBlock;
			if (tempBlock){
				if (tempBlock->dirtyBit && !minDirtyBlock)
					minDirtyBlock = tempBlock;
				else if (tempBlock->dirtyBit && (minDirtyBlock->iTime > tempBlock->iTime))
					minDirtyBlock = tempBlock;
				else if (!tempBlock->dirtyBit && !minBlock)
					minBlock = tempBlock;
				else if (!tempBlock->dirtyBit && (minBlock->iTime > tempBlock->iTime))
					minBlock = tempBlock;
			}
			tempFile = tempFile->next;
		}

		if (minBlock){
			minBlock->file->firstBlock = minBlock->next;
			tempBlock = minBlock;
		}
		else{
			minDirtyBlock->file->firstBlock = minDirtyBlock->next;
			tempBlock = minDirtyBlock;
			writeBlock(dbName, tempBlock);
		}
		tempBlock->clearBlock();
		return tempBlock;
	}
}

void Buffer::readBlock(string& dbName, string& name, int fileType, int blockNum, BlockInfo* tempBlock){
	//从磁盘中读取该块，返回该块
	//考虑重用代码
	string fileName = dbName + '_' + name;
	int blockAmount, emptyAmount;
	int emptyBlock[100];

	if (fileType)										//index file
		fileName += ".index";
	else
		fileName += ".db";

	ifstream infile(fileName, ios::binary);
	if (!infile.is_open()){
		cout << "when read:file open failed" << endl;
		return;
	}

	infile.seekg(0, ios::beg);
	infile.read((char*)(&blockAmount), sizeof(blockAmount));
	if (!fileType)
		infile.seekg(2 * sizeof(int), ios::cur);
	infile.read((char*)(&emptyAmount), sizeof(emptyAmount));

	if ((blockAmount + emptyAmount)<blockNum + 1){
		tempBlock->blockNum = -2;
	}

	else{
		infile.read((char*)(emptyBlock), 100 * sizeof(int));
		int i = 0;
		for (i = 0; i<emptyAmount; i++){					//search the empty block
			if (blockNum == emptyBlock[i]){
				tempBlock->blockNum = -2;
				break;
			}
		}
		if (i == emptyAmount){						//not empty
			infile.seekg((sizeof(BlockInfo) + BLOCKSIZE)*(blockNum), ios::cur);
			char* tempCBlock = tempBlock->cBlock;
			infile.read((char*)tempBlock, sizeof(BlockInfo));
			tempBlock->cBlock = tempCBlock;
			infile.read((char*)tempBlock->cBlock, BLOCKSIZE);
		}
	}

	infile.close();
}

void Buffer::readEmptyBlock(string& dbName, string& name, int fileType, BlockInfo* tempBlock){
	string fileName = dbName + '_' + name;
	int blockAmount, emptyAmount;
	int emptyBlock[100];

	if (fileType)										//index file
		fileName += ".index";
	else
		fileName += ".db";

	ifstream infile(fileName, ios::binary);
	if (!infile.is_open()){
		cout << "when read:file open failed" << endl;
		return;
	}

	infile.seekg(0, ios::beg);
	infile.read((char*)(&blockAmount), sizeof(blockAmount));
	if (!fileType)
		infile.seekg(2 * sizeof(int), ios::cur);
	infile.read((char*)(&emptyAmount), sizeof(emptyAmount));
	infile.read((char*)(emptyBlock), 100 * sizeof(int));
	if (emptyAmount == 0){							//expand the space of the file
		tempBlock->blockNum = blockAmount;
		blockAmount++;
		// tempBlock->dirtyBit = 1;					//record应该会自己改
	}
	else{
		tempBlock->blockNum = emptyBlock[0];
		// tempBlock->dirtyBit = 1;
		emptyBlock[0] = emptyBlock[emptyAmount - 1];
		emptyBlock[emptyAmount - 1] = -1;
		emptyAmount--;
		blockAmount++;
	}
	infile.close();

	fstream outfile;
	outfile.open(fileName, ios::binary | ios::in | ios::out);
	if (!outfile.is_open()){
		cout << "when write read:file open failed" << endl;
		return;
	}

	outfile.seekp(0, ios::beg);
	outfile.write((char*)(&blockAmount), sizeof(blockAmount));
	if (!fileType){
		outfile.seekp(2 * sizeof(int), ios::cur);
	}
	outfile.write((char*)(&emptyAmount), sizeof(emptyAmount));
	outfile.write((char*)(emptyBlock), 100 * sizeof(int));
	outfile.seekp((sizeof(BlockInfo) + BLOCKSIZE)*(tempBlock->blockNum), ios::cur);
	outfile.write((char*)(tempBlock), sizeof(BlockInfo));
	outfile.write((char*)tempBlock->cBlock, BLOCKSIZE);

	outfile.close();
}

void Buffer::writeBlock(string& dbName, BlockInfo* tempBlock){
	string fileName = tempBlock->file->fileName;
	int fileType = tempBlock->file->type;
	tempBlock->dirtyBit = 0;
	tempBlock->next = NULL;
	tempBlock->iTime = 0;
	tempBlock->lock = 0;

	int blockAmount, recordLength, recordAmount, emptyAmount;
	int emptyBlock[100];

	if (fileType)
		fileName += ".index";
	else
		fileName += ".db";

	ifstream infile(fileName, ios::binary);
	if (!infile.is_open()){
		cout << "when write:file open failed" << endl;
		return;
	}

	infile.seekg(0, ios::beg);
	infile.read((char*)(&blockAmount), sizeof(blockAmount));
	if (!fileType){
		infile.read((char*)(&recordLength), sizeof(recordLength));
		infile.read((char*)(&recordAmount), sizeof(recordAmount));
	}
	infile.read((char*)(&emptyAmount), sizeof(emptyAmount));
	infile.read((char*)(emptyBlock), 100 * sizeof(int));

	if (!fileType){									//update the recordAmount
		infile.seekg((sizeof(BlockInfo) + BLOCKSIZE)*(tempBlock->blockNum), ios::cur);
		BlockInfo* oldBlock = new BlockInfo();
		infile.read((char*)oldBlock, sizeof(BlockInfo));

		recordAmount = (int)(recordAmount - oldBlock->charNum / recordLength + tempBlock->charNum / recordLength);
		delete oldBlock;
	}
	infile.close();

	if (tempBlock->charNum == 0){					//remember the empty block
		emptyBlock[blockAmount] = tempBlock->blockNum;
		blockAmount--;
		emptyAmount++;
	}

	fstream outfile;
	outfile.open(fileName, ios::binary | ios::in | ios::out);
	if (!outfile.is_open()){
		cout << "when write read:file open failed" << endl;
		return;
	}

	outfile.seekp(0, ios::beg);
	outfile.write((char*)(&blockAmount), sizeof(int));
	if (!fileType){
		outfile.write((char*)(&recordLength), sizeof(int));
		outfile.write((char*)(&recordAmount), sizeof(int));
	}
	outfile.write((char*)(&emptyAmount), sizeof(emptyAmount));
	outfile.write((char*)(emptyBlock), 100 * sizeof(int));
	outfile.seekp((sizeof(BlockInfo) + BLOCKSIZE)*(tempBlock->blockNum), ios::cur);
	outfile.write((char*)tempBlock, sizeof(BlockInfo));
	outfile.write((char*)tempBlock->cBlock, BLOCKSIZE);

	outfile.close();
}

void Buffer::addBlock(string& dbName, string& name, FileInfo* tempFile, BlockInfo* tempBlock){
	string fileName = dbName + "_" + name;

	if (!tempFile->type){
		fileName = fileName + ".db";
		int recordLength;
		ifstream infile(fileName, ios::binary);
		infile.seekg(sizeof(int), ios::beg);
		infile.read((char*)&recordLength, sizeof(int));
		int recordNum = tempBlock->charNum / recordLength;
		tempFile->recordAmount += recordNum;
	}

	BlockInfo* tempBlock2 = tempFile->firstBlock;
	if (tempBlock2){
		while (tempBlock2->next)
			tempBlock2 = tempBlock2->next;
		tempBlock2->next = tempBlock;
	}
	else
		tempFile->firstBlock = tempBlock;
	tempBlock->next = NULL;
	tempBlock->file = tempFile;
}

void Buffer::deleteBlock(string& dbName, string& name, BlockInfo* tempBlock){
	tempBlock->deleteBlock();
}

void Buffer::closeFile(string& dbName, string& fileName, int fileType){
	FileInfo* tempFile = fileHead->firstFile;
	FileInfo* oldFile = NULL;

	while (tempFile){
		if ((tempFile->fileName == fileName) && (tempFile->type == fileType)){
			BlockInfo* tempBlock = tempFile->firstBlock;
			BlockInfo* oldBlock = NULL;
			while (tempBlock){
				oldBlock = tempBlock;
				tempBlock = tempBlock->next;
				if (oldBlock->dirtyBit)
					writeBlock(dbName, oldBlock);
				fileHead->fileNum--;
				//delete oldBlock;		//在哪分配就在哪释放
			}
			if (oldFile)
				oldFile->next = tempFile->next;
			else
				fileHead->firstFile = tempFile->next;
			delete tempFile;
			fileHead->fileNum--;
			break;
		}
		else{
			oldFile = tempFile;
			tempFile = tempFile->next;
		}
	}
}

void Buffer::closeDatabase(string& dbName){

	FileInfo* tempFile = fileHead->firstFile;
	while (tempFile){
		closeFile(dbName, tempFile->fileName, tempFile->type);
		tempFile = fileHead->firstFile;
	}
}

bool Buffer::quitProc(string& dbName){
	closeDatabase(dbName);

	delete fileHead;

	return true;
}
