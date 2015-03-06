#include "stdafx.h"

#include "matrix.h"

#include <algorithm>
#include <functional>
#include <memory>
#include <sstream>

Matrix::Matrix(const int& m, const int& n) 
	: m_mat(m, std::vector<double>(n))
{
}

Matrix::Matrix(const int& m, const int& n, const std::vector<double>& vec)
{
	int indx = 0;
	for (auto it = vec.begin();it != vec.end(); it = it + n)
	{
		if (indx ++ == m)
			return;

		m_mat.emplace_back(std::vector<double>(it, it + n));
	}	
}

Matrix::Matrix(const int& m, const int& n, std::stringstream* strStream) 
	: m_mat(m, std::vector<double>(n))
{
	for (auto matIt = m_mat.begin();matIt != m_mat.end(); ++matIt)
	{
		for (auto vecIt = matIt->begin();vecIt != matIt->end(); ++vecIt)
		{
			(*strStream) >> (*vecIt);
		}
	}
}

int Matrix::getRowCount()
{
	return m_mat.size();
}
int Matrix::getRowCount() const
{
	return m_mat.size();
}
int Matrix::getColCount()
{
	return m_mat[0].size();
}
int Matrix::getColCount() const
{
	return m_mat[0].size();
}

double& Matrix::at(const int& row, const int& col)
{
	return m_mat[row][col];
}
const double& Matrix::at(const int& row, const int& col) const
{
	return m_mat[row][col];
}

bool Matrix::atRow(const int& row, std::vector<double>& rowVec)
{
	if (row >= getRowCount())
		return false;
	
	rowVec.assign(m_mat[row].begin(), m_mat[row].end());
	return true;
}
bool Matrix::atRow(const int& row, std::vector<double>& rowVec) const
{
	if (row >= getRowCount())
		return false;

	rowVec.assign(m_mat[row].begin(), m_mat[row].end());
	return true;
}

bool Matrix::atCol(const int& col, std::vector<double>& colVec)
{
	std::vector<double> tmp;
	if (col >= getColCount())
		return false;

	for (auto it = m_mat.begin();it != m_mat.end(); ++it)
	{
		colVec.emplace_back((*it)[col]);
	}
	
	return true;
}
bool Matrix::atCol(const int& col, std::vector<double>& colVec) const
{
	if (col >= getColCount())
		return false;

	for (auto it = m_mat.begin();it != m_mat.end(); ++it)
	{
		colVec.emplace_back((*it)[col]);
	}

	return true;
}

void Matrix::transposeMat(std::unique_ptr<Matrix>& ret)
{
	int row = getRowCount();
	int col = getColCount();
	ret = std::unique_ptr<Matrix>(new Matrix(col, row));
	
	for (int i = 0;i < col; ++i)
	{
		for (int j = 0;j < row; ++j)
		{
			ret->m_mat[i][j] = m_mat[j][i];
		}
	}
}

bool Matrix::add(const Matrix& m, std::unique_ptr<Matrix>& ret)
{
	int row = getRowCount();
	int col = getColCount();
	int row2 = m.getRowCount();
	int col2 = m.getColCount();

	if(row != row2 || col != col2)
		return false;

	ret = std::unique_ptr<Matrix>(new Matrix(row, col));
	
	for(int i = 0;i < row; ++i)
	{		
		std::transform(m_mat[i].begin(), m_mat[i].end(), 
			m.m_mat[i].begin(), ret->m_mat[i].begin(), std::plus<double>());		
	}

	return true;
}

bool Matrix::minus(const Matrix& m, std::unique_ptr<Matrix>& ret)
{
	int row = getRowCount();
	int col = getColCount();
	int row2 = m.getRowCount();
	int col2 = m.getColCount();

	if(row != row2 || col != col2)
		return false;

	ret = std::unique_ptr<Matrix>(new Matrix(row, col));

	for(int i = 0;i < row; ++i)
	{		
		std::transform(m_mat[i].begin(), m_mat[i].end(), 
			m.m_mat[i].begin(), ret->m_mat[i].begin(), std::minus<double>());		
	}

	return true;
}

bool Matrix::multiply(const Matrix& m, std::unique_ptr<Matrix>& ret)
{
	int row = getRowCount();
	int col = getColCount();
	int row2 = m.getRowCount();
	int col2 = m.getColCount();

	if(col != row2)	
		return false;

	ret = std::unique_ptr<Matrix>(new Matrix(row, col2));

	for(int i = 0;i < row; ++i)
	{		
		for(int j = 0 ; j < col2; ++j)
		{
			double sum = 0;
			for(int k = 0 ;k < col; k ++)
				sum += m_mat[i][k] * m.m_mat[k][j];

			ret->m_mat[i][j] = sum;
		}	
	}

	return true;
}

bool Matrix::findXTX(std::unique_ptr<Matrix>& ret, int multRow, int multCol)
{
	ret = std::unique_ptr<Matrix>(new Matrix(multRow, multRow));
	
	for(int i = 0;i < multRow; ++i)
	{		
		for(int j = 0 ; j < multRow; ++j)
		{
			double sum = 0;
			for(int k = 0 ;k < multCol; k ++)
				sum += m_mat[i][k] * m_mat[j][k];

			ret->m_mat[i][j] = sum;
		}	
	}

	return true;
}

bool Matrix::success()
{
	return (m_mat.size() == 0? false : true);
}

void Matrix::disp()
{	
	if (m_mat.size() == 0)
		return;
	
	for (auto matIt = m_mat.begin();matIt != m_mat.end(); ++matIt)
	{
		for (auto vecIt = matIt->begin();vecIt != matIt->end(); ++vecIt)
		{
			std::cout << (*vecIt) <<" ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}