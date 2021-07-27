///
/// \file    handleHelper.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef HANDLEHELPER_H
#define HANDLEHELPER_H

#include <cadef.h>
#include <helper.h>
#include <defines.h>
#include <hashConduit.h>
#include <conduitFriends.h>
#include <hashConduitGroup.h>
#include <cafeDataType.h>
#include <defines.h>

//August 2018 Version 1.7
#include <tmDateMap.h>


class HandleHelper : public Helper
{

private:
    CAFEStatus cafeStatus;
    cafeConduit_set::iterator itcs;

    cafeGroup_set::iterator itgs;

    ChannelRequestMetaDataClient channelRequestMetaDataClient;
    ChannelRequestMetaData channelRequestMetaData;

    ChannelRequestMetaDataClient channelRequestMetaCtrlClient;
    ChannelRequestMetaData channelRequestMetaCtrl;

    ChannelDeviceAttribute channelDeviceAttribute;
    CAFEDataTypeCode cafeDataTypeCode;

    PrintErrorPolicy printErrorPolicy;

public:

    HandleHelper() {};
    ~HandleHelper() {};

    //Move to private if possible
    int setSTS(unsigned int  _handle, dbr_short_t a, dbr_short_t s, epicsTimeStamp ets);


    etsNorm _etsNorm;
    etsDate _etsDate;
    TMwdayText   tmDay;
    TMmonthpText tmMonth;

#if HAVE_PYTHON_H
    int setPyConnectCallbackFn(unsigned int _handle, void * callbackFn);
    int setPyGetCallbackFn(unsigned int _handle, void * callbackFn);
    int setPyPutCallbackFn(unsigned int _handle, void * callbackFn);
#endif
    int addWidget (unsigned int _handle, void * widget);
    int removeWidget (unsigned int _handle, void * widget);
    int getWidgets(unsigned int _handle, std::vector<void *> & widgetV);

    int  checkConsistency();
    int  checkConsistency(unsigned int  _handle);

    int  setChannelDeviceAttribute(std::string deliminator);
    int  setChannelDeviceAttribute(unsigned int  _handle, std::string deliminator);

    int  getChannelDevice(unsigned int  _handle, std::string & device);
    int  getChannelAttribute(unsigned int  _handle, std::string & attribute);

    int  getChannelRegalia(unsigned int  _handle, ChannelRegalia & channelInfo);

    int  getChannelRequestStatusGetClassName(unsigned int  _handle, ChannelRequestStatus &crsClassName);
    int  getChannelRequestStatusGetSTSACK   (unsigned int  _handle, ChannelRequestStatus &crsSTSACK);
    int  getChannelRequestStatusGetCtrl     (unsigned int  _handle, ChannelRequestStatus &crsCtrl);
    int  getChannelRequestStatusGet         (unsigned int  _handle, ChannelRequestStatus &crs);

    ca_client_context *  getContextFromPV     (const char * _pv);
    ca_client_context *  getContextFromHandle (unsigned int _handle);

    cafeConduit_set_by_handle::iterator getIterFromHandle(unsigned int handle);
    cafeConduit_set_by_handle & getcsHandleIndex();
    int modifyHandleIndexStatus(unsigned int handle, long status);
    int modifyChannelRequestMetaDataClient(unsigned int handle, 
                                           ChannelRequestMetaDataClient channelRequestMetaDataClient);
    int modifyChannelRequestStatusGet(unsigned int handle, 
                                           ChannelRequestStatus channelRequestStatusGet);
                               
    int modifyChannelTimeoutPolicyGet(unsigned int handle, 
                                           ChannelTimeoutPolicy channelTimeoutPolicyGet);

    const char * getPVFromHandle     (unsigned int  _handle);
    const char * getPVFromHandle     (unsigned int  _handle, ca_client_context * ccc);

    const char * getPV      (unsigned int  _handle)
    {
        return getPVFromHandle(_handle);
    }
    const char * getPVAlias (unsigned int  _handle);
    const char * getPVAlias (unsigned int  _handle,  ca_client_context * ccc);



    unsigned int  getHandleFromPV     (const char * _pv);
    unsigned int  getHandleFromPV     (const char * _pv, ca_client_context * ccc);
    unsigned int  getHandleFromPVAlias(const char * _pv);
    unsigned int  getHandleFromPVAlias(const char * _pv, ca_client_context * ccc);

    std::vector<unsigned int> getHandlesFromPVs(std::vector<std::string> pvV,  ca_client_context * ccc);
    std::vector<unsigned int> getHandlesFromPVs(std::vector<std::string> pvV);

