#ifndef RETRIEVAL_METHOD_H
#define RETRIEVAL_METHOD_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include "interaction.h"

template<typename T1 = size_t, typename T2 = size_t>
struct PairLess{
    bool operator()(const std::pair<T1, T2> &l, const std::pair<T1, T2> &r){
        if(l.first < r.first){
            return true;
        }else if(l.first > r.first){
            return false;
        }else{
            if(l.second < r.second){
                return true;
            }else{
                return false;
            }
        }
    }
};

template<typename T1 = size_t, typename T2 = size_t>
struct PairGreater{
    bool operator()(const std::pair<T1, T2> &l, const std::pair<T1, T2> &r){
        if(l.first > r.first){
            return true;
        }else if(l.first < r.first){
            return false;
        }else{
            if(l.second > r.second){
                return true;
            }else{
                return false;
            }
        }
    }
};


struct InteractionLessForImpressionRetreival{
    bool operator()(const Interaction &l, const Interaction &r){
        if(l.adID < r.adID){
            return true;
        }else if(l.adID > r.adID){
            return false;
        }

        if(l.displayURL_H < r.displayURL_H){
            return true;
        }else if(l.displayURL_H > r.displayURL_H){
            return false;
        }

        if(l.displayURL_L < r.displayURL_L){
            return true;
        }else if(l.displayURL_L > r.displayURL_L){
            return false;
        }

        if(l.advertiserID < r.advertiserID){
            return true;
        }else if(l.advertiserID > r.advertiserID){
            return false;
        }

        if(l.keywordID < r.keywordID){
            return true;
        }else if(l.keywordID > r.keywordID){
            return false;
        }

        if(l.titleID < r.titleID){
            return true;
        }else if(l.titleID > r.titleID){
            return false;
        }

        if(l.descriptionID < r.descriptionID){
            return true;
        }else if(l.descriptionID > r.descriptionID){
            return false;
        }

        return false;
    }
};

template<typename T1>
void findCommonSet(std::set<T1> &s1, std::set<T1> &s2, std::set<T1> &s3){
    s3.clear();
    typename std::set<T1>::iterator iter1 = s1.begin();
    typename std::set<T1>::iterator iter2 = s2.begin();


    while((iter1 != s1.end()) && (iter2 != s2.end())){
        if((*iter1) > (*iter2)){
            ++iter2;
        }else if((*iter1) < (*iter2)){
            ++iter1;
        }else{
            s3.insert(*iter1);
            ++iter1;
            ++iter2;
        }
    }

//    if(iter1 == s1.end()){
//        while(iter2 != s2.end()){
//            if(*iter2 < *iter1){
//                ++iter2;
//            }else if(*iter2 > *iter1){
//                return;
//            }else{
//                s3.insert(*iter2);
//                return;
//            }
//        }
//    }

//    if(iter2 == s2.end()){
//        while(iter1 != s1.end()){
//            if(*iter1 < *iter2){
//                ++iter1;
//            }else if(*iter1 > *iter2){
//                return;
//            }else{
//                s3.insert(*iter2);
//                return;
//            }
//        }
//    }

}

//template<typename T1>
//void findCommonSet(const std::set<T1> &s1, std::set<T1> &s2, std::set<T1> &s3){

//    typename std::set<T1>::iterator iter1 = s1.begin();

//    typename std::set<T1>::iterator iter2 = s2.begin();

//    size_t nLengthS1 = s1.size();
//    size_t nLengthS2 = s2.size();

//    size_t nCntS1 = 0, nCntS2 = 0;
//    while((nCntS1 < nLengthS1) && (nCntS2 < nLengthS2)){
//        if(iter1[nCntS1] > iter2[nCntS2]){
//            ++nCntS2;
//        }else if(iter1[nCntS1] < iter2[nCntS2]){
//            ++nCntS1;
//        }else{
//            s3.insert(iter1[nCntS1]);
//            ++nCntS1;
//            ++nCntS2;
//        }
//    }

//    if(nCntS1 == nLengthS1){
//        while(nCntS2 < nLengthS2){
//            if(iter2[nCntS2] < iter1[nCntS1]){
//                ++nCntS2;
//            }else if(iter2[nCntS2] > iter1[nCntS1]){
//                return;
//            }else{
//                s3.insert(iter2[nCntS2]);
//                return;
//            }
//        }
//    }

//    if(nCntS2 == nLengthS2){
//        while(nCntS1 < nLengthS1){
//            if(iter2[nCntS2] > iter1[nCntS1]){
//                ++nCntS1;
//            }else if(iter2[nCntS2] < iter1[nCntS1]){
//                return;
//            }else{
//                s3.insert(iter2[nCntS2]);
//                return;
//            }
//        }
//    }

//}

void RetrievalForClickedAndImpression(const std::vector<Interaction> &vecData,
                            std::multimap<size_t, size_t> &mapUserID2Index,
                            size_t userID,
                            size_t adID,
                            size_t queryID,
                            size_t position,
                            size_t depth,
                            std::pair<size_t, size_t> &pairClickImpression);

void RetrievalForClicked(const std::vector<Interaction> &vecInteractions,
                std::multimap<size_t, size_t> &mapUserID2Index,
                size_t userID,
                std::set<std::pair<size_t, size_t>, PairLess<size_t, size_t> > &setAdIDQueryIDPair);


void RetrievalForImpressed(const std::vector<Interaction> &vecInteractions,
                           std::multimap<size_t, size_t> &mapUserID2Index,
                           std::map<size_t, std::set<size_t> > &mapUserID2AdIDSet,
                           size_t userID_1,
                           size_t userID_2,
                           std::vector<Interaction> &vecRetrievaledInteraction);

void RetrievalForProfit(std::vector<Interaction> &vecInteractions,
                        std::multimap<size_t, size_t> &mapAdID2Index,
                        size_t adID,
                        double theta);


#endif // RETRIEVAL_METHOD_H

