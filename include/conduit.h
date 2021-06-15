///
/// \file    conduit.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef CONDUIT_H
#define CONDUIT_H

#include <iostream>
#include <vector>
#include <config.h>
#include <cadef.h>

#include <channelRegalia.h>
#include <PVDataHolder.h>
#include <PVCtrlHolder.h>
#include <policies.h>
#include <deque>


#if HAVE_PYTHON_H
#if HAVE_PYCAFE_EXT
#include <Python.h> //required for PyCafe.h
#include <PyCafe.h>
#else
#include <PyCafe_api.h>
#endif
#endif

using namespace boost::posix_time;

/**
 *   Principal constructor has:\n
 *   \param _pv         process variable
 *   \param _ccc        ca_client_context
 */
class Conduit
{
public:
    friend struct change_alarmStatus;
    friend struct change_alarmSeverity;
    friend struct change_epicsTimeStamp;

    friend struct change_accessRightsHandlerArgs;
    friend struct change_accessRead;
    friend struct change_accessWrite;

    friend struct change_beamEventNo;
 
    friend struct change_channelDeviceAttribute;
    friend struct change_channelID;
    friend struct change_channelRegalia;

    friend struct change_channelGetCacheWaitPolicy;
    friend struct change_channelGetActionWhenMonitorPolicy;

    friend struct change_channelRequestDataTypePolicy;

    friend struct change_channelRequestMetaCtrl;
    friend struct change_channelRequestMetaCtrlClient;

    friend struct change_channelRequestMetaData;
    friend struct change_channelRequestMetaDataClient;

    friend struct change_channelRequestMetaPrimitive;

    friend struct change_channelRequestPolicyPut;
    friend struct change_channelRequestStatusPut;

    friend struct change_channelRequestPolicyGet;
    friend struct change_channelRequestStatusGet;

    friend struct change_channelRequestPolicyGetCtrl;
    friend struct change_channelRequestStatusGetCtrl;

    friend struct change_channelRequestStatusGetSTSACK;
    friend struct change_channelRequestStatusGetClassName;

    friend struct change_channelTimeoutPolicyGet;
    friend struct change_channelTimeoutPolicyPut;

    friend struct change_connectionHandlerArgs;

    friend struct change_dataBufferSize_PRIMITIVE;
    friend struct change_dataBufferSize_TIME;
    friend struct change_dataBufferSize_CTRL;
    friend struct change_dataBufferSize_STSACK;

    //friend struct change_dataBufferPVCtrlHolder;
    //friend struct change_dataBufferPVDataHolder;

    friend struct change_eventHandlerArgs;
    friend struct change_hasNewData;
    friend struct change_dequePulseID;
    friend struct change_mapPulseID;
    friend struct change_mapPulseIDBufferSize;
    friend struct change_monitorAction;
    friend struct change_monitorActionClear;
    friend struct change_monitorActionErase;

    friend struct change_monitorPolicy;

    friend struct change_monitorPolicyErase;
    friend struct change_monitorPolicyInsert;

    friend struct change_monitorPolicyInWaitingErase;
    friend struct change_monitorPolicyInWaitingInsert;

    //friend struct change_rule;

    friend struct change_pvAlias;
#if HAVE_PYTHON_H
    friend struct change_pyOpenCallbackFlag;
#endif

    friend struct change_status;

    friend struct change_supplementHandle;
    friend struct change_supplementDescription;
    friend struct change_supplementAlarmSeverity;
    friend struct change_usrArgs;

    friend struct change_widgetInsert;
    friend struct change_widgetErase;
 
    friend struct change_pyGetCallbackFn;
    friend struct change_pyPutCallbackFn;
    friend struct change_pyConnectCallbackFn;

    friend struct free_dataBuffers;

    friend class  Connect;
    friend class  CAFE;
    friend class  Granules;
    friend class  ChannelCreatePolicy;
    friend class  ConduitGroup;
    friend class  MonitorPolicy;

    //friend void CAFE_CALLBACK::PyHandlerPut;
    //friend void CAFE_CALLBACK::PyHandlerGet;
    //friend struct change_pyCafeFlag;

private:
    //from alarmString.h
    const char * epicsAlarmSeverityStrings[ALARM_SEVERITY_STRING_LENGTH];
    const char * epicsAlarmConditionStrings[ALARM_STATUS_STRING_LENGTH];

    static unsigned int  handleNext;

    ca_client_context * ccc;

    union db_access_val * dataBuffer;
    union db_access_val * ctrlBuffer;
    union db_access_val * putBuffer;
    union db_access_val * stsackBuffer;

