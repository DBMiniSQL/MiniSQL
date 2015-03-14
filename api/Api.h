/***********************************
**Tips
**1.功能里面还应该有根据判断语义是否正确的操作
**
***********************************/

#if !defined(_API_H_)
#define _API_H_

#include "Definition.h"
#include "Catalog.h"
#include "Record.h"
#include "IndexManager.h"

extern CatalogManager myCatalog;
extern BPlusTree myIndex;

using namespace std;

class Api{
public:
	Api(){}
	~Api(){};
	void a_createDatabase(string dbName);
	void a_dropDatabase(string dbName);
	void a_useDatabase(string dbName);
	void a_createTable(TableInfo& table);
	void a_dropTable(string tableName);
	void a_createIndex(Index& index);
 	void a_dropIndex(string indexName);
	void a_selectRecord(string tableName,vector<string>& columns,vector<Condition>& conds);
	void a_insertRecord(string tableName, Data& data);
	void a_deleteRecord(string tableName);
	void a_deleteRecord(string tableName,vector<Condition>& conds);

	void a_quit();
	//  void execfile(string fileName);
};

#endif