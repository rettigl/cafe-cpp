///
/// \file    connect.cpp
/// \author  Jan Chrin, PSI
/// \date    June 2018
/// \version CAFE 1.6.0
///

#include <connect.h>
#include <conduitFriends.h>
#include <conduitConnectionHandlerArgs.h>

#if HAVE_PYTHON_H
#if HAVE_PYCAFE_EXT
#include <Python.h>
#include <PyCafe.h>
#else
#include <PyCafe_api.h>
#endif
#if HAVE_LINUX
#include <dlfcn.h>
#endif
#if HAVE_WIN32
#include <windows.h>
#endif
#endif

using namespace std;

/**
 *  \brief Return epics Version as an unsigned short \n
 *  e.g., 31412 for v3.14.12
 *  \param major input
 *  \param minor input
 *  \param patch input
 *  \return epics Version (unsigned short)
 */
unsigned short Connect::epicsVersion(unsigned short & major, unsigned short & minor, unsigned short & patch)
{
    major=EPICS_MAJOR;
    minor=EPICS_MINOR;
    patch=EPICS_PATCH;
    return major*10000+minor*100+patch;
}

/////////////////////////////////////////////////////////////////////
////// PRIVATE METHODS //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/**
 *  \brief Establishes virtual circuit to epics process variable
 *  \param handle input
 *  \param pv input: process variable
 *  \param pCh output: channel identifier (chid)
 *  \return status ECA_NORMAL if all OK else ECA_BADTYPE, ECA_STRTOBIG, ECA_ALLOCMEM
 *
 */
int Connect::createChannel(unsigned int handle, const char * pv, chid &pCh)
{
#define __METHOD__ "Connect::createChannel(unsigned int handle, const char * pv, chid &pCh)"

    ChannelRegalia channelRegalia; 

    cafeConduit_set_by_handle &  handle_index= cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    int _status = ICAFE_NORMAL;

    it_handle =handle_index.find(handle);

    if (it_handle != handle_index.end())
    {
      /*
        // 11 March 2019
        // Allow Python Users to fire their own native callback for when using default callbackHandlerCreate
        // Check pyCallbackFlag in ChannelOpenFolicy and set pyCallbackFlag in conduit object accordingly
        //
        Moved to create Handle
      */     
        _status = ca_create_channel(pv, channelCreatePolicy.getHandler(), (void *) (unsigned long) handle,
                                   channelCreatePolicy.getPriority(), &pCh);

        if (pCh!=NULL)
        {	    
	    if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            
            handle_index.modify(it_handle, change_channelID (pCh));

	    if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

	    channelRegalia = (*it_handle).getChannelRegalia();    
	    //std::cout << " OLD VALUE " << channelRegalia.cafeConnectionState <<  " " << (*it_handle).pv << std::endl;  
     
            if ( channelRegalia.cafeConnectionState == ICAFE_CS_CLOSED) {
	        //This is to overwrite the last values arising from a user supplied closechannelkeephandle method
	        channelRegalia.cafeConnectionState = ICAFE_CS_NEVER_CONN;
                channelRegalia.connectFlag = false;
		if(MUTEX)
		{
		    cafeMutex.lock();   //lock
		}
                handle_index.modify(it_handle, change_channelRegalia (channelRegalia));
                handle_index.modify(it_handle, change_status (ICAFE_CA_OP_CONN_DOWN));
		if(MUTEX)
		{
                    cafeMutex.unlock();   //unlock
		}		     
	    }
            channelRegalia = (*it_handle).getChannelRegalia(); 
	    //std::cout << " NEW VALUE " << channelRegalia.cafeConnectionState <<  " " << (*it_handle).pv << std::endl;
            
            //Install the access rights state change callback handler
            if (ADD_EXCEPTION_EVENT==true)
            {
                ca_add_exception_event(callbackHandlerException, (void *) (unsigned long) handle);
            }
            ca_replace_access_rights_event(pCh, callbackHandlerAccessRights);
        }

        if (_status != ECA_NORMAL)
        {
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << " ca_create_channel failed: " << endl;
            if (_status == ECA_EVDISALLOW)
            {
                cout << " due to inappropriate function " << endl;
            }
            cafeStatus.report(_status);
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            handle_index.modify(it_handle, change_status (_status) );
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
            return (int) _status;
        }

        //To Flush Send Buffer or not? Examine channelOpenPolicy.
        if (channelOpenPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_CREATION)
        {
            using namespace boost::posix_time;
            ptime timeStart(microsec_clock::local_time());
            double  timeElapsed=0;
            unsigned int  nPoll=0;

            channelOpenPolicy.flushSendBufferNow();

            ptime timeEnd(microsec_clock::local_time());
            time_duration duration(timeEnd-timeStart);
            timeElapsed= (double) duration.total_microseconds()/1000000.0;

            while ( !(*it_handle).isConnected() && timeElapsed <= channelOpenPolicy.getTimeout())
            {

#if HAVE_BOOST_THREAD
                boost::this_thread::sleep_for(boost::chrono::microseconds(1000));
#else
#if HAVE_LINUX
                usleep(1000); //1ms;
#endif
#endif

                ++nPoll;

                ptime timeEnd(microsec_clock::local_time());
                time_duration duration(timeEnd-timeStart);
                timeElapsed= (double) duration.total_microseconds()/1000000.0;
            }


            //Let's wait a fraction to allow getCallback to complete
#if HAVE_BOOST_THREAD
            boost::this_thread::sleep_for(boost::chrono::microseconds(1000));
#else
#if HAVE_LINUX
            usleep(1000); //1ms;
#endif
#endif
         
            ChannelRequestStatus cre;
            handleHelper.getChannelRequestStatusGet (handle, cre);
            nPoll=0;

            while (cre.getCallbackProgressKind()==(CAFENUM::CallbackProgressKind) CAFENUM::PENDING
              && nPoll<50)
            {

#if HAVE_BOOST_THREAD
                boost::this_thread::sleep_for(boost::chrono::microseconds(2000));
#else
#if HAVE_LINUX
                usleep(2000);
#endif
#endif
                ++nPoll;
                handleHelper.getChannelRequestStatusGet (handle, cre);
            }

	    //std::cout << __FILE__ << ":" << __LINE__ << " nPoll=" << nPoll << std::endl;
            if ((*it_handle).isConnected() && cre.getCallbackProgressKind()==(CAFENUM::CallbackProgressKind) CAFENUM::PENDING)
            {
                std::cout << __FILE__ << ":" << __LINE__ << std::endl;
                std::cout << __METHOD__ << std::endl;
                std::cout << handleHelper.getPVFromHandle(handle) << " with handle " << handle << std::endl;
                std::cout << "Value of 1 is pending, Value of 2 is complete" << std::endl;
                std::cout << "Get Callback PENDING for createChannel?:         " << cre.getCallbackProgressKind()  << std::endl;
            }

        }//if (channelOpenPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_CREATION)
      
    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    return (int) _status;

#undef __METHOD__
};

/**
 *  \brief Creates handle and calls createChannel to establish virtual circuit to epics process variable
 *  \param pv input:  process variable
 *  \param ccc input: ca_client_context
 *  \param handle input: handle
 *  \exception CAFEException_pv if unable to create channel virtual circuit
 *  \return status ICAFE_NORMAL if all OK
 */
int Connect::createHandle(const char * pv, ca_client_context * ccc,  unsigned int &handle)
//throw (CAFEException_pv)
{
#define __METHOD__ "Connect::createHandle()"

    chid pCh = NULL;
    std::pair<cafeConduit_set::iterator, bool> p;
    int _status = ICAFE_NORMAL;

    // CAFEConduit object initialized and placed into hash map
  
    if(MUTEX)
    {
        cafeMutex.lock();   //lock
    }
    
    p = cs.insert(Conduit(pv, ccc,
                          channelRequestPolicyMasterPut, channelRequestPolicyMasterGet,
                          channelGetActionWhenMonitorPolicyMaster, pyCafeFlag));
   
    if(MUTEX)
    {
        cafeMutex.unlock();   //unlock
    }
     
    // The following should not happen as we do a pre-check in Connect::open
    // p.second true insert success
    // p.second false insert failed as pair already exists
    // or handle number exceeded limit in which case we search for a gap
    if (!(p.second))
    {
        cout << __FILE__ << ":" << __LINE__ << endl;
        cout << __METHOD__ <<endl;
        cout << "HANDLE ALREADY IN USE! " << endl;
        cout << "ISSUING NEW HANDLE FOR PV= " << pv << endl;
        cout << "Next free handle is " << handleHelper.getNextFreeHandle() << endl;
        Conduit::handleNext=handleHelper.getNextFreeHandle()-1;
        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        p = cs.insert(Conduit(pv, ccc,
                              channelRequestPolicyMasterPut, channelRequestPolicyMasterGet,
                              channelGetActionWhenMonitorPolicyMaster, pyCafeFlag));
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }
    }

    if (!(p.second))
    {
        cout << __FILE__ << ":" << __LINE__ << endl;
        cout << __METHOD__ <<endl;
        cout << "FAILED AGAIN TO CREATE A HANDLE!!" << endl;
        CAFEException_pv e;
        e = exceptionsHelper.prepareCAFEException_pv((*(p.first)).pv.c_str(), (*(p.first)).pvAlias.c_str(),
                (*(p.first)).getHandle(), (*(p.first)).getChannelID(),
                ECAFE_HASH_UNIQUEID_EXISTS, __METHOD__, __LINE__);
        throw (e);
        return ECAFE_HASH_UNIQUEID_EXISTS;
    }

    handle=(*(p.first)).getHandle();
  
    cafeConduit_set_by_handle &  handle_index= cs.get<by_handle>();
    //cafeConduit_set_by_handle::iterator it_handle;

    //it_handle =handle_index.find(handle);
   
        // 11 March 2019
        // Allow Python Users to fire their own native callback for when using default callbackHandlerCreate
        // Check pyCallbackFlag in ChannelOpenFolicy and set pyCallbackFlag in conduit object accordingly
        //
#if HAVE_PYTHON_H
      bool _pyCallbackFlag=channelCreatePolicy.getPyCallbackFlag();
      void * _pyConnectCallbackFn=channelCreatePolicy.getPyConnectHandler();
      //cout << "========================================================== " << endl;
      //cout << "pycafeflag " << pyCafeFlag << " handle = " << handle << " " << (*(p.first)).pv << endl;
      //cout << " _pyCallbackFlag " << _pyCallbackFlag << " " << _pyConnectCallbackFn << endl;
      //cout <<  "========================================================== " << endl;

        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        handle_index.modify(p.first, change_pyOpenCallbackFlag (_pyCallbackFlag) );

        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }

        //Oct 2020
        if (_pyCallbackFlag == true) {  

	//channelCreatePolicy.getPyHandler and add to cafe.conduit!
       
	//cout <<  "===Adding PyConnect Handle================================================== " << endl;

        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }

        handle_index.modify(p.first, change_pyConnectCallbackFn (_pyConnectCallbackFn) );

        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }

	}       
#endif
    
    // Normal return is ECA_TIMEOUT if pend_io/pend_event is instigated!!
    _status=createChannel(handle, pv, pCh);

  
    // Channel Access will spit out an Invalid String Error if pv not a string!
    // Possible Errors from ca_create_channel: ECA_NORMAL, ECA_BADTYPE, ECA_STRTOBIG, ECA_ALLOCMEM
    // Possible Error from ca_pend_event: ECA_EVDISALLOW
    // IF FAIL then ERASE HANDLE and clear_channel if pCH exists!!

    // IF FAILED THROW EXCEPTION
    
    if (_status == ECA_BADSTR)
    {
        //Cannot Connect::close(handle) in this event else segmentation fault
        CAFEException_pv e;
        e = exceptionsHelper.prepareCAFEException_pv("empty", "empty", (*(p.first)).getHandle(), 
						     pCh, _status, __METHOD__, __LINE__);
        throw (e);
        return _status;
    }
    else if (_status != ECA_NORMAL && _status != ECA_TIMEOUT)
    {
        //Connect::close(handle);
        CAFEException_pv e;
        e = exceptionsHelper.prepareCAFEException_pv((*(p.first)).pv.c_str(), (*(p.first)).pvAlias.c_str(),
                (*(p.first)).getHandle(), pCh, _status,
                __METHOD__, __LINE__);
        throw (e);
        return _status;
    }
    else if (pCh == NULL)
    {    

        //Connect::close(handle); Close channel in createHandle	
        CAFEException_pv e;
        e = exceptionsHelper.prepareCAFEException_pv( (*(p.first)).pv.c_str(), (*(p.first)).pvAlias.c_str(),
                (*(p.first)).getHandle(), pCh, ECAFE_NULLCHID,
                __METHOD__, __LINE__);
	
	//exceptionsHelper.printCAFEException_pv(e);
        throw (e);
        return ECAFE_NULLCHID;
    }

    return _status;

#undef __METHOD__
};


/////////////////////////////////////////////////////////////////////
////// PUBLIC METHODS ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/**
 *  \brief Called once prior to making any channel access calls \n
 *  Specifies that preemptive callback is allowed: ca_enable_preemptive_callback
 *  \exception CAFEException_init ECA_ALLOCMEM if memory space could not be allocated
 *  \return status ECA_NORMAL if OK else throws an exception
 */
int Connect::init() //throw (CAFEException_init)
{
#define __METHOD__ "Connect::init()"

    int status=ICAFE_NORMAL;

    try
    {
        status=init(ca_enable_preemptive_callback);
    }
    catch (CAFEException_init & e)
    {
        cout << __FILE__ << ":" << __LINE__ << endl;
        cout << __METHOD__ << endl;
        cout << e.what() << endl;
        throw e;
    }
    catch (...)
    {
        cout << __FILE__ << ":" << __LINE__ << endl;
        cout << __METHOD__ << endl;
        cout << "UNKNOW EXCEPTION!" << endl;
    }


    return status;

#undef __METHOD__
};