    //struct connection_handler_args connectionHandlerArgs;
    //struct event_handler_args eventHandlerArgs;

    short alarmStatus;
    short alarmSeverity;
    epicsTimeStamp ts;

    //New Oct. 2018
    std::string desc;

    alarmSeverityStruct aSevStruct;
    bool hasDesc;
    bool hasAlarmSevStruct;

    void * usrArgs; //Filled in conduitEventHandlerArgs.h; used by getUsrArgsAsUInt in CyCafe

    chtype dataType;
    chtype dbrDataType;
    CAFENUM::DBR_TYPE cafeDbrType;

    //Reserved
    unsigned long long  beamEventNo;
  
    std:: deque<PVDataHolder> dequePulseID;

    std::map<unsigned long long, PVDataHolder> mapPulseID;
    std::map<unsigned long long, PVDataHolder>::iterator mpos;
    unsigned short mapPulseIDBufferSize;

    //bool rule;
    bool pyCafeFlag;

    std::string deviceAttributeDeliminator;
    ChannelDeviceAttribute channelDeviceAttribute;

    //PVCtrlHolder pvc;
    //PVDataHolder pvd;

    ChannelEnumStrings  channelEnumStrings;

    //ChannelPolicies

    ChannelGetCacheWaitPolicy channelGetCacheWaitPolicy;
    ChannelGetActionWhenMonitorPolicy channelGetActionWhenMonitorPolicy;

    ChannelRequestDataTypePolicy    channelRequestDataTypePolicy;

    ChannelRequestPolicy      channelRequestPolicyPut;
    ChannelRequestStatus      channelRequestStatusPut;

    ChannelRequestPolicy      channelRequestPolicyGet;       //DATA
    ChannelRequestStatus      channelRequestStatusGet;       //DATA

    ChannelRequestPolicy      channelRequestPolicyGetCtrl;   //CTRL
    ChannelRequestStatus      channelRequestStatusGetCtrl;   //CTRL

    ChannelRequestStatus      channelRequestStatusGetSTSACK;   //STSACK
    ChannelRequestStatus      channelRequestStatusGetClassName;//ClassName

    ChannelTimeoutPolicy      channelTimeoutPolicyGet;
    ChannelTimeoutPolicy      channelTimeoutPolicyPut;

    ChannelRegalia                   channelRegalia;

    ChannelRequestMetaData           channelRequestMetaData; //0 container for actual send
    ChannelRequestMetaDataClient     channelRequestMetaDataClient; //-1 //DATA
    //ChannelRequestMetaDataRepository channelRequestMetaDataRepository;  //  (CAFENUM::DBR_TYPE DBR_TIME) ; //1

    ChannelRequestMetaData           channelRequestMetaCtrl; //0 container for actual send
    ChannelRequestMetaDataClient     channelRequestMetaCtrlClient; //-1 //CTRL
    //ChannelRequestMetaDataRepository channelRequestMetaCtrlRepository;  //   (CAFENUM::DBR_TYPE DBR_CTRL);  //1

    ChannelRequestMetaData  channelRequestMetaSTSACK;//  (CAFENUM::DBR_TYPE DBR_STSACK);//1
    ChannelRequestMetaData  channelRequestMetaPrimitive; //Put operations

#if HAVE_PYTHON_H   
    //void * PyEventHandler() const;
    //void * PyEventHandler(unsigned int) const;
    //void * PyDataEventHandler() const;
    //void * PyCtrlEventHandler() const;

    void * CyMonitorHandler() const; //can handle all three cb signatures
 
    //void * CyHandleHandler() const; //handle
    //void * CyEventHandler() const;  //handle, pv
    //void * CyDataEventHandler() const; //handle, pv, pvdata
    void * CyCtrlEventHandler() const; //handle, pv, pvctrl

    bool   pyOpenCallbackFlag;

    void * pyGetCallbackFn;
    void * pyPutCallbackFn;
    void * pyConnectCallbackFn;
    std::vector<void *> pyConnectCallbackVector;

#endif

    int  putWithCallback(pCallback callbackHandlerPut) const;

    int  put(void) const;
    int  get(void) const;
    int  getWithCallback(pCallback) const;
    int  getCtrl(void) const;
    int  getCtrlWithCallback(pCallback) const;
    int  getSTSACKWithCallback(pCallback) const;
    int  getClassNameWithCallback(pCallback) const;

