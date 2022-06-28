///
/// \file    conduit.cpp
/// \author  Jan Chrin, PSI
/// \date    Release: September 2015
/// \version CAFE 1.1.0
///

#include <conduit.h>
#include <helper.h>
//include <boost/date_time/posix_time/posix_time.hpp>

unsigned int  Conduit::handleNext=0; //4294967295;
unsigned int  MonitorPolicy::idNext=0xfff; //4095

using namespace std;

Conduit::Conduit( ) {};

/**
 *   CAFEConduit destructor \n
 *   Good place to clean up!
 */
Conduit::~Conduit() {};

/*
Conduit::Conduit(const char * _pv, ca_client_context *_ccc,  bool _pyCafeFlag) {

    //Define default policies
    channelRequestPolicyPut.setPolicy(_channelRequestPolicy.getWhenToFlushSendBuffer(),
					_channelRequestPolicy.getWaitKind(), _channelRequestPolicy.getMethodKind());

     cout << "when " << _channelRequestPolicy.getWhenToFlushSendBuffer() << endl;
		 cout << "wait " << _channelRequestPolicy.getWaitKind() << endl;
		 cout << "method " << _channelRequestPolicy.getMethodKind() << endl;


     Conduit::Conduit(_pv, _ccc,  _pyCafeFlag);
     cout << "when " << _channelRequestPolicy.getWhenToFlushSendBuffer() << endl;
		 cout << "wait " << _channelRequestPolicy.getWaitKind() << endl;
		 cout << "method " << _channelRequestPolicy.getMethodKind() << endl;


}
*/


/**
 *   \brief Conduit constructor with flag for PyCafe API \n
 *   \param _pv      input:      	process variable
 *   \param _ccc     input:     	ca_client_context
 *   \param _pyCafeFlag input: bool
 *   \param _channelRequestPolicyPut input: ChannelRequestPolicyPut
 *   \param _channelRequestPolicyGet input: ChannelRequestPolicyGet
 *   \param _channelGetActionWhenMonitorPolicy input: ChannelGetActionWhenPolicy
 */
Conduit::Conduit(const char * _pv, ca_client_context *_ccc,
                 ChannelRequestPolicy _channelRequestPolicyPut, ChannelRequestPolicy _channelRequestPolicyGet,
                 ChannelGetActionWhenMonitorPolicy _channelGetActionWhenMonitorPolicy,
                 bool _pyCafeFlag)
{    
    // To avoid the following compilation error
    // conduit.cpp:41: warning: extended initializer lists only available with -std=c++0x or -std=gnu++0x
    //epicsAlarmSeverityStrings= {"NO_ALARM","MINOR","MAJOR","INVALID"};
    epicsAlarmSeverityStrings[0]=(const char *) "NO_ALARM";
    epicsAlarmSeverityStrings[1]=(const char *) "MINOR";
    epicsAlarmSeverityStrings[2]=(const char *) "MAJOR";
    epicsAlarmSeverityStrings[3]=(const char *) "INVALID";
    //epicsAlarmConditionStrings = {"NO_ALARM","READ","WRITE","HIHI","HIGH",
    //"LOLO","LOW","STATE","COS", "COMM","TIMEOUT","HWLIMIT","CALC","SCAN","LINK",
    //"SOFT","BAD_SUB","UDF","DISABLE","SIMM","READ_ACCESS", "WRITE_ACCESS"};
    epicsAlarmConditionStrings[0]=  (const char *) "NO_ALARM";
    epicsAlarmConditionStrings[1]=  (const char *) "READ";
    epicsAlarmConditionStrings[2]=  (const char *) "WRITE";
    epicsAlarmConditionStrings[3]=  (const char *) "HIHI";
    epicsAlarmConditionStrings[4]=  (const char *) "HIGH";
    epicsAlarmConditionStrings[5]=  (const char *) "LOLO";
    epicsAlarmConditionStrings[6]=  (const char *) "LOW";
    epicsAlarmConditionStrings[7]=  (const char *) "STATE";
    epicsAlarmConditionStrings[8]=  (const char *) "COS";
    epicsAlarmConditionStrings[9]=  (const char *) "COMM";
    epicsAlarmConditionStrings[10]= (const char *) "TIMEOUT";
    epicsAlarmConditionStrings[11]= (const char *) "HWLIMIT";
    epicsAlarmConditionStrings[12]= (const char *) "CALC";
    epicsAlarmConditionStrings[13]= (const char *) "SCAN";
    epicsAlarmConditionStrings[14]= (const char *) "LINK";
    epicsAlarmConditionStrings[15]= (const char *) "SOFT";
    epicsAlarmConditionStrings[16]= (const char *) "BAD_SUB";
    epicsAlarmConditionStrings[17]= (const char *) "UDF";
    epicsAlarmConditionStrings[18]= (const char *) "DISABLE";
    epicsAlarmConditionStrings[19]= (const char *) "SIMM";
    epicsAlarmConditionStrings[20]= (const char *) "READ_ACCESS";
    epicsAlarmConditionStrings[21]= (const char *) "WRITE_ACCESS";

    ccc = _ccc;
    pvAlias = _pv;
    pv = _pv;

    ++handleNext;
    if(handleNext==0)   // In case we loop round 4294967295(!)
    {
        ++handleNext;   // 0 reserved for handle not found
    }

    pyCafeFlag=_pyCafeFlag;

    mapPulseIDBufferSize=SF_PULSE_ID_BUFFER_SIZE;

    //connectionHandlerArgs = (connect_handler_args) NULL;
    //eventHandlerArgs = (event_handler_args) NULL;

    handle = handleNext;
    groupHandle = 0;

    channelID=NULL;

    alarmStatus = -1;
    alarmSeverity = -1;

    //New Oct. 2018
    desc="";
    //hhsv=-1;
    //hsv =-1;
    //lsv =-1;
    //llsv=-1;

    aSevStruct.hhsv=-1;
    aSevStruct.hsv =-1;
    aSevStruct.lsv =-1;
    aSevStruct.llsv=-1;

    hasDesc=false;
    hasAlarmSevStruct=false;

    ts.secPastEpoch=0;
    ts.nsec=0;

    usrArgs= (unsigned long) 0;
    dataType= (chtype) NULL;
    dbrDataType= (chtype) NULL;
    cafeDbrType=CAFENUM::DBR_NONE;

    beamEventNo=0;

    dataBuffer  = NULL;
    ctrlBuffer  = NULL;
    putBuffer   = NULL;
    stsackBuffer= NULL;

    status=ICAFE_CS_NEVER_CONN;

    //Default - we will let the user methods take control of this
    channelRequestMetaDataClient.cafeDbrType     = CAFENUM::DBR_TIME; //PLAIN;
    channelRequestMetaData.cafeDbrType           = CAFENUM::DBR_TIME;
    channelRequestMetaPrimitive.cafeDbrType      = CAFENUM::DBR_PRIMITIVE;

    channelRequestMetaCtrlClient.cafeDbrType     = CAFENUM::DBR_CTRL;
    channelRequestMetaCtrl.cafeDbrType           = CAFENUM::DBR_CTRL;
    //channelRequestMetaCtrlRepository.cafeDbrType = CAFENUM::DBR_CTRL;

    deviceAttributeDeliminator=DEFAULT_DEVICE_ATTRIBUTE_DELIMINATOR;
    channelDeviceAttribute.init(pv,deviceAttributeDeliminator);


    widgetV.reserve(2);

    mpV.reserve(2);
    monitorAction.reserve(2);
    hasNewData=true; // used by HandleHelper.getMonitorAction(); start with true

    //channelRequestPolicyPut
     

    channelRequestPolicyPut.setPolicy(_channelRequestPolicyPut.getWhenToFlushSendBuffer(),
                                      _channelRequestPolicyPut.getWaitKind(), _channelRequestPolicyPut.getMethodKind());

    channelRequestPolicyGet.setPolicy(_channelRequestPolicyGet.getWhenToFlushSendBuffer(),
                                      _channelRequestPolicyGet.getWaitKind(), _channelRequestPolicyGet.getMethodKind());

    //Use same policy for get ctrl
    channelRequestPolicyGetCtrl.setPolicy(_channelRequestPolicyGet.getWhenToFlushSendBuffer(),
                                          _channelRequestPolicyGet.getWaitKind(), _channelRequestPolicyGet.getMethodKind());

    channelGetActionWhenMonitorPolicy.setActionKind(_channelGetActionWhenMonitorPolicy.getActionKind());

    //cout << "when /" << channelRequestPolicyMaster.getWhenToFlushSendBuffer() << endl;
    //cout << "wait /" << channelRequestPolicyMaster.getWaitKind() << endl;
    //cout << "method /" << channelRequestPolicyMaster.getMethodKind() << endl;

    //Define default policies
    //channelRequestPolicyPut.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
    //CAFENUM::NO_WAIT, CAFENUM::WITH_CALLBACK_DEFAULT);

    //channelRequestPolicyGet.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
    //							  CAFENUM::WAIT,  CAFENUM::WITH_CALLBACK_DEFAULT); //WITHOUT_CALLBACK);

    //channelRequestPolicyGetCtrl.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
    //							  CAFENUM::WAIT, CAFENUM::WITHOUT_CALLBACK); //WITH_CALLBACK_DEFAULT);

#if HAVE_PYTHON_H
    pyConnectCallbackVector.reserve(1);
    pyOpenCallbackFlag=false;

#if HAVE_PYCAFE_EXT
    //Do nothing as PyCafe_ext is compiled

    //Py_Initialize();
    //dlopen("libpython2.6.so", RTLD_GLOBAL|RTLD_LAZY);
    //dlopen("/opt/gfa/python-3.5/latest/lib/libpython3.5m.so", RTLD_GLOBAL|RTLD_LAZY);
    //PyInit_PyCafe(); from Python 3 onwards
    //initPyCafe(); //only if PyCafe.h as opposed to PyCafe_api.h
    //PyEval_InitThreads();
    //#else
    //Py_Initialize();

#else
    //Give non Python APIs (e.g. MATLAB) a chance to turn this off
    //MATLAB needs to turn this off

    //Careful With That GIL,
    //Any method that calls this from Python, e.g., open, must be done so ***with*** the GIL
    //Set pyCafeFlag to false if opening from C++!
    if (pyCafeFlag)
    {    
      PyGILState_STATE gstate;
      gstate =  PyGILState_Ensure();
      //std::cout << "GIL state " << std::endl;
      import_PyCafe(); // Use PyCafe_api.h

      PyGILState_Release(gstate);
    }
#endif

#endif

}

