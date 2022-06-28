///
/// \file    cafe.h
/// \author  Jan Chrin, PSI
/// \date    Release: June 2018
/// \version CAFE 1.6.0

#ifndef CAFE_H
#define CAFE_H


#include <hashConduit.h>
#include <connect.h>
#include <conduitEventHandlerArgs.h>
#include <instant.h>
#include <cafeDataType.h>

#if HAVE_PYTHON_H
#include <unicodeobject.h>
#endif

#if HAVE_ZEROMQ
#include <zbsDataHolders.h>
using namespace CAFEBS;
#endif

/**
* \class CAFE
* \brief Methods for synchronous and asynchronous interactions
*/
class CAFE : public Connect
{
private:
    Instant<dbr_string_t>   cafeSoluble;
    Instant<dbr_float_t>    cafeFrappuccino;
    Instant<dbr_short_t>    cafeSchale;
    Instant<dbr_enum_t>     cafeEspresso;
    Instant<dbr_char_t>     cafeCappuccino;
    Instant<dbr_long_t>     cafeLatte;
    Instant<dbr_double_t>   cafeDoppio;

    Transpose<dbr_string_t> renderString; // 0
    Transpose<dbr_short_t>  renderShort;  // 1
    Transpose<dbr_float_t>  renderFloat;  // 2
    Transpose<dbr_enum_t>   renderEnum;   // 3
    Transpose<dbr_char_t>   renderChar;   // 4
    Transpose<dbr_long_t>   renderLong;   // 5
    Transpose<dbr_double_t> renderDouble; // 6

    Granules cafeGranules;
    ChannelGetCacheWaitPolicy channelGetCacheWaitPolicy;
    ChannelRequestStatus  channelRequestStatusGet;
    ChannelRequestStatus  channelRequestStatusGetCtrl;
    ChannelRequestMetaDataClient channelRequestMetaDataClient; //used for PVDataHolder
    ChannelRequestMetaDataClient channelRequestMetaCtrlClient; //used for PVCtrlHolder
    CAFENUM::DBR_TYPE vectorDbrType;
    //Groups
    ChannelTimeoutPolicy channelTimeoutPolicySGGet;
    ChannelTimeoutPolicy channelTimeoutPolicySGPut;

public:
    CAFE()
    {
        vectorDbrType=CAFENUM::DBR_NONE; //cafeVectors.h
        //Put in default values
        //make no wait
        channelRequestPolicyMasterPut.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
                                                CAFENUM::NO_WAIT,  CAFENUM::WITH_CALLBACK_DEFAULT); //WITHOUT_CALLBACK)

        // channelRequestPolicyMasterGet.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
	//					CAFENUM::WAIT, CAFENUM::WITH_CALLBACK_DEFAULT); // WITHOUT_CALLBACK);
        //channelRequestPolicyMasterGetCtrl.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
        //	 CAFENUM::WAIT, CAFENUM::WITH_CALLBACK_DEFAULT); //WITH_CALLBACK_DEFAULT);
    };

    //28 May 2017
    CAFE(ChannelRequestPolicy _channelRequestPolicyMasterPut,
         ChannelRequestPolicy _channelRequestPolicyMasterGet,
         ChannelGetActionWhenMonitorPolicy _channelGetActionWhenMonitorPolicyMaster)
    {
        channelRequestPolicyMasterPut = _channelRequestPolicyMasterPut;
        channelRequestPolicyMasterGet = _channelRequestPolicyMasterGet;
        channelRequestPolicyMasterGetCtrl = _channelRequestPolicyMasterGet;
        channelGetActionWhenMonitorPolicyMaster = _channelGetActionWhenMonitorPolicyMaster;
        vectorDbrType=CAFENUM::DBR_NONE;
    }

    //28 May 2017
    CAFE(ChannelRequestPolicy _channelRequestPolicyMasterPut, ChannelRequestPolicy _channelRequestPolicyMasterGet)
    {
        channelRequestPolicyMasterPut = _channelRequestPolicyMasterPut;
        channelRequestPolicyMasterGet = _channelRequestPolicyMasterGet;
        channelRequestPolicyMasterGetCtrl = _channelRequestPolicyMasterGet;
        vectorDbrType=CAFENUM::DBR_NONE;
    }

    //28 May 2017
    CAFE(ChannelRequestPolicy _channelRequestPolicyMasterPut)
    {
        channelRequestPolicyMasterPut = _channelRequestPolicyMasterPut;
        //channelRequestPolicyMasterGet.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
        //						  CAFENUM::WAIT,  CAFENUM::WITH_CALLBACK_DEFAULT); //WITHOUT_CALLBACK);
        //channelRequestPolicyMasterGetCtrl.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
        //						  CAFENUM::WAIT, CAFENUM::WITH_CALLBACK_DEFAULT); //WITH_CALLBACK_DEFAULT);
        vectorDbrType=CAFENUM::DBR_NONE;
    }

