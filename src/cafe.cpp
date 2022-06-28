///
/// \file    cafe.cpp
/// \author  Jan Chrin, PSI
/// \date    Release: June, 2018
/// \version CAFE 1.6.0
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


#include <cafe.h>
#include <conduitGroup.h>
#include <cafeEnum.h>
#include <enumStrings.h>
#include <set>

#include <hashConduit.h>

using namespace std;

bool MUTEX=true;
bool CHECK_CONSISTENCY_CA_STATE=true;
bool SF_WITH_PULSE_ID = false;

vector<PVGroup> PVGroupV;
vector<PVGroup> PVGroupPseudo;
vector<deviceCollection>  deviceCollectionV;
vector<string> globalChannelList;
map<vector<unsigned int>, string> groupPseudoMap;

cafeGroup_set gs;
cafeConduit_set cs;
epicsMutex cafeMutex;

/*
int CAFE::wfExpress(const unsigned int handle) {
  int status = ICAFE_NORMAL;
  
  cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
  cafeConduit_set_by_handle::iterator it_handle;

  it_handle = handle_index.find(handle);

  if (it_handle != handle_index.end())
    {
      std::cout << "Initiate Callback =============" << std::endl;
      status=(*it_handle).getWithCallback(CALLBACK_CAFE::handlerGet);
      ca_flush_io();  
    }

  return status; 
}
*/

int CAFE::wfExpress(const unsigned int handle,  const unsigned int nelem, dbr_char_t * chval) {

  int status = ICAFE_NORMAL;
  
  chval = new dbr_char_t[nelem];
  //dbr_float_t * val = new dbr_float_t[nelem];


  //cafeGranules.channelExecuteGet(handle);

  cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
  cafeConduit_set_by_handle::iterator it_handle;

  it_handle = handle_index.find(handle);

  if (it_handle != handle_index.end())
    {
      
      
      std::cout << "Initiate Callback =============" << std::endl;
      status=(*it_handle).getWithCallback(CALLBACK_CAFE::handlerGet);
      ca_flush_io();
      for (int i=0; i <20; ++i) {

	sleep(0.05);
	//std::cout << "iscb done " << cafeGranules.isGetCallbackDone(handle) << std::endl;

      }
      

     //(*it_handle).getWithCallback(CALLBACK_CAFE::handlerGet);
      
      union db_access_val * PVDataL  = (*it_handle).getDataBuffer();

      for (unsigned int  i=0; i<nelem; ++i) {
      	//val[i] =  (dbr_float_t) (*(&((PVDataL)->tfltval.value)+i));
	chval[i] =  (dbr_char_t) (*(&((PVDataL)->tchrval.value)+i));
      }
         

      //memcpy( chval, &(&((PVDataL)->charval))[0], sizeof(dbr_char_t)*nelem);
      std::cout << chval[0] << " //0//==> "  << (short)  chval[0] << std::endl;
      std::cout << chval[1] << " //1//==> "  << (short)  chval[1] << std::endl;
    }

  delete [] chval;

  return status; 


 
  //union db_access_val * data;
  ChannelRegalia channelInfo;
  CAFE::getChannelInfo(handle, channelInfo);
  chid channelID = channelInfo.getChannelID();

  dbr_time_char * data = (dbr_time_char *) malloc(dbr_size_n(DBR_TIME_CHAR, nelem));
  
  //chval[0] = 2;

  std::cout << channelID << " // " << nelem << std::endl;
  //data = (db_access_val *) malloc (dbr_size_n(DBR_CHAR, nelem) );

  status = ca_array_get(DBR_TIME_CHAR, nelem, channelID, data);
  
  status = ca_pend_io(4.1);
  std::cout << status << " // " << nelem << std::endl;

  if (status == ICAFE_NORMAL) {

  dbr_char_t * value = & data->value;
 
  std::cout << value[0] << "///"  << (short)  value[0] << std::endl; 
  std::cout << value[1] << "///"  << (short)  value[1] << std::endl; 
  }
  //memcpy(chval, &((data)->charval), nelem*sizeof(dbr_char_t));
  /*
 
  for (unsigned int  i=0; i<nelem; ++i)
        {	
	  chval[i] = (unsigned short) value[i]; //(*(&((data)->charval)+i));
        }
  std::cout << chval[0] << "///"  << (unsigned short)  chval[0] << std::endl; 
  */
  free(data);
  
  return status; 
}


//5+ long long
/**
 *  \brief Set an array of handles with long long(s)
 *  \param handleArray input: array of handles
 *  \param nelem input: number of elements defining size of array
 *  \param _val input: array of long longs
 *  \param statusArray output: array of corresponding status
 *  \return ECA_NORMAL if all OK else first ECAFE error encountered
 */
int CAFE::set(const unsigned int *handleArray, const unsigned int nelem, const long long * _val,  int *statusArray)
{
#define __METHOD__ \
  "CAFE::set(const unsigned int *handleArray, const unsigned int nelem, const long long * _val,  int *statusArray)"
    //what is the highest native type?
    chtype chtMax=DBF_CHAR;
    ChannelRegalia channelInfo;
    unsigned short nbyteChType=1;
    unsigned short nbyteMax=nbyteChType;

    for (unsigned int i=0; i<nelem; ++i)
    {
        CAFE::getChannelInfo(handleArray[i], channelInfo);

        switch (channelInfo.getDataType())
        {
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
        }//switch

        if (channelInfo.getCafeConnectionState() != ICAFE_CS_NEVER_CONN
                && channelInfo.getCafeConnectionState() != ICAFE_CS_CLOSED)
        {
            if (channelInfo.getDataType()==DBF_STRING)
            {
                chtMax=DBR_STRING;
                break;
            }
            if(nbyteChType>nbyteMax && channelInfo.getDataType()< CAFE_NO_ACCESS)
            {
                chtMax=channelInfo.getDataType();
                nbyteMax=4;
            }
        }
    }//for

    int _status=ICAFE_NORMAL;

    if (chtMax == DBF_STRING)
    {
        dbr_string_t * _sVal = new dbr_string_t[nelem];
        std::stringstream ss;
        for (unsigned int i=0; i < nelem; ++i)
        {
            ss.clear();
            ss << _val[i];
            strcpy(_sVal[i], ss.str().c_str());
        }
        _status=CAFE::set (handleArray, nelem, _sVal, statusArray);
        delete [] _sVal;
    }
    else if (chtMax == DBF_DOUBLE)
    {
        dbr_double_t * _dVal = new dbr_double_t[nelem];
        for (unsigned int i=0; i < nelem; ++i)
        {
            _dVal[i] = (double) _val[i];
        }
        _status=CAFE::set (handleArray, nelem, _dVal, statusArray);
        delete [] _dVal;
    }
    else if (chtMax == DBF_FLOAT)
    {
        dbr_float_t * _fVal = new dbr_float_t[nelem];
        for (unsigned int i=0; i < nelem; ++i)
        {
            _fVal[i] = (float) _val[i];
        }
        _status=CAFE::set (handleArray, nelem, _fVal, statusArray);
        delete [] _fVal;
    }
    else
    {
        dbr_long_t * _lVal = new dbr_long_t[nelem];
        for (unsigned int i=0; i < nelem; ++i)
        {
            _lVal[i] = (dbr_long_t) _val[i];
        }
        _status=CAFE::set (handleArray, nelem, _lVal, statusArray);
        delete [] _lVal;
    }

    return _status;
#undef __METHOD__
}

//5+ long long
/**
 *  \brief Get long long(s), alarms and timeStamp
 *  \param handle input: handle to conduit object
 *  \param _val output: array of long longs
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const unsigned int handle, long long * _val, \
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
#define __METHOD__ \
  "CAFE::get(const unsigned int handle, long long * _val,  \
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"

    ChannelRegalia channelInfo;
    CAFE::getChannelInfo(handle, channelInfo);

    if (channelInfo.getCafeConnectionState() == ICAFE_CS_NEVER_CONN)
    {
        return ICAFE_CS_NEVER_CONN;
    }
    else if ( channelInfo.getCafeConnectionState()==ICAFE_CS_CLOSED)
    {
        return ICAFE_CS_CLOSED;
    }

    status=ICAFE_NORMAL;
    unsigned int nn=handleHelper.getNelemNative(handle);

    if (channelInfo.getDataType() == DBR_STRING)
    {
        dbr_string_t * _sVal = new dbr_string_t[nn];
        status=cafeSoluble.get (handle, DBR_TIME_STRING, _sVal, alarmStatus, alarmSeverity, ts);

        if (status!=ICAFE_NORMAL)
        {
            delete [] _sVal;
            return status;
        }
        std::istringstream ss;
        long long l=0;
        for (unsigned short i=0; i<nn; ++i)
        {
            l=0;
            ss.clear();
            ss.str(_sVal[i]);
            ss>>l;
            if ( !ss.fail())
            {
                _val[i] = (long  long) l;
                std::string strInput=_sVal[i];
                std::stringstream ssOut;
                ssOut << l;
                std::string strOutput=ssOut.str();
                if (strInput!=strOutput)
                {
                    cout << __FILE__ << ":" << __LINE__ << endl;
                    cout << __METHOD__ << endl;
                    cout << "***WARNING*** STRING TO LONG LONG CONVERSION REPORTS: " << endl;
                    cout << "STRING VALUE: " << strInput << " CONVERTED TO: " << strOutput << endl;
                }
            }
            else
            {
                cout << __FILE__ << ":" << __LINE__ << endl;
                cout << __METHOD__ << endl;
                cout << "***WARNING*** NO STRING TO LONG LONG  CONVERSION for ELEMENT " << i \
                     << " of " << nn << " !! " << endl;
                cout << "***WARNING*** COULD NOT CONVERT: ";
                cout << _sVal[i];
                cout << " TO LONG LONG ; CONVERSION NOT MADE!" << endl;
                cout << "Returning value 0 for element " << i << endl;
                _val[i]=0;
            }
        }//for
        delete [] _sVal;
    }
    else if ( channelInfo.getDataType() == DBR_DOUBLE)
    {
        dbr_double_t * _dVal = new dbr_double_t[nn];
        status=cafeDoppio.get (handle, DBR_TIME_DOUBLE,  _dVal, alarmStatus, alarmSeverity, ts);
        for (unsigned int i=0; i < handleHelper.getNelemRequest(handle); ++i)
        {
            _val[i] = (long  long) _dVal[i];
        }
        delete [] _dVal;
    }
    else if ( channelInfo.getDataType() == DBR_FLOAT)
    {
        dbr_float_t * _fVal = new dbr_float_t[nn];
        status=cafeFrappuccino.get (handle, DBR_TIME_FLOAT,  _fVal, alarmStatus, alarmSeverity, ts);
        for (unsigned int i=0; i < handleHelper.getNelemRequest(handle); ++i)
        {
            _val[i] = (long  long) _fVal[i];
        }
        delete [] _fVal;
    }
    else
    {
        dbr_long_t * _lVal = new dbr_long_t[nn];
        status=cafeLatte.get (handle, DBR_TIME_LONG,  _lVal, alarmStatus, alarmSeverity, ts);
        for (unsigned int i=0; i < handleHelper.getNelemRequest(handle); ++i)
        {
            _val[i] = (long  long) _lVal[i];
        }
        delete [] _lVal;
    }
    return status;
#undef __METHOD__
};

/**
 *  \brief  Get long long(s) and alarms
 *  \param handle input: handle to conduit object
 *  \param _val output: array of long longs
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const unsigned int handle, long long   * _val, \
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
#define __METHOD__ \
  "CAFE::get(unsigned int handle, long long * _val, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)"
    ChannelRegalia channelInfo;
    CAFE::getChannelInfo(handle, channelInfo);
    if (channelInfo.getCafeConnectionState() == ICAFE_CS_NEVER_CONN)
    {
        return ICAFE_CS_NEVER_CONN;
    }
    else if (channelInfo.getCafeConnectionState()==ICAFE_CS_CLOSED)
    {
        return ICAFE_CS_CLOSED;
    }
    int _status=ICAFE_NORMAL;
    unsigned int nn=handleHelper.getNelemNative(handle);

    if (channelInfo.getDataType() == DBR_STRING)
    {
        dbr_string_t * _sVal = new dbr_string_t[nn];
        _status=cafeSoluble.get (handle, DBR_STS_STRING,  _sVal, alarmStatus, alarmSeverity);
        if (_status!=ICAFE_NORMAL)
        {
            delete [] _sVal;
            return _status;
        }
        istringstream ss;
        long long l=0;

        for (unsigned short i=0; i<nn; ++i)
        {
            l=0;
            ss.clear();
            ss.str(_sVal[i]);
            ss>>l;

            if (!ss.fail())
            {
                _val[i] = (long  long) l;
                std::string strInput=_sVal[i];
                std::stringstream ssOut;
                ssOut << l;
                std::string strOutput=ssOut.str();
                if (strInput!=strOutput)
                {
                    cout << __FILE__ << ":" << __LINE__ << endl;
                    cout << __METHOD__ << endl;
                    cout << "***WARNING*** STRING TO LONG LONG CONVERSION REPORTS: " << endl;
                    cout << "STRING VALUE: " << strInput << " CONVERTED TO: " << strOutput << endl;
                }
            }
            else
            {
                cout << __FILE__ << ":" << __LINE__ << endl;
                cout << __METHOD__ << endl;
                cout << "***WARNING*** NO STRING TO LONG LONG  CONVERSION for ELEMENT " << i \
                     << " of " << nn << " !! " << endl;
                cout << "***WARNING*** COULD NOT CONVERT: ";
                cout << _sVal[i];
                cout << " TO LONG LONG ; CONVERSION NOT MADE!" << endl;
                cout << "Returning value 0 for element " << i << endl;
                _val[i]=0;
            }
        }//for
        delete [] _sVal;
    }
    else if ( channelInfo.getDataType() == DBR_DOUBLE)
    {
        dbr_double_t * _dVal = new dbr_double_t[nn];
        _status=cafeDoppio.get (handle, DBR_STS_DOUBLE,  _dVal, alarmStatus, alarmSeverity);
        for (unsigned int i=0; i < handleHelper.getNelemRequest(handle); ++i)
        {
            _val[i] = (long  long) _dVal[i];
        }
        delete [] _dVal;
    }
    else if ( channelInfo.getDataType() == DBR_FLOAT)
    {
        dbr_float_t * _fVal = new dbr_float_t[nn];
        _status=cafeFrappuccino.get (handle, DBR_STS_FLOAT,  _fVal, alarmStatus, alarmSeverity);
        for (unsigned int i=0; i < handleHelper.getNelemRequest(handle); ++i)
        {
            _val[i] = (long  long) _fVal[i];
        }
        delete [] _fVal;
    }
    else
    {
        dbr_long_t * _lVal = new dbr_long_t[nn];
        _status=cafeLatte.get (handle, DBR_STS_LONG,  _lVal, alarmStatus, alarmSeverity);
        for (unsigned int i=0; i < handleHelper.getNelemRequest(handle); ++i)
        {
            _val[i] = (long  long) _lVal[i];
        }
        delete [] _lVal;
    }
    return _status;
#undef __METHOD__
};

/**
 *  \brief Get long long(s)
 *  \param handle input: handle
 *  \param _val output: array of long longs
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const unsigned int handle, long long * _val)
{
#define __METHOD__ \
  "CAFE::get(const unsigned int handle, long long * _val)"

    ChannelRegalia channelInfo;
    CAFE::getChannelInfo(handle, channelInfo);

    if (channelInfo.getCafeConnectionState() == ICAFE_CS_NEVER_CONN)
    {
        return ICAFE_CS_NEVER_CONN;
    }
    else if ( channelInfo.getCafeConnectionState()==ICAFE_CS_CLOSED)
    {
        return ICAFE_CS_CLOSED;
    }

    int _status=ICAFE_NORMAL;
    unsigned int nn=handleHelper.getNelemNative(handle);

    if (channelInfo.getDataType() == DBR_STRING)
    {
        dbr_string_t * _sVal = new dbr_string_t[nn];
        _status=cafeSoluble.get (handle, DBR_STRING,  _sVal);
        if (_status!=ICAFE_NORMAL)
        {
            delete [] _sVal;
            return _status;
        }
        std::istringstream ss;
        long long l=0;

        for (unsigned short i=0; i<nn; ++i)
        {
            l=0;
            ss.clear();
            ss.str(_sVal[i]);
            ss>>l;
            if (!ss.fail())
            {
                _val[i] = (long  long) l;
                std::string strInput=_sVal[i];
                std::stringstream ssOut;
                ssOut << l;
                std::string strOutput=ssOut.str();
                if (strInput!=strOutput)
                {
                    cout << __FILE__ << ":" << __LINE__ << endl;
                    cout << __METHOD__ << endl;
                    cout << "***WARNING*** STRING TO LONG LONG CONVERSION REPORTS: " << endl;
                    cout << "STRING VALUE: " << strInput << " CONVERTED TO: " << strOutput << endl;
                }
            }
            else
            {
                cout << __FILE__ << ":" << __LINE__ << endl;
                cout << __METHOD__ << endl;
                cout << "***WARNING*** NO STRING TO LONG LONG  CONVERSION for ELEMENT " << i \
                     << " of " << nn << " !! " << endl;
                cout << "***WARNING*** COULD NOT CONVERT: ";
                cout << _sVal[i];
                cout << " TO LONG LONG ; CONVERSION NOT MADE!" << endl;
                cout << "Returning value 0 for element " << i << endl;
                _val[i]=0;
            }
        }//for
        delete [] _sVal;
    }
    else if (channelInfo.getDataType() == DBR_DOUBLE)
    {
        dbr_double_t * _dVal = new dbr_double_t[nn];
        _status=cafeDoppio.get (handle, DBR_DOUBLE,  _dVal);
        if (_status!=ICAFE_NORMAL)
        {
            delete [] _dVal;
            return _status;
        }
        for (unsigned int i=0; i < handleHelper.getNelemRequest(handle); ++i)
        {
            _val[i] = (long  long) _dVal[i];
        }
        delete [] _dVal;
    }
    else if ( channelInfo.getDataType() == DBR_FLOAT)
    {
        dbr_float_t * _fVal = new dbr_float_t[nn];
        _status=cafeFrappuccino.get (handle, DBR_FLOAT,  _fVal);
        if (_status!=ICAFE_NORMAL)
        {
            delete [] _fVal;
            return _status;
        }
        for (unsigned int i=0; i < handleHelper.getNelemRequest(handle); ++i)
        {
            _val[i] = (long  long) _fVal[i];
        }
        delete [] _fVal;
    }
    else
    {
        dbr_long_t * _lVal = new dbr_long_t[nn];
        _status=cafeLatte.get (handle, DBR_LONG,  _lVal);
        if (_status!=ICAFE_NORMAL)
        {
            delete [] _lVal;
            return _status;
        }
        for (unsigned int i=0; i < handleHelper.getNelemRequest(handle); ++i)
        {
            _val[i] = (long  long) _lVal[i];
        }
        delete [] _lVal;
    }
    return _status;
#undef __METHOD__
};

/**
 *  \brief Sends ca_get message to IOC and returns without waiting, i.e., non-blocking \n
 *  Data can be subsequently retrieved through any getCache method
 *  \param handle input: handle to conduit object
 *  \return ICAFE_NORMAL if all OK else ICAFE_WAITING_FOR_PREV_CALLBACK or other;
 */