    std::vector<unsigned int>  getDisconnectedHandlesFromWithinGroupV(unsigned int  gh);

    std::vector<unsigned int>  getHandlesFromWithinGroupV(unsigned int  gh);
    unsigned int * getHandlesFromWithinGroup(unsigned int  gh);

    unsigned int  getHandleFromPVWithinGroup(const char * _pv, unsigned int  gh);
    unsigned int  getHandleFromPVWithinGroup(const char * _pv,
            ca_client_context * ccc, unsigned int  gh);

    int getStatus   (unsigned int  h);
    int getTimeStamp(unsigned int  h, epicsTimeStamp &ts);





    etsNorm getEpicsTimeStampAsUInt32(unsigned int h)
    {
        epicsTimeStamp ts;
        getTimeStamp(h, ts);
        _etsNorm.secPastEpoch=ts.secPastEpoch;
        _etsNorm.nsec=ts.nsec;
        return _etsNorm;
    };

    etsDate getEpicsTimeStampAsDate(unsigned int h)
    {
        epicsTimeStamp ts;

        getTimeStamp(h, ts);

        epicsTime time(ts);

        local_tm_nano_sec local = (local_tm_nano_sec) time;
        _etsDate.year = local.ansi_tm.tm_year + 1900;
        _etsDate.mon = local.ansi_tm.tm_mon  + 1;
        _etsDate.day = local.ansi_tm.tm_mday;
        _etsDate.hour = local.ansi_tm.tm_hour;
        _etsDate.min = local.ansi_tm.tm_min;
        _etsDate.sec = local.ansi_tm.tm_sec;

        _etsDate.nsec = (unsigned long) ts.nsec;

        _etsDate.wday  = local.ansi_tm.tm_wday;
        _etsDate.yday  = local.ansi_tm.tm_yday;
        _etsDate.isdst = local.ansi_tm.tm_isdst;

        return _etsDate;
    }

    etsDate epicsTimeStampToDate(epicsTimeStamp ts)
    {
        epicsTime time(ts);
        local_tm_nano_sec local = (local_tm_nano_sec) time;
        _etsDate.year = local.ansi_tm.tm_year + 1900;
        _etsDate.mon = local.ansi_tm.tm_mon  + 1;
        _etsDate.day = local.ansi_tm.tm_mday;
        _etsDate.hour = local.ansi_tm.tm_hour;
        _etsDate.min = local.ansi_tm.tm_min;
        _etsDate.sec = local.ansi_tm.tm_sec;
        _etsDate.nsec = (unsigned long) ts.nsec;

        _etsDate.wday  = local.ansi_tm.tm_wday;
        _etsDate.yday  = local.ansi_tm.tm_yday;
        _etsDate.isdst = local.ansi_tm.tm_isdst;

        return _etsDate;
    }


    etsDate etsNormToDate(etsNorm ts)
    {
        epicsTimeStamp ets;
        ets.secPastEpoch=ts.secPastEpoch;
        ets.nsec=ts.nsec;
        epicsTime time(ets);
        local_tm_nano_sec local = (local_tm_nano_sec) time;
        _etsDate.year = local.ansi_tm.tm_year + 1900;
        _etsDate.mon = local.ansi_tm.tm_mon  + 1;
        _etsDate.day = local.ansi_tm.tm_mday;
        _etsDate.hour = local.ansi_tm.tm_hour;
        _etsDate.min = local.ansi_tm.tm_min;
        _etsDate.sec = local.ansi_tm.tm_sec;
        _etsDate.nsec = (unsigned long) ts.nsec;

        _etsDate.wday  = local.ansi_tm.tm_wday;
        _etsDate.yday  = local.ansi_tm.tm_yday;
        _etsDate.isdst = local.ansi_tm.tm_isdst;

        return _etsDate;
    }


    etsDate unixTimeToDate(etsNorm ts)
    {
        time_t t= ts.secPastEpoch;// = time(NULL);

        struct tm * local;
        local=localtime(&t);
        //local_tm_nano_sec local = (local_tm_nano_sec) time;
        _etsDate.year = local->tm_year +1900;
        _etsDate.mon = local->tm_mon  + 1;
        _etsDate.day = local->tm_mday;
        _etsDate.hour = local->tm_hour;
        _etsDate.min = local->tm_min;
        _etsDate.sec = local->tm_sec;
        _etsDate.nsec = (unsigned long) ts.nsec;

        _etsDate.wday  = local->tm_wday;
        _etsDate.yday  = local->tm_yday;
        _etsDate.isdst = local->tm_isdst;

        return _etsDate;
    }