#if HAVE_PYTHON_H


/*void * Conduit::PyOpenHandler() const
{
    py_cb_handle_open_wrapper(handle, channelRegalia.cafeConnectionState);

    return (void *) 0;
}*/


 /*void * Conduit::PyConnectHandler() const
{
    py_cb_handle_connect_wrapper(handle, pv, channelRegalia.cafeConnectionState);

    return (void *) 0;
}*/

void * Conduit::PyConnectHandler() const
{

  //std::cout << pyConnectCallbackFn << std::endl;

  //if (pyConnectCallbackFn != NULL) {
  //      cy_connect_handler_wrapper(pyConnectCallbackFn, handle, pv, channelRegalia.cafeConnectionState);
  //  }

   
    for (unsigned int i=0; i < pyConnectCallbackVector.size(); ++i) {
      //std::cout << "\n void * Conduit::PyConnectHandler() " << handle << " " 
      //          << " " <<  pv << " " << channelRegalia.cafeConnectionState << std::endl; 
      //std::cout << i << " " << pyConnectCallbackVector[i] << std::endl;
        cy_connect_handler_wrapper(pyConnectCallbackVector[i], handle, pv, channelRegalia.cafeConnectionState);
	//std::cout << "==============================================================================\n" << std::endl;
    } 
   

    return (void *) 0;
}


void * Conduit::PyGetHandler() const
{
    void * cythonCallback=channelRequestPolicyGet.getCyCallback();
    if (cythonCallback == NULL) {
     std::cout << " NO CYTHON CALLBACK MATCH FOUND in Conduit::PyGetHandler()" << std::endl;
     return (void *) 0; ;  
    }

    cy_cb_handle_get_wrapper(cythonCallback, handle);
    //py_cb_handle_get_wrapper(handle);

    return (void *) 0;
}

void * Conduit::PyPutHandler() const
{
  
  void * cythonCallback=channelRequestPolicyPut.getCyCallback();
   if (cythonCallback == NULL) {
     std::cout << " NO CYTHON CALLBACK MATCH FOUND in Conduit::PyPutHandler()" << std::endl;
     return (void *) 0; ;  
   }

  cy_cb_handle_put_wrapper(cythonCallback, handle);
      //py_cb_handle_put_wrapper(handle);

    return (void *) 0;
}


/*void * Conduit::PyEventHandler() const
{
    //py_cb_handle_wrapper(handle);

    py_cb_handle_monid_wrapper(handle, (unsigned long) usrArgs);

    return (void *) 0;
}
*/


/*void * Conduit::PyEventHandler(unsigned int monid) const
{
    //py_cb_handle_wrapper(handle);

    py_cb_handle_monid_wrapper(handle, monid);

    return (void *) 0;
}
*/