int CAFE::get(const unsigned int handle)
{
#define __METHOD__ "CAFE::get(unsigned int handle)"

    status=ICAFE_NORMAL;
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {
        chtype dbrtype= (*it_handle).getChannelRequestMetaDataClient().getDbrDataType();
        //Find Native DataType
        if ((status=cafeGranules.channelVerifyGet(handle, dbrtype)) != ICAFE_NORMAL)
        {
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            handle_index.modify(it_handle, change_status(status));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
            cafeStatus.report(status);
            return status;
        }

        if ((status=cafeGranules.channelPrepareGet(handle)) != ICAFE_NORMAL)
        {
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            handle_index.modify(it_handle, change_status(status));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
            cafeStatus.report(status);
            return status;
        }

        if ((status=cafeGranules.channelExecuteGetNoWait(handle)) != ICAFE_NORMAL)
        {
	    cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__  << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
	    cout << "has status = " << status << endl;
                cafeStatus.report(status);
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            handle_index.modify(it_handle, change_status(status));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
            if (status !=ICAFE_WAITING_FOR_PREV_CALLBACK)
            {
                cout << __FILE__ << ":" << __LINE__ << endl;
                cout << __METHOD__  << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
		cout << "has status = " << status << endl;
                cafeStatus.report(status);
            }
            return status;
        }
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
        }
        return ECAFE_INVALID_HANDLE;
    }

    if(MUTEX)
    {
        cafeMutex.lock();
    }; //lock
    handle_index.modify(it_handle, change_status(status));
    if(MUTEX)
    {
        cafeMutex.unlock();
    }; //unlock

    return status;
#undef __METHOD__
}

/**
 *  \brief Sends message to IOC to retreive DBR_CTRL data without waiting, i.e., non-blocking \n
 *  Data can be subsequently retrieved through any getCtrlCache method
 *  \param handle input: handle to conduit object
 *  \return ICAFE_NORMAL if all OK else ICAFE_WAITING_FOR_PREV_CALLBACK or other;
 */
int CAFE::getCtrl(const unsigned int handle)
{
#define __METHOD__ "CAFE::getCtrl(unsigned int handle)"

    status=ICAFE_NORMAL;
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {
        //Use Native DataType
        if ((status=cafeGranules.channelVerifyGetCtrl(handle,
                    dbf_type_to_DBR_CTRL((*it_handle).getChannelRegalia().getDataType()))) != ICAFE_NORMAL)
        {
            if(MUTEX)
            {
                cafeMutex.lock();
            }; //lock
            handle_index.modify(it_handle, change_status(status));
            if(MUTEX)
            {
                cafeMutex.unlock();
            }; //unlock
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__  << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
            cafeStatus.report(status);
            return status;
        }

        if ((status=cafeGranules.channelPrepareGetCtrl(handle)) != ICAFE_NORMAL)
        {
            if(MUTEX)
            {
                cafeMutex.lock();
            }; //lock
            handle_index.modify(it_handle, change_status(status));
            if(MUTEX)
            {
                cafeMutex.unlock();
            }; //unlock
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
            cafeStatus.report(status);
            return status;
        }

        if ((status=cafeGranules.channelExecuteGetCtrlNoWait(handle)) != ICAFE_NORMAL)
        {
            if(MUTEX)
            {
                cafeMutex.lock();
            }; //lock
            handle_index.modify(it_handle, change_status(status));
            if(MUTEX)
            {
                cafeMutex.unlock();
            }; //unlock
            if (status !=ICAFE_WAITING_FOR_PREV_CALLBACK)
            {
                cout << __FILE__ << ":" << __LINE__ << endl;
                cout << __METHOD__  << " for handle/pv=" << handle <<"/" << (*it_handle).getPV() <<endl;
                cafeStatus.report(status);
            }
            return status;
        }
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
        }
        return ECAFE_INVALID_HANDLE;
    }

    if(MUTEX)
    {
        cafeMutex.lock();
    }; //lock
    handle_index.modify(it_handle, change_status(status));
    if(MUTEX)
    {
        cafeMutex.unlock();
    }; //unlock

    return status;
#undef __METHOD__
};

/**
 *  \brief Sends ca_get message to IOC and returns without waiting, i.e., non-blocking \n
 *  Data can be subsequently retrieved through any getCache method
 *  \param pv input: process variable name
 *  \return ICAFE_NORMAL if all OK else ICAFE_WAITING_FOR_PREV_CALLBACK or other;
 */
