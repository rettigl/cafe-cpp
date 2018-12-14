///
/// \file    granules.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0

/**
 *  Contains methods that are used by Instant template
 *  Among these are the Verify, Prepare, Execute
 */

#ifndef GRANULES_H
#define GRANULES_H

#include <boost/date_time/posix_time/posix_time.hpp>

#include <config.h>

#if HAVE_BOOST_THREAD
#include <boost/thread/thread_only.hpp>
#include <boost/chrono.hpp>
#endif


#include <global.h>
#include <conduitFriends.h>
#include <handleHelper.h>



class Granules {
public:

    template <class CTYPE> friend class Instant;
    friend class CAFE;
    Granules() {};

private:
    HandleHelper helper;
    CAFEStatus cafeStatus;

    ChannelRequestDataTypePolicy  channelRequestDataTypePolicy;

    ChannelRequestPolicyKind methodKind; //for channelExecutePut

    ChannelRequestPolicy  channelRequestPolicyPut;
    ChannelRequestStatus  channelRequestStatusPut;

    ChannelRequestPolicy  channelRequestPolicyGet;
    ChannelRequestStatus  channelRequestStatusGet;

    ChannelRequestMetaDataClient      channelRequestMetaDataClient;
    ChannelRequestMetaData            channelRequestMetaData;
    ChannelRequestMetaData            channelRequestMetaPrimitive;

    ChannelRequestPolicy  channelRequestPolicyGetCtrl;
    ChannelRequestStatus  channelRequestStatusGetCtrl;

    ChannelRequestMetaDataClient    channelRequestMetaCtrlClient;
    ChannelRequestMetaData          channelRequestMetaCtrl;

    ChannelTimeoutPolicy        channelTimeoutPolicyGet;
    ChannelTimeoutPolicy        channelTimeoutPolicyPut;

    int  channelVerifyPut      (const unsigned int, chtype); //isChannelConnected //writeAccess
    int  channelPreparePut     (const unsigned int); //nelem offset requestType
    int  channelExecutePut     (const unsigned int);

    int  waitForPutEvent    (const unsigned int, double);
    bool isPutCallbackDone  (const unsigned int  _handle);

    int  channelVerifyGet       (const unsigned int, chtype); //isChannelConnected //readAccess
    int  channelPrepareGet      (const unsigned int); //nelem offset requestType
    int  channelExecuteGet      (const unsigned int); //report status
    int  channelExecuteGetNoWait(const unsigned int); //report status

    int  waitForManyGetEvents(const unsigned int * handleArray,  unsigned int arrayLength, std::map<unsigned int, int> & bundleResponse);

    int  waitForGetEvent     (const unsigned int, double);
    bool isGetCallbackDone   (const unsigned int  _handle);

    int  channelVerifyGetCtrl  (const unsigned int, chtype); //isChannelConnected //readAccess
    int  channelPrepareGetCtrl (const unsigned int); //nelem offset requestType
    int  channelExecuteGetCtrl (const unsigned int); //report status
    int  channelExecuteGetCtrlNoWait(const unsigned int  _handle);

    int  waitForGetCtrlEvent(const unsigned int, double);
    bool isGetCtrlCallbackDone(const unsigned int  _handle);

    int  status;

    chtype convertMatrix(const chtype nativeType, const chtype clientT);

};



#endif // GRANULES_H
