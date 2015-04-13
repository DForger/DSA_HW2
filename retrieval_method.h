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

//        if(l.displayURL < r.displayURL){
//            return true;
//        }else if(l.displayURL > r.displayURL){
//            return false;
//        }
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

        if(l.descriptionID < r.titleID){
            return true;
        }else if(l.titleID > r.titleID){
            return false;
        }

        return false;
    }
};


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
                           size_t userID_1,
                           size_t userID_2,
                           std::vector<Interaction> &vecRetrievaledInteraction);

void RetrievalForProfit(std::vector<Interaction> &vecInteractions,
                        std::multimap<size_t, size_t> &mapAdID2Index,
                        size_t adID,
                        double theta);


#endif // RETRIEVAL_METHOD_H

