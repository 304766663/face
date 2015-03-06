#include "StdAfx.h"
#include "datareader.h"

#include <vector>

namespace
{
const long MAXLEN = 10 * 1024 * 1024;// 10M
}

DataReader::DataReader()
{
}

DataReader::~DataReader(void)
{
}

std::stringstream* DataReader::getFileData(const std::string& filePath)
{
	std::FILE* f = std::fopen(filePath.c_str(), "r");

	std::vector<char> buf(MAXLEN); 
	int size = std::fread(&buf[0], sizeof buf[0], buf.size(), f);

	buf.resize(size);

	m_fileStrStream = std::make_shared<std::stringstream>(&buf[0]);

	return m_fileStrStream.get();
}
