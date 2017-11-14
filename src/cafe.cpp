///
/// \file    cafe.cc
/// \author  Jan Chrin, PSI
/// \date    Release: June, 2015
/// \version CAFE 1.0.0
///
///    Copyright 2014, Jan Chrin, PSI
/// 
///    This program is free software: it may be redistributed and modifified
///    under the terms of the GNU General Public License as published by
///    the Free Software Foundation, under License version 3 or later.
///
///    This program is distributed in the hope that it will be useful,
///    but WITHOUT ANY WARRANTY; without even the implied warranty of
///    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///    GNU General Public License for more details.
///
///    The GNU General Public License can be gotten from
///    http://www.gnu.org/licenses/.
///

#include "cafe.h"
#include "conduitGroup.h"
#include <set>


bool MUTEX=true;
bool CHECK_CONSISTENCY_CA_STATE=true;

epicsMutex  cafeMutex;

cafeConduit_set cs;
cafeGroup_set gs;

vector<PVGroup> PVGroupV;
vector<PVGroup> PVGroupPseudo;
vector<deviceCollection>  deviceCollectionV;

vector<string> globalChannelList;

map<vector<unsigned int>, string> groupPseudoMap;


#include "cafeEnum.h"
#include "enumStrings.h"


/**
 *  \brief Set an array of handles with long long(s)
 *  \param handleArray input: array of handles
 *  \param nelem input: number of elements defining size of array
 *  \param _val input: array of long longs
 *  \param statusArray output: array of corresponding status
 *  \return ECA_NORMAL if all OK else first ECAFE error encountered
 */
int  CAFE::set(const unsigned int *handleArray, const unsigned int nelem, const long long * _val,  int  *statusArray) {
   //what is the highest native type?
    chtype chtMax=DBF_CHAR;

    ChannelRegalia channelInfo;
    unsigned short nbyteChType=1;
    unsigned short nbyteMax=nbyteChType;

    for (unsigned int i=0; i<nelem; ++i) {
        CAFE::getChannelInfo(handleArray[i], channelInfo);

        switch (channelInfo.getDataType()) {
        case DBF_CHAR:
            nbyteChType=sizeof(dbr_char_t);
            break;
        case DBF_SHORT:
            nbyteChType=sizeof(dbr_short_t);
            break;
        case DBF_ENUM:
            nbyteChType=sizeof(dbr_enum_t);
            break;
        case DBF_LONG:
            nbyteChType=sizeof(dbr_long_t);
            break;
        case DBF_FLOAT:
            nbyteChType=sizeof(dbr_float_t);
            break;
        case DBF_DOUBLE:
            nbyteChType=sizeof(dbr_double_t);
            break;
        case DBF_STRING:
            nbyteChType=sizeof(dbr_string_t);
            break;
        default:
            nbyteChType=1;
        }


        if ( channelInfo.getCafeConnectionState() != ICAFE_CS_NEVER_CONN  && channelInfo.getCafeConnectionState() != ICAFE_CS_CLOSED ) {
            if (channelInfo.getDataType()==DBF_STRING) {
                chtMax=DBR_STRING;
                break;
            }

            if(nbyteChType>nbyteMax && channelInfo.getDataType()< CAFE_NO_ACCESS) {
                chtMax=channelInfo.getDataType();
                nbyteMax=4;
            }

        }
    }

    int  _status=ICAFE_NORMAL;

    if ( chtMax == DBF_STRING) {
        dbr_string_t * _sVal = new dbr_string_t[nelem];
        for (unsigned int  i=0; i < nelem; ++i) {
            std::stringstream ss;
            ss.clear();
            ss << _val[i];
            strcpy(_sVal[i] , ss.str().c_str());
        }
        _status=CAFE::set (handleArray, nelem, _sVal, statusArray);

        delete [] _sVal;
    }
    else if ( chtMax == DBF_DOUBLE) {
        dbr_double_t * _dVal = new dbr_double_t[nelem];
        for (unsigned int  i=0; i < nelem; ++i) {
         _dVal[i] = (double) _val[i];
        }
        _status=CAFE::set (handleArray, nelem, _dVal, statusArray);

        delete [] _dVal;
    }
    else if ( chtMax == DBF_FLOAT) {
        dbr_float_t * _fVal = new dbr_float_t[nelem];
        for (unsigned int  i=0; i < nelem; ++i) {
         _fVal[i] = (float) _val[i];
        }
        _status=CAFE::set (handleArray, nelem, _fVal, statusArray);

        delete [] _fVal;
    }
    else {
        dbr_long_t * _lVal = new dbr_long_t[nelem];
        for (unsigned int  i=0; i < nelem; ++i) {
         _lVal[i] = (dbr_long_t) _val[i];
        }
        _status=CAFE::set (handleArray, nelem, _lVal, statusArray);

        delete [] _lVal;
    }
    return _status;


}

//5+ long long
/**
 *  Get long long(s), alarms and timeStamp
 *  \param handle input: handle to conduit object
 *  \param _val output: array of long longs
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const unsigned int handle, long long * _val, \
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) {
#define __METHOD__ \
    "CAFE::get(const unsigned int handle, long long * _val,  \
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"

    ChannelRegalia channelInfo;
    CAFE::getChannelInfo(handle, channelInfo);
    if ( channelInfo.getCafeConnectionState() == ICAFE_CS_NEVER_CONN) {
        return ICAFE_CS_NEVER_CONN;
    }
		else if ( channelInfo.getCafeConnectionState()==ICAFE_CS_CLOSED)  {
        return ICAFE_CS_CLOSED;
    }
		
    status=ICAFE_NORMAL;
    unsigned int  nn=handleHelper.getNelemNative(handle);


    if ( channelInfo.getDataType() == DBR_STRING) {
        dbr_string_t * _sVal = new dbr_string_t[nn];
        status=cafeSoluble.get (handle, DBR_TIME_STRING, _sVal, alarmStatus, alarmSeverity, ts);
        if (status!=ICAFE_NORMAL) { delete [] _sVal; return status;}
        istringstream ss;

        for (unsigned short i=0; i<nn; ++i) {
            long long l=0;
            ss.clear();
            ss.str(_sVal[i]);
            ss>>l;

            if ( !ss.fail()) {
                _val[i] = (long  long) l;
                std::string strInput=_sVal[i];
                std::stringstream ssOut;
                ssOut << l;
                std::string strOutput=ssOut.str();
                if (strInput!=strOutput) {
                   cout << __METHOD__ << "//" << __LINE__ << endl;
                   cout << "***WARNING*** STRING TO LONG LONG CONVERSION REPORTS: " << endl;
                   cout << "STRING VALUE: " << strInput << " CONVERTED TO: " << strOutput << endl;
                }
            }
            else {
                cout << __METHOD__ << "//" << __LINE__ << endl;
                cout << "***WARNING*** NO STRING TO LONG LONG  CONVERSION for ELEMENT " << i \
                        << " of " << nn << " !! " << endl;
                cout << "***WARNING*** COULD NOT CONVERT: ";
                cout << _sVal[i];
                cout << " TO LONG LONG ; CONVERSION NOT MADE!" << endl;
                cout << "Returning value 0 for element " << i << endl;
                _val[i]=0;
            }
        }

        delete [] _sVal;
    }

    else if ( channelInfo.getDataType() == DBR_DOUBLE) {
        dbr_double_t * _dVal = new dbr_double_t[nn];
        status=cafeDoppio.get (handle, DBR_TIME_DOUBLE,  _dVal, alarmStatus, alarmSeverity, ts);
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _dVal[i];
        }
        delete [] _dVal;
    }
    else if ( channelInfo.getDataType() == DBR_FLOAT) {
        dbr_float_t * _fVal = new dbr_float_t[nn];
        status=cafeFrappuccino.get (handle, DBR_TIME_FLOAT,  _fVal, alarmStatus, alarmSeverity, ts);
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _fVal[i];
        }
        delete [] _fVal;
    }
    else {
        dbr_long_t * _lVal = new dbr_long_t[nn];
        status=cafeLatte.get (handle, DBR_TIME_LONG,  _lVal, alarmStatus, alarmSeverity, ts);
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _lVal[i];
        }
        delete [] _lVal;
    }
    return status;
#undef __METHOD__
};

/**
 *  Get long long(s) and alarms
 *  \param handle input: handle to conduit object
 *  \param _val output: array of long longs
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const unsigned int  handle, long long   * _val, \
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity) {
#define __METHOD__ \
"CAFE::get(unsigned int handle, long long * _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)"
    ChannelRegalia channelInfo;
    CAFE::getChannelInfo(handle, channelInfo);
    if ( channelInfo.getCafeConnectionState() == ICAFE_CS_NEVER_CONN) {
        return ICAFE_CS_NEVER_CONN;
    }
		else if ( channelInfo.getCafeConnectionState()==ICAFE_CS_CLOSED)  {
        return ICAFE_CS_CLOSED;
    }
    int  _status=ICAFE_NORMAL;
    unsigned int  nn=handleHelper.getNelemNative(handle);


    if ( channelInfo.getDataType() == DBR_STRING) {
        dbr_string_t * _sVal = new dbr_string_t[nn];
        _status=cafeSoluble.get (handle, DBR_STS_STRING,  _sVal, alarmStatus, alarmSeverity);
        if (_status!=ICAFE_NORMAL) { delete [] _sVal; return _status;}
        istringstream ss;

        for (unsigned short i=0; i<nn; ++i) {
            long long l=0;
            ss.clear();
            ss.str(_sVal[i]);
            ss>>l;

            if ( !ss.fail()) {
                _val[i] = (long  long) l;
                std::string strInput=_sVal[i];
                std::stringstream ssOut;
                ssOut << l;
                std::string strOutput=ssOut.str();
                if (strInput!=strOutput) {
                   cout << __METHOD__ << "//" << __LINE__ << endl;
                   cout << "***WARNING*** STRING TO LONG LONG CONVERSION REPORTS: " << endl;
                   cout << "STRING VALUE: " << strInput << " CONVERTED TO: " << strOutput << endl;
                }
            }
            else {
                cout << __METHOD__ << "//" << __LINE__ << endl;
                cout << "***WARNING*** NO STRING TO LONG LONG  CONVERSION for ELEMENT " << i \
                        << " of " << nn << " !! " << endl;
                cout << "***WARNING*** COULD NOT CONVERT: ";
                cout << _sVal[i];
                cout << " TO LONG LONG ; CONVERSION NOT MADE!" << endl;
                cout << "Returning value 0 for element " << i << endl;
                _val[i]=0;
            }
        }

        delete [] _sVal;
    }

    else if ( channelInfo.getDataType() == DBR_DOUBLE) {
        dbr_double_t * _dVal = new dbr_double_t[nn];
        _status=cafeDoppio.get (handle, DBR_STS_DOUBLE,  _dVal, alarmStatus, alarmSeverity);
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _dVal[i];
        }
        delete [] _dVal;
    }
    else if ( channelInfo.getDataType() == DBR_FLOAT) {
        dbr_float_t * _fVal = new dbr_float_t[nn];
        _status=cafeFrappuccino.get (handle, DBR_STS_FLOAT,  _fVal, alarmStatus, alarmSeverity);
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _fVal[i];
        }
        delete [] _fVal;
    }
    else {
        dbr_long_t * _lVal = new dbr_long_t[nn];
        _status=cafeLatte.get (handle, DBR_STS_LONG,  _lVal, alarmStatus, alarmSeverity);
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _lVal[i];
        }
        delete [] _lVal;
    }
    return _status;
 #undef __METHOD__
};

/**
 *  Get long long(s)
 *  \param handle input: handle
 *  \param _val output: array of long longs
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const unsigned int handle, long long * _val){
#define __METHOD__ \
    "CAFE::get(const unsigned int handle, long long * _val)"

    ChannelRegalia channelInfo;
    CAFE::getChannelInfo(handle, channelInfo);
    if ( channelInfo.getCafeConnectionState() == ICAFE_CS_NEVER_CONN) {
        return ICAFE_CS_NEVER_CONN;
    }
		else if ( channelInfo.getCafeConnectionState()==ICAFE_CS_CLOSED)  {
        return ICAFE_CS_CLOSED;
    }
    int  _status=ICAFE_NORMAL;
    unsigned int  nn=handleHelper.getNelemNative(handle);

    if ( channelInfo.getDataType() == DBR_STRING) {
        dbr_string_t * _sVal = new dbr_string_t[nn];
        _status=cafeSoluble.get (handle, DBR_STRING,  _sVal);
        if (_status!=ICAFE_NORMAL) { delete [] _sVal; return _status;}      
        istringstream ss;

        for (unsigned short i=0; i<nn; ++i) {
            long long l=0;
            ss.clear();
            ss.str(_sVal[i]);
            ss>>l;

            if ( !ss.fail()) {
                _val[i] = (long  long) l;
                std::string strInput=_sVal[i];
                std::stringstream ssOut;
                ssOut << l;
                std::string strOutput=ssOut.str();
                if (strInput!=strOutput) {
                   cout << __METHOD__ << "//" << __LINE__ << endl;
                   cout << "***WARNING*** STRING TO LONG LONG CONVERSION REPORTS: " << endl;
                   cout << "STRING VALUE: " << strInput << " CONVERTED TO: " << strOutput << endl;
                }
            }
            else {
                cout << __METHOD__ << "//" << __LINE__ << endl;
                cout << "***WARNING*** NO STRING TO LONG LONG  CONVERSION for ELEMENT " << i \
                        << " of " << nn << " !! " << endl;
                cout << "***WARNING*** COULD NOT CONVERT: ";
                cout << _sVal[i];
                cout << " TO LONG LONG ; CONVERSION NOT MADE!" << endl;
                cout << "Returning value 0 for element " << i << endl;
                _val[i]=0;
            }
        }

        delete [] _sVal;
    }
    else if ( channelInfo.getDataType() == DBR_DOUBLE) {
        dbr_double_t * _dVal = new dbr_double_t[nn];
        _status=cafeDoppio.get (handle, DBR_DOUBLE,  _dVal);
        if (_status!=ICAFE_NORMAL) { delete [] _dVal; return _status;}
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _dVal[i];
        }
        delete [] _dVal;
    }
    else if ( channelInfo.getDataType() == DBR_FLOAT) {
        dbr_float_t * _fVal = new dbr_float_t[nn];
        _status=cafeFrappuccino.get (handle, DBR_FLOAT,  _fVal);
        if (_status!=ICAFE_NORMAL) { delete [] _fVal; return _status;}
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _fVal[i];
        }
        delete [] _fVal;
    }
    else {
        dbr_long_t * _lVal = new dbr_long_t[nn];
        _status=cafeLatte.get (handle, DBR_LONG,  _lVal);
        if (_status!=ICAFE_NORMAL) { delete [] _lVal; return _status;}
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _lVal[i];
        }
        delete [] _lVal;
    }
    return _status;

#undef __METHOD__
};



/**
 *  Sends ca_get message to IOC and returns without waiting, i.e. non-blocking \n
 *  To be followed by a getCache method
 *  \param handle input: handle to conduit object
 *  \return ICAFE_NORMAL if all OK else ICAFE_WAITING_FOR_PREV_CALLBACK or other;
 */
