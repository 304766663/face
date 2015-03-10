#pragma once
#include <memory>

class Matrix;
class Trainer
{
public:
	Trainer(const int& dimension, const std::string& filePath);
	~Trainer(void);

	void prepareToTrain(const int& label);
	bool training();
	void run();

private:
	int m_dimension_X;
	std::unique_ptr<Matrix> m_X_All;   //训练数据集
	std::unique_ptr<Matrix> m_X_Train; //某一标签的数据集
	std::unique_ptr<Matrix> m_XTX;     //对称矩阵XTX,增量形式
	std::unique_ptr<Matrix> m_XTX_;    //XTX的逆
};

