///
/// \file    cafeCache.h
/// \brief   CAFE methods retrieving data from cache. (Belongs to cafe.h)
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef CAFECACHE_H
#define CAFECACHE_H


//No longer required - get from TS
/*
int getPulseID(unsigned int * handle, unsigned int  nelem, unsigned int * pulseID) {
    int status=ICAFE_NORMAL; int statusLocal=ICAFE_NORMAL;
    for (unsigned int i=0; i<nelem; ++i) {
        statusLocal=handleHelper.getPulseID(handle[i], pulseID[i]);
        if (statusLocal!=ICAFE_NORMAL && status==ICAFE_NORMAL) {
            status=statusLocal;
        }
    }
    return status;
}

int getPulseID(unsigned int handle, unsigned int &pulseID) {
    pulseID=0;
    return handleHelper.getPulseID(handle, pulseID);
}

int getPulseID(char * pv, unsigned int &pulseID) {
    pulseID=0;
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return handleHelper.getPulseID(handle, pulseID);
}

unsigned int getPulseIDFromTS(epicsTimeStamp ts) {
    return handleHelper.getPulseIDFromTS(ts);
}

std::vector<unsigned int>getPulseIDFromTS(PVDataHolder * pvd, unsigned int nelem) {
    std::vector<unsigned int> vui;
    vui.clear();
    vui.reserve(nelem);
    for (unsigned int i=0; i<nelem; ++i) {
        vui.push_back( getPulseIDFromTS(pvd[i].getEpicsTimeStamp()));
    }
    return vui;
}

std::vector<unsigned int>getPulseIDFromTS(PVGroup pvg) {
    std::vector<unsigned int> vui;
    vui.clear();
    vui.reserve(pvg.getNPV());
    PVDataHolder * pvd=pvg.getPVData();
    for (unsigned int i=0; i<pvg.getNPV(); ++i) {
        vui.push_back( getPulseIDFromTS(pvd[i].getEpicsTimeStamp()));
    }
    return vui;
}
*/

/*
int  getCachePVArray (std::vector<unsigned int> handleV, PVDataHolder * pvd){
	unsigned int * handleArray = new unsigned int [handleV.size()];
	for (size_t i=0; i< (size_t) handleV.size(); ++i) {handleArray[i]=  (unsigned int) handleV[i];}
	status=getCache(handleArray, (unsigned int) handleV.size(), pvd);
	delete [] handleArray; return status;
};
*/


int  getCachePVArray (std::vector<unsigned int> handleV, PVDataHolder * pvd)
{
    return getCache(&handleV[0], (unsigned int) handleV.size(), pvd);
};

int  getCachePVArrayNoWait (std::vector<unsigned int> handleV, PVDataHolder * pvd)
{

    int overallStatus=ICAFE_NORMAL;
    for (size_t i=0; i< (size_t) handleV.size(); ++i)
    {
        status=getCacheNoWait(handleV[i], pvd[i]);
	//std::cout << "handle= " << handleV[i] << " pvFromHandle= " << CAFE::getPVFromHandle(handleV[i]) << " status= " << pvd[i].getStatus() << " val= " << pvd[i].getAsString() << std::endl;
        if (status!=ICAFE_NORMAL)
        {
            resetCallbackGet(handleV[i]);
            if(overallStatus==ICAFE_NORMAL)
            {
                overallStatus=status;
            }
        }
        if (pvd[i].getStatus() !=ICAFE_NORMAL)
        {
            if(overallStatus==ICAFE_NORMAL)
            {
                overallStatus=pvd[i].getStatus();
            }
        }
    }
    return overallStatus;
};