int CAFE::get  (const char * pv)
{
#define __METHOD__ "CAFE::get(const char * pv)"
    unsigned int handle;
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
        status= CAFE::get(handle);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Sends message to IOC to retreive DBR_CTRL data without waiting, i.e., non-blocking \n
 *  Data can be subsequently retrieved through any getCtrlCache method
 *  \param pv input: process variable name
 *  \return ICAFE_NORMAL if all OK else ICAFE_WAITING_FOR_PREV_CALLBACK or other;
 */
int CAFE::getCtrl  (const char * pv)
{
#define __METHOD__ "CAFE::getCtrl(const char * pv)"
    unsigned int handle;
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
        status= CAFE::getCtrl(handle);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Retrieves PV data packaged into the PVDataHolder class
 *  \param pv input: pv name
 *  \param pvd output: PVDataHolder
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, PVDataHolder & pvd)
{
#define __METHOD__ "CAFE::get(const char *, PVDataHolder & pvd)"
    unsigned int handle;
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
        status= CAFE::get(handle, pvd);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Retrieves PV data packaged into the PVDataHolder class
 *  \param handle input: handle to Conduit object
 *  \param pvd output: PVDataHolder containing data
 *  \return ICAFE_NORMAL(=ECA_NORMAL) if all OK
 */
int CAFE::get(const unsigned int handle, PVDataHolder & pvd)
{
#define __METHOD__ "CAFE::get(unsigned int handle, PVDataHolder & pvd)"
    status=ICAFE_NORMAL;
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {
        if ((*it_handle).getChannelGetActionWhenMonitorPolicy().getActionKind() == CAFENUM::GET_FROM_CACHE)
        {
            if ( handleHelper.getNmonitorData(handle) >0)
            {
                bool doGetCache=true;
                vector<MonitorPolicy> mpV;
                handleHelper.getMonitorPolicyVector(handle, mpV);

                for (unsigned int n=0; n<mpV.size(); ++n)
                {
                    if (mpV[n].getCafeDbrType() != CAFENUM::DBR_TIME)
                    {
                        doGetCache=false;
                        cout << __FILE__ << ":" << __LINE__ << endl;
                        cout << __METHOD__ << endl;
                        cout << "***INFO*** YOU HAVE A MONITOR STARTED OM THIS CHANNEL "  << endl;
                        cout << "THAT DOES NOT RETRIEVE THE REQUESTED TIMESTAMPS " << endl;
                        cout << "WE WILL THEREFORE ATTEMPT A GET WITH DRB_TIME (RATHER THAN FROM CACHE)" << endl;
                        cout << "WE ADVICE THAT YOU MODIFY YOUR CODE TO START YOUR MONITOR WITH DRB_TIME" << endl;
                        cout << "Handle= " << handle <<  " PV= " << handleHelper.getPVFromHandle(handle) << endl;
                        mpV[n].print();
                    }
                }//for

                if(doGetCache)
                {
                    return CAFE::getCache(handle, pvd);
                }
            }
        }

        //Let us take a look at hasAlarm and hasTS flags to determine if these are retrieved from SC
        chtype _chtype=(*it_handle).getChannelRegalia().getDataType();
        if (!pvd.hasAlarm && !pvd.hasTS)
        {
            handleHelper.setCafeDbrType(handle, CAFENUM::DBR_PRIMITIVE);
        }
        else if (pvd.hasAlarm && !pvd.hasTS)
        {
            _chtype=dbf_type_to_DBR_STS (_chtype);
            handleHelper.setCafeDbrType(handle, CAFENUM::DBR_STS) ;
        }
        else if (pvd.hasTS)
        {
            _chtype=dbf_type_to_DBR_TIME (_chtype);
            handleHelper.setCafeDbrType(handle, CAFENUM::DBR_TIME) ;
        }

        //Find Native DataType
        if ( (status=cafeGranules.channelVerifyGet(handle, _chtype))  != ICAFE_NORMAL)
        {
            //Do this so that at least PVName etc is returned
            (*it_handle).getPVDataHolder(pvd);
            pvd.status=status;
            pvd.alarmStatus=-1;
            pvd.alarmSeverity=-1;
            pvd.ts.secPastEpoch=0;
            pvd.ts.nsec=0;
            return status;
        }
        if ((status=cafeGranules.channelPrepareGet(handle)) != ICAFE_NORMAL)
        {
            (*it_handle).getPVDataHolder(pvd);
            pvd.status=status;
            pvd.alarmStatus=-1;
            pvd.alarmSeverity=-1;
            pvd.ts.secPastEpoch=0;
            pvd.ts.nsec=0;
            return status;
        }
        if ((status=cafeGranules.channelExecuteGet(handle)) != ICAFE_NORMAL)
        {
            (*it_handle).getPVDataHolder(pvd);
            pvd.status=status;
            pvd.alarmStatus=-1;
            pvd.alarmSeverity=-1;
            pvd.ts.secPastEpoch=0;
            pvd.ts.nsec=0;
            return status;
        }

        (*it_handle).getPVDataHolder(pvd);
        //Report back timestamps to conduit for the blocking get methods
        handleHelper.setSTS(handle, pvd.alarmStatus, pvd.alarmSeverity, pvd.ts);
        handleHelper.setNelemToRetrieveFromCache(handle,pvd.nelem);
    }
    else
    {
        cout << __FILE__ << ":" << __LINE__ << endl;
        cout << __METHOD__ << endl;
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        pvd.status=ECAFE_INVALID_HANDLE;
        return ECAFE_INVALID_HANDLE;
    }

    if(MUTEX)
    {
        cafeMutex.lock();
    }; //lock
    handle_index.modify(it_handle, change_status(status));
    if(MUTEX)
    {
        cafeMutex.unlock();
    }; //unlock

    return status;
#undef __METHOD__
};

/**
 *  \brief Waits until handle callbacks has reported or timed out
 *  \param handle input: Handle to Conduit objects
 *  \return overallStatus: ICAFE_NORMAL if all OK else ECAFE error if encountered
 */
int CAFE::waitForGetEvent(unsigned int handle)
{
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);
    if (it_handle != handle_index.end())
    {
        return cafeGranules.waitForGetEvent(handle, (*it_handle).getChannelTimeoutPolicyGet().getTimeout());
    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }
};

/**
 *  \brief Waits until all handle callbacks have reported or timed out
 *  \param handleV input: Vector of handles to Conduit objects
 *  \param vRB output: vector of Bundled responses (reporting whether callback was completed)
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int CAFE::waitForBundledEvents(std::vector<unsigned int> handleV, std::vector<int> &vRB)
{
    return waitForBundledEvents(&handleV[0], handleV.size(), vRB);
};

/**
 *  \brief Waits until all handle callbacks have reported or timed out
 *  \param handleArray input: Array of handles to Conduit objects
 *  \param nHandles input: Lemgth of handleArray
 *  \param vRB output: vector of Bundled responses (reporting whether callback was completed)
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int CAFE::waitForBundledEvents(unsigned int * handleArray, unsigned int nHandles, std::vector<int> &vRB)
{
#define __METHOD__ \
"int CAFE::waitForBundledEvents(unsigned int * handleArray, unsigned int nHandles, vector<unsigned int> vRB)	"

    vRB.clear();
    vRB.reserve(nHandles);

    int statusBundle=ICAFE_NORMAL;
    map<unsigned int, int> bundleResponse;
    map<unsigned int, int>::iterator pos;
    std::vector<unsigned int> handleArrayV;
    handleArrayV.reserve(nHandles);
    handleArrayV.insert(handleArrayV.end(), &handleArray[0], &handleArray[nHandles]);
    std::set<unsigned int> uniqueHandles;
    std::set<unsigned int>::iterator uniquePos;

    //Remove duplicate handles
    for (unsigned int i=0; i<nHandles; ++i)
    {
        //std::cout << __METHOD__ << " handleArray[i] " << handleArray[i] << " " << i << std::endl;
        uniqueHandles.insert(handleArray[i]);
    }

    if (uniqueHandles.size() != nHandles)
    {
        cout << __FILE__ << ":" << __LINE__ << endl;
        cout << __METHOD__ << endl;
        cout << "For information purposes only: " << endl;
        cout << "Removed " << (nHandles - uniqueHandles.size())
             << " duplicate handles from the given " << nHandles
             << "; bundleResponse in waitForManyGetEvents will return with reduced length: "
             << uniqueHandles.size() << endl;
        cout << "This is not an error, but user might like to be aware that duplicate handles were submitted " << endl;

        unsigned int ij=0;
        for (uniquePos = uniqueHandles.begin(); uniquePos != uniqueHandles.end(); ++uniquePos)
        {
            handleArray[ij]= (unsigned int) (*uniquePos);
            ++ij;
        }
        nHandles=ij;
    }

    statusBundle=cafeGranules.waitForManyGetEvents(handleArray, nHandles, bundleResponse) ;
    if (statusBundle!=ICAFE_NORMAL)
    {
        cout << __FILE__ << ":" << __LINE__ << endl;
        cout << __METHOD__ << endl;
        cafeStatus.report(statusBundle);
    }

    for (unsigned int i=0; i<handleArrayV.size(); ++i)
    {
        pos=bundleResponse.find(handleArrayV[i]);
        if(pos != bundleResponse.end())
        {
            vRB.push_back(pos->second);
            if(pos->second != ICAFE_NORMAL)
            {
                resetCallbackGet(pos->first);
            }
        }
    }//for

    return statusBundle;
#undef __METHOD__
};

/**
 *  \brief Retrieves PV data packaged into an array of PVDataHolder class'
 *  \param handleArray input: Array of handles to Conduit objects
 *  \param nelem input: No of array elements
 *  \param pvdArray output: Array of PVDataHolder
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int CAFE::get(const unsigned int * handleArray, unsigned int nelem, \
              PVDataHolder * pvdArray)
{
#define __METHOD__ \
"CAFE::get(unsigned int * handleArray, unsigned int nelem, PVDataHolder * pvdArray)"

    int overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    CAFENUM::DBR_TYPE cdt;

    for (unsigned int i=0; i<nelem; ++i)
    {
        //Ensure that get retrieves DBR_TIME!
        status=handleHelper.getCafeDbrType(handleArray[i],cdt);

        if ((status == ECAFE_INVALID_HANDLE))
        {
            overallStatus=status;
            continue;
        }
        if (cdt!=CAFENUM::DBR_TIME)
        {
            handleHelper.setCafeDbrType(handleArray[i],CAFENUM::DBR_TIME);
        }

        bool doGet=true;
        ChannelGetActionWhenMonitorPolicy  channelGetActionWhenMonitorPolicy;
        policyHelper.getChannelGetActionWhenMonitorPolicy(handleArray[i],channelGetActionWhenMonitorPolicy);
        if (channelGetActionWhenMonitorPolicy.getActionKind() == CAFENUM::GET_FROM_CACHE)
        {
            if (handleHelper.getNmonitorData(handleArray[i])>0)
            {
                vector<MonitorPolicy> mpV;
                handleHelper.getMonitorPolicyVector(handleArray[i], mpV);
                for (unsigned int n=0; n<mpV.size(); ++n)
                {
                    doGet=false;
                    if (mpV[n].getCafeDbrType() != CAFENUM::DBR_TIME)
                    {
                        doGet=true;
                        cout << __FILE__ << ":" << __LINE__ << endl;
                        cout <<  __METHOD__ << endl;
                        cout << "***INFO*** YOU HAVE A MONITOR STARTED OM THIS CHANNEL "  << endl;
                        cout << "THAT DOES NOT RETRIEVE THE REQUESTED TIMESTAMPS " << endl;
                        cout << "WE WILL THEREFORE ATTEMPT AN ASYNCHRONOUS GET WITH DRB_TIME " << endl;
                        cout << "WE ADVICE THAT YOU MODIFY YOUR CODE TO START YOUR MONITOR WITH DRB_TIME" << endl;
                        cout << "Handle= " << handleArray[i] <<  " PV= " << handleHelper.getPVFromHandle(handleArray[i]) << endl;
                        mpV[n].print();
                    }
                }
            }
        }

        //Only do a get if handle does not have a monitor and policy dictates!
        if (doGet)
        {
            status=get(handleArray[i]);
            if ((status != ICAFE_NORMAL) && isGood)
            {
                overallStatus=status;
                isGood=false;
            }
        }
    }//for

    //Do Flush in waitForManyGetEvents

    if (overallStatus==ICAFE_WAITING_FOR_PREV_CALLBACK)
    {
        //reset
        overallStatus=ICAFE_NORMAL;
        isGood=true;
    }

    map<unsigned int, int> bundleResponse;
    int statusBundle=cafeGranules.waitForManyGetEvents(handleArray, nelem, bundleResponse) ;

    //For debugging
    //for (pos=bundleResponse.begin(); pos != bundleResponse.end(); ++pos) {
    //	cout << pos->first << " <-----------> " << pos->second << endl;
    //}

    //setNoElement to retrieve from Cache has to be number of elements in c.channelRequestMetaData.nelemCache
    //handleHelper.setNelemToRetrieveFromCache(handleArray[i],pvdArray[i].getNelem());
    //However getCache with pvdArray does not use c.channelRequestMetaData.nelemCache
    //Rather uses  pvdArray[i].nelem! So all OK
    //Added handleHelper.setNelemToRetrieveFromCache(handleArray[i],pvdArray[i].getNelem())
    //to get(handle, pvd); True also for the equivalent pvc
    //isDone??
    //Check on isDone for all
    //Our own timeout here!
    for (unsigned int i=0; i<nelem; ++i)
    {
        //status is always normal or ICAFE_CS_DISCONN
        status=getCacheNoWait(handleArray[i], pvdArray [i]);
        if (status!=ICAFE_NORMAL)
        {
            resetCallbackGet(handleArray[i]);
        }

        //cout << __METHOD__ << " status after getCacheNoWait\n " << status << " elem " << i <<  " pv " << pvdArray[i].getPV() << endl;
        //cout << "status = " << status << " pvdStatus= " << pvdArray[i].status << endl;
        //cout << "VALUE " << pvdArray[i].getAsString() << endl;

        if ((pvdArray[i].status != ICAFE_NORMAL) && isGood)
        {
            overallStatus=pvdArray[i].status;
            isGood=false;
        }
    }// for

    //cout<< statusBundle << " SB/OS "  << overallStatus << endl;

    if (overallStatus==ICAFE_NORMAL)
    {
        return statusBundle;
    }
    else
    {
        return overallStatus;
    }
#undef __METHOD__
};


/**
 *  \brief Retrieves PV DBR_CTRL data, packed into the PVCtrlHolder class
 *  \param pv input: pv name
 *  \param pvc output: PVCtrlHolder
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::getCtrl(const char * pv, PVCtrlHolder & pvc)
{
#define __METHOD__ "CAFE::getCtrl(const char *, PVCtrlHolder & pvc)"
    unsigned int handle;
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
        status= CAFE::getCtrl(handle, pvc);
    }
    return status;
#undef __METHOD__
}



/**
 *  Retrieves PV DBR_CTRL data, packaged into the PVCtrlHolder class
 *  \param  handle input: handle to cafeConduit object
 *  \param  pvc output: PVCtrlHolder containing ctrl data
 *  \return ICAFE_NORMAL(=ECA_NORMAL) if all OK
 */
int CAFE::getCtrl(const unsigned int handle, PVCtrlHolder & pvc)
{
#define __METHOD__ "CAFE::getCtrl(unsigned int handle, PVCtrlHolder & pvc)"

    status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {

        channelRequestMetaCtrlClient= (*it_handle).getChannelRequestMetaCtrlClient();


        if (pvc.getNelem() != channelRequestMetaCtrlClient.getNelem())
        {
            channelRequestMetaCtrlClient.setNelem(pvc.getNelem());
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            handle_index.modify(it_handle,
                                change_channelRequestMetaCtrlClient(channelRequestMetaCtrlClient));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
        }

        //Use Native DataType
        if ( (status=cafeGranules.channelVerifyGetCtrl(handle,
                     dbf_type_to_DBR_CTRL((*it_handle).getChannelRegalia().getDataType())))
                != ICAFE_NORMAL)
        {
            (*it_handle).getPVCtrlHolder(pvc);
            pvc.status=status;
            return status;
        }
        if ( (status=cafeGranules.channelPrepareGetCtrl(handle)) != ICAFE_NORMAL)
        {
            (*it_handle).getPVCtrlHolder(pvc);
            pvc.status=status;
            return status;
        }
        if ( (status=cafeGranules.channelExecuteGetCtrl(handle)) != ICAFE_NORMAL)
        {
            (*it_handle).getPVCtrlHolder(pvc);
            pvc.status=status;
            return status;
        }
        (*it_handle).getPVCtrlHolder(pvc);

        handleHelper.setNelemToRetrieveFromCtrlCache(handle,pvc.nelem);

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            pvc.status=ECAFE_INVALID_HANDLE;
        }
        return ECAFE_INVALID_HANDLE;
    }

    if(MUTEX)
    {
        cafeMutex.lock();
    }; //lock
    handle_index.modify(it_handle, change_status(status));
    if(MUTEX)
    {
        cafeMutex.unlock();
    }; //unlock

    return status;
#undef __METHOD__
};

/**
 *  \brief Retrieves data, packaged into an array of PVCtrlHolder class'
 *  \param handleArray input: Array of handles to Conduit object
 *  \param nelem input: No of array elements
 *  \param pvcArray output: Array of PVCtrlHolder
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int CAFE::getCtrl(const unsigned int * handleArray, unsigned int nelem, \
                  PVCtrlHolder * pvcArray)
{
#define __METHOD__ \
"CAFE::getCtrl(unsigned int * handleArray, unsigned int nelem, PVCtrlHolder * pvcArray)"

    int overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    for (unsigned int i=0; i<nelem; ++i)
    {
        status=getCtrl(handleArray[i]);
        if ((status != ICAFE_NORMAL) && isGood)
        {
            overallStatus=status;
            isGood=false;
        }
    }

    ca_flush_io();

    if (overallStatus==ICAFE_WAITING_FOR_PREV_CALLBACK)
    {
        //reset
        overallStatus=ICAFE_NORMAL;
        isGood=true;
    }

    for (unsigned int i=0; i<nelem; ++i)
    {
        status=getCtrlCache(handleArray[i], pvcArray [i]);
        if ((status != ICAFE_NORMAL) && isGood)
        {
            overallStatus=status;
            isGood=false;
        }
    }

    return overallStatus;
#undef __METHOD__
};



/**
 *  \brief   get for array of handles; data retrieved from cache
 *  \param   handleArray input:  array of handles
 *  \param   nHandles input: size of array
 *  \param   statusV output: vector of int values
 *  \return  overallStatus is ICAFE_NORMAL if all OK else the first ECAFE error encountered

 */
int CAFE::get(const unsigned int * handleArray, unsigned int nHandles, std::vector<int> &statusV)
{
    int overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    statusV.clear();
    statusV.reserve(nHandles);

    for (unsigned int i=0; i<nHandles; ++i)
    {
        //Does already have a monitor?
        if (handleHelper.getNmonitorData(handleArray[i])>0)
        {
            statusV.push_back(ICAFE_NORMAL); //ICAFE_HAS_MONITOR_GET_DONE_FROM_CACHE);
        }
        else
        {
            statusV.push_back(get(handleArray[i]));
        }
        if ((statusV[i] != ICAFE_NORMAL) && isGood)
        {
            overallStatus=statusV[i];
            isGood=false;
        }
    }//for
    //ca_flush_io(); //getCache will otherwise do flush
    return overallStatus; // report first failure in return statement
}

/**
 *  \brief get for array of handles; data retrieved from cache
 *  \param handleArray input: Array of handles to conduit object
 *  \param nelem input: Size of array
 *  \param statusArray output: Status array reporting from each get(handle)
 *  \return overallStatus ICAFE_NORMAL if all OK else the first ECAFE error encountered
 */
int CAFE::get(const unsigned int *handleArray, unsigned int nelem, \
              int *statusArray)
{
#define __METHOD__ "CAFE::get(unsigned int * handleArray, unsigned int nelem)"
    int overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    for (unsigned int i=0; i<nelem; ++i)
    {
        //Does already have a monitor?
        if (handleHelper.getNmonitorData(handleArray[i])>0)
        {
            statusArray[i]=ICAFE_NORMAL; //ICAFE_HAS_MONITOR_GET_DONE_FROM_CACHE;
        }
        else
        {
            statusArray[i]=get(handleArray[i]);
        }
        if ((statusArray[i] != ICAFE_NORMAL) && isGood)
        {
            overallStatus=statusArray[i];
            isGood=false;
        }
    }//for
    // flush now!
    //ca_flush_io(); //flush in get
    return overallStatus; // report first failure in return statement
#undef __METHOD__
};

/**
 *  \brief Sets data as packaged in the PVDataHolder class
 *  \param pv input: pv name
 *  \param pvd in/output: PVDataHolder
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, PVDataHolder & pvd)
{
#define __METHOD__ "CAFE::set(const char *, PVDataHolder & pvd)"
    unsigned int handle;
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
        status= CAFE::set(handle, pvd);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Sets data as packaged in the PVDataHolder class
 *  \param handle input: handle to Conduit object
 *  \param pvd input/output: PVDataHolder containing data
 *  \return ICAFE_NORMAL if all OK
 */
