#include "StdAfx.h"
#include "trainer.h"

#include "matrix.h"
#include "datareader.h"

namespace
{
	double VecMultiply(const std::vector<double>& vec1, const std::vector<double>& vec2)
	{
		auto it1 = vec1.begin();
		auto it2 = vec2.begin();
		double sum = 0;
		for (;it1 != vec1.end(), it2 != vec2.end(); ++ it1, ++ it2)
		{
			sum += fabs(*it1 + *it2);
		}
		return sum;
	}
}

Trainer::Trainer(const std::string& filePath, const int& dimension_X)
	: m_dimension_X(dimension_X)
{
	DataReader::GetInstance().updateTrainData(filePath, dimension_X);
	m_Train_All = DataReader::GetInstance().getTrainAll();
	//std::cout << m_Train_All->labelCount();
}

Trainer::~Trainer(void)
{
}

Matrix* Trainer::getX_Train_T()
{
	return m_X_Train_T;
}

Matrix* Trainer::getX_Train_All()
{
	return m_Train_All;
}

double Trainer::findDistance(const Matrix* mat1, const Matrix* mat2)
{
// 	std::vector<double> vec1, vec2;
// 	mat1->atRow(0, vec1);
// 	mat2->atRow(0, vec2);
// 
// 	auto it1 = vec1.begin();
// 	auto it2 = vec2.begin();
// 	double distance = 0;
// 	for (;it1 != vec1.end(), it2 != vec2.end(); ++ it1, ++ it2)
// 	{
// 		distance += fabs(*it1 - *it2);
// 	}
	return 1;
}

void Trainer::prepareToTrain(const int& ith)
{
 	m_X_Train_T = DataReader::GetInstance().getTrainByIndx(ith);
}

bool Trainer::trainingToFindXTXInverse(const TrainerSpace::InverseMethod& inverseType, 
	std::unique_ptr<Matrix>& XTX_Inverse)
{
//	int trainRow = m_X_Train->getRowCount();
//	double distance = 1e7;

	//ÕÒ³öXTXµÄÄæ
	switch (inverseType)
	{
	case TrainerSpace::Direct:
		{
			std::unique_ptr<Matrix> XTX, X_Train, XTY, _b_, _y_, tmp;
			m_X_Train_T->findXTX(XTX, 0, 0);
			XTX->inverse(XTX_Inverse);

			//XTX_Inverse->multiply(*XTX, tmp);
			//XTX->multiply(*XTX_Inverse, tmp);
			//tmp->disp();
			
			//m_X_Train->transposeMat(XT);
//			m_X_Train_T->multiply(*m_Y_Train, XTY);
//			XTX_Inverse->multiply(*XTY, _b_);

//			m_X_Train_T->transposeMat(X_Train);
//			X_Train->multiply(*_b_, _y_);

//			distance = findDistance(m_Y_Train.get(), _y_.get());
			break;
		}
	case TrainerSpace::Increase:
		{
			//VecMultiply()
			break;
		}
	default:
		{
			return false;
		}		
	}

	return true;
}

void Trainer::run()
{
// 	prepareToTrain(1);
// 	std
// 		double s = trainingToFindXTXInverse(Direct);
// 	std::cout << "distance : " << s;
	return;
}