/**
 *  \brief Called once prior to making any channel access calls \n
 *  \param select input: enum ca_preemptive_callback_select \n
 *  {ca_disable_preemptive_callback=0, ca_enable_preemptive_callback=1}
 *  \exception CAFEbad_allocMem ECA_ALLOCMEM if memory space could not be allocated
 *  \return status ECA_NORMAL if OK else else throws an exception
 */
int  Connect::init(ca_preemptive_callback_select select) //throw (CAFEException_init)
{
#define __METHOD__ "Connect::init(ca_preemptive_callback_select)"
    int status=ICAFE_NORMAL;
 
#if HAVE_PYTHON_H
#if HAVE_PYCAFE_EXT
    int pyStatus = PyImport_AppendInittab("PyCafe", PyInit_PyCafe); //Python 3
    if (pyStatus == -1) {
      std::cout <<  "PyCafe Init failed!!" << std::endl;
      exit(1);
    }
    Py_Initialize();
    PyObject *module = PyImport_ImportModule("PyCafe");
    if(module==NULL){
      std::cout <<  "PyCafe import failed!!" << std::endl;
      exit(1);
    } 
    PyInit_PyCafe;
    //PyInit_PyCafe_sf();
    //Py_Finalize();
    //Python 2
    //Py_Initialize();
    //dlopen("libpython2.6.so", RTLD_GLOBAL|RTLD_LAZY);
    //dlopen("/opt/gfa/python-3.5/latest/lib/libpython3.5m.so", RTLD_GLOBAL|RTLD_LAZY);
    //initPyCafe(); //only if PyCafe.h as opposed to PyCafe_api.h
    //PyEval_InitThreads();
    //#else
    //Py_Initialize();
  
#else
    //Py_Initialize();
#endif
#endif

    status = ca_context_create(select);
    if (status != ECA_NORMAL)
    {
        cout << __FILE__ << ":" << __LINE__ << endl;
        cout << __METHOD__ << endl;
        cafeStatus.report(status);
        if (status==ECA_ALLOCMEM)
        {
            CAFEException_init e;
            throw (e);
        }
        else if (status==ECA_NOTTHREADED)
        {
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << endl;
            cout << "WARNING: ECA_NOTTHREADED " << endl;
            cafeStatus.report(status);
        }
        else
        {
            cout << __FILE__ << ":" << __LINE__ << endl;
            cout << __METHOD__ << endl;
            cout << "THE GIVEN RETURN CODE DOES NOT MATCH THE EPICS DOCUMENTATION!" << endl;
            cout << "PLEASE REPORT THE ABOVE STATUS CODE TO THE AUTHOR. Thanks!   " << endl;
        }
    }

    return status;

#undef __METHOD__
};

/**
 *  \brief Establishes whether all callbacks from methodCallbacks.cpp have completed\n
 *  - If yes, nothing further to do, else test again \n
 *  - This method is used for callbacks initiated on creation of channel only\n
 *  \param hArray output: array of reference handles to Conduit objects
 *  \param nHandles input: size ofhArray
 *  \return true if all callbacks completed else false
 */
bool Connect::initCallbackComplete(unsigned int * hArray, unsigned int nHandles)
{
#define __METHOD__ "Connect:initCallbackComplete(unsigned int * hArray, unsigned int nHandle) "

    int status=ICAFE_NORMAL;

    ChannelRequestStatus cre, crc, crs, crn;
    for (unsigned int i=0; i < nHandles; ++i)
    {
        
        if(handleHelper.isChannelConnected(hArray[i]))
        {

            handleHelper.getChannelRequestStatusGetCtrl  (hArray[i], crc);
            handleHelper.getChannelRequestStatusGetSTSACK(hArray[i], crs);
            handleHelper.getChannelRequestStatusGet      (hArray[i], cre);
            handleHelper.getChannelRequestStatusGetClassName      (hArray[i], crn);

            if (crc.getCallbackProgressKind()<=(CAFENUM::CallbackProgressKind) CAFENUM::PENDING ||
                    crs.getCallbackProgressKind()<=(CAFENUM::CallbackProgressKind) CAFENUM::PENDING ||
		cre.getCallbackProgressKind()<=(CAFENUM::CallbackProgressKind) CAFENUM::PENDING ||
		crn.getCallbackProgressKind()<=(CAFENUM::CallbackProgressKind) CAFENUM::PENDING
               )
            {
	      /*
                std::cout << "======================================================================" << std::endl;
                cout << __FILE__ << ":" << __LINE__ << ":" << __METHOD__ << endl;
                cout <<  handleHelper.getPVFromHandle(hArray[i]) << " with handle " << hArray[i] << endl;
                cout << "Value of 1 is pending, Value of 2 is complete" << endl;
                cout << "Callback PENDING for ClassName?:   " << crn.getCallbackProgressKind()  << endl;
                cout << "Callback PENDING for Ctrl?:        " << crc.getCallbackProgressKind()  << endl;
                cout << "Callback PENDING for STSACK?:      " << crs.getCallbackProgressKind()  << endl;
                cout << "Callback PENDING for GET?:         " << cre.getCallbackProgressKind()  << endl;
                std::cout << "======================================================================" << std::endl;
	      */
                return false;
            }



        }
    }
    return true;

#undef __METHOD__
};

/**
 *  \brief Establishes virtual circuits for a vector of epics process variables \n
 *  - Check if handle already exists for given pv/ca_client_context \n
 *  - If yes, nothing further to do, else create channel \n
 *  - Determine connection status and properties of the pv \n
 *  - Create CAFEConduit object and add to multi-index hash table
 *  \param pvArray input: array of process variables (PVNAME_SIZE)
 *  \param handleArray output: array of reference handle to Conduit objects
 *  \param nHandles input: size of pvArray
 *  \exception CAFEException_open if unable to create channel virtual circuit
 *  \return status ICAFE_NORMAL if OK
 */
int Connect::open(const char ** pvArray, unsigned int * handleArray, const unsigned int nHandles)
//throw (CAFEException_open)
{
#define __METHOD__ "Connect::open(const char **, unsigned int *, const unsigned int)"
    int status=ICAFE_NORMAL;
    bool flushAtEndFlag=false;

    if (channelOpenPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_CREATION)
    {
        channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
        flushAtEndFlag=true;
    }

    for (unsigned int  i=0;  i < nHandles; ++i)
    {
        try
        {
            status=open(pvArray[i], handleArray[i]);
        }
        catch (CAFEException_open &e)
        {
            if (flushAtEndFlag)
            {
                //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
                Connect::openNow();
            }
            throw(e);
            return status;
        }
    } //for

    if (flushAtEndFlag)
    {
        //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
        //Connect::openNow();
        //14 March 2019
        Connect::openNowHandles(handleArray, nHandles);
    }

    return ICAFE_NORMAL;

#undef __METHOD__
};

/**
 *  \brief Establishes virtual circuits for a vector of epics process variables \n
 *  - Check if handle already exists for given pv/ca_client_context \n
 *  - If yes, nothing further to do, else create channel \n
 *  - Determine connection status and properties of the pv \n
 *  - Create CAFEConduit object and add to multi-index hash table \n
 *
 *  \param pvArrayS input: array of process variables givenas "strings"
 *  \param handleArray output: array of reference handle to Conduit objects
 *  \param nHandles input: size of pvArray
 *  \exception CAFEException_open if unable to create channel virtual circuit
 *  \return status ICAFE_NORMAL if OK
 */
int Connect::open(const std::string *pvArrayS, unsigned int *handleArray, const unsigned int nHandles)
//throw (CAFEException_open)
{
#define __METHOD__ "Connect::open(const std::string *, unsigned int *, unsigned int)"
    int status=ICAFE_NORMAL;
    bool flushAtEndFlag=false;
    if (channelOpenPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_CREATION)
    {
        channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
        flushAtEndFlag=true;
    }

    for (unsigned int i=0; i<nHandles; ++i)
    {
        try
        {
            status=open(pvArrayS[i].c_str(), handleArray[i]);
        }
        catch (CAFEException_open &e)
        {
            if (flushAtEndFlag)
            {
                // this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
                Connect::openNow();
            }
            throw(e);
            return status;
        }
    }//for

    if (flushAtEndFlag)
    {
        //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
        //Connect::openNow();
        //14 March 2019
        Connect::openNowHandles(handleArray, nHandles);
    }

    return ICAFE_NORMAL;

#undef __METHOD__

};


/**
 *  \brief Establishes virtual circuits for a vector of epics process variables \n
 *  - Check if handle already exists for given pv/ca_client_context \n
 *  - If yes, nothing further to do, else create channel \n
 *  - Determine connection status and properties of the pv \n
 *  - Create CAFEConduit object and add to multi-index hash table \n
 *
 *  \param pvV input: vector of process variables (string)
 *  \param handleV output: vector of reference handle to Conduit objects
 *  \exception CAFEException_open if unable to create channel virtual circuit
 *  \return status ICAFE_NORMAL if OK
 */
int  Connect::open(std::vector<std::string> pvV, std::vector<unsigned int> &handleV)
//throw (CAFEException_open)
{
#define __METHOD__ "Connect::open(vector<string>, vector<unsigned int> &)"
    int status=ICAFE_NORMAL;
    unsigned int  _h=0;
    bool flushAtEndFlag=false;
    handleV.clear();
    handleV.reserve(pvV.size());

    if (channelOpenPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_CREATION)
    {
        channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
        flushAtEndFlag=true;
    }

    for (unsigned int i=0;  i<pvV.size(); ++i)
    {
        try
        {
            status=open(pvV[i].c_str(),_h);
            handleV.push_back(_h);
        }
        catch (CAFEException_open &e)
        {
            if (flushAtEndFlag)
            {
                // this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
                Connect::openNow();
            }
            throw(e);
            return status;
        }
    } //for

    if (flushAtEndFlag)
    {
        //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
        //Connect::openNow();
        //14 March 2019
        Connect::openNowHandleV(handleV);
    }

    return ICAFE_NORMAL;

#undef __METHOD__
};

/**
 *  \brief Establishes virtual circuits for a vector of epics process variables \n
 *  - Check if handle already exists for given pv/ca_client_context \n
 *  - If yes, nothing further to do, else create channel \n
 *  - Determine connection status and properties of the pv \n
 *  - Create CAFEConduit object and add to multi-index hash table
 *  \param pvV input: vector of process variables (char *)
 *  \param handleV output: vector of reference handle to Conduit objects
 *  \exception CAFEException_open if unable to create channel virtual circuit
 *  \return status ICAFE_NORMAL if OK
 */
int Connect::open(std::vector<const char *> pvV, std::vector<unsigned int> &handleV)
//throw (CAFEException_open)
{
#define __METHOD__ "Connect::open(vector<char *>, vector<unsigned int> &)"
    int status=ICAFE_NORMAL;
    unsigned int _h=0;
    bool flushAtEndFlag=false;
    handleV.clear();
    handleV.reserve(pvV.size());

    if (channelOpenPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_CREATION)
    {
        channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
        flushAtEndFlag=true;
    }

    for (unsigned int i=0;  i<pvV.size(); ++i)
    {
        try
        {
            status=open(pvV[i],_h);
            handleV.push_back(_h);
        }
        catch (CAFEException_open &e)
        {
            if (flushAtEndFlag)
            {
                //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
                Connect::openNow();
            }
            throw(e);
            return status;
        }
    }

    if (flushAtEndFlag)
    {
        //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
        //Connect::openNow();
        //14 March 2019
        Connect::openNowHandleV(handleV);
    }

    return ICAFE_NORMAL;

#undef __METHOD__
};

/**
 *  \brief Establishes virtual circuit to epics process variable and defines its pv alias name \n
 *  - Check if handle already exists for given pv/ca_client_context  \n
 *  - If yes, nothing further to do, else create channel  \n
 *  - Determine connection status and properties of the pv  \n
 *  - Create Conduit object and add to multi-index hash table  \n
 *  \param _pv input: process variable
 *  \param _pvAlias input: process variable alias
 *  \param handle output: reference handle to Conduit object
 *  \exception CAFEException_pvOpen if unable to create channel virtual circuit
 *  \return status ICAFE_NORMAL if OK
 */
int Connect::open(const char * _pv, const char * _pvAlias, unsigned int  & handle)
//throw (CAFEException_open)
{
#define __METHOD__ "Connect::open(const char * pv, const char * _pvAlias, unsigned int  & _handle)"
    int status=ICAFE_NORMAL;
    try
    {
        status = open (_pv, handle);
    }
    catch (CAFEException_open &e)
    {
        throw(e);
        return status;
    }

    if (handle != 0)
    {
        try
        {
            status=setPVAlias(handle,_pvAlias);
        }
        catch (CAFEException_open & badpv)
        {
            throw(badpv);
            return status;
        }
    }

    return status;

#undef __METHOD__
};

/**
 *  \brief Establishes virtual circuit to epics process variable  \n
 *  - Check if handle already exists for given pv/ca_client_context  \n
 *  - If yes, nothing further to do, else create channel  \n
 *  - Determine connection status and properties of the pv  \n
 *  - Create Conduit object and add to multi-index hash table  \n
 *  \param _pv input: process variable
 *  \param handle output: reference handle to Conduit object
 *  \exception CAFEException_pvOpen if unable to create channel virtual circuit
 *  \return status ICAFE_NORMAL if OK
 */