    //Monitor Policies
    //map<evid,MonitorPolicy> emp;
    //map<evid,MonitorPolicy>::iterator iemp;
    //map<unsigned long,MonitorPolicy> lump;
    //map<unsigned long,MonitorPolicy>::iterator ilump;



    ////MonitorPolicy mpBase;
    std::vector<MonitorPolicy> mpV;
    std::vector<MonitorPolicy> mpInWaitingV;

    int  monitorStart(MonitorPolicy &mp) const;
    int  monitorStop(evid eventID) const;

    std::vector<std::string> monitorAction;

    std::vector<void *> widgetV;


    bool hasNewData; // used by HandleHelper.getMonitorAction();

public:

#if HAVE_PYTHON_H
    void * PyGetHandler() const;
    void * PyPutHandler() const;
    bool getPyOpenCallbackFlag() const
    {
        return pyOpenCallbackFlag;
    }
    //void * PyOpenHandler() const; //make public so that it can be called from a callback fn
    void * PyConnectHandler() const; //make public so that it can be called from a callback fn
   

    void * getPyGetCallbackFn(void) const 
    {
        return pyGetCallbackFn;
    };
    void * getPyPutCallbackFn(void) const 
    {
        return pyPutCallbackFn;
    };
    void * getPyConnectCallbackFn(void) const 
    {
        return pyConnectCallbackFn;
    };

    std::vector<void *> getPyConnectCallbackVector(void) const 
    {
        return pyConnectCallbackVector;
    };

#endif

    Conduit(void );

    Conduit(const char  * _pv, ca_client_context *_ccc,
            ChannelRequestPolicy _channelRequestPolicyPut, ChannelRequestPolicy _channelRequestPolicyGet,
            ChannelGetActionWhenMonitorPolicy _channelGetActionWhenMonitorPolicy, bool _pyCafeFlag);

    //Conduit(const char  * _pv, ca_client_context *_ccc, bool _pyCafeFlag);
    //Conduit(const char  * _pv, ca_client_context *_ccc);
    virtual ~Conduit();
    unsigned int  groupHandle;  // Group handle this pv handle belongs to!!
    unsigned int  handle;

    chid channelID;
    //evid eventID;

    std::string pv;
    std::string pvAlias;

    int  status;

    friend std::ostream& operator<<(std::ostream& os, const Conduit& e)
    {
        os<< "handle="  << e.handle<<" pv=" << e.pv<<  std::endl;
        return os;
    };

    //struct connection_handler_args  getConnectionHandlerArgs(void) const{
    //  return connectionHandlerArgs;
    //};

   
    //struct event_handler_args  getEventHandlerArgs(void) const {
    //  return eventHandlerArgs;
    //}; 

    bool getPyCafe() const
    {
        return pyCafeFlag;
    };


    bool operator<(const Conduit& c)const
    {
        return handle<c.handle;
    };

    const char * getDevice(void) const
    {
        return channelDeviceAttribute.getDevice();
    };
    const char * getAttribute(void) const
    {
        return channelDeviceAttribute.getAttribute();
    };

    const char * getClassName(void) const
    {
        return channelRegalia.className;
    };

    const char * getHostName(void) const
    {
        return channelRegalia.hostName;
    };
    bool isConnected(void) const
    {
        return channelRegalia.connectFlag;
    };

    std::string getDescription(void) const
    {
        return desc;
    }

    epicsTimeStamp getTimeStamp(void) const
    {
        return ts;
    }
    short getAlarmStatus(void) const
    {
        return alarmStatus;
    }
    short getAlarmSeverity(void) const
    {
        return alarmSeverity;
    }
    short getHHSV(void) const
    {
        return aSevStruct.hhsv;
    }
    short getHSV(void) const
    {
        return aSevStruct.hsv;
    }
    short getLSV(void) const
    {
        return aSevStruct.lsv;
    }
    short getLLSV(void) const
    {
        return aSevStruct.llsv;
    }
    alarmSeverityStruct getAlarmSeverityStruct(void) const
    {
        return aSevStruct;
    }

    bool hasDescription(void) const
    {
        return hasDesc;
    }

    bool hasAlarmSeverityStruct(void) const
    {
        return hasAlarmSevStruct;
    }

    bool hasAlarmSeverity(void) const
    {
        if (!hasAlarmSevStruct)
        {
            return false;
        }
        if (aSevStruct.hhsv>SEV_NO_ALARM && aSevStruct.hhsv<=SEV_INVALID)
        {
            return true;
        }
        else if (aSevStruct.hsv>SEV_NO_ALARM && aSevStruct.hsv<=SEV_INVALID)
        {
            return true;
        }
        else if (aSevStruct.lsv>SEV_NO_ALARM && aSevStruct.lsv<=SEV_INVALID)
        {
            return true;
        }
        else if (aSevStruct.llsv>SEV_NO_ALARM && aSevStruct.llsv<=SEV_INVALID)
        {
            return true;
        }
        return false;
    }

