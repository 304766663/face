#pragma once

#include <memory>

class Matrix;
class DataReader
{
public:
	static DataReader& GetInstance();
	bool updateTrainData(const std::string& filePath, const int& Dim);
	bool updateTestData(const std::string& filePath, const int& Dim);
	Matrix* getTestAll();
	Matrix* getTestByIndx(const int& ith);
	Matrix* getTrainAll();
	Matrix* getTrainByIndx(const int& ith);

private:
	DataReader(void);
	~DataReader(void);
	DataReader(const DataReader&);
	DataReader& operator = (const DataReader&);	
	std::unique_ptr<Matrix> m_Train_All;
	std::unique_ptr<Matrix> m_Test_All;
	std::unique_ptr<Matrix> m_Train_ith;
	std::unique_ptr<Matrix> m_Test_ith;
};