int Connect::open(const char * _pv, unsigned int &handle)
//throw (CAFEException_open)
{
#define __METHOD__ "Connect::open(const char * _pv, unsigned int &handle)"
    int status=ICAFE_NORMAL;
    char pv[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(_pv, pv);

    // Determine ca-client context
    ca_client_context * ccc = ca_current_context();

    if (ccc == NULL)
    {
        try
        {
            status=init( ); // ca_disable_preemptive_callback
        }
        catch(CAFEException_init)
        {
            //status will be ECA_ALLOCMEM
            CAFEException_pv e;
            e = exceptionsHelper.prepareCAFEException_pv(pv, pv, 0, NULL,
                    ECA_ALLOCMEM,__METHOD__, __LINE__);
	    std::string ewhat ="CAFEException_open," + std::string(e.pv)  + ","  + std::to_string(0) \
	      + "," + std::to_string(ECA_ALLOCMEM) + "," +  cafeStatus.csc.message(ECA_ALLOCMEM) \
              + "," + cafeStatus.csi.message(ECA_ALLOCMEM); 
            CAFEException_open badpv(ewhat);
            badpv.pvEx=e;
            throw(badpv);
            return status;
        }
        ccc = ca_current_context();
    }

    // Check if handle already exists for this pv
    // Handles for pvs that are members of a group are treated separately!

    handle = handleHelper.getHandleFromPV(pv,ccc);

    if (handle != 0)
    {
        return ICAFE_NORMAL;
    }

    try
    {
        
        status = createHandle(pv, ccc, handle);
       
    }
    catch (CAFEException_pv & e)
    {    
         std::cout << __METHOD__ << " EXCEPTION "  << std::endl;

        if (e.statusCode != ECA_BADSTR)
	{ 
	    //ca_flush_io(); // Better Flush or not. Seems that there is not a need.
              
#if HAVE_BOOST_THREAD
            boost::this_thread::sleep_for(boost::chrono::microseconds(2000));
#else
#if HAVE_LINUX
            usleep(2000);
#endif
#endif
	    close(handle);
	}
       
	std::string ewhat ="CAFEException_open," + std::string(e.pv)  + ","  + std::to_string(e.handle) +  "," \
            + std::to_string(e.statusCode) + "," + e.statusCodeText + "," + e.statusMessage; 
		
	CAFEException_open badpv(ewhat);
        badpv.pvEx=e;

        throw(badpv);
        return status;
    }

    return ICAFE_NORMAL;

#undef __METHOD__
};


/**
 *  \brief Defines a pv alias for pv and adds this to multi-index hash table
 *  \param handle input: handle
 *  \param _pvAlias input: process variable alias
 *  \exception CAFEException_pvOopen if unable to set an alias
 *  \return status ICAFE_NORMAL if OK
 */
int Connect::setPVAlias(unsigned int handle, const char * _pvAlias) //throw(CAFEException_open)
{
#define __METHOD__ "Connect::setPVAlias(unsigned int  * _handle, const char * _pv)"
    int status=ICAFE_NORMAL;
    Helper helper;
    char pvAlias[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(_pvAlias, pvAlias);
    _pvAlias=pvAlias;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {
        ca_client_context * ccc = ca_current_context();
        //Need to make sure that Alias is NOT already in use as a PVName for another handle (would be strange!)
        unsigned int   pvHandle=handleHelper.getHandleFromPV(pvAlias,  ccc);

        if (pvHandle > 0 &&  pvHandle != handle)
        {
            CAFEException_pv e;
            e = exceptionsHelper.prepareCAFEException_pv((*it_handle).pv.c_str(), _pvAlias,
                    (*it_handle).getHandle(), (*it_handle).getChannelID(),
                    ECAFE_PVALIAS_INVALID,__METHOD__, __LINE__);

	    std::string ewhat ="CAFEException_open," + std::string(e.pv)  + ","  + std::to_string(e.handle) +  "," \
            + std::to_string(e.statusCode) + "," + e.statusCodeText + "," + e.statusMessage; 
            CAFEException_open badpv(ewhat);
	    

            badpv.pvEx=e;
            throw(badpv);
            return ECAFE_PVALIAS_PREV_DEF;
        }

        pvHandle=handleHelper.getHandleFromPVAlias(pvAlias,  ccc);

        //Need to make sure that Alias is NOT already in use
        if ( pvHandle > 0 && pvHandle != handle)
        {
            CAFEException_pv e;
            e = exceptionsHelper.prepareCAFEException_pv((*it_handle).pv.c_str(), _pvAlias,
                    (*it_handle).getHandle(), (*it_handle).getChannelID(),
                    ECAFE_PVALIAS_PREV_DEF,__METHOD__, __LINE__);

	    std::string ewhat ="CAFEException_open," + std::string(e.pv)  + ","  + std::to_string(e.handle) +  "," \
            + std::to_string(e.statusCode) + "," + e.statusCodeText + "," + e.statusMessage; 
            
	    CAFEException_open badpv(ewhat);
	    
            badpv.pvEx=e;
            throw(badpv);
            return   ECAFE_PVALIAS_PREV_DEF;
        }

        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        handle_index.modify(it_handle, change_pvAlias( _pvAlias));
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }

    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;

#undef __METHOD__
};






/**
 *  \brief Closes connection for channel within a ca_client_context and deletes corresponding handle \n
 *  The handle must be within the same context. \n
 *  Note that this does NOT cause the channel's disconnect handler to be called. \n
 *  It does however invoke event handlers for subscriptions (i.e., monitors). \n
 *  The Conduit handle is erased
 *  \param  handleArray input: array of handles
 *  \param  nHandles input: size of array
 *  \return ECA_NORMAL if all OK else first encountered error
 */
int Connect::closeChannels(unsigned int * handleArray, unsigned int nHandles)
{
#define __METHOD__ "Connect::closeChannels(unsigned int * handleArray, unsigned int nHandles)"
    
    ca_client_context * cctLocal=  ca_current_context();
    if (cctLocal == NULL )
    {
        // We do not know if this channel belongs to this context!
        return ECAFE_NULLCONTEXT;
    }

    //copy array to vector and remove duplicate handles
    vector<unsigned int> vec;
    vec.reserve(nHandles);
    vec.insert(vec.end(), &handleArray[0], &handleArray[nHandles]);
    //Remove duplicate values
    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end());

    int  statusGroup = ECA_NORMAL;
    int  status      = ICAFE_NORMAL;
    bool   isClearChannel   =false;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    for (unsigned int i=0; i<vec.size(); ++i)
    {
        it_handle = handle_index.find(vec[i]);
        if (it_handle != handle_index.end())
        {
            if ((*it_handle).getClientContext() != cctLocal )
            {
                continue;
            }
            // Only clear channel if its ca_client_context is valid
            if ((*it_handle).getChannelID() != NULL)
            {
                status = ca_clear_channel((*it_handle).getChannelID());
                if (status != ECA_NORMAL)
                {
                    statusGroup = status;
                }
                isClearChannel=true;
            }
        }
    }//for

    // Even if Null Context remove handle. This could happen if a ca_context_destroy was called
    if (isClearChannel)
    {
        status = ca_pend_io(channelClosePolicy.getTimeout());
        if (status != ECA_NORMAL)
        {
            statusGroup = status;
        }
    }

    // Loop through and identify valid elements and clear their databuffers
    // i.e., only for those channels within the ca_client_context

    for (unsigned int i=0; i<vec.size(); ++i)
    {
        it_handle = handle_index.find(vec[i]);
        if (it_handle != handle_index.end())
        {
            // Only clear channel if its ca_client_context is valid
            if ((*it_handle).getChannelID() != NULL && (*it_handle).getClientContext() == cctLocal )
            {
                if(MUTEX)
                {
                    cafeMutex.lock();
                }
                handle_index.modify(it_handle, free_dataBuffers());
                if(MUTEX)
                {
                    cafeMutex.unlock();
                }
            }
            cs.erase(it_handle); //Moved to within loop; fix of May 2017
        }
    }

    return statusGroup;

#undef __METHOD__
};

/**
 *  \brief Closes connection for channel within a ca_client_context and deletes corresponding handle \n
 *  The handle must be within the same context. \n
 *  Note that this does NOT cause the channel's disconnect handler to be called. \n
 *  It does however invoke event handlers for subscriptions (i.e., monitors). \n
 *  The Conduit handle is erased
 *  \param  handle input
 *  \return ECA_NORMAL if all OK
 */
int Connect::close(unsigned int handle)
{
#define __METHOD__ "Connect::close(unsigned int handle)"
    
    ca_client_context * cctLocal=  ca_current_context();
    if (cctLocal == NULL )
    {
        // We do not know if this channel belongs to this context!
        return ECAFE_NULLCONTEXT;
    }
    int status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {

  

        if ((*it_handle).getClientContext() == NULL )
        {
            //Channel already forcefully cleared
            cs.erase(it_handle);
            return ECAFE_NULLCONTEXT;
        }
        if ((*it_handle).getClientContext() != cctLocal )
        {
            return ECAFE_WRONG_CA_CONTEXT;
        }
        if ((*it_handle).getChannelID() != NULL)
        {
           
  
    //This hangs when pCH is Null
    
            status = ca_clear_channel((*it_handle).getChannelID());

            if (status != ECA_NORMAL)
            {
                return status; //ECA_BADCHID
            }
    
   
            status = ca_pend_io(channelClosePolicy.getTimeout());
   
  
            if(MUTEX)
            {
                cafeMutex.lock();
            }
            handle_index.modify(it_handle, free_dataBuffers());
            if(MUTEX)
            {
                cafeMutex.unlock();
            }
        }
        cs.erase(it_handle);
        return status;
    }
    else
    {
        // Loop through all elements
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
        {
            if ((*itcs).handle == handle)
            {
                if ((*itcs).getClientContext() == NULL )
                {
                    //Channel already forcefully cleared
                    cs.erase(itcs);
                    return ECAFE_NULLCONTEXT;
                }
                if ((*itcs).getClientContext() != cctLocal )
                {
                    return ECAFE_WRONG_CA_CONTEXT;
                }
                if ((*itcs).getChannelID() != NULL)
                {
                    status = ca_clear_channel((*itcs).getChannelID());
                    if (status != ECA_NORMAL)
                    {
                        return status; //ECA_BADCHID
                    }
                    status = ca_pend_io(channelClosePolicy.getTimeout());
                    if(MUTEX)
                    {
                        cafeMutex.lock();
                    }
                    handle_index.modify(itcs, free_dataBuffers());
                    if(MUTEX)
                    {
                        cafeMutex.unlock();
                    }
                }
                cs.erase(itcs);
                return status;
            }
        }//for
        return ECAFE_INVALID_HANDLE;
    }//else

    //return status; // This line is not reached
#undef __METHOD__
}

/**
 *  \brief Closes all channel connections within a ca_client_context and removes the corresponding handles.  \n
 *  Note that this does NOT cause the channel's disconnect handler to be called.  \n
 *  It does however invoke event subscriptions (for monitors).   \n
 *  Corresponding Conduit handles are erased.
 *  \return ECA_NORMAL if all OK; error if one or more channelIDs fail to close
 */
int  Connect::closeChannels()
{
#define __METHOD__ "Connect::closeChannels()"
    ca_client_context * cctLocal=  ca_current_context();
    if (cctLocal == NULL )
    {
        return ECAFE_NULLCONTEXT;
    }
    return Connect::closeChannels(cctLocal);

#undef __METHOD__
}

/**
 *  \brief Closes all channel connections within the given ca_client_context and
 *  removes the corresponding handles.  \n
 *  Note that this does NOT cause the channel's disconnect handler to be called.  \n
 *  It does however invoke event subscriptions (for monitors).  \n
 *  Corresponding Conduit handles are erased.
 *  \return ECA_NORMAL if all OK; error if one or more channelIDs fail to close
 */
int Connect::closeChannels(ca_client_context * cctLocal)
{
#define __METHOD__ "Connect::closeChannelsca_client_context * cctLocal)"

    if (cctLocal == NULL )
    {
        return ECAFE_NULLCONTEXT;
    }
    int status = ICAFE_NORMAL;
    int statusGroup = ICAFE_NORMAL;
    bool isClearChannel=false;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        if ((*itcs).getClientContext() != cctLocal )
        {
            continue;
        }
        // Only clear channel if its ca_client_context is valid
        if ((*itcs).getChannelID() != NULL)
        {
            status = ca_clear_channel((*itcs).getChannelID());
            if (status != ECA_NORMAL)
            {
                statusGroup = status;
            }
            isClearChannel=true;
        }
    }//for

    // Even if Null Context remove handle. This could happen if a ca_context_destroy was called
    if (isClearChannel)
    {
        status = ca_pend_io(channelClosePolicy.getTimeout());
        if (status != ECA_NORMAL)
        {
            statusGroup = status;
        }
        cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();

        // Loop through and identify valid elements and clear their databuffers
        // i.e. only for those channels within the ca_client_context
        // Removing elements in maps and multimaps p.205 Josuttis
        // Calling erase() for the element to which one is referring to
        // with itcs invalidates itcs as an iterator of cs. Having ++itcs
        // in the for loop line results in undefined behaviour!
        // The following is the correct way to remove elements to which
        // an iterator refers. Note that itcs++ increments itcs so that
        // it refers to the next element but yields a copy of its original value.
        // Thus itcs does not refer to the element that is removed when erase()
        // is called. Hurrah!

        //cout << __FILE__ << ":" << __LINE__ << endl;
        //cout << __METHOD__ << endl;
        //cout << "--------------------------------------------------------------------" << endl;
        //cout << "CAFE HAS CLOSED ALL CHANNELS WITHIN THE CA_CLIENT_CONTEXT: " << ca_current_context()  << endl;
        //cout << "AND IS ERASING THEIR RESPECTIVE HANDLES:" << endl;
        //cout << "--------------------------------------------------------------------" << endl;

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        for (itcs = cs.begin(); itcs != cs.end(); )
        {
            if ((*itcs).getChannelID() != NULL && ((*itcs).getClientContext() == cctLocal))
            {
                // good test of memory handling
                handle_index.modify(itcs, free_dataBuffers());
                cs.erase(itcs++);
            }
            else
            {
                ++itcs;
            }
        }//for
        if(MUTEX)
        {
            cafeMutex.unlock();
        }
    } //if (isClearChannel)
    return statusGroup;

#undef __METHOD__
};

