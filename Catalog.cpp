#include "Catalog.h"

bool CatalogManager::existDB(string DB_Name)
{
	const string filename = DB_Name + "_table.catalog";
	fstream file(filename, ios::in);
	if (!file)
		return false;
	file.close();

	return true;
}

bool CatalogManager::existTable(string Table_Name)
{
	if (dbName.empty())
	{
		cout << "Please USE DATABASE." << endl;
		return false;
	}
	for (int i = 0; i < tableNum; i++)
		if (tables[i].name == Table_Name)
			return true;

	return false;
}

bool  CatalogManager::existAttr(string Attr_Name, string Table_Name)
{
	if (dbName.empty())
	{
		cout << "Please USE DATABASE." << endl;
		return false;
	}
	for (int i = 0; i < tableNum; i++)
		if (tables[i].name == Table_Name)
			for (int j = 0; j < tables[i].attrNum; j++)
				if (tables[i].attributes[j].name == Attr_Name)
					return true;
}

bool  CatalogManager::existIndex(string Index_Name)
{
	if (dbName.empty())
	{
		cout << "Please USE DATABASE." << endl;
		return false;
	}
	for (int i = 0; i < indexNum; i++)
		if (indexes[i].name == Index_Name)
			return true;

	return false;
}

bool  CatalogManager::existIndex(string Table_Name, string Attr_Name)
{
	if (dbName.empty())
	{
		cout << "Please USE DATABASE." << endl;
		return false;
	}
	for (int i = 0; i < tableNum; i++)
		if (tables[i].name == Table_Name)
			for (int j = 0; j < tables[i].attrNum; j++)
				if (tables[i].attributes[j].name == Attr_Name)
					return true;

	return false;
}

bool CatalogManager::initialCatalog()
{
	string filename = dbName + "_table.catalog";
	fstream  fin(filename, ios::in);
	if (!fin)
		return false;
	fin >> tableNum;
	for (int i = 0; i < tableNum; i++)
	{//fill in the vector of tables
		TableInfo temp_table;
		fin >> temp_table.name;
		fin >> temp_table.primaryKey;
		fin >> temp_table.uniNum;
		for (int j = 0; j < temp_table.uniNum; ++j)
		{
			string tempStr;
			fin >> tempStr;
			temp_table.unique.push_back(tempStr);
		}
		fin >> temp_table.attrNum;
		//fin >> temp_table.blockNum;
		for (int j = 0; j < temp_table.attrNum; j++)
		{//fill in the vector of temp_table.attributes
			AttrInfo temp_attr;
			fin >> temp_attr.name;
			fin >> temp_attr.type;
			fin >> temp_attr.length;
			fin >> temp_attr.isPrimaryKey;
			fin >> temp_attr.indexName;
			temp_table.attributes.push_back(temp_attr);
			temp_table.totalLength += temp_attr.length;
		}
		tables.push_back(temp_table);
	}
	fin.close();
	filename = dbName + "_index.catalog";
	fin.open(filename, ios::in);
	if (!fin)
		return false;
	fin >> indexNum;
	for (int i = 0; i < indexNum; i++)
	{//fill in the vector of tables
		Index temp_index;
		fin >> temp_index.name;
		fin >> temp_index.tableName;
		fin >> temp_index.attrName;
		indexes.push_back(temp_index);
	}
	fin.close();

	return true;
}

bool CatalogManager::storeCatalog()
{
	string filename = dbName + "_table.catalog";
	if (tableNum == 0)
		return false;
	fstream  fout(filename, ios::out);
	fout << tableNum << endl;
	for (int i = 0; i < tableNum; i++)
	{
		fout << tables[i].name << " ";
		fout << tables[i].primaryKey << " ";
		fout << tables[i].uniNum << " ";
		for (int j = 0; j < tables[i].uniNum; ++j)
			fout << tables[i].unique[j] << " ";
		fout << tables[i].attrNum << " ";
		fout << tables[i].totalLength << endl;
		for (int j = 0; j < tables[i].attrNum; j++)
		{
			fout << tables[i].attributes[j].name << " ";
			fout << tables[i].attributes[j].type << " ";
			fout << tables[i].attributes[j].length << " ";
			fout << tables[i].attributes[j].isPrimaryKey << " ";
			fout << tables[i].attributes[j].indexName << endl;
		}
	}
	fout.close();
	filename = dbName + "_index.catalog";
	fout.open(filename, ios::out);
	fout << indexNum << endl;
	for (int i = 0; i < indexNum; i++)
	{
		fout << indexes[i].name << " ";
		fout << indexes[i].tableName << " ";
		fout << indexes[i].attrName << endl;
	}
	fout.close();

	return true;
}

bool CatalogManager::createDatabase(string DB_Name)
{
	if (existDB(DB_Name))
		return false;
	string filename = DB_Name + "_table.catalog";
	ofstream fout(filename, ios::out);
	fout << 0 << endl;
	fout.close();
	filename = DB_Name + "_index.catalog";
	fout.open(filename, ios::out);
	fout << 0 << endl;
	fout.close();
	initialCatalog();

	return true;
}
//emptyAmount/emptyBlock[100]:-1。
bool CatalogManager::createTable(TableInfo newTable)
{
	if (dbName.empty())
	{
		cout << "Please USE DATABASE." << endl;
		return false;
	}
	const string filename = dbName + "_" + newTable.name + ".db";
	fstream file;
	file.open(filename, ios::in|ios::binary);
	if (file)
	{
		file.close();
		return false;
	}
	file.open(filename, ios::out|ios::binary);
	int temp = 0;
	file.seekp(0, ios::beg);
	file.write((char*)&temp, sizeof(int));         //blockAmount
	file.write((char*)&newTable.totalLength,sizeof(int));   //recordLength
	file.write((char*)&temp, sizeof(int));			//recordAmount
	file.write((char*)&temp, sizeof(int));         //emptyAmount
	int emptyBlock[100];
	memset(emptyBlock, -1, sizeof(emptyBlock));
	file.write((char*)emptyBlock, 100*sizeof(int));
	file.close();
	tableNum++;
	tables.push_back(newTable);

	return true;
}

