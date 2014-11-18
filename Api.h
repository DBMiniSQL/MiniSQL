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

class Api{
public:
	Api(){}
	virtual ~Api()
	static bool createDatabase(string dbName);
	static bool dropDatabase(string dbName);
	static bool useDatabase(string dbName);
	static bool createTable(TableInfo& table);
	static bool dropTable(TableInfo& table);
	static bool createIndex(Index& index);
	static bool dropIndex(Index& index);
	static bool selectRecord(string tableName,vector<string>& columns,vector<Condition>& conds);
	static bool insertRecord(string tableName,vector<string>& values);
	static bool deleteRecord(string tableName);
	static bool quit();
	// static void execfile(string fileName);
};

#endif