/*

void * Conduit::CyHandleHandler() const
{
    void * cythonCallback = NULL;
    
    for (int i=0; i < mpV.size(); ++i) {
        if  ((unsigned long) mpV[i].getID() == (unsigned long) usrArgs) {
	    cythonCallback = mpV[i].getCyCallback(); 	    
	    break;  
        }        
    }
    //std::cout << "  cythonCallback " << cythonCallback << std:: endl;
    if (cythonCallback == NULL) {
      //std::cout << " NO CYTHON CALLBACK MATCH FOUND in Conduit::CyEventHandler()" << std::endl;
        return (void *) 0; ;  
    }
    cy_handle_handler_wrapper(cythonCallback, handle);
    return (void *) 0;
}



void * Conduit::CyEventHandler() const
{
    void * cythonCallback = NULL;
    
    for (int i=0; i < mpV.size(); ++i) {
        if  ((unsigned long) mpV[i].getID() == (unsigned long) usrArgs) {
	    cythonCallback = mpV[i].getCyCallback(); 	    
	    break;  
        }        
    }
    //std::cout << "  cythonCallback " << cythonCallback << std:: endl;
    if (cythonCallback == NULL) {
      //std::cout << " NO CYTHON CALLBACK MATCH FOUND in Conduit::CyEventHandler()" << std::endl;
        return (void *) 0; ;  
    }
    cy_event_handler_wrapper(cythonCallback, handle, pv);
    return (void *) 0;
}


void * Conduit::CyDataEventHandler() const
{
   
  //std::cout << " void * Conduit::CyDataEventHandler() " << std::endl;
    PVDataHolder pvd(channelRequestMetaData.nelem);
    //size is set in conduitEventHandlerArgs.h
    void * cythonCallback = NULL;
    
    for (int i=0; i < mpV.size(); ++i) {
      //std::cout << i << " " <<   mpV[i].getCyCallback() 
      //		<< " " <<   mpV[i].getID() << " " << pv << " " << handle 
      //	         << std::endl; 
      //std::cout << "user args " << (unsigned long) usrArgs << std::endl;
        if  ((unsigned long) mpV[i].getID() == (unsigned long) usrArgs) {
	     
	    cythonCallback = mpV[i].getCyCallback(); 	    
	    break;  
        }        
    }


    if (cythonCallback == NULL) {
      std::cout << " NO CYTHON CALLBACK MATCH FOUND Conduit::CyDataEventHandler()" << std::endl;
        return (void *) 0; 
    }
   
    getPVDataHolder(pvd);
   
    cy_data_event_handler_wrapper(cythonCallback, handle, pv, pvd);
    //std::cout << " CYTHON CALLBACK MATCH FOUND Conduit::CyDataEventHandler()" << std::endl;
    return (void *) 0;
}
*/


/*void * Conduit::PyDataEventHandler() const
{
    PVDataHolder pvd(channelRequestMetaData.nelem);
    //size is set in conduitEventHandlerArgs.h
    getPVDataHolder(pvd);
    py_cb_wrapper(pvd, handle, pv);
    return (void *) 0;
}*/

void * Conduit::CyMonitorHandler() const
{
#define __METHOD__ "Conduit::CyMonitorHandler() "

    void * cythonCallback = NULL;
    unsigned int ncbparameters = 1;
    
    for (int i=0; i < mpV.size(); ++i) {
        if  ((unsigned long) mpV[i].getID() == (unsigned long) usrArgs) {	     
	    cythonCallback = mpV[i].getCyCallback(); 	    
	    ncbparameters = mpV[i].getNoCyCallbackParameters(); 
	    break;  
        }        
    }

    if (cythonCallback == NULL) {
        std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ <<  std::endl;
        std::cout << " NO CYTHON CALLBACK MATCH FOUND " << std::endl;
        return (void *) 0; ;  
    }

    //ptime timeStart(microsec_clock::local_time());
    //double  timeElapsed=0;
           
     if (ncbparameters == 0) {
        cy_monitor_handler_wrapper(cythonCallback);
     }     

     else if (ncbparameters == 1) {
        cy_handle_handler_wrapper(cythonCallback, handle);
    }
    else if (ncbparameters == 2) {
        cy_event_handler_wrapper(cythonCallback, handle, pv);
    }
    else if (ncbparameters == 3) {
        PVDataHolder pvd(channelRequestMetaData.nelem);
        getPVDataHolder(pvd);   
	cy_data_event_handler_wrapper(cythonCallback, handle, pv, pvd);
    }
    else {
        std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ <<  std::endl;
        std::cout << " Wrong no. cb parameters " << ncbparameters << std::endl; 
        std::cout << " Allowed are 1 (handle) 2 (+pv) 3 (+pvdata) "  << std::endl;
    }
     
    //ptime timeEnd(microsec_clock::local_time());
    //time_duration duration(timeEnd-timeStart);
    //timeElapsed= (double) duration.total_microseconds()/1000000.0;
    //std::cout << "timeElapsed  " << timeElapsed << std::endl;

    return (void *) 0; 

#undef __METHOD__
}




/*void * Conduit::PyCtrlEventHandler() const
{

    PVCtrlHolder pvc(channelRequestMetaCtrl.nelem);
    //size is set in conduitEventHandlerArgs.h
    getPVCtrlHolder(pvc);
    py_cb_ctrl_wrapper(pvc, handle, pv);

    return (void *) 0;
}*/


void * Conduit::CyCtrlEventHandler() const
{
   
    PVCtrlHolder pvc(channelRequestMetaCtrl.nelem);
    //size is set in conduitEventHandlerArgs.h
    void * cythonCallback = NULL;
    
    for (int i=0; i < mpV.size(); ++i) {
        //std::cout << i << " " <<   mpV[i].getCyCallback() 
        //	         << " " <<   mpV[i].getID() 
        //	         << std::endl;  
        if  ((unsigned long) mpV[i].getID() == (unsigned long) usrArgs) {
	    cythonCallback = mpV[i].getCyCallback(); 	    
	    break;  
        }        
    }
    if (cythonCallback == NULL) {
      //std::cout << " NO CYTHON CALLBACK MATCH FOUND in Conduit::CyCtrlEventHandler()" << std::endl;
        return (void *) 0; ;  
    }

    getPVCtrlHolder(pvc);
    cy_ctrl_event_handler_wrapper(cythonCallback, handle, pv, pvc);
    return (void *) 0;
}
#endif



/**
 *  \brief Called from Granules.cc \n
 *  Sends data from the Conduit::putBuffer to the process variable.
 *  It does not wait to determine if the data was actually written or not.
 *  \return ECA_NORMAL     - Normal successful completion
 *  \return ECA_BADCHID    - Corrupted Channel Identifier (chid)
 *  \return ECA_BADTYPE    - Invalid DBR_XXX type
 *  \return ECA_BADCOUNT   - Requested count larger than native element count
 *  \return ECA_NOWTACCESS - Write access denied
 *  \return ECA_ALLOCMEM   - Unable to allocate memory
 *  \return ECA_DISCONN    - Channel is disconnected
 */
int  Conduit::put(void) const
{
#define __METHOD__ "Conduit::put()"

    return ca_array_put(channelRequestMetaPrimitive.dbrDataType, channelRequestMetaPrimitive.nelem,
                        channelRegalia.channelID, putBuffer);
#undef __METHOD__
};


/**
 *  \brief Called from Granules.cc \n
 *  Sends data from the Conduit::putBuffer to the process variable.
 *  It does not wait to determine if the data was actually written or not.
 *  \param callbackHandlerPut input: pCallback
 *  \return ECA_NORMAL if OK else ECA error
 */
