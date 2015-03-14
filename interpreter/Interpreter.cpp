//
//  Interpreter.cpp
//  miniSQL2
//
//  Created by Leon on 14/11/8.
//  Copyright (c) 2014�� ZJU. All rights reserved.
//
//  ��ôʶ��'��"��insert���������
#include "Interpreter.h"

Interpreter::Interpreter(void)
{
}
Interpreter::~Interpreter(void)
{
}

//�ӿ���̨��ȡ�û���һ��SQL��䣬�����µ�һ�������µ�һ�п�ʼ
string Interpreter::read_input(){
	string temp;
	string result;
	bool mark = true;
	cin >> temp;
	result += temp;
	result += " ";
	while (temp.find(";") == temp.npos){
		cin >> temp;
		result += temp;
		result += " ";
	}
//	cout << "����������: " << result << endl;
	return result;

}
//���ļ���ȡ�û���SQL���
string Interpreter::read_input(ifstream& fin){
	string temp;
	string result;
	bool mark = true;
	while (temp.find(";") == temp.npos){
		fin >> temp;
		if (temp.compare("") == 0)
			return "";
		result += temp;
		result += " ";
	}
//	cout << "�ļ����룺" << result << endl;
	return result;
}

//�� �� �� �� ��* " ' �ȶ��ÿո�ֿ�
string Interpreter::process(string& SQL){
	int i;
	string result;
	for (i = 0; i<SQL.length(); i++){
		if (SQL[i] == ',')
			result += " , ";
		else if (SQL[i] == '(')
			result += " ( ";
		else if (SQL[i] == ')')
			result += " ) ";
		else if (SQL[i] == ';')
			result += " ; ";
		else if (SQL[i] == '*')
			result += " * ";
		else if (SQL[i] == '=')
			result += " = ";
        else if(SQL[i]=='\''){
            result +=" ";
        }
        else if(SQL[i]=='\"'){
            result +=" ";
        }
		else if (SQL[i] == '>'){
			if (SQL[i + 1] == '=') {
				result += " >= ";
				i++;
			}
			else{
				result += " > ";
			}
		}
		else if (SQL[i] == '<'){
			if (SQL[i + 1] == '=') {
				result += " <= ";
				i++;
			}
			else if (SQL[i + 1] == '>'){
				result += " <> ";
				i++;
			}
			else{
				result += " < ";
			}
		}
		else
			result += SQL[i];
	}
//	cout << "����ո�" << result << endl;
	return result;
}

//��ȡ�ÿո�ֿ���word
string Interpreter::getword(string& sql){
	string word;
	for (int i = 0; i<sql.length(); i++){
		if (sql[i] != ' ')
			word += sql[i];
		else{
			while (sql[i] == ' '&&i + 1<sql.length()){
				i++;
			}
			sql.erase(0, i);
			break;
		}
	}
	if (word == "")
		word = "XXX";
	//cout <<word<<endl;
	return word;
}