int CAFE::set(const unsigned int handle, PVDataHolder & pvd)
{
#define __METHOD__ "CAFE::set(unsigned int handle, PVDataHolder & pvd)"
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

    if (it_handle != handle_index.end())
    {
        channelRequestMetaDataClient= (*it_handle).getChannelRequestMetaDataClient();

        //cout << __METHOD__ << " pvd.getNelem() " << pvd.getNelem() << endl;
        //cout << " channelRequestMetaDataClient.getNelem() " << channelRequestMetaDataClient.getNelem() << endl;
        //cout << " channelRequestMetaDataClient.getOffset() " << channelRequestMetaDataClient.getOffset() << endl;

        if (pvd.getNelem() != (channelRequestMetaDataClient.getNelem() - channelRequestMetaDataClient.getOffset() ))
        {
            nelemPrevious=channelRequestMetaDataClient.getNelem();

            //Do NOT*** add offset to this!
            //This will extend the buffer to beyond the alloted size. Offsets are for READ only!
            channelRequestMetaDataClient.setNelem(pvd.getNelem());
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            handle_index.modify(it_handle, change_channelRequestMetaDataClient(channelRequestMetaDataClient));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
            changeNelemFlag=true;
        }

        bool soFarSoGood=true;

        //Find Native DataType
        if (soFarSoGood && (status=cafeGranules.channelVerifyPut(handle, pvd.dataType)) != ICAFE_NORMAL)
        {
            pvd.status=status;
            soFarSoGood=false;
        }
        if (soFarSoGood && (status=cafeGranules.channelPreparePut(handle)) != ICAFE_NORMAL)
        {
            pvd.status=status;
            soFarSoGood=false;
        }
        if (!soFarSoGood)
        {
            if(MUTEX)
            {
                cafeMutex.lock();
            }; //lock
            handle_index.modify(it_handle, change_status(status));
            if(MUTEX)
            {
                cafeMutex.unlock();
            }; //unlock

            if(changeNelemFlag)
            {
                channelRequestMetaDataClient.setNelem(nelemPrevious); //handleHelper.getNelemNative(handle));
                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                handle_index.modify(it_handle, change_channelRequestMetaDataClient(channelRequestMetaDataClient));
                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }
            }
            return status;
        }

        pvd.dbrDataType= ((*it_handle).getChannelRequestMetaPrimitive().getDbrDataType());

        switch ((*it_handle).getChannelRequestMetaPrimitive().getDbrDataType())
        {
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
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
            cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" <<  pvd.dataType <<endl;
            cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
            if(changeNelemFlag)
            {
                channelRequestMetaDataClient.setNelem(nelemPrevious); //handleHelper.getNelemNative(handle));
                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                handle_index.modify(it_handle, change_channelRequestMetaDataClient(channelRequestMetaDataClient));
                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }
            }
            return ECAFE_INVALID_SWITCH_CASE;
            break;
        }//switch

        if(status!=ICAFE_NORMAL)
        {
            pvd.status=status;
            soFarSoGood=false;
        }
        if (soFarSoGood && (status=cafeGranules.channelExecutePut(handle)) != ICAFE_NORMAL)
        {
            pvd.status=status;
            soFarSoGood=false;
        }
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
        }
        pvd.status=ECAFE_INVALID_HANDLE;
        return ECAFE_INVALID_HANDLE;
    }

    //Return for all, regardless of last status!
    if(MUTEX)
    {
        cafeMutex.lock();
    }; //lock
    handle_index.modify(it_handle, change_status(status));
    if(MUTEX)
    {
        cafeMutex.unlock();
    }; //unlock

    if(changeNelemFlag)
    {
        channelRequestMetaDataClient.setNelem(nelemPrevious); //handleHelper.getNelemNative(handle));
        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        handle_index.modify(it_handle, change_channelRequestMetaDataClient(channelRequestMetaDataClient));
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }
    }

    return status;
#undef __METHOD__
};

/**
 *  \brief Sets data as packaged in an array of PVDataHolder class
 *  \param handleArray input: Array of handles to Conduit object
 *  \param nelem input: No of array elements
 *  \param pvdArray output: Array of PVDataHolder containing data
 *  \return overallStatus: ICAFE_NORMAL if all OK else first ECAFE error encountered
 */
int CAFE::set(const unsigned int * handleArray, const unsigned int nelem, PVDataHolder * pvdArray)
{
#define __METHOD__ "CAFE::set(unsigned int *handleArray, unsigned int nelem, PVDataHolder * pvdArray)"

    ChannelRequestPolicy originalPol;
    ChannelRequestPolicy pol;
    PolicyHelper policyHelper;
    int statusL, overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    for (unsigned int i=0; i<nelem; ++i)
    {
        pvdArray[i].alarmStatus=-1;
        pvdArray[i].alarmSeverity=-1;
        pvdArray[i].ts.secPastEpoch=0;
        pvdArray[i].ts.nsec=0;
        pvdArray[i].status=ICAFE_NORMAL;

        it_handle = handle_index.find(handleArray[i]);
        if (it_handle != handle_index.end())
        {
            //Change policy not to flush
            //NO_WAIT, FLUSH_DESIGNATED_TO_CLIENT
            policyHelper.getChannelRequestPolicyPut(handleArray[i], originalPol);
            policyHelper.getChannelRequestPolicyPut(handleArray[i], pol);
            pol.setWaitKind(NO_WAIT);
            pol.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
            policyHelper.setChannelRequestPolicyPut(handleArray[i], pol);

            statusL=CAFE::set(handleArray[i], pvdArray[i]);

            //Return policy to previous
            policyHelper.setChannelRequestPolicyPut(handleArray[i], originalPol);
        }
        else
        {
            pvdArray[i].status=ECAFE_INVALID_HANDLE;
            statusL=pvdArray[i].status;
        }

        if ((statusL != ICAFE_NORMAL) && isGood)
        {
            overallStatus=statusL;
            isGood=false;
        }
    }//for
    ca_flush_io();
    return overallStatus;
#undef __METHOD__
};

/**
 *  \brief Interpets the waveform as a string
 *  \param handle input: Handle to Conduit object
 *  \param psWF output: string
 *  \return status: ICAFE_NORMAL if all OK else ECAFE error
 */
int CAFE::getWFAsString(unsigned int handle, std::string & psWF)
{
#define __METHOD__ "int CAFE::getWFAsString(unsigned int handle, string & psWF)"

    status=ICAFE_NORMAL;
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {
        unsigned int c=(*it_handle).getChannelRequestMetaDataClient().getNelem();
        unsigned int n=(*it_handle).getChannelRegalia().getNelem();
        dbr_char_t * val = new dbr_char_t[min(c,n)];

        status=cafeCappuccino.get(handle, DBR_CHAR, val);

        psWF = "";
        chtype cdt;
        handleHelper.getDataTypeNative(handle, cdt);
        unsigned int r=(*it_handle).getChannelRequestMetaData().getNelem();

        if (cdt==DBR_CHAR)
        {
            for (unsigned int i=0; i<r; ++i)
            {
                if (val[i] != '\0')
                {
                    psWF.append(1, (dbr_char_t) val[i]);
                }
            }
        }
        delete [] val;
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
        }
        return ECAFE_INVALID_HANDLE;
    }
    return status;
#undef __METHOD__
};


/**
 *  \brief Interpets the waveform, retrieved from Cache, as a string
 *  \param handle input: Handle to Conduit object
 *  \param psWF output: string
 *  \return status: ICAFE_NORMAL if all OK else ECAFE error
 */
int CAFE::getWFAsStringCache(unsigned int handle, std::string & psWF)
{
#define __METHOD__ "int CAFE::getWFAsStringCache(unsigned int handle, string & psWF)"

    status=ICAFE_NORMAL;
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {
        //unsigned int c=(*it_handle).getChannelRequestMetaDataClient().getNelem();
        unsigned int n=(*it_handle).getChannelRegalia().getNelem();
        unsigned int r =min( (*it_handle).getChannelRequestMetaData().getNelemCache(),
                             ((*it_handle).getChannelRequestMetaData().getNelem()-(*it_handle).getChannelRequestMetaData().getOffset()) ) ;
        //cout << "Cache c: " << c << " r: " << r << " n: " << n << endl;
        dbr_char_t * val = new dbr_char_t[min(r,n)];

        status=cafeCappuccino.getCache(handle, DBR_CHAR, val);

        psWF = "";
        chtype cdt;
        handleHelper.getDataTypeNative(handle, cdt);

        if (cdt==DBR_CHAR)
        {
            for (unsigned int i=0; i<r; ++i)
            {
                if (val[i] != '\0')
                {
                    psWF.append(1, (dbr_char_t) val[i]);
                }
            }
        }
        delete [] val;
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
        }
        return ECAFE_INVALID_HANDLE;
    }
    return status;
#undef __METHOD__
};