int  CAFE::get(const unsigned int handle) {
#define __METHOD__ "CAFE::get(unsigned int handle)"

	status=ICAFE_NORMAL;
	cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
	cafeConduit_set_by_handle::iterator it_handle;
	it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end()) {

        chtype dbrtype= (*it_handle).getChannelRequestMetaDataClient().getDbrDataType();

        //Find Native DataType
        if ( (status=cafeGranules.channelVerifyGet(handle, dbrtype))
            != ICAFE_NORMAL) {
            if(MUTEX){cafeMutex.lock();}  //lock
            handle_index.modify(it_handle, change_status(status));
            if(MUTEX){cafeMutex.unlock();}//unlock
            cout << __METHOD__ << "//" << __LINE__
                    << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
            cafeStatus.report(status);
            return status;
        }


        if ( (status=cafeGranules.channelPrepareGet(handle)) != ICAFE_NORMAL) {
            if(MUTEX){cafeMutex.lock();}  //lock
            handle_index.modify(it_handle, change_status(status));
            if(MUTEX){cafeMutex.unlock();}//unlock
            cout << __METHOD__ << "//" << __LINE__
                   << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
            cafeStatus.report(status);
            return status;
        }

        if ( (status=cafeGranules.channelExecuteGetNoWait(handle)) != ICAFE_NORMAL) {
            if(MUTEX){cafeMutex.lock();}  //lock
            handle_index.modify(it_handle, change_status(status));
            if(MUTEX){cafeMutex.unlock();}//unlock
            if (status !=ICAFE_WAITING_FOR_PREV_CALLBACK) {
                cout << __METHOD__ << "//" << __LINE__
                    << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
                cafeStatus.report(status);
            }
            return status;
        }

    }
    else {
				if (printErrorPolicy.getInvalidHandle())
        	cafeStatus.report(ECAFE_INVALID_HANDLE);

        return ECAFE_INVALID_HANDLE;
    }

    if(MUTEX){cafeMutex.lock();}; //lock
    handle_index.modify(it_handle, change_status(status));
    if(MUTEX){cafeMutex.unlock();}; //unlock

    return status;
#undef __METHOD__
}


/**
 *  Sends message to IOC to retreive DBR_CTRL data without waiting, i.e. non-blocking \n
 *  To be followed by a getCtrlCache method
 *  \param handle input: handle to conduit object
 *  \return ICAFE_NORMAL if all OK else ICAFE_WAITING_FOR_PREV_CALLBACK or other;
 */
int  CAFE::getCtrl(const unsigned int handle) {
#define __METHOD__ "CAFE::getCtrl(unsigned int handle)"


    status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end()) {

      
        //Use Native DataType
        if ( (status=cafeGranules.channelVerifyGetCtrl(handle,
              dbf_type_to_DBR_CTRL((*it_handle).getChannelRegalia().getDataType())))
            != ICAFE_NORMAL) {
             if(MUTEX){cafeMutex.lock();}; //lock
            handle_index.modify(it_handle, change_status(status));
             if(MUTEX){cafeMutex.unlock();}; //unlock
            cout << __METHOD__ << "//" << __LINE__
                    << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
            cafeStatus.report(status);
            return status;
        }
	
        if ( (status=cafeGranules.channelPrepareGetCtrl(handle)) != ICAFE_NORMAL) {
             if(MUTEX){cafeMutex.lock();}; //lock
            handle_index.modify(it_handle, change_status(status));
             if(MUTEX){cafeMutex.unlock();}; //unlock
            cout << __METHOD__ << "//" << __LINE__
                    << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
            cafeStatus.report(status);
            return status;
        }

				
        if ( (status=cafeGranules.channelExecuteGetCtrlNoWait(handle)) != ICAFE_NORMAL) {
             if(MUTEX){cafeMutex.lock();}; //lock
            handle_index.modify(it_handle, change_status(status));
             if(MUTEX){cafeMutex.unlock();}; //unlock
            if (status !=ICAFE_WAITING_FOR_PREV_CALLBACK) {
                cout << __METHOD__ << "//" << __LINE__
                    << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
                cafeStatus.report(status);
            }
            return status;
        }
				
		
    }
    else {
				if (printErrorPolicy.getInvalidHandle()){
        	cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        	cafeStatus.report(ECAFE_INVALID_HANDLE);
				}
        return ECAFE_INVALID_HANDLE;
    }

    if(MUTEX){cafeMutex.lock();}; //lock
    handle_index.modify(it_handle, change_status(status));
    if(MUTEX){cafeMutex.unlock();}; //unlock

    return status;
#undef __METHOD__
};



//pvName
//NON_BLOCKING
int  CAFE::get  (const char * pv) {
#define __METHOD__ "CAFE::getCtrl(const char *)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status= CAFE::get(handle);}
    return status;
    //unsigned int handle=getHandleHelper().getHandleFromPV(pv); return get(handle);}
#undef __METHOD__
}

int  CAFE::getCtrl  (const char * pv) {
#define __METHOD__ "CAFE::getCtrl(const char *)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status= CAFE::getCtrl(handle);}
    return status;
#undef __METHOD__
    //unsigned int handle=getHandleHelper().getHandleFromPV(pv); return getCtrl(handle);}
}




/**
 *  Retrieves PV data packaged into the PVDataHolder class
 *  \param pv input: pv name
 *  \param pvd output: PVDataHolder
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, PVDataHolder & pvd) {
#define __METHOD__ "CAFE::get(const char *, PVDataHolder & pvd)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status= CAFE::get(handle, pvd);}
    return status;
#undef __METHOD__
}


/**
 *  Retrieves PV data packaged into the PVDataHolder class
 *  \param handle input: handle to Conduit object
 *  \param pvd output: PVDataHolder containing data
 *  \return ICAFE_NORMAL(=ECA_NORMAL) if all OK
 */
int  CAFE::get(const unsigned int handle, PVDataHolder & pvd) {
#define __METHOD__ "CAFE::get(unsigned int handle, PVDataHolder & pvd)"

		//cout << __FILE__ << " " << __LINE__ << " " << __METHOD__ << endl;
		
    status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end()) {

		if ( (*it_handle).getChannelGetActionWhenMonitorPolicy().getActionKind() == CAFENUM::GET_FROM_CACHE) {
			if ( handleHelper.getNmonitorData(handle) >0){

				bool doGetCache=true;

				vector<MonitorPolicy> mpV;
				handleHelper.getMonitorPolicyVector(handle, mpV);
					 //loop
				for (unsigned int n=0; n<mpV.size(); ++n) {

					if (mpV[n].getCafeDbrType() != CAFENUM::DBR_TIME) {
							 doGetCache=false;
							 cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
							 cout << "***INFO*** YOU HAVE A MONITOR STARTED OM THIS CHANNEL "  << endl;
							 cout << "THAT DOES NOT RETRIEVE THE REQUESTED TIMESTAMPS " << endl;
							 cout << "WE WILL THEREFORE ATTEMPT A GET WITH DRB_TIME (RATHER THAN FROM CACHE)" << endl;
							 cout << "WE ADVICE THAT YOU MODIFY YOUR CODE TO START YOUR MONITOR WITH DRB_TIME" << endl;
							 cout << "Handle= " << handle <<  " PV= " << handleHelper.getPVFromHandle(handle) << endl;
							 mpV[n].print();
					}
				}


				if(doGetCache) {return CAFE::getCache(handle, pvd);}
			}
		}

//new
/*
        channelRequestMetaDataClient= (*it_handle).getChannelRequestMetaDataClient();

        if (pvd.getNelem() !=
            (channelRequestMetaDataClient.getNelem() - channelRequestMetaDataClient.getOffset() )) {
            channelRequestMetaDataClient.setNelem(pvd.getNelem()+channelRequestMetaDataClient.getOffset());
             if(MUTEX){cafeMutex.lock();}   //lock
            handle_index.modify(it_handle,
                change_channelRequestMetaDataClient(channelRequestMetaDataClient));
             if(MUTEX){cafeMutex.unlock();} //unlock
        }
*/


		//Let us take a look at hasAlarm and hasTS flags to determine if these are retrieved from SC
		chtype _chtype=(*it_handle).getChannelRegalia().getDataType();


		if (!pvd.hasAlarm && !pvd.hasTS) {
			handleHelper.setCafeDbrType(handle, CAFENUM::DBR_PRIMITIVE);
		}
		else if (pvd.hasAlarm && !pvd.hasTS) {
			_chtype=dbf_type_to_DBR_STS (_chtype);
			handleHelper.setCafeDbrType(handle, CAFENUM::DBR_STS) ;

		}
		else if (pvd.hasTS) {
			_chtype=dbf_type_to_DBR_TIME (_chtype);
			 handleHelper.setCafeDbrType(handle, CAFENUM::DBR_TIME) ;
		}


		


        //Find Native DataType
        if ( (status=cafeGranules.channelVerifyGet(handle,
			 _chtype))
			 // dbf_type_to_DBR_TIME ((*it_handle).getChannelRegalia().getDataType())))
            != ICAFE_NORMAL) {

             //Do this so that at least PVName etc is returned
            (*it_handle).getPVDataHolder(pvd);
            pvd.status=status;           
            pvd.alarmStatus=-1;
            pvd.alarmSeverity=-1;
            pvd.ts.secPastEpoch=0;
            pvd.ts.nsec=0;
            return status;
        }
        if ( (status=cafeGranules.channelPrepareGet(handle)) != ICAFE_NORMAL) {
            (*it_handle).getPVDataHolder(pvd);
            pvd.status=status;           
            pvd.alarmStatus=-1;
            pvd.alarmSeverity=-1;
            pvd.ts.secPastEpoch=0;
            pvd.ts.nsec=0;           
            return status;
        }
        if ( (status=cafeGranules.channelExecuteGet(handle)) != ICAFE_NORMAL) {
            (*it_handle).getPVDataHolder(pvd);
            pvd.status=status;           
            pvd.alarmStatus=-1;
            pvd.alarmSeverity=-1;
            pvd.ts.secPastEpoch=0;
            pvd.ts.nsec=0;
            return status;
        }
				
		
				
		(*it_handle).getPVDataHolder(pvd);
		
 
	
		//report back timestamps to conduit for the blocking get methods
		handleHelper.setSTS(handle, pvd.alarmStatus, pvd.alarmSeverity, pvd.ts);


		handleHelper.setNelemToRetrieveFromCache(handle,pvd.nelem);


    }
    else {
				if (printErrorPolicy.getInvalidHandle()) {
        	cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        	cafeStatus.report(ECAFE_INVALID_HANDLE);
				}
        pvd.status=ECAFE_INVALID_HANDLE;
        return ECAFE_INVALID_HANDLE;
    }

    if(MUTEX){cafeMutex.lock();}; //lock

    handle_index.modify(it_handle, change_status(status));
    if(MUTEX){cafeMutex.unlock();}; //unlock

    return status;
#undef __METHOD__
};


/**
 *  Waits until handle callbacks has reported or timed out
 *  \param handle input: Handle to Conduit objects
 *  \return overallStatus: ICAFE_NORMAL if all OK else ECAFE error if encountered
 */
int CAFE::waitForGetEvent(unsigned int handle){
			cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
			cafeConduit_set_by_handle::iterator it_handle;		
			it_handle = handle_index.find(handle);
			if (it_handle != handle_index.end()) {
				return cafeGranules.waitForGetEvent(handle, (*it_handle).getChannelTimeoutPolicyGet().getTimeout());
			}
      else {return ECAFE_INVALID_HANDLE;}
};		





