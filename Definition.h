#ifndef __DEFINITION_H__
#define __DEFINITION_H__

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#define LT			100			//less than <
#define LE			101			//less or equal <=
#define	GT			102			//great than >
#define GE			103			//great or equal >=
#define EQ			104			//equal =
#define NE			105			//not equal <>

#define DATAFILE	0
#define INT			1
#define FLOAT		2
#define CHAR		3

#define FILLEMPTY "0"
#define BLOCKSIZE 4096			//the size of the block

#define MAX_FILE_ACTIVE	5		//limit the active files in the buffer
#define MAX_BLOCK		50 		//the max number of blocks

using namespace std;

class Result
{
public:
	int blockNum;
	vector<int> offsets;
	Result() :blockNum(-1){};
	~Result(){};
};

class Record   //Record类，用来存储一条记录
{
public:
	vector<string> columns;
	int blockNum;
	int offset;
};

class AttrInfo
{
public:
	string name;
	int type;
	int length;
	bool isPrimeryKey;
	bool isUnique;
	string indexName;
	AttrInfo() :isPrimeryKey(false), isUnique(false){};
	AttrInfo(string a_name, int a_type, int a_length, bool isP, bool isU) :name(a_name), type(a_type), length(a_length), isPrimeryKey(isP), isUnique(isU){};
	void init(){
		name = "";
		type = 0;
		length = 0;
		isPrimeryKey = false;
		isUnique = false;
	}
	void debug(){ //调试时使用
		cout << "name=" << name << "  type=" << type << "  length=" << length << "  isPrimeryKey=" << isPrimeryKey << "  isUnique=" << isUnique << endl;
	}
};

class TableInfo
{
public:
	string name;
	string primaryKey;
	int attrNum;		//表中含有的属性数
						//在Interpreter中位attriNum
	int totalLength;	//值为sum(attributes[i].length)
	vector<AttrInfo> attributes;
	TableInfo() :attrNum(0), totalLength(0){};
	~TableInfo(){};
	void debug(){
		cout << name << " " << primaryKey << " " << attrNum << endl;
		for (int i = 0; i<attributes.size(); i++)
			attributes.at(i).debug();
	}
};

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

class singleIndex
{
	vector<IndexInfo> values;
};

class multiIndex
{
	vector<singleIndex> indexes;
};

class Data 	//???
{
public:
	vector<Record> records;
	vector<int> location;
};

//stants for less than, less equal, greater than, greater equal, equal, not equal respectivly
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

#endif