/**
  *  \brief Closes channel connections (even if in other thread) and deletes handle.  \n
  *  Note that this does NOT cause the channel's disconnect handler to be called.  \n
  *  It does however invoke event handlers for subscriptions (i.e. monitors).  \n
  *  The Conduit handle is erased
  *  \param  handle input
  *  \return ECA_NORMAL if all OK
  */
int  Connect::closeHandle(unsigned int handle)
{
#define __METHOD__ "Connect::closeHandle(unsigned int handle)"
    int status = ICAFE_NORMAL;
    //We can close handle irrespective of ca_current_context!
    ca_client_context * cctLocal=  ca_current_context();
    status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(handle);
    if (it_handle != handle_index.end())
    {
        if ((*it_handle).getChannelID() != NULL &&  cctLocal != NULL )
        {
            status = ca_clear_channel((*it_handle).getChannelID());
            if (status != ECA_NORMAL)
            {
                return status; //ECA_BADCHID
            }
            status = ca_pend_io(channelClosePolicy.getTimeout()); //See channelClosePolicy.setPolicy in connect.h
            //    channelClosePolicy.flushSendBufferNow();
            if(MUTEX)
            {
                cafeMutex.lock();
            }
            handle_index.modify(it_handle, free_dataBuffers());
            if(MUTEX)
            {
                cafeMutex.unlock();
            }
        }
        cs.erase(it_handle);
        return status;
    }
    else
    {
        // Loop through all elements
        for (itcs = cs.begin(); itcs != cs.end();)
        {
            if ((*itcs).handle == handle)
            {
                if ((*itcs).getChannelID() != NULL &&  cctLocal != NULL)
                {
                    status = ca_clear_channel((*itcs).getChannelID());
                    if (status != ECA_NORMAL)
                    {
                        return status; //ECA_BADCHID
                    }
                    status = ca_pend_io(channelClosePolicy.getTimeout());
                    if(MUTEX)
                    {
                        cafeMutex.lock();
                    }
                    handle_index.modify(itcs, free_dataBuffers());
                    if(MUTEX)
                    {
                        cafeMutex.unlock();
                    }
                }
                cs.erase(itcs++);
                return status;
            }
            else
            {
                ++itcs;
            }
        }//for
        return ECAFE_INVALID_HANDLE;
    }
    //return status; // This line is not reached

#undef __METHOD__
};


/**
 *  \brief Closes given channel connections (even if in other threads) and deletes their handles.  \n
 *  Note that this does NOT cause the channel's disconnect handler to be called.  \n
 *  It does however invoke event subscriptions (for monitors).  \n
 *  All corresponding Conduit handles are erased.
 *  \param handleArray input: unsigned int *
 *  \param nHandles input: unsigned int
 *  \return ECA_NORMAL if all OK; error if one or more channelIDs fail to close
 */
int Connect::closeHandles(unsigned int * handleArray, unsigned int nHandles)
{
#define __METHOD__ "Connect::closeHandles(unsigned int *, unsigned int) "

    //copy array to vector and remove dupliacte handles
    vector<unsigned int> vec;
    vec.reserve(nHandles);
    vec.insert(vec.end(), &handleArray[0], &handleArray[nHandles]);
    //Remove duplicate values
    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end() );

    int  statusGroup = ECA_NORMAL;
    int  status      = ICAFE_NORMAL;
    bool   isClearChannel = false;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    if (ca_current_context() != NULL)
    {
        for (unsigned int i=0; i<vec.size(); ++i)
        {
            it_handle = handle_index.find(vec[i]);
            if (it_handle != handle_index.end())
            {
                if ((*it_handle).getChannelID() != NULL  )
                {
		  //std::cout << ca_current_context() << (*it_handle).getClientContext() << std::endl;
		  //std::cout << (*it_handle).getHandle()  <<  (*it_handle).getPV()  << std::endl;
                    status = ca_clear_channel((*it_handle).getChannelID());
		    //ca_pend_io(0.1);
		    //std::cout << i <<  " of " << vec.size() << std::endl;
                    if (status != ECA_NORMAL)
                    {
                        statusGroup=status; //ECA_BADCHID
                    }
                }
                isClearChannel=true;
            }
            else
            {
                // Loop through all elements
                for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
                {
                    if ((*itcs).getHandle()==vec[i])
                    {
                        if ((*itcs).getChannelID() != NULL )
                        {
                            status = ca_clear_channel((*itcs).getChannelID());
                            if (status != ECA_NORMAL)
                            {
                                statusGroup = status;
                            }
                            isClearChannel=true;
                        }
                        break;
                    }
                } //for
            }// else
        } //for
    }//if ca_current_context != NULL

    if (isClearChannel)
    {
        status = ca_pend_io(channelClosePolicy.getTimeout());
        if (statusGroup == ECA_NORMAL)
        {
            statusGroup = status;
        }
    }

    for (unsigned int i=0; i<vec.size(); ++i)
    {
        it_handle = handle_index.find(vec[i]);
        if (it_handle != handle_index.end())
        {
            if(MUTEX)
            {
                cafeMutex.lock();
            }
            handle_index.modify(it_handle, free_dataBuffers());
            if(MUTEX)
            {
                cafeMutex.unlock();
            }
            cs.erase(it_handle);
        }
        else
        {
            // Loop through all elements
            for (itcs = cs.begin(); itcs != cs.end();)
            {
                if ((*itcs).handle == vec[i])
                {
                    if(MUTEX)
                    {
                        cafeMutex.lock();
                    }
                    handle_index.modify(itcs, free_dataBuffers());
                    if(MUTEX)
                    {
                        cafeMutex.unlock();
                    }
                    cs.erase(itcs++);
                    break;
                }
                else
                {
                    ++itcs;
                }
            }
        }
    }//for

    return statusGroup;

#undef __METHOD__
};

/**
 *  \brief Closes all channel connections (even if in other threads) and deletes all handles.  \n
 *  Note that this does NOT cause the channel's disconnect handler to be called.  \n
 *  It does however invoke event subscriptions (for monitors).  \n
 *  All Conduit handles are erased.
 *  \return ECA_NORMAL if all OK; error if one or more channelIDs fail to close
 */
int Connect::closeHandles()
{
#define __METHOD__ "Connect::closeHandles()"
    int  status      = ICAFE_NORMAL;
    int  statusGroup = ECA_NORMAL;
    bool   isClearChannel   =false;
    bool   isCS             =false;

    if (ca_current_context() != NULL)
    {
        // Loop through all elements
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
        {
	    //std::cout << "(*itcs).getHandle() " << (*itcs).getHandle() << std::endl;

            if ((*itcs).getChannelID() != NULL)
            {
	        //std::cout << "(*itcs).getHandle() " << (*itcs).getHandle() << std::endl;
                status = ca_clear_channel((*itcs).getChannelID());
	        //std::cout << "Cleared " << std::endl;
		ca_flush_io();
                if (status != ECA_NORMAL)
                {
                    statusGroup = status;
                }
                isClearChannel=true;
            }
            isCS=true;
        }
    }

    if (isClearChannel)
    {
        status = ca_pend_io(channelClosePolicy.getTimeout());
        if (statusGroup == ECA_NORMAL)
        {
            statusGroup = status;
        }
    }

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();

    // Loop through all elements and clear databuffers
    if(MUTEX)
    {
        cafeMutex.lock();
    }
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        handle_index.modify(itcs, free_dataBuffers());
    }
    if(MUTEX)
    {
        cafeMutex.unlock();
    }

    if (isCS)
    {
        cs.clear();
    }

    return statusGroup;

#undef __METHOD__
};

/**
 *  \brief Closes channel connection (even if in other thread) but does not delete handle.  \n
 *  Note that this does NOT cause the channel's disconnect handler to be called.  \n
 *  It also does not invoke event handlers for subscriptions (i.e., monitors).  \n
 *  The Conduit handle is NOT erased
 *  \param  handle input
 *  \return ECA_NORMAL if all OK else error
 */
int Connect::closeChannelKeepHandle(unsigned int handle)
{
#define __METHOD__ "Connect::closeChannelKeepHandle(unsigned int handle)"

    int status=ICAFE_NORMAL;
    //We can close handle irrespective of ca_current_context!
    ca_client_context * cctLocal=  ca_current_context();
    ChannelRegalia channelRegalia;
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    bool statusFlag=false;

    it_handle = handle_index.find(handle);
    if (it_handle != handle_index.end())
    {
        if ((*it_handle).getChannelID() != NULL &&  cctLocal != NULL )
        {
	    Connect::monitorStop(handle);

	      
            status = ca_clear_channel((*it_handle).getChannelID());
             
	   
            if (status != ECA_NORMAL)
            {
                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                handle_index.modify(it_handle, change_status (status));
                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }
                return status; //ECA_BADCHID
            }
            
            channelRegalia=(*it_handle).getChannelRegalia();
           
         
            channelClosePolicy.flushSendBufferNow();
            statusFlag=true;
        }
    }
    else
    {
        // Loop through all elements
        for (itcs = cs.begin(); itcs != cs.end();)
        {
            if ((*itcs).handle == handle)
            {
                if ((*itcs).getChannelID() != NULL &&  cctLocal != NULL)
                {
                    Connect::monitorStop(handle);
                    status = ca_clear_channel((*itcs).getChannelID());
		    
                    if (status != ECA_NORMAL)
                    {
                        if(MUTEX)
                        {
                            cafeMutex.lock();   //lock
                        }
                        handle_index.modify(it_handle, change_status (status) );
                        if(MUTEX)
                        {
                            cafeMutex.unlock();   //unlock
                        }
                        return status; //ECA_BADCHID
                    }
                    channelRegalia=(*it_handle).getChannelRegalia();
                   
                    channelClosePolicy.flushSendBufferNow();
                }
                statusFlag=true;
                break;
            }
            else
            {
                ++itcs;
            }
        }
        return ECAFE_INVALID_HANDLE;
    }

    if (statusFlag)
    {
        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        handle_index.modify(it_handle, change_status (ICAFE_CS_CLOSED));
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }

        //Also callback done
        ChannelRequestStatus channelRequestStatusGet;
        ChannelRequestStatus channelRequestStatusPut;
        channelRequestStatusGet.setCallbackKind(false, true); //mimic completion
        channelRequestStatusPut.setCallbackKind(false, true); //mimic completion
        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        handle_index.modify(it_handle, change_channelRequestStatusGet (channelRequestStatusGet));
        handle_index.modify(it_handle, change_channelRequestStatusPut (channelRequestStatusPut));
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }
        channelRegalia.setCafeConnectionState(ICAFE_CS_CLOSED);
        channelRegalia.setConnectFlag(false);
        channelRegalia.setConnectionState(ICAFE_CA_OP_CONN_DOWN);
        channelRegalia.channelID=NULL;
        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        handle_index.modify(it_handle, change_channelRegalia (channelRegalia));
        //struct connection_handler_args _args = (*it_handle).getConnectionHandlerArgs();
        //ChannelCreatePolicy::callbackHandlerCreate(_args);

#if HAVE_PYTHON_H	
        (*it_handle).PyConnectHandler();
#endif

        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }
    }
    return status;

#undef __METHOD__
};

/**
  *  \brief Closes given channel connections (even if in other threads) but does not delete their handles.  \n
  *  Note that this does NOT cause the channel's disconnect handler to be called.  \n
  *  It also does not invoke event handlers for subscriptions (i.e., monitors).  \n
  *  All corresponding Conduit handles are not erased.
  *  \param handleArray input: unsigned int *
  *  \param nHandles input: unsigned int
  *  \return ECA_NORMAL if all OK; error if one or more channelIDs fail to close
  */