#include <cafeCache.h>
#include <cafeVectors.h>
#include <cafeRoast.h>
    //#if HAVE_LIBQTXML
    //only if qt xml activated
#include <cafeXML.h>
    // if HAVE_LIBQTXML moved to within header file
    //#endif

    int wfExpress(const unsigned int handle,  const unsigned int nelem, dbr_char_t * chval);
    //int wfExpress(const unsigned int handle);

    //get NON_BLOCKING
    int getNonBlocking(const unsigned int  handle)
    {
        return get(handle);
    };
    int get      (const unsigned int  handle);
    int get      (const unsigned int  *handleArray, unsigned int nelem, int  *statusArray);
    int get      (const unsigned int  *handleArray, unsigned int nHandles, std::vector<int> &statusV);
    int getCtrl  (const unsigned int  handle);

    //get BLOCKING
    int  get      (const unsigned int  handle, PVDataHolder & pvd);
    int  get      (const unsigned int  *handleArray, unsigned int nelem, PVDataHolder * pvd);

    int waitForBundledEvents(std::vector<unsigned int> handleV, std::vector<int> &vRB);
    int waitForBundledEvents(unsigned int  *handleArray, unsigned int nHandles, std::vector<int> &vRB);
    int waitForGetEvent(unsigned int handle);

    int getCtrl  (const unsigned int  handle, PVCtrlHolder & pvc);
    int getCtrl  (const unsigned int  *handleArray, unsigned int nelem, PVCtrlHolder * pvc);

    //pvName
    //NON_BLOCKING
    int get      (const char * pv);
    int getCtrl  (const char * pv);

    //BLOCKING
    int get    (const char * pv, PVDataHolder & pvd);
    int getCtrl(const char * pv, PVCtrlHolder & pvc);

    //Special
    int getWFAsString(unsigned int handle, std::string & message);
    int getWFAsString(const char * pv, std::string & message)
    {
        unsigned int  handle;
        status=ICAFE_NORMAL;
        try
        {
            status = open(pv,  handle);
        }
        catch (CAFEException_open & e)
        {
            return e.pvEx.statusCode;
        }
        if (status == ICAFE_NORMAL)
        {
            status=getWFAsString(handle, message);
        }
        return status;
    }

    int getWFAsStringCache(unsigned int handle, std::string & message);
    int getWFAsStringCache(const char * pv, std::string & message)
    {
        unsigned int  handle;
        status=ICAFE_NORMAL;
        try
        {
            status = open(pv,  handle);
        }
        catch (CAFEException_open & e)
        {
            return e.pvEx.statusCode;
        }
        if (status == ICAFE_NORMAL)
        {
            status=getWFAsStringCache(handle, message);
        }
        return status;
    }

    std::string getChannelIDAsString(chid _chid)
    {
        std::stringstream ss;
        ss << _chid;
        return ss.str();
    }

    int  getAllChannelInfo(unsigned int handle, ChannelRegalia & channelInfo, PVCtrlHolder & pvc, PVDataHolder &pvd, std::string & desc);
    int  getChannelDataStore(unsigned int handle, ChannelDataStore & cds);

    int supplementHandles();
    int supplementHandlesV(std::vector<unsigned int> hV);
    int supplementHandle(unsigned int handle)
    {
        std::vector<unsigned int> hV;
        hV.push_back(handle);
        return supplementHandlesV(hV);
    }
    int addWidget(unsigned int _handle, void * widget)
    {
        return handleHelper.addWidget(_handle, widget);
    }

    int removeWidget (unsigned int _handle, void * widget)
    {
        return handleHelper.removeWidget(_handle, widget);
    }

    int getWidgets(unsigned int _handle, std::vector<void *> & widgetV)
    {
        return  handleHelper.getWidgets(_handle, widgetV);
    }

#if HAVE_ZEROMQ
    int calcDBPMStatus(int, int, std::string);
    int initBSwithCA(CAFEBS::BSDataHolder &bsd);

    int setBS2CAGroup(CAFEBS::BSDataHolder &bsd);
    int setBS2CA(CAFEBS::BSDataHolder &bsd);
    
    int setBS(CAFEBS::BSDataHolder &bsd);
    int getBS(CAFEBS::BSDataHolder &bsd);
    int closeBS(CAFEBS::BSDataHolder &bsd);
    int setPulseIDBufferSize(unsigned int _handle, unsigned short _bsize)
    {
        return handleHelper.setMapPulseIDBufferSize(_handle, _bsize);
    }
    int setPulseIDBufferSize(const char * _pv, unsigned short _bsize)
    {
        unsigned int _handle=handleHelper.getHandleFromPV(_pv);
        return handleHelper.setMapPulseIDBufferSize(_handle, _bsize);
    }
    int setPulseIDBufferSizeAll(unsigned short _bsize)
    {
        return handleHelper.setMapPulseIDBufferSizeAll(_bsize);
    }
    int getDBPM(DBPMKeeper &dbpm);
    int readDBPMOffsets(DBPMKeeper &dbpm);
