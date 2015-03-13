#pragma once
#include <memory>

enum InverseMethod{
	Direct,
	Increase
};

class Matrix;
class Trainer
{
public:
	Trainer(const int& dimension, const std::string& filePath);
	~Trainer(void);

	double findDistance(const Matrix* mat1, const Matrix* mat2);
	void prepareToTrain(const int& label);
	double training(const InverseMethod& inverseType);
	void run();

private:
	int m_dimension_X;
	std::unique_ptr<Matrix> m_X_All;   //训练数据集
	std::unique_ptr<Matrix> m_X_Train_T; //某一标签的数据集
	std::unique_ptr<Matrix> m_Y_Train; //
	//std::unique_ptr<Matrix> m_XTX;     //对称矩阵XTX,增量形式
	//std::unique_ptr<Matrix> m_XTX_;    //XTX的逆
};