int Connect::closeChannelsKeepHandles(unsigned int * handleArray, unsigned int nHandles)
{
#define __METHOD__ "Connect::closeChannelsKeepHandles(unsigned int *, unsigned int) "

    //std::cout << __FILE__ << "//" << __METHOD__ << std::endl;

    //copy array to vector and remove duplicate handles
    vector<unsigned int> vec;
    vec.reserve(nHandles);
    vec.insert(vec.end(), &handleArray[0], &handleArray[nHandles]);
    //Remove duplicate values
    sort(vec.begin(), vec.end());
    vec.erase( unique( vec.begin(), vec.end() ), vec.end() );

    int  statusGroup = ECA_NORMAL;
    int  status      = ICAFE_NORMAL;
    bool isClearChannel = false;
    ChannelRegalia channelRegalia;
    vector<bool>   isClearChannelV;
    isClearChannelV.reserve(vec.size());

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    if (ca_current_context() != NULL)
    {
        for (unsigned int i=0; i<vec.size(); ++i)
        {
            isClearChannelV.push_back(false);
            it_handle = handle_index.find(vec[i]);
            if (it_handle != handle_index.end())
            {
                if ((*it_handle).getChannelID() != NULL)
                {
                    Connect::monitorStop(vec[i]);
		   
		   
                    status = ca_clear_channel((*it_handle).getChannelID());
                    
                    if (status != ECA_NORMAL)
                    {
                        statusGroup=status; //ECA_BADCHID
                    }
                }
                isClearChannelV[i]=true;
                isClearChannel=true;
            }
            else
            {
                // Loop through all elements
                for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
                {
                    if ((*itcs).getHandle()==vec[i])
                    {
                        if ((*itcs).getChannelID() != NULL )
                        {
                            Connect::monitorStop(vec[i]);
                            status = ca_clear_channel((*itcs).getChannelID());
                            if (status != ECA_NORMAL)
                            {
                                statusGroup = status;
                            }
                            isClearChannelV[i]=true;
                            isClearChannel=true;
                        }
                        break;
                    }
                }//for
            }// else
        } //for
    }//if ca_current_context != NULL
 
     
    if (isClearChannel)
    {
        channelClosePolicy.flushSendBufferNow();
        for (unsigned int i=0; i<vec.size(); ++i)
        {
            it_handle = handle_index.find(vec[i]);
            if (it_handle != handle_index.end() &&  isClearChannelV[i])
            {
                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                handle_index.modify(it_handle, change_status (ICAFE_CS_CLOSED) );
                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }

                //Also callback done
                ChannelRequestStatus channelRequestStatusGet;
                ChannelRequestStatus channelRequestStatusPut;
                channelRequestStatusGet.setCallbackKind(false, true); //mimic completion
                channelRequestStatusPut.setCallbackKind(false, true); //mimic completion
                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                handle_index.modify(it_handle, change_channelRequestStatusGet (channelRequestStatusGet) );
                handle_index.modify(it_handle, change_channelRequestStatusPut (channelRequestStatusPut) );
                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }
                channelRegalia=(*it_handle).getChannelRegalia();
                channelRegalia.setCafeConnectionState(ICAFE_CS_CLOSED);
                channelRegalia.setConnectFlag(false);
                channelRegalia.setConnectionState(ICAFE_CA_OP_CONN_DOWN);
                channelRegalia.channelID=NULL;
                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                handle_index.modify(it_handle, change_channelRegalia (channelRegalia) );
		 if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }
               
               #if HAVE_PYTHON_H	
                  (*it_handle).PyConnectHandler();
               #endif
             
             
               
            }
            else
            {
                // Loop through all elements
                for (itcs = cs.begin(); itcs != cs.end();)
                {
                    if ((*itcs).handle == vec[i]  && isClearChannelV[i])
                    {
                        if(MUTEX)
                        {
                            cafeMutex.lock();   //lock
                        }
                        handle_index.modify(itcs, change_status (ICAFE_CS_CLOSED) );
                        if(MUTEX)
                        {
                            cafeMutex.unlock();   //unlock
                        }
                        //Also callback done
                        ChannelRequestStatus channelRequestStatusGet;
                        ChannelRequestStatus channelRequestStatusPut;
                        channelRequestStatusGet.setCallbackKind(false, true); //mimic completion
                        channelRequestStatusPut.setCallbackKind(false, true); //mimic completion
                        if(MUTEX)
                        {
                            cafeMutex.lock();   //lock
                        }
                        handle_index.modify(itcs, change_channelRequestStatusGet (channelRequestStatusGet) );
                        handle_index.modify(itcs, change_channelRequestStatusPut (channelRequestStatusPut) );
                        if(MUTEX)
                        {
                            cafeMutex.unlock();   //unlock
                        }
                        channelRegalia=(*itcs).getChannelRegalia();
                        channelRegalia.setCafeConnectionState(ICAFE_CS_CLOSED);
                        channelRegalia.setConnectFlag(false);
                        channelRegalia.setConnectionState(ICAFE_CA_OP_CONN_DOWN);
                        channelRegalia.channelID=NULL;
                        if(MUTEX)
                        {
                            cafeMutex.lock();   //lock
                        }
                        handle_index.modify(itcs, change_channelRegalia (channelRegalia) );

                        if(MUTEX)
                        {
                            cafeMutex.unlock();   //unlock
                        }

                        #if HAVE_PYTHON_H	
                            (*it_handle).PyConnectHandler();
                        #endif
			
                        break;
                    }
                    else
                    {
                        ++itcs;
                    }
                }  //for itcs
            }//if else
        }//for
    } //isClearChannel
   
    return statusGroup;

#undef __METHOD__
};



/**
  *  \brief Open (previously closed) connections to  given handles (even if in other threads)  \n
  *  \param handleArray input: unsigned int *
  *  \param nHandles input: unsigned int
  *  \return ECA_NORMAL if all OK; error if one or more channelIDs fail to close
  */
int Connect::openChannelsWithHandles(unsigned int * handleArray, unsigned int nHandles)
{
#define __METHOD__ "Connect::openChannelsWithHandles(unsigned int *, unsigned int) "

    //copy array to vector and remove duplicate handles
    vector<unsigned int> vec;
    vec.reserve(nHandles);
    vec.insert(vec.end(), &handleArray[0], &handleArray[nHandles]);
    //Remove duplicate values
    sort(vec.begin(), vec.end());
    vec.erase( unique( vec.begin(), vec.end() ), vec.end() );

    int  statusGroup = ECA_NORMAL;
    int  status      = ICAFE_NORMAL;
    bool isClearChannel = false;
    ChannelRegalia channelRegalia;
    vector<bool>   isClearChannelV;
    isClearChannelV.reserve(vec.size());

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    chid pCh = NULL;
    const char * pv;

    if (ca_current_context() != NULL)
    {
        for (unsigned int i=0; i<vec.size(); ++i)
        {
            isClearChannelV.push_back(false);
            it_handle = handle_index.find(vec[i]);
            if (it_handle != handle_index.end())
            {
	      pv = (*it_handle).getPV();
              
	        status=Connect::createChannel(vec[i],  pv, pCh);
                //std::cout << "Reopening " <<   (*it_handle).getHandle() << " " << pCh << std::endl;
		
                if (status == ECA_BADSTR)
                {
                      //Cannot Connect::close(handle) in this event else segmentation fault
                     CAFEException_pv e;
                     e = exceptionsHelper.prepareCAFEException_pv("empty", "empty", 
                        vec[i] , pCh, status, __METHOD__, __LINE__);
                     throw (e);
                     return status;
                }
                else if (status != ECA_NORMAL && status != ECA_TIMEOUT)
                {
                //Connect::close(handle);
                 CAFEException_pv e;
                 e = exceptionsHelper.prepareCAFEException_pv(pv, (*it_handle).pvAlias.c_str(),
                  vec[i], pCh, status, __METHOD__, __LINE__);
                 throw (e);
                 return status;
                }
                else if (pCh == NULL)
                {    
                //Connect::close(handle); Close channel in createHandle	
                CAFEException_pv e;
                e = exceptionsHelper.prepareCAFEException_pv(pv, (*it_handle).pvAlias.c_str(),
                vec[i], pCh, ECAFE_NULLCHID,  __METHOD__, __LINE__);
	
	        //exceptionsHelper.printCAFEException_pv(e);
                throw (e);
               return ECAFE_NULLCHID;
               }

           
               isClearChannelV[i]=true;
               isClearChannel=true;


            }
            else
            {
                // Loop through all elements
                for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
                {
                    if ((*itcs).getHandle()==vec[i])
                    {
		      pv = (*it_handle).getPV();
              
	                status =Connect::createChannel(vec[i],  pv, pCh);   

			if (status == ECA_BADSTR)
			  {
                      //Cannot Connect::close(handle) in this event else segmentation fault
                     CAFEException_pv e;
                     e = exceptionsHelper.prepareCAFEException_pv("empty", "empty", 
                         vec[i], pCh, status, __METHOD__, __LINE__);
                     throw (e);
                     return status;
                }
                else if (status != ECA_NORMAL && status != ECA_TIMEOUT)
                {
                //Connect::close(handle);
                 CAFEException_pv e;
                 e = exceptionsHelper.prepareCAFEException_pv(pv, (*it_handle).pvAlias.c_str(),
							      vec[i], pCh, status, __METHOD__, __LINE__);
                 throw (e);
                 return status;
                }
                else if (pCh == NULL)
                {    
                //Connect::close(handle); Close channel in createHandle	
                CAFEException_pv e;
                e = exceptionsHelper.prepareCAFEException_pv( pv, (*it_handle).pvAlias.c_str(),
                vec[i], pCh, ECAFE_NULLCHID,  __METHOD__, __LINE__);
	
	        //exceptionsHelper.printCAFEException_pv(e);
                throw (e);
               return ECAFE_NULLCHID;
               }

                        isClearChannelV[i]=true;
                        isClearChannel=true;
                        break;
                    }
                }//for
            }// else
        } //for


       if (isClearChannel)
       {
          if (channelOpenPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_CREATION)
          {
             channelOpenPolicy.flushSendBufferNow();
          } 	
       }

       return status;

    }//if ca_current_context != NULL
    else {
      return ECAFE_NULLCONTEXT;
    }	

#undef __METHOD__
}





/**
 *  \brief Closes channels for the given channel access client context. \n
 *  Shuts down the 'local' channel access client context and frees allocated resources  \n
 *  Note that in thread:stop, these two contexts are different!!  \n
 *  Not essential as resources are normally automatically released by the system.  \n
 *  This invokes the connection_handler callback function
 *  \return ECA_NORMAL if all OK or ECAFE_NULLCONTEXT if ca client context does not exist
 */
int Connect::contextDestroy(ca_client_context * cctLocal)
{
#define __METHOD__ "Connect::contextDestroy(ca_client_context * cctLocal)"

    int  statusCloseChannels=ICAFE_NORMAL;
    if (cctLocal == NULL)
    {
        return ECAFE_NULLCONTEXT;
    }

    //In case channels have not already been closed, then close them.
    //Closing Channels within this context first!
    statusCloseChannels = closeChannels(cctLocal);
    if (cctLocal==ca_current_context())
    {
        ca_context_destroy();
    }

    return statusCloseChannels;

#undef __METHOD__
};

/**
  *  \brief Shuts down a channel access client context and frees allocated resources
  *  Not essential as resources are normally automatically released by the system.
  *  This invokes the connection_handler callback function
  *  \return ECA_NORMAL if all OK or ECAFE_NULLCONTEXT if ca client context does not exist
  */
int Connect::contextDestroy()
{
#define __METHOD__ "Connect::contextDestroy "

    // Determine ca-client context
    ca_client_context * ccc = ca_current_context();
    if (ccc == NULL)
    {
        return ECAFE_NULLCONTEXT;
    }

    //In case channels have not already been closed, then close them.
    //Closing Channels within this context first!
    closeChannels(ccc);
    ca_context_destroy();
    return ICAFE_NORMAL;

#undef __METHOD__
};


/////////////////////////  Monitors         //////////////////////////////////////////////////

/**
  *  \brief Monitor an array of PV with default MonitorPolicy object parameters
  *  \param handleV input: Array of handles to conduit object
  *  \param statusV output: Array of statuses
  *  \param monitorIDV output: Array of unique MonitorIDs
  *  \return overallStatus: ICAFE_NORMAL if all OK else the first ECAFE error encountered
  */
int  Connect::monitorStart(std::vector<unsigned int> handleV, std::vector<int> &statusV,
                           std::vector<unsigned int> &monitorIDV)
{
#define __METHOD__ \
"monitorStart(vector<unsigned int> handleV, vector<int> &statusV,vector<unsigned int> &monitorIDV)"
    statusV.clear();
    statusV.reserve(handleV.size());
    monitorIDV.clear();
    monitorIDV.reserve(handleV.size());

    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;

    //ChangePolicy TO FLUSH ONLY AT END!! (not yet tested!)
    CAFENUM::ChannelWhenToFlushSendBufferPolicyKind previous=channelMonitorPolicy.getWhenToFlushSendBuffer() ;

    if (previous != FLUSH_DESIGNATED_TO_CLIENT)
    {
        channelMonitorPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
    }

    for (size_t i=0; i<handleV.size(); ++i)
    {

        unsigned int  monitorid;
        statusV.push_back(monitorStart(handleV[i], monitorid));
        monitorIDV.push_back(monitorid);

        if ( (statusV[i] != ICAFE_NORMAL) && isGood)
        {
            overallStatus=statusV[i];
            isGood=false;
        }
    }

    // flush now!
    //ca_flush_io();
    channelMonitorPolicy.flushSendBufferNow();

    if (channelMonitorPolicy.getWhenToFlushSendBuffer() ==
            FLUSH_DESIGNATED_TO_CLIENT)
    {
        channelMonitorPolicy.setWhenToFlushSendBuffer(previous);
    }
    return overallStatus; // report first failure in return statement


#undef __METHOD__
}


/**
  *  \brief Monitor an array of PV with parameters set by MonitorPolicy objects
  *  \param handleV input: Array of handles to conduit object
  *  \param statusV output: Array of statuses
  *  \param mpV input/output: Array of MonitorPolicy objects \n
  *         identifying the monitor subscription
  *  \return overallStatus: ICAFE_NORMAL if all OK else the first ECAFE error encountered
  */
int  Connect::monitorStart(std::vector<unsigned int> handleV, std::vector<int> &statusV,
                           std::vector<MonitorPolicy> &mpV)
{
#define __METHOD__ \
     "monitorStart(vector<unsigned int> handleV, vector<int> &statusV, vector<MonitorPolicy> &mpV) "

    if (mpV.size() < handleV.size())
    {
        //WARNING....
        cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
        cout << "WARNING VECTOR SIZE MISMATCH! vector<MonitorPolicy> has size " << mpV.size()
             << " while handle vector size is " << handleV.size() << endl;
        cout << "Adding default MonitorPolicy to handles: " << endl;
        for (size_t i=mpV.size(); i < handleV.size(); ++i)
        {
            MonitorPolicy mp;
            mpV.push_back(mp);
            cout << i << " " ;
        }
        cout << endl;
    }

    statusV.clear();
    statusV.reserve(handleV.size());

    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;

    //ChangePolicy TO FLUSH ONLY AT END!! (not yet tested!)
    CAFENUM::ChannelWhenToFlushSendBufferPolicyKind previous=channelMonitorPolicy.getWhenToFlushSendBuffer() ;

    if (previous != FLUSH_DESIGNATED_TO_CLIENT)
    {
        channelMonitorPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
    }

    for (size_t i=0; i<handleV.size(); ++i)
    {

        statusV.push_back(monitorStart(handleV[i], mpV[i]));

        if ( (statusV[i] != ICAFE_NORMAL) && isGood)
        {
            overallStatus=statusV[i];
            isGood=false;
        }

    }

    // flush now!
    //ca_flush_io();
    channelMonitorPolicy.flushSendBufferNow();

    if (channelMonitorPolicy.getWhenToFlushSendBuffer() ==
            FLUSH_DESIGNATED_TO_CLIENT)
    {
        channelMonitorPolicy.setWhenToFlushSendBuffer(previous);
    }

    return overallStatus; // report first failure in return statement
#undef __METHOD__
}

