#pragma once

#include <vector>

class Matrix
{
public:
	Matrix(const int& m,const int& n);

	Matrix(const int& m, const int& n, const std::vector<double>& vec);

	Matrix(const int& m, const int& n, std::stringstream* strStream);

	int getRowCout();
	int getRowCout() const;

	int getColCount();
	int getColCount() const;

	double& at(const int& row, const int& col);
	const double& at(const int& row, const int& col) const;

	const std::vector<double>& atRow(const int& row);
	const std::vector<double>& atRow(const int& row) const;

	const std::vector<double>& atCol(const int& row);	
	const std::vector<double>& atCol(const int& row) const;

	Matrix* transposeMat();

	Matrix* add(const Matrix& m);

	Matrix* minus(const Matrix& m);

	Matrix* multiply(const Matrix& m);

	bool success();

	void disp();

private:
	std::vector<std::vector<double> > m_mat;
	std::unique_ptr<Matrix> m_ret;
};