int  Conduit::putWithCallback(pCallback callbackHandlerPut) const
{
#define __METHOD__ "Conduit::putWithCallback(pCallback callbackHandlerPut) "

    //cout <<  "channelRequestMetaPrimitive.nelem: " << channelRequestMetaPrimitive.nelem << endl;
    //for (int i=0; i< channelRequestMetaPrimitive.nelem; ++i) {
    //                   cout << (*(&((putBuffer)->fltval)+i)) << " [" << i << "] " << endl;
    //}


    return ca_array_put_callback(channelRequestMetaPrimitive.dbrDataType, channelRequestMetaPrimitive.nelem,
                                 channelRegalia.channelID,  putBuffer, callbackHandlerPut,(void *) (long long) handle );
#undef __METHOD__
};


/**
 *  \brief Called from Granules.cc \n
 *  Retrieves PV data through channel access into the Conduit::dataBuffer
 *  \return ECA_NORMAL if OK else ECA error
 */
int  Conduit::get(void) const
{
#define __METHOD__  "Conduit::get(void) "
  /* For testing readout time for waveforms! */
  /*
    using namespace boost::posix_time;
    ptime timeStart(microsec_clock::local_time());
    double  timeElapsed=0;
    unsigned int  nPoll=0;
    unsigned nevent = 10;
    for (int i=0; i< nevent; ++i) {
        ca_array_get(channelRequestMetaData.dbrDataType, channelRequestMetaData.nelem,
			  channelRegalia.channelID,dataBuffer);
    } 
    ptime timeEnd(microsec_clock::local_time());
    time_duration duration(timeEnd-timeStart);
    timeElapsed= (double) duration.total_microseconds()/1000000.0;

    std::cout << "Time Elapsed " << timeElapsed << std::endl;
    std::cout << "Time Elapsed/per event " << timeElapsed/nevent << std::endl;
    */ 
    return ca_array_get(channelRequestMetaData.dbrDataType, channelRequestMetaData.nelem,
                        channelRegalia.channelID, dataBuffer);

#undef __METHOD__
};



/**
 * \brief Called from Granules.cc \n
 *  Retrieves PV data through channel access into the Conduit::dataBuffer thru callbackHandlerGet
 *  \param callbackHandlerGet input: pCallback
 *  \return ECA_NORMAL if OK else ECA error
 */
int  Conduit::getWithCallback(pCallback callbackHandlerGet) const
{
#define __METHOD__ "Conduit::getCallback(pCallback callbackHandlerGet) "
  //Need ca_pend_io and ca-flush_io!!
  /*
  if (channelRequestMetaData.nelem > 1) {

    using namespace boost::posix_time;
    ptime timeStart(microsec_clock::local_time());
    double  timeElapsed=0;
    unsigned int  nPoll=0;
    unsigned int nevent = 1000;

    std::cout << "NELM ==>  " << channelRequestMetaData.nelem << std::endl;

    for (int i=0; i< nevent; ++i) {
        ca_array_get(channelRequestMetaData.dbrDataType, channelRequestMetaData.nelem,
			  channelRegalia.channelID,dataBuffer);
    } 
    ptime timeEnd(microsec_clock::local_time());
    time_duration duration(timeEnd-timeStart);
    timeElapsed= (double) duration.total_microseconds()/1000000.0;

    std::cout << "Time Elapsed nevents " << timeElapsed << " " << nevent << std::endl;
    std::cout << "Time Elapsed/per event " << timeElapsed/nevent << std::endl;

    ptime timeStart2(microsec_clock::local_time());
    timeElapsed=0;
    nPoll=0;
  
    for (int i=0; i<nevent; ++i) {
        ca_array_get_callback(channelRequestMetaData.dbrDataType, channelRequestMetaData.nelem,
                                 channelRegalia.channelID,callbackHandlerGet,(void *) (long long) handle );
       
    } 
    ptime timeEnd2(microsec_clock::local_time());
    time_duration duration2(timeEnd2-timeStart2);
    timeElapsed= (double) duration2.total_microseconds()/1000000.0;

    std::cout << "Time Elapsed with CB " << timeElapsed << std::endl;
    std::cout << "Time Elapsed/per event " << timeElapsed/nevent << std::endl;

  }
 */

  //std::cout << "Datatype " << channelRequestMetaData.dbrDataType << std::endl;
  //std::cout << "Nelem    " << channelRequestMetaData.nelem << std::endl;
  //std::cout << "channelRegalia.channelID " <<  channelRegalia.channelID << std::endl;
  //std::cout << "handle " << handle << std::endl;

    return ca_array_get_callback(channelRequestMetaData.dbrDataType, channelRequestMetaData.nelem,
                                 channelRegalia.channelID,callbackHandlerGet,(void *) (long long) handle );
#undef __METHOD__
};



/**
 * \brief Called from Granules.cc \n
 *  Retrieves Ctrl data through channel access into the Conduit::ctrlBuffer
 *  \return ECA_NORMAL if OK else ECA error
 */
int  Conduit::getCtrl(void) const
{
#define __METHOD__ "Conduit::getCtrl(void) "


    return ca_array_get(channelRequestMetaCtrl.dbrDataType, channelRequestMetaCtrl.nelem,
                        channelRegalia.channelID, ctrlBuffer);
#undef __METHOD__
};


/**
 *  \brief Called from granules.cc and conduitConnectionHandersArgs.h \n
 *  Retrieves Ctrl data through channel access into the Conduit::ctrlBuffer thru callbackHandlerCtrl
 *  \param callbackHandlerCtrl input: pCallback
 *  \return ECA_NORMAL if OK else ECA error
 */
int  Conduit::getCtrlWithCallback(pCallback callbackHandlerCtrl) const
{
#define __METHOD__ "Conduit::getCtrlCallback(pCallback callbackHandlerCtrl) "


    return ca_array_get_callback(channelRequestMetaCtrl.dbrDataType,
                                 channelRequestMetaCtrl.nelem,
                                 //min(channelRequestMetaCtrl.nelem,max_nelem_for_ctrl_buffer),
                                 channelRegalia.channelID, callbackHandlerCtrl, (void *) (long long) handle);
#undef __METHOD__
};



/**
 *  \brief Called from Granules.cc and conduitConnectionHandersArgs.h \n
 *  Retrieves STSACK data through channel access into the Conduit::STSACKBuffer thru callbackHandlerSTSACK
 *  \param callbackHandlerSTSACK input: pCallback
 *  \return ECA_NORMAL if OK else ECA error
 */
int  Conduit::getSTSACKWithCallback(pCallback callbackHandlerSTSACK) const
{
#define __METHOD__ "Conduit::getSTSACKWithCallback(pCallback callbackHandlerSTSACK) "

    return ca_array_get_callback(DBR_STSACK_STRING, channelRequestMetaSTSACK.nelem,
                                 channelRegalia.channelID, callbackHandlerSTSACK, (void *) (long long) handle);
#undef __METHOD__
};



/**
 *  \brief Called from Granules.cc and conduitConnectionHandersArgs.h \n
 *  Retrieves ClassName data through channel access into the Conduit::ClassNameBuffer thru callbackHandlerClassName
 *  \param callbackHandlerClassName input: pCallback
 *  \return ECA_NORMAL if OK else ECA error
 */
