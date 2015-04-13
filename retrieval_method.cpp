#include "retrieval_method.h"

bool compareForGet(const Interaction &interaction,
                   size_t adID,
                   size_t queryID,
                   short position,
                   short depth){
    if(interaction.adID != adID){
        return false;
    }

    if(interaction.queryID != queryID){
        return false;
    }

    if(interaction.position != position){
        return false;
    }

    if(interaction.depth != depth){
        return false;
    }

    return true;
}

//bool compareForImpressed(const Interaction& a, const Interaction& b){
//    if(a.adID != b.adID){
//        return false;
//    }
//    if(a.advertiserID != b.advertiserID){
//        return false;
//    }

//    if(a.keywordID != b.keywordID){
//        return false;
//    }

//    if(a.titleID != b.titleID){
//        return false;
//    }

//    if(a.descriptionID != b.descriptionID){
//        return false;
//    }

//    if(a.displayURL.compare(b.displayURL)){
//        return false;
//    }

//    return true;
//}

void RetrievalForClickedAndImpression(const std::vector<Interaction> &vecData,
                            std::multimap<size_t, size_t> &mapUserID2Index,
                            size_t userID,
                            size_t adID,
                            size_t queryID,
                            size_t position,
                            size_t depth,
                            std::pair<size_t, size_t> &pairClickImpression){

    std::multimap<size_t, size_t>::iterator iter;
    std::pair<std::multimap<size_t, size_t>::iterator, std::multimap<size_t, size_t>::iterator> iterRange;
    iterRange = mapUserID2Index.equal_range(userID);

    size_t unClickCnt = 0;
    size_t unImpressionCnt = 0;
    for(iter = iterRange.first; iter != iterRange.second; ++iter){
       if(compareForGet(vecData[iter->second], adID, queryID, position, depth)){
           unClickCnt += vecData[iter->second].click;
           unImpressionCnt += vecData[iter->second].impression;

       }
    }

    pairClickImpression = std::pair<size_t, size_t>(unClickCnt, unImpressionCnt);

    //print
    std::cout <<"********************\n";
    std::cout << unClickCnt << "\t" << unImpressionCnt << std::endl;
    std::cout <<"********************\n";
}

void RetrievalForClicked(const std::vector<Interaction> &vecInteractions,
                std::multimap<size_t, size_t> &mapUserID2Index,
                size_t userID,
                std::set<std::pair<size_t, size_t>, PairLess<size_t, size_t> > &setAdIDQueryIDPair){
    //init
    setAdIDQueryIDPair.clear();


    std::pair<std::multimap<size_t, size_t>::iterator, std::multimap<size_t, size_t>::iterator> iterRange;
    iterRange = mapUserID2Index.equal_range(userID);

    std::multimap<size_t, size_t>::iterator iter;
    for(iter = iterRange.first; iter != iterRange.second; ++iter){
        if(vecInteractions[iter->second].click >= 1){
            setAdIDQueryIDPair.insert(
                    std::pair<size_t, size_t>(  vecInteractions[iter->second].adID,
                                                vecInteractions[iter->second].queryID));
        }
    }

    //print
    {
        std::cout<<"********************\n";
        std::set<std::pair<size_t, size_t>, PairLess<size_t, size_t> >::iterator iter;
        for(iter = setAdIDQueryIDPair.begin(); iter != setAdIDQueryIDPair.end(); ++iter){
            std::cout<<iter->first << "\t" << iter->second <<std::endl;
        }
        std::cout<<"********************\n";
    }
}


void findInteractionWithAdSet(const std::vector<Interaction> &vecInteractions,
                              std::multimap<size_t, size_t> &mapUserID2Index,
                              size_t userID,
                              std::set<size_t> &setCommonAdID,
                              std::set<Interaction, InteractionLessForImpressionRetreival> &setCommonRetrieval){


    std::pair<std::multimap<size_t, size_t>::iterator, std::multimap<size_t, size_t>::iterator> iterRange;
    iterRange = mapUserID2Index.equal_range(userID);

    std::multimap<size_t, size_t>::iterator interactionIter = iterRange.first;
    std::set<size_t>::iterator adIDIter = setCommonAdID.begin();

    while((adIDIter!= setCommonAdID.end()) && (interactionIter != iterRange.second)){
        if(vecInteractions[interactionIter->second].adID > (*adIDIter)){
            ++adIDIter;
        }else if(vecInteractions[interactionIter->second].adID < (*adIDIter)){
            ++interactionIter;
        }else{
            setCommonRetrieval.insert(vecInteractions[interactionIter->second]);
            ++interactionIter;
        }
    }

}



