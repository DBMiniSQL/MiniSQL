#include "Api.h"

void Api::a_createDatabase(string dbName){
	if(myCatalog.createDatabase(dbName))
		cout << "You created a database:" << dbName << endl;
	else
		cout << "The database already exists!" << endl;
}

void Api::a_dropDatabase(string dbName){
	if(myCatalog.dropDatabase(dbName))
		cout << "You dropped a database:" << dbName << endl;
	else
		cout << "Drop Failed!" << endl;
}

void Api::a_useDatabase(string dbName){
	myCatalog.useDatabase(dbName);
	cout << "You are using:" << dbName << endl;
}

void Api::a_createTable(TableInfo& table){
	Index tempIndex;
	tempIndex.initial("sys" + table.primaryKey, table.name, table.primaryKey);
	myCatalog.createIndex(tempIndex);
	for (int i = 0; i < table.unique.size();i++){
		Index tempIndex2;
		tempIndex2.initial("sys" + table.unique[i], table.name, table.unique[i]);
		myCatalog.createIndex(tempIndex2);
	}

	if(myCatalog.createTable(table))
		cout << "You created a table:" << table.name << endl;
	else
		cout << "Create Failed!" << endl;
}

void Api::a_dropTable(string tableName){
	TableInfo table;
	myCatalog.getTableInfo(tableName,table);
	if(myCatalog.dropTable(table))
		cout << "You dropped a table:" << table.name << endl;
	else
		cout << "Dropped Failed!" << endl;
}

void Api::a_createIndex(Index& index){
	if (myCatalog.createIndex(index)){
		string dbName;
		TableInfo table;
		vector<IndexInfo> indexValues;
		myCatalog.getDBName(dbName);
		myCatalog.getTableInfo(index.tableName,table);
		initialIndex(dbName,table, index, indexValues);
		myIndex.insertIndex(dbName, indexValues);
		cout << "You created an index: " << index.name << " on " << index.attrName << endl;
	}
	else
		cout << "Create Failed!" << endl;
}

void Api::a_dropIndex(string indexName){
	if(myCatalog.dropIndex(indexName))
		cout << "You dropped an index: " << indexName << endl;
	else
		cout << "Dropped Failed!" << endl;
}

//未测试

void Api::a_selectRecord(string tableName,vector<string>& columns,vector<Condition>& conds){
	Condition tempCond;
	tempCond.op = -1;
	string dbName;
	TableInfo table;

	if (!myCatalog.getDBName(dbName) || !myCatalog.getTableInfo(tableName, table))
		cout << "Select Failed!" << endl;
	else{
		for(int i=0;i < conds.size();i++){
			if(myCatalog.existIndex(tableName,conds[i].columname)){
				if(conds[i].op == EQ && tempCond.op != EQ)
					tempCond = conds[i];
				else if(conds[i].op != EQ && tempCond.op == -1)
					tempCond = conds[i];
			}
		}
		if(tempCond.op == -1)
			selectRecord(dbName,columns,table,conds);
		else{
			vector<Result> results;
			int type,length;
			int i = 0;
			for(i=0;i<table.attributes.size();i++){
				if(table.attributes[i].name == tempCond.columname){
					type = table.attributes[i].type;
					length = table.attributes[i].length;
					break;
				}
			}
			IndexInfo inform;
			myCatalog.getIndexInfo(tableName, table.attributes[i].name, inform);
			inform.initial(type, length, tempCond.value);
			myIndex.searchIndex(dbName,inform,tempCond,results);
			printSelectedRecord(dbName, columns, table, conds, results);
		}
	}
}

