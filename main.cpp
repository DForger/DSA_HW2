#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include "data_io.h"
#include "interaction.h"
#include "retrieval_method.h"

using namespace std;


const size_t k_get = 0;
const size_t k_clicked = 1;
const size_t k_impressed = 2;
const size_t k_profit = 3;
const size_t k_quit = 4;

int loadFile(const string &filename,
             std::vector<Interaction> &vecInteractions,
             std::multimap<size_t, size_t> &mapUserID2Index,
             std::multimap<size_t, size_t> &mapAdID2Index){
    std::fstream fileReader;
    fileReader.open(filename.c_str(),std::ios::in);
    try{
        if(!fileReader.is_open()){
            throw "unable to open file\n";
        }
    }catch(char *msg){
        std::cout<<msg;
        std::cout.flush();
        return -1;
    }

    std::string strLineBuffer;

    size_t nCnt = 0;
    while(std::getline(fileReader, strLineBuffer)){
        std::istringstream buffer;
        buffer.str(strLineBuffer);
        Interaction interaction(buffer);
        vecInteractions.push_back(interaction);
        mapUserID2Index.insert(std::pair<size_t, size_t>(interaction.userID, nCnt));
        mapAdID2Index.insert(std::pair<size_t, size_t>(interaction.adID, nCnt));
        ++nCnt;
    }
    return 0;
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
                --paramCnt;
            }

        }else if(!cmd.compare("clicked")){
            cmdParam.push_back(k_clicked);
            size_t paramCnt = 1;
            while(paramCnt > 0){
                size_t param;
                cin>>param;
                cmdParam.push_back(param);
                --paramCnt;
            }
        }else if(!cmd.compare("impressed")){
            cmdParam.push_back(k_impressed);
            size_t paramCnt = 2;
            while(paramCnt > 0){
                size_t param;
                cin>>param;
                cmdParam.push_back(param);
                --paramCnt;
            }
        }else if(!cmd.compare("profit")){
            cmdParam.push_back(k_profit);
            size_t paramCnt = 2;
            while(paramCnt > 0){
                size_t param;
                cin>>param;
                cmdParam.push_back(param);
                --paramCnt;
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




void run(std::vector<std::vector<size_t> > &cmdList){

}

int main(int argc, char *argv[], char *envp[])
{
    std::string strFileAddress(argv[1]);
    std::vector<Interaction> vecTotalInteractions;
    std::multimap<size_t, size_t> mapUserID2Index;
    std::multimap<size_t, size_t> mapAdID2Index;
    vecTotalInteractions.reserve(1000000);

    std::cout <<"the input argument is "<< strFileAddress << std::endl;

    if(loadFile(strFileAddress, vecTotalInteractions, mapUserID2Index, mapAdID2Index)){
        return -1;
    }
    std::pair<size_t, size_t> clickImpressionPair;
    std::set<pair<size_t, size_t>, PairLess<size_t, size_t> > setAdIDQueryIDPair;
    std::vector<Interaction> vecRetrievalInteractions;

//    RetrievalForClickedAndImpression(vecTotalInteractions, mapUserID2Index,8232973,21353949,27,2,3,clickImpressionPair);//8232973
    RetrievalForClickedAndImpression(vecTotalInteractions, mapUserID2Index,490234,21560710,4165614,2,2,clickImpressionPair);
    RetrievalForClicked(vecTotalInteractions, mapUserID2Index, 490234, setAdIDQueryIDPair);
    RetrievalForImpressed(vecTotalInteractions, mapUserID2Index,490234, 372875,vecRetrievalInteractions);
    std::cout.flush();
    std::vector<std::vector<size_t> > cmdList;
//    parseCommand(cmdList);

    return 0;
}

