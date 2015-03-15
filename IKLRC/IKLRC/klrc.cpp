#include "StdAfx.h"
#include "klrc.h"

#include <ctime>
#include <sstream>

#include "matrix.h"
#include "trainer.h"
#include "datareader.h"

namespace
{
	double FindDistance(Matrix* mat1, Matrix* mat2)
	{
		auto& m1 = mat1->getMat();
		auto& m2 = mat2->getMat();

		if (m1.size() == 0 || m2.size() == 0 || m1.size() != m2.size())
		{
			std::cout << "find distance failed !" << std::endl;
			return -1;
		}

		double sum = 0;
		int n = m1.size();
		for (int i = 0;i < n; ++i)
		{
			sum += (m1[i][0] - m2[i][0]) * (m1[i][0] - m2[i][0]);
		}
		return sum;
	}
}

KLRC::KLRC(void)
	: m_Test_All(nullptr)
	, m_trainer(nullptr)
{
}


KLRC::~KLRC(void)
{
}

void KLRC::prepareToTest(const int& label)
{
	auto labelVec = m_Test_All->getIndxVec();
	auto X_All_Mat = m_Test_All->getMat();
	int row = m_Test_All->getRowCount();
	for (int i = 0; i < row; ++i)
	{
		if (label == labelVec[i])
		{
			m_Test_All->addRow(X_All_Mat[i]);
		}
	}
}

int KLRC::figureLabel( Matrix* Y )
{
	double distance = 1e7;
	int label = -1;
	Matrix* X_Train_All = m_trainer->getX_Train_All();
	int n = X_Train_All->labelCount();
	for (int i = 0; i < n; ++ i)
	{	
		//find XTX_Inverse
		std::unique_ptr<Matrix> XTX_Inverse;
		m_trainer->prepareToTrain(i);
		m_trainer->trainingToFindXTXInverse(TrainerSpace::Direct, XTX_Inverse);
		//find XTY
		Matrix* trainData = nullptr;
		std::unique_ptr<Matrix> XTY;
		trainData = DataReader::GetInstance().getTrainByIndx(i);
		trainData->multiply(*Y, XTY);
		//find b
		std::unique_ptr<Matrix> b;
		XTX_Inverse->multiply(*XTY, b);
		//find predY
		std::unique_ptr<Matrix> predY;
		trainData->multiply(*b, predY, true, false);
		//find distance
		double newDistance = FindDistance(Y, predY.get());
		if (newDistance < distance)
		{
			distance = newDistance;
			label = X_Train_All->getLabelVec()[i];
		}
	}
	return label;
}

void KLRC::readTestDatas(const std::string& filePath, const int& dimension)
{
	DataReader::GetInstance().updateTestData(filePath, dimension);
	m_Test_All = DataReader::GetInstance().getTestAll();
}

void KLRC::run()
{
	std::string test = "D:\\小桌面\\2014.6\\项目\\test\\Result\\ORL_32x32\\StTestFile", //"D:\\StTestFile", 
		train = "D:\\StTrainFile", end = ".txt";
	int Dim = 1024;
	for (int i = 1;i <= 10; ++i)
	{
		std::stringstream trainFile, testFile;
		trainFile << train << i << end;
		testFile << test << i << end;
		//trainData
		m_trainer = std::unique_ptr<Trainer>(new Trainer(trainFile.str(), Dim));
		//testData
		readTestDatas(testFile.str(), Dim);
		//right rate
		int right = 0;
		//time
		//std::time_t begin = std::time(nullptr);
		std::time_t begin, end;
		//foreach testData, find distance
		int labelCount = m_Test_All->labelCount();
		for (int j = 0;j < labelCount; ++ j)
		{			
			Matrix* testDatas = DataReader::GetInstance().getTestByIndx(j);
			int n = testDatas->getRowCount();
			for (int k = 0;k < n; ++ k)
			{
				begin = std::clock();
				std::cout << "start to test :" << j << "-" << k << std::endl;
				std::unique_ptr<Matrix> testData;
				testDatas->atRow(k, testData);
				std::cout << "time : " << (double)(std::clock() - begin)/CLOCKS_PER_SEC <<std::endl;
				begin = std::clock();
				if (figureLabel(testData.get()) == testDatas->getIndxVec()[k])
				{
					++ right;
				}
				std::cout << "time : " << (double)(std::clock() - begin)/CLOCKS_PER_SEC <<std::endl;
				begin = std::clock();
				std::cout << "test done" << std::endl;				
			}
		}
		std::unique_ptr<Matrix> XTX_Inverse;

	}
}