/**
 *  Waits until all handle callbacks have reported or timed out
 *  \param handleV input: Vector of handles to Conduit objects
 *  \param vRB output: vector of Bundled responses (reporting whether callback was completed)
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int CAFE::waitForBundledEvents(vector<unsigned int> handleV, vector<int> &vRB){
#define __METHOD__ \
"int CAFE::waitForBundledEvents(vector<unsigned int> handleV, vector<unsigned int> vRB)	"

return waitForBundledEvents(&handleV[0], handleV.size(), vRB);

/*
	int statusBundle=ICAFE_NORMAL;

	map<unsigned int, int> bundleResponse;
	map<unsigned int, int>::iterator pos;

	unsigned int nHandles=handleV.size();
	vRB.clear();
	vRB.reserve(nHandles);

	std::set<unsigned int> uniqueHandles;
	std::set<unsigned int>::iterator uniquePos;
	
	//std::vector<unsigned int> handleArrayV(handleV);


	//Remove duplicate handles
	for (unsigned int i=0; i<nHandles; ++i) {
			uniqueHandles.insert(handleV[i]);		
	} 
	
	//cout << __METHOD__ << " " << endl;
	//cout << " uniqueHandles.size()= " << uniqueHandles.size() << " nH= " << nHandles << endl;
	
	//handleV.clear();

	unsigned int * handleArray = new unsigned int[uniqueHandles.size()];

	//for (int i=0; i<nHandles; ++i) {
	//	handleArray[i]=handleV[i];
	//}


	if (uniqueHandles.size() != nHandles) {
		cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
		cout << "For information purposes only: " << endl;
		cout << "Removed " << (nHandles - uniqueHandles.size()) << " duplicate handles from the given " << nHandles
		<< "; bundleResponse in waitForManyGetEvents will return with reduced length: "
		<< uniqueHandles.size() << endl;
		cout << "This is not an error, but user might like to be aware that duplicate handles were submitted " << endl;

		unsigned int ij=0;
		for (uniquePos = 	uniqueHandles.begin(); uniquePos != uniqueHandles.end(); ++uniquePos) {
			handleArray[ij]= (unsigned int) (*uniquePos);
			++ij;
		}
		nHandles=ij;
	}
	else {
		std::copy(handleV.begin(), handleV.end(), handleArray);
	}

	//bundleResponse will be of different length for duplicate channels!
	statusBundle=cafeGranules.waitForManyGetEvents(handleArray, nHandles, bundleResponse) ;
	if (statusBundle!=ICAFE_NORMAL) {
		cout << __METHOD__ << " ERROR: " << statusBundle << endl;
	}

	for (unsigned int i=0; i<handleV.size(); ++i) {
		pos=bundleResponse.find(handleV[i]);
		if(pos != bundleResponse.end()) {
			vRB.push_back(pos->second);
			//cout << "handle " << pos->first << " status " << pos->second  << " [" << i << "] "<< endl;
			if(pos->second != ICAFE_NORMAL) {
				cout << resetCallbackGet(pos->first) << endl;
			}
		}
	}

	//for (unsigned int i=0; i<vRB.size(); ++i) {
	//	cout << "status from waitForBundleEvents " << vRB[i] << " [" << i << "] " << endl;
	//}

	
	//for (pos=bundleResponse.begin(); pos != bundleResponse.end(); ++pos) {
	//	if (statusBundle!=ICAFE_NORMAL) {
	//	  cout << "Handle=" << pos->first << " Status= " << pos->second << endl;
	//	
	//		if(pos->second != ICAFE_NORMAL) {
	//		
	//			resetCallbackGet(pos->first);
	//		}
	//	}
	//	vRB.push_back(pos->second);
	//}
	
	delete [] handleArray;
	return statusBundle;
	*/
#undef __METHOD__
};



/**
 *  Waits until all handle callbacks have reported or timed out
 *  \param handleArray input: Array of handles to Conduit objects
 *  \param nHandles input: Lemgth of handleArray
 *  \param vRB output: vector of Bundled responses (reporting whether callback was completed)
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int CAFE::waitForBundledEvents(unsigned int * handleArray, unsigned int nHandles, vector<int> &vRB){
#define __METHOD__ \
"int CAFE::waitForBundledEvents(unsigned int * handleArray, unsigned int nHandles, vector<unsigned int> vRB)	"
	int statusBundle=ICAFE_NORMAL;

	map<unsigned int, int> bundleResponse;
	map<unsigned int, int>::iterator pos;

	vRB.clear();
	vRB.reserve(nHandles);

	std::vector<unsigned int> handleArrayV;
	handleArrayV.reserve(nHandles);
	handleArrayV.insert(handleArrayV.end(), &handleArray[0], &handleArray[nHandles]);

	std::set<unsigned int> uniqueHandles;
	std::set<unsigned int>::iterator uniquePos;

	
	//Remove duplicate handles
	for (unsigned int i=0; i<nHandles; ++i) {
			uniqueHandles.insert(handleArray[i]);		
	} 
		
	if (uniqueHandles.size() != nHandles) {
		cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
		cout << "For information purposes only: " << endl;
		cout << "Removed " << (nHandles - uniqueHandles.size())
				<< " duplicate handles from the given " << nHandles
				<< "; bundleResponse in waitForManyGetEvents will return with reduced length: "
						<< uniqueHandles.size() << endl;
		cout << "This is not an error, but user might like to be aware that duplicate handles were submitted " << endl;
		unsigned int ij=0;
		for (uniquePos = 	uniqueHandles.begin(); uniquePos != uniqueHandles.end(); ++uniquePos) {
			handleArray[ij]= (unsigned int) (*uniquePos);
			++ij;
		}
		nHandles=ij;
	}

	statusBundle=cafeGranules.waitForManyGetEvents(handleArray, nHandles, bundleResponse) ;
	if (statusBundle!=ICAFE_NORMAL) {
		cout << __METHOD__ << " ERROR: " << statusBundle << endl;
	}

	for (unsigned int i=0; i<handleArrayV.size(); ++i) {
		pos=bundleResponse.find(handleArrayV[i]);
		if(pos != bundleResponse.end()) {
			vRB.push_back(pos->second);
			if(pos->second != ICAFE_NORMAL) {
				cout << resetCallbackGet(pos->first) << endl;
			}
		}
	}

	//for (unsigned int i=0; i<vRB.size(); ++i) {
	//	cout << "status from waitForBundleEvents " << vRB[i] << " [" << i << "] " << endl;
	//}

	/*
	//bundleResponse will be of different length for duplicate channels!
	for (pos=bundleResponse.begin(); pos != bundleResponse.end(); ++pos) {
		if (statusBundle!=ICAFE_NORMAL) {
		  cout << "Handle= " << pos->first << " Status= " << pos->second << endl;
			//wrong! cout << "Handle=" << handleArray[pos->first] << " Status= " << pos->second << endl;
			if(pos->second != ICAFE_NORMAL) {
			  cout << resetCallbackGet(pos->first) << endl; 
				//wrong! resetCallbackGet(handleArray[pos->first]);
			}
		}
		vRB.push_back(pos->second);
	}
	*/

	return statusBundle;
#undef __METHOD__
};




/**
 *  Retrieves PV data packaged into an array of PVDataHolder class'
 *  \param handleArray input: Array of handles to Conduit objects
 *  \param nelem input: No of array elements
 *  \param pvdArray output: Array of PVDataHolder
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int  CAFE::get(const unsigned int  * handleArray, unsigned int  nelem, \
               PVDataHolder * pvdArray) {
#define __METHOD__ \
"CAFE::get(unsigned int  * handleArray, unsigned int  nelem, PVDataHolder * pvdArray)"


	  int  overallStatus=ICAFE_NORMAL;
	  bool isGood=true;

	  CAFENUM::DBR_TYPE cdt;

    for (unsigned int  i=0; i<nelem; ++i) {

		//Ensure that get retrieves DBR_TIME!
		status=handleHelper.getCafeDbrType(handleArray[i],cdt);

		if ( (status == ECAFE_INVALID_HANDLE)) {
			overallStatus=status;
			continue;
		}

		if(cdt!=CAFENUM::DBR_TIME) {
			handleHelper.setCafeDbrType(handleArray[i],CAFENUM::DBR_TIME);
		}

		bool doGet=true;

		ChannelGetActionWhenMonitorPolicy  channelGetActionWhenMonitorPolicy;
		policyHelper.getChannelGetActionWhenMonitorPolicy(handleArray[i],channelGetActionWhenMonitorPolicy);

		if ( channelGetActionWhenMonitorPolicy.getActionKind() == CAFENUM::GET_FROM_CACHE) {

			if (handleHelper.getNmonitorData(handleArray[i])>0) {
				 vector<MonitorPolicy> mpV;
				 handleHelper.getMonitorPolicyVector(handleArray[i], mpV);
				 //loop
				 for (unsigned int n=0; n<mpV.size(); ++n) {
					 doGet=false;
					 if (mpV[n].getCafeDbrType() != CAFENUM::DBR_TIME) {
						 doGet=true;
						 cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
						 cout << "***INFO*** YOU HAVE A MONITOR STARTED OM THIS CHANNEL "  << endl;
						 cout << "THAT DOES NOT RETRIEVE THE REQUESTED TIMESTAMPS " << endl;
						 cout << "WE WILL THEREFORE ATTEMPT AN ASYNCHRONOUS GET WITH DRB_TIME " << endl;
						 cout << "WE ADVICE THAT YOU MODIFY YOUR CODE TO START YOUR MONITOR WITH DRB_TIME" << endl;
						 cout << "Handle= " << handleArray[i] <<  " PV= " << handleHelper.getPVFromHandle(handleArray[i]) << endl;
						 mpV[n].print();
					 }
				 }
			}

		}//if

			//Only do a get if handle does not have a monitor and policy dictates!
			if (doGet) {

				status=get(handleArray[i]);

				if ( (status != ICAFE_NORMAL) && isGood) {
					overallStatus=status;
					isGood=false;
				}
			}

    }

	//ca_poll();
	//Fo Flush in waitForManyGetEvents

    if (overallStatus==ICAFE_WAITING_FOR_PREV_CALLBACK) {
        //reset
        overallStatus=ICAFE_NORMAL; isGood=true;
    }

	map<unsigned int, int> bundleResponse;
	//map<unsigned int, int>::iterator pos;

	int statusBundle=cafeGranules.waitForManyGetEvents(handleArray, nelem, bundleResponse) ;

	//for (pos=bundleResponse.begin(); pos != bundleResponse.end(); ++pos) {
	//	cout << pos->first << " <-----------> " << pos->second << endl;
	//}

    for (unsigned int  i=0; i<nelem; ++i) {
        //setNoElement to retrieve from Cache
        //Has to be number of elements in c.channelRequestMetaData.nelemCache
        //handleHelper.setNelemToRetrieveFromCache(handleArray[i],pvdArray[i].getNelem());
        //However getCache with pvdArray does not use c.channelRequestMetaData.nelemCache	
        //Rather uses  pvdArray[i].nelem! So all OK
		//Added handleHelper.setNelemToRetrieveFromCache(handleArray[i],pvdArray[i].getNelem())
		//to get(handle, pvd); True also for the equivalent pvc

		//isDone??
		//Check on is Done for all!?
		//Our own timeout here!!!


		//status is always normal or ICAFE_CS_DISCONN
		status=getCacheNoWait(handleArray[i], pvdArray [i]);
		if (status!=ICAFE_NORMAL) {
			resetCallbackGet(handleArray[i]);
		}

		//cout << __METHOD__ << " status after getCacheNoWait\n " << status << " elem " << i <<  " pv " << pvdArray[i].getPV() << endl;
		//cout << "status = " << status << " pvdStatus= " << pvdArray[i].status << endl;
		//cout << "VALUE " << pvdArray[i].getAsString() << endl;


		if ( (pvdArray[i].status != ICAFE_NORMAL) && isGood) {
			overallStatus=pvdArray[i].status;
            isGood=false;
        }
    }

	//cout<< statusBundle << " SB/OS "  << overallStatus << endl;

	if (overallStatus==ICAFE_NORMAL) {
		return statusBundle;
	}
	else {
		return overallStatus;
	}
#undef __METHOD__
};


/**
 *  Retrieves PV DBR_CTRL data, packed into the PVCtrlHolder class
 *  \param pv input: pv name
 *  \param pvc output: PVCtrlHolder
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCtrl(const char * pv, PVCtrlHolder & pvc) {
#define __METHOD__ "CAFE::getCtrl(const char *, PVCtrlHolder & pvc)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status= CAFE::getCtrl(handle, pvc);}
    return status;
#undef __METHOD__
}



/**
 *  Retrieves PV DBR_CTRL data, packaged into the PVCtrlHolder class
 *  \param  handle input: handle to cafeConduit object
 *  \param  pvc output: PVCtrlHolder containing ctrl data
 *  \return ICAFE_NORMAL(=ECA_NORMAL) if all OK
 */