//����һ����䣬Ϊ�˼򻯼���������д��ʽ���Ϲ淶 �﷨������Ҫ�Ǵ����
int Interpreter::run(string SQL, string& execfilename){
	string opcode;
	string temp, s1, s2, s3;
	SQL = process(SQL);
	opcode = getword(SQL);
	if (opcode.compare("create") == 0){
		temp = getword(SQL);
		if (temp.compare("database") == 0){
			s1 = getword(SQL);
			if (s1.compare(";") == 0) {
				cout << "Expected database name" << endl;
				return 1;
			}
			temp = getword(SQL);
			if (temp.compare(";") != 0) {
				cout << "Expected ';'" << endl;
				return 1;
			}
			cout << "create database! database name: " << s1 << endl;//test
			myApi.a_createDatabase(s1);
			return 0;
		}
		if (temp.compare("table") == 0){
			TableInfo table;
			AttrInfo attr;
			string temp = getword(SQL);    //table name
			table.name = temp;
			temp = getword(SQL);
			if (temp.compare("(") != 0){
				cout << "Expected '(' after 'table'" << endl;
				return 1; //�﷨����
			}
			temp = getword(SQL);
			while (temp.compare(")") != 0){  //��û�д���������
				if (temp.compare("primary") == 0){  //primary���
					temp = getword(SQL);
					if (temp.compare("key") != 0){
						cout << "Expected 'key'" << endl;
						return 1; //primary ����û�и�key
					}
					temp = getword(SQL);
					if (temp.compare("(") != 0){
						cout << "Expected '(' after 'primary key'" << endl;
						return 1;
					}
					temp = getword(SQL);
					table.primaryKey = temp;
					for (int i = 0; i<table.attributes.size(); i++)
						if (table.attributes.at(i).name.compare(temp) == 0)
							table.attributes.at(i).isPrimaryKey = true;
					temp = getword(SQL);
					if (temp.compare(")") != 0){
						cout << "Expected ')'" << endl;
						return 1;
					}
					temp = getword(SQL);
					break;
				}
				attr.init();
				attr.name = temp;
				temp = getword(SQL);
				if (temp.compare("int") == 0){
					attr.length = 4;
					attr.type = INT;
				}
				else if (temp.compare("float") == 0){
					attr.length = 10;
					attr.type = FLOAT;
				}
				else if (temp.compare("char") == 0){ //SQL����Ԥ�ȴ���("("ǰ��ӿո�)�� char(12)��ת��� char (12);
					temp = getword(SQL);
					if (temp.compare("(") != 0){
						cout << "Do you input the number of char?" << endl;
						return 1;
					}
					temp = getword(SQL);
					attr.length = atoi(temp.c_str());
					attr.type = CHAR;
					temp = getword(SQL);
					if (temp.compare(")") != 0){
						cout << "Expected')'" << endl;
						return 1;
					}
				}
				else
					return 2; //��֧�ָ���������
				s1 = getword(SQL);
				if (s1.compare("unique") == 0){
					attr.isUnique = true;
					table.attrNum++;
					table.attributes.push_back(attr);
                    table.unique.push_back(attr.name);
					table.totalLength += attr.length;
					temp = getword(SQL);
					if (temp.compare(",") != 0){
						cout << "Expected','" << endl;
						return 1;
					}
					temp = getword(SQL);
					continue;
				}
				else{  //û��unique
					if (s1.compare(",") != 0 && s1.compare(")") != 0){
						cout << "Expected','" << endl;
						return 1;
					}
					table.attrNum++;
					table.attributes.push_back(attr);
					table.totalLength += attr.length;
					temp = getword(SQL);
					continue;
				}
			}
			temp = getword(SQL);
			if (temp.compare(";") != 0){
				cout << "Expected';'" << endl;
				return 1;
			}
			myApi.a_createTable(table);
//			table.debug(); //test
//            cout << "!" << endl;
//            for (int i = 0; i < table.unique.size(); i++) {
//                cout << table.unique[i] << endl;
//            }
//            cout << "!" << endl;
			return 0; //��ȷִ��
		}
        
		else if (temp.compare("index") == 0){
			Index index;
			s1 = getword(SQL);
			index.name = s1;
			s1 = getword(SQL);
			if (s1.compare("on") != 0){
				cout << "Expected'on'" << endl;
				return 1;//�﷨����
			}
			s1 = getword(SQL);
			index.tableName = s1;
			s1 = getword(SQL);
			if (s1.compare("(") != 0){
				cout << "Expected'('" << endl;
				return 1;
			}
			s1 = getword(SQL);
			index.attrName = s1;
			s1 = getword(SQL);
			if (s1.compare(")") != 0){
				cout << "Expected')'" << endl;
				return 1;
			}
			s1 = getword(SQL);
			if (s1.compare(";") != 0){
				cout << "Expected';'" << endl;
				return 1;
			}
			myApi.a_createIndex(index);
			index.debug();//test
			return 0; //ִ�гɹ�

		}
		else{
			cout << "We can only create database, table or index!" << endl;
			return 1;
		}
	}
	else if (opcode.compare("drop") == 0){
		temp = getword(SQL);
		if (temp.compare("database") == 0){
			s1 = getword(SQL);
			cout << "drop database! database name: " << s1 << endl;//test
			myApi.a_dropDatabase(s1);
			return 0;
		}
		if (temp.compare("table") == 0){
			s1 = getword(SQL);
			cout << "drop table! table name: " << s1 << endl;//test
			myApi.a_dropTable(s1);
			return 0;
		}
		else if (temp.compare("index") == 0){
			s1 = getword(SQL);
			myApi.a_dropIndex(s1);
			cout << "drop index! index name: " << s1 << endl;//test
			return 0;
		}
		else{
			cout << "We can only drop database, table or index!" << endl;
			return 1;
		}
	}

	else if (opcode.compare("select") == 0){
		vector<Condition> conds;
		vector<string> colomns;
		Condition cond;
		string tablename;
		temp = getword(SQL);
		if (temp.compare("*") == 0) {
			colomns.push_back(temp);
			temp = getword(SQL);
		}
		else{
			colomns.push_back(temp);
			temp = getword(SQL);

			while (temp.compare(",") == 0) {
				temp = getword(SQL);
				colomns.push_back(temp);
				temp = getword(SQL);
			}
		}
		//        if(temp.compare("*")!=0){
		//            cout<<"We can only support select * "<<endl;
		//            return 1;
		//        }
		//        temp = getword(SQL);
		if (temp.compare("from") != 0){
			cout << "Expect 'from'" << endl;
			return 1;
		}
		tablename = getword(SQL);
		temp = getword(SQL);
		if (temp.compare("where") == 0){//��where���
			s1 = getword(SQL);
			while (s1.compare(";") != 0){   //����������
				cond.columname = s1;
				s2 = getword(SQL);  //����
				if (s2.compare("=") == 0){
					cond.op = EQ;
				}
				else if (s2.compare("<>") == 0){
					cond.op = NE;
				}
				else if (s2.compare("<") == 0){
					cond.op = LT;
				}
				else if (s2.compare(">") == 0){
					cond.op = GT;
				}
				else if (s2.compare("<=") == 0){
					cond.op = LE;
				}
				else if (s2.compare(">=") == 0){
					cond.op = GE;
				}
				else{
					cout << "Not support this kind of op" << endl;
					return 1;
				}
				s3 = getword(SQL);
				/*if(s3[0]=='\''){
				cond.value = "";
				for(int i=1;i<s3.length()-1;i++){
				cond.value +=s3[i];
				}
				}
				else{ */
				cond.value = s3;
				//}
				conds.push_back(cond);
				s1 = getword(SQL);
				if (s1.compare(";") == 0)
					break;
				s1 = getword(SQL);
			}

		}
		cout << "select ";
		for (int i = 0; i < colomns.size(); i++) {
			cout << colomns[i] << " ";
		}
		cout << "from " << tablename << " where" << endl;
		for (int i = 0; i < conds.size(); i++) {
			cout << conds[i].columname << " " << conds[i].op << " " << conds[i].value << endl;
		}
		//cout <<"==" <<  conds[0].op << " " << conds.size() << endl;
		myApi.a_selectRecord(tablename, colomns, conds);//colomns, conds����Ϊ�� Ҳ���ܲ�Ϊ��
		return 0;
	}
	else if (opcode.compare("insert") == 0){
		temp = getword(SQL);
		string tablename;
        Data data;
        Record record;
//		vector<string> values;
		if (temp.compare("into") != 0){
			cout << "Expected 'into'" << endl;
			return 1;
		}
		tablename = getword(SQL);
		temp = getword(SQL);
		if (temp.compare("values") != 0){
			cout << "Expected 'values'" << endl;
			return 1;
		}
		temp = getword(SQL);
		if (temp.compare("(") != 0){
			cout << "Expected '('" << endl;
			return 1;
		}
		temp = getword(SQL);
		//        cout << temp <<endl;
		while (temp.compare(";") != 0){
			record.columns.push_back(temp);
			temp = getword(SQL);
			temp = getword(SQL);
		}
        data.records.push_back(record);
		cout << "insert into values(";
		for (int i = 0; i < data.records[0].columns.size(); i++) {
			cout << " " << data.records[0].columns[i];
		}
		cout << " )" << endl;
		myApi.a_insertRecord(tablename,data);
		return 0;
	}
	else if (opcode.compare("delete") == 0){
		temp = getword(SQL);
		Condition cond;
		string tablename;
		if (temp.compare("from") != 0){
			cout << "Expected 'from'" << endl;
			return 1;
		}
		tablename = getword(SQL);
		temp = getword(SQL);
		if (temp.compare("where") == 0){
			s1 = getword(SQL);
			s2 = getword(SQL);
			s3 = getword(SQL);
			cond.columname = s1;
			cond.value = s3;
			if (s2.compare("=") == 0){
				cond.op = EQ;
			}
			else if (s2.compare("<>") == 0){
				cond.op = NE;
			}
			else if (s2.compare("<") == 0){
				cond.op = LT;
			}
			else if (s2.compare(">") == 0){
				cond.op = GT;
			}
			else if (s2.compare("<=") == 0){
				cond.op = LE;
			}
			else if (s2.compare(">=") == 0){
				cond.op = GE;
			}
			else{
				cout << "Not support this kind of op!" << endl;
				return 1;
			}
			s1 = getword(SQL);
		}
        else if(temp.compare(";")==0){
            myApi.a_deleteRecord(tablename);
            cout << "delete from table! tablename: " << tablename << endl;
            return 0;
        }
        else{
            cout << "syntax error when delete" << endl;
            return 1;
        }

		cout << "delete from " << tablename << " where " << cond.columname << cond.op << cond.value << endl;
		//һ��������
		vector<Condition> conds;
		conds.push_back(cond);
		myApi.a_deleteRecord(tablename,conds);
		return 0;
	}

	else if (opcode.compare("quit") == 0){
		myApi.a_quit();
		cout << "quit!" << endl;
		exit(0);
	}
	else if (opcode.compare("use") == 0){
		s1 = getword(SQL);
		temp = getword(SQL);
		if (temp.compare(";") != 0) {
			cout << "Expected ';'" << endl;
			return 1;
		}
		cout << "use database! database name: " << s1 << endl;//test
		myApi.a_useDatabase(s1);
		return 0;
	}
	else if (opcode.compare("execfile") == 0){
		temp = getword(SQL);
		execfilename = temp;
		temp = getword(SQL);
		if (temp.compare(";") != 0){
			cout << "Expected ';'" << endl;
			return 1;
		}
		return -1; //��ʾ��execfile;
	}
	else {
		cout << "Not support this kind of  SQL sentence ! " << endl;
		return 0;
	}
}
