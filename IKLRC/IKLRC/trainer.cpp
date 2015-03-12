#include "StdAfx.h"
#include "trainer.h"

#include "matrix.h"
#include "datareader.h"

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

void Trainer::prepareToTrain(const int& label)
{
	m_X_Train = std::unique_ptr<Matrix>(new Matrix());
	auto labelVec = m_X_All->getLabelVec();
	auto X_AllVec = m_X_All->getMat();
	int row = m_X_All->getRowCount();
	for (int i = 0; i < row; ++i)
	{
		if (label == labelVec[i])
		{
			m_X_Train->addRow(X_AllVec[i]);
		}
	}
}

bool Trainer::training()
{
	int trainRow = m_X_Train->getRowCount();
	m_XTX = std::unique_ptr<Matrix>(new Matrix(trainRow, trainRow));
	//ÕÒ³öXTX
// 	for (int i = )
// 	{
// 	}
}

void Trainer::run()
{

}