/*
int  getCachePVArrayNoWait (std::vector<unsigned int> handleV, PVDataHolder * pvd){
	unsigned int * handleArray = new unsigned int [handleV.size()];
	int overallStatus=ICAFE_NORMAL;
	for (size_t i=0; i< (size_t) handleV.size(); ++i) {handleArray[i]=  (unsigned int) handleV[i];
		status=getCacheNoWait(handleArray[i], pvd[i]);
		if (status!=ICAFE_NORMAL) {
			resetCallbackGet(handleArray[i]);
			if(overallStatus==ICAFE_NORMAL) {overallStatus=status;}
		}
	}
	delete [] handleArray; return status;
};
*/

int  resetCallbackGet(const unsigned int handle);
int  resetCallbackPut(const unsigned int handle);

int  getCacheNoWait(const unsigned int  handle, PVDataHolder & pvd);

int  getCacheFromPulseID (const unsigned int  handle, const unsigned long long globalPulseID, PVDataHolder & pvd);

int  getCache (const unsigned int  handle, PVDataHolder & pvd);
int  getCache (const unsigned int  *handleArray, unsigned int  nelem, PVDataHolder * pvd);

int  getCtrlCache (const unsigned int  handle, PVCtrlHolder & pvc);
int  getCtrlCache (const unsigned int  *handleArray, unsigned int  nelem, PVCtrlHolder * pvc);

int  getCache    (const char * pv, PVDataHolder & pvd)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return getCache(handle, pvd);
}

int  getCtrlCache    (const char * pv, PVCtrlHolder & pvc)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return getCtrlCache(handle, pvc);
}

//6
int  getCacheDoubleArray(const unsigned int  handle, dbr_double_t * _val)
{
    return cafeDoppio.getCache(handle, DBR_DOUBLE, _val);
};
int  getCache(const unsigned int  handle, dbr_double_t * _val)
{
    return cafeDoppio.getCache(handle, DBR_DOUBLE, _val);
};
int  getCache(const unsigned int  handle, dbr_double_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    return cafeDoppio.getCache(handle, DBR_STS_DOUBLE, _val, alarmStatus, alarmSeverity);
};
int  getCache(const unsigned int  handle, dbr_double_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    return cafeDoppio.getCache(handle, DBR_TIME_DOUBLE, _val, alarmStatus, alarmSeverity, ts);
};
//5+ long long
int  getCache(const unsigned int  handle, long long * _val);
int  getCache(const unsigned int  handle, long long * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);
int  getCache(const unsigned int  handle, long long * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);

