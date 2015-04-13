#include <iostream>
#include <stdio.h>
#include <map>
#include <set>
#include <utility>
#include <algorithm>

#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include "data_io.h"
#include "interaction.h"
#include "retrieval_method.h"

#define DEBUG

using namespace std;


const size_t k_get = 0;
const size_t k_clicked = 1;
const size_t k_impressed = 2;
const size_t k_profit = 3;
const size_t k_quit = 4;
const double k_thetaScale = 10000000;
const size_t k_totalLineNum = 149639105;

/*
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

//int loadFileC(const char* fileName,
//              std::vector<Interaction> &vecInteractions,
//              std::multimap<size_t, size_t> &mapUserID2Index,
//              std::multimap<size_t, size_t> &mapAdID2Index){
//    FILE *file = fopen(fileName, "r");
//    char line[256];

//    time_t t1, t2;
//    t1 = time(NULL);

//    std::cout<<"scan file with fgets\n";
//    while(fgets(line, 256, file)){
//        int length = 0;
//        for(; length < 256; ++length){
//            if(line[length] == '\n'){
//                break;
//            }
//        }

//    }

//    t2 = time(NULL);
//    std::cout<<"loading compeleted, used "<< t2-t1 << "sec \n";
//    std::cout.flush();
//    return 0;

//}
*/

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
                 std::multimap<size_t, size_t> &mapAdID2Index,
                 std::map<size_t, std::set<size_t> > &mapUserID2AdIDSet){
    //init
    vecInteractions.clear();
    mapUserID2Index.clear();
    mapAdID2Index.clear();

#ifdef DEBUG
    time_t t1, t2;
    t1 = time(NULL);
    std::cout<<"start loading file\n";
    std::cout.flush();
#endif

    //file descriptor
    int fd;
    //file info
    struct stat fileState;
    int status;
    size_t fileSize;

    //the memory-mapped thing itself
    void *mapped;

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
    status = fstat(fd, & fileState);
    if(status){
        std::cout<<"failed to get file status\n";

    }
    fileSize = fileState.st_size;

    //memory-map the file
    try{
        //mapped = mmap(0, fileSize, PROT_READ, MAP_PRIVATE| MAP_POPULATE, fd, 0);
        mapped = mmap(0, fileSize, PROT_READ, MAP_SHARED, fd, 0);
        if(mapped == MAP_FAILED){
            throw("failed to map file\n");
        }

    }catch(const char* msg){
        std::cout<<msg<<std::endl;
        std::cout.flush();
        return -1;
    }
    madvise(mapped, fileSize, MADV_SEQUENTIAL|MADV_WILLNEED);




#ifdef DEBUG
    std::cout<<"start conuting line\n";
#endif

    //count line num
    char *ptr = (char*)mapped;
    size_t nChrCnt = 0;
    size_t nLineNum = 0;

    for(; nChrCnt < fileSize; ++nChrCnt){
        if(*ptr == '\n'){
            ++nLineNum;
        }
        ++ptr;
    }

    nChrCnt = 0;
    ptr = static_cast<char*>(mapped);

    vecInteractions.resize(nLineNum);

#ifdef DEBUG
    t2 = time(NULL);
    std::cout << "line num " << nLineNum << " time used " << t2-t1 << "sec \n";
    std::cout.flush();
#endif




    //map userid to its adList
    std::map<size_t, std::set<size_t> >::iterator iter;

    size_t nLineCnt = 0;
    while(1){
        int nEndPos = 0;
        while((nChrCnt < fileSize) && (ptr[nEndPos] != '\n')){
            ++nEndPos;
            ++nChrCnt;
        }

//        Interaction interaction(ptr, ptr+nEndPos);
//        vecInteractions.push_back(interaction);
//        mapUserID2Index.insert(std::pair<size_t, size_t>(interaction.userID, nCnt));
//        mapAdID2Index.insert(std::pair<size_t, size_t>(interaction.adID, nCnt));

        vecInteractions[nLineCnt].init(ptr, ptr+nEndPos);
        mapUserID2Index.insert(std::pair<size_t, size_t>(vecInteractions[nLineCnt].userID, nLineCnt));
        mapAdID2Index.insert(std::pair<size_t, size_t>(vecInteractions[nLineCnt].adID, nLineCnt));
        iter = mapUserID2AdIDSet.find(vecInteractions[nLineCnt].userID);
        if(iter != mapUserID2AdIDSet.end()){
            (iter->second).insert(vecInteractions[nLineCnt].adID);
        }else{
            std::set<size_t> adIDset;
            adIDset.insert(vecInteractions[nLineCnt].adID);
            mapUserID2AdIDSet.insert(std::pair<size_t, std::set<size_t> >(vecInteractions[nLineCnt].userID, adIDset));
        }

#ifdef DEBUG
        if(nLineCnt%1000000 == 0){
            t2 = time(NULL);
            std::cout << (double)nLineCnt*100/nLineNum << "% loaded. used time " << t2-t1 << "sec \n";
            std::cout.flush();
        }
#endif

        ++nEndPos;
        ++nChrCnt;
        ++nLineCnt;

        if(nChrCnt >= fileSize){
            break;
        }
        ptr = ptr + nEndPos;
    }

    //close file and file mapping
    try{
        if(munmap(mapped, fileSize)){
            throw("failed to unmapping file\n");
        }
        if(close(fd)){
            throw("failed to close file\n");
        }
    }catch(const char *msg){
        std::cout<<msg<<std::endl;
        std::cout.flush();
        return -1;
    }

#ifdef DEBUG
    t2 = time(NULL);
    std::cout<<"loading compeleted, used "<< t2-t1 << "sec \n";
#endif

    return 0;

}


