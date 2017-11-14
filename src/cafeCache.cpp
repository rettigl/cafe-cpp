///
/// \file    cafeCache.cc
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#include "cafe.h"

/**
 *  \fn int CAFE::getCache(const unsigned int  *handleArray, unsigned int  nelem, long long * val,
 *    int *statusArray, dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts)
 *  \brief Retrieves data (long long) from cache
 *  \param handleArray input: Array of handles to Conduit object
 *  \param nelem input: No. of array elements
 *  \param val output: Array of long longs containing cached data
 *  \param statusArray output: Array of status'
 *  \param alarmStatus output: Alarm Status
 *  \param alarmSeverity output: Corresponding severity of alarm
 *  \param ts output: epics timestamp struct
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int CAFE::getCache(const unsigned int  *handleArray, unsigned int  nelem, long long * val,
     int *statusArray, dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts) {
#define __METHOD__ \
    "CAFE::getCache(*handleArray, nelem, * val, *statusArray, *alarmStatus, *alarmSeverity, *ts"

    unsigned int * nelemPrevious = new unsigned int [nelem];

    for (unsigned int  i=0; i < nelem; ++i) {
        nelemPrevious[i] =CAFE::setNelemToRetrieveFromCacheToOne(handleArray[i]);
    }

    //Whatever is already there!
    //find biggest data type
    //what is the highest native type?

    chtype chtMax=DBF_CHAR;
    ChannelRegalia channelInfo;

     unsigned short nbyteChType=1;   unsigned short nbyteMax=nbyteChType;

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

        if ( channelInfo.getCafeConnectionState() != ICAFE_CS_NEVER_CONN  && channelInfo.getCafeConnectionState() != ICAFE_CS_CLOSED) {
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

    switch (chtMax) {
    case DBR_STRING:{
        dbr_string_t * _sVal = new dbr_string_t[nelem];
        status=cafeSoluble.getCache(handleArray, nelem, DBR_TIME_STRING, _sVal, statusArray,
                                   alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {
            istringstream ss;
            for (unsigned int  i=0; i < nelem; ++i) {
                long long l=0;
                ss.clear();
                ss.str(_sVal[i]);
                ss>>l;

                if ( !ss.fail()) {
                    val[i] = (long  long) l;
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
                            << " of " << nelem << " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << _sVal[i];
                    cout << " TO LONG LONG ; CONVERSION NOT MADE!" << endl;
                    cout << "Returning value 0 for element " << i << endl;
                    cout << "Status value is ECAFE_NO_CONVERT for element " << i << endl;
                    val[i]=0;
                    statusArray[i]=ECAFE_NO_CONVERT;
                }           
            }
        }
        delete [] _sVal;
        break;
        }
    case DBR_DOUBLE:{
        dbr_double_t * _dVal = new dbr_double_t[nelem];
        status=cafeDoppio.getCache(handleArray, nelem, DBR_TIME_DOUBLE, _dVal, statusArray,
                                   alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {
            for (unsigned int  i=0; i < nelem; ++i) {
             val[i] = (long  long) _dVal[i];
            }
        }
        delete [] _dVal;
        break;
        }
    case DBR_FLOAT: {
        dbr_float_t * _fVal = new dbr_float_t[nelem];
        status=cafeFrappuccino.getCache(handleArray, nelem, DBR_TIME_FLOAT, _fVal, statusArray,
                                   alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {
            for (unsigned int  i=0; i < nelem; ++i) {
             val[i] = (long  long) _fVal[i];
            }
        }
        delete [] _fVal;
        break;
        }
    case DBR_LONG: {
        dbr_long_t * _lVal = new dbr_long_t[nelem];
        status=cafeLatte.getCache(handleArray, nelem, DBR_TIME_LONG, _lVal, statusArray,
                                   alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL) {
            for (unsigned int  i=0; i < nelem; ++i) {
             val[i] = (long  long) _lVal[i];
            }
        }
        delete [] _lVal;
        break;
        }

    }//switch

    for (unsigned int  i=0; i < nelem; ++i) {
        CAFE::setNelemToRetrieveFromCacheToPrevious(handleArray[i],nelemPrevious[i]);
    }

    return status;
#undef __METHOD__
};

/** \fn int  CAFE::getCache(const unsigned int *handleArray, unsigned int nelem, long long * val,
 *                   int *statusArray, dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity)
 *  \brief Retrieves data (long long) from cache
 *  \param handleArray input: Array of handles to Conduit object
 *  \param nelem input: No. of array elements
 *  \param val output: Array of long longs containing cached data
 *  \param statusArray output: Array of status'
 *  \param alarmStatus output: Alarm Status
 *  \param alarmSeverity output: Corresponding severity of alarm
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int  CAFE::getCache(const unsigned int *handleArray, unsigned int nelem, long long * val,
                    int *statusArray, dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity) {
    epicsTimeStamp *ts = new epicsTimeStamp[nelem];
    status=CAFE::getCache(handleArray, nelem, val, statusArray, alarmStatus, alarmSeverity, ts);
    delete [] ts;
    return status;
};

/**
 *  \brief Retrieves data (long long) from cache
 *  \param handleArray input: Array of handles to Conduit object
 *  \param nelem input: No of array elements
 *  \param val output: Array of long longs containing cached data
 *  \param statusArray output: Array of status'
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int  CAFE::getCache (const unsigned int *handleArray, unsigned int nelem, long long * val,
                     int  *statusArray) {
    dbr_short_t *alarmStatus; dbr_short_t *alarmSeverity; epicsTimeStamp *ts;
    ts = new epicsTimeStamp[nelem];
    alarmStatus = new dbr_short_t[nelem];
    alarmSeverity = new dbr_short_t[nelem];
    status=CAFE::getCache(handleArray, nelem, val, statusArray, alarmStatus, alarmSeverity, ts);
    delete [] ts; delete [] alarmStatus; delete [] alarmSeverity;
    return status;
}

/**
 *  \brief Retrieves data (long long) from cache
 *  \param handle input: Handle Conduit object
 *  \param _val output: Array of long longs containing cached data
 *  \param alarmStatus output: Alarm Status
 *  \param alarmSeverity output: Corresponding severity of alarm
 *  \param ts output: epics timestamp struct
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
//5+ long long getCache
int  CAFE::getCache(const unsigned int handle, long long * _val, \
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) {
#define __METHOD__ \
    "CAFE::getCache(const unsigned int handle, long long * _val,  \
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"

    ChannelRegalia channelInfo;
    CAFE::getChannelInfo(handle, channelInfo);
    if ( channelInfo.getCafeConnectionState() == ICAFE_CS_NEVER_CONN) {
        return ICAFE_CS_NEVER_CONN;
    }
		else if ( channelInfo.getCafeConnectionState()==ICAFE_CS_CLOSED)  {
        return ICAFE_CS_CLOSED;
    }
		
    int  _status=ICAFE_NORMAL;
    unsigned int  nn=handleHelper.getNelemRequest(handle);


    if ( channelInfo.getDataType() == DBR_STRING) {
        dbr_string_t * _sVal = new dbr_string_t[nn];
        _status=cafeSoluble.getCache (handle, DBR_TIME_STRING,  _sVal, alarmStatus, alarmSeverity, ts);
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
        _status=cafeDoppio.getCache (handle, DBR_TIME_DOUBLE,  _dVal, alarmStatus, alarmSeverity, ts);
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _dVal[i];
        }
        delete [] _dVal;
    }
    else if ( channelInfo.getDataType() == DBR_FLOAT) {
        dbr_float_t * _fVal = new dbr_float_t[nn];
        _status=cafeFrappuccino.getCache (handle, DBR_TIME_FLOAT,  _fVal, alarmStatus, alarmSeverity, ts);
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _fVal[i];
        }
        delete [] _fVal;
    }
    else {
        dbr_long_t * _lVal = new dbr_long_t[nn];
        _status=cafeLatte.getCache (handle, DBR_TIME_LONG,  _lVal, alarmStatus, alarmSeverity, ts);
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _lVal[i];
        }
        delete [] _lVal;
    }
    return _status;
#undef __METHOD__
};

/**
 *  \brief Retrieves data (long long) from cache
 *  \param handle input: Handle Conduit object
 *  \param _val output: Array of long longs containing cached data
 *  \param alarmStatus output: Alarm Status
 *  \param alarmSeverity output: Corresponding severity of alarm
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int  CAFE::getCache(const unsigned int  handle, long long   * _val, \
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity) {
#define __METHOD__ \
    "CAFE::getCache(const unsigned int handle, long long * _val, dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity)"
    ChannelRegalia channelInfo;
    CAFE::getChannelInfo(handle, channelInfo);
    if ( channelInfo.getCafeConnectionState() == ICAFE_CS_NEVER_CONN) {
        return ICAFE_CS_NEVER_CONN;
    }
		else if ( channelInfo.getCafeConnectionState()==ICAFE_CS_CLOSED)  {
        return ICAFE_CS_CLOSED;
    }
    int  _status=ICAFE_NORMAL;
    unsigned int  nn=handleHelper.getNelemRequest(handle);


    if ( channelInfo.getDataType() == DBR_STRING) {
        dbr_string_t * _sVal = new dbr_string_t[nn];
        _status=cafeSoluble.getCache (handle, DBR_STS_STRING,  _sVal, alarmStatus, alarmSeverity);
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
        _status=cafeDoppio.getCache(handle, DBR_STS_DOUBLE,  _dVal, alarmStatus, alarmSeverity);
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _dVal[i];
        }
        delete [] _dVal;
    }
    else if ( channelInfo.getDataType() == DBR_FLOAT) {
        dbr_float_t * _fVal = new dbr_float_t[nn];
        _status=cafeFrappuccino.getCache (handle, DBR_STS_FLOAT,  _fVal, alarmStatus, alarmSeverity);
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _fVal[i];
        }
        delete [] _fVal;
    }
    else {
        dbr_long_t * _lVal = new dbr_long_t[nn];
        _status=cafeLatte.getCache (handle, DBR_STS_LONG,  _lVal, alarmStatus, alarmSeverity);
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _lVal[i];
        }
        delete [] _lVal;
    }
    return _status;
 #undef __METHOD__
};


/**
 *  \brief Retrieves data (long long) from cache
 *  \param handle input: Handle Conduit object
 *  \param _val output: Array of long longs containing cached data
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int  CAFE::getCache(const unsigned int handle, long long * _val){
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
    unsigned int  nn=handleHelper.getNelemRequest(handle);

    if ( channelInfo.getDataType() == DBR_STRING) {
        dbr_string_t * _sVal = new dbr_string_t[nn];
        _status=cafeSoluble.getCache (handle, DBR_STRING,  _sVal);
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
        _status=cafeDoppio.getCache (handle, DBR_DOUBLE,  _dVal);
        if (_status!=ICAFE_NORMAL) { delete [] _dVal; return _status;}
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _dVal[i];
        }
        delete [] _dVal;
    }
    else if ( channelInfo.getDataType() == DBR_FLOAT) {
        dbr_float_t * _fVal = new dbr_float_t[nn];
        _status=cafeFrappuccino.getCache (handle, DBR_FLOAT,  _fVal);
        if (_status!=ICAFE_NORMAL) { delete [] _fVal; return _status;}
        for (unsigned int  i=0; i < handleHelper.getNelemRequest(handle); ++i) {
         _val[i] = (long  long) _fVal[i];
        }
        delete [] _fVal;
    }
    else {
        dbr_long_t * _lVal = new dbr_long_t[nn];
        _status=cafeLatte.getCache (handle, DBR_LONG,  _lVal);
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
 *  \brief Retrieves data from cache, packaged into the PVDataHolder class
 *  \param handleArray input: Array of handles to Conduit object
 *  \param nelem input: No of array elements
 *  \param pvdArray outout: Array of PVDataHolder containing cached data
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int  CAFE::getCache(const unsigned int * handleArray, unsigned int nelem, PVDataHolder * pvdArray){
#define __METHOD__ \
    "CAFE::getCache(unsigned int  * handleArray, unsigned int  nelem, PVDataHolder * pvdArray)"

    int  overallStatus=ICAFE_NORMAL; bool isGood=true;
    for (unsigned int  i=0; i<nelem; ++i) {

        status=getCache(handleArray[i],  pvdArray [i]);        
        if ( (status != ICAFE_NORMAL) && isGood) {            
            overallStatus=status;
            isGood=false;
        }
    }
    return overallStatus;
#undef __METHOD__
};




/**
 *  \brief Retrieves data from cache, packaged into the PVDataHolder class
 *  \param handle input: handle to Conduit object
 *  \param pvd outout: PVDataHolder containing cached data
 *  \return ICAFE_NORMAL if all OK else ICAFE_WAITING_FOR_PREV_CALLBACK;
 */