int  CAFE::getCtrl(const unsigned int  handle, PVCtrlHolder & pvc) {
#define __METHOD__ "CAFE::getCtrl(unsigned int  handle, PVCtrlHolder & pvc)"

    status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end()) {

        channelRequestMetaCtrlClient= (*it_handle).getChannelRequestMetaCtrlClient();

        if (pvc.getNelem() != channelRequestMetaCtrlClient.getNelem()) {
            channelRequestMetaCtrlClient.setNelem(pvc.getNelem());
             if(MUTEX){cafeMutex.lock();}   //lock
            handle_index.modify(it_handle,
                change_channelRequestMetaCtrlClient(channelRequestMetaCtrlClient));
             if(MUTEX){cafeMutex.unlock();} //unlock
        }

        //Use Native DataType
        if ( (status=cafeGranules.channelVerifyGetCtrl(handle,
              dbf_type_to_DBR_CTRL((*it_handle).getChannelRegalia().getDataType())))
            != ICAFE_NORMAL) {         
            (*it_handle).getPVCtrlHolder(pvc);
            pvc.status=status;
            return status;
        }
        if ( (status=cafeGranules.channelPrepareGetCtrl(handle)) != ICAFE_NORMAL) {
            (*it_handle).getPVCtrlHolder(pvc);
            pvc.status=status;
            return status;
        }
        if ( (status=cafeGranules.channelExecuteGetCtrl(handle)) != ICAFE_NORMAL) {
            (*it_handle).getPVCtrlHolder(pvc);
            pvc.status=status;
            return status;
        }
        (*it_handle).getPVCtrlHolder(pvc);

		handleHelper.setNelemToRetrieveFromCtrlCache(handle,pvc.nelem);

    }
    else {
				if (printErrorPolicy.getInvalidHandle()) {
        	cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        	cafeStatus.report(ECAFE_INVALID_HANDLE);
       	 	pvc.status=ECAFE_INVALID_HANDLE;
				}
        return ECAFE_INVALID_HANDLE;
    }

    if(MUTEX){cafeMutex.lock();}; //lock
    handle_index.modify(it_handle, change_status(status));
    if(MUTEX){cafeMutex.unlock();}; //unlock

    return status;
#undef __METHOD__
};

/**
 *  Retrieves data, packaged into an array of PVCtrlHolder class'
 *  \param handleArray input: Array of handles to Conduit object
 *  \param nelem input: No of array elements
 *  \param pvcArray output: Array of PVCtrlHolder
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int  CAFE::getCtrl(const unsigned int  * handleArray, unsigned int  nelem, \
                   PVCtrlHolder * pvcArray) {
#define __METHOD__ \
"CAFE::getCtrl(unsigned int  * handleArray, unsigned int  nelem, PVCtrlHolder * pvcArray)"

    int  overallStatus=ICAFE_NORMAL; bool isGood=true;
    for (unsigned int  i=0; i<nelem; ++i) {
        status=getCtrl(handleArray[i]);
        if ( (status != ICAFE_NORMAL) && isGood) {
            overallStatus=status;
            isGood=false;
        }
    }
    ca_flush_io();

    if (overallStatus==ICAFE_WAITING_FOR_PREV_CALLBACK) {
        //reset
        overallStatus=ICAFE_NORMAL; isGood=true;
    }

    for (unsigned int  i=0; i<nelem; ++i) {
        status=getCtrlCache(handleArray[i], pvcArray [i]);
        if ( (status != ICAFE_NORMAL) && isGood) {
            overallStatus=status;
            isGood=false;
        }
    }

    return overallStatus;


#undef __METHOD__
};





/**
 *  \brief   get for array of handles
 *  \param   handleArray input:  array of handles
 *  \param   nHandles input: size of array
 *  \param   statusV output: vector of int values
 *  \return  overallStatus is ICAFE_NORMAL if all OK else the first ECAFE error encountered
 */
int  CAFE::get(const unsigned int * handleArray, unsigned int nHandles, vector<int> &statusV) {
	int  overallStatus=ICAFE_NORMAL;
	bool isGood=true;

	statusV.clear();
	statusV.reserve(nHandles);

	for (unsigned int  i=0; i<nHandles; ++i) {

		//Does already have a monitor?
		if (handleHelper.getNmonitorData(handleArray[i])>0) {
			statusV.push_back(ICAFE_NORMAL); //ICAFE_HAS_MONITOR_GET_DONE_FROM_CACHE);
		}
		else {
			statusV.push_back(get(handleArray[i]));
		}
		if ( (statusV[i] != ICAFE_NORMAL) && isGood) {
			overallStatus=statusV[i];
			isGood=false;
		}
	}
	//ca_flush_io();
	return overallStatus; // report first failure in return statement
}


/**
 *  Sends ca_get messages to IOC and waits until the end before flushing.\n
 *  To be followed by a getCache method
 *  \param handleArray input: Array of handles to conduit object
 *  \param nelem input: Size of array
 *  \param statusArray output: Status array reporting from each get(handle)
 *  \return overallStatus ICAFE_NORMAL if all OK else the first ECAFE error encountered
 */
int  CAFE::get(const unsigned int  *handleArray, unsigned int  nelem, \
               int  *statusArray) {
#define __METHOD__ "CAFE::get(unsigned int  * handleArray, unsigned int  nelem)"
    int  overallStatus=ICAFE_NORMAL; bool isGood=true;
    for (unsigned int  i=0; i<nelem; ++i) {
			//Does already have a monitor?
			if (handleHelper.getNmonitorData(handleArray[i])>0) {
				statusArray[i]=ICAFE_NORMAL; //ICAFE_HAS_MONITOR_GET_DONE_FROM_CACHE;
			}
			else {
				statusArray[i]=get(handleArray[i]);
			}

      if ( (statusArray[i] != ICAFE_NORMAL) && isGood) {
          overallStatus=statusArray[i];
          isGood=false;
      }
    }
    // flush now!
	//ca_flush_io(); //flush in get

    return overallStatus; // report first failure in return statement
#undef __METHOD__
};



/**
 *  Sets data as packaged in the PVDataHolder class
 *  \param pv input: pv name
 *  \param pvd in/output: PVDataHolder
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, PVDataHolder & pvd) {
#define __METHOD__ "CAFE::set(const char *, PVDataHolder & pvd)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status= CAFE::set(handle, pvd);}
    return status;
#undef __METHOD__
}



/**
 *  Sets data as packaged in the PVDataHolder class
 *  \param handle input: handle to Conduit object
 *  \param pvd input/output: PVDataHolder containing data
 *  \return ICAFE_NORMAL if all OK
 */
int  CAFE::set(const unsigned int  handle, PVDataHolder & pvd) {
#define __METHOD__ "CAFE::set(unsigned int  handle, PVDataHolder & pvd)"

    status=ICAFE_NORMAL;

	  bool changeNelemFlag=false;
		unsigned int nelemPrevious=1;

    pvd.alarmStatus=-1;
    pvd.alarmSeverity=-1;
    pvd.ts.secPastEpoch=0;
    pvd.ts.nsec=0;
    pvd.status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end()) {
		
				
        channelRequestMetaDataClient= (*it_handle).getChannelRequestMetaDataClient();
								
		    //cout << __METHOD__ << " pvd.getNelem() " << 		  pvd.getNelem() << endl;
				//cout << " channelRequestMetaDataClient.getNelem() " << channelRequestMetaDataClient.getNelem() << endl;
				//cout << " channelRequestMetaDataClient.getOffset() " << channelRequestMetaDataClient.getOffset() << endl;
				
        if (pvd.getNelem() !=
            (channelRequestMetaDataClient.getNelem() - channelRequestMetaDataClient.getOffset() )) {
						
						nelemPrevious=channelRequestMetaDataClient.getNelem();
						
            //Do NOT*** add offset to this!
						//This will extend the buffer to beyond the alloted size. Offsets are for READ only! 
            channelRequestMetaDataClient.setNelem(pvd.getNelem()); // +channelRequestMetaDataClient.getOffset()); No offset as this is for put!!!
            if(MUTEX){cafeMutex.lock();}   //lock
            handle_index.modify(it_handle,
                change_channelRequestMetaDataClient(channelRequestMetaDataClient));
            if(MUTEX){cafeMutex.unlock();} //unlock
						changeNelemFlag=true;
        }
				
												
				bool soFarSoGood=true;
				
        //Find Native DataType
        if ( soFarSoGood && (status=cafeGranules.channelVerifyPut(handle, pvd.dataType))
            != ICAFE_NORMAL) {
            pvd.status=status;						
						soFarSoGood=false;
        }
        if ( soFarSoGood && (status=cafeGranules.channelPreparePut(handle)) != ICAFE_NORMAL) {
            pvd.status=status;
						soFarSoGood=false;
        }     

				if (!soFarSoGood) {
						if(MUTEX){cafeMutex.lock();}; //lock
        		handle_index.modify(it_handle, change_status(status));
    				if(MUTEX){cafeMutex.unlock();}; //unlock	
						
						if(changeNelemFlag) {
							channelRequestMetaDataClient.setNelem(nelemPrevious); //handleHelper.getNelemNative(handle));
							if(MUTEX){cafeMutex.lock();}   //lock
              handle_index.modify(it_handle,
                change_channelRequestMetaDataClient(channelRequestMetaDataClient));
              if(MUTEX){cafeMutex.unlock();} //unlock							
						}					
            return status;				
				}


        pvd.dbrDataType=
                ((*it_handle).getChannelRequestMetaPrimitive().getDbrDataType());

        switch ((*it_handle).getChannelRequestMetaPrimitive().getDbrDataType()) {

        case DBR_STRING:
            status=renderString.putString(handle, pvd.val.get());
            break;
        case DBR_SHORT:
            status=renderShort.put(handle, pvd.val.get(), pvd.dataType);
            break;
        case DBR_FLOAT:
            status=renderFloat.put(handle, pvd.val.get(), pvd.dataType);
            break;
        case DBR_ENUM:
            status=renderEnum.put (handle, pvd.val.get(), pvd.dataType);
            break;
        case DBR_CHAR:
            status=renderChar.put (handle, pvd.val.get(), pvd.dataType);
            break;
        case DBR_LONG:
            status=renderLong.put (handle, pvd.val.get(), pvd.dataType);
            break;
        case DBR_DOUBLE:
            status=renderDouble.put(handle, pvd.val.get(), pvd.dataType);
            break;
        default:
            cout << "ERROR: " << __FILE__ << "//" << __LINE__ << "//"
                    << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
            cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" <<
                    pvd.dataType <<endl;
            cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
								
						if(changeNelemFlag) {
							channelRequestMetaDataClient.setNelem(nelemPrevious); //handleHelper.getNelemNative(handle));
							if(MUTEX){cafeMutex.lock();}   //lock
              handle_index.modify(it_handle,
                change_channelRequestMetaDataClient(channelRequestMetaDataClient));
              if(MUTEX){cafeMutex.unlock();} //unlock							
						}
            return ECAFE_INVALID_SWITCH_CASE;
            break;
        }

        if(status!=ICAFE_NORMAL) {
				
				//cout << __METHOD__ << " status " << status << endl;
				
				    pvd.status=status;
						soFarSoGood=false;
        }

        if ( soFarSoGood && (status=cafeGranules.channelExecutePut(handle)) != ICAFE_NORMAL) {
            pvd.status=status;
						soFarSoGood=false;
        }							
				
    }
    else {
				if (printErrorPolicy.getInvalidHandle()) {
        	cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        	cafeStatus.report(ECAFE_INVALID_HANDLE);
				}
        pvd.status=ECAFE_INVALID_HANDLE;
			
        return ECAFE_INVALID_HANDLE;
    }

   	 
		//Return for all, regardless of last status!
		if(MUTEX){cafeMutex.lock();}; //lock
    handle_index.modify(it_handle, change_status(status));
    if(MUTEX){cafeMutex.unlock();}; //unlock	
						
		if(changeNelemFlag) {
			channelRequestMetaDataClient.setNelem(nelemPrevious); //handleHelper.getNelemNative(handle));
			if(MUTEX){cafeMutex.lock();}   //lock
      handle_index.modify(it_handle,
                change_channelRequestMetaDataClient(channelRequestMetaDataClient));
      if(MUTEX){cafeMutex.unlock();} //unlock							
		}					
    return status;				
			
	 
     
#undef __METHOD__
};


