//
//  main.cpp
//  miniSQL
//
//  Created by Leon on 14/11/8.
//  Copyright (c) 2014年 ZJU. All rights reserved.
//

#include "Definition.h"
#include "Interpreter.h"
#include "Api.h"

using namespace std;

Api myApi;
CatalogManager myCatalog;
Buffer myBuffer;
Interpreter myInterpreter;
BPlusTree myIndex;

int main(int argc, const char * argv[]) {
	string filename;
	int i;
	while (cin){
		cout << "miniSQL>>";
		string s = myInterpreter.read_input();
		i = myInterpreter.run(s, filename);
		if (i == -1){
			ifstream fin;
			//cout<<"filename="<<filename<<endl;
			fin.open(filename.c_str());
			int num = 0;
			while (fin){
				s = myInterpreter.read_input(fin);
				if (s == "")
					break;
				i = myInterpreter.run(s, filename);
				if (i == -1){
					cout << "文件里面包含了execfile 语句" << endl;
					return 0;
				}
				printf("%d sentences executed \n", ++num);
			}
		}
	}
}

