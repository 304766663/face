// IKLRC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "datareader.h"
#include <vector>
#include "matrix.h"
#include <map>
#include "trainer.h"

const int MAXN = 10000;
const long MAXS = 6*1024*1024;

const std::string testFolder = "E:\\test\\";
const std::string testPath = "StTestFile1.txt";

void fread_analyse()
{
	std::stringstream ss;
	ss << "3 2 7 9 6 1 5 5 3 5		3 1 5 3 7		5 6 7 8 3		1 1 8 9 2";
	Matrix mat(5,5,&ss);
	mat.disp();
	std::unique_ptr<Matrix> m;
	mat.inverse(m);
	m->disp();
 	std::unique_ptr<Matrix> ret;
 	mat.multiply(*m, ret);
 	ret->disp();
}

int _tmain(int argc, _TCHAR* argv[])
{
// 	DataReader dr;
// 	std::stringstream* it = dr.getFileData("E:\\test\\StTestFile1.txt");
// 	
// 	double i;
// 	while (!it->eof())
// 	{
// 		(*it) >> i;
// 		std::cout << i << " ";
// 	}

	fread_analyse();
	//Trainer tr(0, "");
	//tr.run();

	return 0;
}