int  Conduit::getClassNameWithCallback(pCallback callbackHandlerClassName) const
{
#define __METHOD__ "Conduit::getClassNameWithCallback(pCallback callbackHandlerClassName) "

    return ca_array_get_callback(DBR_CLASS_NAME, 1,
                                 channelRegalia.channelID, callbackHandlerClassName, (void *)(long long) handle);
#undef __METHOD__
};


/**
 *  \brief Called from cafe.cc
 *  \param _pvd output: PVDataHolder
 *  \return ECA_NORMAL if OK else ECA error
 */
int  Conduit::getPVDataHolder(PVDataHolder & _pvd) const
{
#define __METHOD__ "Conduit::getPVDataHolder(PVDataHolder & _pvd) "

    Helper helper;

    //Check on the datatype transferred

    chtype channelType = channelRequestMetaData.getDbrDataType();
    CAFENUM::DBR_TYPE dbrTypeClass=helper.convertToCAFEDbrTypeClass(channelType);

    if ( channelRegalia.getCafeConnectionState()  != ICAFE_CS_NEVER_CONN  &&  channelRegalia.getCafeConnectionState()  != ICAFE_CS_CLOSED)
    {

        switch(dbrTypeClass)
        {
        case CAFENUM::DBR_PRIMITIVE:
        case CAFENUM::DBR_STS:
        case CAFENUM::DBR_TIME:
            break;
        default:

            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cout << "CAFE INTERNAL FUNNY: dbrTypeClass = " <<  dbrTypeClass <<
                 " is NOT appropriate for this method" << endl;
            cout << "Method does not deal with this DBR_TYPE: "
                 << dbr_type_to_text(channelType) << endl;
            break;
        }

    } //if

    unsigned int  offset   = channelRequestMetaData.getOffset(); //channelRequestMetaDataClient.getOffset();
    unsigned int  nelem    = channelRequestMetaData.getNelem()-offset;

    nelem=min(_pvd.nelem,nelem); // Add this for getCache method

    _pvd.alarmStatus    = -1;//alarmStatus;
    _pvd.alarmSeverity  = -1;//alarmSeverity;
    _pvd.ts.nsec        =  0;//ts;
    _pvd.ts.secPastEpoch=  0;

    _pvd.nelem          =  min(_pvd.size,nelem); //channelRequestMetaData.getNelem();
    _pvd.beamEventNo    =  beamEventNo;
    _pvd.status         =  status; //ICAFE_NORMAL;

    _pvd.dataTypeNative = (CAFE_DATATYPE) channelRegalia.getDataType();
    _pvd.dataType       = (CAFE_DATATYPE) channelRequestMetaData.getDataType();//dataTypeRequest;
    _pvd.dbrDataType    = channelRequestMetaData.getDbrDataType();

    strcpy(_pvd.pv,      pv.c_str());
    strcpy(_pvd.pvAlias, pvAlias.c_str());
    strcpy(_pvd.device,  channelDeviceAttribute.getDevice());
    strcpy(_pvd.attrib,  channelDeviceAttribute.getAttribute());

    if (_pvd.dataTypeNative==DBR_ENUM)
    {
        _pvd.noStr =  ((struct dbr_ctrl_enum *) ctrlBuffer)->no_str;
        memcpy( _pvd.strs,  &(((struct dbr_ctrl_enum *) ctrlBuffer)->strs),  sizeof(_pvd.strs)) ;
        //for (int i=0; i<_pvd.noStr; ++i) cout << __METHOD__ << i << " " << _pvd.strs[i] << endl;
    }

    switch (channelType)
    {
    case DBR_TIME_DOUBLE:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].d = (*(&((dataBuffer)->tdblval.value)+i+offset));
        }
        _pvd.ts            = ((struct dbr_time_double *) dataBuffer)->stamp;
        _pvd.alarmStatus   = ((struct dbr_time_double *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_time_double *) dataBuffer)->severity;

        break;

    case DBR_TIME_FLOAT:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].f = (*(&((dataBuffer)->tfltval.value)+i+offset));
        }
        _pvd.ts            = (epicsTimeStamp) ((struct dbr_time_float *) dataBuffer)->stamp;
        _pvd.alarmStatus   = ((struct dbr_time_float *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_time_float *) dataBuffer)->severity;

        break;

    case DBR_TIME_LONG:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].l = (*(&((dataBuffer)->tlngval.value)+i+offset));
        }
        _pvd.ts            = ((struct dbr_time_long  *) dataBuffer)->stamp;
        _pvd.alarmStatus   = ((struct dbr_time_long  *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_time_long  *) dataBuffer)->severity;

        break;

    case DBR_TIME_SHORT:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].s = (*(&((dataBuffer)->tshrtval.value)+i+offset));
        }
        _pvd.ts            = ((struct dbr_time_short *) dataBuffer)->stamp;
        _pvd.alarmStatus   = ((struct dbr_time_short *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_time_short *) dataBuffer)->severity;

        break;

    case DBR_TIME_ENUM:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].us = (*(&((dataBuffer)->tenmval.value)+i+offset));
        }
        _pvd.ts            = ((struct dbr_time_enum *) dataBuffer)->stamp;
        _pvd.alarmStatus   = ((struct dbr_time_enum *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_time_enum *) dataBuffer)->severity;

        break;

    case DBR_TIME_CHAR:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].ch = (*(&((dataBuffer)->tchrval.value)+i+offset));
        }
        _pvd.ts            = ((struct dbr_time_char *) dataBuffer)->stamp;
        _pvd.alarmStatus   = ((struct dbr_time_char *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_time_char *) dataBuffer)->severity;

        break;

    case DBR_TIME_STRING:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            strcpy(_pvd.val[i].str, (*(&((dataBuffer)->tstrval.value)+i+offset)));
        }
        _pvd.ts            = ((struct dbr_time_string *) dataBuffer)->stamp;
        _pvd.alarmStatus   = ((struct dbr_time_string *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_time_string *) dataBuffer)->severity;

        break;

    case DBR_STS_DOUBLE:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].d = (*(&((dataBuffer)->sdblval.value)+i+offset));
        }
        _pvd.alarmStatus   = ((struct dbr_sts_double *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_sts_double *) dataBuffer)->severity;

        break;

    case DBR_STS_FLOAT:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].f = (*(&((dataBuffer)->sfltval.value)+i+offset));
        }
        _pvd.alarmStatus   = ((struct dbr_sts_float *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_sts_float *) dataBuffer)->severity;

        break;

    case DBR_STS_LONG:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].l = (*(&((dataBuffer)->slngval.value)+i+offset));
        }
        _pvd.alarmStatus   = ((struct dbr_sts_int  *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_sts_int  *) dataBuffer)->severity;

        break;

    case DBR_STS_SHORT:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].s = (*(&((dataBuffer)->sshrtval.value)+i+offset));
        }
        _pvd.alarmStatus   = ((struct dbr_sts_short *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_sts_short *) dataBuffer)->severity;

        break;

    case DBR_STS_ENUM:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].us = (*(&((dataBuffer)->senmval.value)+i+offset));
        }
        _pvd.alarmStatus   = ((struct dbr_sts_enum *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_sts_enum *) dataBuffer)->severity;

        break;

    case DBR_STS_CHAR:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].ch = (*(&((dataBuffer)->schrval.value)+i+offset));
        }
        _pvd.alarmStatus   = ((struct dbr_sts_char *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_sts_char *) dataBuffer)->severity;

        break;

    case DBR_STS_STRING:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            strcpy(_pvd.val[i].str, (*(&((dataBuffer)->sstrval.value)+i+offset)));
        }
        _pvd.alarmStatus   = ((struct dbr_sts_string *) dataBuffer)->status;
        _pvd.alarmSeverity = ((struct dbr_sts_string *) dataBuffer)->severity;

        break;

    case DBR_DOUBLE:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].d = (*(&((dataBuffer)->doubleval)+i+offset));
        }
        break;

    case DBR_FLOAT:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].f = (*(&((dataBuffer)->fltval)+i+offset));
        }
        break;

    case DBR_LONG:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].l = (*(&((dataBuffer)->longval)+i+offset));
        }
        break;

    case DBR_SHORT:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].s = (*(&((dataBuffer)->shrtval)+i+offset));
        }
        break;

    case DBR_ENUM:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].us = (*(&((dataBuffer)->enmval)+i+offset));
        }
        break;

    case DBR_CHAR:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            _pvd.val[i].ch = (*(&((dataBuffer)->charval)+i+offset));
        }
        break;

    case DBR_STRING:
        for (unsigned int  i=0; i<_pvd.nelem; ++i)
        {
            strcpy(_pvd.val[i].str, (*(&((dataBuffer)->strval)+i+offset)));
        }
        break;

    case TYPENOTCONN:
        if ( channelRegalia.getCafeConnectionState()  != ICAFE_CS_NEVER_CONN  &&  channelRegalia.getCafeConnectionState()  != ICAFE_CS_CLOSED )
        {
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__  << endl;
            cout << "ERROR CAFE_TYPENOTCONN: dataType: "
                 <<  channelType <<  " : " << dbr_type_to_text(channelType) << endl;
        }
        break;
    default:
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__  << endl;
        cout << "The switch case does not support this channelType: "
             << channelType << " : " << dbr_type_to_text(channelType) << endl;
        break;
    }

    //Do this to prevent overflow error in epicsTime time(ts) routines!
    //This bad number can occur in timeouts
    if(_pvd.ts.nsec>1000000000)
    {
        _pvd.ts.nsec=0;
    }


    return ICAFE_NORMAL;
