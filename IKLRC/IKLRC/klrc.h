#pragma once

#include <memory>

class Matrix;
class Trainer;
class KLRC
{
public:
	KLRC(void);
	~KLRC(void);
	
	void readTestDatas(const std::string& filePath, const int& dimension);
	void run();

private:
	void prepareToTest(const int& label);
	int figureLabel(Matrix* Y);
	//std::unique_ptr<Matrix> m_Test_All;
	Matrix* m_Test_All;
	std::unique_ptr<Trainer> m_trainer;
};