/**
 *  Sets data as packaged in an array of PVDataHolder class
 *  \param handleArray input: Array of handles to Conduit object
 *  \param nelem input: No of array elements
 *  \param pvdArray outout: Array of PVDataHolder containing data
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int  CAFE::set(const unsigned int  * handleArray, const unsigned int  nelem,
               PVDataHolder * pvdArray) {
#define __METHOD__ \
    "CAFE::set(unsigned int  *handleArray, unsigned int  nelem, PVDataHolder * pvdArray)"

    ChannelRequestPolicy originalPol;
    ChannelRequestPolicy pol;
    PolicyHelper policyHelper;
    int  statusL, overallStatus=ICAFE_NORMAL; 
		bool isGood=true;


    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
  
    
    for (unsigned int  i=0; i<nelem; ++i) {
        //Change policy not to flush
        //NO_WAIT, FLUSH_DESIGNATED_TO_CLIENT

				//Check here if handle exists


   			pvdArray[i].alarmStatus=-1;
    		pvdArray[i].alarmSeverity=-1;
    		pvdArray[i].ts.secPastEpoch=0;
    		pvdArray[i].ts.nsec=0;
    		pvdArray[i].status=ICAFE_NORMAL;

    		it_handle = handle_index.find(handleArray[i]);

    		if (it_handle != handle_index.end()) {

        	policyHelper.getChannelRequestPolicyPut(handleArray[i], originalPol);
        	policyHelper.getChannelRequestPolicyPut(handleArray[i], pol);
        	pol.setWaitKind(NO_WAIT);
        	pol.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);

        	policyHelper.setChannelRequestPolicyPut(handleArray[i], pol);

        	statusL=CAFE::set(handleArray[i], pvdArray[i]);
					
							
        	//Return policy to previous
        	policyHelper.setChannelRequestPolicyPut(handleArray[i], originalPol);
				}
				else{				
				
   				pvdArray[i].alarmStatus=-1;
    			pvdArray[i].alarmSeverity=-1;
    			pvdArray[i].ts.secPastEpoch=0;
    			pvdArray[i].ts.nsec=0;
    			pvdArray[i].status=ECAFE_INVALID_HANDLE;
					statusL=pvdArray[i].status;
				}
				
					
        if ( (statusL != ICAFE_NORMAL) && isGood) {
            overallStatus=statusL;
            isGood=false;
        }
				
    }//for
    ca_flush_io();
		
    return overallStatus;
#undef __METHOD__
};


int  CAFE::getWFAsString(unsigned int handle, string & psWF) {
#define __METHOD__ "int  CAFE::getWFAsString(unsigned int handle, string & message)"

    status=ICAFE_NORMAL;
		cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
		cafeConduit_set_by_handle::iterator it_handle;		
		it_handle = handle_index.find(handle);
		if (it_handle != handle_index.end()) {
					
				unsigned int c=(*it_handle).getChannelRequestMetaDataClient().getNelem();			  
				unsigned int n=(*it_handle).getChannelRegalia().getNelem();
					
				//cout << "c: " << c << " n: " << n << endl;	
								
        dbr_char_t * val = new dbr_char_t[min(c,n)];     
				//dbr_char_t * val = new dbr_char_t[n];   
				status=cafeCappuccino.get(handle, DBR_CHAR, val);				
				psWF = "";
				  
				chtype cdt;	
				handleHelper.getDataTypeNative(handle, cdt); 
			
			
			  unsigned int r=(*it_handle).getChannelRequestMetaData().getNelem();
				
        if (cdt==DBR_CHAR) {
				
            for (unsigned int i=0; i<r; ++i) {
                if (val[i] != '\0') {
                    psWF.append(1, (dbr_char_t) val[i]);                
                }            
            }
        }
		
        delete [] val;    	
		}	
    return status;
#undef __METHOD__
};


int  CAFE::getWFAsStringCache(unsigned int handle, string & psWF) {
#define __METHOD__ "int  CAFE::getWFAsString(unsigned int handle, string & message)"

    status=ICAFE_NORMAL;
		cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
		cafeConduit_set_by_handle::iterator it_handle;		
		it_handle = handle_index.find(handle);
		if (it_handle != handle_index.end()) {
					
				unsigned int c=(*it_handle).getChannelRequestMetaDataClient().getNelem();
			 
				unsigned int n=(*it_handle).getChannelRegalia().getNelem();
			
        
				unsigned int r =min( (*it_handle).getChannelRequestMetaData().getNelemCache(),
			  ((*it_handle).getChannelRequestMetaData().getNelem()-(*it_handle).getChannelRequestMetaData().getOffset()) ) ;
				
				
				//cout << "Cache c: " << c << " r: " << r << " n: " << n << endl;
					
        dbr_char_t * val = new dbr_char_t[min(r,n)];   
				//dbr_char_t * val = new dbr_char_t[n];        
				status=cafeCappuccino.getCache(handle, DBR_CHAR, val);				
				psWF = "";
				  
				chtype cdt;	
				handleHelper.getDataTypeNative(handle, cdt); 
			
			  //unsigned int r=(*it_handle).getChannelRequestMetaData().getNelem();
			  //r=(*it_handle).getNelemToRetrieveFromCache();
			
			
        if (cdt==DBR_CHAR) {
				
            for (unsigned int i=0; i<r; ++i) {
                if (val[i] != '\0') {
                    psWF.append(1, (dbr_char_t) val[i]);                
                }            
            }
        }
		
        delete [] val;    	
		}	
    return status;
#undef __METHOD__
};


////////////////////////////////////////////// by Name ////////////////////////////////
/**
 *  Get string(s), alarms and timeStamp
 *  \param pv input: pv name
 *  \param valStr output: array of strings
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, string  * valStr,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //0
#define __METHOD__ \
"int  CAFE::get(const char * pv, string  * val, dbr_short_t &, dbr_short_t &, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        dbr_string_t * val = new dbr_string_t[handleHelper.getNelemClient(handle)];
        status=cafeSoluble.get(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
        for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i) { valStr[i]=val[i]; }
        delete [] val;
    }
    return status;
#undef __METHOD__
}
/**
 *  Get string(s) and alarms
 *  \param pv input: pv name
 *  \param valStr output: array of strings
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, string  * valStr,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity) { //0
#define __METHOD__ \
"int  CAFE::get(const char * pv, string  * val, dbr_short_t &, dbr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        dbr_string_t * val = new dbr_string_t[handleHelper.getNelemClient(handle)];
        status=cafeSoluble.get(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
        for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i) { valStr[i]=val[i]; }
        delete [] val;
    }
    return status;
#undef __METHOD__
}
/**
 *  Get string(s) (DBR_STRING )
 *  \param pv input: pv name
 *  \param valStr output: array of strings
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, string  * valStr) { //0
#define __METHOD__ "int  CAFE::get(const char * pv, string * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) { return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        dbr_string_t * val = new dbr_string_t[handleHelper.getNelemClient(handle)];
        status=cafeSoluble.get(handle, DBR_STRING, val);
        for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i) { valStr[i]=val[i]; }
        delete [] val;
    }
    return status;
#undef __METHOD__
}


/**
 *  Get string(s), alarms and timeStamp (DBR_TIME_STRING)
 *  \param pv input: pv name
 *  \param val output: array of strings
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_string_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //0
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_string_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeSoluble.get(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);}
    return status;
#undef __METHOD__
}
/**
 *  Get string(s) and alarms (DBR_STS_STRING)
 *  \param pv input: pv name
 *  \param val output: array of strings
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_string_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity) { //0
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_string_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeSoluble.get(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);}
    return status;
#undef __METHOD__
}
/**
 *  Get string(s) (DBR_STRING )
 *  \param pv input: pv name
 *  \param val output: array of strings
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_string_t  * val) { //0
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_string_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) { return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status=cafeSoluble.get(handle, DBR_STRING, val);}
    return status;
#undef __METHOD__
}
/**
 *  Get short(s), alarms and timeStamp (DBR_TIME_SHORT)
 *  \param pv input: pv name
 *  \param val output: array of shorts
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_short_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //1
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_short_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeSchale.get(handle, DBR_TIME_SHORT, val, alarmStatus, alarmSeverity, ts);}
    return status;
#undef __METHOD__
}
/**
 *  Get short(s) and alarms (DBR_STS_SHORT)
 *  \param pv input: pv name
 *  \param val output: array of shorts
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_short_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity) { //1
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_short_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeSchale.get(handle, DBR_STS_SHORT, val, alarmStatus, alarmSeverity);}
    return status;
#undef __METHOD__
}
/**
 *  Get short(s) (DBR_SHORT)
 *  \param pv input: pv name
 *  \param val output: array of shorts
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_short_t  * val) { //1
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_short_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) { return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status=cafeSchale.get(handle, DBR_SHORT, val);}
    return status;
#undef __METHOD__
}
/**
 *  Get float(s), alarms and timeStamp (DBR_TIME_FLOAT)
 *  \param pv input: pv name
 *  \param val output: array of floats
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_float_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //2
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_float_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeFrappuccino.get(handle, DBR_TIME_FLOAT, val, alarmStatus, alarmSeverity, ts);}
    return status;
#undef __METHOD__
}
/**
 *  Get float(s) and alarms (DBR_STS_FLOAT)
 *  \param pv input: pv name
 *  \param val output: array of floats
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_float_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity) { //2
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_float_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeFrappuccino.get(handle, DBR_STS_FLOAT, val, alarmStatus, alarmSeverity);}
    return status;
#undef __METHOD__
}
/**
 *  Get float(s) (DBR_FLOAT)
 *  \param pv input: pv name
 *  \param val output: array of floats
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_float_t  * val) { //2
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_float_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) { return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status=cafeFrappuccino.get(handle, DBR_FLOAT, val);}
    return status;
#undef __METHOD__
}
/**
 *  Get enum(s), alarms and timeStamp (DBR_TIME_ENUM)
 *  \param pv input: pv name
 *  \param val output: array of enums (dbr_enum_t i.e. unsigned short)
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_enum_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //3
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_enum_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeEspresso.get(handle, DBR_TIME_ENUM, val, alarmStatus, alarmSeverity, ts);}
    return status;
#undef __METHOD__
}
/**
 *  Get enum(s) and alarms (DBR_STS_ENUM)
 *  \param pv input: pv name
 *  \param val output: array of enums (dbr_enum_t i.e. unsigned short)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_enum_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity) { //3
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_enum_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeEspresso.get(handle, DBR_STS_SHORT, val, alarmStatus, alarmSeverity);}
    return status;
#undef __METHOD__
}
/**
 *  Get enum(s) (DBR_ENUM)
 *  \param pv input: pv name
 *  \param val output: array of enums (dbr_enum_t i.e. unsigned short)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_enum_t  * val) { //3
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_enum_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) { return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status=cafeEspresso.get(handle, DBR_SHORT, val);}
    return status;
#undef __METHOD__
}
/**
 *  Get unsigned char(s), alarms and timeStamp (DBR_TIME_CHAR)
 *  \param pv input: pv name
 *  \param val output: array of unsigned char (dbr_char_t)
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_char_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //4
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_char_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeCappuccino.get(handle, DBR_TIME_CHAR, val, alarmStatus, alarmSeverity, ts);}
    return status;
#undef __METHOD__
}
/**
 *  Get unsigned char(s) and alarms (DBR_STS_CHAR)
 *  \param pv input: pv name
 *  \param val output: array of unsigned char (dbr_char_t)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_char_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity) { //4
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_char_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeCappuccino.get(handle, DBR_STS_CHAR, val, alarmStatus, alarmSeverity);}
    return status;
#undef __METHOD__
}
/**
 *  Get unsigned char(s) (DBR_CHAR)
 *  \param pv input: pv name
 *  \param val output: array of unsigned char (dbr_char_t)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_char_t  * val) { //4
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_char_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) { return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status=cafeCappuccino.get(handle, DBR_CHAR, val);}
    return status;
#undef __METHOD__
}
/**
 *  Get long(s), alarms and timeStamp (DBR_TIME_LONG)
 *  \param pv input: pv name
 *  \param val output: array of longs (dbr_long_t)
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_long_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //5
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_long_t * val, dbr_short_t &, dbr_short_t &, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeLatte.get(handle, DBR_TIME_LONG, val, alarmStatus, alarmSeverity, ts);}
    return status;
#undef __METHOD__
}
/**
 *  Get long(s) and alarms (DBR_STS_LONG)
 *  \param pv input: pv name
 *  \param val output: array of longs (dbr_long_t)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_long_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity) { //5
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_long_t  * val, dbr_short_t &, dbr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeLatte.get(handle, DBR_STS_LONG, val, alarmStatus, alarmSeverity);}
    return status;
#undef __METHOD__
}

/**
 *  Get long(s) (DBR_LONG)
 *  \param pv input: pv name
 *  \param val output: array of dbr_long_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_long_t  * val) { //5
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_long_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) { return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status=cafeLatte.get(handle, DBR_LONG, val);}
    return status;
#undef __METHOD__
}

/**
 *  Get long(s) (DBR_LONG)
 *  \param pv input: pv name
 *  \param val output: array of long long
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, long long  * val){ //5+ long long
#define __METHOD__ "int  CAFE::get(const char * pv, long long * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) { return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status=CAFE::get(handle, val);}
    return status;
#undef __METHOD__
}

/**
 *  Get long long(s) (DBR_STS_*)
 *  \param pv input: pv name
 *  \param val output: array of long long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, long long  * val, dbr_short_t &alarmStatus,
                                dbr_short_t &alarmSeverity){ //5+ long long
#define __METHOD__ \
"int  CAFE::get(const char * pv, long long  * val, dbr_short_t &, dr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status=CAFE::get(handle, val, alarmStatus, alarmSeverity);}
    return status;
#undef __METHOD__
}

/**
 *  Get long long(s) (DBR_TIME_*)
 *  \param pv input: pv name
 *  \param val output: array of long long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, long long  * val,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts){ //5+ long long
#define __METHOD__ \
"int  CAFE::get(const char * pv, long long * val, dbr_short_t &, dbr_short_t &, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=CAFE::get(handle, val, alarmStatus, alarmSeverity, ts);}
    return status;
#undef __METHOD__
}
/**
 *  Get double(s), alarms and timeStamp (DBR_TIME_DOUBLE)
 *  \param pv input: pv name
 *  \param val output: array of doubles (dbr_double_t)
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_double_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //6
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_double_t  * val, dbr_short_t &, dbr_short_t &, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeDoppio.get(handle, DBR_TIME_DOUBLE, val, alarmStatus, alarmSeverity, ts);}
    return status;
#undef __METHOD__
}
/**
 *  Get double(s) and alarms (DBR_STS_DOUBLE)
 *  \param pv input: pv name
 *  \param val output: array of enums (dbr_double_t)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_double_t  * val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity) { //6
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_double_t  * val, dbr_short_t &, dbr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=cafeDoppio.get(handle, DBR_STS_DOUBLE, val, alarmStatus, alarmSeverity);}
    return status;
#undef __METHOD__
}

/**
 *  Get double(s) (DBR_DOUBLE)
 *  \param pv input: pv name
 *  \param val output: array of doubles (dbr_double_t)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_double_t  * val) { //6
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_double_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) { return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {status=cafeDoppio.get(handle, DBR_DOUBLE, val);}
    return status;
#undef __METHOD__
}


/**
 *  Get string
 *  \param pv input: pv name
 *  \param valStr output: string
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, string &valStr,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //0+
#define __METHOD__ \
"int  CAFE::get(const char * pv, string  &valStr," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]={""};
        status=cafeSoluble.get(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {valStr= val[0];}
        else {valStr= "";}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  Get string
 *  \param pv input: pv name
 *  \param valStr output: string
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, string &valStr, dbr_short_t &alarmStatus, \
               dbr_short_t &alarmSeverity) { //0+
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_string_t  &valStr, dbr_short_t &, dbr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]={""};
        status=cafeSoluble.get(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL) {valStr= val[0];}
        else {valStr= "";}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get string
 *  \param pv input: pv name
 *  \param valStr output: string
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, string  &valStr) { //0+
#define __METHOD__ "int  CAFE::get(const char * pv, string &_val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv, handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]={""};
        status=cafeSoluble.get(handle, DBR_STRING, val);
        if (status==ICAFE_NORMAL)  {valStr= val[0];}
        else {valStr= "";}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  Get string (DBR_TIME_STRING)
 *  \param pv input: pv name
 *  \param _val output: string
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_string_t  &_val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //0
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_string_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]={""};
        status=cafeSoluble.get(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {sprintf(_val, val[0]);}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get string (DBR_STS_STRING)
 *  \param pv input: pv name
 *  \param _val output: string
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_string_t  &_val, dbr_short_t &alarmStatus, \
               dbr_short_t &alarmSeverity) { //0
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_string_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]={""};
        status=cafeSoluble.get(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL) {sprintf(_val, val[0]);}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get string (DBR_STRING)
 *  \param pv input: pv name
 *  \param _val output: string
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_string_t  &_val) { //0
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_string_t  &_val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]={""};   
        status=cafeSoluble.get(handle, DBR_STRING, val);   
        if (status==ICAFE_NORMAL) {sprintf(_val, val[0]);}     
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);       
    }
    return status;
#undef __METHOD__
}
/**
 *  Get short (DBR_SHORT)
 *  \param pv input: pv name
 *  \param _val output: hort
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_short_t  &_val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //1
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_short_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_short_t val[1]={0};
        status=cafeSchale.get(handle, DBR_TIME_SHORT, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  Get short (DBR_SHORT)
 *  \param pv input: pv name
 *  \param _val output: 2hort
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_short_t  &_val, dbr_short_t &alarmStatus, \
               dbr_short_t &alarmSeverity) { //1
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_short_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_short_t val[1]={0};
        status=cafeSchale.get(handle, DBR_STS_SHORT, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL){_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get short (DBR_SHORT)
 *  \param pv input: pv name
 *  \param _val output: short
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_short_t  &_val) { //1
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_short_t  &_val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_short_t val[1]={0};
        status=cafeSchale.get(handle, DBR_SHORT, val);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get float (DBR_FLOAT)
 *  \param pv input: pv name
 *  \param _val output:float
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_float_t  &_val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //2
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_float_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_float_t val[1]={0};
        status=cafeFrappuccino.get(handle, DBR_TIME_FLOAT, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get float (DBR_FLOAT)
 *  \param pv input: pv name
 *  \param _val output:float
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_float_t  &_val, dbr_short_t &alarmStatus, \
               dbr_short_t &alarmSeverity) { //2
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_float_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_float_t val[1]={0};
        status=cafeFrappuccino.get(handle, DBR_STS_FLOAT, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL){_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get float (DBR_FLOAT)
 *  \param pv input: pv name
 *  \param _val output:float
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_float_t  &_val) { //2
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_float_t  &_val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_float_t val[1]={0};
        status=cafeFrappuccino.get(handle, DBR_FLOAT, val);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get enum (DBR_ENUM)
 *  \param pv input: pv name
 *  \param _val output:dbr_enum_t (unsigned short)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_enum_t  &_val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //3
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_enum_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_enum_t val[1]={0};
        status=cafeEspresso.get(handle, DBR_TIME_ENUM, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get enum (DBR_ENUM)
 *  \param pv input: pv name
 *  \param _val output: dbr_enum_t (unsigned short)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_enum_t  &_val, dbr_short_t &alarmStatus, \
               dbr_short_t &alarmSeverity) { //3
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_enum_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_enum_t val[1]={0};
        status=cafeEspresso.get(handle, DBR_STS_ENUM, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL){_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get enum (DBR_ENUM)
 *  \param pv input: pv name
 *  \param _val output: dbr_enum_t (unsigned short)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_enum_t  &_val) { //3
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_enum_t  &_val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_enum_t val[1]={0};
        status=cafeEspresso.get(handle, DBR_ENUM, val);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get dbr_char_t (DBR_CHAR)
 *  \param pv input: pv name
 *  \param _val output:unsigned char (dbr_char_t)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_char_t  &_val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //4
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_char_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_char_t val[1]={0};
        status=cafeCappuccino.get(handle, DBR_TIME_CHAR, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get dbr_char_t (DBR_CHAR)
 *  \param pv input: pv name
 *  \param _val output:unsigned char (dbr_char_t)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_char_t  &_val, dbr_short_t &alarmStatus, \
               dbr_short_t &alarmSeverity) { //4
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_char_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_char_t val[1]={0};
        status=cafeCappuccino.get(handle, DBR_STS_CHAR, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL){_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get dbr_char_t (DBR_CHAR)
 *  \param pv input: pv name
 *  \param _val output: unsigned char (dbr_char_t)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_char_t  &_val) { //4
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_char_t  &_val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_char_t val[1]={0};
        status=cafeCappuccino.get(handle, DBR_CHAR, val);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get int  (DBR_LONG)
 *  \param pv input: pv name
 *  \param _val output:long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_long_t  &_val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //5
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_long_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_long_t val[1]={0};
        status=cafeLatte.get(handle, DBR_TIME_LONG, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get int  (DBR_LONG)
 *  \param pv input: pv name
 *  \param _val output:long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_long_t  &_val, dbr_short_t &alarmStatus, \
               dbr_short_t &alarmSeverity) { //5
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_long_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_long_t val[1]={0};
        status=cafeLatte.get(handle, DBR_STS_LONG, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL){_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get int  (DBR_LONG)
 *  \param pv input: pv name
 *  \param _val output:long
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_long_t  &_val) { //5
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_long_t  &_val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_long_t val[1]={0};
        status=cafeLatte.get(handle, DBR_LONG, val);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  Get long long
 *  \param pv input: pv name
 *  \param _val output:long long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, long long &_val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //5
#define __METHOD__ \
"int  CAFE::get(const char * pv, long long  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        long long val[1]={0};
        status=CAFE::get(handle, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get long long
 *  \param pv input: pv name
 *  \param _val output:long long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, long long &_val, dbr_short_t &alarmStatus, \
               dbr_short_t &alarmSeverity) { //5
#define __METHOD__ \
"int  CAFE::get(const char * pv, long long  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        long long val[1]={0};
        status=CAFE::get(handle, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL){_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get long long
 *  \param pv input: pv name
 *  \param _val output:long long
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, long long  &_val) { //5
#define __METHOD__ "int  CAFE::get(const char * pv, long long  &_val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        long long val[1]={0};
        status=CAFE::get(handle, val);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}



/**
 *  Get double (DBR_DOUBLE)
 *  \param pv input: pv name
 *  \param _val output: dbr_double_t (double)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_double_t  &_val,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) { //6
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_double_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_double_t val[1]={0};
        status=cafeDoppio.get(handle, DBR_TIME_DOUBLE, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get double (DBR_DOUBLE)
 *  \param pv input: pv name
 *  \param _val output: dbr_double_t (double)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_double_t  &_val, dbr_short_t &alarmStatus, \
               dbr_short_t &alarmSeverity) { //6
#define __METHOD__ \
"int  CAFE::get(const char * pv, dbr_double_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_double_t val[1]={0};
        status=cafeDoppio.get(handle, DBR_STS_DOUBLE, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL){_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Get double (DBR_DOUBLE)
 *  \param pv input: pv name
 *  \param _val output: dbr_double_t (double)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::get(const char * pv, dbr_double_t  &_val) { //6
#define __METHOD__ "int  CAFE::get(const char * pv, dbr_double_t  &_val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_double_t val[1]={0};
        status=cafeDoppio.get(handle, DBR_DOUBLE, val);
        if (status==ICAFE_NORMAL) {_val= val[0];}
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}


/**
 *  Helper function to methods calling scalars
 *  Sets number of elements to retrieve from cache to one
 *  \param handle input: handle to Conduit object
 *  \return nelements previous to the set
 */