////////////////////////////////////////////// by Name ////////////////////////////////
/**
 *  \brief Get string(s), alarms and timeStamp
 *  \param pv input: pv name
 *  \param valStr output: array of strings
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, std::string  * valStr,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //0
{
#define __METHOD__ \
  "int CAFE::get(const char * pv, string  * val, dbr_short_t &, dbr_short_t &, epicsTimeStamp &ts)"

    unsigned int handle;
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
        dbr_string_t * val = new dbr_string_t[handleHelper.getNelemClient(handle)];
        status=cafeSoluble.get(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
        for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
        {
            valStr[i]=val[i];
        }
        delete [] val;
    }
    return status;
#undef __METHOD__
}

/**
 *
 *  \brief Get string(s) and alarms
 *  \param pv input: pv name
 *  \param valStr output: array of strings
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, std::string  * valStr,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)   //0
{
#define __METHOD__ \
"int CAFE::get(const char * pv, string  * val, dbr_short_t &, dbr_short_t &)"

    unsigned int handle;
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
        dbr_string_t * val = new dbr_string_t[handleHelper.getNelemClient(handle)];
        status=cafeSoluble.get(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
        for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
        {
            valStr[i]=val[i];
        }
        delete [] val;
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get string(s) (DBR_STRING )
 *  \param pv input: pv name
 *  \param valStr output: array of strings
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, std::string  * valStr)   //0
{
#define __METHOD__ "int CAFE::get(const char * pv, string * val)"

    unsigned int handle;
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
        dbr_string_t * val = new dbr_string_t[handleHelper.getNelemClient(handle)];
        status=cafeSoluble.get(handle, DBR_STRING, val);
        for (unsigned int i=0; i< handleHelper.getNelemRequest(handle); ++i)
        {
            valStr[i]=val[i];
        }
        delete [] val;
    }
    return status;
#undef __METHOD__
}


/**
 *  \brief Get string(s), alarms and timeStamp (DBR_TIME_STRING)
 *  \param pv input: pv name
 *  \param val output: array of strings
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_string_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //0
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_string_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"

    unsigned int handle;
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
        status=cafeSoluble.get(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get string(s) and alarms (DBR_STS_STRING)
 *  \param pv input: pv name
 *  \param val output: array of strings
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_string_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)   //0
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_string_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity)"
    unsigned int handle;
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
        status=cafeSoluble.get(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get string(s) (DBR_STRING )
 *  \param pv input: pv name
 *  \param val output: array of strings
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_string_t  * val)   //0
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_string_t * val)"
    unsigned int handle;
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
        status=cafeSoluble.get(handle, DBR_STRING, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get short(s), alarms and timeStamp (DBR_TIME_SHORT)
 *  \param pv input: pv name
 *  \param val output: array of shorts
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_short_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //1
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_short_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        status=cafeSchale.get(handle, DBR_TIME_SHORT, val, alarmStatus, alarmSeverity, ts);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get short(s) and alarms (DBR_STS_SHORT)
 *  \param pv input: pv name
 *  \param val output: array of shorts
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_short_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)   //1
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_short_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity)"
    unsigned int handle;
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
        status=cafeSchale.get(handle, DBR_STS_SHORT, val, alarmStatus, alarmSeverity);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get short(s) (DBR_SHORT)
 *  \param pv input: pv name
 *  \param val output: array of shorts
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_short_t  * val)   //1
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_short_t * val)"
    unsigned int handle;
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
        status=cafeSchale.get(handle, DBR_SHORT, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get float(s), alarms and timeStamp (DBR_TIME_FLOAT)
 *  \param pv input: pv name
 *  \param val output: array of floats
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_float_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //2
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_float_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        status=cafeFrappuccino.get(handle, DBR_TIME_FLOAT, val, alarmStatus, alarmSeverity, ts);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get float(s) and alarms (DBR_STS_FLOAT)
 *  \param pv input: pv name
 *  \param val output: array of floats
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_float_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)   //2
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_float_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity)"
    unsigned int handle;
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
        status=cafeFrappuccino.get(handle, DBR_STS_FLOAT, val, alarmStatus, alarmSeverity);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get float(s) (DBR_FLOAT)
 *  \param pv input: pv name
 *  \param val output: array of floats
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_float_t  * val)   //2
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_float_t * val)"
    unsigned int handle;
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
        status=cafeFrappuccino.get(handle, DBR_FLOAT, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get enum(s), alarms and timeStamp (DBR_TIME_ENUM)
 *  \param pv input: pv name
 *  \param val output: array of enums (dbr_enum_t i.e. unsigned short)
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_enum_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //3
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_enum_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        status=cafeEspresso.get(handle, DBR_TIME_ENUM, val, alarmStatus, alarmSeverity, ts);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get enum(s) and alarms (DBR_STS_ENUM)
 *  \param pv input: pv name
 *  \param val output: array of enums (dbr_enum_t i.e. unsigned short)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_enum_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)   //3
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_enum_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity)"
    unsigned int handle;
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
        status=cafeEspresso.get(handle, DBR_STS_SHORT, val, alarmStatus, alarmSeverity);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get enum(s) (DBR_ENUM)
 *  \param pv input: pv name
 *  \param val output: array of enums (dbr_enum_t, i.e., unsigned short)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_enum_t  * val)   //3
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_enum_t * val)"
    unsigned int handle;
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
        status=cafeEspresso.get(handle, DBR_SHORT, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get unsigned char(s), alarms and timeStamp (DBR_TIME_CHAR)
 *  \param pv input: pv name
 *  \param val output: array of unsigned char (dbr_char_t)
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_char_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //4
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_char_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        status=cafeCappuccino.get(handle, DBR_TIME_CHAR, val, alarmStatus, alarmSeverity, ts);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get unsigned char(s) and alarms (DBR_STS_CHAR)
 *  \param pv input: pv name
 *  \param val output: array of unsigned char (dbr_char_t)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_char_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)   //4
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_char_t  * val, dbr_short_t &alarmStatus, \
                dbr_short_t &alarmSeverity)"
    unsigned int handle;
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
        status=cafeCappuccino.get(handle, DBR_STS_CHAR, val, alarmStatus, alarmSeverity);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get unsigned char(s) (DBR_CHAR)
 *  \param pv input: pv name
 *  \param val output: array of unsigned char (dbr_char_t)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_char_t  * val)   //4
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_char_t * val)"
    unsigned int handle;
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
        status=cafeCappuccino.get(handle, DBR_CHAR, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get long(s), alarms and timeStamp (DBR_TIME_LONG)
 *  \param pv input: pv name
 *  \param val output: array of longs (dbr_long_t)
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_long_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //5
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_long_t * val, dbr_short_t &, dbr_short_t &, epicsTimeStamp &ts)"
    unsigned int handle;
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
        status=cafeLatte.get(handle, DBR_TIME_LONG, val, alarmStatus, alarmSeverity, ts);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get long(s) and alarms (DBR_STS_LONG)
 *  \param pv input: pv name
 *  \param val output: array of longs (dbr_long_t)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_long_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)   //5
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_long_t  * val, dbr_short_t &, dbr_short_t &)"
    unsigned int handle;
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
        status=cafeLatte.get(handle, DBR_STS_LONG, val, alarmStatus, alarmSeverity);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get long(s) (DBR_LONG)
 *  \param pv input: pv name
 *  \param val output: array of dbr_long_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_long_t  * val)   //5
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_long_t * val)"
    unsigned int handle;
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
        status=cafeLatte.get(handle, DBR_LONG, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get long(s) (DBR_LONG)
 *  \param pv input: pv name
 *  \param val output: array of long long
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, long long  * val)  //5+ long long
{
#define __METHOD__ "int CAFE::get(const char * pv, long long * val)"
    unsigned int handle;
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
        status=CAFE::get(handle, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get long long(s) (DBR_STS_*)
 *  \param pv input: pv name
 *  \param val output: array of long long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, long long  * val, dbr_short_t &alarmStatus,
              dbr_short_t &alarmSeverity)  //5+ long long
{
#define __METHOD__ \
"int CAFE::get(const char * pv, long long  * val, dbr_short_t &, dr_short_t &)"
    unsigned int handle;
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
        status=CAFE::get(handle, val, alarmStatus, alarmSeverity);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get long long(s) (DBR_TIME_*)
 *  \param pv input: pv name
 *  \param val output: array of long long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, long long  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)  //5+ long long
{
#define __METHOD__ \
"int CAFE::get(const char * pv, long long * val, dbr_short_t &, dbr_short_t &, epicsTimeStamp &ts)"
    unsigned int handle;
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
        status=CAFE::get(handle, val, alarmStatus, alarmSeverity, ts);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get double(s), alarms and timeStamp (DBR_TIME_DOUBLE)
 *  \param pv input: pv name
 *  \param val output: array of doubles (dbr_double_t)
 *  \param alarmStatus output: alarm status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_double_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //6
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_double_t  * val, dbr_short_t &, dbr_short_t &, epicsTimeStamp &ts)"
    unsigned int handle;
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
        status=cafeDoppio.get(handle, DBR_TIME_DOUBLE, val, alarmStatus, alarmSeverity, ts);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get double(s) and alarms (DBR_STS_DOUBLE)
 *  \param pv input: pv name
 *  \param val output: array of enums (dbr_double_t)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_double_t  * val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)   //6
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_double_t  * val, dbr_short_t &, dbr_short_t &)"
    unsigned int handle;
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
        status=cafeDoppio.get(handle, DBR_STS_DOUBLE, val, alarmStatus, alarmSeverity);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get double(s) (DBR_DOUBLE)
 *  \param pv input: pv name
 *  \param val output: array of doubles (dbr_double_t)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_double_t  * val)   //6
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_double_t * val)"
    unsigned int handle;
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
        status=cafeDoppio.get(handle, DBR_DOUBLE, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get string
 *  \param pv input: pv name
 *  \param valStr output: string
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, std::string &valStr,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //0+
{
#define __METHOD__ \
"int CAFE::get(const char * pv, string  &valStr," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            valStr= val[0];
        }
        else
        {
            valStr= "";
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get string
 *  \param pv input: pv name
 *  \param valStr output: string
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, std::string &valStr, dbr_short_t &alarmStatus, \
              dbr_short_t &alarmSeverity)   //0+
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_string_t  &valStr, dbr_short_t &, dbr_short_t &)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            valStr= val[0];
        }
        else
        {
            valStr= "";
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get string
 *  \param pv input: pv name
 *  \param valStr output: string
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, std::string  &valStr)   //0+
{
#define __METHOD__ "int CAFE::get(const char * pv, string &_val)"
    unsigned int handle;
    status=ICAFE_NORMAL;
    try
    {
        status = open(pv, handle);
    }
    catch (CAFEException_open & e)
    {
        return e.pvEx.statusCode;
    }
    if (status == ICAFE_NORMAL)
    {
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_STRING, val);
        if (status==ICAFE_NORMAL)
        {
            valStr= val[0];
        }
        else
        {
            valStr= "";
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief  Get string (DBR_TIME_STRING)
 *  \param pv input: pv name
 *  \param _val output: string
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_string_t  &_val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //0
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_string_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            sprintf(_val, val[0]);
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get string (DBR_STS_STRING)
 *  \param pv input: pv name
 *  \param _val output: string
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_string_t  &_val, dbr_short_t &alarmStatus, \
              dbr_short_t &alarmSeverity)   //0
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_string_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            sprintf(_val, val[0]);
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get string (DBR_STRING)
 *  \param pv input: pv name
 *  \param _val output: string
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_string_t  &_val)   //0
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_string_t  &_val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1]= {""};
        status=cafeSoluble.get(handle, DBR_STRING, val);
        if (status==ICAFE_NORMAL)
        {
            sprintf(_val, val[0]);
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get short (DBR_SHORT)
 *  \param pv input: pv name
 *  \param _val output: hort
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_short_t  &_val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //1
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_short_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_short_t val[1]= {0};
        status=cafeSchale.get(handle, DBR_TIME_SHORT, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get short (DBR_SHORT)
 *  \param pv input: pv name
 *  \param _val output: 2hort
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_short_t  &_val, dbr_short_t &alarmStatus, \
              dbr_short_t &alarmSeverity)   //1
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_short_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_short_t val[1]= {0};
        status=cafeSchale.get(handle, DBR_STS_SHORT, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get short (DBR_SHORT)
 *  \param pv input: pv name
 *  \param _val output: short
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_short_t  &_val)   //1
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_short_t  &_val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_short_t val[1]= {0};
        status=cafeSchale.get(handle, DBR_SHORT, val);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get float (DBR_FLOAT)
 *  \param pv input: pv name
 *  \param _val output:float
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_float_t  &_val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //2
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_float_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_float_t val[1]= {0};
        status=cafeFrappuccino.get(handle, DBR_TIME_FLOAT, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  \brief Get float (DBR_FLOAT)
 *  \param pv input: pv name
 *  \param _val output:float
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_float_t  &_val, dbr_short_t &alarmStatus, \
              dbr_short_t &alarmSeverity)   //2
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_float_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_float_t val[1]= {0};
        status=cafeFrappuccino.get(handle, DBR_STS_FLOAT, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get float (DBR_FLOAT)
 *  \param pv input: pv name
 *  \param _val output:float
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_float_t  &_val)   //2
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_float_t  &_val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_float_t val[1]= {0};
        status=cafeFrappuccino.get(handle, DBR_FLOAT, val);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  \brief Get enum (DBR_ENUM)
 *  \param pv input: pv name
 *  \param _val output:dbr_enum_t (unsigned short)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_enum_t  &_val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //3
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_enum_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_enum_t val[1]= {0};
        status=cafeEspresso.get(handle, DBR_TIME_ENUM, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get enum (DBR_ENUM)
 *  \param pv input: pv name
 *  \param _val output: dbr_enum_t (unsigned short)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_enum_t  &_val, dbr_short_t &alarmStatus, \
              dbr_short_t &alarmSeverity)   //3
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_enum_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_enum_t val[1]= {0};
        status=cafeEspresso.get(handle, DBR_STS_ENUM, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get enum (DBR_ENUM)
 *  \param pv input: pv name
 *  \param _val output: dbr_enum_t (unsigned short)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_enum_t  &_val)   //3
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_enum_t  &_val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_enum_t val[1]= {0};
        status=cafeEspresso.get(handle, DBR_ENUM, val);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get dbr_char_t (DBR_CHAR)
 *  \param pv input: pv name
 *  \param _val output:unsigned char (dbr_char_t)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_char_t  &_val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //4
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_char_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_char_t val[1]= {0};
        status=cafeCappuccino.get(handle, DBR_TIME_CHAR, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get dbr_char_t (DBR_CHAR)
 *  \param pv input: pv name
 *  \param _val output:unsigned char (dbr_char_t)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_char_t  &_val, dbr_short_t &alarmStatus, \
              dbr_short_t &alarmSeverity)   //4
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_char_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_char_t val[1]= {0};
        status=cafeCappuccino.get(handle, DBR_STS_CHAR, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get dbr_char_t (DBR_CHAR)
 *  \param pv input: pv name
 *  \param _val output: unsigned char (dbr_char_t)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_char_t  &_val)   //4
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_char_t  &_val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_char_t val[1]= {0};
        status=cafeCappuccino.get(handle, DBR_CHAR, val);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get int (DBR_LONG)
 *  \param pv input: pv name
 *  \param _val output:long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_long_t  &_val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //5
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_long_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_long_t val[1]= {0};
        status=cafeLatte.get(handle, DBR_TIME_LONG, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get int (DBR_LONG)
 *  \param pv input: pv name
 *  \param _val output:long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_long_t  &_val, dbr_short_t &alarmStatus, \
              dbr_short_t &alarmSeverity)   //5
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_long_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_long_t val[1]= {0};
        status=cafeLatte.get(handle, DBR_STS_LONG, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get int (DBR_LONG)
 *  \param pv input: pv name
 *  \param _val output:long
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_long_t  &_val)   //5
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_long_t  &_val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_long_t val[1]= {0};
        status=cafeLatte.get(handle, DBR_LONG, val);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get long long
 *  \param pv input: pv name
 *  \param _val output:long long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, long long &_val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //5
{
#define __METHOD__ \
"int CAFE::get(const char * pv, long long  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        long long val[1]= {0};
        status=CAFE::get(handle, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get long long
 *  \param pv input: pv name
 *  \param _val output:long long
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, long long &_val, dbr_short_t &alarmStatus, \
              dbr_short_t &alarmSeverity)   //5
{
#define __METHOD__ \
"int CAFE::get(const char * pv, long long  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        long long val[1]= {0};
        status=CAFE::get(handle, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get long long
 *  \param pv input: pv name
 *  \param _val output:long long
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, long long  &_val)   //5
{
#define __METHOD__ "int CAFE::get(const char * pv, long long  &_val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        long long val[1]= {0};
        status=CAFE::get(handle, val);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get double (DBR_DOUBLE)
 *  \param pv input: pv name
 *  \param _val output: dbr_double_t (double)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \param ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_double_t  &_val,
              dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)   //6
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_double_t  &_val," \
        " dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_double_t val[1]= {0};
        status=cafeDoppio.get(handle, DBR_TIME_DOUBLE, val, alarmStatus, alarmSeverity, ts);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get double (DBR_DOUBLE)
 *  \param pv input: pv name
 *  \param _val output: dbr_double_t (double)
 *  \param alarmStatus output: alarm Status
 *  \param alarmSeverity output: alarm Severity
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_double_t  &_val, dbr_short_t &alarmStatus, \
              dbr_short_t &alarmSeverity)   //6
{
#define __METHOD__ \
"int CAFE::get(const char * pv, dbr_double_t  &_val, dbr_short_t &, dbr_short_t &)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_double_t val[1]= {0};
        status=cafeDoppio.get(handle, DBR_STS_DOUBLE, val, alarmStatus, alarmSeverity);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Get double (DBR_DOUBLE)
 *  \param pv input: pv name
 *  \param _val output: dbr_double_t (double)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::get(const char * pv, dbr_double_t  &_val)   //6
{
#define __METHOD__ "int CAFE::get(const char * pv, dbr_double_t  &_val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_double_t val[1]= {0};
        status=cafeDoppio.get(handle, DBR_DOUBLE, val);
        if (status==ICAFE_NORMAL)
        {
            _val= val[0];
        }
        //Switch back to previous value
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Helper function to methods calling scalars. \n
 *  Sets number of elements to retrieve from cache to one
 *  \param handle input: handle to Conduit object
 *  \return nelements previous to the set
 */
unsigned int CAFE::setNelemToRetrieveFromCacheToOne(const unsigned int handle)
{
#define __METHOD__ "int CAFE::setNelemToRetrieveFromCacheToOne(const unsigned int handle)"
    unsigned int nelemPrevious, nelemRequestedCheck, nelemPreviousCache=0;
    unsigned int nelemRequested=1;
    nelemPrevious=handleHelper.getNelemRequest(handle);
    nelemPreviousCache=handleHelper.getNelemToRetrieveFromCache(handle);
    //Check the number of elements requested?
    if (max(nelemPrevious,nelemPreviousCache)>1)
    {
        nelemRequestedCheck = handleHelper.setNelemToRetrieveFromCache(handle,nelemRequested);
        if (nelemRequestedCheck != nelemRequested)
        {
            std::cout << __FILE__ << ":" << __LINE__ << std::endl;
            std::cout << __METHOD__ << std::endl;
            std::cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
                      << nelemPrevious << std::endl;
            std::cout << "to: " << nelemRequested << " but got instead: "
                      << nelemRequestedCheck  << std::endl;
        }
    }
    return nelemPrevious;
#undef __METHOD__
}

/**
 *  \brief  Helper function to methods calling scalars \n
 *  Sets number of elements to retrieve from cache to the previous number
 *  \param handle input: handle to Conduit object
 *  \param nelemPrevious input: nelem to set
 *  \return nelements actually set (should equal nelemPrevious)
 */
unsigned int CAFE::setNelemToRetrieveFromCacheToPrevious(const unsigned int handle, \
        unsigned int nelemPrevious)
{
#define __METHOD__ \
"int CAFE::setNelemToRetrieveFromCacheToPrevious(unsigned int handle, unsigned int nelemPrevious)"
    unsigned int nelemPreviousCheck=0;
    unsigned int nelemRequested=1;
    //Switch back to previous value
    if (nelemPrevious>0)
    {
        nelemPreviousCheck= handleHelper.setNelemToRetrieveFromCache(handle,nelemPrevious);
        if (nelemPreviousCheck != nelemPrevious)
        {
            std::cout << __FILE__ << ":" << __LINE__ << std::endl;
            std::cout << __METHOD__ << std::endl;
            std::cout << "Internal CAFE FUNNY: Wanted to re-set the no. elements from: "
                      << nelemRequested << std::endl;
            std::cout << "to the previous: " << nelemPrevious << " but got instead: "
                      << nelemPreviousCheck  << std::endl;
        }
    }
    return nelemPreviousCheck;
#undef __METHOD__
}

/**
 *  \brief Helper function to methods calling scalars. \n
 *  Sets number of elements to one
 *  \param handle input: handle to Conduit object
 *  \return nelements previous to the set
 */
unsigned int CAFE::setNelemToOne(const unsigned int handle)
{
#define __METHOD__ "int CAFE::setNelemToOne(const unsigned int handle)"
    unsigned int nelemPrevious, nelemRequestedCheck=0;
    unsigned int nelemRequested=1;
    nelemPrevious=handleHelper.getNelemClient(handle);

    //Check the number of elements requested?
    if (nelemPrevious>1)
    {
        nelemRequestedCheck = handleHelper.setNelem(handle,nelemRequested);
        if (nelemRequestedCheck != nelemRequested)
        {
            std::cout << __FILE__ << ":" << __LINE__ << std::endl;
            std::cout << __METHOD__ << std::endl;
            std::cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
                      << nelemPrevious << endl;
            std::cout << "to: " << nelemRequested << " but got instead: "
                      << nelemRequestedCheck  << endl;
        }
    }
    return nelemPrevious;
#undef __METHOD__
}

/**
 *  \brief Helper function to methods calling scalars. \n
 *  Sets number of elements to the "previous" number
 *  \param handle input: handle to Conduit object
 *  \param nelemPrevious input: nelem to set
 *  \return nelements actually set (should equal nelemPrevious)
 */
unsigned int CAFE::setNelemToPrevious(const unsigned int handle, \
                                      unsigned int nelemPrevious)
{
#define __METHOD__ \
"int CAFE::setNelemToPrevious(unsigned int handle, unsigned int nelemPrevious)"
    unsigned int nelemPreviousCheck=nelemPrevious;

    if (handleHelper.getNelemClient(handle)!= nelemPrevious)
    {
        nelemPreviousCheck= handleHelper.setNelem(handle,nelemPrevious);
        if (nelemPreviousCheck != nelemPrevious)
        {
            if (nelemPreviousCheck != handleHelper.getNelemNative(handle))
            {
                std::cout << __FILE__ << ":" << __LINE__ << std::endl;
                std::cout << __METHOD__ << std::endl;
                std::cout << "Internal CAFE FUNNY: Wanted to re-set the no. elements " << std::endl;
                std::cout << "to the previous: " << nelemPrevious << " but got instead: "
                          << nelemPreviousCheck  << endl;
                std::cout << "No of native elements is " << handleHelper.getNelemNative(handle) << std::endl;
            }
        }
    }
    return nelemPreviousCheck;
#undef __METHOD__
}

