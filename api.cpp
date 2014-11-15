#include <api.h>

bool API::createDatabase(string dbName){
	return CatalogManager::createDatabase(dbName);
}

bool dropDatabase(string dbName){
	return CatalogManager::dropDatabase(dbName);
}

bool useDatabase(string dbName){
	// DB_Name = dbName;	//DB_Name是一个全局变量
	return CatalogManager::useDatabase(dbName)
}

bool API::createTable(Table table){
	return CatalogManager::createTable(table);
}

bool API::dropTabel(Table table){
	return CatalogManager::dropTable(table);
}

bool API::createIndex(Index index){
	return CatalogManager::createIndex(index);
}

bool API::dropIndex(Index index){
	return CatalogManager::dropIndex(index);
}

bool API::selectRecord(string tableName,vector<string>& columns,vector<Condition>& conds){
	return RecordManager::selectRecord(tableName,columns,conds);
}

bool API::insertRecord(string tableName,vector<string>& values){
	return RecordManager::insertRecord(tableName,values);
}

bool API::deleteRecord(string tableName,vector<Condition>& conds){
	return RecordManager::deleteRecord(tableName,conds);
}

bool API::quit(){
	return quitProc()
}

// bool API::execfile(string fileName){
// }