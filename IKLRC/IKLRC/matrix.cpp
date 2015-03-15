#include "stdafx.h"

#include "matrix.h"

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <sstream>

Matrix::Matrix()
	: m_isInverse(false)
{
}

Matrix::Matrix(const int& m, const int& n) 
	: m_isInverse(false)
	, m_mat(m, std::vector<double>(n))
{
}

Matrix::Matrix(const int& m, const int& n, const double& init)
	: m_isInverse(false)
	, m_mat(m, std::vector<double>(n))
{
	int max = (m > n)? (m): (n);
	for (int i = 0; i < max; ++ i)
	{
		m_mat[i][i] = 1;
	}
}

Matrix::Matrix(const int& n, const std::vector<double>& vec)
	: m_isInverse(false)
{
	for (auto it = vec.begin();it != vec.end(); it = it + n)
	{
		m_mat.emplace_back(std::vector<double>(it, it + n));
	}	
}

Matrix::Matrix(const int& m, const int& n, const std::vector<double>& vec)
	: m_isInverse(false)
{
	int indx = 0;
	for (auto it = vec.begin();it != vec.end(); it = it + n)
	{
		if (indx ++ == m)
			return;

		m_mat.emplace_back(std::vector<double>(it, it + n));
	}	
}

Matrix::Matrix(const int& n, std::stringstream* strStream)
	: m_isInverse(false)
{
	double tmp;
	std::vector<double> tmpVec;

	while (!strStream->eof())
	{
		(*strStream) >> tmp;
		tmpVec.emplace_back(tmp);
	}

	int rest = tmpVec.size() % (n + 1);
	while (rest -- > 0)
	{
		tmpVec.pop_back();
	}
	//find Index------------
	auto it = tmpVec.begin();
	std::set<int> tmpSet;
	while(it != tmpVec.end())
	{
		m_mat.emplace_back(std::vector<double>(it, it + n));
		m_indx.emplace_back(*(it + n));
		tmpSet.insert(*(it + n));
		it += n + 1;
	}
	m_label.assign(tmpSet.begin(), tmpSet.end());
}

Matrix::Matrix(const int& m, const int& n, std::stringstream* strStream) 
	: m_isInverse(false)
	, m_mat(m, std::vector<double>(n))
{
	for (auto matIt = m_mat.begin();matIt != m_mat.end(); ++matIt)
	{
		for (auto vecIt = matIt->begin();vecIt != matIt->end(); ++vecIt)
		{
			(*strStream) >> (*vecIt);
		}
	}
}

Matrix::Matrix(const Matrix& mat)
	: m_isInverse(false)
{
	m_mat = mat.m_mat;
	m_indx = mat.m_indx;
}

bool& Matrix::getInverseAttr()
{
	return m_isInverse;
}

std::vector<int>& Matrix::getIndxVec()
{
	return m_indx;
}

std::vector<int>& Matrix::getLabelVec()
{
	return m_label;
}

std::vector<std::vector<double>>& Matrix::getMat()
{
	return m_mat;
}

int Matrix::getRowCount()
{
	if (m_isInverse)
	{
		return m_mat[0].size();
	}
	else
	{
		return m_mat.size();
	}	
}
int Matrix::getRowCount() const
{
	if (m_isInverse)
	{
		return m_mat[0].size();
	}
	else
	{
		return m_mat.size();
	}	
}
int Matrix::getColCount()
{
	if (m_isInverse)
	{
		return m_mat.size();
	}
	else
	{
		return m_mat[0].size();
	}	
}
int Matrix::getColCount() const
{
	if (m_isInverse)
	{
		return m_mat.size();
	}
	else
	{
		return m_mat[0].size();
	}	
}

double& Matrix::at(const int& row, const int& col)
{
	if (m_isInverse)
	{
		return m_mat[col][row];
	}
	else
	{
		return m_mat[row][col];
	}	
}
const double& Matrix::at(const int& row, const int& col) const
{
	if (m_isInverse)
	{
		return m_mat[col][row];
	}
	else
	{
		return m_mat[row][col];
	}	
}

bool Matrix::atRow(const int& row, std::unique_ptr<Matrix>& ret)
{
	if (row >= getRowCount())
		return false;
	
	ret = std::unique_ptr<Matrix>(new Matrix(1, m_mat[row]));
//	rowVec.assign(m_mat[row].begin(), m_mat[row].end());
	return true;
}
// bool Matrix::atRow(const int& row, std::vector<double>& rowVec) const
// {
// 	if (row >= getRowCount())
// 		return false;
// 
// 	rowVec.assign(m_mat[row].begin(), m_mat[row].end());
// 	return true;
// }

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

void Matrix::addIndx(const int& indx)
{
	m_indx.push_back(indx);
}