/**
 *  \brief Set long long
 *  \param handle input: handle to Conduit object
 *  \param val input: array of double
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(unsigned int handle, const long long * val)   //5+ long long
{
#define __METHOD__ "CAFE::set(unsigned int handle, const long long * _val)"

    ChannelRegalia channelInfo;
    CAFE::getChannelInfo(handle, channelInfo);
    if (channelInfo.getCafeConnectionState() == ICAFE_CS_NEVER_CONN)
    {
        return ICAFE_CS_NEVER_CONN;
    }
    else if (channelInfo.getCafeConnectionState()==ICAFE_CS_CLOSED)
    {
        return ICAFE_CS_CLOSED;
    }

    int _status=ICAFE_NORMAL;
    unsigned int nn=handleHelper.getNelemNative(handle);

    if (channelInfo.getDataType() == DBR_STRING)
    {
        dbr_string_t * _sVal = new dbr_string_t[nn];

        //Convert long long to string;
        std::stringstream ss;
        for (unsigned int i=0; i < handleHelper.getNelemClient(handle); ++i)
        {
            ss.clear();
            ss << val[i];
            strcpy(_sVal[i], ss.str().c_str());
        }
        _status=cafeSoluble.set (handle, DBR_STRING,  _sVal);
        delete [] _sVal;
    }
    else if (channelInfo.getDataType() == DBR_DOUBLE)
    {
        dbr_double_t * _dVal = new dbr_double_t[nn];
        for (unsigned int i=0; i < handleHelper.getNelemClient(handle); ++i)
        {
            _dVal[i] = (double) val[i];
        }
        _status=cafeDoppio.set (handle, DBR_DOUBLE,  _dVal);
        delete [] _dVal;
    }
    else if (channelInfo.getDataType() == DBR_FLOAT)
    {
        dbr_float_t * _fVal = new dbr_float_t[nn];
        for (unsigned int i=0; i < handleHelper.getNelemClient(handle); ++i)
        {
            _fVal[i] = (float) val[i];
        }
        _status=cafeFrappuccino.set (handle, DBR_FLOAT,  _fVal);
        delete [] _fVal;
    }
    else
    {
        dbr_long_t * _lVal = new dbr_long_t[nn];
        for (unsigned int i=0; i < handleHelper.getNelemClient(handle); ++i)
        {
            _lVal[i] = (dbr_long_t) val[i];
        }
        _status=cafeLatte.set (handle, DBR_LONG,  _lVal);
        delete [] _lVal;
    }
    return _status;
#undef __METHOD__
}

/**
 *  \brief Set double(s)
 *  \param pv input: pv name
 *  \param val input: array of double
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_double_t * val)   //6
{
#define __METHOD__ "CAFE::set(const char *, dbr_double_t * val)"
    unsigned int handle;
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
        status=CAFE::cafeDoppio.set((const unsigned int) handle, DBR_DOUBLE, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set long long(s)
 *  \param pv input: pv name
 *  \param val input: array of long long
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const long long * val)   //5+ long long
{
#define __METHOD__ "CAFE::set(const char *, long long * val)"
    unsigned int handle;
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
        status=CAFE::set((const unsigned int) handle, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set dbr_long_t(s)
 *  \param pv input: pv name
 *  \param val input: array of dbr_long_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_long_t * val)   //5
{
#define __METHOD__ "CAFE::set(const char *, dbr_long_t * val)"
    unsigned int handle;
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
        status=CAFE::cafeLatte.set((const unsigned int) handle, DBR_LONG, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set unsigned char(s)
 *  \param pv input: pv name
 *  \param val input: array of dbr_char_t (unsigned char)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_char_t * val)   //4
{
#define __METHOD__ "CAFE::set(const char *, dbr_char_t * val)"
    unsigned int handle;
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
        status=CAFE::cafeCappuccino.set((const unsigned int) handle, DBR_CHAR, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set dbr_enum_t(s)
 *  \param pv input: pv name
 *  \param val input: array ofdbr_enum_t (unsigned short)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_enum_t * val)   //3
{
#define __METHOD__ "CAFE::set(const char *, dbr_enum_t * val)"
    unsigned int handle;
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
        status=CAFE::cafeEspresso.set((const unsigned int) handle, DBR_ENUM, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set float(s)
 *  \param pv input: pv name
 *  \param val input: array of float
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_float_t * val)   //2
{
#define __METHOD__ "CAFE::set(const char *, dbr_float_t * val)"
    unsigned int handle;
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
        status=CAFE::cafeFrappuccino.set((const unsigned int) handle, DBR_FLOAT, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set short(s)
 *  \param pv input: pv name
 *  \param val input: array of dbr_short_t (signed short)
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_short_t * val)   //1
{
#define __METHOD__ "CAFE::set(const char *, dbr_short_t * val)"
    unsigned int handle;
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
        status=CAFE::cafeSchale.set((const unsigned int) handle, DBR_SHORT, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set dbr_string_t(s)
 *  \param pv input: pv name
 *  \param val input: array of dbr_string_t (char[40])
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_string_t * val)   //0
{
#define __METHOD__ "CAFE::set(const char *, dbr_string_t * val)"
    unsigned int handle;
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
        status=CAFE::cafeSoluble.set((const unsigned int) handle, DBR_STRING, val);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set dbr_double_t
 *  \param pv input: pv name
 *  \param _val input: dbr_double_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_double_t _val)   //6
{
#define __METHOD__ "CAFE::set(const char *, dbr_double_t  val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_double_t val[1];
        val[0]= _val;
        status=CAFE::cafeDoppio.set((const unsigned int) handle, DBR_DOUBLE, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set long long
 *  \param pv input: pv name
 *  \param _val input: long long
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const long long _val)   //5+ long long
{
#define __METHOD__ "CAFE::set(const char *, long long  val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_long_t val[1];
        val[0]= _val;
        status=CAFE::set((const unsigned int) handle, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}
/**
 *  \brief Set dbr_long_t
 *  \param pv input: pv name
 *  \param _val input: dbr_long_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_long_t _val)   //5
{
#define __METHOD__ "CAFE::set(const char *, dbr_long_t  val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_long_t val[1];
        val[0]= _val;
        status=CAFE::cafeLatte.set((const unsigned int) handle, DBR_LONG, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set dbr_char_t (unsigned char)
 *  \param pv input: pv name
 *  \param _val input: dbr_char_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_char_t _val)   //4
{
#define __METHOD__ "CAFE::set(const char *, dbr_char_t  val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_char_t val[1];
        val[0]= _val;
        status=CAFE::cafeCappuccino.set((const unsigned int) handle, DBR_CHAR, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set dbr_enum_t (unsigned short)
 *  \param pv input: pv name
 *  \param _val input: dbr_enum_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_enum_t _val)   //3
{
#define __METHOD__ "CAFE::set(const char *, dbr_enum_t  val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_enum_t val[1];
        val[0]= _val;
        status=CAFE::cafeEspresso.set((const unsigned int) handle, DBR_ENUM, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set dbr_float_t
 *  \param pv input: pv name
 *  \param _val input: dbr_float_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_float_t _val)   //2
{
#define __METHOD__ "CAFE::set(const char *, dbr_float_t  val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_float_t val[1];
        val[0]= _val;
        status=CAFE::cafeFrappuccino.set((const unsigned int) handle, DBR_FLOAT, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set dbr_short_t
 *  \param pv input: pv name
 *  \param _val input: dbr_short_t
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_short_t _val)   //1
{
#define __METHOD__ "CAFE::set(const char *, dbr_short_t  val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_short_t val[1];
        val[0]= _val;
        status=CAFE::cafeSchale.set((const unsigned int) handle, DBR_SHORT, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set dbr_string_t
 *  \param pv input: pv name
 *  \param _val input: dbr_string_t (char[40])
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const dbr_string_t _val)   //0
{
#define __METHOD__ "CAFE::set(const char *, dbr_string_t  val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1];
        sprintf(val[0],"%s", _val);
        status=CAFE::cafeSoluble.set((const unsigned int) handle, DBR_STRING, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief Set string
 *  \param pv input: pv name
 *  \param _val input: string
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::set(const char * pv, const std::string _val)   //0 string
{
#define __METHOD__ "CAFE::set(const char *, string  val)"
    unsigned int handle;
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
        unsigned int nelemPrevious=CAFE::setNelemToOne(handle);
        dbr_string_t val[1];
        sprintf(val[0],"%s", _val.c_str());
        status=CAFE::cafeSoluble.set((const unsigned int) handle, DBR_STRING, val);
        CAFE::setNelemToPrevious(handle, nelemPrevious);
    }
    return status;
#undef __METHOD__
}

/**
 *  \brief  Wait for action for given hadnles to complete
 *  \param h input: Array of handles
 *  \param nelem input: No of elements in Array
 *  \return ECA_NORMAL if all OK else ECAFE error
 */
int CAFE::waitForPut(const unsigned int *h, const unsigned int nelem)
{
    ca_flush_io();
    status=ICAFE_NORMAL;
    bool isGood=true;
    int overallStatus=status;
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    ChannelTimeoutPolicy channelTimeoutPolicyPut;

    for (unsigned int i=0; i<nelem; ++i)
    {
        it_handle = handle_index.find(h[i]);
        if (it_handle != handle_index.end())
        {
            channelTimeoutPolicyPut = (*it_handle).getChannelTimeoutPolicyPut();
            status=cafeGranules.waitForPutEvent(h[i], channelTimeoutPolicyPut.getTimeout() );
            if ((status != ICAFE_NORMAL) && isGood)
            {
                overallStatus=status;
                isGood=false;
            }
        }
        else
        {
            overallStatus=ECAFE_INVALID_HANDLE;
            isGood=false;
        }
    }
    return overallStatus;
}


//Rearrange

int CAFE::getChannelDataStore(unsigned int handle, ChannelDataStore & cds)
{
    HandleHelper hh;
    ChannelRegalia chin;
    PVCtrlHolder pvc ;
    PVDataHolder pvd(CAFE::getNelemRequest(handle));
    std::string desc;
    int status=CAFE::getAllChannelInfo(handle, chin, pvc, pvd, desc);
    cds.description=desc;
    cds.info=chin;
    cds.pvd= pvd;
    cds.pvc= pvc;

    return status;
}


int CAFE::getAllChannelInfo(unsigned int handle, ChannelRegalia & channelInfo, PVCtrlHolder & pvc, PVDataHolder &pvd, std::string & desc)
{

    std::string pvDesc="";
    HandleHelper hh;
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);
    if (it_handle != handle_index.end())
    {
        pvDesc.append((*it_handle).getPV());
        channelInfo=(*it_handle).getChannelRegalia();
        (*it_handle).getPVCtrlHolder(pvc);
    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    int status=ICAFE_NORMAL;

    //Check for .VAL and remove
    std::size_t found = pvDesc.find(".");
    if (found != std::string::npos)
    {
        pvDesc=pvDesc.substr(0, found);
    }

    pvDesc.append(".DESC");

    //std::cout << pvDesc << std::endl;
    // But does it already have a description??
    unsigned int h=hh.getHandleFromPV((const char *)pvDesc.c_str());
    bool openDescFlag=false;
    if (h>0)
    {
        if (Connect::isConnected(h))
        {
            openDescFlag=true;
            status= CAFE::get(h, desc);
        }
    }

    if(openDescFlag==false)
    {
        Connect::open(pvDesc, h);
        status=CAFE::get(h, desc);
        Connect::close(h);
    }

    unsigned int nelemPrevious=hh.getNelemToRetrieveFromCache(handle);

    //if (CAFE::getNoMonitors(handle) >0 ) {
    status= CAFE::getCache(handle, pvd);
    //}

    hh.setNelemToRetrieveFromCache(handle, nelemPrevious);

    return status;
}




/**
 *  \brief Supplment all handles with Severity levels and Desc information.
 *  If PV has a special field (DESC, HHSV HSV etc..) then do not supplement
 *  \return ICAFE_NORMAL
 */