void parseCommand(std::vector<std::vector<size_t> > &cmdList){
    int nCmdCnt = 0;
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
            size_t adID;
            double theta;
            cin>>adID;
            cin>>theta;
            cmdParam.push_back(adID);
            cmdParam.push_back(static_cast<size_t>(theta*k_thetaScale));

        }else if(!cmd.compare("quit")){
            cmdParam.push_back(k_quit);
        }

//        for(int i = 0; i < cmdParam.size(); ++i){
//            std::cout<<cmdParam[i]<<" ";
//        }
//        std::cout<<std::endl;
//        std::cout.flush();

        if(!cmdParam.empty()){
            cmdList.push_back(cmdParam);
        }

        if(cmdList[nCmdCnt][0] == k_quit){
            return;
        }
    }
}


void run(std::vector<std::vector<size_t> > &cmdList,
         std::vector<Interaction> &vecTotalInteractions,
         std::multimap<size_t, size_t> &mapUserID2Index,
         std::multimap<size_t, size_t> &mapAdID2Index,
         std::map<size_t, std::set<size_t> > &mapUserID2adIDSet){



    for(size_t i = 0; i < cmdList.size(); ++i){
        std::pair<size_t, size_t> clickImpressionPair;
        std::set<pair<size_t, size_t>, PairLess<size_t, size_t> > setAdIDQueryIDPair;
        std::vector<Interaction> vecRetrievalInteractions;
        switch (cmdList[i][0]) {
        case k_get:
            RetrievalForClickedAndImpression(vecTotalInteractions,
                                             mapUserID2Index,
                                             cmdList[i][1], cmdList[i][2], cmdList[i][3], cmdList[i][4], cmdList[i][5],
                                            clickImpressionPair);
            break;
        case k_clicked:
            RetrievalForClicked(vecTotalInteractions, mapUserID2Index,
                                cmdList[i][1], setAdIDQueryIDPair);
            break;
        case k_impressed:
            RetrievalForImpressed(vecTotalInteractions, mapUserID2Index, mapUserID2adIDSet,
                                  cmdList[i][1], cmdList[i][2],
                    vecRetrievalInteractions);
            break;
        case k_profit:
            RetrievalForProfit(vecTotalInteractions, mapAdID2Index,cmdList[i][1], static_cast<double>(cmdList[i][2])/k_thetaScale);
            break;
        case k_quit:
            return;
        }
    }
}


int main(int argc, char *argv[], char *envp[])
{
    std::string strFileAddress(argv[1]);
    std::vector<Interaction> vecTotalInteractions;
    std::multimap<size_t, size_t> mapUserID2Index;
    std::multimap<size_t, size_t> mapAdID2Index;
    std::map<size_t, std::set<size_t> > mapUserID2adIDSet;

    std::pair<size_t, size_t> clickImpressionPair;
    std::set<pair<size_t, size_t>, PairLess<size_t, size_t> > setAdIDQueryIDPair;
    std::vector<Interaction> vecRetrievalInteractions;

#ifdef DEUBG
    std::cout <<"the input argument is "<< strFileAddress << std::endl;
#endif

    if(readFile(strFileAddress,
                vecTotalInteractions,
                mapUserID2Index,
                mapAdID2Index,
                mapUserID2adIDSet)){
        return -1;
    }

#ifdef DEBUG
    time_t t1, t2;
    t1 = time(NULL);



    for(int i = 0; i < 400; ++i){
        RetrievalForClicked(vecTotalInteractions, mapUserID2Index, 490234, setAdIDQueryIDPair);
        RetrievalForClickedAndImpression(vecTotalInteractions, mapUserID2Index,490234,21560664,2255103,2,2,clickImpressionPair);
        RetrievalForProfit(vecTotalInteractions, mapAdID2Index,21375650, 0.5);
        RetrievalForImpressed(vecTotalInteractions, mapUserID2Index,mapUserID2adIDSet, 6231944, 490234,vecRetrievalInteractions);


        //clicked 12565
        //get 6231937 21459920 2416 2 2
        //profit 21459920 0.1
        //impressed 6231938 0
        //quit
          RetrievalForClicked(vecTotalInteractions, mapUserID2Index, 12565, setAdIDQueryIDPair);
          RetrievalForClickedAndImpression(vecTotalInteractions, mapUserID2Index,6231937,21459920,2416,2,2,clickImpressionPair);
          RetrievalForProfit(vecTotalInteractions, mapAdID2Index,21459920, 0.1);
          RetrievalForImpressed(vecTotalInteractions, mapUserID2Index,mapUserID2adIDSet, 6231938, 0,vecRetrievalInteractions);
    }

      t2 = time(NULL);
      std::cout<<"process 400 cmd used "<<t2-t1<<" sec\n";
      std::cout.flush();
#endif


#ifndef DEBUG
    std::vector<std::vector<size_t> > cmdList;
    parseCommand(cmdList);
    run(cmdList, vecTotalInteractions, mapUserID2Index, mapAdID2Index, mapUserID2adIDSet);
#endif

    return 0;

}

