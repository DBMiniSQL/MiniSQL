#ifndef __DEFINITION_H__
#define __DEFINITION_H__
#define LT			100			//less than <
#define LE			101			//less or equal <=
#define	GT			102			//great than >
#define GE			103			//great or equal >=
#define EQ			104			//equal =
#define NE			105			//not equal <>

#define DATAFILE	0
#define INDEXFILE	1
#define INT			201
#define FLOAT		202
#define CHAR		203

#define FILLEMPTY "0"
#define BLOCKSIZE 4096			//the size of the block

#define MAX_FILE_ACTIVE	2		//limit the active files in the buffer
#define MAX_BLOCK		5 		//the max number of blocks
//暂时改成2和5来测试
//之后改成5和50

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

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

class Data
{
public:
	vector<Record> records;
};

class AttrInfo
{
public:
	string name;
	int type;
	int length;
	bool isPrimaryKey;
	bool isUnique;
	string indexName;
	AttrInfo() :isPrimaryKey(false), isUnique(false){};
	AttrInfo(string a_name, int a_type, int a_length, bool isP, bool isU) :name(a_name), type(a_type), length(a_length), isPrimaryKey(isP), isUnique(isU){};
	void init(){
		name = "";
		type = 0;
		length = 0;
		isPrimaryKey = false;
		isUnique = false;
	}
	void debug(){ //调试时使用
		cout << "name=" << name << "  type=" << type << "  length=" << length << "  isPrimaryKey=" << isPrimaryKey << "  isUnique=" << isUnique << endl;
	}
};

class TableInfo
{
public:
	string name;
	string primaryKey;
	int uniNum;			//uniNum = unique.size()
	vector<string> unique;
	int attrNum;		//表中含有的属性数
						//在Interpreter中位attriNum
	int totalLength;	//值为sum(attributes[i].length)
	vector<AttrInfo> attributes;
	TableInfo() :attrNum(0), totalLength(0), uniNum(0){};
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
	Index():name(""),tableName(""){};
	~Index(){};
	void initial(string i_name, string i_tableName, string i_attrName){
		name = i_name;
		tableName = i_tableName;
		attrName = i_attrName;
	}
	void debug(){
		cout << name << " " << tableName << " " << attrName << endl;
	}
};

class IndexInfo : public Index //B+TreeNode
{
public:
	int blockNum;
	int offset;
	int type;			//int or char or floats
	int length;			//char的长度
	string value;		//
	IndexInfo() :blockNum(-1), offset(-1){};
	IndexInfo(int i_type,int i_length,string i_value):blockNum(-1),offset(-1),type(i_type),length(i_length),value(i_value){};
	~IndexInfo(){};
	void initial(int i_type, int i_length, string i_value) {
		blockNum = -1;
		offset = -1;
		type = i_type;
		length = i_length;
		value = i_value;
	}
	void indexInit(Index& index){
		name = index.name;
		tableName = index.tableName;
		attrName = index.attrName;
	}
};

//stants for less than, less equal, greater than, greater equal, equal, not equal respectivly
class Condition
{
public:
	int op;
	string columname;
	string value;
	Condition(){};
	Condition(int c_op,string c_name,string c_value):op(c_op),columname(c_name),value(c_value){};
	~Condition(){};
	void debug(){
		cout << columname << " " << op << " " << value << endl;
	}
};

#endif