void RetrievalForImpressed(const std::vector<Interaction> &vecInteractions,
                           std::multimap<size_t, size_t> &mapUserID2Index,
                           std::map<size_t,  std::set<size_t>  > &mapUserID2AdIDSet,
                           size_t userID_1,
                           size_t userID_2,
                           std::vector<Interaction> &vecRetrievaledInteraction){

    std::set<Interaction, InteractionLessForImpressionRetreival> setRetrievals_1;
    std::set<Interaction, InteractionLessForImpressionRetreival> setRetrievals_2;
    std::set<Interaction, InteractionLessForImpressionRetreival> setCommonRetrieval;

    std::set<size_t> setCommonAdID;

    findCommonSet(mapUserID2AdIDSet[userID_1], mapUserID2AdIDSet[userID_2], setCommonAdID);
    {
        std::multimap<size_t, size_t>::iterator iter;
        std::pair<std::multimap<size_t, size_t>::iterator, std::multimap<size_t, size_t>::iterator> iterRange;
        iterRange = mapUserID2Index.equal_range(userID_1);

        for(iter = iterRange.first; iter != iterRange.second; ++iter){
            if(setCommonAdID.find(vecInteractions[iter->second].adID) != setCommonAdID.end()){
                setRetrievals_1.insert(vecInteractions[iter->second]);
            }
        }

        iterRange = mapUserID2Index.equal_range(userID_2);
        for(iter = iterRange.first; iter != iterRange.second; ++iter){
            if(setCommonAdID.find(vecInteractions[iter->second].adID) != setCommonAdID.end()){
                setRetrievals_2.insert(vecInteractions[iter->second]);
            }
        }
    }


//    findInteractionWithAdSet(vecInteractions, mapUserID2Index, userID_1, setCommonAdID, setCommonRetrieval);
//    findInteractionWithAdSet(vecInteractions, mapUserID2Index, userID_2, setCommonAdID, setCommonRetrieval);


    {
        std::set<Interaction, InteractionLessForImpressionRetreival>::iterator iter1;
        std::set<Interaction, InteractionLessForImpressionRetreival>::iterator iter2;
        for(iter1 = setRetrievals_1.begin(); iter1 != setRetrievals_1.end();  ++iter1){
            setCommonRetrieval.insert(*iter1);
        }

        for(iter2 = setRetrievals_2.begin(); iter2 != setRetrievals_2.end(); ++iter2){
            setCommonRetrieval.insert(*iter2);
        }
    }

    //print result
    {
        std::cout<<"********************\n";

        size_t tmpAdID = 0;
        std::set<Interaction, InteractionLessForImpressionRetreival>::iterator iter = setCommonRetrieval.begin();
        for(; iter != setCommonRetrieval.end(); ++iter){
            if(tmpAdID != (*iter).adID){
                tmpAdID = (*iter).adID;
                std::cout<<tmpAdID<<std::endl;
            }
            std::cout<< "\t" << (*iter).displayURL_H <<(*iter).displayURL_L << " " << (*iter).advertiserID << " " << (*iter).keywordID << " "
                    << (*iter).titleID<< " " << (*iter).descriptionID<<std::endl;
        }

        std::cout<<"********************\n";
    }
}

void RetrievalForProfit(std::vector<Interaction> &vecInteractions,
                        std::multimap<size_t, size_t> &mapAdID2Index,
                        size_t adID,
                        double theta){
    std::map<size_t, std::pair<size_t, size_t> > mapUserID2ClkImp;
    std::pair<std::multimap<size_t, size_t>::iterator, std::multimap<size_t, size_t>::iterator> adIterRange;
    adIterRange = mapAdID2Index.equal_range(adID);

    std::multimap<size_t, size_t>::iterator adIter;
    std::map<size_t, std::pair<size_t, size_t> >::iterator userIter;
    Interaction tmp;
    for(adIter = adIterRange.first; adIter != adIterRange.second; ++adIter){
        tmp =  vecInteractions[adIter->second];
        userIter = mapUserID2ClkImp.find(tmp.userID);
        if(userIter == mapUserID2ClkImp.end()){
           mapUserID2ClkImp.insert(std::pair<size_t, std::pair<size_t, size_t> >(tmp.userID, std::pair<size_t, size_t>(tmp.click, tmp.impression)));
        }else{
            userIter->second.first += tmp.click;
            userIter->second.second += tmp.impression;
        }
    }

    std::set<pair<double, size_t>, PairLess<double, size_t> > retrievaledUserID;

    for(userIter = mapUserID2ClkImp.begin();userIter != mapUserID2ClkImp.end(); ++userIter){
        double ratio;
        if(userIter->second.second == 0){
            ratio = 0;
        }else{
            ratio = (double)userIter->second.first/(double)userIter->second.second;
        }
        if(ratio >= theta){
            retrievaledUserID.insert(std::pair<double, size_t>(ratio, userIter->first));
        }
    }

    //print
    {
        std::cout<<"********************\n";
        std::set<pair<double, size_t>, PairLess<double, size_t> >::iterator iter = retrievaledUserID.begin();

        for(; (iter != retrievaledUserID.end()); ++iter){
            std::cout << (*iter).second << std::endl;
        }
        std::cout<<"********************\n";
    }
}