//5
int  getCacheLongArray(const unsigned int  handle, dbr_long_t * _val)
{
    return cafeLatte.getCache(handle, DBR_LONG, _val);
};
int  getCache(const unsigned int  handle, dbr_long_t * _val)
{
    return cafeLatte.getCache(handle, DBR_LONG, _val);
};
int  getCache(const unsigned int  handle, dbr_long_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    return cafeLatte.getCache(handle, DBR_STS_LONG, _val, alarmStatus, alarmSeverity);
};
int  getCache(const unsigned int  handle, dbr_long_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    return cafeLatte.getCache(handle, DBR_TIME_LONG, _val, alarmStatus, alarmSeverity, ts);
};
//4
int  getCache(const unsigned int  handle, dbr_char_t * _val)
{
    return cafeCappuccino.getCache(handle, DBR_CHAR, _val);
};
int  getCache(const unsigned int  handle, dbr_char_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    return cafeCappuccino.getCache(handle, DBR_STS_ENUM, _val, alarmStatus, alarmSeverity);
};
int  getCache(const unsigned int  handle, dbr_char_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    return cafeCappuccino.getCache(handle, DBR_TIME_ENUM, _val, alarmStatus, alarmSeverity, ts);
};
//3
int  getCache(const unsigned int  handle, dbr_enum_t * _val)
{
    return cafeEspresso.getCache(handle, DBR_ENUM, _val);
};
int  getCache(const unsigned int  handle, dbr_enum_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    return cafeEspresso.getCache(handle, DBR_STS_ENUM, _val, alarmStatus, alarmSeverity);
};
int  getCache(const unsigned int  handle, dbr_enum_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    return cafeEspresso.getCache(handle, DBR_TIME_ENUM, _val, alarmStatus, alarmSeverity, ts);
};
//2
int  getCacheFloatArray(const unsigned int  handle, dbr_float_t * _val)
{
    return cafeFrappuccino.getCache(handle, DBR_FLOAT, _val);
};
int  getCache(const unsigned int  handle, dbr_float_t * _val)
{
    return cafeFrappuccino.getCache(handle, DBR_FLOAT, _val);
};
int  getCache(const unsigned int  handle, dbr_float_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    return cafeFrappuccino.getCache(handle, DBR_STS_FLOAT, _val, alarmStatus, alarmSeverity);
};
int  getCache(const unsigned int  handle, dbr_float_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    return cafeFrappuccino.getCache(handle, DBR_TIME_FLOAT, _val, alarmStatus, alarmSeverity, ts);
};
//1
int  getCacheShortArray(const unsigned int  handle, dbr_short_t * _val)
{
    return cafeSchale.getCache(handle, DBR_SHORT, _val);
};
int  getCache(const unsigned int  handle, dbr_short_t * _val)
{
    return cafeSchale.getCache(handle, DBR_SHORT, _val);
};
int  getCache(const unsigned int  handle, dbr_short_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    return cafeSchale.getCache(handle, DBR_STS_SHORT, _val, alarmStatus, alarmSeverity);
};
int  getCache(const unsigned int  handle, dbr_short_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    return cafeSchale.getCache(handle, DBR_TIME_SHORT, _val, alarmStatus, alarmSeverity, ts);
};
int getCacheDbrStringArray(const unsigned int  handle, dbr_string_t * _val)
{
    return cafeSoluble.getCache(handle, DBR_STRING, _val);
};

//0
int  getCache(const unsigned int  handle, dbr_string_t * _val)
{
    return cafeSoluble.getCache(handle, DBR_STRING, _val);
};
int  getCache(const unsigned int  handle, dbr_string_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    return cafeSoluble.getCache(handle, DBR_STS_STRING, _val, alarmStatus, alarmSeverity);
};
int  getCache(const unsigned int  handle, dbr_string_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    return cafeSoluble.getCache(handle, DBR_TIME_STRING, _val, alarmStatus, alarmSeverity, ts);
}
//0+
int  getCacheStringArray(const unsigned int  handle, std::string * valStr)
{
    dbr_string_t * _val = new dbr_string_t[handleHelper.getNelemRequest(handle)];
    status=cafeSoluble.getCache(handle, DBR_STRING, _val);
    for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
    {
        valStr[i]=_val[i];
    }
    delete [] _val;
    return status;
};
int  getCache(const unsigned int  handle, std::string * valStr)
{
    dbr_string_t * _val = new dbr_string_t[handleHelper.getNelemRequest(handle)];
    status=cafeSoluble.getCache(handle, DBR_STRING, _val);
    for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
    {
        valStr[i]=_val[i];
    }
    delete [] _val;
    return status;
};
int  getCache(const unsigned int  handle, std::string * valStr,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    dbr_string_t * _val = new dbr_string_t[handleHelper.getNelemRequest(handle)];
    status=cafeSoluble.getCache(handle, DBR_STS_STRING, _val, alarmStatus, alarmSeverity);
    for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
    {
        valStr[i]=_val[i];
    }
    delete [] _val;
    return status;
};
int  getCache(const unsigned int  handle, std::string * valStr,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    dbr_string_t * _val = new dbr_string_t[handleHelper.getNelemRequest(handle)];
    status=cafeSoluble.getCache(handle, DBR_TIME_STRING, _val, alarmStatus, alarmSeverity, ts);
    for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
    {
        valStr[i]=_val[i];
    }
    delete [] _val;
    return status;
}
//Single value
//0+
int  getCacheString(const unsigned int handle, std::string  & valStr)  //0
{
    unsigned int  nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]= {""};
    status=cafeSoluble.getCache(handle, DBR_STRING, val);
    if (status==ICAFE_NORMAL)
    {
        valStr=val[0];
    }
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}

