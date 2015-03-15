#pragma once
#include <memory>

namespace TrainerSpace
{
enum InverseMethod{
	Direct,
	Increase
};
}

class Matrix;
class Trainer
{
public:
	Trainer(const std::string& filePath, const int& dimension);
	~Trainer(void);

	Matrix* getX_Train_T();
	Matrix* getX_Train_All();
	double findDistance(const Matrix* mat1, const Matrix* mat2);
	void prepareToTrain(const int& label);
	bool trainingToFindXTXInverse(const TrainerSpace::InverseMethod& inverseType, 
		std::unique_ptr<Matrix>& XTX_Inverse);
	void run();

private:
	int m_dimension_X;
	Matrix* m_Train_All;   //训练数据集
	Matrix* m_X_Train_T; //某一标签的数据集
//	std::unique_ptr<Matrix> m_Y_Train; //
	//std::unique_ptr<Matrix> m_XTX;     //对称矩阵XTX,增量形式
	//std::unique_ptr<Matrix> m_XTX_;    //XTX的逆
};