unsigned int  CAFE::setNelemToRetrieveFromCacheToOne(const unsigned int  handle) {
#define __METHOD__ "int  CAFE::setNelemToRetrieveFromCacheToOne(const unsigned int  handle)"
    unsigned int  nelemPrevious, nelemRequestedCheck, nelemPreviousCache=0;
    unsigned int  nelemRequested=1;
    nelemPrevious=handleHelper.getNelemRequest(handle);
    nelemPreviousCache=handleHelper.getNelemToRetrieveFromCache(handle);
    //Check the number of elements requested?
    if (max(nelemPrevious,nelemPreviousCache)>1) {
        nelemRequestedCheck = handleHelper.setNelemToRetrieveFromCache(handle,nelemRequested);
        if (nelemRequestedCheck != nelemRequested) {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
                    << nelemPrevious << endl;
            cout << "to: " << nelemRequested << " but got instead: "
                    << nelemRequestedCheck  << endl;
        }
    }
    return nelemPrevious;
#undef __METHOD__
}
/**
 *  Helper function to methods calling scalars
 *  Sets number of elements to retrieve from cache to the "previous" number
 *  \param handle input: handle to Conduit object
 *  \param nelemPrevious input: nelem to set
 *  \return nelements actually set (should equal nelemPrevious)
 */
unsigned int  CAFE::setNelemToRetrieveFromCacheToPrevious(const unsigned int  handle, \
                                                          unsigned int  nelemPrevious) {
#define __METHOD__ \
"int  CAFE::setNelemToRetrieveFromCacheToPrevious(unsigned int handle, unsigned int nelemPrevious)"
    unsigned int  nelemPreviousCheck=0;
    unsigned int  nelemRequested=1;
    //Switch back to previous value
	if (nelemPrevious>0) {
        nelemPreviousCheck= handleHelper.setNelemToRetrieveFromCache(handle,nelemPrevious);
        if (nelemPreviousCheck != nelemPrevious) {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Internal CAFE FUNNY: Wanted to re-set the no. elements from: "
                    << nelemRequested << endl;
            cout << "to the previous: " << nelemPrevious << " but got instead: "
                    << nelemPreviousCheck  << endl;
        }
    }
    return nelemPreviousCheck;
#undef __METHOD__
}
/**
 *  Helper function to methods calling scalars
 *  Sets number of elements to one
 *  \param handle input: handle to Conduit object
 *  \return nelements previous to the set
 */
unsigned int  CAFE::setNelemToOne(const unsigned int  handle) {
#define __METHOD__ "int  CAFE::setNelemToOne(const unsigned int  handle)"
    unsigned int  nelemPrevious, nelemRequestedCheck=0;
    unsigned int  nelemRequested=1;
    nelemPrevious=handleHelper.getNelemClient(handle);

    //Check the number of elements requested?
    if (nelemPrevious>1) {
        nelemRequestedCheck = handleHelper.setNelem(handle,nelemRequested);
        if (nelemRequestedCheck != nelemRequested) {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
                    << nelemPrevious << endl;
            cout << "to: " << nelemRequested << " but got instead: "
                    << nelemRequestedCheck  << endl;
        }
    }
    return nelemPrevious;
#undef __METHOD__
}
/**
 *  \brief Helper function to methods calling scalars
 *  Sets number of elements to the "previous" number
 *  \param handle input: handle to Conduit object
 *  \param nelemPrevious input: nelem to set
 *  \return nelements actually set (should equal nelemPrevious)
 */
unsigned int  CAFE::setNelemToPrevious(const unsigned int  handle, \
                                       unsigned int  nelemPrevious) {
#define __METHOD__ \
    "int  CAFE::setNelemToPrevious(unsigned int  handle, unsigned int  nelemPrevious)"
    unsigned int  nelemPreviousCheck=nelemPrevious;
    unsigned int  nelemRequested=1;

    //Switch back to previous value
    //if (nelemPrevious>1) {
	//Apr 2015: Change from getNelemRequest(handle) to getNelemClient
	//This is because nelemClient(handle) also used by set methods!
	if(handleHelper.getNelemClient(handle)!= nelemPrevious) {
        nelemPreviousCheck= handleHelper.setNelem(handle,nelemPrevious);
        if (nelemPreviousCheck != nelemPrevious) {         
            if (nelemPreviousCheck != handleHelper.getNelemNative(handle)) {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Internal CAFE FUNNY: Wanted to re-set the no. elements from: "
                    << nelemRequested << endl;
            cout << "to the previous: " << nelemPrevious << " but got instead: "
                    << nelemPreviousCheck  << endl;
            cout << "No of native elements is " << handleHelper.getNelemNative(handle) << endl;
            }
        }
    }
    return nelemPreviousCheck;
#undef __METHOD__
}


