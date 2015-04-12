#include <iostream>
#include <stdio.h>
#include <map>
#include <set>
#include <utility>
#include <algorithm>

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include "data_io.h"
#include "interaction.h"
#include "retrieval_method.h"

using namespace std;


const size_t k_get = 0;
const size_t k_clicked = 1;
const size_t k_impressed = 2;
const size_t k_profit = 3;
const size_t k_quit = 4;
const size_t k_totalLineNum = 149639105;
//int loadFile(const string &filename,
//             std::vector<Interaction> &vecInteractions,
//             std::multimap<size_t, size_t> &mapUserID2Index,
//             std::multimap<size_t, size_t> &mapAdID2Index){
//    std::fstream fileReader;
//    fileReader.open(filename.c_str(),std::ios::in);
//    try{
//        if(!fileReader.is_open()){
//            throw "unable to open file\n";
//        }
//    }catch(char *msg){
//        std::cout<<msg;
//        std::cout.flush();
//        return -1;
//    }

//    std::string strLineBuffer;

//    size_t nCnt = 0;
//    while(std::getline(fileReader, strLineBuffer)){
//        std::istringstream buffer;
//        buffer.str(strLineBuffer);
//        Interaction interaction(buffer);
//        vecInteractions.push_back(interaction);
//        mapUserID2Index.insert(std::pair<size_t, size_t>(interaction.userID, nCnt));
//        mapAdID2Index.insert(std::pair<size_t, size_t>(interaction.adID, nCnt));
//        ++nCnt;
//    }
//    return 0;
//}

int loadFile(const string &filename,
             std::vector<Interaction> &vecInteractions,
             std::multimap<size_t, size_t> &mapUserID2Index,
             std::multimap<size_t, size_t> &mapAdID2Index){

    time_t t1, t2;
    t1 = time(NULL);
    std::cout<<"start loading file\n";
    std::cout.flush();

    std::fstream fileReader;
    fileReader.open(filename.c_str(),std::ios::in);
    //std::cout.sync_with_stdio(false);
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
        if(nCnt%1000000 == 0){
            t2 = time(NULL);
            std::cout << (double)nCnt/k_totalLineNum << "% lines loaded. used time " << t2-t1 << "sec \n";
            std::cout.flush();
        }
    }
     std::cout.sync_with_stdio(true);

    t2 = time(NULL);
    std::cout<<"loading compeleted, used "<< t2-t1 << "sec \n";
    return 0;
}

int readFile(const string &filename,
                 std::vector<Interaction> &vecInteractions,
                 std::multimap<size_t, size_t> &mapUserID2Index,
                 std::multimap<size_t, size_t> &mapAdID2Index){
    //file descriptor
    int fd;
    //file info
    struct stat s;
    int status;
    size_t size;

    //the memory-mapped thing itself
    void *mapped;
    int i;

    time_t t1, t2;
    t1 = time(NULL);
    std::cout<<"start loading file\n";
    std::cout.flush();

    //open file for reading
    try{
        fd = open(filename.c_str(), O_RDONLY);
        if(fd < 0){
            throw("failed to open file\n");
        }
    }catch(const char* msg){
        std::cout<<msg<<std::endl;
        std::cout.flush();
        return -1;
    }
    status = fstat(fd, & s);
    size = s.st_size;

    //memory-map the file
    try{
        mapped = mmap(0, size, PROT_READ, MAP_PRIVATE| MAP_POPULATE, fd, 0);
        if(mapped == MAP_FAILED){
            throw("failed to map file\n");
        }
    }catch(const char* msg){
        std::cout<<msg<<std::endl;
        std::cout.flush();
        return -1;
    }

    char *ptr = (char*)mapped;
    size_t nCnt = 0;
    size_t nChrCnt = 0;
    while(1){
        int nEndPos = 0;
        while((nChrCnt < size) && (ptr[nEndPos] != '\n')){
            ++nEndPos;
            ++nChrCnt;
        }

        Interaction interaction(ptr, ptr+nEndPos);
        vecInteractions.push_back(interaction);
        mapUserID2Index.insert(std::pair<size_t, size_t>(interaction.userID, nCnt));
        mapAdID2Index.insert(std::pair<size_t, size_t>(interaction.adID, nCnt));
        ++nCnt;
        if(nCnt%1000000 == 0){
            t2 = time(NULL);
            std::cout << (double)nCnt*100/k_totalLineNum << "% loaded. used time " << t2-t1 << "sec \n";
            std::cout.flush();
        }

        ++nEndPos;
        ++nChrCnt;

        if(nChrCnt >= size){
            break;
        }
        ptr = ptr + nEndPos;
    }

    t2 = time(NULL);
    std::cout<<"loading compeleted, used "<< t2-t1 << "sec \n";
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
    vecTotalInteractions.reserve(50000000);

    std::cout <<"the input argument is "<< strFileAddress << std::endl;

    if(readFile(strFileAddress, vecTotalInteractions, mapUserID2Index, mapAdID2Index)){
        return -1;
    }

    std::pair<size_t, size_t> clickImpressionPair;
    std::set<pair<size_t, size_t>, PairLess<size_t, size_t> > setAdIDQueryIDPair;
    std::vector<Interaction> vecRetrievalInteractions;



    RetrievalForClickedAndImpression(vecTotalInteractions, mapUserID2Index,490234,21560710,4165614,2,2,clickImpressionPair);
    RetrievalForClicked(vecTotalInteractions, mapUserID2Index, 490234, setAdIDQueryIDPair);
    RetrievalForImpressed(vecTotalInteractions, mapUserID2Index,490234, 372875,vecRetrievalInteractions);
    RetrievalForProfit(vecTotalInteractions, mapAdID2Index,7686695, 0.0001);
    std::cout.flush();
    std::vector<std::vector<size_t> > cmdList;
//    parseCommand(cmdList);

    return 0;
}

