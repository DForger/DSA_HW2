#ifndef DATA_IO_H
#define DATA_IO_H
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

template<typename DataType>
void readText_(std::vector<std::vector<DataType> > &data, std::string filename){
    data.clear();
    {
        std::fstream fileReader;
        fileReader.open(filename,std::ios::in);

        if(!fileReader.is_open()){
            return;
        }
        std::string sLineBuffer;
        while(std::getline(fileReader, sLineBuffer)){
            std::istringstream buffer(sLineBuffer);
            float fTmp;
            std::vector<DataType> line;
            while(buffer>>fTmp){
                line.push_back(fTmp);
            }

            data.push_back(line);
        }
    }
}

#endif // DATA_IO_H

