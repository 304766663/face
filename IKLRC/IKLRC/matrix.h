#pragma once

#include <set>
#include <vector>

class Matrix
{
public:
	Matrix();

	Matrix(const int& m,const int& n);

	Matrix(const int& m,const int& n, const double& init);

	Matrix(const int& n, const std::vector<double>& vec);

	Matrix(const int& m, const int& n, const std::vector<double>& vec);

	Matrix(const int& n, std::stringstream* strStream);

	Matrix(const int& m, const int& n, std::stringstream* strStream);

	Matrix(const Matrix& mat);

	bool& getInverseAttr();

	std::vector<int>& getIndxVec();

	std::vector<int>& getLabelVec();

	std::vector<std::vector<double>>& getMat();

	int getRowCount();
	int getRowCount() const;

	int getColCount();
	int getColCount() const;

	double& at(const int& row, const int& col);
	const double& at(const int& row, const int& col) const;

	bool atRow(const int& row, std::unique_ptr<Matrix>& ret);
// 	bool atRow(const int& row, std::unique_ptr<Matrix>& ret) const;

	bool atCol(const int& col, std::vector<double>& colVec);	
	bool atCol(const int& col, std::vector<double>& colVec) const;

	void addIndx(const int& indx);

	void addRow(const std::vector<double>& rowVec);

	void transposeMat(std::unique_ptr<Matrix>& ret);

	bool add(const Matrix& m, std::unique_ptr<Matrix>& ret);

	bool minus(const Matrix& m, std::unique_ptr<Matrix>& ret);

	bool multiply(Matrix& m, std::unique_ptr<Matrix>& ret, 
		const bool& m1IsInverse = false, const bool& m2IsInverse = false);

	int labelCount();

	void clon(std::unique_ptr<Matrix>& ret);

	void checkZero();
	
	double sumUp();

	void addSmallDia();

	bool inverse(std::unique_ptr<Matrix>& ret);

	bool findXTX(std::unique_ptr<Matrix>& ret, int multRow = 0, int multCol = 0);
	
	bool success();

	void disp();

private:
	std::vector<std::vector<double> > m_mat;
	std::vector<int> m_indx;
	std::vector<int> m_label;
	bool m_isInverse;
};