int CAFE::supplementHandles()
{
#define __METHOD__ "CAFE::supplementHandles()"

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    std::vector<unsigned int> activeSpecialFieldsSizeV;
    std::vector<unsigned int> handleAsGivenV;
    std::vector<std::string> pvV;
    std::vector<std::string> pvAsGivenV;
    std::vector<std::string>  pvsToClose;
    std::vector<unsigned int> allHandles;
    std::vector<std::string>  allPVs;

    std::size_t found;
    std::size_t foundField;
    std::string field="";
    std::string pv="";
    std::string pvDevAttrib="";

    std::vector<std::string> specialFields;
    specialFields.reserve(5);
    specialFields.push_back( (char *) "DESC");
    specialFields.push_back( (char *) "HHSV");
    specialFields.push_back( (char *) "HSV");
    specialFields.push_back( (char *) "LSV");
    specialFields.push_back( (char *) "LLSV");

    unsigned int activeSpecialFieldsSize=specialFields.size();
    std::vector<std::string> pvContainer;
    pvContainer.reserve(5);

    unsigned int localHandle=0;
    int status=ICAFE_NORMAL;
    int overallStatus=ICAFE_NORMAL;


    // Loop through all elements and get PVNames
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        activeSpecialFieldsSize=specialFields.size();

        //ENUM type does not have alarmr limits - hence get desc only!
        PVCtrlHolder  _pvc;
        int _stat = (*itcs).getPVCtrlHolder(_pvc) ;
        //std::cout << (*itcs).getPV() << " has status " << _stat << " and pvc.status " <<  _pvc.getStatus() << std::endl;
        //std::cout <<  "is Connected? " << (*itcs).isConnected() << std::endl;

        if (_stat==ICAFE_NORMAL && _pvc.getStatus()!=ICAFE_NORMAL)
        {
            ChannelRequestStatus cre;
            handleHelper.getChannelRequestStatusGet ((*itcs).getHandle(), cre);
            unsigned int nPoll=0;

            while (cre.getCallbackProgressKind()==(CAFENUM::CallbackProgressKind) CAFENUM::PENDING && nPoll<20)
            {

#if HAVE_BOOST_THREAD
                boost::this_thread::sleep_for(boost::chrono::microseconds(2000));
#else
#if HAVE_LINUX
                usleep(2000);
#endif
#endif

                ++nPoll;
                handleHelper.getChannelRequestStatusGet ((*itcs).getHandle(), cre);
            }
            std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            std::cout << __METHOD__ << " nPoll=" << nPoll <<std::endl;

            if ((*itcs).isConnected() && cre.getCallbackProgressKind()==(CAFENUM::CallbackProgressKind) CAFENUM::PENDING)
            {
                std::cout << __FILE__ << ":" << __LINE__ << std::endl;
                std::cout << __METHOD__ << std::endl;
                std::cout << handleHelper.getPVFromHandle((*itcs).handle) << " with handle " << (*itcs).getHandle() << std::endl;
                std::cout << "Value of 1 is pending, Value of 2 is complete" << std::endl;
                std::cout << "Callback PENDING for GET?:         " << cre.getCallbackProgressKind()  << std::endl;
            }
        }

        _stat = (*itcs).getPVCtrlHolder(_pvc) ;

        //:pvc.getStatus()  can also be CHANNEL CONNECTION: ICAFE_CA_OP_CONN_UP - THis occurs when not all callbacks have completed
        if ( (_stat==ICAFE_NORMAL || _stat==ICAFE_CS_CLOSED) && 
	     (_pvc.getStatus()==ICAFE_NORMAL || _pvc.getStatus()==ICAFE_CS_CLOSED))
        {

            //std::cout <<  "No of enum strings " << _pvc.getNoEnumStrings() << std::endl;
            if (_pvc.getNoEnumStrings()>0 )
            {
                activeSpecialFieldsSize=1;
            }
        }
        else
        {
            std::cout << "//-----------------------------------------------------------------------------------------------//" << std::endl;
            std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(_pvc.getStatus());
            std::cout << "Handle=" << (*itcs).getHandle() << " pv=" << (*itcs).getPV() << " error retrieving PVCtrlHolder " << std::endl;
            std::cout << "Handle ***not*** supplemented!" << std::endl;
            std::cout << "//-----------------------------------------------------------------------------------------------//" <<std::endl<<std::endl;
            continue;
        }


        //New ... July 24, 2019 - Activate DESC Only!!!
        activeSpecialFieldsSize=1;


        pv=(*itcs).getPV();
        pvDevAttrib=pv;

        field="";

        found = pv.find(".");

        if (found != std::string::npos)
        {
            field=pv.substr(found, pv.size()-1);

            //DO NOT SUPPLEMENT CONDUIT OBJECT FOR PVS WITH SPECIAL FIELDS
            bool breakFlag=false;

            for (unsigned int j=0; j<activeSpecialFieldsSize; ++j)
            {
                foundField=field.find(specialFields[j]);
                if (foundField != std::string::npos)
                {
                    breakFlag=true;
                    break;
                }
            }

            std::cout << "field = " << field << std::endl;
            std::cout << "pv.substr = " << pv.substr(0,found) << std::endl;
            if (breakFlag)
            {
                std::cout << "This pv is not supplemented = " << pv << std::endl;
                continue;
            }

            if  ( (*itcs).hasDescription() && (*itcs).hasAlarmSeverityStruct() )
            {
                std::cout << "This pv has already been supplemtented = " << pv << std::endl;
                continue;
            }
            else if  ( (*itcs).hasDescription() && activeSpecialFieldsSize==1 )
            {
                std::cout << "This pv has already been supplemented = " << pv << std::endl;
                continue;
            }

            pvDevAttrib=pv.substr(0,found);
            pvV.push_back(pvDevAttrib);

        }
        else
        {
            pvV.push_back(pv);
        }

        activeSpecialFieldsSizeV.push_back(activeSpecialFieldsSize);
        pvAsGivenV.push_back(pv);
        handleAsGivenV.push_back((*itcs).getHandle());
        pvContainer.clear();

        for (unsigned int j=0; j<activeSpecialFieldsSize; ++j)
        {
            std::string pvS=pvDevAttrib;
            pvS.append((char *)".");
            pvS.append(specialFields[j]);
            pvContainer.push_back(pvS);
        }
        //Check if already open -if yes do not add to pvHandlestoOpenAndClose
        for (unsigned int i=0; i<pvContainer.size(); ++i)
        {
            localHandle= handleHelper.getHandleFromPV(pvContainer[i].c_str());
            if (localHandle == 0 )
            {
                pvsToClose.push_back(pvContainer[i]);
                allPVs.push_back(pvContainer[i]);
            }
            else
            {
                //pvHandlesAlreadyExisiting.insert(pvContainer[i]);
                allPVs.push_back(pvContainer[i]);
            }
        }
    } //for

    bool _pyCafeFlag=false;

    if (CAFE::getPyCafe() )
    {
        CAFE::setPyCafe(false);
        _pyCafeFlag=true;
    }


    CAFE::openPrepare();
    //std::cout << allPVs.size() << std::endl;
    try
    {
        CAFE::open(allPVs, allHandles);
    }
    catch(CAFEException_open &e)
    {
        CAFE::openNowAndWait(2.0);
        if (_pyCafeFlag)
        {
            CAFE::setPyCafe(true);
        }
        return e.pvEx.statusCode;
    }
    // Already handles in openNowAndWaitHandleV (!!!)
    CAFE::openNowAndWait(2.0);
    //std::cout << allHandles.size() << std::endl;
    //Get Asyn No Blocking
    std::vector<int> allStatus;
    std::vector<int> vRB;
    vRB.reserve(allHandles.size());
    overallStatus=CAFE::get(allHandles, allStatus);

    //for (unsigned int i=0; i<allHandles.size(); ++i) {
    //   std::cout << allPVs[i] << " " << allHandles[i] << std::endl;
    //}

    status=CAFE::waitForBundledEvents(allHandles, vRB);

    if (status != ICAFE_NORMAL)
    {
        overallStatus=status;
    }

    std::vector<int> localStatus;
    localStatus.reserve(5);

    for (unsigned int i=0; i<pvV.size(); ++i)
    {
        localStatus.clear();
        pvContainer.clear();
        //Recalculate activeSpecialFieldsSize

        for (unsigned int j=0; j<activeSpecialFieldsSizeV[i]; ++j)
        {
            std::string pvS=pvV[i];
            pvS.append((char *)".");
            pvS.append(specialFields[j]);
            pvContainer.push_back(pvS);
        }

        alarmSeverityStruct ass;
        std::string desc="";
        status=CAFE::getCache(pvContainer[0].c_str(), desc);
        localStatus.push_back(status);
        if (status != ICAFE_NORMAL)
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            std::cout << "pv=" << pvContainer[0] << " reports error: " << std::endl;
            cafeStatus.report(status);
            std::cout << std::endl;
            ///continue;
        }
        //std::cout << i  << " " << pvContainer[0] << " " << desc << std::endl;
        if (pvContainer.size() > 4)  // integer value of enum!
        {
            status=CAFE::getCache(pvContainer[1].c_str(), ass.hhsv);
            localStatus.push_back(status);
            status=CAFE::getCache(pvContainer[2].c_str(), ass.hsv);
            localStatus.push_back(status);
            status=CAFE::getCache(pvContainer[3].c_str(), ass.lsv);
            localStatus.push_back(status);
            status=CAFE::getCache(pvContainer[4].c_str(), ass.llsv);
            localStatus.push_back(status);

            //std::cout << i  << " " << pvContainer[1] << " " << ass.hhsv << " status= " << localStatus[1] << std::endl;
            //std::cout << i  << " " << pvContainer[2] << " " << ass.hsv  << " status= " << localStatus[2] << std::endl;
            //std::cout << i  << " " << pvContainer[3] << " " << ass.lsv  << " status= " << localStatus[3] << std::endl;
            //std::cout << i  << " " << pvContainer[4] << " " << ass.llsv << " status= " << localStatus[4] << std::endl;
        }

        bool dataIsGoodDesc=false;
        bool dataIsGood=false;
        bool dataIsGoodAlarmSeverity=false;

        //std::cout << "ls[0] and size  " << localStatus[0] <<  " "  << localStatus.size() << std::endl;

        if (localStatus[0] == ICAFE_NORMAL)
        {
            dataIsGoodDesc=true;
        }
        if (localStatus.size() > 4)
        {
            dataIsGoodAlarmSeverity=true;
            for (int i=1; i<localStatus.size(); ++i)
            {
                if (localStatus[i] != ICAFE_NORMAL)
                {
                    dataIsGoodAlarmSeverity=false;
                    break;
                }
            }
        }

        if (dataIsGoodDesc && dataIsGoodAlarmSeverity)
        {
            dataIsGood=true;
        }
        // std::cout << "data is good " <<  dataIsGood << std::endl;
        //std::cout << "data is good Desc " <<  dataIsGoodDesc << std::endl;
        //std::cout << "data is good Alarm" <<  dataIsGoodAlarmSeverity << std::endl;

        it_handle = handle_index.find(handleAsGivenV[i]); //Following does not cater for handles within a group: handleHelper.getHandleFromPV(pvAsGivenV[i].c_str()));
        if (it_handle != handle_index.end())
        {
            if(MUTEX)
            {
                cafeMutex.lock();
            };
            if (dataIsGood)
            {
                handle_index.modify(it_handle, change_supplementHandle(ass,desc));
                //std::cout << "handle_index.modify(it_handle, change_supplementHandle(ass,desc));" << std::endl;
            }
            else if (dataIsGoodDesc)
            {
                handle_index.modify(it_handle, change_supplementDescription(desc)); //enum type desc only
                //std::cout << "handle_index.modify(it_handle, change_supplementHandle(desc));" << std::endl;
            }
            else if (dataIsGoodAlarmSeverity)
            {
                handle_index.modify(it_handle, change_supplementAlarmSeverity(ass)); //enum type desc only
                //std::cout << "handle_index.modify(it_handle, change_supplementHandle(ass));" << std::endl;
            }
            if(MUTEX)
            {
                cafeMutex.unlock();
            }
        }
        else
        {
            std::cout << __METHOD__ << " INVALID HANDLE for " << pvAsGivenV[i].c_str() <<  std::endl;
        }
    }


    channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
    channelOpenPolicy.setFlushSendBufferKind(WITH_FLUSH_IO);

    for (unsigned int i=0; i < pvsToClose.size(); ++i)
    {
        //std::cout << "CLOSING " << pvsToClose[i] << " handle " <<   handleHelper.getHandleFromPV(pvsToClose[i].c_str()) << std::endl;
        CAFE::close(handleHelper.getHandleFromPV(pvsToClose[i].c_str()));
    }


    channelOpenPolicy.flushSendBufferNow();
    //reset
    channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_NOW);
    channelOpenPolicy.setFlushSendBufferKind(WITH_POLL);

    //CAFE::printHandles();
    //for (unsigned int i=0; i<pvAsGivenV.size(); ++i) {
    //    std::string desc="";
    //    handleHelper.getDescription( handleHelper.getHandleFromPV(pvAsGivenV[i].c_str()), desc);
    //    std::cout << i << " DESC=" << desc << " for pv= " << pvAsGivenV[i].c_str() << std::endl;
    //}

    if (_pyCafeFlag)
    {
        CAFE::setPyCafe(true);
    }


    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief Supplment all handles with Severity levels and Desc information.
 *  If PV has a special field (DESC, HHSV HSV etc..) then do not supplement
 *  \return ICAFE_NORMAL
 */
