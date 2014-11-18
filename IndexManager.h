/*
 * Index_Manager.h
 *
 *  Created on: 2014��10��31��
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

// ����ȡ�����Ϣ
class Index 			//����catalog����
{
public:
	string name;		//Interpreter��Ϊindex_name
	string tableName;	//Interpreter��ΪTable_name
	string attrName;	//Interpreter��Ϊcolumn_name
	int column;			//�����������Ͻ���������
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
	int columnNum;						//����ʲô?
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
/*����buffer�ĺ�������������
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
	int N;//ÿ���ڵ�ָ�����
    int leaf_least;//Ҷ�ӽڵ����ٵ�valueֵ�ĸ���
	int nonleaf_least;//��Ҷ�ӽڵ����ٵ�valueֵ�ĸ���
	const int VBIT;//ÿ���ڵ�value�ĸ�����4λ��string��ʾ
	const int BBIT;//ÿ���ڵ��к��ӵĿ����3λ��string��ʾ
	const int OBIT;//Ҷ�ӽڵ�ļ�¼�ڿ��е�ƫ������4λ��string��ʾ
	const int LBIT;//Ҷ�ӽڵ�Ŀ��+ƫ������λ��
	int searchLeaf(string dbName, IndexInfo & inform);
	void write(BlockInfo* b,string s);
	string findLinfo(string dbName,IndexInfo inform,int nodenum,int type);
	int compareInt(string int1,string int2);//��������ʾint���ַ������д�С�Ƚ�
	int compareFloat(string float1,string float2);//��������ʾfloat���ַ������д�С�Ƚ�
	int compareStr(string str1,string str2);
	int getValueNum(string snum);//����ʾvalue�������ַ���ת����int
	int getBlockNum(string sblocknum);//����ʾ��ţ��м�ڵ��У����ַ���ת����int
	int getOffset(string stableoffset);//����ʾƫ������Ҷ�ӽڵ��У����ַ���ת����int
	string intToStr(int value,int length);//��intת����length���ȵ��ַ���
	string toLength(IndexInfo inform);//���ַ���ת���ɱ�׼����
	void insertInLeaf(string database,IndexInfo &inform,int Node);
	void insertInParent(string dbName,IndexInfo inform,int N,string K1,int N1);
	void deleteEntry(string dbName,IndexInfo inform,int n,string K,int nod );
	int findPrevLeafSibling(string dbName, IndexInfo inform, int nodenum);//��ǰһ��Ҷ�ӵĿ��
	int findNextLeafSibling(string dbName, IndexInfo inform,int nodenum);//�Һ�һ��Ҷ�ӵĿ��
	int findLeftestChild (string dbName,IndexInfo  inform);
	int findFather(string dbName,IndexInfo inform, int num);//�ҵ��ڵ�ĸ��ڵ�
	void setN(int n);
	void getResult(string dbname,string name,int start,int end,IndexInfo inform,string Linfo,int type,vector<Result>& res);
	void searchOne(string dbName,IndexInfo inform,vector<Result>& res);
	void searchMany(string dbName,int type,IndexInfo inform,vector<Result>& res) ;
	void insertOne(string dbName, IndexInfo inform);
	void deleteOne(string dbName,IndexInfo inform);
public:
	BPlusTree();
	/*inform ���ṩ��blockNum,offset�����в���*/
	void searchIndex(string dbName,IndexInfo inform,Condition con,vector<Result>& res);
	/*inform ���ṩ�����в���*/
	void insertIndex(string dbName, vector<IndexInfo> inform);
	/*inform ���ṩ��blockNum,offset�����в���*/
	void deleteIndex(string dbName,vector<IndexInfo> inform);
};
#endif /* INDEX_MANAGER_H_ */
