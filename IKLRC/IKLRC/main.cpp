// IKLRC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "datareader.h"
#include <vector>
#include "matrix.h"

const int MAXN = 10000;
const long MAXS = 6*1024*1024;

const std::string testFolder = "E:\\test\\";
const std::string testPath = "StTestFile1.txt";

void fread_analyse()
{
	std::stringstream ss;
	ss << "1 2 3 4 5 6 7 8 9 9 9 9";
	Matrix mat(3, 4, &ss);
	
	Matrix* temp = mat.add(mat);
	Matrix* temp2 = mat.minus(mat);
	temp->disp();
	temp2->disp();
	
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
	return 0;
}