/**
 *  \brief Monitor an array of PV with parameters set by MonitorPolicy objects
 *  \param handleArray input: Array of handles to conduit object
 *  \param nelem input: Number of elements in the Array
 *  \param statusArray: Array of statuses
 *  \param mpArray input/output: Array of MonitorPolicy objects \n
 *         identifying the monitor subscription
 *  \return overallStatus: ICAFE_NORMAL if all OK else the first ECAFE error encountered
 */
int  Connect::monitorStart(unsigned int  * handleArray, unsigned int  nelem, int  * statusArray,
                           MonitorPolicy * mpArray)
{
#define __METHOD__ \
     "monitorStart(unsigned int  * handleArray, unsigned int  nelem, int  * statusArray, MonitorPolicy *  mpArray "
    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;

    //ChangePolicy TO FLUSH ONLY AT END!
    CAFENUM::ChannelWhenToFlushSendBufferPolicyKind previous=channelMonitorPolicy.getWhenToFlushSendBuffer() ;

    if (previous != FLUSH_DESIGNATED_TO_CLIENT)
    {
        channelMonitorPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
    }

    for (unsigned int  i=0; i<nelem; ++i)
    {
        statusArray[i]=monitorStart(handleArray[i], mpArray[i]);
        if ( (statusArray[i] != ICAFE_NORMAL) && isGood)
        {
            overallStatus=statusArray[i];
            isGood=false;
        }
    }
    // flush now!
    //ca_flush_io();
    channelMonitorPolicy.flushSendBufferNow();

    if (channelMonitorPolicy.getWhenToFlushSendBuffer() ==
            FLUSH_DESIGNATED_TO_CLIENT)
    {
        channelMonitorPolicy.setWhenToFlushSendBuffer(previous);
    }

    return overallStatus; // report first failure in return statement
#undef __METHOD__
}

/**
 *  \brief Monitor an array of PV with parameters with default policies
 *  \param handleArray input: Array of handles to conduit object
 *  \param nelem input: Number of elements in the Array
 *  \param statusArray: Array of statuses
 *  \param monitorIDArray output: Array of MonitorIDs to be used by monitorStop
 *  \return overallStatus: ICAFE_NORMAL if all OK else the first ECAFE error encountered
 */
int  Connect::monitorStart(unsigned int  * handleArray, unsigned int  nelem, int  * statusArray,
                           unsigned int  *  monitorIDArray)
{
#define __METHOD__ \
     "monitorStart(unsigned int  * handleArray, unsigned int  nelem, int  * statusArray, unsigned int  *  monitorIDArray"
    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;

    //ChangePolicy TO FLUSH ONLY AT END!! (not yet tested!)

    CAFENUM::ChannelWhenToFlushSendBufferPolicyKind previous=channelMonitorPolicy.getWhenToFlushSendBuffer() ;

    if (previous != FLUSH_DESIGNATED_TO_CLIENT)
    {
        channelMonitorPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
    }


    for (unsigned int  i=0; i<nelem; ++i)
    {
        statusArray[i]=monitorStart(handleArray[i], monitorIDArray[i]);
        if ( (statusArray[i] != ICAFE_NORMAL) && isGood)
        {
            overallStatus=statusArray[i];
            isGood=false;
        }
    }
    // flush now!
    //ca_flush_io();

    channelMonitorPolicy.flushSendBufferNow();
    if (channelMonitorPolicy.getWhenToFlushSendBuffer() ==
            FLUSH_DESIGNATED_TO_CLIENT)
    {
        channelMonitorPolicy.setWhenToFlushSendBuffer(previous);
    }

    return overallStatus; // report first failure in return statement
#undef __METHOD__
}

//// Let user add their own handler
/**
 *  \brief Monitor a PV
 *  \param handle input: handle
 *  \param mp input/output: MonitorPolicy object identifying the monitor subscription
 *  \return ICAFE_NORMAL if all OK else ECAFE error
 */
int  Connect::monitorStart(unsigned int handle, MonitorPolicy &mp)
{
#define __METHOD__ "monitorStart(unsigned int handle, MonitorPolicy &mp)"

    int status = ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {

        vector<MonitorPolicy> _mpV =(*it_handle).getMonitorPolicyVector();
        vector<MonitorPolicy>::iterator it;

        for (it = _mpV.begin(); it != _mpV.end(); ++it)
        {
            if ( (*it).getID()==mp.getID())
            {

                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "MONITOR POLICY OBJECT ALREADY IN USE!! " << endl;
                cafeStatus.report(ECAFE_NULLEVID);
                status=ECAFE_NULLEVID;
                return status;
            }
        }

        std::vector<MonitorPolicy> _mpiwV =(*it_handle).getMonitorPolicyInWaitingVector();
        std::vector<MonitorPolicy>::iterator itiw;

        for (itiw = _mpiwV.begin(); itiw != _mpiwV.end(); ++itiw)
        {
            if ( (*itiw).getID()==mp.getID())
            {

                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "MONITOR POLICY OBJECT ALREADY IN WAITING!! " << endl;
                cafeStatus.report(ECAFE_NULLEVID);
                status=ECAFE_NULLEVID;
                return status;
            }
        }

        // Check how many monitors have already been started.
        if (_mpV.size()>= MAX_NO_MONITORS_PER_CHANNEL)
        {
            cout << " HEY DUDE, YOU ALREADY HAVE " <<  MAX_NO_MONITORS_PER_CHANNEL
                 << " MONITORS ON THIS CHANNEL " << endl;
            cout << " I AM NOT GOING TO START ANOTHER ONE FOR YOU!! " << endl;
            return ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED;
        }


        //IS CHANNEL CONNECTED?
        //IF NOT DELAY START OF MONITOR

        if (!(*it_handle).isConnected())
        {

            // Check how many monitors are in aiting
            if (_mpiwV.size()>= MAX_NO_MONITORS_PER_CHANNEL)
            {
                cout << " HEY DUDE, YOU ALREADY HAVE " <<  MAX_NO_MONITORS_PER_CHANNEL
                     << " MONITORS WAITING TO START FOR THIS CHANNEL " << endl;
                cout << " I AM NOT GOING TO ADD ANOTHER ONE FOR YOU!! " << endl;
                return ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED;
            }


            //check in to vector<monitorMap>
            if(MUTEX)
            {
                cafeMutex.lock();
            }; //lock
            handle_index.modify(it_handle, change_monitorPolicyInWaitingInsert(mp));
            if(MUTEX)
            {
                cafeMutex.unlock();
            }; //unlock

            return ICAFE_MONITOR_DELAYED_AS_CONN_DOWN;
        }

        //check mp and replace default values!
        if (mp.getNelem()==0)
        {
            mp.setNelem((*it_handle).getChannelRegalia().getNelem());
        }
        else
        {
            mp.setNelem(min(mp.getNelem(), (*it_handle).getChannelRegalia().getNelem()));
        }

        if (mp.getDataType() == CAFE_NOT_REQUESTED)
        {
            mp.setDataType((*it_handle).getChannelRegalia().getDataType());
        }

        if (mp.getUserArgs() == NULL)
        {
            mp.setUserArgs((void *) (long long)(*it_handle).getHandle());
        }


        status=(*it_handle).monitorStart(mp);
        mp.setStatus(status);

        if (status==ICAFE_NORMAL)
        {

            //What is the POLICY!??
            //To Flush or to Pend this is the question!

            if (channelMonitorPolicy.getWhenToFlushSendBuffer()==
                    FLUSH_AFTER_EACH_CHANNEL_SUBSCRIPTION)
            {
                channelMonitorPolicy.flushSendBufferNow();
            }


            if (mp.getEventID()==NULL)
            {
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cafeStatus.report(ECAFE_NULLEVID);
                return ECAFE_NULLEVID;
            }

            //check in to vector<monitorMap>
            if(MUTEX)
            {
                cafeMutex.lock();
            }; //lock
            handle_index.modify(it_handle, change_monitorPolicyInsert(mp));
            if(MUTEX)
            {
                cafeMutex.unlock();
            }; //unlock

            _mpV =(*it_handle).getMonitorPolicyVector();

        }
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
        }
        return ECAFE_INVALID_HANDLE;
    }

    return status;
#undef __METHOD__
};


/**
 *  \brief Monitor a PV
 *  \param handle input: handle
 *  \param monitorID output: monitorID identifying the monitor subscription
 *  \return ICAFE_NORMAL if all OK else ECAFE error
 */
int  Connect::monitorStart(unsigned int handle, unsigned int  & monitorID)
{
#define __METHOD__ "monitorStart(unsigned int handle, unsigned int  & monitorID)"

    int status = ICAFE_NORMAL;
    evid eventID = NULL;
    monitorID = (unsigned int) 0;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {

        vector<MonitorPolicy> _mpV =(*it_handle).getMonitorPolicyVector();

        // Check how many monitors have already been started.
        if (_mpV.size()>= MAX_NO_MONITORS_PER_CHANNEL)
        {
            cout << " HEY DUDE, YOU ALREADY HAVE " <<  MAX_NO_MONITORS_PER_CHANNEL
                 << " MONITORS ON THIS CHANNEL " << endl;
            cout << " I AM NOT GOING TO START ANOTHER ONE FOR YOU!! " << endl;
            return ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED;
        }


//IS CHANNEL CONNECTED?
        //IF NOT DELAY START OF MONITOR

        if (!(*it_handle).isConnected())
        {

            vector<MonitorPolicy> _mpiwV =(*it_handle).getMonitorPolicyInWaitingVector();

            // Check how many monitors are in aiting
            if (_mpiwV.size()>= MAX_NO_MONITORS_PER_CHANNEL)
            {
                cout << " HEY DUDE, YOU ALREADY HAVE " <<  MAX_NO_MONITORS_PER_CHANNEL
                     << " MONITORS WAITING TO START FOR THIS CHANNEL " << endl;
                cout << " I AM NOT GOING TO ADD ANOTHER ONE FOR YOU!! " << endl;
                return ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED;
            }
        }


        // Do Prechecks - verify channel exists

        MonitorPolicy mp;// = (*it_handle).getMonitorPolicy();


        //FIRST CHECK IF CONNECTED!
        //Once connected, then we have this information stored
        //
        if ((*it_handle).getChannelRegalia().getCafeConnectionState() !=  ICAFE_CS_NEVER_CONN
                &&  (*it_handle).getChannelRegalia().getCafeConnectionState() !=  ICAFE_CS_CLOSED )
        {
            mp.setDataType((*it_handle).getChannelRegalia().getDataType());
            mp.setNelem((*it_handle).getChannelRegalia().getNelem());
        }

        mp.setUserArgs((void *) (long long) (*it_handle).getHandle());

        status=monitorStart(handle, mp);

        monitorID=mp.getID();

        if (status==ICAFE_NORMAL)
        {

            eventID=mp.getEventID();

            if (eventID==NULL)
            {
                return ECAFE_NULLEVID;
            }

        }
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
        }
        return ECAFE_INVALID_HANDLE;
    }

    return status;
#undef __METHOD__
}



/**
  *  \brief Monitor PulseID PV
  *  \return ICAFE_NORMAL if all OK else ECAFE error
  */
int  Connect::monitorPulseID()
{
#define __METHOD__ "monitorPulseID()"
    std::string pulseID=SF_PULSE_ID_PV;
    unsigned int  hPulseID;
    try
    {
        Connect::open(pulseID, hPulseID);
    }
    catch (CAFEException_open &e)
    {
        throw(e);
    }

    int status = ICAFE_NORMAL;
    MonitorPolicy mp;

    mp.setUserArgs((void *) (long long) (hPulseID));
    mp.setHandler(CALLBACK_CAFE::handlerPulseID);
    mp.setCafeDbrType(CAFENUM::DBR_TIME);
    mp.setDataType(DBR_DOUBLE);
    mp.setMask(DBE_VALUE | DBE_LOG | DBE_ALARM);

    status=monitorStart(hPulseID, mp);
    SF_WITH_PULSE_ID=true;

    return status;

#undef __METHOD__
}


/**
  *  \brief Monitor PulseID PV
  *  \return ICAFE_NORMAL if all OK else ECAFE error
  */
int  Connect::monitorStopPulseID()
{
#define __METHOD__ "monitorStopPulseID()"

    std::string pulseID=SF_PULSE_ID_PV;
    int status = ICAFE_NORMAL;
    status = monitorStop(handleHelper.getHandleFromPV(pulseID.c_str()));
    return status;

#undef __METHOD__
}


/**
 *  \brief  Stop all monitors
 *  \return ICAFE_NORMAL if all OK else ECAFE error
 */
int  Connect::monitorStop()
{
#define __METHOD__ "monitorStop()"

    int  statusGroup = ECA_NORMAL;
    int status = ICAFE_NORMAL;
    bool   isClearChannel   =false;

    if (ca_current_context() != NULL)
    {

        // Loop through all elements
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
        {

            //HandleHelper::clearMonitorAction(unsigned int _handle)

            status=monitorStop ((*itcs).getHandle() );

            if (status != ECA_NORMAL)
            {
                statusGroup = status;
            }
            isClearChannel=true;
        }
    }

    if (isClearChannel)
    {
        status = ca_pend_io(channelClosePolicy.getTimeout());
        if (statusGroup == ECA_NORMAL)
        {
            statusGroup = status;
        }
    }

    return statusGroup;

#undef __METHOD__
}


