if(blockAmount<blockNum){						//expand the space of the file
	infile.close();
	ofstream outfile(fileName,ios::binary);
	if(!outfile.is_open()){						//需要改成抛出错误
		cout << "when read&write:file open failed" << endl;
	}
	oufile.seekp(0,ios::end);
	//这里有个假设：block在文件中是连续的
	outfile.write((char*)tempBlock,sizeof(blcokInfo));
	outfile.close();
}

取出的时候已经从中删除过了，没有必要再找
	int i=0;									   //see whether the block is empty
	for(i=0;i<emptyAmount;i++){
		if(tempBlock->blockNum == emptyBlock[i]){
			int temp = emptyBlock[emptyAmount-1];
			emptyBlock[emptyAmount-1] = -1;
			emptyBlock[i] = temp;
			emptyAmount--;
			blockAmount++;
			break;
		}
	}

	if(!fileType && i == emptyAmount){				//update the recordAmount
		infile.seekg(sizeof(BlockInfo)*(tempBlock->blockNum),ios::cur);
		BlockInfo* oldBlock = new BlockInfo();
		infile.read((char*)oldBlock,sizeof(BlockInfo));

		recordAmount = recordAmount - oldBlock->charNum/recordLength + tempBlock->charNum/recordLength;
		delete oldBlock;
	}
	else if(!fileType && i != emptyAmount)
		recordAmount = recordAmount + tempBlock->charNum/recordLength;