    std::string getAlarmStatusAsString(void) const
    {
        if (alarmStatus>-1 && alarmStatus<ALARM_STATUS_STRING_LENGTH)
        {
            return (std::string) epicsAlarmConditionStrings[alarmStatus];
        }
        else
        {
            std::cout << "alarmStatusValue=" << alarmStatus << " is not within the valid range of 0-3!" << std::endl;
            return (std::string) "ALARM_UNKNOWN";
        }
    }
    std::string getAlarmSeverityAsString(void) const
    {
        if (alarmSeverity>-1 && alarmSeverity<ALARM_SEVERITY_STRING_LENGTH)
        {
            return (std::string) epicsAlarmSeverityStrings[alarmSeverity];
        }
        else
        {
            std::cout << "alarmStatusSeverity=" << alarmSeverity << " is not within the valid range of 0-21!" << std::endl;
            return (std::string) "SEVERITY_UNKNOWN";
        }
    }


    unsigned short getAccessRead(void) const
    {
        return channelRegalia.accessRead;
    };
    unsigned short getAccessWrite(void) const
    {
        return channelRegalia.accessWrite;
    };
    //Reserved
    unsigned long long getBeamEventNo(void) const
    {
        return beamEventNo;
    };
    unsigned long long getPulseID(void) const
    {
        return beamEventNo;
    };

 
    /*
    PVDataHolder getPVDataFromPulseID(unsigned long long globalPulseID) const {
    PVDataHolder pvd;
    std::map<unsigned long long, PVDataHolder>::iterator mpos;

    mpos=mapPulseID.find(globalPulseID);

    	if (mpos != mapPulseID.end() ) {
    		pvd = (mpos->second);
    		//std::cout << "key: EXACT MATCH FOUND " << mpos->first << " pid  " << globalPulseID << std::endl;
    		return pvd;
    	}
    	else {getPVDataHolder(pvd);
    	return pvd;
    	}
    }
    */

    std::map<unsigned long long, PVDataHolder> getPulsePVDataMap() const
    {
        //std::map<unsigned long long, PVDataHolder> a; return a;};
        return mapPulseID;
    };
    unsigned short getMapPulseIDBufferSize() const
    {
        return mapPulseIDBufferSize;
    };

    std::deque<PVDataHolder> getPulsePVData() const
    {
        return dequePulseID;
    }


    chid getChannelID(void) const
    {
        return channelRegalia.channelID;
    };
    ChannelRegalia getChannelRegalia(void) const
    {
        return channelRegalia;
    };
    ca_client_context * getClientContext(void) const
    {
        return ccc;
    };
    unsigned int  getGroupHandle(void) const
    {
        return groupHandle;
    };
    unsigned int  getHandle(void)  const
    {
        return handle;
    };
    const char * getPV(void) const
    {
        return pv.c_str();
    };
    std::string  getPVAsString(void) const
    {
        return pv;
    };
    const char * getPVAlias(void) const
    {
        return pvAlias.c_str();
    };
    std::string  getPVAliasAsString(void) const
    {
        return pvAlias;
    };

    int  getStatus(void) const
    {
        return status;
    };

    void * getUsrArgs(void) const
    {
        return usrArgs;
    };
    chtype getDataType(void) const
    {
        return dataType;
    };
    chtype getDbrDataType(void) const
    {
        return dbrDataType;
    };
    CAFENUM::DBR_TYPE getCafeDbrType(void) const
    {
        return cafeDbrType;
    };

    db_access_val * getCtrlBuffer(void) const
    {
        return ctrlBuffer;
    };
    db_access_val * getDataBuffer(void) const
    {
        return dataBuffer;
    };
    db_access_val * getPutBuffer(void) const
    {
        return putBuffer;
    };

    int  getPVDataHolder(PVDataHolder &) const ;
    int  getPVCtrlHolder(PVCtrlHolder &) const ;


    std::string getUnits(void) const
    {
        PVCtrlHolder pvc;
        getPVCtrlHolder(pvc);
        return pvc.getUnitsAsString();
    };

    short getPrecision(void) const
    {
        PVCtrlHolder pvc;
        getPVCtrlHolder(pvc);
        return pvc.getPrecision();
    };

    //bool getRule(void) const {return rule;};