/**
 *  \brief  Stop all monitors for the given ca_client_context
 *  \return ICAFE_NORMAL if all OK else ECAFE error
 */
int  Connect::monitorStop(ca_client_context * cctLocal)
{
#define __METHOD__ "monitorStop(ca_client_context * cccLocal)"

    if (cctLocal == NULL )
    {
        return ECAFE_NULLCONTEXT;
    }
    int status = ICAFE_NORMAL;
    int  statusGroup = ECA_NORMAL;
    bool   isClearChannel   =false;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        if ((*itcs).getClientContext() != cctLocal )
        {
            continue;
        }

        status=monitorStop ((*itcs).getHandle() );

        if (status != ECA_NORMAL)
        {
            statusGroup = status;
        }
        isClearChannel=true;
    }

    if (isClearChannel)
    {
        status = ca_pend_io(channelClosePolicy.getTimeout());

        if (statusGroup == ECA_NORMAL)
        {
            statusGroup = status;
        }
    }

    return statusGroup;

#undef __METHOD__
}


/**
  *  \brief Stop all monitors for a vector of handles
  *  \param handleV input: vector of handles
  *  \param statusV output: vector of statuses
  *  \return overallStatus: ICAFE_NORMAL if all OK else ECAFE error of first reported failure
  */
int  Connect::monitorStop(std::vector<unsigned int> handleV, std::vector<int> &statusV)
{
#define __METHOD__ "monitorStop(vector<unsigned int> handleV, vector<int> statusV)"

    statusV.clear();
    statusV.reserve(handleV.size());

    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    for (unsigned int  i=0; i<handleV.size(); ++i)
    {

        statusV.push_back(monitorStop(handleV[i]));
        if ( (statusV[i] != ICAFE_NORMAL) && isGood)
        {
            overallStatus=statusV[i];
            isGood=false;
        }
    }
    // flush now!
    ca_flush_io();
    return overallStatus; // report first failure in return statement

#undef __METHOD__
}

/**
  *  \brief Stop all monitors for this array of handles
  *  \param handleArray input: array of handle
  *  \param nelem input: array of elements
  *  \param statusArray output: array of statuses
  *  \return overallStatus: ICAFE_NORMAL if all OK else ECAFE error of first reported failure
  */
int  Connect::monitorStop(unsigned int  * handleArray, unsigned int  nelem, int  * statusArray)
{
#define __METHOD__ "monitorStop(unsigned int  * handleArray, unsigned int  nelem)"

    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    for (unsigned int  i=0; i<nelem; ++i)
    {

        statusArray[i]=monitorStop(handleArray[i]);
        if ( (statusArray[i] != ICAFE_NORMAL) && isGood)
        {
            overallStatus=statusArray[i];
            isGood=false;
        }
    }
    // flush now!
    ca_flush_io();
    return overallStatus; // report first failure in return statement

#undef __METHOD__
}

/**
 *  \brief Stop all monitors for this handle
 *  \param handle input: handle
 *  \return ICAFE_NORMAL if all OK else ECAFE error
 */
int  Connect::monitorStop(unsigned int handle)
{
#define __METHOD__ "monitorStop(unsigned int  _handle)"

    int status = ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {

        evid eventID=NULL;
        unsigned int  monitorID= (unsigned int) NULL;

        //Loop all round those in waiting and those that started!

        vector<MonitorPolicy>  _mpiwV =(*it_handle).getMonitorPolicyInWaitingVector();
        vector<MonitorPolicy>::iterator itiw;

        for (itiw = _mpiwV.begin(); itiw != _mpiwV.end(); ++itiw)
        {
            //erase vector<monitorMap>

            if(MUTEX)
            {
                cafeMutex.lock();
            }; //lock

            monitorID=(*itiw).getMonitorID();
            handle_index.modify(it_handle, change_monitorPolicyInWaitingErase(monitorID));

            if(MUTEX)
            {
                cafeMutex.unlock();
            }; //unlock

        }


        vector<MonitorPolicy>  _mpV =(*it_handle).getMonitorPolicyVector();
        vector<MonitorPolicy>::iterator it;

        for (it = _mpV.begin(); it != _mpV.end(); ++it)
        {
            eventID=(*it).getEventID();
            monitorID=(*it).getMonitorID();
            //monitor has been removed
            if (eventID==NULL)
            {
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "Monitor never started or previously stopped " << endl;
                cafeStatus.report(ECAFE_NULLEVID);
                status=ECAFE_NULLEVID;
                continue;
            }

            //std::cout << "MS eventID  " << eventID << " monitorID " << monitorID
            //		  << " for h= " << handle  << std::endl;
            status=(*it_handle).monitorStop(eventID);



            if (status==ICAFE_NORMAL)
            {

                //JC It appears that ca_clear_subscription does not need a flush!?
                if (channelMonitorPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_SUBSCRIPTION)
                {
                    channelMonitorPolicy.flushSendBufferNow();
                }

                if(MUTEX)
                {
                    cafeMutex.lock();
                }; //lock
                handle_index.modify(it_handle, change_monitorPolicyErase(monitorID));
                if(MUTEX)
                {
                    cafeMutex.unlock();
                }; //unlock
            }
        }//for
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
        }
        return ECAFE_INVALID_HANDLE;
    }

    return status;

#undef __METHOD__
}


/**
 *  \brief Stop monitor of a PV
 *  \param handle input: handle
 *  \param mp input: MonitorPolicy identifying the monitor subscription
 *  \return ICAFE_NORMAL if all OK else ECAFE error
 */
int  Connect::monitorStop(unsigned int handle, MonitorPolicy mp)
{
#define __METHOD__ "monitorStop(unsigned int handle, MonitorPolicy mp)"

    return monitorStop(handle, mp.getMonitorID());

#undef __METHOD__
}

/**
 *  \brief Stop monitor of a PV
 *  \param handle input: handle
 *  \param  monitorID input: monitorID identifying the monitor subscription
 *  \return ICAFE_NORMAL if all OK else ECAFE error
 */
int  Connect::monitorStop (unsigned int handle, unsigned int  monitorID)
{
#define __METHOD__ "monitorStop(unsigned int handle, unsigned int  monitorID)"

    int status = ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {

        evid eventID=NULL;

        //Monitor ID  in waiting?
        vector<MonitorPolicy>  _mpiwV =(*it_handle).getMonitorPolicyInWaitingVector();
        vector<MonitorPolicy>::iterator itiw;

        for (itiw = _mpiwV.begin(); itiw != _mpiwV.end(); ++itiw)
        {
            if ( (*itiw).getID()==monitorID)
            {

                if(MUTEX)
                {
                    cafeMutex.lock();
                }; //lock
                handle_index.modify(it_handle, change_monitorPolicyInWaitingErase(monitorID));
                if(MUTEX)
                {
                    cafeMutex.unlock();
                }; //unlock

                return status;
            }
        }//for


        vector<MonitorPolicy>  _mpV =(*it_handle).getMonitorPolicyVector();
        vector<MonitorPolicy>::iterator it;

        for (it = _mpV.begin(); it != _mpV.end(); ++it)
        {
            if ( (*it).getID()==monitorID)
            {
                eventID=    (*it).getEventID();

                if (eventID==NULL)
                {
                    cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                    cafeStatus.report(ECAFE_NULLEVID);
                    status=ECAFE_NULLEVID;
                    return status;
                }
                else
                {
                    break;
                }
            }
        }

        //monitor has been removed
        if (eventID==NULL)
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Monitor never started or previously stopped " << endl;
            cafeStatus.report(ECAFE_NULLEVID);
            status=ECAFE_NULLEVID;
            return status;
        }

        status=(*it_handle).monitorStop(eventID);

        if (status==ICAFE_NORMAL)
        {

            //JC It appears that ca_clear_subscription does not need a flush!?

            if (channelMonitorPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_SUBSCRIPTION)
            {
                channelMonitorPolicy.flushSendBufferNow();
            }
            //erase vector<monitorMap>
            if(MUTEX)
            {
                cafeMutex.lock();
            }; //lock
            handle_index.modify(it_handle, change_monitorPolicyErase(monitorID));
            if(MUTEX)
            {
                cafeMutex.unlock();
            }; //unlock
        }
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
        }
        return ECAFE_INVALID_HANDLE;
    }

    return status;
#undef __METHOD__
}



/**
 *  \brief send the command to the ioc to open channels
 *  \param handleV input: vector of handles to wait for
 *  \param _timeout input: max pend time to establish connections \n
 */
void Connect::openNowAndWaitHandleV(std::vector<unsigned int> handleV, double _timeout)
{
    double dto = channelOpenPolicy.getTimeout();
    channelOpenPolicy.setTimeout(_timeout);
    //channelOpenPolicy.flushSendBufferNow();

    //Time lapsed

    using namespace boost::posix_time;
    ptime timeStart(microsec_clock::local_time());

    double  timeElapsed=0;
    unsigned int  nPoll=0;

    channelOpenPolicy.flushSendBufferNow();

    ptime timeEnd(microsec_clock::local_time());
    time_duration duration(timeEnd-timeStart);
    timeElapsed= (double) duration.total_microseconds()/1000000.0;

    //std::cout << "Timeout " << _timeout << " " << channelOpenPolicy.getTimeout() << std::endl;

    while ( !handleHelper.areChannelsConnectedV(handleV)
            && (timeElapsed <= channelOpenPolicy.getTimeout()))
    {

#if HAVE_BOOST_THREAD
        boost::this_thread::sleep_for(boost::chrono::microseconds(1000));
#else
#if HAVE_LINUX
        usleep(1000);
#endif
#endif

        ++nPoll;

        ptime timeEnd(microsec_clock::local_time());
        time_duration duration(timeEnd-timeStart);
        timeElapsed= (double) duration.total_microseconds()/1000000.0;

    }

    //std::cout << nPoll << std::endl;
    //reset
    channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_NOW);
    channelOpenPolicy.setFlushSendBufferKind(WITH_POLL); //PEND_EVENT);
    //channelOpenPolicy.setTimeoutToDefault();
    channelOpenPolicy.setTimeout(dto);
    return;
}




/**
 *  \brief send the command to the ioc to open channels
 *  \param handleArray input: array of handles to wait for
 *  \param nHanldes input: no of handles, i.e., size of handleArray
 *  \param _timeout input: max pend time to establish connections \n
 */
void Connect::openNowAndWaitHandles(unsigned int * handleArray, unsigned int nHandles, double _timeout)
{
    double dto = channelOpenPolicy.getTimeout();
    channelOpenPolicy.setTimeout(_timeout);
    //channelOpenPolicy.flushSendBufferNow();

    //Time lapsed

    using namespace boost::posix_time;
    ptime timeStart(microsec_clock::local_time());

    double  timeElapsed=0;
    unsigned int  nPoll=0;

    channelOpenPolicy.flushSendBufferNow();

    ptime timeEnd(microsec_clock::local_time());
    time_duration duration(timeEnd-timeStart);
    timeElapsed= (double) duration.total_microseconds()/1000000.0;

    //std::cout << "Timeout " << _timeout << " " << channelOpenPolicy.getTimeout() << std::endl;

    while ( !handleHelper.areChannelsConnected(handleArray, nHandles)
            && (timeElapsed <= channelOpenPolicy.getTimeout()))
    {

#if HAVE_BOOST_THREAD
        boost::this_thread::sleep_for(boost::chrono::microseconds(1000));
#else
#if HAVE_LINUX
        usleep(1000);
#endif
#endif

        ++nPoll;

        ptime timeEnd(microsec_clock::local_time());
        time_duration duration(timeEnd-timeStart);
        timeElapsed= (double) duration.total_microseconds()/1000000.0;

    }



    //reset
    channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_NOW);
    channelOpenPolicy.setFlushSendBufferKind(WITH_POLL); //PEND_EVENT);
    //channelOpenPolicy.setTimeoutToDefault();
    channelOpenPolicy.setTimeout(dto);
    return;
}



/**
 *  \brief send the command to the ioc to open channels
 *  \param _timeout input: max pend time to establish connections \n
 */
void Connect::openNowAndWait(double _timeout)
{
    double dto = channelOpenPolicy.getTimeout();
    channelOpenPolicy.setTimeout(_timeout);
    //channelOpenPolicy.flushSendBufferNow();

    //Time lapsed

    using namespace boost::posix_time;
    ptime timeStart(microsec_clock::local_time());

    double  timeElapsed=0;
    unsigned int  nPoll=0;

    channelOpenPolicy.flushSendBufferNow();

    ptime timeEnd(microsec_clock::local_time());
    time_duration duration(timeEnd-timeStart);
    timeElapsed= (double) duration.total_microseconds()/1000000.0;

    //std::cout << "Timeout " << _timeout << " " << channelOpenPolicy.getTimeout() << std::endl;

    while ( !handleHelper.allChannelsConnected() && (timeElapsed <= channelOpenPolicy.getTimeout()))
    {

#if HAVE_BOOST_THREAD
        boost::this_thread::sleep_for(boost::chrono::microseconds(1000));
#else
#if HAVE_LINUX
        usleep(1000);
#endif
#endif

        ++nPoll;

        ptime timeEnd(microsec_clock::local_time());
        time_duration duration(timeEnd-timeStart);
        timeElapsed= (double) duration.total_microseconds()/1000000.0;

    }
    /*
    if ( handleHelper.allChannelsConnected() ) {
    		cout << " // all is connected //"  << " nPoll = " << nPoll << endl;
    }
    else {
    		cout << " // all is not connected //" << endl;
    }
    cout << "timeElapsed for open " << timeElapsed << " nPoll = " << nPoll << endl;
    */


    //reset
    channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_NOW);
    channelOpenPolicy.setFlushSendBufferKind(WITH_POLL); //PEND_EVENT);
    //channelOpenPolicy.setTimeoutToDefault();
    channelOpenPolicy.setTimeout(dto);
    return;
}





