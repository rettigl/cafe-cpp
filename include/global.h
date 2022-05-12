///
/// \file    global.h
/// \brief   For access to containers for callbacks
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///


#ifndef GLOBAL_H
#define GLOBAL_H
#include <epicsMutex.h>
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
extern std::vector<PVGroup> PVGroupV;
extern std::vector<PVGroup> PVGroupPseudo;
extern std::vector<deviceCollection>  deviceCollectionV;
extern std::map<std::vector<unsigned int>, std::string> groupPseudoMap;

extern std::vector<std::string> globalChannelList;

extern std::string contentsS;
extern std::string contentsBS;

extern unsigned long nCBs;
extern bool callbackLiveFlag;
extern bool SF_WITH_PULSE_ID;


#endif // GLOBAL_H
