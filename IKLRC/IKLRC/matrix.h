#pragma once

#include <vector>

class Matrix
{
public:
	Matrix(const int& m,const int& n);

	Matrix(const int& m, const int& n, const std::vector<double>& vec);

	Matrix(const int& m, const int& n, std::stringstream* strStream);

	int getRowCount();
	int getRowCount() const;

	int getColCount();
	int getColCount() const;

	double& at(const int& row, const int& col);
	const double& at(const int& row, const int& col) const;

	bool atRow(const int& row, std::vector<double>& rowVec);
	bool atRow(const int& row, std::vector<double>& rowVec) const;

	bool atCol(const int& col, std::vector<double>& colVec);	
	bool atCol(const int& col, std::vector<double>& colVec) const;

	void transposeMat(std::unique_ptr<Matrix>& ret);

	bool add(const Matrix& m, std::unique_ptr<Matrix>& ret);

	bool minus(const Matrix& m, std::unique_ptr<Matrix>& ret);

	bool multiply(const Matrix& m, std::unique_ptr<Matrix>& ret);

	bool findXTX(std::unique_ptr<Matrix>& ret, int multRow = 0, int multCol = 0);

	bool success();

	void disp();

private:
	int m_row;
	std::vector<std::vector<double> > m_mat;
};