bool CatalogManager::createIndex(Index index)
{
	if (dbName.empty())
	{
		cout << "Please USE DATABASE." << endl;
		return false;
	}
	const string filename = dbName + "_" + index.tableName + "_" + index.attrName + "_" + index.name + ".index";
	fstream file;
	file.open(filename, ios::in|ios::binary);
	if (file)
	{
		file.close();
		return false;
	}
	file.open(filename, ios::out|ios::binary);
	int temp = 0;
	file.seekp(0, ios::beg);
	file.write((char*)&temp, sizeof(int));	//blockAmount
	file.write((char*)&temp, sizeof(int));  //emptyAmount
	int emptyBlock[100];
	memset(emptyBlock, -1, sizeof(emptyBlock));
	file.write((char*)emptyBlock, 100 * sizeof(int));
	file.close();
	indexNum++;
	indexes.push_back(index);
	for (int i = 0; i < tableNum; ++i)
		if (tables[i].name == index.tableName)
			for (int j = 0; j < tables[i].attrNum; j++)
				if (tables[i].attributes[j].name == index.attrName)
					tables[i].attributes[j].indexName = index.name;

	return true;
}

bool CatalogManager::dropDatabase(string DB_Name)
{
	useDatabase(DB_Name);
	string filename = DB_Name + "_table.catalog";
	if (remove(filename.c_str()) != 0)
		return false;
	filename = DB_Name + "_index.catalog";
	if (remove(filename.c_str()) != 0)
		return false;
	for (int i = 0; i < tableNum; ++i)
	{
		filename = DB_Name + "_" + tables[i].name + ".db";
		if (remove(filename.c_str()) != 0)
			return false;
	}
	for (int i = 0; i < indexNum; ++i)
	{
		filename = DB_Name + "_" + indexes[i].tableName + "_" + indexes[i].attrName + "_" + indexes[i].name + ".index";
		if (remove(filename.c_str()) != 0)
			return false;
	}
	tableNum = 0;
	indexNum = 0;
	tables.clear();
	indexes.clear();
}

bool CatalogManager::dropTable(TableInfo table)
{
	if (dbName.empty())
	{
		cout << "Please USE DATABASE." << endl;
		return false;
	}
	for (int i = 0; i < table.attrNum; i++)
		if (!table.attributes[i].indexName.empty())
			dropIndex(table.attributes[i].indexName);
	const string filename = dbName + "_" + table.name + ".db";
	if (remove(filename.c_str()) != 0)
		return false;
	for (int i = tableNum - 1; i >= 0; i--)
	{
		if (tables[i].name == table.name)
		{
			tables.erase(tables.begin() + i);
			tableNum--;
			return true;
		}
	}

	return false;
}

bool CatalogManager::dropIndex(string Index_Name)
{
	if (dbName.empty())
	{
		cout << "Please USE DATABASE." << endl;
		return false;
	}
	for (int i = indexNum - 1; i >= 0; i--)
	{
		if (indexes[i].name == Index_Name)
		{
			for (int j = 0; j < tableNum; j++)
				if (tables[j].name == indexes[i].tableName)
					for (int k = 0; k < tables[i].attrNum; k++)
						if (tables[j].attributes[k].name == indexes[i].attrName)
							tables[j].attributes[k].indexName = "";
			const string filename = dbName + "_" + indexes[i].tableName + "_" + indexes[i].attrName + "_" + Index_Name + ".index";
			if (remove(filename.c_str()) != 0)
				return false;
			indexes.erase(indexes.begin() + i);
			indexNum--;
			return true;
		}
	}

	return false;
}

bool CatalogManager::getTableInfo(string Table_Name, TableInfo& table)
{
	if (dbName.empty())
	{
		cout << "Please USE DATABASE." << endl;
		return false;
	}
	for (int i = 0; i < tableNum; i++)
	{
		if (tables[i].name == Table_Name)
		{
			table = tables[i];
			return true;
		}
	}

	return false;
}

bool CatalogManager::getIndexInfo(string Index_Name, Index& index)
{
	if (dbName.empty())
	{
		cout << "Please USE DATABASE." << endl;
		return false;
	}
	for (int i = 0; i < indexNum; i++)
	{
		if (indexes[i].name == Index_Name)
		{
			index = indexes[i];
			return true;
		}
	}

	return false;
}

bool CatalogManager::getIndexInfo(string Table_Name, string Attr_Name, Index& index)
{
	if (dbName.empty())
	{
		cout << "Please USE DATABASE." << endl;
		return false;
	}
	for (int i = 0; i < indexNum; i++)
	{
		if (indexes[i].tableName == Table_Name && indexes[i].attrName == Attr_Name)
		{
			index = indexes[i];
			return true;
		}
	}

	return false;
}

bool CatalogManager::getAllIndex(string Table_Name, vector<Index>& existIndex)
{
	if (dbName.empty())
	{
		cout << "Please USE DATABASE." << endl;
		return false;
	}
	for (int i = 0; i < indexNum; ++i)
	{
		if (indexes[i].tableName == Table_Name)
			existIndex.push_back(indexes[i]);
	}

	return (existIndex.size() != 0);
}