    //RequestPolicy
    ChannelGetCacheWaitPolicy getChannelGetCacheWaitPolicy(void) const
    {
        return channelGetCacheWaitPolicy;
    };
    ChannelGetActionWhenMonitorPolicy getChannelGetActionWhenMonitorPolicy(void) const
    {
        return channelGetActionWhenMonitorPolicy;
    }

    ChannelTimeoutPolicy getChannelTimeoutPolicyGet (void) const
    {
        return channelTimeoutPolicyGet;
    };
    ChannelRequestPolicy getChannelRequestPolicyGet (void) const
    {
        return channelRequestPolicyGet;
    };
    ChannelRequestStatus getChannelRequestStatusGet (void) const
    {
        return channelRequestStatusGet;
    };
    ChannelRequestPolicy getChannelRequestPolicyGetCtrl (void) const
    {
        return channelRequestPolicyGetCtrl;
    };
    ChannelRequestStatus getChannelRequestStatusGetCtrl (void) const
    {
        return channelRequestStatusGetCtrl;
    };

    ChannelRequestStatus getChannelRequestStatusGetSTSACK (void) const
    {
        return channelRequestStatusGetSTSACK;
    };
    ChannelRequestStatus getChannelRequestStatusGetClassName (void) const
    {
        return channelRequestStatusGetClassName;
    };

    ChannelTimeoutPolicy getChannelTimeoutPolicyPut (void) const
    {
        return channelTimeoutPolicyPut;
    };
    ChannelRequestPolicy getChannelRequestPolicyPut (void) const
    {
        return channelRequestPolicyPut;
    };
    ChannelRequestStatus getChannelRequestStatusPut (void) const
    {
        return channelRequestStatusPut;
    };

    ChannelRequestDataTypePolicy getChannelRequestDataTypePolicy(void) const
    {
        return channelRequestDataTypePolicy;
    };

    ChannelRequestMetaData       getChannelRequestMetaData(void) const
    {
        return channelRequestMetaData;
    };   //0
    ChannelRequestMetaDataClient getChannelRequestMetaDataClient(void) const
    {
        return channelRequestMetaDataClient;
    }; //-1
    ChannelRequestMetaData       getChannelRequestMetaCtrl(void) const
    {
        return channelRequestMetaCtrl;
    };   //0
    ChannelRequestMetaDataClient getChannelRequestMetaCtrlClient(void) const
    {
        return channelRequestMetaCtrlClient;
    }; //-1

    ChannelRequestMetaData       getChannelRequestMetaPrimitive(void) const
    {
        return channelRequestMetaPrimitive;
    };   //0

    //ChannelRequestMetaDataRepository       getChannelRequestMetaDataRepository  (void) const {return channelRequestMetaDataRepository;};   //1
    //ChannelRequestMetaDataRepository       getChannelRequestMetaCtrlRepository  (void) const {return channelRequestMetaCtrlRepository;};   //1
    //ChannelRequestMetaDataRepository       getChannelRequestMetaSTSACKRepository(void) const {return channelRequestMetaSTSACKRepository;}; //1
    //ChannelRequestMetaDataRepository       getChannelRequestMetaPrimitiveRepository(void) const {return channelRequestMetaPrimitiveRepository;}; //1

    ////MonitorPolicy getMonitorPolicy(void) const {return mpBase;};


    MonitorPolicy getMonitorPolicy(unsigned int monitor_id) const
    {
          
        bool mpidFound=false;
        
        //Iterate
        for (size_t i=0; i< mpV.size(); ++i)
        {         
	  if ( mpV[i].getID() == monitor_id)
            {
                mpidFound=true;
                return mpV[i];                
            }
          
        }

        if (!mpidFound)
        {
	    std::cout << __FILE__ << "//" << __LINE__ << std::endl;
	    std::cout << "getMonitorPolicyVector(monitor_id):  " << monitor_id << " NOT FOUND! " << std::endl;
            std::cout << "Could not modify entry!" << std::endl;
        }
    };

    std::vector<MonitorPolicy> getMonitorPolicyVector(void) const
    {
        return mpV;
    };


    std::vector<MonitorPolicy> getMonitorPolicyInWaitingVector(void) const
    {
        return mpInWaitingV;
    };

    std::vector<std::string> getMonitorAction(void) const
    {
        return monitorAction;
    };

    std::vector<void *> getWidgetV(void) const
    {
        return std::vector<void *>(widgetV);
    };

    bool getHasNewData(void) const
    {
        return hasNewData;
    };
};

#endif // CONDUIT_H