    std::string etsDateAsString(etsNorm ts)
    {

        time_t t= ts.secPastEpoch;

        struct tm * local;
        local=localtime(&t);
        char buf[40];
        strftime (buf,80,"%b %d, %Y %T.",local);
        std::string date=(std::string) buf;
        char buft[10];
        sprintf(buft,"%u",ts.nsec);
        date.append((std::string) buft);


        return date;

    }


    //Deprecated!!!
    int getPulseID(unsigned int h, unsigned int &pulseID)
    {
        epicsTimeStamp ts;
        int status=getTimeStamp(h, ts);
        if (status==ICAFE_NORMAL)
        {
            std::string nsS = static_cast<std::ostringstream*>( &(std::ostringstream() << ts.nsec) )->str();
            int l=nsS.length();
            int startPos=std::max(l-6,0);
            std::string pidS = nsS.substr(startPos,std::min(6,l));
            if ( ! (std::istringstream(pidS) >> pulseID) ) pulseID = 0;
        }
        return status;
    }

    unsigned int getPulseIDFromTS(epicsTimeStamp ts)
    {
        unsigned int pulseID;
        std::string nsS = static_cast<std::ostringstream*>( &(std::ostringstream() << ts.nsec) )->str();
        int l=nsS.length();
        int startPos=std::max(l-6,0);
        std::string pidS = nsS.substr(startPos,std::min(6,l));
        if ( ! (std::istringstream(pidS) >> pulseID) ) pulseID = 0;
        return pulseID;
    }

    bool hasAlarmStatusSeverity(unsigned int h);
    int  getAlarmStatusSeverity(unsigned int  h, dbr_short_t as[2]);
    int  getAlarmStatusSeverityAsString(unsigned int  h, std::string asas[2]);
    unsigned int  getNoHandles();
    unsigned int  getNextFreeHandle();

    bool hasDescription(unsigned int _handle);

    int getDescription(unsigned int _handle, std::string & desc);
    int getUnits    (unsigned int h,  std::string & units);
    int getPrecision(unsigned int h, short &precision);


    bool isChannelConnected(unsigned int handle);
    bool allChannelsConnected();
    bool areChannelsConnected(unsigned int * handleArray, const unsigned int nHandles);
    bool areChannelsConnectedV(std::vector<unsigned int>);
    bool allChannelsWithinGroupConnected();
    bool allChannelsWithinGroupConnectedV(std::vector<unsigned int>);

    int  printHandle (unsigned int  h);
    int  printHandlesV(std::vector<unsigned int> handleV);
    int  printHandles(unsigned int * handleArray, unsigned int nHandles);
    unsigned int  printHandles();
    unsigned int  printChannels()
    {
        return printHandles();
    };
    unsigned int  printDisconnectedHandles();
    unsigned int  printDisconnectedChannels()
    {
        return printDisconnectedChannels();
    };
    unsigned int  getDisconnectedHandles(std::vector<unsigned int> &, std::vector<std::string> &);
    unsigned int  getConnectedHandles(std::vector<unsigned int> &, std::vector<std::string> &);
    unsigned int  getHandles(std::vector<unsigned int> &, std::vector<std::string> &);
    unsigned int  getHandleStates(std::vector<unsigned int> &, std::vector<std::string> &, std::vector<unsigned short> &);
    unsigned int  printMonitors();

    std::vector<unsigned int> getHandlesWithMonitors();
    int  getMonitorHandlesAndActions(std::vector<unsigned int> & handleV, std::vector<std::string> & actionV);

    int  setCafeDbrTypeV(std::vector<unsigned int>  _handleV, CAFENUM::DBR_TYPE cdt);
    int  setCafeDbrType(unsigned int  _handle, CAFENUM::DBR_TYPE cdt);
    int  getCafeDbrType(unsigned int  _handle, CAFENUM::DBR_TYPE &cdt);

    bool isEnum(unsigned int  _handle);
    short  getEnumFromString(unsigned int  _handle, std::string enumStringValue);
    std::string getStringFromEnum(unsigned int  _handle, unsigned short enumValue);
    std::vector<std::string> getEnumStrings(unsigned int handle);

    int  getDataTypeNative (unsigned int  _handle, chtype &ndt);
    int  getDataTypeRequest(unsigned int  _handle, chtype &rdt);

