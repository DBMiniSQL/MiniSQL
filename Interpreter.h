//
//  Interpreter.h
//  miniSQL2
//
//  Created by Leon on 14/11/8.
//  Copyright (c) 2014年 ZJU. All rights reserved.
//

#ifndef __miniSQL2__Interpreter__
#define __miniSQL2__Interpreter__

#include "Definition.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdio.h>

using namespace std;

class Interpreter
{
public:
	Interpreter(void);
	~Interpreter(void);
	int run(string SQL, string& execfilename);
	string read_input();//读取用户一条记录（以；结束）
	string read_input(ifstream& fin);
	string process(string& SQL);
	string getword(string& sql);

};

#endif /* defined(__miniSQL2__Interpreter__) */