int CAFE::supplementHandlesV(std::vector<unsigned int> hV)
{
#define __METHOD__ "CAFE::supplementHandlesV(std::vector<unsigned int>)"

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    std::vector<unsigned int> activeSpecialFieldsSizeV;
    std::vector<unsigned int> handleAsGivenV;
    std::vector<std::string> pvV;
    std::vector<std::string> pvAsGivenV;
    std::vector<std::string>  pvsToClose;
    std::vector<unsigned int> allHandles;
    std::vector<std::string>  allPVs;

    std::size_t found;
    std::size_t foundField;
    std::string field="";
    std::string pv="";
    std::string pvDevAttrib="";

    std::vector<std::string> specialFields;
    specialFields.reserve(5);
    specialFields.push_back( (char *) "DESC");
    specialFields.push_back( (char *) "HHSV");
    specialFields.push_back( (char *) "HSV");
    specialFields.push_back( (char *) "LSV");
    specialFields.push_back( (char *) "LLSV");

    unsigned int activeSpecialFieldsSize=specialFields.size();

    std::vector<std::string> pvContainer;
    pvContainer.reserve(5);

    //Only these have alarm/severity
    std::vector<std::string>alarmSeverityRecordTypes;
    alarmSeverityRecordTypes.reserve(10);
    alarmSeverityRecordTypes.push_back( (char *) "ai");
    alarmSeverityRecordTypes.push_back( (char *) "ao");
    alarmSeverityRecordTypes.push_back( (char *) "calc");
    alarmSeverityRecordTypes.push_back( (char *) "dfanout");
    alarmSeverityRecordTypes.push_back( (char *) "longin");
    alarmSeverityRecordTypes.push_back( (char *) "longout");
    alarmSeverityRecordTypes.push_back( (char *) "pid");
    alarmSeverityRecordTypes.push_back( (char *) "sel");
    alarmSeverityRecordTypes.push_back( (char *) "steppermotor");
    alarmSeverityRecordTypes.push_back( (char *) "sub");

    std::vector<std::string> specialBinaryFields;
    specialBinaryFields.reserve(4);
    specialBinaryFields.push_back( (char *) "DESC");
    specialBinaryFields.push_back( (char *) "ZSV");
    specialBinaryFields.push_back( (char *) "OSV");
    specialBinaryFields.push_back( (char *) "COSV");

    std::vector<std::string> specialMultiBitBinaryFields;
    specialMultiBitBinaryFields.reserve(19);
    specialMultiBitBinaryFields.push_back( (char *) "DESC");
    specialMultiBitBinaryFields.push_back( (char *) "ZRSV");
    specialMultiBitBinaryFields.push_back( (char *) "ONSV");
    specialMultiBitBinaryFields.push_back( (char *) "TWSV");
    specialMultiBitBinaryFields.push_back( (char *) "THSV");
    specialMultiBitBinaryFields.push_back( (char *) "FRSV");
    specialMultiBitBinaryFields.push_back( (char *) "FVSV");
    specialMultiBitBinaryFields.push_back( (char *) "SXSV");
    specialMultiBitBinaryFields.push_back( (char *) "SVSV");
    specialMultiBitBinaryFields.push_back( (char *) "TWSV");
    specialMultiBitBinaryFields.push_back( (char *) "EISV");
    specialMultiBitBinaryFields.push_back( (char *) "NISV");
    specialMultiBitBinaryFields.push_back( (char *) "TESV");
    specialMultiBitBinaryFields.push_back( (char *) "ELSV");
    specialMultiBitBinaryFields.push_back( (char *) "TVSV");
    specialMultiBitBinaryFields.push_back( (char *) "TTSV");
    specialMultiBitBinaryFields.push_back( (char *) "FTSV");
    specialMultiBitBinaryFields.push_back( (char *) "FFSV");
    specialMultiBitBinaryFields.push_back( (char *) "UNSV");
    specialMultiBitBinaryFields.push_back( (char *) "COSV");


    std::vector<std::string>alarmSeverityBinaryTypes;
    alarmSeverityBinaryTypes.reserve(4);
    alarmSeverityBinaryTypes.push_back( (char *) "bi");
    alarmSeverityBinaryTypes.push_back( (char *) "bo");
    alarmSeverityBinaryTypes.push_back( (char *) "mbbi");
    alarmSeverityBinaryTypes.push_back( (char *) "mbbo");


    unsigned int localHandle=0;
    int status=ICAFE_NORMAL;
    int overallStatus=ICAFE_NORMAL;

    //Special records have special fields.. mbbi and bi for instance(!!!)

    for (unsigned int i=0; i<hV.size(); ++i)
    {
        it_handle = handle_index.find(hV[i]);

        if (it_handle != handle_index.end())
        {

            activeSpecialFieldsSize=specialFields.size();

            //ENUM type does not have alarmr limits - hence get desc only!
            PVCtrlHolder  _pvc;
            int _stat = (*it_handle).getPVCtrlHolder(_pvc);

            //std::cout << (*it_handle).getPV()  << " CLASSNAME " << (std::string) (*it_handle).getClassName() << std::endl;
            //_pvc.print();
            //std::cout << _stat << " < ERROR CODES > " << _pvc.getStatus() << std::endl;
            //:pvc.getStatus()  can also be CHANNEL CONNECTION: ICAFE_CA_OP_CONN_UP - THis occurs when not all callbacks have completed
            if ( (_stat==ICAFE_NORMAL || _stat==ICAFE_CS_CLOSED) && 
		 (_pvc.getStatus()==ICAFE_NORMAL || _pvc.getStatus() ==ICAFE_CS_CLOSED) )
            {
                //std::cout << "No of enum strings" << _pvc.getNoEnumStrings() << std::endl;
                if (_pvc.getNoEnumStrings()>0 )
                {
                    activeSpecialFieldsSize=1;
                }
            }
            else 
            {
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cafeStatus.report(_stat);
                std::cout << "Handle=" << (*it_handle).getHandle() << " pv=" << (*it_handle).getPV() << " error retrieving PVCtrlHolder " << std::endl;
            }

            //New ... July 24, 2019 - Activate DESC Only!!!
            activeSpecialFieldsSize=1;

            pv=(*it_handle).getPV();
            pvDevAttrib=pv;

            field="";

            found = pv.find(".");

            if (found != std::string::npos)
            {
                field=pv.substr(found, pv.size()-1);

                //DO NOT SUPPLEMENT CONDUIT OBJECT FOR PVS WITH SPECIAL FIELDS
                bool breakFlag=false;

                for (unsigned int j=0; j<activeSpecialFieldsSize; ++j)
                {
                    foundField=field.find(specialFields[j]);
                    if (foundField != std::string::npos)
                    {
                        breakFlag=true;
                        break;
                    }
                }

                //std::cout << "field = " << field << std::endl;
                //std::cout << "pv.substr = " << pv.substr(0,found) << std::endl;
                if (breakFlag)
                {
                    std::cout << "This pv is not supplemented = " << pv << std::endl;
                    continue;
                }

                if  ( (*it_handle).hasDescription() && (*it_handle).hasAlarmSeverityStruct() )
                {
                    std::cout << "This pv has already been supplemented = " << pv << std::endl;
                    continue;
                }
                else if  ( (*it_handle).hasDescription() && activeSpecialFieldsSize==1 )
                {
                    std::cout << "This pv has already been supplemented = " << pv << std::endl;
                    continue;
                }

                pvDevAttrib=pv.substr(0,found);
                pvV.push_back(pvDevAttrib);
            }
            else
            {
                pvV.push_back(pv);
            }

            //std::cout << pv << " activce special size " << activeSpecialFieldsSize << std::endl;
            activeSpecialFieldsSizeV.push_back(activeSpecialFieldsSize);
            pvAsGivenV.push_back(pv);
            handleAsGivenV.push_back((*it_handle).getHandle());
            pvContainer.clear();

            for (unsigned int j=0; j<activeSpecialFieldsSize; ++j)
            {
                std::string pvS=pvDevAttrib;
                pvS.append((char *)".");
                pvS.append(specialFields[j]);
                pvContainer.push_back(pvS);
            }
            //Check if already open -if yes do not add to pvHandlestoOpenAndClose
            for (unsigned int k=0; k<pvContainer.size(); ++k)
            {
                localHandle= handleHelper.getHandleFromPV(pvContainer[k].c_str());
                if (localHandle == 0 )
                {
                    pvsToClose.push_back(pvContainer[k]);
                    allPVs.push_back(pvContainer[k]);
                }
                else
                {
                    //pvHandlesAlreadyExisiting.insert(pvContainer[k]);
                    allPVs.push_back(pvContainer[k]);
                }
            }
        }
        else
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << hV[i] << " either never existed or no longer exists " << endl;
            overallStatus=ECAFE_INVALID_HANDLE;
        }

    }//for



    bool _pyCafeFlag=false;

    if (CAFE::getPyCafe() )
    {
        CAFE::setPyCafe(false);
        _pyCafeFlag=true;
    }
   
    CAFE::openPrepare();
    try
    {
        CAFE::open(allPVs, allHandles);
    }
    catch(CAFEException_open &e)
    {
        CAFE::openNowAndWaitHandleV(allHandles, channelOpenPolicy.getTimeout());
        if (_pyCafeFlag)
        {
            CAFE::setPyCafe(true);
        }
        return e.pvEx.statusCode;
    }

    CAFE::openNowAndWaitHandleV(allHandles, channelOpenPolicy.getTimeout());

    for (int ij=0; ij<20; ++ij) {
	    if (!initCallbackComplete(allHandles) ) {
	      //cout << "Will wait for " << ij << " " << (1+ij)*0.1 << " ======= SECONDS ==== " << endl;
	      usleep(100000); //tenth of a sec and loop over 2 sec
	    }
     }
   
    //isCallbackComplete?

    if (_pyCafeFlag)
    {
        CAFE::setPyCafe(true);
    }
    
    //Get Asyn No Blocking
    std::vector<int> allStatus;
    std::vector<int> vRB;
    vRB.reserve(allHandles.size());
    overallStatus=CAFE::get(allHandles, allStatus);
   
    

    status=CAFE::waitForBundledEvents(allHandles, vRB);

   
    
    //ca_pend_io(1.1);

    if (status != ICAFE_NORMAL)
    {
        overallStatus=status;
    }

    std::vector<int> localStatus;
    localStatus.reserve(5);
    for (unsigned int i=0; i<pvV.size(); ++i)
    {
        pvContainer.clear();
        localStatus.clear();
        for (unsigned int j=0; j<activeSpecialFieldsSizeV[i]; ++j)
        {
            std::string pvS=pvV[i];
            pvS.append((char *)".");
            pvS.append(specialFields[j]);
            pvContainer.push_back(pvS);
        }
        alarmSeverityStruct ass;
        std::string desc="";
        
        status=CAFE::getCache(pvContainer[0].c_str(), desc);
       
        localStatus.push_back(status);

        if (pvContainer.size() > 4)
        {
            status=CAFE::getCache(pvContainer[1].c_str(), ass.hhsv);
            localStatus.push_back(status);
            status=CAFE::getCache(pvContainer[2].c_str(), ass.hsv);
            localStatus.push_back(status);
            status=CAFE::getCache(pvContainer[3].c_str(), ass.lsv);
            localStatus.push_back(status);
            status=CAFE::getCache(pvContainer[4].c_str(), ass.llsv);
            localStatus.push_back(status);

            //std::cout << i  << " " << pvContainer[1] << " " << ass.hhsv << " status= " << localStatus[1] << std::endl;
            //std::cout << i  << " " << pvContainer[2] << " " << ass.hsv  << " status= " << localStatus[2] << std::endl;
            //std::cout << i  << " " << pvContainer[3] << " " << ass.lsv  << " status= " << localStatus[3] << std::endl;
            //std::cout << i  << " " << pvContainer[4] << " " << ass.llsv << " status= " << localStatus[4] << std::endl;
        }

        bool dataIsGoodDesc=false;
        bool dataIsGood=false;
        bool dataIsGoodAlarmSeverity=false;

        if (localStatus[0] == ICAFE_NORMAL)
        {
            dataIsGoodDesc=true;
        }
        if (localStatus.size() > 4)
        {
            dataIsGoodAlarmSeverity=true;
            for (int i=1; i<localStatus.size(); ++i)
            {
                if (localStatus[i] != ICAFE_NORMAL)
                {
                    dataIsGoodAlarmSeverity=false;
                    break;
                }
            }
        }
        if (dataIsGoodDesc && dataIsGoodAlarmSeverity)
        {
            dataIsGood=true;
        }

        it_handle = handle_index.find(handleAsGivenV[i]); //Following does not cater for handles within a group: handleHelper.getHandleFromPV(pvAsGivenV[i].c_str()));
        if (it_handle != handle_index.end())
        {
            if(MUTEX)
            {
                cafeMutex.lock();
            };
            if (dataIsGood)
            {
                handle_index.modify(it_handle, change_supplementHandle(ass,desc));
            }
            else if (dataIsGoodDesc)
            {
                handle_index.modify(it_handle, change_supplementDescription(desc)); //enum type desc only
            }
            else if (dataIsGoodAlarmSeverity)
            {
                handle_index.modify(it_handle, change_supplementAlarmSeverity(ass)); //enum type desc only
            }
            if(MUTEX)
            {
                cafeMutex.unlock();
            };
        }
        else
        {
            std::cout << __METHOD__ << " INVALID HANDLE for " << pvAsGivenV[i].c_str() <<  std::endl;
        }
    }


    channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
    channelOpenPolicy.setFlushSendBufferKind(WITH_FLUSH_IO);

    for (unsigned int i=0; i < pvsToClose.size(); ++i)
    {
        //std::cout << "CLOSING " << pvsToClose[i] << " handle " <<   handleHelper.getHandleFromPV(pvsToClose[i].c_str()) << std::endl;
        CAFE::close(handleHelper.getHandleFromPV(pvsToClose[i].c_str()));
    }

    

    channelOpenPolicy.flushSendBufferNow();
    //reset
    channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_NOW);
    channelOpenPolicy.setFlushSendBufferKind(WITH_POLL);

    //CAFE::printHandles();
    //for (unsigned int i=0; i<pvAsGivenV.size(); ++i) {
    //    std::string desc="";
    //    handleHelper.getDescription( handleHelper.getHandleFromPV(pvAsGivenV[i].c_str()), desc);
    //    std::cout << i << " DESC=" << desc << " for pv= " << pvAsGivenV[i].c_str() << std::endl;
    //}

    



    return ICAFE_NORMAL;
#undef __METHOD__
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if HAVE_PYTHON_H
/**
 *  loadSFGroups is a SwissFEL specific method (M. Aiba)
 *  \param suff input: suffix for Virtual Accelerator
 *  \return void
 */
void CAFE::loadSFGroups(std::string suff)
{

    std::cout << " ---> reporting from: void CAFE::loadSFGroups(string suff) <--- " << std::endl;

    //Test for file directory
    string fName="";
    char * p= NULL;
    char const* d = "OMC.py";
    bool isFile=false;
    ifstream ifile;

    if(std::getenv("SFEL_LAYOUT_PYTHONPATH") == NULL)
    {
        std::cout << "SFEL_LAYOUT_PYTHONPATH is not defined, thus may be missing from PYTHONPATH" << std::endl;
    }

    if(std::getenv("SFEL_OMC_PYTHONPATH") != NULL)
    {
        p = std::getenv("SFEL_OMC_PYTHONPATH");
    }

    if (p != NULL)
    {
        fName.append(p);
        fName.append("/");
        fName.append(d);

        ifile.open(fName.c_str(),ios::in);
        if (ifile.good())
        {
            ifile.close();
            isFile=true;
        }
    }

    if (!isFile)
    {
        fName="";
        fName.append("/afs/psi.ch/intranet/SF/Applications/on-line_model/default/PythonModule") ;

        std::cout << "Did not find file in $SFEL_OMC_PYTHONPATH " << std::endl;
        std::cout << "therefore using the following default directory:  " << std::endl;
        std::cout << fName.c_str() << std::endl;

        fName.append("/");
        fName.append(d);

        ifile.open(fName.c_str(),ios::in);
        if (ifile.good())
        {
            ifile.close();
            isFile=true;
        }
        else
        {
            std::cout << "Error opening default file: " << fName.c_str() << std::endl;
        }
    }

    // No luck with finding OMC.py
    if (!isFile)
    {
        return;
    }
 
    if (! Py_IsInitialized () )
    {
        std::cout << "INITIALIZING PY IN CAFE" << std::endl;
        Py_Initialize();
    }

    globalChannelList.clear();
    globalChannelList.reserve(9000);

    PyObject* psys = PyImport_ImportModule( "sys" );
    if (psys==NULL)
    {
        std::cout << "PSYS IS NULL " << std::endl;
    }

    PyObject* psys_path = PyObject_GetAttrString( psys, "path" );
    if (psys_path==NULL)
    {
        std::cout << "PSYS PATH IS NULL " << std::endl;
    }

    PyObject* folder_path = PyUnicode_FromString(fName.c_str());
    //"/afs/psi.ch/intranet/SF/Applications/Development/OnlineModel/PythonModule" );
    if (folder_path==NULL)
    {
        std::cout << "FOLDER PATH IS NULL " << std::endl;
    }
    PyList_Append( psys_path, folder_path );
    //Py_DECREF(folder_path);


    // Load module, class and object
    PyObject *mymod, *pclass, *pobject;
    //cout <<  fName.c_str() << endl;
    //cout << " PY VERSION " << PY_MAJOR_VERSION << endl;

    mymod = PyImport_ImportModule("OMC");
    if (mymod==NULL)
    {
        std::cout << " MYMOD IS NULL. Printing Error: " << std::endl;
        PyErr_Print();
        return;
    }

    pclass = PyObject_GetAttrString(mymod, "OMC");
    if (pclass==NULL)
    {
        std::cout << " pClass IS NULL " << std::endl;
        PyErr_Print() ;
        return;
    }

    PyObject *pmethod, *pargs, *pret;
    pargs=NULL;

    //pobject = PyEval_CallObject(pclass,pargs);
    pobject = PyObject_CallObject(pclass,pargs);
    if (pobject==NULL)
    {
        std::cout << " pobject IS NULL " << std::endl;
        PyErr_Print() ;
        return;
    }

    pmethod = PyObject_GetAttrString(pobject, "initFunction");
    if (pmethod==NULL)
    {
        std::cout << " pobject IS NULL " << std::endl;
        PyErr_Print() ;
        return;
    }

    //Required for Python3
    pargs=Py_BuildValue("(s)", suff.c_str());
    //pargs=Py_BuildValue("s",(suff.c_str()));
    if (pargs==NULL)
    {
        std::cout << " pargs IS NULL " << std::endl;
        PyErr_Print() ;
    }

    //pret=PyObject_CallFunction(pmethod, "s", suff.c_str());
    pret=PyObject_CallObject(pmethod, pargs);
    if (pret==NULL)
    {
        std::cout << " pret IS NULL  " << std::endl;
        PyErr_Print() ;
        return;
    }

    string GroupName;
    vector<string> ChannelList;
    PyObject *plist1, *plist2, *plist3, *plist4;
    int i,j,li;
    long lj;

    //cout << "PyArg_ParseTuple(pret, 'iOOO', &li,&plist1, &plist2, &plist3);" << endl;
    if (!PyArg_ParseTuple(pret, "iOOO", &li,&plist1, &plist2, &plist3))
    {
        std::cout << "is NULL " << std::endl;
        return;
    }


    PyObject *ps, *pyi;
    string mystr;

    //Check type of pList1 etc...
    if (plist1 == NULL )
    {
        std::cout << "pList1 is NULL" << std::endl;
    }
    if (plist2 == NULL )
    {
        std::cout << "pList2 is NULL" << std::endl;
    }
    if (plist3 == NULL )
    {
        std::cout << "pList3 is NULL" << std::endl;
    }

#if HAVE_LIBQTXML
    CAFE::openGroupXMLFile("/tmp/test.xml");
#endif

    for (i=0; i<li; i++)
    {
        ps=PyList_GetItem(plist2,i);
        if (ps == NULL)
        {
            std::cout << " ps is NULL; indx in loop=" << i << std::endl;
            PyErr_Print();
        }

#if PY_MAJOR_VERSION >= 3
        GroupName=PyUnicode_AsUTF8(ps);
#elif PY_MAJOR_VERSION < 3
        GroupName=PyString_AsString(ps);
#endif

        plist4=PyList_GetItem(plist3,i); // Corresponding channel list

        pyi=PyList_GetItem(plist1,i);
        lj=PyLong_AsLong(pyi);
        ChannelList.clear();

        for (j=0; j<lj; j++)
        {
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

        status=CAFE::groupDefine(GroupName.c_str(), ChannelList);
        if (status != ICAFE_NORMAL)
        {
            CAFE::printStatusMessage(status);
        }
#if HAVE_LIBQTXML
        CAFE::group2XML(GroupName.c_str(), "/tmp/test.xml");
#endif
    }//for

#if HAVE_LIBQTXML
    CAFE::closeGroupXMLFile("/tmp/test.xml");
#endif


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
    if (pargs !=NULL)
    {
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