void Api::a_insertRecord(string tableName,Data& data){
	TableInfo table;
	vector<Result> results;
	Record record = data.records[0];
	string dbName;

	myCatalog.getDBName(dbName);
	myCatalog.getTableInfo(tableName, table);
	
	int primaryPos=0;
	vector<int> uniquePos;

	for(int i=0;i<table.attributes.size();i++){
		if(table.attributes[i].isPrimaryKey)
			primaryPos = i;
		else if(table.attributes[i].isUnique)
			uniquePos.push_back(i);
	}
	
	Condition tempCond(EQ,table.attributes[primaryPos].name,record.columns[primaryPos]);
	IndexInfo inform;
	Index index;
	vector<Condition> conds;
	vector<string> columns;
	myCatalog.getIndexInfo(tableName, table.attributes[primaryPos].name, index);
	inform.indexInit(index);
	inform.initial(table.attributes[primaryPos].type, table.attributes[primaryPos].length, tempCond.value);
	myIndex.searchIndex(dbName, inform, tempCond, results);
	
	if(results.size() != 0){
		cout <<"The record already exists!" << endl;	
		return;
	}
	else{
		int i = 0;
		for(i=0;i<uniquePos.size();i++){
			Condition tempCond2(EQ,table.attributes[uniquePos[i]].name,record.columns[uniquePos[i]]);
			IndexInfo inform2;
			inform2.initial(table.attributes[uniquePos[i]].type,table.attributes[uniquePos[i]].length,tempCond2.value);
			myCatalog.getIndexInfo(tableName, table.attributes[uniquePos[i]].name, inform2);
			myIndex.searchIndex(dbName,inform2,tempCond2,results);
			if(results.size() != 0){
				cout << "The record already exists!" << endl;
				return;
			}
		}

		vector<Index> existIndex;
		vector<IndexInfo> indexValues; 
		myCatalog.getAllIndex(tableName, existIndex);
		insertRecord(dbName, table, data, existIndex, indexValues);
		myIndex.insertIndex(dbName, indexValues);
	}
}

void Api::a_deleteRecord(string tableName){
	TableInfo table;
	if(!myCatalog.getTableInfo(tableName,table))
		cout << "Delete Failed!" << endl;
	else{
		if(!myCatalog.dropTable(table) || !myCatalog.createTable(table))
			cout << "Delete Failed!" << endl;
	}
}

void Api::a_deleteRecord(string tableName,vector<Condition>& conds){
	Condition tempCond;
	tempCond.op = -1;
	string dbName;
	TableInfo table;

	if(!myCatalog.getDBName(dbName) || !myCatalog.getTableInfo(tableName,table))
		cout << "Select Failed!" << endl;
	else{
		for(int i=0;i < conds.size();i++){
			if(myCatalog.existIndex(tableName,conds[i].columname)){
				if(conds[i].op == EQ && tempCond.op != EQ)
					tempCond = conds[i];
				else if(conds[i].op != EQ && tempCond.op == -1)
					tempCond = conds[i];
			}
		}
		if(tempCond.op == -1){
			deletRecord(dbName,table,conds);
		}
		else{
			vector<Result> results;
			int type,length;
			int i = 0;
			for(i=0;i<table.attributes.size();i++){
				if(table.attributes[i].name == tempCond.columname){
					type = table.attributes[i].type;
					length = table.attributes[i].length;
					break;
				}
			}
			IndexInfo inform;
			inform.initial(type, length, tempCond.value);
			myCatalog.getIndexInfo(tableName, table.attributes[i].name, inform);
			myIndex.searchIndex(dbName,inform,tempCond,results);
			vector<Index> existIndex;
			vector<IndexInfo> indexValues;
			myCatalog.getAllIndex(tableName, existIndex);
			deleteIndexRecord(dbName,table,conds,results,existIndex,indexValues);
			myIndex.deleteIndex(dbName, indexValues);
		}
	}
}

void Api::a_quit(){
	string dbName;
	if (!myCatalog.getDBName(dbName)){
		cout << "Exit!" << endl;
		return;
	}
	else{
		if (myBuffer.quitProc(dbName) && myCatalog.storeCatalog())
			cout << "Exit miniSQL." << endl;
		else
			cout << "Exit Failed!" << endl;
	}
	
}