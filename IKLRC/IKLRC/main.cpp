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
	ss << "1 2 3 4 5 6";
	Matrix mat(2, 3, &ss);
	
	//Matrix* temp = nullptr, * temp2 = nullptr;
	std::unique_ptr<Matrix> temp, temp2;
	mat.transposeMat(temp);
	mat.multiply(*temp, temp2);
	//temp->multiply(mat, temp2);
	mat.disp();
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