    int  eraseMonitorAction(unsigned int  _handle);
    int  clearMonitorAction(unsigned int  _handle);

    int  clearMonitorAction();

    int  addMonitorAction(unsigned int  _handle, std::string mAction);
    int  getMonitorAction(unsigned int  _handle, std::vector<std::string> &msV);
    std::vector<std::string> getMonitorAction(bool onlyIfNewData); //all handles; false gives all

    int updateMonitorPolicyDeltaMS(unsigned int _handle, unsigned int _monid, 
				   unsigned short deltaMS);

    int  getMonitorPolicyVector(unsigned int  _handle, std::vector<MonitorPolicy> &mpV);
    int  getMonitorPolicyInWaitingVector(unsigned int  _handle, std::vector<MonitorPolicy> &mpV);
    int  getNmonitor(unsigned int _handle);
    int  getNmonitorData(unsigned int _handle);
    int  getNmonitorCtrl(unsigned int _handle);
    std::vector<unsigned int> getMonitorIDs(unsigned int _handle);
    std::vector<unsigned int> getMonitorIDsInWaiting(unsigned int _handle);

    unsigned int getUsrArgsAsUInt(unsigned int _handle); //From  c.channelRequestMetaData.usrArg
    chtype getDataTypeCB(unsigned int _handle);
    chtype getDbrDataTypeCB(unsigned int _handle);
    CAFENUM::DBR_TYPE getCafeDbrTypeCB(unsigned int _handle);

    //setNoElements
    int     setNelem (); //All handles to native
    unsigned int     setNelem (unsigned int  _handle); //To Native
    unsigned int     setNelemToNative (unsigned int  _handle)
    {
        return setNelem(_handle);
    }; //To Native
    unsigned int     setNelem (unsigned int  _handle, unsigned int  _nelem);
    //For Arrays:
    unsigned int     setNelemToRetrieveFromCache (unsigned int  _handle);
    unsigned int     setNelemToRetrieveFromCache (unsigned int  _handle, unsigned int  _nelem);
    unsigned int     setNelemToRetrieveFromCtrlCache (unsigned int  _handle);
    unsigned int     setNelemToRetrieveFromCtrlCache (unsigned int  _handle, unsigned int  _nelem);
    unsigned int     getNelemToRetrieveFromCache (unsigned int  _handle);
    unsigned int     getNelemToRetrieveFromCtrlCache (unsigned int  _handle);

    unsigned int     getNelemClient  (unsigned int  _handle);
    unsigned int     getNelemNative  (unsigned int  _handle);
    unsigned int     getNelemRequest (unsigned int  _handle);

    int  getNelem(unsigned int  _handle, unsigned int  &c, unsigned int  &n,  unsigned int  &r);

    //Ctrl
    unsigned int     setNelemCtrl (unsigned int  _handle, unsigned int  _nelem);
    unsigned int     getNelemRequestCtrl (unsigned int _handle);
    unsigned int     getNelemClientCtrl  (unsigned int _handle);
    //setOffSet
    unsigned int     setOffset(unsigned int  _handle, unsigned int  _offset);
    unsigned int     getOffset(unsigned int  _handle);
    unsigned int     getOffsetLast(unsigned int  _handle);

    void    setFirstAndLastArrayElements(unsigned int  _handle,
                                         unsigned int  _start, unsigned int  _last)
    {
        if (_last > _start )
        {
            setOffset(_handle, _start);
            setNelem(_handle, _last);
        }
        else
        {
            std::cout << " offset must be less than the nelements" <<std::endl;
        };
    };


    //long    setOffsetCtrl(unsigned int  _handle, unsigned int  _offset);

    //bs read
    int setMapPulseIDBufferSize(unsigned int _handle, unsigned short _bufferSize);
    int setMapPulseIDBufferSizeAll(unsigned short _bufferSize);

    //Groups
    unsigned int  getGroupHandleFromGroupName(const char * _groupName)
    {
        ca_client_context * ccc= ca_current_context();
        return getGroupHandleFromGroupName(_groupName, ccc);
    }

    unsigned int  getGroupHandleFromGroupName(const char * _groupName, ca_client_context * ccc);
    std::string getGroupNameFromGroupHandle(unsigned int  groupHandle);

    unsigned int getGroupNPV(unsigned int gHandle);
    unsigned int getGroupNPV(const char * _groupName, ca_client_context * ccc);
    unsigned int getGroupNPV(const char * _groupName)
    {
        ca_client_context * ccc= ca_current_context();
        return getGroupNPV(_groupName, ccc);
    }

};
#endif
