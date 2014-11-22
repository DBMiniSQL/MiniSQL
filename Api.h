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
extern IndexManager myIndex;

using namespace std;

class Api{
public:
	Api(){}
	~Api(){};
	void createDatabase(string dbName);
	void dropDatabase(string dbName);
	void useDatabase(string dbName);
	void createTable(TableInfo& table);
	void dropTable(TableInfo table);
	void createIndex(Index& index);
 	void dropIndex(string indexName);
	void selectRecord(string tableName,vector<string>& columns,vector<Condition>& conds);
	void insertRecord(string tableName,vector<string>& values);
	void deleteRecord(string tableName);
	void deleteRecord(string tableName,vector<Condition>& conds);

	void quit();
	//  void execfile(string fileName);
};

#endif