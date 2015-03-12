#include "stdafx.h"

#include "matrix.h"

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <sstream>

Matrix::Matrix()
{
}

Matrix::Matrix(const int& m, const int& n) 
	: m_mat(m, std::vector<double>(n))
{
}

Matrix::Matrix(const int& m,const int& n, const double& init)
	: m_mat(m, std::vector<double>(n))
{
	int max = (m > n)? (m): (n);
	for (int i = 0; i < max; ++ i)
	{
		m_mat[i][i] = 1;
	}
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

Matrix::Matrix(const int& n, std::stringstream* strStream)
{
	double tmp;
	std::vector<double> tmpVec;
	while (!strStream->eof())
	{
		(*strStream) >> tmp;
		tmpVec.emplace_back(tmp);
	}

	std::multimap<int, std::vector<double>> tmpMap;
	for (auto it = tmpVec.begin();it != tmpVec.end(); it = it + n)
	{
		int indx = *(it + n + 1);
		tmpMap.emplace(std::make_pair(*(it + n + 1), std::vector<double>(it, it + n)));
	}

	for (auto it = tmpMap.begin();it != tmpMap.end(); ++it)
	{
		m_indx.emplace_back(it->first);
		m_mat.emplace_back(it->second);
	}
}

Matrix::Matrix(const Matrix& mat)
{
	m_mat = mat.m_mat;
	m_indx = mat.m_indx;
}

std::vector<int>& Matrix::getLabelVec()
{
	return m_indx;
}

std::vector<std::vector<double>>& Matrix::getMat()
{
	return m_mat;
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

int Matrix::labelCount()
{	
	int count = 0;
	int label = m_indx[0];
	for (auto it = m_indx.begin() + 1; it != m_indx.end(); ++it)
	{
		if (*it == label)
			continue;
		else
			count ++;
	}
}

void Matrix::clon(std::unique_ptr<Matrix>& ret)
{
	ret = std::unique_ptr<Matrix>(new Matrix());
	ret->m_mat = m_mat;
	ret->m_indx = m_indx;
}

bool Matrix::inverse(std::unique_ptr<Matrix>& ret)
{
	int n = getRowCount();
	if (n != getColCount())
		return false;

	std::vector<std::vector<double> > tmpMat = m_mat;	
	ret = std::unique_ptr<Matrix>(new Matrix(n, n, 1));

	int l = 0;
	double max = 0, tmp = 0;
	for(int k = 0; k <= n-2; ++ k)
	{
		max = tmpMat[k][k];
		l = k;
		for(int ik=k; ik <= n-1; ++ ik)//������Ԫ���Ԫ��
		{
			if(fabs(tmpMat[ik][k]) > fabs(max))//l�洢���е������Ԫ��������
			{
				l = ik;
				max = tmpMat[l][k];
			}
		}
		if(l != k)//����������ԪԪ��������l������k,����
		{
			for (int repIndx = k; repIndx < n; ++ repIndx)
			{
				if (repIndx >= k)
				{
					tmp = tmpMat[k][k];
					tmpMat[k][k] = tmpMat[l][k];
					tmpMat[l][k] = tmp;	
				}
				//ret swap
				tmp = ret->at(k, k);
				ret->at(k, k) = ret->at(l, k);
				ret->at(l, k) = tmp;
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
				ret->at(i, j) -= tmp*ret->at(k, j)
			}
		}
	}
// 	for(int k = n-1; k >= 0; -- k)
// 	{	
// 		tmp = 0;
// 		if(k != n-1)
// 		{
// 			for(int j = k+1; j <= n-1; ++ j)
// 			{
// 				tmp += m_mat[k][j]*m_mat[j];
// 			}
// 		}
// 		q[k]=(q[k]-temp)/p[k][k];
// 		if(fabs(q[k])<MIN)
// 			q[k]=0;
// 	}
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