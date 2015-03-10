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
	std::unique_ptr<Matrix> m_X_All;   //ѵ�����ݼ�
	std::unique_ptr<Matrix> m_X_Train; //ĳһ��ǩ�����ݼ�
	std::unique_ptr<Matrix> m_XTX;     //�Գƾ���XTX,������ʽ
	std::unique_ptr<Matrix> m_XTX_;    //XTX����
};

