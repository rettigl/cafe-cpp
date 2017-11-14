///
/// \file    global.h
/// \brief   For access to containers for callbacks
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///


#ifndef GLOBAL_H
#define GLOBAL_H

#include <cadef.h>
#include <hashConduit.h>
#include <hashConduitGroup.h>
#include <PVGroup.h>
#include <deviceCollection.h>
#include <enumStrings.h>

extern bool CHECK_CONSISTENCY_CA_STATE;
extern bool MUTEX;
extern epicsMutex cafeMutex;



extern cafeConduit_set cs;
extern cafeGroup_set gs;
extern vector<PVGroup> PVGroupV;
extern vector<PVGroup> PVGroupPseudo;
extern vector<deviceCollection>  deviceCollectionV;
extern map<vector<unsigned int>, string> groupPseudoMap;

extern vector<string> globalChannelList;


extern string contentsS;
extern string contentsBS;
extern unsigned long nCBs;

#endif // GLOBAL_H