int  CAFE::getCache(const unsigned int  handle, PVDataHolder & pvd) {
#define __METHOD__ "CAFE::getCache(unsigned int  handle, PVDataHolder & pvd)"

	status=ICAFE_NORMAL;

	cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
	cafeConduit_set_by_handle::iterator it_handle;
	it_handle = handle_index.find(handle);

	if (it_handle != handle_index.end()) {

		//meant for use in callbacks in monitors!
		if ( (*it_handle).getChannelGetCacheWaitPolicy().getWaitKind()	== CAFENUM::GET_CACHE_NO_CHECK) {
			(*it_handle).getPVDataHolder(pvd);
			return status;
		}

		//Whatever is already there!
		chtype dbrtype= (*it_handle).getChannelRequestMetaData().getDbrDataType();

		//Find Native DataType
		if ( (status=cafeGranules.channelVerifyGet(handle, dbrtype)) != ICAFE_NORMAL) {
			(*it_handle).getPVDataHolder(pvd);
			return status;
		}


    //Add CAFENUM::WITH_CALLBACK_USER_SUPPLIED 
		if ((*it_handle).getChannelRequestStatusGet().getCallbackProgressKind() == CAFENUM::PENDING 
						&& handleHelper.getNmonitorData(handle)==0
					 	&& (*it_handle).getChannelRequestPolicyGet().getMethodKind()
						   != CAFENUM::WITH_CALLBACK_USER_SUPPLIED 
						&& (*it_handle).getChannelGetCacheWaitPolicy().getWaitKind()	== CAFENUM::GET_CACHE_WAIT
						&& (*it_handle).isConnected() ) {

			ChannelTimeoutPolicy channelTimeoutPolicyGet = (*it_handle).getChannelTimeoutPolicyGet();

			ca_poll();

			status=cafeGranules.waitForGetEvent(handle,
								channelTimeoutPolicyGet.getTimeout() );

			if (status==ECAFE_TIMEOUT && channelTimeoutPolicyGet.getSelfGoverningTimeout()) {
				unsigned short ntries=0;

				while (status==ECAFE_TIMEOUT && ntries<channelTimeoutPolicyGet.getNtries()) {
					status=cafeGranules.waitForGetEvent(handle, channelTimeoutPolicyGet.getTimeout() +
									  channelTimeoutPolicyGet.getDeltaTimeout()*(++ntries));
					cout << __METHOD__ << " status= " << status << endl;
				}


				if ((*it_handle).isConnected()) {

					std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
					std::cout << "No of additional ca_pend_io tries=" << ntries << std::endl;
					if (status==ECA_TIMEOUT) {
						std::cout << "is the MAXIMUM allowed as configured through TimeoutPolicy! "  << std::endl;
						std::cout << "TURNING OFF SELF-GOVERNING TIMEOUT FOR GET OPERATIONS FOR THIS CHANNEL"  << std::endl;
						channelTimeoutPolicyGet.setSelfGoverningTimeout(false);
						channelTimeoutPolicyGet.setTimeout( channelTimeoutPolicyGet.getTimeout() );
						std::cout << "AND TIMEOUT RESTORED TO START VALUE OF " << channelTimeoutPolicyGet.getTimeout() << endl;
					}
					else {
						std::cout <<  "Changing timeout for handle/pv "
							<< handle << "/" << (*it_handle).getPV() << " to: "  <<
						(channelTimeoutPolicyGet.getTimeout() +
						 channelTimeoutPolicyGet.getDeltaTimeout()*ntries) << endl;

						//modify timeout for handle
						channelTimeoutPolicyGet.setTimeout( (channelTimeoutPolicyGet.getTimeout() +
												  channelTimeoutPolicyGet.getDeltaTimeout()*ntries));
					}

					if(MUTEX){cafeMutex.lock();}    //lock
					handle_index.modify(it_handle, change_channelTimeoutPolicyGet(channelTimeoutPolicyGet));
					if(MUTEX){cafeMutex.unlock();}  //unlock

				}
			}


			if (!(*it_handle).isConnected() || status != ICAFE_NORMAL) {

				if(MUTEX){cafeMutex.lock();}  //lock
				ChannelRequestStatus channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
				channelRequestStatusGet.setCallbackStatus    (status);
				channelRequestStatusGet.setCallbackKind(false, false); // NOT_INITIATED NOT_TRIGGERED

				handle_index.modify(it_handle,
					change_channelRequestStatusGet(channelRequestStatusGet));

				if (!(*it_handle).isConnected()) {
					handle_index.modify(it_handle, change_status(ICAFE_CS_DISCONN));
					if(MUTEX){cafeMutex.unlock();
					 (*it_handle).getPVDataHolder(pvd);
					}//unlock
					return ICAFE_CS_DISCONN;
				}
				else {
					handle_index.modify(it_handle, change_status(status));
					if(MUTEX){cafeMutex.unlock();}//unlock
				}

			   cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			   cafeStatus.report(status);
			   //cout << "Method returning ICAFE_WAITING_FOR_PREV_CALLBACK " << endl;
			   (*it_handle).getPVDataHolder(pvd);
			   return ICAFE_WAITING_FOR_PREV_CALLBACK;
			}
		}

	   //For Monitors
	   (*it_handle).getPVDataHolder(pvd);

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
 *  \brief reset callback for set operation to NOT_INITIATED / NOT_TRIGGERED
 *  \param handle input: handle to Conduit object
 *  \return ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int CAFE::resetCallbackPut(const unsigned int handle) {
#define __METHOD__ "CAFE::resetCallbackPut(unsigned int)"
	status=ICAFE_NORMAL;

	cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
	cafeConduit_set_by_handle::iterator it_handle;
	it_handle = handle_index.find(handle);

	if (it_handle != handle_index.end()) {
		if(MUTEX){cafeMutex.lock();}  //lock
		ChannelRequestStatus channelRequestStatusPut = (*it_handle).getChannelRequestStatusPut();
		channelRequestStatusPut.setCallbackStatus    (status);
		channelRequestStatusPut.setCallbackKind(false, false); // NOT_INITIATED NOT_TRIGGERED

		handle_index.modify(it_handle,
							change_channelRequestStatusPut(channelRequestStatusPut));

		if(MUTEX){cafeMutex.unlock();}//unlock

	}
	else {
		if (printErrorPolicy.getInvalidHandle()) {	
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cafeStatus.report(ECAFE_INVALID_HANDLE);
		}	

		return ECAFE_INVALID_HANDLE;
	}

#undef __METHOD__
}



/**
 *  \brief reset callback for get operation to NOT_INITIATED / NOT_TRIGGERED
 *  \param handle input: handle to Conduit object
 *  \return ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int CAFE::resetCallbackGet(const unsigned int handle) {
#define __METHOD__ "CAFE::resetCallbackGet(unsigned int)"
	status=ICAFE_NORMAL;

	cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
	cafeConduit_set_by_handle::iterator it_handle;
	it_handle = handle_index.find(handle);

	if (it_handle != handle_index.end()) {
		if(MUTEX){cafeMutex.lock();}  //lock
		ChannelRequestStatus channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
		channelRequestStatusGet.setCallbackStatus    (status);
		channelRequestStatusGet.setCallbackKind(false, false); // NOT_INITIATED NOT_TRIGGERED

		handle_index.modify(it_handle,
							change_channelRequestStatusGet(channelRequestStatusGet));

		if(MUTEX){cafeMutex.unlock();}//unlock

	}
	else {
		if (printErrorPolicy.getInvalidHandle()) {	
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cafeStatus.report(ECAFE_INVALID_HANDLE);
		}
		return ECAFE_INVALID_HANDLE;
	}

#undef __METHOD__
}


/**
 *  \brief Retrieves data from cache, packaged into the PVDataHolder class
 *  \param handle input: handle to Conduit object
 *  \param pvd outout: PVDataHolder containing cached data
 *  \return ICAFE_NORMAL if all OK
 */
int  CAFE::getCacheNoWait(const unsigned int  handle, PVDataHolder & pvd) {
#define __METHOD__ "CAFE::getCacheNoWait(unsigned int  handle, PVDataHolder & pvd)"

	//status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end()) {

        //Whatever is already there!
        chtype dbrtype= (*it_handle).getChannelRequestMetaData().getDbrDataType();

        //Find Native DataType
        if ( (status=cafeGranules.channelVerifyGet(handle, dbrtype)) != ICAFE_NORMAL) {
            (*it_handle).getPVDataHolder(pvd);          
            return status;
        }


			if (!(*it_handle).isConnected() ) {

				if(MUTEX){cafeMutex.lock();}  //lock
				ChannelRequestStatus channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
				channelRequestStatusGet.setCallbackStatus    (status);
				channelRequestStatusGet.setCallbackKind(false, false); // NOT_INITIATED NOT_TRIGGERED

				handle_index.modify(it_handle,
					change_channelRequestStatusGet(channelRequestStatusGet));

				handle_index.modify(it_handle, change_status(ICAFE_CS_DISCONN));
				if(MUTEX){cafeMutex.unlock();}//unlock

				(*it_handle).getPVDataHolder(pvd);

				cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(ICAFE_CS_DISCONN);
				return ICAFE_CS_DISCONN;

			}



       (*it_handle).getPVDataHolder(pvd);

	//cout << __METHOD__ << " pvd.getStatus()  STATUS ===> " << pvd.getStatus() << endl;

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

	return pvd.getStatus();
#undef __METHOD__
};



/**
 *  \brief Retrieves PV DBR_CTRL data from cache, packaged into the PVCtrlHolder class
 *  \param handleArray input: Array of handles to Conduit object
 *  \param nelem input: No of array elements
 *  \param pvcArray outout: Array of PVCtrlHolder containing cached data
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int  CAFE::getCtrlCache(const unsigned int * handleArray, unsigned int nelem, PVCtrlHolder * pvcArray){
#define __METHOD__ \
    "CAFE::getCtrlCache(unsigned int  * handleArray, unsigned int  nelem, PVCtrlHolder * pvcArray)"

    int  overallStatus=ICAFE_NORMAL; bool isGood=true;
    for (unsigned int  i=0; i<nelem; ++i) {
        status=getCtrlCache(handleArray[i],  pvcArray [i]);
        if ( (status != ICAFE_NORMAL) && isGood) {
            overallStatus=status;
            isGood=false;
        }
    }
    return overallStatus;
#undef __METHOD__
};


/**
 *  \brief Retrieves DBR_CTRL data from cache, packaged into the PVCtrlHolder class
 *  \param handle input: handle to Conduit object
 *  \param pvc outout: PVCtrlHolder containing cached data
 *  \return ICAFE_NORMAL if all OK else ICAFE_WAITING_FOR_PREV_CALLBACK;
 */
int  CAFE::getCtrlCache(const unsigned int  handle, PVCtrlHolder & pvc) {
#define __METHOD__ "CAFE::getCtrlCache(unsigned int  handle, PVCtrlHolder & pvc)"

    status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end()) {

        //Whatever is already there!
        chtype dbrtype= (*it_handle).getChannelRequestMetaData().getDbrDataType();

        //Use Native DataType
        if ( (status=cafeGranules.channelVerifyGetCtrl(handle, dbrtype)) != ICAFE_NORMAL) {
            (*it_handle).getPVCtrlHolder(pvc);
            pvc.status=status;
            return status;
        }

        if ((*it_handle).getChannelRequestStatusGetCtrl().getCallbackProgressKind() == CAFENUM::PENDING 
					  && handleHelper.getNmonitorCtrl(handle)==0
					  && (*it_handle).getChannelRequestPolicyGetCtrl().getMethodKind()
						   != CAFENUM::WITH_CALLBACK_USER_SUPPLIED 
						&& (*it_handle).getChannelGetCacheWaitPolicy().getWaitKind()	== CAFENUM::GET_CACHE_WAIT
            && (*it_handle).isConnected()    ) {

            ChannelTimeoutPolicy channelTimeoutPolicyGet=(*it_handle).getChannelTimeoutPolicyGet();

            ca_poll();
            status=cafeGranules.waitForGetCtrlEvent(handle,
                                        channelTimeoutPolicyGet.getTimeout() );

            if (status==ECAFE_TIMEOUT && channelTimeoutPolicyGet.getSelfGoverningTimeout()
                 && (*it_handle).isConnected() ) {
                unsigned short ntries=0;
                while (status==ECAFE_TIMEOUT && ntries<channelTimeoutPolicyGet.getNtries()) {
                    status=cafeGranules.waitForGetCtrlEvent(handle,
                                      channelTimeoutPolicyGet.getTimeout() +
                                      channelTimeoutPolicyGet.getDeltaTimeout()*(++ntries));
                }
                if ((*it_handle).isConnected()) {
                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << "No of waitForGetCtrlEvent tries=" << ntries << std::endl;

                    if (status==ECAFE_TIMEOUT) {
                        std::cout <<
                                "is the MAXIMUM allowed as configured through TimeoutPolicy! "
                                << std::endl;
                    }

                    std::cout << "Changing timeout for handle/pv "
                            << handle << "/" << (*it_handle).getPV() << " to: "  <<
                        (channelTimeoutPolicyGet.getTimeout() +
                         channelTimeoutPolicyGet.getDeltaTimeout()*ntries) << " seconds"  <<endl;

                    //modify timeout for handle
                    channelTimeoutPolicyGet.setTimeout( (channelTimeoutPolicyGet.getTimeout() +
                                                channelTimeoutPolicyGet.getDeltaTimeout()*ntries));
                    if(MUTEX){cafeMutex.lock();}    //lock
                    handle_index.modify(it_handle,
                                        change_channelTimeoutPolicyGet(channelTimeoutPolicyGet));
                    if(MUTEX){cafeMutex.unlock();}  //unlock
                }//isConnected
            }

			if (!(*it_handle).isConnected() || status != ICAFE_NORMAL) {
				 if(MUTEX){cafeMutex.lock();}   //lock
				channelRequestStatusGetCtrl = (*it_handle).getChannelRequestStatusGetCtrl();
				channelRequestStatusGetCtrl.setCallbackStatus    (status);
				channelRequestStatusGetCtrl.setCallbackKind(false, false); // NOT_INITIATED NOT_TRIGGERED

				handle_index.modify(it_handle,
					change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));


				if (!(*it_handle).isConnected()) {
					handle_index.modify(it_handle, change_status(ICAFE_CS_DISCONN));
					if(MUTEX){cafeMutex.unlock();}//unlock
					return ICAFE_CS_DISCONN;
				}
				else {
					handle_index.modify(it_handle, change_status(status));
					if(MUTEX){cafeMutex.unlock();}//unlock
                }

                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cafeStatus.report(status);
                cout << "Method returning ICAFE_WAITING_FOR_PREV_CALLBACK " << endl;
                return ICAFE_WAITING_FOR_PREV_CALLBACK;
            }
        }
        (*it_handle).getPVCtrlHolder(pvc);
    }
    else {
				if (printErrorPolicy.getInvalidHandle()) {	
        	cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        	cafeStatus.report(ECAFE_INVALID_HANDLE);
				}	
        pvc.status=ECAFE_INVALID_HANDLE;
        return ECAFE_INVALID_HANDLE;
    }

    if(MUTEX){cafeMutex.lock();}; //lock
    handle_index.modify(it_handle, change_status(status));
    if(MUTEX){cafeMutex.unlock();}; //unlock

    return status;
#undef __METHOD__
};



