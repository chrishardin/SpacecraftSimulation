#include "CSVParser.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

CSVParser::CSVParser(std::string filepath) : _filepath(filepath), _file(filepath, std::ios::in)
{
    if (!_file.is_open()) 
    {
        throw std::runtime_error("Error: Could not open file " + _filepath);
    }
}


std::vector<std::string> CSVParser::next()
{
    std::string line;
    std::stringstream lineStream;
    char ch;
    while (_file.get(ch) && ch != '\n')
    {
        lineStream << ch;
    }
    line = lineStream.str();
    _currentLine = split(line, ',');
    _currentLineNumber++;
    return _currentLine;
}


std::vector<std::string> CSVParser::peek()
{
    std::streampos currentPos = _file.tellg();
    std::string line;
    std::stringstream lineStream;
    char ch;
    while (_file.get(ch) && ch != '\n')
    {
        lineStream << ch;
    }
    line = lineStream.str();
    _file.seekg(currentPos);
    return split(line, ',');
}




void CSVParser::verifyHeaders(std::vector<std::string>& headers)
{
    std::streampos currentPos = _file.tellg();
    std::string headerLineStr;
    std::stringstream headerLineStream;
    char ch;
    while (_file.get(ch) && ch != '\n')
    {
        headerLineStream << ch;
    }
    headerLineStr = headerLineStream.str();
    std::vector<std::string> headerLine = split(headerLineStr, ',');

    if (headerLine.size() != headers.size())
    {
        throw std::runtime_error("Error: Incorrect number of headers. Expected " + std::to_string(headers.size()) + " but got " + std::to_string(headerLine.size()));
    }
    for (int i = 0; i < (int)headers.size(); i++)
    {
        if (headerLine[i] != headers[i])
        {
            throw std::runtime_error("Error: Incorrect header. Expected " + headers[i] + " but got " + headerLine[i]);
        }
    }
    _file.seekg(currentPos);
}


void CSVParser::verifyFile(const std::string& filename)
{
    std::streampos currentPos = _file.tellg();
    std::string firstLine;
    std::stringstream firstLineStream;
    char ch;
    while (_file.get(ch) && ch != '\n')
    {
        firstLineStream << ch;
    }
    firstLine = firstLineStream.str();
    if (firstLine != filename)
    {
        throw std::runtime_error("Error: Incorrect file. Expected " + filename + " but got " + firstLine);
    }
    _file.seekg(currentPos);
}


bool CSVParser::isValid()
{
    std::string line;
    if (std::getline(_file, line))
    {
        _currentLine = split(line, ',');
        _currentLineNumber++;
        return true;
    }
    else
    {
        return false;
    }
}

int CSVParser::convertRowToInt(const std::string & str) const
{
    try
    {
        return std::stoi(str);
    }
    catch (const std::invalid_argument&)
    {
        throw std::runtime_error("Error: Could not convert string to int: " + str);
    }
    catch (const std::out_of_range&)
    {
        throw std::runtime_error("Error: Integer value out of range: " + str);
    }
}

double CSVParser::convertRowToDouble(const std::string & str) const
{
    try
    {
        return std::stod(str);
    }
    catch (const std::invalid_argument&)
    {
        throw std::runtime_error("Error: Could not convert string to double: " + str);
    }
    catch (const std::out_of_range&)
    {
        throw std::runtime_error("Error: Double value out of range: " + str);
    }
}
