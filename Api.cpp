#include "Api.h"

void Api::createDatabase(string dbName){
	if(myCatalog.createDatabase(dbName))
		cout << "You created a database:" << dbName << endl;
	else
		cout << "The database already exists!" << endl;
}

void Api::dropDatabase(string dbName){
	if(myCatalog.dropDatabase(dbName))
		cout << "You dropped a database:" << dbName << endl;
	else
		cout << "Drop Failed!" << endl;
}

void Api::useDatabase(string dbName){
	// DB_Name = dbName;	//DB_Name是一个全局变量
	myCatalog.useDatabase(dbName);
	cout << "You are using:" << dbName << endl;
}

bool Api::createTable(TableInfo& table){
	if(myCatalog.createTable(table))
		cout << "You created a table:" << table.name << endl;
	else
		cout << "Create Failed!" << endl;
}

bool Api::dropTable(TableInfo& table){
	if(myCatalog.dropTable(table))
		cout << "You dropped a table:" << table.name << endl;
	else
		cout << "Dropped Failed!" << endl;
}

bool Api::createIndex(Index& index){
	if(myCatalog.createIndex(index))
		cout << "You created an index: " << index.name << " on " << index.attrName << endl;
	else
		cout << "Create Failed!" << endl;
}

bool Api::dropIndex(Index& index){
	if(myCatalog.dropIndex(index))
		cout << "You dropped an index: " << index.name << " on " << index.attrName << endl;
	else
		cout << "Dropped Failed!" << endl;
}

bool Api::selectRecord(string tableName,vector<string>& columns,vector<Condition>& conds){
	myCatalog.existIndex(tableName,)
	selectRecord(tableName,columns,conds);
}

bool Api::insertRecord(string tableName,vector<string>& values){
	return RecordManager::insertRecord(tableName,values);
}

bool Api::deleteRecord(string tableName,vector<string>& values){
	return RecordManager::deleteRecord(tableName,conds);
}

bool Api::quit(){
	return quitProc()
}