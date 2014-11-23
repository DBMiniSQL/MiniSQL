#ifndef __CATALOG_H__
#define __CATALOG_H__

#include "Definition.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class CatalogManager
{
private:
	string dbName;
	vector<TableInfo> tables;	//全部表
	int tableNum;				//数据库内表的数量,等于tables.size()
	vector<Index> indexes;
	int indexNum;
public:
	CatalogManager() :tableNum(0), indexNum(0){};
	CatalogManager(string name) :dbName(name)
	{
		initialCatalog();
	};
	~CatalogManager()
	{
		storeCatalog();
	};
	//在开始执行功能前将文件内信息读入内存,结束时写回硬盘
	bool initialCatalog();
	bool storeCatalog();
	bool getDBName(string& name)
	{
		if (dbName.empty())
		{
			cout << "Please USE DATABASE." << endl;
			return false;
		}
		else
			name = dbName;
		return true;
	}
	bool createDatabase(string DB_Name);
	bool createTable(TableInfo newTable);
	bool createIndex(Index index);
	bool dropDatabase(string DB_Name);
	bool dropTable(TableInfo table);
	bool dropIndex(string Index_Name);
	bool existDB(string DB_Name);
	bool existTable(string Table_Name);
	bool existAttr(string Attr_Name, string Table_Name);
	bool existIndex(string Index_Name);
	bool existIndex(string Table_Name, string Attr_Name);
	bool getTableInfo(string Table_Name, TableInfo& table);
	bool getIndexInfo(string Index_Name, Index& index);
	bool getIndexInfo(string Table_Name, string Attr_Name, Index& index);
	bool getAllIndex(string Table_Name, vector<Index>& existIndex);
	void useDatabase(string DB_Name)
	{
		if (!dbName.empty())
			storeCatalog();
		dbName = DB_Name;
		initialCatalog();
	};
};
#endif