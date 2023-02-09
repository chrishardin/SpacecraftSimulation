#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

class CSVParser 
{
    public:
        CSVParser(std::string filepath);

        std::vector<std::string> next();
        std::vector<std::string> peek();
        void verifyHeaders(std::vector<std::string>& headers);
        void verifyFile(const std::string& filename);
        bool isValid();
        std::vector<std::string> getRow() const { return _currentLine; }

        int convertRowToInt(const std::string& str) const;
        double convertRowToDouble(const std::string& str) const;


    private:
        std::ifstream _file;
        std::string _filepath;
        std::vector<std::string> _currentLine;
        int _currentLineNumber;

        std::vector<std::string> split(std::string str, char delimiter) 
        {
            std::vector<std::string> values;
            std::stringstream ss(str);
            std::string value;
            while (std::getline(ss, value, delimiter)) { values.push_back(value); }
            return values;
        }
};

#endif // CSVPARSER_H