void Matrix::addRow(const std::vector<double>& rowVec)
{
	m_mat.emplace_back(std::vector<double>(rowVec.begin(), rowVec.end()));
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

bool Matrix::multiply(Matrix& m, std::unique_ptr<Matrix>& ret, 
	const bool& m1IsInverse, const bool& m2IsInverse)
{
	bool isInverse1 = m_isInverse;
	bool isInverse2 = m.m_isInverse;
	
	m_isInverse = m1IsInverse;
	m.getInverseAttr() = m2IsInverse;

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
				sum += at(i, k) * m.at(k, j);

			ret->m_mat[i][j] = sum;
		}	
	}

	ret->checkZero();

	m_isInverse = isInverse1;
	m.getInverseAttr() = isInverse2;

	return true;
}

int Matrix::labelCount()
{		
	return m_label.size();
}

void Matrix::clon(std::unique_ptr<Matrix>& ret)
{
	ret = std::unique_ptr<Matrix>(new Matrix());
	ret->m_mat = m_mat;
	ret->m_indx = m_indx;
}

void Matrix::checkZero()
{
	for (auto matIt = m_mat.begin();matIt != m_mat.end(); ++matIt)
	{
		for (auto vecIt = matIt->begin(); vecIt != matIt->end(); ++ vecIt)
		{
			if (fabs(*vecIt) < 1e-6)
			{
				*vecIt = 0;
			}
		}
	}
}

double Matrix::sumUp()
{
	double sum = 0;
	for (auto it = m_mat[0].begin(); it != m_mat[0].end(); ++ it)
	{
		sum += (*it)*(*it);
	}
	return sum;
}

void Matrix::addSmallDia()
{
	int m = getRowCount();
	int n = getColCount();
	int max = (m > n)? (m): (n);
	for (int i = 0; i < max; ++ i)
	{
		m_mat[i][i] += 1e-6;
	}
}

bool Matrix::inverse(std::unique_ptr<Matrix>& ret)
{
	int n = getRowCount();
	if (n != getColCount())
		return false;
	//add small dia
	std::vector<std::vector<double> > tmpMat = m_mat;	
// 	for (int i = 0;i < n; ++i)
// 	{
// 		tmpMat[i][i] += 1e-6;
// 	}

	ret = std::unique_ptr<Matrix>(new Matrix(n, n, 1));

	int l = 0;
	double max = 0, tmp = 0;
	for(int k = 0; k <= n-2; ++ k)
	{
		max = tmpMat[k][k];
		l = k;
		for(int ik=k; ik <= n-1; ++ ik)//找列主元最大元素
		{
			if(fabs(tmpMat[ik][k]) - fabs(max) > 1e-6)//l存储该列的最大主元素所在行
			{
				l = ik;
				max = tmpMat[l][k];
			}
		}
		if(l != k)//如果最大列主元元素所在行l不等于k,则换行
		{
			auto tmpVec = tmpMat[k];
			tmpMat[k] = tmpMat[l];
			tmpMat[l] = tmpVec;

			//ret swap
			for (int colIndx = 0;colIndx < n; ++ colIndx)
			{
				tmp = ret->at(k, colIndx);
				ret->at(k, colIndx) = ret->at(l, colIndx);
				ret->at(l, colIndx) = tmp;
			}							
		}
		for(int i = k+1;i < n; ++ i)
		{
			tmp = tmpMat[i][k] / tmpMat[k][k];
			for(int j = 0; j < n; ++ j)
			{

				if (j >= k)
				{
					tmpMat[i][j] = tmpMat[i][j] - tmp*tmpMat[k][j];
				}
				ret->at(i, j) -= tmp*ret->at(k, j);
			}
		}
	}
	//回溯
	for (int i = n-2; i >= 0; -- i)
	{		
		for (int j = 0;j <= i; ++ j)
		{			
			tmp = tmpMat[j][i + 1] / tmpMat[i + 1][i + 1];
			for (int retIndx = 0; retIndx < n; ++ retIndx)
			{
				ret->at(j, retIndx) -= tmp*ret->at(i + 1, retIndx);
			}
			tmpMat[j][i + 1] = 0;
		}
		for (int j = 0;j < n; ++ j)
		{
			ret->at(i+1, j) /= tmpMat[i+1][i+1];	
		}
		tmpMat[i+1][i+1] = 1;
	}
	//first row 归1
	for (int j = 0;j < n; ++ j)
	{
		ret->at(0, j) /= tmpMat[0][0];	
	}
	tmpMat[0][0] = 1;

	//极小数视为0
	//ret->checkZero();
	return true;
}

bool Matrix::findXTX(std::unique_ptr<Matrix>& ret, int multRow, int multCol)
{
	if (multRow == 0 || multCol == 0)
	{
		multRow = getRowCount();
		multCol = getColCount();
	}

	if (m_isInverse)
	{
		int tmp = multCol;
		multCol = multRow;
		multRow = tmp;
	}

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