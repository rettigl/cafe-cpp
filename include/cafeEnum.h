///
/// \file    cafeEnum.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef CAFEENUM_H
#define CAFEENUM_H

#include <cadef.h>

namespace  CAFENUM
{

/**
 *  Data request buffer types \n
 *  i.e. whether PRIMITIVE, STS, TIME, GR or CTRL
 */
enum DBR_TYPE
{
    DBR_PRIMITIVE,
    DBR_PLAIN=DBR_PRIMITIVE,
    DBR_STS,
    DBR_TIME,
    DBR_GR,
    DBR_CTRL,
    DBR_PUT, //DBR_PUT_ACKT and DBR_PUT_ACKS Write only - used from global alarm acknowledge.
    DBR_STSACK, // is DBR_STSACK_STRING
    DBR_CLASS,  // is DBR_CLASS_NAME,
    DBR_NONE    // should not occur, but used internally within cafeVectors.h
};


/**
 *  Define enum type to flush io after creating channels
 */
enum ChannelFlushSendBufferPolicyKind
{
    WITH_FLUSH_IO,
    WITH_PEND_IO,   // needs timeout duration
    WITH_PEND_EVENT, // needs timeout duration
    WITH_POLL
};


/**
 *  Define enum type defining when to flush io after creating channels
 */
enum ChannelWhenToFlushSendBufferPolicyKind
{
    FLUSH_AUTOMATIC=0,
    FLUSH_NOW=FLUSH_AUTOMATIC,
    FLUSH_AFTER_EACH_CHANNEL_CREATION=FLUSH_NOW, // instantly
    FLUSH_AFTER_EACH_CHANNEL_SUBSCRIPTION=FLUSH_NOW,
    FLUSH_AFTER_EACH_MESSAGE=FLUSH_NOW,
    FLUSH_AFTER_EACH_GROUP_CREATION=FLUSH_NOW,
    FLUSH_DESIGNATED_TO_CLIENT         //
};

/**
 *  Enum type defining ca server dispatch priority
 *  Note that specifying different priorities within the same program
 *  can increase resource consumption in thw client and server because
 *  an independent virtual circuit, and associated data structures, is
 *  created for each priority that is used on a particular server
 */
enum ChannelServerDispatchPriority
{
    CA_SERVER_DISPATCH_PRIORITY_MIN     =CA_PRIORITY_MIN, //0
    CA_SERVER_DISPATCH_PRIORITY_VERYLOW =CA_PRIORITY_MIN+1, 
    CA_SERVER_DISPATCH_PRIORITY_LOW     =CA_PRIORITY_MIN+25,
    CA_SERVER_DISPATCH_PRIORITY_MED     =CA_PRIORITY_MIN+50,
    CA_SERVER_DISPATCH_PRIORITY_HIGH    =CA_PRIORITY_MIN+75,
    CA_SERVER_DISPATCH_PRIORITY_VERYHIGH=CA_PRIORITY_MIN+98,
    CA_SERVER_DISPATCH_PRIORITY_MAX     =CA_PRIORITY_MAX,
    CA_SERVER_DISPATCH_PRIORITY_DEFAULT =CA_PRIORITY_MAX
};


/**
 *  Enum type defining the wait/nowait modes for cache operations.
 *  Meant for use within callbacks to ensure that cache operations
 *  are never blocked
 */
enum ChannelGetCacheWaitPolicyKind
{
    GET_CACHE_NO_CHECK=0,
    GET_CACHE_NO_WAIT,
    GET_CACHE_NOW     =GET_CACHE_NO_WAIT,
    GET_CACHE_WAIT
};


/**
 *  Enum type defining the modes for get operations when a
 *  monitor is in place
 *
 */
enum ChannelGetActionWhenMonitorPolicyKind
{
    GET_FROM_CACHE,
    GET_FROM_IOC
};

/**
 *  Enum type defining the blocking modes for ca operations.
 *  Blocking can be achieved with or without callback.
 *  Callback can further be supplied by the user
 */
enum ChannelRequestPolicyKind
{
    WITHOUT_CALLBACK,
    WITH_CALLBACK_DEFAULT,
    WITH_CALLBACK_USER_SUPPLIED
};


/**
 *  Enum type defining level of datatype conversion at the IOC
 *
 */
enum ChannelRequestDataTypePolicyKind
{
    NATIVE_DATATYPE,
    LOWEST_DATATYPE // The smaller in byte size of type requested and native datatype
};


/**
 *  Enum type defining whether to wait for a response
 *  from a message sent with a callback, whether it be
 *  user supplied or the CAFE default.
 */
enum ChannelWaitForResponsePolicyKind
{
    BLOCKING=0,
    WAIT=BLOCKING,
    NON_BLOCKING=1,
    NO_WAIT=NON_BLOCKING
};


enum StatusMessageKind
{
    NO_MESSAGE,
    PRE_REQUEST,
    FROM_REQUEST,
    FROM_PEND,
    FROM_CALLBACK,
    FROM_MESSAGE
};

enum CallbackProgressKind
{
    NOT_INITIATED,
    PENDING,
    COMPLETE
};

}; //namespace CAFENUM

#endif // CAFEENUM_H
