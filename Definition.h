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
//��ʱ�ĳ�2��5������
//֮��ĳ�5��50

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

class Record   //Record�࣬�����洢һ����¼
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
	void debug(){ //����ʱʹ��
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
	int attrNum;		//���к��е�������
						//��Interpreter��λattriNum
	int totalLength;	//ֵΪsum(attributes[i].length)
	vector<AttrInfo> attributes;
	TableInfo() :attrNum(0), totalLength(0), uniNum(0){};
	~TableInfo(){};
	void debug(){
		cout << name << " " << primaryKey << " " << attrNum << endl;
		for (int i = 0; i<attributes.size(); i++)
			attributes.at(i).debug();
	}
};

class Index 			//����catalog����
{
public:
	string name;		//Interpreter��Ϊindex_name
	string tableName;	//Interpreter��ΪTable_name
	string attrName;	//Interpreter��Ϊcolumn_name
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
	int length;			//char�ĳ���
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