#undef __METHOD__
};



/**
 *  \brief Called from cafe.cc
 *  \param _pvc output: PVCtrlHolder
 *  \return ICAFE_NORMAL if OK else ICAFE error
 */
int  Conduit::getPVCtrlHolder(PVCtrlHolder & _pvc) const
{
#define __METHOD__ " Conduit::getPVCtrlHolder(PVCtrlHolder & _pvc) "

    Helper helper;

    int channelType=channelRequestMetaCtrl.getDbrDataType();

    CAFENUM::DBR_TYPE dbrTypeClass=helper.convertToCAFEDbrTypeClass(channelType);

    if ( channelRegalia.getCafeConnectionState()  != ICAFE_CS_NEVER_CONN &&  channelRegalia.getCafeConnectionState()  != ICAFE_CS_CLOSED )
    {

        switch(dbrTypeClass)
        {
        case CAFENUM::DBR_GR:
        case CAFENUM::DBR_CTRL:
            break;
        default:
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cout << "CAFE INTERNAL FUNNY: dbrTypeClass = " <<  dbrTypeClass <<
                 " is NOT appropriate for this method" << endl;
            cout << "Method does not deal with this DBR_TYPE: "
                 << dbr_type_to_text(channelType) << endl;
            break;
        }

    }

    unsigned int  offset   = channelRequestMetaCtrlClient.getOffset();
    unsigned int  nelem    = channelRequestMetaCtrl.getNelem()-offset;

    nelem=min(_pvc.nelem,nelem); // Add this for getCache method

    //
    _pvc.alarmStatus    = -1;//alarmStatus;
    _pvc.alarmSeverity  = -1;//alarmSeverity;
    _pvc.nelem          =  min(_pvc.size,nelem);

    //Default from constructor if not connected
    _pvc.dataType   = (CAFE_DATATYPE) channelRequestMetaCtrl.getDataType(); // dataTypeRequest;

    _pvc.dataTypeNative   = (CAFE_DATATYPE) channelRegalia.getDataType(); // dataTypeRequest;

    _pvc.dbrDataType    = channelRequestMetaCtrl.getDbrDataType();
    _pvc.status         = status;


    strcpy(_pvc.pv, pv.c_str());
    strcpy(_pvc.pvAlias, pvAlias.c_str());
    strcpy(_pvc.device,channelDeviceAttribute.getDevice());
    strcpy(_pvc.attrib,channelDeviceAttribute.getAttribute());


    if (_pvc.dataTypeNative==DBR_ENUM)
    {
        _pvc.noStr =  ((struct dbr_ctrl_enum *) ctrlBuffer)->no_str;
        memcpy( _pvc.strs,  &(((struct dbr_ctrl_enum *) ctrlBuffer)->strs),  sizeof(_pvc.strs)) ;
        //for (int i=0; i<_pvc.noStr; ++i) cout << __METHOD__ << i << " " << _pvc.strs[i] << endl;
    }


    switch (channelType)
    {

    case DBR_CTRL_CHAR:
        for (unsigned int  i=0; i<_pvc.nelem; ++i)
        {
            _pvc.val[i].ch =   (*(&((ctrlBuffer)->cchrval.value)+i+offset));
        }

        _pvc.precision        = 0;  // struct dbr_ctrl_char does not have the precision member
        _pvc.RISC_pad.ch      = ((struct dbr_ctrl_char *) ctrlBuffer)->RISC_pad;

        _pvc.alarmStatus   = ((struct dbr_ctrl_char *) ctrlBuffer)->status;
        _pvc.alarmSeverity = ((struct dbr_ctrl_char *) ctrlBuffer)->severity;

        memcpy(_pvc.units, &(((struct dbr_ctrl_char *) ctrlBuffer)->units), sizeof(char[MAX_UNITS_SIZE]));

        _pvc.upperDispLimit.ch    = (dbr_char_t) ((struct dbr_ctrl_char *) ctrlBuffer)->upper_disp_limit;
        _pvc.lowerDispLimit.ch    = (dbr_char_t) ((struct dbr_ctrl_char *) ctrlBuffer)->lower_disp_limit;
        _pvc.upperAlarmLimit.ch   = (dbr_char_t) ((struct dbr_ctrl_char *) ctrlBuffer)->upper_alarm_limit;
        _pvc.upperWarningLimit.ch = (dbr_char_t) ((struct dbr_ctrl_char *) ctrlBuffer)->upper_warning_limit;
        _pvc.lowerWarningLimit.ch = (dbr_char_t) ((struct dbr_ctrl_char *) ctrlBuffer)->lower_warning_limit;
        _pvc.lowerAlarmLimit.ch   = (dbr_char_t) ((struct dbr_ctrl_char *) ctrlBuffer)->lower_alarm_limit;
        _pvc.upperCtrlLimit.ch    = (dbr_char_t) ((struct dbr_ctrl_char *) ctrlBuffer)->upper_ctrl_limit;
        _pvc.lowerCtrlLimit.ch    = (dbr_char_t) ((struct dbr_ctrl_char *) ctrlBuffer)->lower_ctrl_limit;


        break;

    case DBR_CTRL_FLOAT:
        for (unsigned int  i=0; i<_pvc.nelem; ++i)
        {
            _pvc.val[i].f =   (*(&((ctrlBuffer)->cfltval.value)+i+offset));
        }

        _pvc.precision     = ((struct dbr_ctrl_float *) ctrlBuffer)->precision;
        _pvc.RISC_pad.s    = ((struct dbr_ctrl_float *) ctrlBuffer)->RISC_pad;

        _pvc.alarmStatus   = ((struct dbr_ctrl_float *) ctrlBuffer)->status;
        _pvc.alarmSeverity = ((struct dbr_ctrl_float *) ctrlBuffer)->severity;

        memcpy(_pvc.units, &(((struct dbr_ctrl_float *) ctrlBuffer)->units), sizeof(char[MAX_UNITS_SIZE]));

        _pvc.upperDispLimit.f    = (dbr_float_t) ((struct dbr_ctrl_float *) ctrlBuffer)->upper_disp_limit;
        _pvc.lowerDispLimit.f    = (dbr_float_t) ((struct dbr_ctrl_float *) ctrlBuffer)->lower_disp_limit;
        _pvc.upperAlarmLimit.f   = (dbr_float_t) ((struct dbr_ctrl_float *) ctrlBuffer)->upper_alarm_limit;
        _pvc.upperWarningLimit.f = (dbr_float_t) ((struct dbr_ctrl_float *) ctrlBuffer)->upper_warning_limit;
        _pvc.lowerWarningLimit.f = (dbr_float_t) ((struct dbr_ctrl_float *) ctrlBuffer)->lower_warning_limit;
        _pvc.lowerAlarmLimit.f   = (dbr_float_t) ((struct dbr_ctrl_float *) ctrlBuffer)->lower_alarm_limit;
        _pvc.upperCtrlLimit.f    = (dbr_float_t) ((struct dbr_ctrl_float *) ctrlBuffer)->upper_ctrl_limit;
        _pvc.lowerCtrlLimit.f    = (dbr_float_t) ((struct dbr_ctrl_float *) ctrlBuffer)->lower_ctrl_limit;


        break;

    case DBR_CTRL_DOUBLE:
        for (unsigned int  i=0; i<_pvc.nelem; ++i)
        {
            _pvc.val[i].d =   (*(&((ctrlBuffer)->cdblval.value)+i+offset));
        }
        _pvc.precision     = ((struct dbr_ctrl_double *) ctrlBuffer)->precision;
        _pvc.RISC_pad.s    = ((struct dbr_ctrl_double *) ctrlBuffer)->RISC_pad0;

        _pvc.alarmStatus   = ((struct dbr_ctrl_double *) ctrlBuffer)->status;
        _pvc.alarmSeverity = ((struct dbr_ctrl_double *) ctrlBuffer)->severity;

        memcpy(_pvc.units, &(((struct dbr_ctrl_double *) ctrlBuffer)->units), sizeof(char[MAX_UNITS_SIZE]));

        _pvc.upperDispLimit.d   = (dbr_double_t)  ((struct dbr_ctrl_double *) ctrlBuffer)->upper_disp_limit;
        _pvc.lowerDispLimit.d    = (dbr_double_t) ((struct dbr_ctrl_double *) ctrlBuffer)->lower_disp_limit;
        _pvc.upperAlarmLimit.d   = (dbr_double_t) ((struct dbr_ctrl_double *) ctrlBuffer)->upper_alarm_limit;
        _pvc.upperWarningLimit.d = (dbr_double_t) ((struct dbr_ctrl_double *) ctrlBuffer)->upper_warning_limit;
        _pvc.lowerWarningLimit.d = (dbr_double_t) ((struct dbr_ctrl_double *) ctrlBuffer)->lower_warning_limit;
        _pvc.lowerAlarmLimit.d   = (dbr_double_t) ((struct dbr_ctrl_double *) ctrlBuffer)->lower_alarm_limit;
        _pvc.upperCtrlLimit.d    = (dbr_double_t) ((struct dbr_ctrl_double *) ctrlBuffer)->upper_ctrl_limit;
        _pvc.lowerCtrlLimit.d    = (dbr_double_t) ((struct dbr_ctrl_double *) ctrlBuffer)->lower_ctrl_limit;

        break;

    case DBR_CTRL_SHORT:
        for (unsigned int  i=0; i<_pvc.nelem; ++i)
        {
            _pvc.val[i].s =   (*(&((ctrlBuffer)->cshrtval.value)+i+offset));
        }

        _pvc.precision     = 0;
        _pvc.RISC_pad.s    = 0;

        _pvc.alarmStatus   = ((struct dbr_ctrl_short *) ctrlBuffer)->status;
        _pvc.alarmSeverity = ((struct dbr_ctrl_short *) ctrlBuffer)->severity;

        memcpy(_pvc.units, &(((struct dbr_ctrl_short *) ctrlBuffer)->units), sizeof(char[MAX_UNITS_SIZE]));

        _pvc.upperDispLimit.s    = (dbr_short_t) ((struct dbr_ctrl_short *) ctrlBuffer)->upper_disp_limit;
        _pvc.lowerDispLimit.s    = (dbr_short_t) ((struct dbr_ctrl_short *) ctrlBuffer)->lower_disp_limit;
        _pvc.upperAlarmLimit.s   = (dbr_short_t) ((struct dbr_ctrl_short *) ctrlBuffer)->upper_alarm_limit;
        _pvc.upperWarningLimit.s = (dbr_short_t) ((struct dbr_ctrl_short *) ctrlBuffer)->upper_warning_limit;
        _pvc.lowerWarningLimit.s = (dbr_short_t) ((struct dbr_ctrl_short *) ctrlBuffer)->lower_warning_limit;
        _pvc.lowerAlarmLimit.s   = (dbr_short_t) ((struct dbr_ctrl_short *) ctrlBuffer)->lower_alarm_limit;
        _pvc.upperCtrlLimit.s    = (dbr_short_t) ((struct dbr_ctrl_short *) ctrlBuffer)->upper_ctrl_limit;
        _pvc.lowerCtrlLimit.s    = (dbr_short_t) ((struct dbr_ctrl_short *) ctrlBuffer)->lower_ctrl_limit;

        break;

    case DBR_CTRL_LONG:
        for (unsigned int  i=0; i<_pvc.nelem; ++i)
        {
            _pvc.val[i].l =   (*(&((ctrlBuffer)->clngval.value)+i+offset));
        }

        _pvc.precision     = 0;
        _pvc.RISC_pad.s    = 0;

        _pvc.alarmStatus   = ((struct dbr_ctrl_int  *) ctrlBuffer)->status;
        _pvc.alarmSeverity = ((struct dbr_ctrl_int  *) ctrlBuffer)->severity;

        memcpy(_pvc.units, &(((struct dbr_ctrl_int  *) ctrlBuffer)->units), sizeof(char[MAX_UNITS_SIZE]));

        _pvc.upperDispLimit.l    = (dbr_long_t) ((struct dbr_ctrl_int  *) ctrlBuffer)->upper_disp_limit;
        _pvc.lowerDispLimit.l    = (dbr_long_t) ((struct dbr_ctrl_int  *) ctrlBuffer)->lower_disp_limit;
        _pvc.upperAlarmLimit.l   = (dbr_long_t) ((struct dbr_ctrl_int  *) ctrlBuffer)->upper_alarm_limit;
        _pvc.upperWarningLimit.l = (dbr_long_t) ((struct dbr_ctrl_int  *) ctrlBuffer)->upper_warning_limit;
        _pvc.lowerWarningLimit.l = (dbr_long_t) ((struct dbr_ctrl_int  *) ctrlBuffer)->lower_warning_limit;
        _pvc.lowerAlarmLimit.l   = (dbr_long_t) ((struct dbr_ctrl_int  *) ctrlBuffer)->lower_alarm_limit;
        _pvc.upperCtrlLimit.l    = (dbr_long_t) ((struct dbr_ctrl_int  *) ctrlBuffer)->upper_ctrl_limit;
        _pvc.lowerCtrlLimit.l    = (dbr_long_t) ((struct dbr_ctrl_int  *) ctrlBuffer)->lower_ctrl_limit;

        break;

    case DBR_CTRL_ENUM:

        memcpy(_pvc.val.get(), &(&((ctrlBuffer)->cenmval.value))[offset], sizeof(dbr_short_t)*_pvc.nelem);

        _pvc.precision     = 0;
        _pvc.RISC_pad.s    = 0;
        _pvc.alarmStatus   = ((struct dbr_ctrl_enum *) ctrlBuffer)->status;
        _pvc.alarmSeverity = ((struct dbr_ctrl_enum *) ctrlBuffer)->severity;
        _pvc.noStr         = ((struct dbr_ctrl_enum *) ctrlBuffer)->no_str;
        memcpy(_pvc.strs, &(((struct dbr_ctrl_enum *) ctrlBuffer)->strs),
               sizeof(char)*MAX_ENUM_STRING_SIZE*MAX_ENUM_STATES);

        //no units - with gcc 9.3.0 gives warning: 
	//reading 8 bytes from a region of size 1
        //memcpy(_pvc.units,"",sizeof(char[MAX_UNITS_SIZE]));
	strcpy(_pvc.units, "");

        _pvc.upperDispLimit.us    = 0;
        _pvc.lowerDispLimit.us    = 0;
        _pvc.upperAlarmLimit.us   = 0;
        _pvc.upperWarningLimit.us = 0;
        _pvc.lowerWarningLimit.us = 0;
        _pvc.lowerAlarmLimit.us   = 0;
        _pvc.upperCtrlLimit.us    = 0;
        _pvc.lowerCtrlLimit.us    = 0;

        break;

    case DBR_CTRL_STRING:

        memcpy(_pvc.val.get(), &(&((ctrlBuffer)->cstrval.value))[offset],  sizeof(dbr_string_t)*_pvc.nelem);
        _pvc.alarmStatus   = ((struct dbr_sts_string *) ctrlBuffer)->status;
        _pvc.alarmSeverity = ((struct dbr_sts_string *) ctrlBuffer)->severity;


	//no units - with gcc 9.3.0 gives warning: 
	//reading 8 bytes from a region of size 1
        //memcpy(_pvc.units,"",sizeof(char[MAX_UNITS_SIZE]));
	strcpy(_pvc.units, "");
        
        _pvc.upperDispLimit.us    = 0;
        _pvc.lowerDispLimit.us    = 0;
        _pvc.upperAlarmLimit.us   = 0;
        _pvc.upperWarningLimit.us = 0;
        _pvc.lowerWarningLimit.us = 0;
        _pvc.lowerAlarmLimit.us   = 0;
        _pvc.upperCtrlLimit.us    = 0;
        _pvc.lowerCtrlLimit.us    = 0;

        break;

    default:
        break;
    }



    return ICAFE_NORMAL;

#undef __METHOD__
};