/**
 *  Set long long
 *  \param handle input: handle to Conduit object
 *  \param val input: array of double
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(unsigned int handle, const long long * val) { //5+ long long
#define __METHOD__ \
    "CAFE::set(unsigned int handle, const long long * _val)"

    ChannelRegalia channelInfo;
    CAFE::getChannelInfo(handle, channelInfo);
    if ( channelInfo.getCafeConnectionState() == ICAFE_CS_NEVER_CONN) {
        return ICAFE_CS_NEVER_CONN;
    }
		else if ( channelInfo.getCafeConnectionState()==ICAFE_CS_CLOSED)  {
        return ICAFE_CS_CLOSED;
    }
		
    int  _status=ICAFE_NORMAL;
    unsigned int  nn=handleHelper.getNelemNative(handle);

    if ( channelInfo.getDataType() == DBR_STRING) {
        dbr_string_t * _sVal = new dbr_string_t[nn];

        //Convert long long to string;
        for (unsigned int  i=0; i < handleHelper.getNelemClient(handle); ++i) {
            std::stringstream ss;
            ss.clear();
            ss << val[i];
            strcpy(_sVal[i] , ss.str().c_str());
        }
        _status=cafeSoluble.set (handle, DBR_STRING,  _sVal);

        delete [] _sVal;
    }
    else if ( channelInfo.getDataType() == DBR_DOUBLE) {
        dbr_double_t * _dVal = new dbr_double_t[nn];
        for (unsigned int  i=0; i < handleHelper.getNelemClient(handle); ++i) {
         _dVal[i] = (double) val[i];
        }
        _status=cafeDoppio.set (handle, DBR_DOUBLE,  _dVal);

        delete [] _dVal;
    }
    else if ( channelInfo.getDataType() == DBR_FLOAT) {
        dbr_float_t * _fVal = new dbr_float_t[nn];
        for (unsigned int  i=0; i < handleHelper.getNelemClient(handle); ++i) {
         _fVal[i] = (float) val[i];
        }
        _status=cafeFrappuccino.set (handle, DBR_FLOAT,  _fVal);

        delete [] _fVal;
    }
    else {
        dbr_long_t * _lVal = new dbr_long_t[nn];
        for (unsigned int  i=0; i < handleHelper.getNelemClient(handle); ++i) {
         _lVal[i] = (dbr_long_t) val[i];
        }
        _status=cafeLatte.set (handle, DBR_LONG,  _lVal);

        delete [] _lVal;
    }
    return _status;
#undef __METHOD__
}

/**
 *  Set double(s)
 *  \param pv input: pv name
 *  \param val input: array of double
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_double_t * val) { //6
#define __METHOD__ "CAFE::set(const char *, dbr_double_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=CAFE::cafeDoppio.set((const unsigned int) handle, DBR_DOUBLE, val);}
    return status;
#undef __METHOD__
}
/**
 *  Set long long(s)
 *  \param pv input: pv name
 *  \param val input: array of long long
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const long long * val) { //5+ long long
#define __METHOD__ "CAFE::set(const char *, long long * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=CAFE::set((const unsigned int) handle, val);}
    return status;
#undef __METHOD__
}
/**
 *  Set dbr_long_t(s)
 *  \param pv input: pv name
 *  \param val input: array of dbr_long_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_long_t * val) { //5
#define __METHOD__ "CAFE::set(const char *, dbr_long_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=CAFE::cafeLatte.set((const unsigned int) handle, DBR_LONG, val);}
    return status;
#undef __METHOD__
}
/**
 *  Set unsigned char(s)
 *  \param pv input: pv name
 *  \param val input: array of dbr_char_t (unsigned char)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_char_t * val) { //4
#define __METHOD__ "CAFE::set(const char *, dbr_char_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=CAFE::cafeCappuccino.set((const unsigned int) handle, DBR_CHAR, val);}
    return status;
#undef __METHOD__
}
/**
 *  Set dbr_enum_t(s)
 *  \param pv input: pv name
 *  \param val input: array ofdbr_enum_t (unsigned short)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_enum_t * val) { //3
#define __METHOD__ "CAFE::set(const char *, dbr_enum_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=CAFE::cafeEspresso.set((const unsigned int) handle, DBR_ENUM, val);}
    return status;
#undef __METHOD__
}
/**
 *  Set float(s)
 *  \param pv input: pv name
 *  \param val input: array of float
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_float_t * val) { //2
#define __METHOD__ "CAFE::set(const char *, dbr_float_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=CAFE::cafeFrappuccino.set((const unsigned int) handle, DBR_FLOAT, val);}
    return status;
#undef __METHOD__
}
/**
 *  Set short(s)
 *  \param pv input: pv name
 *  \param val input: array of dbr_short_t (signed short)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_short_t * val) { //1
#define __METHOD__ "CAFE::set(const char *, dbr_short_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=CAFE::cafeSchale.set((const unsigned int) handle, DBR_SHORT, val);}
    return status;
#undef __METHOD__
}
/**
 *  Set dbr_string_t(s)
 *  \param pv input: pv name
 *  \param val input: array of dbr_string_t (char[40])
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_string_t * val) { //0
#define __METHOD__ "CAFE::set(const char *, dbr_string_t * val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) {
        status=CAFE::cafeSoluble.set((const unsigned int) handle, DBR_STRING, val);}
    return status;
#undef __METHOD__
}
/**
 *  Set dbr_double_t
 *  \param pv input: pv name
 *  \param _val input: dbr_double_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_double_t _val) { //6
#define __METHOD__ "CAFE::set(const char *, dbr_double_t  val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) { unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_double_t val[1]; val[0]= _val;
        status=CAFE::cafeDoppio.set((const unsigned int) handle, DBR_DOUBLE, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Set long long
 *  \param pv input: pv name
 *  \param _val input:long long
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const long long _val) { //5+ long long
#define __METHOD__ "CAFE::set(const char *, long long  val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) { unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_long_t val[1]; val[0]= _val;
        status=CAFE::set((const unsigned int) handle, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Set dbr_long_t
 *  \param pv input: pv name
 *  \param _val input: dbr_long_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_long_t _val) { //5
#define __METHOD__ "CAFE::set(const char *, dbr_long_t  val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) { unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_long_t val[1]; val[0]= _val;
        status=CAFE::cafeLatte.set((const unsigned int) handle, DBR_LONG, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Set dbr_char_t (unsigned char)
 *  \param pv input: pv name
 *  \param _val input: dbr_char_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_char_t _val) { //34
#define __METHOD__ "CAFE::set(const char *, dbr_char_t  val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) { unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_char_t val[1]; val[0]= _val;
        status=CAFE::cafeCappuccino.set((const unsigned int) handle, DBR_CHAR, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Set dbr_enum_t (unsigned short)
 *  \param pv input: pv name
 *  \param _val input: dbr_enum_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_enum_t _val) { //3
#define __METHOD__ "CAFE::set(const char *, dbr_enum_t  val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) { unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_enum_t val[1]; val[0]= _val;
        status=CAFE::cafeEspresso.set((const unsigned int) handle, DBR_ENUM, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Set dbr_float_t
 *  \param pv input: pv name
 *  \param _val input: dbr_float_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_float_t _val) { //1
#define __METHOD__ "CAFE::set(const char *, dbr_float_t  val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) { unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_float_t val[1]; val[0]= _val;
        status=CAFE::cafeFrappuccino.set((const unsigned int) handle, DBR_FLOAT, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Set dbr_short_t
 *  \param pv input: pv name
 *  \param _val input: dbr_short_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_short_t _val) { //0
#define __METHOD__ "CAFE::set(const char *, dbr_short_t  val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) { unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_short_t val[1]; val[0]= _val;
        status=CAFE::cafeSchale.set((const unsigned int) handle, DBR_SHORT, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  Set dbr_string_t
 *  \param pv input: pv name
 *  \param _val input: dbr_string_t (char[40])
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::set(const char * pv, const dbr_string_t _val) { //0
#define __METHOD__ "CAFE::set(const char *, dbr_string_t  val)"
    unsigned int  handle; status=ICAFE_NORMAL;
    try {status = open(pv,  handle);}
    catch (CAFEException_open & e) {return e.pvEx.statusCode;}
    if (status == ICAFE_NORMAL) { unsigned int  nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]; sprintf(val[0],"%s", _val);
        status=CAFE::cafeSoluble.set((const unsigned int) handle, DBR_STRING, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  waitForPut
 *  \param h input: Array of hanldes
 *  \param nelem input:No of elements in Array
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::waitForPut(const unsigned int *h, const unsigned int nelem) {

    ca_flush_io();
    status=ICAFE_NORMAL; bool isGood=true;
    int overallStatus=status;
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    ChannelTimeoutPolicy channelTimeoutPolicyPut;
    for (unsigned int  i=0; i<nelem; ++i) {
        it_handle = handle_index.find(h[i]);
        if (it_handle != handle_index.end()) {
            channelTimeoutPolicyPut = (*it_handle).getChannelTimeoutPolicyPut();
            status=cafeGranules.waitForPutEvent(h[i], channelTimeoutPolicyPut.getTimeout() );
            cout << "Handle = " << h[i] << " status = " << status << endl;
            if ( (status != ICAFE_NORMAL) && isGood) {
                overallStatus=status;
                isGood=false;
            }
        }
        else {
            overallStatus=ECAFE_INVALID_HANDLE;
            isGood=false;
        }
    }
    return overallStatus;
}


#if HAVE_PYTHON_H
/**
 *  loadSFGroups is a SwissFEL specific method (M. Aiba)
 *  \param suff input: suffix for Virtual Accelerator
 *  \return void
 */
