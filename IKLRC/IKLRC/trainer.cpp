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

Trainer::Trainer(const int& dimension_X, const std::string& filePath)
	: m_dimension_X(dimension_X)
{
	DataReader dr;
	std::stringstream* ss = dr.getFileData(filePath);

	if (ss != nullptr)
	{//get X_All
		m_X_All = std::unique_ptr<Matrix>(new Matrix(m_dimension_X, ss));
	}
}

Trainer::~Trainer(void)
{
}

double Trainer::findDistance(const Matrix* mat1, const Matrix* mat2)
{
	std::vector<double> vec1, vec2;
	mat1->atRow(0, vec1);
	mat2->atRow(0, vec2);

	auto it1 = vec1.begin();
	auto it2 = vec2.begin();
	double distance = 0;
	for (;it1 != vec1.end(), it2 != vec2.end(); ++ it1, ++ it2)
	{
		distance += fabs(*it1 - *it2);
	}
	return distance;
}

void Trainer::prepareToTrain(const int& label)
{
	m_X_Train_T = std::unique_ptr<Matrix>(new Matrix());
	auto labelVec = m_X_All->getLabelVec();
	auto X_AllVec = m_X_All->getMat();
	int row = m_X_All->getRowCount();
	for (int i = 0; i < row; ++i)
	{
		if (label == labelVec[i])
		{
			m_X_Train_T->addRow(X_AllVec[i]);
		}
	}
	//m_Y_Train =
}

double Trainer::training(const InverseMethod& inverseType)
{
//	int trainRow = m_X_Train->getRowCount();
	double distance = 1e7;

	//ÕÒ³öXTXµÄÄæ
	switch (inverseType)
	{
	case Direct:
		{
			std::unique_ptr<Matrix> XTX, XTX_Inverse, X_Train, XTY, _b_, _y_, tmp;
			m_X_Train_T->findXTX(XTX, 0, 0);
			XTX->inverse(XTX_Inverse);

			XTX_Inverse->multiply(*XTX, tmp);
			//XTX->multiply(*XTX_Inverse, tmp);
			tmp->disp();
			
			//m_X_Train->transposeMat(XT);
			m_X_Train_T->multiply(*m_Y_Train, XTY);
			XTX_Inverse->multiply(*XTY, _b_);

			m_X_Train_T->transposeMat(X_Train);
			X_Train->multiply(*_b_, _y_);

			distance = findDistance(m_Y_Train.get(), _y_.get());
			break;
		}
	case Increase:
		{
			//VecMultiply()
			break;
		}
	default:
		break;
	}

	return distance;
}

void Trainer::run()
{
	std::stringstream ss, yy;
	ss << "1 2 3 1 1 1 1 1 1 3 5 1 2 1 2 1 3 5 1 1";
	yy << "1 2 2";
	m_X_Train_T = std::unique_ptr<Matrix>(new Matrix(3, &ss));
	m_Y_Train = std::unique_ptr<Matrix>(new Matrix(3,1, &yy));
	double s = training(Direct);
	return;
}