////////////////////////////////////// Monitors //////////////////////////////////////////


/**
 *  \brief Starts a monitor on the epics channel.
 *         Should the MonitorPolicy class have been configured before the channel was first connected
 *         then the dbrDataType will not be known (-1) if the user requested native data type,
 *         resulting in an ECA_BADTYPE error. To avoid this mp.dbrDataType should be overwritten
 *         after interrogating native and dbr data types
 *  \param  mp input: MonitorPolicy class \n
 *          mp.eventID output: evid identifying the monitor subscription
 *  \return ECA_NORMAL   - normal sucessful completiom
 *  \return ECA_BADCHID  - corrupted chid
 *  \return ECA_BADTYPE  - invalid DBR_XXXX type
 *  \return ECA_ALLOCMEM - a local database event add failed
 */
int  Conduit::monitorStart(MonitorPolicy &mp) const
{
#define __METHOD__  "Conduit::monitorStart(MonitorPolicy mp)"

    //cout << __METHOD__ << " mp.getDbrDataType()= " << mp.getDbrDataType() << endl;
    //cout << __METHOD__ << " mp.getHandler() = " << mp.getHandler() << endl;
    //cout << __METHOD__ << " mp.getUserArgs() = " << mp.getUserArgs() << endl;
    //cout << "dataType " << (CAFE_DATATYPE) channelRegalia.getDataType() << endl;
    //cout << "native dataType " << (CAFE_DATATYPE) channelRequestMetaData.getDataType() << endl;
    //cout << "dbrDataType " << channelRequestMetaData.getDbrDataType() << endl;

    if  (mp.getDbrDataType()== -1)
    {
        mp.setDataType((CAFE_DATATYPE) channelRegalia.getDataType() ); // Also sets dbrDataType accordingly
    }

    evid eventID;
    int  status = ca_create_subscription(mp.getDbrDataType(), mp.getNelem(), 
					 channelRegalia.channelID, mp.getMask(),
                                         mp.getHandler(), (void *) mp.getUserArgs(), &eventID);

    mp.setEventID(eventID);

    return status;
#undef __METHOD__
};


/**
 *  \brief Stops a monitor on the epics channel
 *  \param eventID input: evid
 *  \return ECA_NORMAL if OK else ECA_BADCHID (corrupted CHID)
 */
int  Conduit::monitorStop(evid eventID) const
{
#define __METHOD__  "Conduit::monitorStop(evid eventID)"

    return ca_clear_subscription(eventID);

#undef __METHOD__
};