void CAFE::loadSFGroups(string suff) {

  cout << " ---> reporting from: void CAFE::loadSFGroups(string suff) <--- " << endl;
 
  //Test for file directory
	string fName="";
  char * p= NULL;
  char const* d = "OMC.py";
    
  bool isFile=false;
	ifstream ifile;

  
	if(std::getenv("SFEL_LAYOUT_PYTHONPATH") == NULL){  
				cout << "SFEL_LAYOUT_PYTHONPATH is not defined, thus may be missing from PYTHONPATH" << endl;	
       //putenv((char *) "SFEL_LAYOUT_PYTHONPATH=/afs/psi.ch/intranet/SF/Applications/on-line_model/default/scripts/VA");	
			 //putenv((char *) "PYTHONPATH=/afs/psi.ch/intranet/SF/Applications/on-line_model/default/scripts/VA");
  }
		

	//if(std::getenv("PYTHONPATH") != NULL){  
  //    char * pp= std::getenv("PYTHONPATH");
	//		cout << "PYTHONPATH=" << pp << endl;			
  //}
  //if(std::getenv("SFEL_LAYOUT_PYTHONPATH") != NULL){  
  //    char * ll= std::getenv("PYTHONPATH");
	//		cout << "SFEL_LAYOUT_PYTHONPATH=" << ll << endl;			
  //}
	

  if(std::getenv("SFEL_OMC_PYTHONPATH") != NULL){  
       p = std::getenv("SFEL_OMC_PYTHONPATH");	
  }
		
  if (p != NULL) {
				
		fName.append(p);
		fName.append("/");
		fName.append(d);
			
		ifile.open(fName.c_str(),ios::in);
  		 
		if (ifile.good()) {
			ifile.close();
			isFile=true;
		}	
				
	}
	
	if (!isFile) {
		fName="";
		fName.append("/afs/psi.ch/intranet/SF/Applications/on-line_model/default/PythonModule") ;
	  std::cout << "Did not find file in $SFEL_OMC_PYTHONPATH " << std::endl;
		std::cout << "therefore using the following default directory:  " << std::endl;
		std::cout << fName.c_str() << std::endl;
				
		fName.append("/");
		fName.append(d);
		
		ifile.open(fName.c_str(),ios::in);
			 
		if (ifile.good()) {
			ifile.close();
			isFile=true;
		}	
		else {
			std::cout << "Error opening default file: " << fName.c_str() << std::endl; 		
		}					
	}
	
	// No luck with finding OMC.py
	if (!isFile) { return;}
		
	if (! Py_IsInitialized () ) {
	  cout << "INITIALIZING PY IN CAFE" << endl;
  	Py_Initialize();
	}
	
	
	//Py_Initialize();
	//PyEval_InitThreads();

	globalChannelList.clear();
	globalChannelList.reserve(9000);
	
  PyObject* psys = PyImport_ImportModule( "sys" );
	
  if (psys==NULL) {	
	  cout << "PSYS IS NULL " << endl;
	}
		
  PyObject* psys_path = PyObject_GetAttrString( psys, "path" );
	
  if (psys_path==NULL) {	
	  cout << "PSYS PATH IS NULL " << endl;
	}
		
  PyObject* folder_path = PyUnicode_FromString( fName.c_str()); //"/afs/psi.ch/intranet/SF/Applications/Development/OnlineModel/PythonModule" );
  
  if (folder_path==NULL) {	
	  cout << "FOLDER PATH IS NULL " << endl;
	}
 
  PyList_Append( psys_path, folder_path );
  
	//Py_DECREF(folder_path);


  // Load module, class and object
  PyObject *mymod, *pclass, *pobject;
  
	//cout <<  fName.c_str() << endl;
	//cout << " PY VERSION " << PY_MAJOR_VERSION << endl;
	
  mymod = PyImport_ImportModule("OMC");
	
		
  if (mymod==NULL) {	
	  cout << " MYMOD IS NULL. Printing Error: " << endl;
		 PyErr_Print() ;
		return;
	}
			
  pclass = PyObject_GetAttrString(mymod, "OMC");	
		 					
	if (pclass==NULL) {	
	 cout << " pClass IS NULL " << endl;
	  PyErr_Print() ;
		return;
	}			
				
  PyObject *pmethod, *pargs, *pret;
  pargs=NULL;
	
  //pobject = PyEval_CallObject(pclass,pargs);
	pobject = PyObject_CallObject(pclass,pargs);
	
	if (pobject==NULL) {	
	 cout << " pobject IS NULL " << endl;
	  PyErr_Print() ;
		return;
	}			
				
  pmethod = PyObject_GetAttrString(pobject, "initFunction");
	
	if (pmethod==NULL) {	
	 cout << " pobject IS NULL " << endl;
	  PyErr_Print() ;
		return;
	}		


  //Required for Python3
	pargs=Py_BuildValue("(s)", suff.c_str());
	//pargs=Py_BuildValue("s",(suff.c_str()));
	
	if (pargs==NULL) {
	 cout << " pargs IS NULL " << endl;
	  PyErr_Print() ;
	}	
	
		
	 // PyObject * temp = PyUnicode_AsEncodedString(pargs, "ASCII", "strict"); 
		//PyObject * my_pargs = PyBytes_AS_STRING(temp); // Borrowed pointer
    //my_pargs = strdup(my_pargs);

  //pret=PyEval_CallObject(pmethod,PyObject(pargs));

 
	//pret=PyObject_CallFunction(pmethod, "s", suff.c_str());	
	pret=PyObject_CallObject(pmethod, pargs);	
		
	
	if (pret==NULL) {
	 cout << " pret IS NULL /  " << endl;
	 PyErr_Print() ;
	 return;
	}	
	  
	
	//if (PyTuple_Check(pret)) {
	//	cout << "pret ia a TUPLE " << endl;
	//}

  string GroupName;
  vector<string> ChannelList;
	
  PyObject *plist1, *plist2, *plist3, *plist4;
  int i,j,li;
  long lj;
	
	//cout << "PyArg_ParseTuple(pret, 'iOOO', &li,&plist1, &plist2, &plist3);" << endl;
  if (!PyArg_ParseTuple(pret, "iOOO", &li,&plist1, &plist2, &plist3)) {
		cout << "is NULL " << endl;
		return;
	}
 
  	/*
	if (PyUnicode_Check(plist2)) {
    // Convert string to bytes.
    // strdup() bytes into my_result.
		cout << "UNICODE " << endl;
	} else if (PyBytes_Check(plist2)) {
    // strdup() bytes into my_result.
		cout << "BYTES " << endl;
	
	} else if (PyTuple_Check(plist2)) {
  
		cout << "TUPLE " << endl;
	} else if (PyList_Check(plist2)) {
  
		cout << "List " << endl;
	}
	else if (PyByteArray_Check(plist2)) {
   
		cout << "PyByteArray " << endl;
	}
	
		else if (PyDict_Check(plist2)) {
   
		cout << "PyDict " << endl;
	}
	else if (PyDictKeys_Check(plist2)) {
   
		cout << "PyDict_Keys " << endl;
	}
	
		else if (PyMemoryView_Check(plist2)) {
   
		cout << "PymemoryView " << endl;
	}
		else if (PyModule_Check(plist2)) {
   
		cout << "PyModule " << endl;
	}
	
	
	else if (PyCell_Check(plist2)) {
    // strdup() bytes into my_result.
		cout << "PyCell " << endl;
	}
	
	else {
    // Convert into your favorite string representation.
    // Convert string to bytes if it is not already.
    // strdup() bytes into my_result.
		cout << "SOMEOT ELSE " << endl;
	}
 
 
	if (PyList_Check(plist1)) {
    // strdup() bytes into my_result.
		cout << "PyList check for plist1 is OK " << endl;
	}
 
	if (PyDictValues_Check(plist3)) {
    // strdup() bytes into my_result.
		cout << "PyList check for plist1 is OK " << endl;
	}
 */
 
  //Py_ssize_t pli;
  //pli=PyList_Size(plist1); // This causes segfault for some reason...
  //printf("length%i\n",li);
  PyObject *ps, *pyi;
  string mystr;

  //Check type of pList1 etc...

  if (plist1 == NULL ) {
	cout << "pList1 is NULL" << endl;
	}
	if (plist2 == NULL ) {
	cout << "pList2 is NULL" << endl;
	}
	if (plist3 == NULL ) {
	cout << "pList3 is NULL" << endl;
	}

//if HAVE_LIBQTXML
  CAFE::openGroupXMLFile("/tmp/test.xml");
//endif
 
  //cout << PY_MAJOR_VERSION << endl;
 
 
 
  for (i=0; i<li; i++){

    ps=PyList_GetItem(plist2,i);
		if (ps == NULL) {
		 cout << " ps is NULL; indx in loop=" << i << endl; 
		 PyErr_Print() ;
		 }
	
		#if PY_MAJOR_VERSION >= 3		
    	 GroupName=PyUnicode_AsUTF8(ps);
		#elif PY_MAJOR_VERSION < 3
			 GroupName=PyString_AsString(ps);
		#endif
			
    plist4=PyList_GetItem(plist3,i); // Corresponding channel list
    //lj=PyList_Size(plist3);
		
    pyi=PyList_GetItem(plist1,i);
    lj=PyLong_AsLong(pyi);
      //cout << " GROUP " << i << " of " << li << endl;
			// printf("Group Name:  %s has %d members\n",GroupName.c_str(), lj);
   // printf("Group Name:  %s\n",GroupName.c_str());
	 	ChannelList.clear();
    for (j=0; j<lj; j++){
      ps=PyList_GetItem(plist4,j);
			#if PY_MAJOR_VERSION >= 3				
    	 mystr=PyUnicode_AsUTF8(ps); 
		  #elif PY_MAJOR_VERSION < 3			
			 mystr=PyString_AsString(ps);
		  #endif
	
      //printf("Channel Name:  %s\n",mystr.c_str());
			//printf("Channel %d Name:   %s\n",j, mystr.c_str());
      ChannelList.push_back (mystr);
			globalChannelList.push_back(mystr);
		
    }
    //cafefunction.grouping(GroupName,ChannelList);
		status=CAFE::groupDefine(GroupName.c_str(), ChannelList);
		if (status != ICAFE_NORMAL) {
			CAFE::printStatusMessage(status);
		}
//if HAVE_LIBQTXML
		 CAFE::group2XML(GroupName.c_str(), "/tmp/test.xml");
//endif		 	 
  }

//if HAVE_LIBQTXML 
  CAFE::closeGroupXMLFile("/tmp/test.xml");
//endif	 	
	/*
	cout << "psys = " << (psys->ob_refcnt) << endl;
	cout << "psys_path = " << (psys_path->ob_refcnt) << endl;
	cout << "folder_path = " << (folder_path->ob_refcnt) << endl;
	
	cout << "mymod = " << (mymod->ob_refcnt) << endl;
	cout << "pclass = " <<(pclass->ob_refcnt) << endl;
	cout << "pobject = " << (pobject->ob_refcnt) << endl;
	
	cout << "pmethod = " << (pmethod->ob_refcnt) << endl;
	cout << "pargs = " << (pargs->ob_refcnt) << endl;
	cout << "pret = " <<(pret->ob_refcnt) << endl;
	
	cout << "plist1 = " << (plist1->ob_refcnt) << endl;
	cout << "plist2 = " << (plist2->ob_refcnt) << endl;
	cout << "plist3 = " << (plist3->ob_refcnt) << endl;

	cout << "plist4 = " << (plist4->ob_refcnt) << endl;
	cout << "ps = " << (ps->ob_refcnt) << endl;
	cout << "pyi = " << (pyi->ob_refcnt) << endl;
  
	*/
		
	
		
	Py_DECREF(psys);
	Py_DECREF(psys_path);	
	Py_DECREF(folder_path);

	
  Py_DECREF(mymod);
  Py_DECREF(pclass);
	Py_DECREF(pobject);
	
  

	
	if (pargs !=NULL) {
	   	Py_DECREF(pargs);	
	}

	
  Py_DECREF(pret);


	//Cannot Py_DECREF(pmethod) in Python3 !	
	//if (pmethod !=NULL)  {	
	//	Py_DECREF(pmethod);			
	//}
	
		
	//Py_DECREF(plist1);
	//Py_DECREF(plist2);	
	//Py_DECREF(plist3);	
	
	//Py_DECREF(plist4); //RELATED TO PLIST3
  //Py_DECREF(ps); //RELATED TO PLIST2	
  //Py_DECREF(pyi); //related to plist1
  	
	
	//Py_Finalize(); // No!
	
	/*
	//Note some of these objects will no longer exist after decref
	cout << "psys = " << (psys->ob_refcnt) << endl;
	cout << "psys_path = " << (psys_path->ob_refcnt) << endl;
	cout << "folder_path = " << (folder_path->ob_refcnt) << endl;
	
	cout << "mymod = " << (mymod->ob_refcnt) << endl;
	cout << "pclass = " <<(pclass->ob_refcnt) << endl;
	cout << "pobject = " << (pobject->ob_refcnt) << endl;
	
	cout << "pmethod = " << (pmethod->ob_refcnt) << endl;
	cout << "pargs = " << (pargs->ob_refcnt) << endl;
	cout << "pret = " <<(pret->ob_refcnt) << endl;
	
	cout << "plist1 = " << (plist1->ob_refcnt) << endl;
	cout << "plist2 = " << (plist2->ob_refcnt) << endl;
	cout << "plist3 = " << (plist3->ob_refcnt) << endl;

	cout << "plist4 = " << (plist4->ob_refcnt) << endl;
	cout << "ps = " << (ps->ob_refcnt) << endl;
	cout << "pyi = " << (pyi->ob_refcnt) << endl;
*/
  	

	return;
}
#endif



/*
#if HAVE_PYTHON_H
void CAFE::loadSFGroups(string suff) {

  Py_Initialize();
	PyEval_InitThreads();
	
	cout << " HERE " << endl;
	
  PyObject* psys = PyImport_ImportModule( "sys" );
	// cout << " HERE2a " << endl;
  PyObject* psys_path = PyObject_GetAttrString( psys, "path" );
  PyObject* folder_path = PyUnicode_FromString( "/afs/psi.ch/intranet/SF/Applications/Development/OnlineModel/PythonModule" );
  //PyObject* folder_path = PyUnicode_FromString( "/afs/psi.ch/intranet/SF/Applications/Development/Masa/git/PythonModule" );
  PyList_Append( psys_path, folder_path );

  // Load module, class and object
  PyObject *mymod, *pclass, *pobject;
  cout << " HERE2b " << endl;
  mymod = PyImport_ImportModule("OMC");

	 cout << " HERE2c " << endl;
  pclass = PyObject_GetAttrString(mymod, "OMC");
		 cout << " HERE2d " << endl;
  PyObject *pmethod, *pargs, *pret;
  pargs=NULL;
  pobject = PyEval_CallObject(pclass,pargs);
	 cout << " HERE3 " << endl;
  pmethod = PyObject_GetAttrString(pobject, "initFunction");
  pargs=Py_BuildValue("(s)", suff.c_str());
	//	 cout << " HERE4 " << endl;
  pret=PyEval_CallObject(pmethod,pargs);
	// cout << " HERE5 " << endl;
  string GroupName;
  vector<string> ChannelList;

  PyObject *plist1, *plist2, *plist3, *plist4;
  int i,j,li;
  long lj;
  PyArg_ParseTuple(pret, "iOOO", &li,&plist1, &plist2, &plist3);
 // cout << " HERE6 " << endl;
  //Py_ssize_t pli;
  //pli=PyList_Size(plist1); // This causes segfault for some reason...
  //printf("length%i\n",li);
  PyObject *ps, *pyi;
  string mystr;



  CAFE::openGroupXMLFile("/tmp/test.xml");
 //cout << " HERE7 " << endl;
  for (i=0; i<li; i++){
    ps=PyList_GetItem(plist2,i);
    GroupName=PyString_AsString(ps);
    plist4=PyList_GetItem(plist3,i); // Corresponding channel list
    //lj=PyList_Size(plist3);
    pyi=PyList_GetItem(plist1,i);
    lj=PyLong_AsLong(pyi);
      //cout << " GROUP " << i << " of " << li << endl;
			// printf("Group Name:  %s has %d members\n",GroupName.c_str(), lj);
   // printf("Group Name:  %s\n",GroupName.c_str());
	 	ChannelList.clear();
    for (j=0; j<lj; j++){
      ps=PyList_GetItem(plist4,j);
      mystr= PyString_AsString(ps);
      //printf("Channel Name:  %s\n",mystr.c_str());
			 // printf("Channel %d Name:   %s\n",j, mystr.c_str());
      ChannelList.push_back (mystr);
    }
    //cafefunction.grouping(GroupName,ChannelList);
		status=CAFE::groupDefine(GroupName.c_str(), ChannelList);
		if (status != ICAFE_NORMAL) {
			CAFE::printStatusMessage(status);
		}
		 CAFE::group2XML(GroupName.c_str(), "/tmp/test.xml");
  }

  CAFE::closeGroupXMLFile("/tmp/test.xml");
  // cout << " HERE8 " << endl;
  Py_DECREF(psys);
	Py_DECREF(psys_path);	
	Py_DECREF(folder_path);
  Py_DECREF(mymod);
  Py_DECREF(pclass);	
	if (pargs !=NULL) {
  	Py_DECREF(pargs);
	}
  Py_DECREF(pobject);	
  Py_DECREF(pmethod);
  Py_DECREF(pret);	
	Py_DECREF(plist1);
	Py_DECREF(plist2);
	Py_DECREF(plist3);
	Py_DECREF(plist4);
  Py_DECREF(ps);
  Py_DECREF(pyi);
  //cout << " HERE9 " << endl;
	return;
}
#endif
*/