//Cache single value
//0
/**
 *  \brief Get dbr_string_t from Cache (DBR_STRING)
 *  \param handle input: handle
 *  \param _val output: dbr_string
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(unsigned int handle, dbr_string_t  &_val) {
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]={""};
    status=cafeSoluble.getCache(handle, DBR_STRING, val);
    if (status==ICAFE_NORMAL) {sprintf(_val, val[0]);}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_string_t from Cache (DBR_STS_STRING)
 *  \param handle input: handle
 *  \param _val output: dbr_string_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(unsigned int handle, dbr_string_t  &_val, dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]={""};
    status=cafeSoluble.getCache(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
    if (status==ICAFE_NORMAL) {sprintf(_val, val[0]);}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_string_t from Cache (DBR_TIME_STRING)
 *  \param handle input: const unsigned int
 *  \param _val output: dbr_string_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_string_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]={""};
    status=cafeSoluble.getCache(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
    if (status==ICAFE_NORMAL) {sprintf(_val, val[0]);}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}

//Cache single value
//1
/**
 *  \brief Get short from Cache (DBR_SHORT)
 *  \param handle input: const unsigned int
 *  \param _val output: short
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_short_t  &_val) {

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_short_t val[1]={0};
    status=cafeSchale.getCache(handle, DBR_SHORT, val);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get short from Cache (DBR_STS_SHORT)
 *  \param handle input: const unsigned int
 *  \param _val output: double
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_short_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_short_t val[1]={0};
    status=cafeSchale.getCache(handle, DBR_STS_SHORT, val, alarmStatus, alarmSeverity);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get short from Cache (DBR_TIME_SHORT)
 *  \param handle input: const unsigned int
 *  \param _val output: short
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_short_t  &_val,  dbr_short_t &alarmStatus,\
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_short_t val[1]={0};
    status=cafeSchale.getCache(handle, DBR_TIME_SHORT, val, alarmStatus, alarmSeverity, ts);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}

//Cache single value
//2
/**
 *  \brief Get float from Cache (DBR_FLOAT)
 *  \param handle input: const unsigned int
 *  \param _val output: float
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_float_t  &_val) {

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_float_t val[1]={0};
    status=cafeFrappuccino.getCache(handle, DBR_FLOAT, val);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get float from Cache (DBR_STS_FLOAT)
 *  \param handle input: const unsigned int
 *  \param _val output: float
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_float_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_float_t val[1]={0};
    status=cafeFrappuccino.getCache(handle, DBR_STS_FLOAT, val, alarmStatus, alarmSeverity);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get float from Cache (DBR_TIME_FLOAT)
 *  \param handle input: const unsigned int
 *  \param _val output: float
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_float_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_float_t val[1]={0};
    status=cafeFrappuccino.getCache(handle, DBR_TIME_FLOAT, val, alarmStatus, alarmSeverity, ts);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
//Cache single value
//3
/**
 *  \brief Get dbr_enum_t from Cache (DBR_ENUM)
 *  \param handle input: const unsigned int
 *  \param _val output: dbr_enum_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_enum_t  &_val) {

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_enum_t val[1]={0};
    status=cafeEspresso.getCache(handle, DBR_ENUM, val);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_enum_t from Cache (DBR_STS_ENUM)
 *  \param handle input: const unsigned int
 *  \param _val output: dbr_enum_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_enum_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_enum_t val[1]={0};
    status=cafeEspresso.getCache(handle, DBR_STS_ENUM, val, alarmStatus, alarmSeverity);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_enum_t from Cache (DBR_TIME_ENUM)
 *  \param handle input: const unsigned int
 *  \param _val output: dbr_enum_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_enum_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_enum_t val[1]={0};
    status=cafeEspresso.getCache(handle, DBR_TIME_ENUM, val, alarmStatus, alarmSeverity, ts);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
//Cache single value
//4
/**
 *  \brief Get dbr_char_t from Cache (DBR_CHAR)
 *  \param handle input: const unsigned int
 *  \param _val output: dbr_char_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_char_t  &_val) {

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_char_t val[1]={0};
    status=cafeCappuccino.getCache(handle, DBR_CHAR, val);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_char_t from Cache (DBR_STS_CHAR)
 *  \param handle input: const unsigned int
 *  \param _val output: dbr_char_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_char_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_char_t val[1]={0};
    status=cafeCappuccino.getCache(handle, DBR_STS_CHAR, val, alarmStatus, alarmSeverity);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_char_t from Cache (DBR_TIME_CHAR)
 *  \param handle input: const unsigned int
 *  \param _val output: dbr_char_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_char_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_char_t val[1]={0};
    status=cafeCappuccino.getCache(handle, DBR_TIME_CHAR, val, alarmStatus, alarmSeverity, ts);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
//Cache single value
//5
/**
 *  \brief Get dbr_long_t from Cache (DBR_LONG)
 *  \param handle input: const unsigned int
 *  \param _val output: dbr_long_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_long_t  &_val) {

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_long_t val[1]={0};
    status=cafeLatte.getCache(handle, DBR_LONG, val);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_long_t from Cache (DBR_STS_LONG)
 *  \param handle input: const unsigned int
 *  \param _val output: dbr_long_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_long_t &_val, dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_long_t val[1]={0};
    status=cafeLatte.getCache(handle, DBR_STS_LONG, val, alarmStatus, alarmSeverity);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_long_t from Cache (DBR_TIME_LONG)
 *  \param handle input: const unsigned int
 *  \param _val output: dbr_long_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_long_t  &_val, dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_long_t val[1]={0};
    status=cafeLatte.getCache(handle, DBR_TIME_LONG, val, alarmStatus, alarmSeverity, ts);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}

//Cache single value
//5++
/**
 *  \brief Get long long from Cache (long long)
 *  \param handle input: const unsigned int
 *  \param val output: long long
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, long long  &val) {

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    status=CAFE::getCache(handle, &val);
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get long long from Cache
 *  \param handle input: const unsigned int
 *  \param val output: long long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, long long &val, dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    status=CAFE::getCache(handle, &val, alarmStatus, alarmSeverity);
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get long long from Cache
 *  \param handle input: const unsigned int
 *  \param val output: long long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, long long &val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    status=CAFE::getCache(handle, &val, alarmStatus, alarmSeverity, ts);
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}

//Cache single value
//6
/**
 *  \brief Get double from Cache (DBR_DOUBLE)
 *  \param handle input: const unsigned int
 *  \param _val output: double
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_double_t  &_val) {

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);

    dbr_double_t val[1]={0};
    status=cafeDoppio.getCache(handle, DBR_DOUBLE, val);

    if (status==ICAFE_NORMAL) {_val= val[0];}

    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);

    return status;
}
/**
 *  \brief Get double from Cache (DBR_STS_DOUBLE)
 *  \param handle input: const unsigned int
 *  \param _val output: double
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_double_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_double_t val[1]={0};
    status=cafeDoppio.getCache(handle, DBR_STS_DOUBLE, val, alarmStatus, alarmSeverity);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get double from Cache (DBR_TIME_DOUBLE)
 *  \param handle input: const unsigned int
 *  \param _val output: double
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const unsigned int handle, dbr_double_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){

    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_double_t val[1]={0};
    status=cafeDoppio.getCache(handle, DBR_TIME_DOUBLE, val, alarmStatus, alarmSeverity, ts);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}


////////////////////////////////////////////// by Name ////////////////////////////////


//Cache single value
//0+
/**
 *  \brief Get string from Cache
 *  \param pv input: pv name
 *  \param valStr output: string
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, string  &valStr) {
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]={""};
    status=cafeSoluble.getCache(handle, DBR_STRING, val);
    if (status==ICAFE_NORMAL) {valStr = val[0];}
    else {valStr="";}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get string from Cache
 *  \param pv input: pv name
 *  \param valStr output: string
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, string  &valStr,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]={""};
    status=cafeSoluble.getCache(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
    if (status==ICAFE_NORMAL) {valStr= val[0];}
    else {valStr="";}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get string from Cache
 *  \param pv input: pv name
 *  \param valStr output: string
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, string  &valStr,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]={""};
    status=cafeSoluble.getCache(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
    if (status==ICAFE_NORMAL) {valStr= val[0];}
    else {valStr="";}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}

//0
/**
 *  \brief Get dbr_string_t from Cache (DBR_STRING)
 *  \param pv input: pv name
 *  \param _val output: dbr_string
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_string_t  &_val) {
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]={""};
    status=cafeSoluble.getCache(handle, DBR_STRING, val);
    if (status==ICAFE_NORMAL) {sprintf(_val, val[0]);}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_string_t from Cache (DBR_STS_STRING)
 *  \param pv input: pv name
 *  \param _val output: dbr_string_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_string_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]={""};
    status=cafeSoluble.getCache(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
    if (status==ICAFE_NORMAL) {sprintf(_val, val[0]);}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_string_t from Cache (DBR_TIME_STRING)
 *  \param pv input: pv name
 *  \param _val output: dbr_string_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_string_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_string_t val[1]={""};
    status=cafeSoluble.getCache(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
    if (status==ICAFE_NORMAL) {sprintf(_val, val[0]);}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}

//Cache single value
//1
/**
 *  \brief Get short from Cache (DBR_SHORT)
 *  \param pv input: pv name
 *  \param _val output: short
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_short_t  &_val) {
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_short_t val[1]={0};
    status=cafeSchale.getCache(handle, DBR_SHORT, val);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get short from Cache (DBR_STS_SHORT)
 *  \param pv input: pv name
 *  \param _val output: double
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_short_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_short_t val[1]={0};
    status=cafeSchale.getCache(handle, DBR_STS_SHORT, val, alarmStatus, alarmSeverity);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get short from Cache (DBR_TIME_SHORT)
 *  \param pv input: pv name
 *  \param _val output: short
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_short_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_short_t val[1]={0};
    status=cafeSchale.getCache(handle, DBR_TIME_SHORT, val, alarmStatus, alarmSeverity, ts);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}


//Cache single value
//2
/**
 *  \brief Get float from Cache (DBR_FLOAT)
 *  \param pv input: pv name
 *  \param _val output: float
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_float_t  &_val) {
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_float_t val[1]={0};
    status=cafeFrappuccino.getCache(handle, DBR_FLOAT, val);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get float from Cache (DBR_STS_FLOAT)
 *  \param pv input: pv name
 *  \param _val output: float
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_float_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_float_t val[1]={0};
    status=cafeFrappuccino.getCache(handle, DBR_STS_FLOAT, val, alarmStatus, alarmSeverity);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get float from Cache (DBR_TIME_FLOAT)
 *  \param pv input: pv name
 *  \param _val output: float
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_float_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_float_t val[1]={0};
    status=cafeFrappuccino.getCache(handle, DBR_TIME_FLOAT, val, alarmStatus, alarmSeverity, ts);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
//Cache single value
//3
/**
 *  \brief Get dbr_enum_t from Cache (DBR_ENUM)
 *  \param pv input: pv name
 *  \param _val output: dbr_enum_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_enum_t  &_val) {
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_enum_t val[1]={0};
    status=cafeEspresso.getCache(handle, DBR_ENUM, val);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_enum_t from Cache (DBR_STS_ENUM)
 *  \param pv input: pv name
 *  \param _val output: dbr_enum_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_enum_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_enum_t val[1]={0};
    status=cafeEspresso.getCache(handle, DBR_STS_ENUM, val, alarmStatus, alarmSeverity);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_enum_t from Cache (DBR_TIME_ENUM)
 *  \param pv input: pv name
 *  \param _val output: dbr_enum_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_enum_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_enum_t val[1]={0};
    status=cafeEspresso.getCache(handle, DBR_TIME_ENUM, val, alarmStatus, alarmSeverity, ts);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
//Cache single value
//4
/**
 *  \brief Get dbr_char_t from Cache (DBR_CHAR)
 *  \param pv input: pv name
 *  \param _val output: dbr_char_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_char_t  &_val) {
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_char_t val[1]={0};
    status=cafeCappuccino.getCache(handle, DBR_CHAR, val);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_char_t from Cache (DBR_STS_CHAR)
 *  \param pv input: pv name
 *  \param _val output: dbr_char_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_char_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_char_t val[1]={0};
    status=cafeCappuccino.getCache(handle, DBR_STS_CHAR, val, alarmStatus, alarmSeverity);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_char_t from Cache (DBR_TIME_CHAR)
 *  \param pv input: pv name
 *  \param _val output: dbr_char_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_char_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_char_t val[1]={0};
    status=cafeCappuccino.getCache(handle, DBR_TIME_CHAR, val, alarmStatus, alarmSeverity, ts);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
//Cache single value
//5
/**
 *  \brief Get dbr_long_t from Cache (DBR_LONG)
 *  \param pv input: pv name
 *  \param _val output: dbr_long_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_long_t  &_val) {
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_long_t val[1]={0};
    status=cafeLatte.getCache(handle, DBR_LONG, val);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_long_t from Cache (DBR_STS_LONG)
 *  \param pv input: pv name
 *  \param _val output: dbr_long_t
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_long_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_long_t val[1]={0};
    status=cafeLatte.getCache(handle, DBR_STS_LONG, val, alarmStatus, alarmSeverity);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get dbr_long_t from Cache (DBR_TIME_LONG)
 *  \param pv input: pv name
 *  \param _val output: double
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_long_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_long_t val[1]={0};
    status=cafeLatte.getCache(handle, DBR_TIME_LONG, val, alarmStatus, alarmSeverity, ts);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
//Cache single value
//6
/**
 *  \brief Get double from Cache (DBR_DOUBLE)
 *  \param pv input: pv name
 *  \param _val output: double
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_double_t  &_val) {
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_double_t val[1]={0};
    status=cafeDoppio.getCache(handle, DBR_DOUBLE, val);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get double from Cache (DBR_STS_DOUBLE)
 *  \param pv input: pv name
 *  \param _val output: double
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_double_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_double_t val[1]={0};
    status=cafeDoppio.getCache(handle, DBR_STS_DOUBLE, val, alarmStatus, alarmSeverity);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}
/**
 *  \brief Get double from Cache (DBR_TIME_DOUBLE)
 *  \param pv input: pv name
 *  \param _val output: double
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int  CAFE::getCache(const char * pv, dbr_double_t  &_val,  dbr_short_t &alarmStatus, \
                    dbr_short_t &alarmSeverity, epicsTimeStamp &ts){
    unsigned int handle=getHandleHelper().getHandleFromPV(pv);
    unsigned int nelemPrevious=CAFE::setNelemToRetrieveFromCacheToOne(handle);
    dbr_double_t val[1]={0};
    status=cafeDoppio.getCache(handle, DBR_TIME_DOUBLE, val, alarmStatus, alarmSeverity, ts);
     if (status==ICAFE_NORMAL) {_val= val[0];}
    //Switch back to previous value
    CAFE::setNelemToRetrieveFromCacheToPrevious(handle, nelemPrevious);
    return status;
}

