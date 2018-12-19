///
/// \file   cafeEnumStrings.cpp
/// \author Jan Chrin, PSI
/// \date   Release: February 2015
/// \version CAFE 1.0.0
///

#include <cafeEnum.h>
#include <enumStrings.h>

template<> char const * enumStrings<CAFENUM::ChannelWaitForResponsePolicyKind>::data[]
    = {"CAFENUM::WAIT", "CAFENUM::NO_WAIT"};

template<> char const * enumStrings<CAFENUM::ChannelRequestDataTypePolicyKind>::data[]
    = {"CAFENUM::NATIVE_DATATYPE", "CAFENUM::LOWEST_DATATYPE"};

template<> char const * enumStrings<CAFENUM::ChannelFlushSendBufferPolicyKind>::data[]
    = {"CAFENUM::WITH_FLUSH_IO","CAFENUM::WITH_PEND_IO","CAFENUM::WITH_PEND_EVENT", "CAFENUM::WITH_POLL"};

template<> char const * enumStrings<CAFENUM::ChannelWhenToFlushSendBufferPolicyKind>::data[]
    = {"CAFENUM::FLUSH_AFTER_EACH_MESSAGE","CAFENUM::FLUSH_DESIGNATED_TO_CLIENT"};

template<> char const * enumStrings<CAFENUM::ChannelRequestPolicyKind>::data[]
    = {"CAFENUM::WITHOUT_CALLBACK","CAFENUM::WITH_CALLBACK_DEFAULT","CAFENUM::WITH_CALLBACK_USER_SUPPLIED"};

template<> char const * enumStrings<CAFENUM::DBR_TYPE>::data[]
    = {"CAFENUM::DBR_PRIMITIVE","CAFENUM::DBR_STS","CAFENUM::DBR_TIME", "CAFENUM::DBR_GR",
       "CAFENUM::DBR_CTRL","CAFENUM::DBR_PUT", "CAFENUM::DBR_STSACK","CAFENUM::DBR_CLASS",
       "CAFENUM::DBR_OTHER"
      };

template<> char const * enumStrings<CAFENUM::StatusMessageKind>::data[]
    = {"CAFENUM::NO_MESSAGE","CAFENUM::PRE_REQUEST","CAFENUM::FROM_REQUEST","CAFENUM::FROM_PEND",
       "CAFENUM::FROM_CALLBACK"
      };

template<> char const * enumStrings<CAFENUM::CallbackProgressKind>::data[]
    = {"CAFENUM::NOT_INITIATED","CAFENUM::PENDING","CAFENUM::COMPLETE"};


