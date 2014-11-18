/*
 * Index_Manager.h
 *
 *  Created on: 2014年10月31日
 *      Author: Chong Xiaoya
 */

#ifndef INDEX_MANAGER_H_
#define INDEX_MANAGER_H_

#include<iostream>
#include<CString>
#include<vector>
//#include "Definition.h"
//#include "BlockInfo.h"
//#include "FileInfo.h"
#define BLOCKSIZE 4096
using namespace std;
#define LT 100
#define LE 101
#define GT 102
#define GE 103
#define EQ 104
#define INT 201
#define CHAR 202
#define FLOAT 203
struct BlockInfo;
class FileInfo{
public:
	int type;					//0 for Data File
	//1 for Index File
	string fileName;			//the name of the file
	int recordAmount;			//the number of the record in th file
	int freeNum;				//the free block number which could be used for the file
	FileInfo* next;				//the pointer points to the next file
	BlockInfo* firstBlock;		//point to the first blcok within the file

	FileInfo();
	FileInfo(int fileType, string name){
		type = fileType;
		fileName = name;
		recordAmount = 0;
		freeNum = 0;
		next = NULL;
		firstBlock = NULL;
	}
	~FileInfo();
};

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
		delete[] cBlock;
		cBlock = new char[BLOCKSIZE];
		father = -1;
		iTime = 0;
		lock = 0;
		isFull = 0;
	}
};

// 所读取块的信息
class Index 			//存在catalog里面
{
public:
	string name;		//Interpreter中为index_name
	string tableName;	//Interpreter中为Table_name
	string attrName;	//Interpreter中为column_name
	int column;			//在哪条属性上建立了索引
	Index():name(""),tableName(""){};
	~Index(){};
	void debug(){
		cout << name << " " << tableName << " " << attrName << endl;
	}
};
class IndexInfo : public Index //B+TreeNode
{
public:
	int blockNum;
	int offset;
	int type;
	int length;
	string value;
	IndexInfo() :blockNum(-1), offset(-1){};
	~IndexInfo(){};
};

class Condition
{
public:
	int op;
	int columnNum;						//这是什么?
	string columname;
	string value;
	void debug(){
		cout << columname << " " << op << " " << value << endl;
	}
};

class Result
{
public:
	int blockNum;
	vector<int> offsets;
	Result() :blockNum(-1){};
	~Result(){};
};
/*调用buffer的函数，共三个：
void deleteBlock(string dbName,string name,BlockInfo* tempBlock);
BlockInfo* getEmptyBlock(string dbName,string name);
BlockInfo* getBlock(string dbName, string name,int blockNum, int type);
*/
class BPlusTree
{
private:
	const int LENGTH_OF_INT;
	const int LENGTH_OF_FLOAT;
	int LENGTH_OF_CHAR;
	int N;//每个节点指针个数
    int leaf_least;//叶子节点最少的value值的个数
	int nonleaf_least;//非叶子节点最少的value值的个数
	const int VBIT;//每个节点value的个数用4位的string表示
	const int BBIT;//每个节点中孩子的块号用3位的string表示
	const int OBIT;//叶子节点的记录在块中的偏移量用4位的string表示
	const int LBIT;//叶子节点的块号+偏移量的位数
	int searchLeaf(string dbName, IndexInfo & inform);
	void write(BlockInfo* b,string s);
	string findLinfo(string dbName,IndexInfo inform,int nodenum,int type);
	int compareInt(string int1,string int2);//将连个表示int的字符串进行大小比较
	int compareFloat(string float1,string float2);//将两个表示float的字符串进行大小比较
	int compareStr(string str1,string str2);
	int getValueNum(string snum);//将表示value个数的字符串转化成int
	int getBlockNum(string sblocknum);//将表示块号（中间节点中）的字符串转化成int
	int getOffset(string stableoffset);//将表示偏移量（叶子节点中）的字符串转化成int
	string intToStr(int value,int length);//将int转化成length长度的字符串
	string toLength(IndexInfo inform);//将字符串转化成标准长度
	void insertInLeaf(string database,IndexInfo &inform,int Node);
	void insertInParent(string dbName,IndexInfo inform,int N,string K1,int N1);
	void deleteEntry(string dbName,IndexInfo inform,int n,string K,int nod );
	int findPrevLeafSibling(string dbName, IndexInfo inform, int nodenum);//找前一个叶子的块号
	int findNextLeafSibling(string dbName, IndexInfo inform,int nodenum);//找后一个叶子的块号
	int findLeftestChild (string dbName,IndexInfo  inform);
	int findFather(string dbName,IndexInfo inform, int num);//找到节点的父节点
	void setN(int n);
	void getResult(string dbname,string name,int start,int end,IndexInfo inform,string Linfo,int type,vector<Result>& res);
	void searchOne(string dbName,IndexInfo inform,vector<Result>& res);
	void searchMany(string dbName,int type,IndexInfo inform,vector<Result>& res) ;
	void insertOne(string dbName, IndexInfo inform);
	void deleteOne(string dbName,IndexInfo inform);
public:
	BPlusTree();
	/*inform 需提供除blockNum,offset的所有参数*/
	void searchIndex(string dbName,IndexInfo inform,Condition con,vector<Result>& res);
	/*inform 需提供除所有参数*/
	void insertIndex(string dbName, vector<IndexInfo> inform);
	/*inform 需提供除blockNum,offset的所有参数*/
	void deleteIndex(string dbName,vector<IndexInfo> inform);
};
#endif /* INDEX_MANAGER_H_ */
