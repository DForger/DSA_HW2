#include <iostream>
#include "data_io.h"
#include "interaction.h"

using namespace std;


const size_t k_get = 0;
const size_t k_clicked = 1;
const size_t k_impressed = 2;
const size_t k_profit = 3;
const size_t k_quit = 4;

int loadFile(const string &filename, std::vector<Interaction> &data){
    std::fstream fileReader;
    fileReader.open(filename,std::ios::in);
    try{
        if(!fileReader.is_open()){
            throw "unable to open file\n";
        }
    }catch(char *msg){
        std::cout<<"failed to open file\n";
        std::cout.flush();
        return -1;
    }

    std::string strLineBuffer;
//    while(std::getline(fileReader, strLineBuffer)){
//        std::istringstream buffer(strLineBuffer);
//        float fTmp;
//        std::vector<size_t> line;
//        while(buffer>>fTmp){
//            line.push_back(fTmp);
//        }

//        data.push_back(line);
//    }
}

void parseCommand(std::vector<std::vector<size_t> > &cmdList){
    while(1){
        string cmd;
        std::vector<size_t> cmdParam;
        cin>>cmd;

        if(!cmd.compare("get")){
            cmdParam.push_back(k_get);
            size_t paramCnt = 5;
            while(paramCnt > 0){
                size_t param;
                cin>>param;
                cmdParam.push_back(param);
            }

        }else if(!cmd.compare("clicked")){
            cmdParam.push_back(k_clicked);
            size_t paramCnt = 1;
            while(paramCnt > 0){
                size_t param;
                cin>>param;
                cmdParam.push_back(param);
            }
        }else if(!cmd.compare("impressed")){
            cmdParam.push_back(k_impressed);
            size_t paramCnt = 2;
            while(paramCnt > 0){
                size_t param;
                cin>>param;
                cmdParam.push_back(param);
            }
        }else if(!cmd.compare("profit")){
            cmdParam.push_back(k_profit);
            size_t paramCnt = 2;
            while(paramCnt > 0){
                size_t param;
                cin>>param;
                cmdParam.push_back(param);
            }
        }else if(!cmd.compare("quit")){
            break;
        }

        for(int i = 0; i < cmdParam.size(); ++i){
            std::cout<<cmdParam[i]<<" ";
        }
        std::cout<<std::endl;
        std::cout.flush();

        if(!cmdParam.empty()){
            cmdList.push_back(cmdParam);
        }
    }
}

int main(int argc, char *argv[], char *envp[])
{
    std::string strFileAddress(argv[1]);
    std::vector<Interaction> dataBuffer;
    dataBuffer.reserve(100000);

    std::cout <<"the input argument is "<< strFileAddress << std::endl;

    if(loadFile(strFileAddress, dataBuffer)){

        return -1;
    }


    std::vector<std::vector<size_t> > cmdList;
    parseCommand(cmdList);

    return 0;
}

