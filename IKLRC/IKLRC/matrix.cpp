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

int Matrix::getRowCout()
{
	return m_mat.size();
}
int Matrix::getRowCout() const
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

const std::vector<double>& Matrix::atRow(const int& row)
{
	if (row >= getRowCout())
		return std::vector<double>();
	
	return m_mat[row];
}
const std::vector<double>& Matrix::atRow(const int& row) const
{
	if (row >= getRowCout())
		return std::vector<double>();

	return m_mat[row];
}

const std::vector<double>& Matrix::atCol(const int& col)
{
	std::vector<double> tmp;
	if (col >= getColCount())
		return tmp;

	for (auto it = m_mat.begin();it != m_mat.end(); ++it)
	{
		tmp.emplace_back((*it)[col]);
	}
	
	return tmp;
}
const std::vector<double>& Matrix::atCol(const int& col) const
{
	std::vector<double> tmp;
	if (col >= getColCount())
		return tmp;

	for (auto it = m_mat.begin();it != m_mat.end(); ++it)
	{
		tmp.emplace_back((*it)[col]);
	}

	return tmp;
}

Matrix* Matrix::transposeMat()
{

	return NULL;
}

Matrix* Matrix::add(const Matrix& m)
{
	int row = getRowCout();
	int col = getColCount();
	int row2 = m.getRowCout();
	int col2 = m.getColCount();

	if(row != row2 || col != col2)
		return &Matrix(0, 0);

	std::unique_ptr<Matrix> ret = std::unique_ptr<Matrix>(new Matrix(row, col));
	
	for(int i = 0;i < row; ++i)
	{		
		std::transform(m_mat[i].begin(), m_mat[i].end(), 
			m.m_mat[i].begin(), ret->m_mat[i].begin(), std::plus<double>());		
	}

	return ret.get();
}

Matrix* Matrix::minus(const Matrix& m)
{
	int row = getRowCout();
	int col = getColCount();
	int row2 = m.getRowCout();
	int col2 = m.getColCount();

	if(row != row2 || col != col2)
		return &Matrix(0, 0);

	m_ret = std::unique_ptr<Matrix>(new Matrix(row, col));

	for(int i = 0;i < row; ++i)
	{		
		std::transform(m_mat[i].begin(), m_mat[i].end(), 
			m.m_mat[i].begin(), m_ret->m_mat[i].begin(), std::minus<double>());		
	}

	return m_ret.get();
}

Matrix* Matrix::multiply(const Matrix& m)
{
	int row = getRowCout();
	int col = getColCount();
	int row2 = m.getRowCout();
	int col2 = m.getColCount();

	if(col != row2)	
		return &Matrix(0, 0);

	m_ret = std::unique_ptr<Matrix>(new Matrix(row, col2));

	for(int i = 0;i < row; ++i)
	{		
		for(int j = 0 ; j < col2; j ++)
		{
			double sum = 0;
			for(int k = 0 ;k < col; k ++)
				sum += m_mat[i][k] * m.m_mat[k][j];

			m_ret->m_mat[i][j] = sum;
		}	
	}

	return m_ret.get();
}

bool Matrix::success()
{
	return (m_mat.size() == 0? false : true);
}

void Matrix::disp()
{	
	for (auto matIt = m_mat.begin();matIt != m_mat.end(); ++matIt)
	{
		for (auto vecIt = matIt->begin();vecIt != matIt->end(); ++vecIt)
		{
			std::cout << (*vecIt) <<" ";
		}
		std::cout << std::endl;
	}
}