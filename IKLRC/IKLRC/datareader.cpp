#include "StdAfx.h"
#include "datareader.h"

#include <sstream>
#include <vector>

#include "matrix.h"

namespace
{
const long MAXLEN = 10 * 1024 * 1024;// 10M
}

DataReader::DataReader(void)
{
}

DataReader::~DataReader(void)
{
}

DataReader& DataReader::GetInstance()  
{
	static DataReader instance;
	return instance;
}

bool DataReader::updateTrainData(const std::string& filePath, const int& Dim)
{
	std::FILE* f = std::fopen(filePath.c_str(), "r");
	if (!f)
	{
		std::cout << "Not such file :" << filePath << std::endl;
		return false;
	}
	std::vector<char> buf(MAXLEN); 
	int size = std::fread(&buf[0], sizeof buf[0], buf.size(), f);

	buf.resize(size);

	std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>(&buf[0]);

	m_Train_All = std::unique_ptr<Matrix>(new Matrix(Dim, ss.get()));

	return true;
}
bool DataReader::updateTestData(const std::string& filePath, const int& Dim)
{
	std::FILE* f = std::fopen(filePath.c_str(), "r");
	if (!f)
	{
		std::cout << "Not such file :" << filePath << std::endl;
		return false;
	}
	std::vector<char> buf(MAXLEN); 
	int size = std::fread(&buf[0], sizeof buf[0], buf.size(), f);

	buf.resize(size);

	std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>(&buf[0]);

	m_Test_All = std::unique_ptr<Matrix>(new Matrix(Dim, ss.get()));

	return true;
}

Matrix* DataReader::getTestAll()
{
	return m_Test_All.get();
}

Matrix* DataReader::getTestByIndx(const int& ith)
{
	m_Test_ith = std::unique_ptr<Matrix>(new Matrix());

	auto labelSet = m_Test_All->getLabelVec();
	int label = labelSet[ith];
	m_Test_ith->addIndx(label);

	int n = m_Test_All->getRowCount();
	auto indxVec = m_Test_All->getIndxVec();
	auto test_All_Mat = m_Test_All->getMat();
	for (int i = 0; i < n; ++i)
	{
		if (indxVec[i] == label)
		{
			m_Test_ith->addRow(test_All_Mat[i]);			
		}
	}	
	return m_Test_ith.get();
}

Matrix* DataReader::getTrainByIndx(const int& ith)
{
	m_Train_ith = std::unique_ptr<Matrix>(new Matrix());

	auto labelSet = m_Train_All->getLabelVec();
	int label = labelSet[ith];
	m_Train_ith->addIndx(label);

	int n = m_Train_All->getRowCount();
	auto indxVec = m_Train_All->getIndxVec();
	auto test_All_Mat = m_Train_All->getMat();
	for (int i = 0; i < n; ++i)
	{
		if (indxVec[i] == label)
		{
			m_Train_ith->addRow(test_All_Mat[i]);			
		}
	}	
	return m_Train_ith.get();
}

Matrix* DataReader::getTrainAll()
{
	return m_Train_All.get();
}