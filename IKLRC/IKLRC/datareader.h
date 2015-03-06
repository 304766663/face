#pragma once

#include <memory>
#include <sstream>

class DataReader
{
public:
	DataReader();
	~DataReader(void);

	std::stringstream* getFileData(const std::string& filePath);

private:
	std::shared_ptr<std::stringstream> m_fileStrStream;
};