int  getCache(const unsigned int handle, std::string  & valStr)  //0
{
    unsigned int  nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]= {""};
    status=cafeSoluble.getCache(handle, DBR_STRING, val);
    if (status==ICAFE_NORMAL)
    {
        valStr=val[0];
    }
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
int  getCache(const unsigned int handle, std::string  & valStr, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)  //0
{
    unsigned int  nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]= {""};
    status=cafeSoluble.getCache(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
    if (status==ICAFE_NORMAL)
    {
        valStr=val[0];
    }
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
int  getCache(const unsigned int handle, std::string  & valStr,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //0
{
    unsigned int  nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]= {""};
    status=cafeSoluble.getCache(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
    if (status==ICAFE_NORMAL)
    {
        valStr=val[0];
    }
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
int  getCache(const unsigned int handle, dbr_string_t  & val); //0
int  getCache(const unsigned int handle, dbr_string_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //0
int  getCache(const unsigned int handle, dbr_string_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //0
int  getCache(const unsigned int handle, dbr_short_t  & val); //1
int  getCache(const unsigned int handle, dbr_short_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //1
int  getCache(const unsigned int handle, dbr_short_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //1
int  getCache(const unsigned int handle, dbr_float_t  & val); //2
int  getCache(const unsigned int handle, dbr_float_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //2
int  getCache(const unsigned int handle, dbr_float_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //2
int  getCache(const unsigned int handle, dbr_enum_t  & val); //3
int  getCache(const unsigned int handle, dbr_enum_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //3
int  getCache(const unsigned int handle, dbr_enum_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //3
int  getCache(const unsigned int handle, dbr_char_t  & val); //4
int  getCache(const unsigned int handle, dbr_char_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //4
int  getCache(const unsigned int handle, dbr_char_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //4

int  getCacheLong(const unsigned int handle, dbr_long_t  & val)
{
    return getCache(handle, val);
}

int  getCache(const unsigned int handle, dbr_long_t  & val); //5
int  getCache(const unsigned int handle, dbr_long_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //5
int  getCache(const unsigned int handle, dbr_long_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //5
int  getCache(const unsigned int handle, long long  & val); //5+
int  getCache(const unsigned int handle, long long  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //5+
int  getCache(const unsigned int handle, long long  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //5+
int  getCacheDouble(const unsigned int handle, dbr_double_t  & val)
{
    return getCache(handle, val);
}

int  getCache(const unsigned int handle, dbr_double_t  & val);  //6
int  getCache(const unsigned int handle, dbr_double_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);  //6
int  getCache(const unsigned int handle, dbr_double_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //6


//getCache by array of handles
//0+
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  std::string * valStr, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts)
{
    dbr_string_t * val = new dbr_string_t[nelem];
    status=cafeSoluble.getCache(handleArray, nelem, DBR_TIME_STRING, val, statusArray,
                                alarmStatus, alarmSeverity, ts);
    for (unsigned int i=0; i< nelem; ++i)
    {
        valStr[i]=val[i];
    }
    delete [] val;
    return status;
};
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  std::string * valStr, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity)
{
    dbr_string_t * val = new dbr_string_t[nelem];
    status=cafeSoluble.getCache(handleArray, nelem, DBR_STS_STRING, val, statusArray,
                                alarmStatus, alarmSeverity);
    for (unsigned int i=0; i< nelem; ++i)
    {
        valStr[i]=val[i];
    }
    delete [] val;
    return status;
};
int  getCache (const unsigned int  *handleArray, unsigned int  nelem, std::string * valStr, int  *statusArray)
{
    dbr_string_t * val = new dbr_string_t[nelem];
    status=cafeSoluble.getCache(handleArray, nelem, DBR_STRING, val, statusArray);
    for (unsigned int i=0; i< nelem; ++i)
    {
        valStr[i]=val[i];
    }
    delete [] val;
    return status;
}
//0
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_string_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts)
{
    return cafeSoluble.getCache(handleArray, nelem, DBR_TIME_STRING, val, statusArray,
                                alarmStatus, alarmSeverity, ts);
};
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_string_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity)
{
    return cafeSoluble.getCache(handleArray, nelem, DBR_STS_STRING, val, statusArray,
                                alarmStatus, alarmSeverity);
};
int  getCache (const unsigned int  *handleArray, unsigned int  nelem, dbr_string_t * val, int  *statusArray)
{
    return cafeSoluble.getCache(handleArray, nelem, DBR_STRING, val, statusArray);
}
//1
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_short_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts)
{
    return cafeSchale.getCache(handleArray, nelem, DBR_TIME_SHORT, val, statusArray,
                               alarmStatus, alarmSeverity, ts);
};
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_short_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity)
{
    return cafeSchale.getCache(handleArray, nelem, DBR_STS_SHORT, val, statusArray,
                               alarmStatus, alarmSeverity);
};
int  getCache (const unsigned int  *handleArray, unsigned int  nelem, dbr_short_t * val, int  *statusArray)
{
    return cafeSchale.getCache(handleArray, nelem, DBR_SHORT, val, statusArray);
}
//2
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_float_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts)
{
    return cafeFrappuccino.getCache(handleArray, nelem, DBR_TIME_FLOAT, val, statusArray,
                                    alarmStatus, alarmSeverity, ts);
};
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_float_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity)
{
    return cafeFrappuccino.getCache(handleArray, nelem, DBR_STS_FLOAT, val, statusArray,
                                    alarmStatus, alarmSeverity);
};
int  getCache (const unsigned int  *handleArray, unsigned int  nelem, dbr_float_t * val, int  *statusArray)
{
    return cafeFrappuccino.getCache(handleArray, nelem, DBR_FLOAT, val, statusArray);
}
//3
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_ushort_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts)
{
    return cafeEspresso.getCache(handleArray, nelem, DBR_TIME_ENUM, val, statusArray,
                                 alarmStatus, alarmSeverity, ts);
};
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_ushort_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity)
{
    return cafeEspresso.getCache(handleArray, nelem, DBR_STS_ENUM, val, statusArray,
                                 alarmStatus, alarmSeverity);
};
int  getCache (const unsigned int  *handleArray, unsigned int  nelem, dbr_ushort_t * val, int  *statusArray)
{
    return cafeEspresso.getCache(handleArray, nelem, DBR_ENUM, val, statusArray);
}
//4
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_char_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts)
{
    return cafeCappuccino.getCache(handleArray, nelem, DBR_TIME_CHAR, val, statusArray,
                                   alarmStatus, alarmSeverity, ts);
};
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_char_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity)
{
    return cafeCappuccino.getCache(handleArray, nelem, DBR_STS_CHAR, val, statusArray,
                                   alarmStatus, alarmSeverity);
};
int  getCache (const unsigned int  *handleArray, unsigned int  nelem, dbr_char_t * val, int  *statusArray)
{
    return cafeCappuccino.getCache(handleArray, nelem, DBR_CHAR, val, statusArray);
}
//5
int  getCache(unsigned int  *handleArray, unsigned int  nelem,  dbr_long_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts)
{
    return cafeLatte.getCache(handleArray, nelem, DBR_TIME_LONG, val, statusArray,
                              alarmStatus, alarmSeverity, ts);
};
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_long_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity)
{
    return cafeLatte.getCache(handleArray, nelem, DBR_STS_LONG, val, statusArray,
                              alarmStatus, alarmSeverity);
};
int  getCache (const unsigned int  *handleArray, unsigned int  nelem, dbr_long_t * val, int  *statusArray)
{
    return cafeLatte.getCache(handleArray, nelem, DBR_LONG, val, statusArray);
}
//5+ long long
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  long long * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts);

int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  long long * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity);

int  getCache (const unsigned int  *handleArray, unsigned int  nelem, long long * val, int  *statusArray);
//6
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_double_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts)
{
    return cafeDoppio.getCache(handleArray, nelem, DBR_TIME_DOUBLE, val, statusArray,
                               alarmStatus, alarmSeverity, ts);
};
int  getCache(const unsigned int  *handleArray, unsigned int  nelem,  dbr_double_t * val, int  *statusArray,
              dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity)
{
    return cafeDoppio.getCache(handleArray, nelem, DBR_STS_DOUBLE, val, statusArray,
                               alarmStatus, alarmSeverity);
};
int  getCache (const unsigned int  *handleArray, unsigned int  nelem, dbr_double_t * val, int  *statusArray)
{
    return cafeDoppio.getCache(handleArray, nelem, DBR_DOUBLE, val, statusArray);
}

////////////////////////////////////////////// by PV name ///////////////////////////////////

int  getCache(const char * pv, std::string  & valStr); //0+
int  getCache(const char * pv, std::string  & valStr, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //0+
int  getCache(const char * pv, std::string  & valStr,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //0+
int  getCache(const char * pv, dbr_string_t  & val); //0
int  getCache(const char * pv, dbr_string_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //0
int  getCache(const char * pv, dbr_string_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //0
int  getCache(const char * pv, dbr_short_t  & val); //1
int  getCache(const char * pv, dbr_short_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //1
int  getCache(const char * pv, dbr_short_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //1
int  getCache(const char * pv, dbr_float_t  & val); //2
int  getCache(const char * pv, dbr_float_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //2
int  getCache(const char * pv, dbr_float_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //2
int  getCache(const char * pv, dbr_enum_t  & val); //3
int  getCache(const char * pv, dbr_enum_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //3
int  getCache(const char * pv, dbr_enum_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //3
int  getCache(const char * pv, dbr_char_t  & val); //4
int  getCache(const char * pv, dbr_char_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //4
int  getCache(const char * pv, dbr_char_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //4
int  getCache(const char * pv, dbr_long_t  & val); //5
int  getCache(const char * pv, dbr_long_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity); //5
int  getCache(const char * pv, dbr_long_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //5
int  getCache(const char * pv, long long  & val)  //5+
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return CAFE::getCache(handle, val);
}
int  getCache(const char * pv, long long  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)  //5+
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return CAFE::getCache(handle, val, alarmStatus, alarmSeverity);
}
int  getCache(const char * pv, long long  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //5+
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return CAFE::getCache(handle, val, alarmStatus, alarmSeverity, ts);
}
int  getCache(const char * pv, dbr_double_t  & val);  //6
int  getCache(const char * pv, dbr_double_t  & val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);  //6
int  getCache(const char * pv, dbr_double_t  & val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts); //6


//6
int  getCache(const char * pv, dbr_double_t * _val)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeDoppio.getCache(handle, DBR_DOUBLE, _val);
};
int  getCache(const char * pv, dbr_double_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeDoppio.getCache(handle, DBR_STS_DOUBLE, _val, alarmStatus, alarmSeverity);
};
int  getCache(const char * pv, dbr_double_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeDoppio.getCache(handle, DBR_TIME_DOUBLE, _val, alarmStatus, alarmSeverity, ts);
};

//5+
int  getCache(const char * pv, long long * _val)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return CAFE::getCache(handle, _val);
};
int  getCache(const char * pv, long long * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return CAFE::getCache(handle, _val, alarmStatus, alarmSeverity);
};
int  getCache(const char * pv, long long * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return CAFE::getCache(handle, _val, alarmStatus, alarmSeverity, ts);
};

//5
int  getCache(const char * pv, dbr_long_t * _val)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeLatte.getCache(handle, DBR_LONG, _val);
};
int  getCache(const char * pv, dbr_long_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeLatte.getCache(handle, DBR_STS_LONG, _val, alarmStatus, alarmSeverity);
};
int  getCache(const char * pv, dbr_long_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeLatte.getCache(handle, DBR_TIME_LONG, _val, alarmStatus, alarmSeverity, ts);
};
//4
int  getCache(const char * pv, dbr_char_t * _val)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeCappuccino.getCache(handle, DBR_CHAR, _val);
};
int  getCache(const char * pv, dbr_char_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeCappuccino.getCache(handle, DBR_STS_ENUM, _val, alarmStatus, alarmSeverity);
};
int  getCache(const char * pv, dbr_char_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeCappuccino.getCache(handle, DBR_TIME_ENUM, _val, alarmStatus, alarmSeverity, ts);
};
//3
int  getCache(const char * pv, dbr_enum_t * _val)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeEspresso.getCache(handle, DBR_ENUM, _val);
};
int  getCache(const char * pv, dbr_enum_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeEspresso.getCache(handle, DBR_STS_ENUM, _val, alarmStatus, alarmSeverity);
};
int  getCache(const char * pv, dbr_enum_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeEspresso.getCache(handle, DBR_TIME_ENUM, _val, alarmStatus, alarmSeverity, ts);
};
//2
int  getCache(const char * pv, dbr_float_t * _val)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeFrappuccino.getCache(handle, DBR_FLOAT, _val);
};
int  getCache(const char * pv, dbr_float_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeFrappuccino.getCache(handle, DBR_STS_FLOAT, _val, alarmStatus, alarmSeverity);
};
int  getCache(const char * pv, dbr_float_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeFrappuccino.getCache(handle, DBR_TIME_FLOAT, _val, alarmStatus, alarmSeverity, ts);
};
//1
int  getCache(const char * pv, dbr_short_t * _val)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeSchale.getCache(handle, DBR_SHORT, _val);
};
int  getCache(const char * pv, dbr_short_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeSchale.getCache(handle, DBR_STS_SHORT, _val, alarmStatus, alarmSeverity);
};
int  getCache(const char * pv, dbr_short_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeSchale.getCache(handle, DBR_TIME_SHORT, _val, alarmStatus, alarmSeverity, ts);
};
//0
int  getCache(const char * pv, dbr_string_t * _val)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeSoluble.getCache(handle, DBR_STRING, _val);
};
int  getCache(const char * pv, dbr_string_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeSoluble.getCache(handle, DBR_STS_STRING, _val, alarmStatus, alarmSeverity);
};
int  getCache(const char * pv, dbr_string_t * _val,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    return cafeSoluble.getCache(handle, DBR_TIME_STRING, _val, alarmStatus, alarmSeverity, ts);
}
//0+
int  getCache(const char * pv, std::string * valStr)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    dbr_string_t * _val = new dbr_string_t[handleHelper.getNelemRequest(handle)];
    status=cafeSoluble.getCache(handle, DBR_STRING, _val);
    for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
    {
        valStr[i]=_val[i];
    }
    delete [] _val;
    return status;
};
int  getCache(const char * pv, std::string * valStr,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    dbr_string_t * _val = new dbr_string_t[handleHelper.getNelemRequest(handle)];
    status=cafeSoluble.getCache(handle, DBR_STS_STRING, _val, alarmStatus, alarmSeverity);
    for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
    {
        valStr[i]=_val[i];
    }
    delete [] _val;
    return status;
};
int  getCache(const char * pv, std::string * valStr,  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
    unsigned int handle=handleHelper.getHandleFromPV(pv);
    dbr_string_t * _val = new dbr_string_t[handleHelper.getNelemRequest(handle)];
    status=cafeSoluble.getCache(handle, DBR_TIME_STRING, _val, alarmStatus, alarmSeverity, ts);
    for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
    {
        valStr[i]=_val[i];
    }
    delete [] _val;
    return status;
}

#endif // CAFECACHE_H