/**
 *  \brief print status information of given handle
 *  \param handle input: handle to Conduit object \n
 *  \param  status input: reporting status \n
 *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int  Connect::printStatus(unsigned int handle, int  status)
{
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);
    if (it_handle != handle_index.end())
    {

        cout << "====================================" << endl;
        cout << "Handle=" << handle << " PV=" << (*it_handle).getPV() << endl;
        if ( (strcmp((*it_handle).getPV(), (*it_handle).getPVAlias())!=0)  )
        {
            cout << "PVAlias=" << (*it_handle).getPVAlias() << endl;
        }
        cafeStatus.report(status);
        cout << "====================================" << endl;
        return ICAFE_NORMAL;
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << handle << " is an invalid handle " << endl;
            cafeStatus.report(status);
        }
        return ECAFE_INVALID_HANDLE;
    }
}



/**
 *  \brief print status information of given handle only on error
 *  \param handle input: handle to Conduit object \n
 *  \param  status input: reporting status \n
 *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int  Connect::printStatusIfError(unsigned int handle, int  status)
{
    if (status==ICAFE_NORMAL)
    {
        return ICAFE_NORMAL;
    };
    return Connect::printStatus(handle, status);
}


/**
 *  \brief print status information of given handles
 *  \param handleV input: vector of handles to conduit objects \n
 *  \param statusV input: vector of statuses \n
 *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE (if one or more handles are invalid)
 */
int  Connect::printStatus(std::vector<unsigned int> handleV, std::vector<int> statusV)
{
    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    int  localStatus=ICAFE_NORMAL;
    for (unsigned int  i=0; i < min(handleV.size(),statusV.size()); ++i)
    {
        localStatus=Connect::printStatus(handleV[i], statusV[i]);
        if(isGood && localStatus!=ICAFE_NORMAL)
        {
            overallStatus=localStatus;
            isGood=false;
        }
    }
    return overallStatus;
}

/**
 *  \brief print status information of given handles only on error
 *  \param handleV input: vector of Handles to Conduit objects \n
 *  \param statusV input: vector of statuses \n
 *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE (if one or more handles are invalid)
 */
int  Connect::printStatusIfError(std::vector<unsigned int> handleV, std::vector<int>  statusV)
{
    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    int  localStatus=ICAFE_NORMAL;
    for (unsigned int  i=0; i <min(handleV.size(),statusV.size()); ++i)
    {
        if (statusV[i]!=ICAFE_NORMAL)
        {
            localStatus=Connect::printStatus(handleV[i], statusV[i]);
            if(isGood && localStatus!=ICAFE_NORMAL)
            {
                overallStatus=localStatus;
                isGood=false;
            }
        };
    }
    return overallStatus;
}


/**
 *  \brief print status information of given handles
 *  \param handleArray input: array of handles to conduit objects \n
 *  \param nelem input: size of array of handles
 *  \param statusArray input: array of statuses \n
 *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE (if one or more handles are invalid)
 */
int  Connect::printStatus(unsigned int  * handleArray, unsigned int  nelem, int  *statusArray)
{
    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    int  localStatus=ICAFE_NORMAL;
    for (unsigned int  i=0; i <nelem; ++i)
    {
        localStatus=Connect::printStatus(handleArray[i], statusArray[i]);
        if(isGood && localStatus!=ICAFE_NORMAL)
        {
            overallStatus=localStatus;
            isGood=false;
        }
    }
    return overallStatus;
}

/**
 *  \brief print status information of given handles only on error
 *  \param handleArray input: array of Handles to Conduit objects \n
 *  \param nelem input: size of array of handles
 *  \param statusArray input: array of statuses \n
 *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE (if one or more handles are invalid)
 */
int  Connect::printStatusIfError(unsigned int  * handleArray, unsigned int  nelem, int  *statusArray)
{
    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    int  localStatus=ICAFE_NORMAL;
    for (unsigned int  i=0; i <nelem; ++i)
    {
        if (statusArray[i]!=ICAFE_NORMAL)
        {
            localStatus=Connect::printStatus(handleArray[i], statusArray[i]);
            if(isGood && localStatus!=ICAFE_NORMAL)
            {
                overallStatus=localStatus;
                isGood=false;
            }
        };
    }
    return overallStatus;
}





/**
 *  \brief print status information of given pv
 *  \param pv input: pv for which handle is to be matched \n
 *  \param  status input: reporting status \n
 *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int  Connect::printStatus(const char *pv, int  status)
{

    unsigned int handle=handleHelper.getHandleFromPV(pv);

    if (handle==0)
    {
        return ECAFE_INVALID_HANDLE;
    }
    return Connect::printStatus(handle, status);

}


/**
 *  \brief print status information of given pv only on error
 *  \param pv input: pv for which handle is to be matched\n
 *  \param  status input: reporting status \n
 *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int  Connect::printStatusIfError(const char * pv, int  status)
{

    if (status==ICAFE_NORMAL)
    {
        return ICAFE_NORMAL;
    };

    unsigned int handle=handleHelper.getHandleFromPV(pv);

    if (handle==0)
    {
        return ECAFE_INVALID_HANDLE;
    }
    return Connect::printStatus(handle, status);

}


/**
 *  \brief print status information of given PVs
 *  \param pvV input: vector of pvs for which handles to conduit objects are to be matched\n
 *  \param statusV input: vector of statuses \n
 *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE (if one or more handles are invalid)
 */
int  Connect::printStatus(std::vector<std::string> pvV, std::vector<int> statusV)
{

    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    int  localStatus=ICAFE_NORMAL;
    for (unsigned int  i=0; i < min(pvV.size(),statusV.size()); ++i)
    {
        localStatus=Connect::printStatus(pvV[i].c_str(), statusV[i]);
        if(isGood && localStatus!=ICAFE_NORMAL)
        {
            overallStatus=localStatus;
            isGood=false;
        }
    }
    return overallStatus;
}

/**
 *  \brief print status information of given PVs only on error
 *  \param pvV input: vector of PVs for which handles to Conduit objects are to be matched\n
 *  \param statusV input: vector of statuses \n
 *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE (if one or more handles are invalid)
 */
int  Connect::printStatusIfError(std::vector<std::string> pvV, std::vector<int>  statusV)
{
    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    int  localStatus=ICAFE_NORMAL;
    for (unsigned int  i=0; i <min(pvV.size(),statusV.size()); ++i)
    {
        if (statusV[i]!=ICAFE_NORMAL)
        {
            localStatus=Connect::printStatus(pvV[i].c_str(), statusV[i]);
            if(isGood && localStatus!=ICAFE_NORMAL)
            {
                overallStatus=localStatus;
                isGood=false;
            }
        };
    }
    return overallStatus;
}


/**
 *  \brief print status information of given PVs
 *  \param pvArray input: array of PVs for which handles to conduit objects are to be matched\n
 *  \param nelem input: size of array of PVs
 *  \param statusArray input: array of statuses \n
 *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE (if one or more handles are invalid)
 */
int  Connect::printStatus(const char * pvArray, unsigned int  nelem, int  *statusArray)
{
    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    int  localStatus=ICAFE_NORMAL;
    for (unsigned int  i=0; i <nelem; ++i)
    {
        localStatus=Connect::printStatus(pvArray[i], statusArray[i]);
        if(isGood && localStatus!=ICAFE_NORMAL)
        {
            overallStatus=localStatus;
            isGood=false;
        }
    }
    return overallStatus;
}

/**
 *  \brief print status information of given PVs only on error
 *  \param pvArray input: array of PVnames \n
 *  \param nelem input: size of array of handles
 *  \param statusArray input: array of statuses \n
 *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE (if one or more handles are invalid)
 */
int  Connect::printStatusIfError(const char  * pvArray, unsigned int  nelem, int  *statusArray)
{
    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;
    int  localStatus=ICAFE_NORMAL;
    for (unsigned int  i=0; i <nelem; ++i)
    {
        if (statusArray[i]!=ICAFE_NORMAL)
        {
            localStatus=Connect::printStatus(pvArray[i], statusArray[i]);
            if(isGood && localStatus!=ICAFE_NORMAL)
            {
                overallStatus=localStatus;
                isGood=false;
            }
        };
    }
    return overallStatus;
}




/**
 *  \brief Closes all channels within the given context and their respective handles \n
 *  Shuts down the given channel access client context and frees allocated resources \n
 *  contextDestroy is not essential as resources are normally automatically released by the system.
 *  \return ECA_NORMAL if all OK or ECAFE_NULLCONTEXT if ca client context does not exist
 */
int  Connect::terminate(ca_client_context * cctLocal)
{

    int  statusTerminate=ICAFE_NORMAL;

    if(cctLocal==NULL)
    {
        cout << "Aborting attempt at releasing channel access resources due to:" << endl;
        cafeStatus.report(ECAFE_NULLCONTEXT);
        return statusTerminate=ECAFE_NULLCONTEXT;
    }

    //cout << "Stopping Monitors (if any)" << endl;
    monitorStop(cctLocal);

#if HAVE_BOOST_THREAD
    boost::this_thread::sleep_for(boost::chrono::microseconds(20000));
#else
#if HAVE_LINUX
    usleep(20000);
#endif
#endif
    //cout << "Closing all channels in given ca_client_context" << endl;
    closeChannels(cctLocal);
    groupHandleErase(cctLocal);
    //closeHandles();
    // Give a few ticks to avoid epics race conditions

#if HAVE_BOOST_THREAD
    boost::this_thread::sleep_for(boost::chrono::microseconds(30000));
#else
#if HAVE_LINUX
    usleep(30000);
#endif
#endif
    if(contextDestroy(cctLocal)==ECAFE_NULLCONTEXT)
    {
        cout << "Aborting attempt at releasing channel access resources due to:" << endl;
        cafeStatus.report(ECAFE_NULLCONTEXT);
        statusTerminate=ECAFE_NULLCONTEXT;
    }
    else
    {
        cout << "Channel Access Resources Released" << endl;
    }


#if HAVE_BOOST_THREAD
    boost::this_thread::sleep_for(boost::chrono::microseconds(30000));
#else
#if HAVE_LINUX
    usleep(30000);
#endif
#endif
    //Finally delete all global variables
    /*
    PVGroupV.erase (PVGroupV.begin(),PVGroupV.end());
    PVGroupPseudo.erase (PVGroupPseudo.begin(),PVGroupPseudo.end());
    deviceCollectionV.erase (deviceCollectionV.begin(),deviceCollectionV.end());
    groupPseudoMap.erase (groupPseudoMap.begin(), groupPseudoMap.end()  );

    ConduitGroup::groupHandleNext=0;
    Conduit::handleNext=0;
    */


    // cout << "The CAFEteria is now closed for this ca context" << endl;
    return statusTerminate;
};


/**
 *  \brief Closes all channels within a given context and all handles for all contexts \n
 *  Shuts down the local channel access client context and frees allocated resources \n
 *  contextDestroy is not essential as resources are normally automatically released by the system.
 *  \return ECA_NORMAL if all OK or ECAFE_NULLCONTEXT if ca client context does not exist
 */
int  Connect::terminate()
{
    int  statusTerminate=ICAFE_NORMAL;
    //cout << "\nStopping Monitors (if any)" << endl;
    monitorStop();
    // i.e. let callbacks fire before context is destroyed

#if HAVE_BOOST_THREAD
    boost::this_thread::sleep_for(boost::chrono::microseconds(30000));
  
#else
#if HAVE_LINUX
    usleep(30000);
#endif
#endif
    //cout << "Closing All Channels and Removing All Handles" << endl;
   
    closeHandles(); //All contexts
    //cout << "All Channels Closed and Handles Removed" << endl;
    groupHandleErase(); //erase groupHandles
   

    // Give a few ticks to avoid epics race conditions

#if HAVE_BOOST_THREAD
    boost::this_thread::sleep_for(boost::chrono::microseconds(30000));
#else
#if HAVE_LINUX
    usleep(30000);
#endif
#endif


    /*
    if(contextDestroy()==ECAFE_NULLCONTEXT)
    {
        cout << "Aborting attempt at releasing channel access resources due to:" << endl;
        cafeStatus.report(ECAFE_NULLCONTEXT);
        statusTerminate=ECAFE_NULLCONTEXT;
    }
    else
    {
      //cout << "Channel Access Resources Released" << endl;
    }
    */



#if HAVE_BOOST_THREAD
    boost::this_thread::sleep_for(boost::chrono::microseconds(30000));
#else
#if HAVE_LINUX
    usleep(30000);
#endif
#endif
    //Finally delete all global variables


    PVGroupV.erase (PVGroupV.begin(),PVGroupV.end());
    PVGroupPseudo.erase (PVGroupPseudo.begin(),PVGroupPseudo.end());
    deviceCollectionV.erase (deviceCollectionV.begin(),deviceCollectionV.end());
    groupPseudoMap.erase (groupPseudoMap.begin(), groupPseudoMap.end()  );

    globalChannelList.erase(globalChannelList.begin(), globalChannelList.end() );

    //Reset (for MATLAB)
    ConduitGroup::groupHandleNext=0;
    Conduit::handleNext=0;
    MonitorPolicy::idNext=0xfff;
  
    // Not required - results in segmentation faul!  
//#if HAVE_PYTHON_H
//    #if HAVE_PYCAFE_EXT
//        Py_Finalize();
//    #endif
//#endif

    //cout << "The CAFEteria is now closed" << endl;
    return statusTerminate;
};