#if HAVE_LIBQTXML
    int prepareDBPM(std::vector<std::string> &_glist, std::vector<unsigned int> &_hV, std::vector<std::string> &dev,  std::vector<float> &pos) ;
    int prepareDBPM(std::vector<std::string> &_glist, std::vector<unsigned int> &_hV, std::map<float, std::string> &posDev) ;
#endif
#endif
    //Standard BLOCKING get
    //0+
    int  get(const unsigned int  handle, std::string * valStr,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
    {
        unsigned int nn=handleHelper.getNelemClient(handle);
        if (nn==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        //check on handle number before proceeding!
        dbr_string_t * _val = new dbr_string_t[nn];
        status=cafeSoluble.get(handle, DBR_TIME_STRING, _val, alarmStatus, alarmSeverity, ts);
        for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
        {
            valStr[i]=_val[i];
        }
        delete [] _val;
        return status;
    };

    int  get(const unsigned int  handle, std::string * valStr, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
    {
        unsigned int nn=handleHelper.getNelemClient(handle);
        if (nn==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_string_t * _val = new dbr_string_t[nn];
        status=cafeSoluble.get(handle, DBR_STS_STRING, _val, alarmStatus, alarmSeverity);
        for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
        {
            valStr[i]=_val[i];
        }
        delete [] _val;
        return status;
    };

    int  get(const unsigned int  handle, std::string * valStr)
    {
        unsigned int nn=handleHelper.getNelemClient(handle);
        if (nn==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_string_t * _val = new dbr_string_t[nn];
        status=cafeSoluble.get(handle, DBR_STRING, _val);
        for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
        {
            valStr[i]=_val[i];
        }
        delete [] _val;
        return status;
    };

    //0
    int get(const unsigned int  handle, dbr_string_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
    {
        return cafeSoluble.get(handle, DBR_TIME_STRING, _val, alarmStatus, alarmSeverity, ts);
    };
    int get(const unsigned int  handle, dbr_string_t * _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
    {
        return cafeSoluble.get(handle, DBR_STS_STRING, _val, alarmStatus, alarmSeverity);
    };
    int get(const unsigned int  handle, dbr_string_t * _val)
    {
        return cafeSoluble.get(handle, DBR_STRING, _val);
    };
    //1
    int get(const unsigned int  handle, dbr_short_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
    {
        return cafeSchale.get (handle, DBR_TIME_SHORT, _val, alarmStatus, alarmSeverity, ts);
    };
    int get(const unsigned int  handle, dbr_short_t * _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
    {
        return cafeSchale.get (handle, DBR_STS_SHORT, _val, alarmStatus, alarmSeverity);
    };
    int get(const unsigned int  handle, dbr_short_t * _val)
    {
        return cafeSchale.get (handle, DBR_SHORT, _val);
    };
    //2
    int get(const unsigned int  handle, dbr_float_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
    {
        return cafeFrappuccino.get(handle, DBR_TIME_FLOAT, _val, alarmStatus, alarmSeverity, ts);
    };
    int get(const unsigned int  handle, dbr_float_t * _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
    {
        return cafeFrappuccino.get(handle, DBR_STS_FLOAT, _val, alarmStatus, alarmSeverity);
    };
    int get(const unsigned int  handle, dbr_float_t * _val)
    {
        return cafeFrappuccino.get (handle, DBR_FLOAT, _val);
    };
    //3
    int get(const unsigned int  handle, dbr_ushort_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
    {
        return cafeEspresso.get (handle, DBR_TIME_ENUM, _val, alarmStatus, alarmSeverity, ts);
    };
    int get(const unsigned int  handle, dbr_ushort_t * _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
    {
        return cafeEspresso.get (handle, DBR_STS_ENUM, _val, alarmStatus, alarmSeverity);
    };
    int get(const unsigned int  handle, dbr_ushort_t * _val)
    {
        return cafeEspresso.get (handle, DBR_ENUM, _val);
    };
//4
    int get(const unsigned int  handle, dbr_char_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
    {
        return cafeCappuccino.get (handle, DBR_TIME_CHAR, _val, alarmStatus, alarmSeverity, ts);
    };
    int get(const unsigned int  handle, dbr_char_t * _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
    {
        return cafeCappuccino.get (handle, DBR_STS_CHAR, _val, alarmStatus, alarmSeverity);
    };
    int get(const unsigned int  handle, dbr_char_t * _val)
    {
        return cafeCappuccino.get (handle, DBR_CHAR, _val);
    };
    //5
    int get(const unsigned int  handle, dbr_long_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
    {
        return cafeLatte.get (handle, DBR_TIME_LONG, _val, alarmStatus, alarmSeverity, ts);
    };
    int get(const unsigned int  handle, dbr_long_t * _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
    {
        return cafeLatte.get (handle, DBR_STS_LONG, _val, alarmStatus, alarmSeverity);
    };
    int get(const unsigned int  handle, dbr_long_t * _val)
    {
        return cafeLatte.get (handle, DBR_LONG, _val);
    };
    //5+ long long
    int get(const unsigned int  handle, long long   * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);
    int get(const unsigned int  handle, long long   * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);
    int get(const unsigned int  handle, long long   * _val);

    //6
    int get(const unsigned int handle, dbr_double_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
    {
        return cafeDoppio.get (handle, DBR_TIME_DOUBLE, _val, alarmStatus, alarmSeverity, ts);
    };
    int get(const unsigned int handle, dbr_double_t * _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
    {
        return cafeDoppio.get (handle, DBR_STS_DOUBLE, _val, alarmStatus, alarmSeverity);
    };
    int get(const unsigned int handle, dbr_double_t * _val)
    {
        return cafeDoppio.get (handle, DBR_DOUBLE, _val);
    };



    //Arrays -user allocates space 
    int getCharArray(const unsigned int handle, dbr_char_t * _val)
    {
        return cafeCappuccino.get (handle, DBR_CHAR, _val);
    };
    int getShortArray(const unsigned int handle, dbr_short_t * _val)
    {
        return cafeSchale.get (handle, DBR_SHORT, _val);
    };
    int getUShortArray(const unsigned int handle, dbr_ushort_t * _val)
    {
        return cafeEspresso.get (handle, DBR_ENUM, _val);
    };
    int getDoubleArray(const unsigned int handle, dbr_double_t * _val)
    {
        return cafeDoppio.get (handle, DBR_DOUBLE, _val);
    };
    int getFloatArray(const unsigned int handle, dbr_float_t * _val)
    {
        return cafeFrappuccino.get (handle, DBR_FLOAT, _val);
    };
    int getLongArray(const unsigned int handle, dbr_long_t * _val)
    {
        return cafeLatte.get (handle, DBR_LONG, _val);
    };
    int getLongLongArray(const unsigned int handle, long long * _val)
    {
        return get (handle, _val);
    };
    int getStringArray(const unsigned int handle, std::string * valStr)
    {
        return  get(handle, valStr);
    };
    int getDbrStringArray(const unsigned int  handle, dbr_string_t * _val)
    {
        return cafeSoluble.get(handle, DBR_STRING, _val);
    };


    //0
    //single values
    int getString(const unsigned int handle, std::string  & valStr)   //0+
    {
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_STRING, val);
        if (status==ICAFE_NORMAL)
        {
            valStr=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    };

    int get(const unsigned int handle, std::string  & valStr)   //0+
    {
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_STRING, val);
        if (status==ICAFE_NORMAL)
        {
            valStr=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    };

    int get(const unsigned int  handle, std::string  & valStr, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)  //0
    {
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            valStr=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    };

    int  getStringTS(const unsigned int  handle, std::string  & valStr,
                     dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //0
    {
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            valStr=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }

    int get(const unsigned int  handle, std::string  & valStr,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //0
    {
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            valStr=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }

    int get(const unsigned int handle, dbr_string_t  & _val)   //0
    {
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_STRING, val);
        if (status==ICAFE_NORMAL)
        {
            sprintf(_val, val[0]);
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }

    int get(const unsigned int  handle, dbr_string_t  & _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)  //0
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            sprintf(_val, val[0]);
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }

    int get(const unsigned int  handle, dbr_string_t  & _val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //0
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            sprintf(_val, val[0]);
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }

    int  get(const unsigned int  handle, dbr_short_t  & _val)  //1
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_short_t val[1]= {0};
        status=cafeSchale.get(handle, DBR_SHORT, val);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_short_t  & _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)  //1
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_short_t val[1]= {0};
        status=cafeSchale.get(handle, DBR_STS_SHORT, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_short_t  & _val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //1
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_short_t val[1]= {0};
        status=cafeSchale.get(handle, DBR_TIME_SHORT, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_float_t  &_val)  //2
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_float_t val[1]= {0};
        status=cafeFrappuccino.get(handle, DBR_FLOAT, val);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_float_t  &_val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)  //2
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_float_t val[1]= {0};
        status=cafeFrappuccino.get(handle, DBR_STS_FLOAT, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_float_t  & _val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //2
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_float_t val[1]= {0};
        status=cafeFrappuccino.get(handle, DBR_TIME_FLOAT, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_enum_t  & _val)  //3
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_enum_t val[1]= {0};
        status=cafeEspresso.get(handle, DBR_ENUM, val);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_enum_t  & _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)  //3
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_enum_t val[1]= {0};
        status=cafeEspresso.get(handle, DBR_STS_ENUM, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_enum_t  & _val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //3
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_enum_t val[1]= {0};
        status=cafeEspresso.get(handle, DBR_TIME_ENUM, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_char_t  & _val)  //4
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_char_t val[1]= {0};
        status=cafeCappuccino.get(handle, DBR_CHAR, val);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_char_t  & _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)  //4
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_char_t val[1]= {0};
        status=cafeCappuccino.get(handle, DBR_STS_CHAR, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_char_t  & _val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //4
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_char_t val[1]= {0};
        status=cafeCappuccino.get(handle, DBR_TIME_CHAR, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int getLong(const unsigned int  handle, dbr_long_t  & _val)  //5
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_long_t val[1]= {0};
        status=cafeLatte.get(handle, DBR_LONG, val);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_long_t  & _val)  //5
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_long_t val[1]= {0};
        status=cafeLatte.get(handle, DBR_LONG, val);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_long_t  & _val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)  //5
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_long_t val[1]= {0};
        status=cafeLatte.get(handle, DBR_STS_LONG, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_long_t  & _val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //5
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_long_t val[1]= {0};
        status=cafeLatte.get(handle, DBR_TIME_LONG, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }

    int get(const unsigned int  handle, long long  & _val)  //5+ long long
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        long long val[1]= {0};
        status=CAFE::get(handle, val);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, long long  & _val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)  //5+ long long
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        long long val[1]= {0};
        status=CAFE::get(handle, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, long long & _val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //5+ long long
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        long long val[1]= {0};
        status=CAFE::get(handle, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }

    int getDouble(const unsigned int  handle, dbr_double_t  & _val)   //6
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_double_t val[1]= {0};
        status=cafeDoppio.get(handle, DBR_DOUBLE, val);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_double_t  & _val)   //6
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_double_t val[1]= {0};
        status=cafeDoppio.get(handle, DBR_DOUBLE, val);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_double_t  & _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)   //6
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_double_t val[1]= {0};
        status=cafeDoppio.get(handle, DBR_STS_DOUBLE, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    int get(const unsigned int  handle, dbr_double_t  & _val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //6
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_double_t val[1]= {0};
        status=cafeDoppio.get(handle, DBR_TIME_DOUBLE, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val=val[0];
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }




    ////////////////////////////////////////////// by PV name ///////////////////////////////////

    int get(const char * pv, std::string  * valStr); //0+
    int get(const char * pv, std::string  * valStr, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//0+
    int get(const char * pv, std::string  * valStr,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //0+

    int get(const char * pv, dbr_string_t  * val); //0
    int get(const char * pv, dbr_string_t  * val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //0
    int get(const char * pv, dbr_string_t  * val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //0
    int get(const char * pv, dbr_short_t  * val);//1
    int get(const char * pv, dbr_short_t  * val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //1
    int get(const char * pv, dbr_short_t  * val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //1
    int get(const char * pv, dbr_float_t  * val);//2
    int get(const char * pv, dbr_float_t  * val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //2
    int get(const char * pv, dbr_float_t  * val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //2
    int get(const char * pv, dbr_enum_t  * val); //3
    int get(const char * pv, dbr_enum_t  * val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);  //3
    int get(const char * pv, dbr_enum_t  * val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //3
    int get(const char * pv, dbr_char_t  * val); //4
    int get(const char * pv, dbr_char_t  * val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //4
    int get(const char * pv, dbr_char_t  * val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //4
    int get(const char * pv, dbr_long_t  * val); //5
    int get(const char * pv, dbr_long_t  * val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //5
    int get(const char * pv, dbr_long_t  * val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //5

    int get(const char * pv, long long  * val);  //5+ long long
    int get(const char * pv, long long  * val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //5+ long long
    int get(const char * pv, long long  * val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //5+ long long

    int get(const char * pv, dbr_double_t  * val);//6
    int get(const char * pv, dbr_double_t  * val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);  //6
    int get(const char * pv, dbr_double_t  * val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //6


    int get(const char * pv, std::string  & valStr); //0+
    int get(const char * pv, std::string  & valStr, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //0
    int get(const char * pv, std::string  & valStr,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //0

    int get(const char * pv, dbr_string_t  & val); //0
    int get(const char * pv, dbr_string_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //0
    int get(const char * pv, dbr_string_t  & val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //0
    int get(const char * pv, dbr_short_t  & val); //1
    int get(const char * pv, dbr_short_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //1
    int get(const char * pv, dbr_short_t  & val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //1
    int get(const char * pv, dbr_float_t  & val); //2
    int get(const char * pv, dbr_float_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //2
    int get(const char * pv, dbr_float_t  & val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //2
    int get(const char * pv, dbr_enum_t  & val); //3
    int get(const char * pv, dbr_enum_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //3
    int get(const char * pv, dbr_enum_t  & val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //3
    int get(const char * pv, dbr_char_t  & val); //4
    int get(const char * pv, dbr_char_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //4
    int get(const char * pv, dbr_char_t  & val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //4

    int get(const char * pv, dbr_long_t  & val); //5
    int get(const char * pv, dbr_long_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //5
    int get(const char * pv, dbr_long_t  & val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //5

    int get(const char * pv, long long  & val); //5+ long long
    int get(const char * pv, long long  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //5+ long long
    int get(const char * pv, long long  & val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //5+ long long

    int get(const char * pv, dbr_double_t  & val);  //6
    int get(const char * pv, dbr_double_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);  //6
    int get(const char * pv, dbr_double_t  & val,
            dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //6


    /////////////////////////////////////////////////////////////////////////////////////////////

    int  waitForPut  (const unsigned int *h, const unsigned int nelem);

    /////////////////////////////////////////////////////////////////////////////////////////////


    int set(const unsigned int  handle, PVDataHolder & pvd); //Yes
    int set(const unsigned int  * handleArray, const unsigned int  nelem, PVDataHolder * pvd);
    int set(const char *, PVDataHolder & pvd);

    //0
    int set(const unsigned int  handle, const dbr_string_t * _val)
    {
        return cafeSoluble.set (handle, DBR_STRING, _val);
    };
    //1
    int set(const unsigned int  handle, const dbr_short_t * _val)
    {
        return cafeSchale.set (handle, DBR_SHORT, _val);
    };
    //2
    int set(const unsigned int  handle, const dbr_float_t * _val)
    {
        return cafeFrappuccino.set (handle, DBR_FLOAT, _val);
    };
    //3
    int set(const unsigned int  handle, const dbr_enum_t * _val)
    {
        return cafeEspresso.set (handle, DBR_ENUM, _val);
    };
    //4
    int set(const unsigned int  handle, const dbr_char_t * _val)
    {
        return cafeCappuccino.set (handle, DBR_CHAR, _val);
    };
    //5
    int set(const unsigned int  handle, const dbr_long_t * _val)
    {
        return cafeLatte.set (handle, DBR_LONG, _val);
    };
    //5+ long long
    int set(const unsigned int  handle, const long long * _val);
    //6
    int set(const unsigned int  handle, const dbr_double_t * _val)
    {
        return cafeDoppio.set (handle, DBR_DOUBLE, _val);
    };

    /* NOT REQUIRED 
    int setStringWF(const unsigned int  handle, std::string wfStr) {
      dbr_char_t * wfChar = new dbr_char_t[wfStr.size()];
      unsigned short iCount = 0;
      for (std::string::size_type i=0; i < wfStr.size(); ++i) {
	if (wfStr[i].compare("\\") == 0 ) {
	     wfChar[iCount] = '\n'; 
             ++i;
        } 
        else {
             wfChar[iCount] = wfStr[i] 
        }     
        ++iCount; 
      } 
      return cafeCappuccino.set (handle, DBR_CHAR, wfChar);
      delete [] wfChar;
    }
    */

    //set for PythonTypes
    //0
    int setStringArray(const unsigned int handle, const dbr_string_t * _val)
    {
        return cafeSoluble.set (handle, DBR_STRING, _val);
    };

    //4
    int setCharArray(const unsigned int handle, const dbr_char_t *  _val)
    {
        return cafeCappuccino.set (handle, DBR_CHAR, _val);
    };
    //5
    int setLongArray(const unsigned int handle, const dbr_long_t * _val)
    {
        return cafeLatte.set (handle, DBR_LONG, _val);
    };
    //6
    int setDoubleArray(const unsigned int handle, const dbr_double_t * _val)
    {
        return cafeDoppio.set (handle, DBR_DOUBLE, _val);
    };

    //0
    int setString(const unsigned int handle, const std::string  _val)
    {
        return set (handle, _val);
    };
    //1
    int setShort(const unsigned int  handle, const dbr_short_t  _val)
    {
        return set (handle, _val);
    };
    //2
    int setFloat(const unsigned int  handle, const dbr_float_t  _val)
    {
        return set (handle, _val);
    };
    //3
    int setUShort(const unsigned int  handle, const dbr_enum_t  _val)
    {
        return set (handle, _val);
    };
    //4
    int setChar(const unsigned int handle, const dbr_char_t   _val)
    {
        return set (handle,  _val);
    };
    //5
    int setLong(const unsigned int handle, const dbr_long_t  _val)
    {
        return set (handle, _val);
    };
    //5+
    int setLongLong(const unsigned int handle, const long long  _val)
    {
        return set (handle, _val);
    };
    //6
    int setDouble(const unsigned int handle, const dbr_double_t  _val)
    {
        return set (handle, _val);
    };


    //0
    int set(const unsigned int  handle, std::string _val)
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_string_t val[1];
        //remove leading and trailing blanks;
        char mess[MAX_STRING_SIZE];
        handleHelper.removeLeadingAndTrailingSpacesDbrString((char *) _val.c_str(),mess);
        sprintf(val[0],"%s",mess);

        status=cafeSoluble.set(handle, DBR_STRING, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    //0
    int set(const unsigned int  handle, const dbr_string_t _val)
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_string_t val[1];
        sprintf(val[0],"%s",_val);
        status=cafeSoluble.set(handle, DBR_STRING, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    //1
    int set(const unsigned int  handle, const dbr_short_t _val)
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_short_t val[1];
        val[0]=_val;
        status=cafeSchale.set(handle, DBR_SHORT, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    //2
    int set(const unsigned int  handle, const dbr_float_t _val)
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_float_t val[1];
        val[0]=_val;
        status=cafeFrappuccino.set(handle, DBR_FLOAT, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    //3
    int set(const unsigned int  handle, const dbr_enum_t _val)
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_enum_t val[1];
        val[0]=_val;
        status=cafeEspresso.set(handle, DBR_ENUM, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    //4
    int set(const unsigned int  handle, const dbr_char_t _val)
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_char_t val[1];
        val[0]=_val;
        status=cafeCappuccino.set(handle, DBR_CHAR, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    //5
    int set(const unsigned int  handle, const dbr_long_t _val)
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_long_t val[1];
        val[0]=_val;
        status=cafeLatte.set(handle, DBR_LONG, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    //5+ long long
    int set(const unsigned int  handle, const long long _val)
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        if (_val <= LONG_MAX)
        {
            dbr_long_t val[1];
            val[0]=_val;
            status=CAFE::set(handle,  val);
        }
        else
        {
            dbr_double_t val[1];
            val[0]=_val;
            status=CAFE::set(handle,  val);
        }
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }
    //6
    int set(const unsigned int  handle, const dbr_double_t _val)
    {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        if (nelemPrevious==0)
        {
            return ECAFE_INVALID_HANDLE;
        }
        dbr_double_t val[1];
        val[0]=_val;
        status=cafeDoppio.set(handle, DBR_DOUBLE, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
        return status;
    }


    //0  non-blocking, flush at end
    int set(const unsigned int  *handleArray,  const unsigned int  nelem, const dbr_string_t * _val,  int  *statusArray)
    {
        return cafeSoluble.set (handleArray, nelem, DBR_STRING, _val, statusArray);
    }
    //1  non-blocking, flush at end
    int set(const unsigned int  *handleArray, const unsigned int  nelem, const dbr_short_t * _val,  int  *statusArray)
    {
        return cafeSchale.set (handleArray, nelem, DBR_SHORT, _val, statusArray);
    }
    //2  non-blocking, flush at end
    int set(const unsigned int  *handleArray, const unsigned int  nelem, const dbr_float_t * _val,  int  *statusArray)
    {
        return cafeFrappuccino.set (handleArray, nelem, DBR_FLOAT, _val, statusArray);
    }
    //3  non-blocking, flush at end
    int set(const unsigned int  *handleArray, const unsigned int  nelem, const dbr_enum_t * _val,  int  *statusArray)
    {
        return cafeEspresso.set (handleArray, nelem, DBR_ENUM, _val, statusArray);
    }
    //4  non-blocking, flush at end
    int set(const unsigned int  *handleArray, const unsigned int  nelem, const dbr_char_t * _val,  int  *statusArray)
    {
        return cafeCappuccino.set (handleArray, nelem, DBR_CHAR, _val, statusArray);
    }
    //5  non-blocking, flush at end
    int set(const unsigned int  *handleArray, const unsigned int  nelem, const dbr_long_t * _val,  int  *statusArray)
    {
        return cafeLatte.set (handleArray, nelem, DBR_LONG, _val, statusArray);
    }
    //5+ long long  non-blocking, flush at end
    int set(const unsigned int  *handleArray, const unsigned int  nelem, const long long * _val,  int  *statusArray);

    //6  non-blocking, flush at end
    int set(const unsigned int  *handleArray,  const unsigned int  nelem, const dbr_double_t * _val,  int  *statusArray)
    {
        return cafeDoppio.set (handleArray, nelem, DBR_DOUBLE, _val, statusArray);
    }



    //0
    int set(const char *, const dbr_string_t * _val);
    //1
    int set(const char *, const dbr_short_t * _val);
    //2
    int set(const char *, const dbr_float_t * _val);
    //3
    int set(const char *, const dbr_enum_t * _val);
    //4
    int set(const char *, const dbr_char_t * _val);
    //5
    int set(const char *, const dbr_long_t * _val);
    //5+ long long
    int set(const char *, const long long * _val);
    //6
    int set(const char *, const dbr_double_t * _val);

    //0 string
    int set(const char * pv, const std::string _val);
    //0
    int set(const char *, const dbr_string_t _val);
    //1
    int set(const char *, const dbr_short_t  _val);
    //2
    int set(const char *, const dbr_float_t  _val);
    //3
    int set(const char *, const dbr_enum_t   _val);
    //4
    int set(const char *, const dbr_char_t   _val);
    //5
    int set(const char *, const dbr_long_t   _val);
    //5+ long long
    int set(const char *, const long long   _val);
    //6
    int set(const char *, const dbr_double_t _val);


    //helper functions for &val methods
    unsigned int setNelemToOne(const unsigned int handle);
    unsigned int setNelemToPrevious(const unsigned int handle, unsigned int nelemPrevious);

    unsigned int setNelemToRetrieveFromCacheToOne(const unsigned int handle);
    unsigned int setNelemToRetrieveFromCacheToPrevious(const unsigned int handle, unsigned int nelemPrevious);

    //      group functions
    int groupAttach (const unsigned int groupHandle, PVGroup &pvgroup);
    int groupFetch(const unsigned int groupHandle, PVGroup &pvgroup);

    int groupGet(PVGroup &pvgroup);
    int groupGet(const unsigned int groupHandle, PVGroup &pvgroup);

    int groupGetScalar(const unsigned int groupHandle, std::vector<std::string> &valV, std::vector<int> &statusV);
    int groupGetScalar(const unsigned int groupHandle, std::vector<double> &valV, std::vector<int> &statusV);
    int groupGetScalar(const unsigned int groupHandle, std::vector<float> &valV, std::vector<int> &statusV);
    int groupGetScalar(const unsigned int groupHandle, std::vector<short> &valV, std::vector<int> &statusV);
    int groupGetScalar(const unsigned int groupHandle, std::vector<long> &valV, std::vector<int> &statusV);
    int groupGetScalar(const unsigned int groupHandle, std::vector<long long> &valV, std::vector<int> &statusV);
    int groupGetScalar(const unsigned int groupHandle, std::vector<unsigned short> &valV, std::vector<int> &statusV);
    int groupGetScalar(const unsigned int groupHandle, std::vector<unsigned char> &valV, std::vector<int> &statusV);
    int groupGetScalar(const unsigned int groupHandle, std::vector<char> &valV, std::vector<int> &statusV);

    int groupGetScalar(const unsigned int groupHandle, std::string * val,int * statusArray );
    int groupGetScalar(const unsigned int groupHandle, double * val, int * statusArray);
    int groupGetScalar(const unsigned int groupHandle, float * val, int * statusArray);
    int groupGetScalar(const unsigned int groupHandle, short * val, int * statusArray);
    int groupGetScalar(const unsigned int groupHandle, long * val,int * statusArray );
    int groupGetScalar(const unsigned int groupHandle, long long * val, int * statusArray);
    int groupGetScalar(const unsigned int groupHandle, unsigned short * val, int * statusArray );
    int groupGetScalar(const unsigned int groupHandle, unsigned char * val,int * statusArray );
    int groupGetScalar(const unsigned int groupHandle, char * val,int * statusArray );

    int groupGetCache(const unsigned int groupHandle, PVGroup &pvgroup)
    {
        const unsigned int * handleArray = handleHelper.getHandlesFromWithinGroup(groupHandle);
        status=getCache (handleArray, pvgroup.getNPV(),  pvgroup.pvdata);
        pvgroup.statusGroup=status;
        return status;
    }

    //Closes channels but does not delete handle
    std::vector<unsigned int> closeDisconnectedChannelsFromWithinGroupV(const unsigned int groupHandle)
    {
        std::vector<unsigned int> dhV=handleHelper.getDisconnectedHandlesFromWithinGroupV(groupHandle);
        if (dhV.size() >0)
        {
            closeChannelsKeepHandles(dhV);
        }
        return dhV;
    }

    std::vector<unsigned int> getDisconnectedHandlesFromWithinGroupV(const unsigned int groupHandle)
    {
        return handleHelper.getDisconnectedHandlesFromWithinGroupV(groupHandle);
    }

    std::vector<unsigned int> getHandlesFromWithinGroupV(const unsigned int groupHandle)
    {
        return handleHelper.getHandlesFromWithinGroupV(groupHandle);
    }

    unsigned int * getHandlesFromWithinGroup(const unsigned int groupHandle)
    {
        return handleHelper.getHandlesFromWithinGroup(groupHandle);
    }

    int groupSet(PVGroup &pvgroup);
    int groupSet(const unsigned int groupHandle, PVGroup &pvgroup);

    int groupMonitorStart(const unsigned int  groupHandle, std::vector<int> &statusV)
    {
        std::vector<unsigned int> monitorIDV;
        return groupMonitorStart(groupHandle, statusV, monitorIDV);
    }

    int groupMonitorStart(const unsigned int  groupHandle, std::vector<int> &statusV, std::vector<MonitorPolicy> &mpV);
    int groupMonitorStart(const unsigned int  groupHandle, std::vector<int> &statusV, std::vector<unsigned int> &monitorIDV);
    int groupMonitorStop (const unsigned int  groupHandle, std::vector<int> &statusV);

    unsigned int groupGetNPV(unsigned int _gHandle)
    {
        return handleHelper.getGroupNPV(_gHandle);
    }
    unsigned int groupGetNPV(const char * _gName)
    {
        return handleHelper.getGroupNPV(_gName);
    }
    unsigned int groupGetNPV(const char * _gName, ca_client_context * ccc)
    {
        return handleHelper.getGroupNPV(_gName, ccc);
    }

#if HAVE_PYTHON_H
    void loadSFGroups(std::string suff);
#endif

};

#endif // CAFE_H

