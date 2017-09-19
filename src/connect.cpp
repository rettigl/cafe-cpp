///
/// \file    connect.cc
/// \author  Jan Chrin, PSI
/// \date    November 2013
/// \version CAFE 1.0.0
///

#include "connect.h"
#include "conduitFriends.h"
#include "conduitConnectionHandlerArgs.h"

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


/**
 *  Connect::epicsVersion
 *
 *  Establishes virtual circuit to epics process variable
 *
 *  \param major input
 *  \param minor input
 *  \param patch input
 *  \return 31412 for v3.14.12
 *
 */
unsigned short Connect::epicsVersion(unsigned short & major, unsigned short & minor, unsigned short & patch) {

    major=EPICS_MAJOR;
    minor=EPICS_MINOR;
    patch=EPICS_PATCH;
    return major*10000+minor*100+patch;
}

/////////////////////////////////////////////////////////////////////
////// PRIVATE METHODS //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/**
 *  Connect::createChannel
 *
 *  Establishes virtual circuit to epics process variable
 *
 *  \param handle input
 *  \param pv input: process variable
 *  \param pCh output: channel identifier (chid)
 *  \return status ECA_NORMAL if all OK else ECA_BADTYPE, ECA_STRTOBIG, ECA_ALLOCMEM
 *
 */
int  Connect::createChannel(unsigned int handle, const char * pv, chid &pCh) {
#define __METHOD__ "Connect::createChannel(unsigned int handle, const char * pv, chid &pCh)"
 
    cafeConduit_set_by_handle &  handle_index= cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle =handle_index.find(handle);

    if (it_handle != handle_index.end()) {

										
        status = ca_create_channel(pv, channelCreatePolicy.getHandler(), (void *) handle,
                     channelCreatePolicy.getPriority() , &pCh);
			       
					
        if(pCh!=NULL) {
						
             if(MUTEX){cafeMutex.lock();}   //lock
            handle_index.modify(it_handle, change_channelID (pCh) );
             if(MUTEX){cafeMutex.unlock();} //unlock

            //Install the access rights state change callback handler
            if (ADD_EXCEPTION_EVENT==true) {
                ca_add_exception_event(callbackHandlerException, (void *) handle);
            }
												
             ca_replace_access_rights_event(pCh, callbackHandlerAccessRights);						
        }



        if (status != ECA_NORMAL) {
            cout << __FILE__ << "//" << __LINE__ << "//"<< __METHOD__ << " ca_create_channel failed: " << endl;
            cafeStatus.report(status);

             if(MUTEX){cafeMutex.lock();}  //lock
            handle_index.modify(it_handle, change_status (status) );
             if(MUTEX){cafeMutex.unlock();} //unlock
            return (int) status;
        }

				
				
        //What is the POLICY!??
        //To Flush or to Pend this is the question!
		
        if (channelOpenPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_CREATION) {
							 	     
              channelOpenPolicy.flushSendBufferNow(); 						        
        }

	
        // Peculiar if true
        if (status == ECA_EVDISALLOW) {
            cout << __FILE__ << "//" << __LINE__ << "//"<< __METHOD__ << " inappropriate function " << endl;
            cafeStatus.report(status);
             if(MUTEX){cafeMutex.lock();}  //lock
            handle_index.modify(it_handle, change_status (status) );
             if(MUTEX){cafeMutex.unlock();} //unlock
        }
    }
    else {
        return ECAFE_INVALID_HANDLE;
    }
   

return (int) status;

#undef __METHOD__
}



/**
 *  Connect::createHandle
 *
 *  Creates handle and calls createChannel to establish virtual circuit to epics process variable
 *
 *  \param pv input:  process variable
 *  \param ccc input: ca_client_context
 *  \param handle input: handle
 *  \exception CAFEException_pv if unable to create channel virtual circuit
 *  \return status ICAFE_NORMAL if all OK
 */
int  Connect::createHandle(const char * pv, ca_client_context * ccc,  unsigned int &handle)
         throw (CAFEException_pv)
{
#define __METHOD__ "Connect::createHandle()"

    chid pCh = NULL;
    std::pair<cafeConduit_set::iterator, bool> p;

    // unsigned int  uniqueID =helper.convertToUniqueNumber(pv,ccc);
    // CAFEConduit object initialized and placed into hash map
   
     if(MUTEX){cafeMutex.lock();}  //lock
	   p = cs.insert(Conduit(pv, ccc, 
		 channelRequestPolicyMasterPut, channelRequestPolicyMasterGet, 
		 channelGetActionWhenMonitorPolicyMaster, pyCafeFlag));// uniqueID));
     if(MUTEX){cafeMutex.unlock();}  //unlock

    ///Conduit cc = *(p.first);

    // The following should not happen as we do a pre-check in Connect::open
    // p.second true insert success
    // p.second false insert failed as pair already exists
     //has handle no exceeded limit
     if (!(p.second)) {
         cout << "HANDLE ALREADY IN USE! " << endl;
         cout << "ISSUING NEW HANDLE FOR PV= " << pv << endl;
         cout << "Next free handle is " << handleHelper.getNextFreeHandle() << endl;
         Conduit::handleNext=handleHelper.getNextFreeHandle()-1;
         if(MUTEX){cafeMutex.lock();}  //lock
         p = cs.insert(Conduit(pv, ccc, 
				 channelRequestPolicyMasterPut, channelRequestPolicyMasterGet, 
		     channelGetActionWhenMonitorPolicyMaster, pyCafeFlag));
         if(MUTEX){cafeMutex.unlock();}  //unlock
     }

    if (!(p.second)) {
         cout << "FAILED AGAIN" << endl;
        CAFEException_pv e;
        e = exceptionsHelper.prepareCAFEException_pv((*(p.first)).pv.c_str(), (*(p.first)).pvAlias.c_str(),
                                                     (*(p.first)).getHandle(), (*(p.first)).getChannelID(),
                                   ECAFE_HASH_UNIQUEID_EXISTS, __METHOD__, __LINE__);
        throw (e);
        return ECAFE_HASH_UNIQUEID_EXISTS;
    }
	

    ///_handle=cc.getHandle();
    handle=(*(p.first)).getHandle();


   
    // Normal return is ECA_TIMEOUT if pend_io/pend_event is instigated!!
    status=createChannel(handle, pv, pCh);
	
	
    // Channel Access will spit out an Invalid String Error if pv not a string!
    // Possible Errors from ca_create_channel: ECA_NORMAL, ECA_BADTYPE, ECA_STRTOBIG, ECA_ALLOCMEM
    // Possible Error from ca_pend_event: ECA_EVDISALLOW
    // IF FAIL then ERASE HANDLE and clear_channel if pCH exists!!


    //IF FAILED THROW EXCEPTION
    if (status == ECA_BADSTR) {

       // Connect::close(handle); //Cannot close else segmentation fault
        CAFEException_pv e;
        e = exceptionsHelper.prepareCAFEException_pv("empty", "empty", (*(p.first)).getHandle(), pCh, status,
                                    __METHOD__, __LINE__);
        throw (e);
        return status;
    }

    else if (status != ECA_NORMAL && status != ECA_TIMEOUT) {

        Connect::close(handle);
        CAFEException_pv e;
        e = exceptionsHelper.prepareCAFEException_pv((*(p.first)).pv.c_str(), (*(p.first)).pvAlias.c_str(),(*(p.first)).getHandle(), pCh, status,
                                    __METHOD__, __LINE__);
        throw (e);
        return status;
    }
    else if (pCh == NULL) {
        Connect::close(handle);
        CAFEException_pv e;
        e = exceptionsHelper.prepareCAFEException_pv( (*(p.first)).pv.c_str(), (*(p.first)).pvAlias.c_str(),(*(p.first)).getHandle(), pCh, ECAFE_NULLCHID,
                                    __METHOD__, __LINE__);
        throw (e);
        return ECAFE_NULLCHID;
    }

    //
    // testing use of handels and cs
    //


return status;

#undef __METHOD__
}


/////////////////////////////////////////////////////////////////////
////// PUBLIC METHODS ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/**
 *  Called once prior to making any channel access calls \n
 *  Specifies that preemptive callback is allowed:
 *  ca_enable_preemptive_callback
 *  \exception CAFEException_init ECA_ALLOCMEM if memory space could not be allocated
 *  \return status ECA_NORMAL if OK else throws an exception
 */
int  Connect::init() throw (CAFEException_init) {
#define __METHOD__ "Connect::init()"

    try {
		    status=init(ca_enable_preemptive_callback);
    }
    catch (CAFEException_init & e){
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << e.what() << endl;
        throw e;
    }
    catch (...){
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "UNKNOW EXCEPTION!" << endl;
	}


    return status;

#undef __METHOD__
};


/**
 *  Called once prior to making any channel access calls \n
 *  \param select input: enum ca_preemptive_callback_select \n
 *  {ca_disable_preemptive_callback=0, ca_enable_preemptive_callback=1}
 *  \exception CAFEbad_allocMem ECA_ALLOCMEM if memory space could not be allocated
 *  \return status ECA_NORMAL if OK else else throws an exception
 */
int  Connect::init(ca_preemptive_callback_select select) throw (CAFEException_init) {
#define __METHOD__ "Connect::init(ca_preemptive_callback_select)"

    status = ca_context_create(select);

    if (status != ECA_NORMAL) {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cafeStatus.report(status);

        if (status==ECA_ALLOCMEM) {
            CAFEException_init e;
            throw (e);
        }
        else if (status==ECA_NOTTHREADED) {
            cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(status);
        }
        else {
            cout << "THE GIVEN RETURN CODE DOES NOT MATCH THE EPICS DOCUMENTATION!" << endl;
            cout << "PLEASE REPORT THE ABOVE STATUS CODE TO THE AUTHOR. Thanks!   " << endl;
        }
    }


#if HAVE_PYTHON_H

  #if HAVE_PYCAFE_EXT
	Py_Initialize();
	//dlopen("libpython2.6.so", RTLD_GLOBAL|RTLD_LAZY);
	
	//dlopen("/opt/gfa/python-3.5/latest/lib/libpython3.5m.so", RTLD_GLOBAL|RTLD_LAZY);
	
	initPyCafe(); //only if PyCafe.h as opposed to PyCafe_api.h
	PyEval_InitThreads();
	
  #else 
	Py_Initialize();
	
	
	//dlopen("libpython2.6.so", RTLD_GLOBAL|RTLD_LAZY);
	//#if HAVE_LINUX
	//  dlopen("/opt/gfa/python-3.5/latest/lib/libpython3.5m.so", RTLD_GLOBAL|RTLD_LAZY);
	//#endif
	//PyEval_InitThreads();
	//import_PyCafe();  Do this in conduit.cc
	#endif
		
#endif


    return status;

#undef __METHOD__
};



/**
 *  \brief Establishes whether all callbacks from methodCallbacks.cpp have completed\n
 *  - If yes, nothing further to do, else test again \n
 *  - Theis method is used for callbacks initiated on creation of channel only\n
 *
 *  \param hArray output: array of reference handles to Conduit objects
 *  \param nHandles input: size ofhArray
 *  \return true if all callbavks completed else false
 */
bool Connect::initCallbackComplete(unsigned int * hArray, unsigned int nHandles) {
#define __METHOD__ "Connect:initCallbackComplete:"		
		
		ChannelRequestStatus cre, crc, crs, crn;
				
		for (unsigned int i=0; i < nHandles; ++i) {
			 if(handleHelper.isChannelConnected(hArray[i])) {
	  			handleHelper.getChannelRequestStatusGetCtrl  (hArray[i], crc);			
				  handleHelper.getChannelRequestStatusGetSTSACK(hArray[i], crs);
					handleHelper.getChannelRequestStatusGet      (hArray[i], cre);
					handleHelper.getChannelRequestStatusGet      (hArray[i], crn);
				
					if (crc.getCallbackProgressKind()==(CAFENUM::CallbackProgressKind) CAFENUM::PENDING ||
						crs.getCallbackProgressKind()==(CAFENUM::CallbackProgressKind) CAFENUM::PENDING ||
						cre.getCallbackProgressKind()==(CAFENUM::CallbackProgressKind) CAFENUM::PENDING ||
						crn.getCallbackProgressKind()==(CAFENUM::CallbackProgressKind) CAFENUM::PENDING 						
							) {
							cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;		
							cout << handleHelper.getPVFromHandle(hArray[i]) << " with handle " << hArray[i] << endl;
							cout << "Value of 1 is pending, Value of 2 is complete" << endl;
					  	cout << "Callback PENDING for ClassName?:   " << crn.getCallbackProgressKind()  << endl;
	    	    	cout << "Callback PENDING for Ctrl?:        " << crc.getCallbackProgressKind()  << endl;	
				    	cout << "Callback PENDING for STSACK?:      " << crs.getCallbackProgressKind()  << endl;
				    	cout << "Callback PENDING for GET?:         " << cre.getCallbackProgressKind()  << endl;
					
							return false;
					}
					
			}
	 	}
		
		return true;
			 
#undef __METHOD__			  	
}


/**
 *  \brief Establishes virtual circuits for a vector of epics process variables \n
 *  - Check if handle already exists for given pv/ca_client_context \n
 *  - If yes, nothing further to do, else create channel \n
 *  - Determine connection status and properties of the pv \n
 *  - Create CAFEConduit object and add to multi-index hash table \n
 *
 *  \param pvArray input: array of process variables (PVNAME_SIZE)
 *  \param handleArray output: array of reference handle to Conduit objects
 *  \param nHandles input: size of pvArray
 *  \exception CAFEException_open if unable to create channel virtual circuit
 *  \return status ICAFE_NORMAL if OK
 */
int  Connect::open(const char ** pvArray, unsigned int * handleArray,
                   const unsigned int nHandles)
        throw (CAFEException_open) {
#define __METHOD__ "Connect::open(const char **, unsigned int *, const unsigned int)"

    bool flushAtEndFlag=false;
    
    if (channelOpenPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_CREATION) {           
							channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
							flushAtEndFlag=true;						        
    }

    for (unsigned int  i=0;  i < nHandles; ++i) {
        try {
            status=open(pvArray[i], handleArray[i]);
        }
        catch (CAFEException_open &e) {
						if (flushAtEndFlag) { 
		   				channelOpenPolicy.flushSendBufferNow(); //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
						}
				
            throw(e);
            return status;
        }
    }
		
		if (flushAtEndFlag) { 
		   channelOpenPolicy.flushSendBufferNow(); //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
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
int  Connect::open(const string *pvArrayS, unsigned int *handleArray, const unsigned int nHandles)
        throw (CAFEException_open) {
#define __METHOD__ "Connect::open(const std::string *, unsigned int *, unsigned int)"


    bool flushAtEndFlag=false;
    
    if (channelOpenPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_CREATION) {           
							channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
							flushAtEndFlag=true;						        
    }



    for (unsigned int  i=0;  i < nHandles; ++i) {
        try {
            status=open(pvArrayS[i].c_str(), handleArray[i]);
        }
        catch (CAFEException_open &e) {
						if (flushAtEndFlag) { 
		  				 channelOpenPolicy.flushSendBufferNow(); //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
						}
				
            throw(e);
            return status;
        }
    }
		
		if (flushAtEndFlag) { 
		   channelOpenPolicy.flushSendBufferNow(); //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
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
int  Connect::open(vector<string> pvV, vector<unsigned int> &handleV)
        throw (CAFEException_open) {
#define __METHOD__ "Connect::open(vector<string>, vector<unsigned int> &)"

    unsigned int  _h=0;
    handleV.clear();
    handleV.reserve(pvV.size());


    bool flushAtEndFlag=false;
    
    if (channelOpenPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_CREATION) {           
							channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
							flushAtEndFlag=true;						        
    }



    for (unsigned int  i=0;  i < pvV.size(); ++i) {
        try {
            status=open(pvV[i].c_str(),_h);
            handleV.push_back(_h);
        }
        catch (CAFEException_open &e) {
						if (flushAtEndFlag) { 
		  				channelOpenPolicy.flushSendBufferNow(); //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
						}
				
            throw(e);
            return status;
        }
    }
		
		if (flushAtEndFlag) { 
		   channelOpenPolicy.flushSendBufferNow(); //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
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
 *  \param pvV input: vector of process variables (char *)
 *  \param handleV output: vector of reference handle to Conduit objects
 *  \exception CAFEException_open if unable to create channel virtual circuit
 *  \return status ICAFE_NORMAL if OK
 */
int  Connect::open(vector<const char *> pvV, vector<unsigned int> &handleV)
        throw (CAFEException_open) {
#define __METHOD__ "Connect::open(vector<char *>, vector<unsigned int> &)"

    unsigned int  _h=0;
    handleV.clear();
    handleV.reserve(pvV.size());

    
    bool flushAtEndFlag=false;
    
    if (channelOpenPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_CREATION) {           
							channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
							flushAtEndFlag=true;						        
    }
 

    for (unsigned int  i=0;  i < pvV.size(); ++i) {
        try {
            status=open(pvV[i],_h);
            handleV.push_back(_h);
        }
        catch (CAFEException_open &e) {
						if (flushAtEndFlag) { 
		   				channelOpenPolicy.flushSendBufferNow(); //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
						}
				
            throw(e);
            return status;
        }
    }
		
		if (flushAtEndFlag) { 
		   channelOpenPolicy.flushSendBufferNow(); //this also resets whenKind=FLUSH_AFTER_EACH_CHANNEL_CREATION
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
int  Connect::open(const char * _pv, const char * _pvAlias, unsigned int  & handle)
        throw (CAFEException_open) {
#define __METHOD__ "Connect::open(const char * pv, const char * _pvAlias, unsigned int  & _handle)"

    try {
        status = open (_pv, handle);
    }
    catch (CAFEException_open &e) {
        throw(e);
        return status;
    }

    if (handle != 0) {
        try {
            status=setPVAlias(handle,_pvAlias);
            }
        catch (CAFEException_open & badpv) {
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
  *
  *  \param _pv input: process variable
  *  \param handle output: reference handle to Conduit object
  *  \exception CAFEException_pvOpen if unable to create channel virtual circuit
  *  \return status ICAFE_NORMAL if OK
  */
 int  Connect::open(const char * _pv, unsigned int &handle)
         throw (CAFEException_open) {
 #define __METHOD__ "Connect::open(const char * _pv, unsigned int &handle)"

    char pv[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(_pv, pv);	

    // Determine ca-client context
    ca_client_context * ccc = ca_current_context();

    if (ccc == NULL) {

        try{
            status=init( ); // ca_disable_preemptive_callback
        }
        catch(CAFEException_init){
            //status will be ECA_ALLOCMEM
            CAFEException_pv e;
            e = exceptionsHelper.prepareCAFEException_pv(pv, pv, 0, NULL,
                                        ECA_ALLOCMEM,__METHOD__, __LINE__);
            CAFEException_open badpv;
            badpv.pvEx=e;
            throw(badpv);

            return status;
        }

        ccc = ca_current_context();
    }

    // Check if handle already exists for this pv
    // Handles for pvs that are members of a group are treated separately!

    handle = handleHelper.getHandleFromPV(pv,ccc);

    
    if (handle != 0) {
        return ICAFE_NORMAL;
    }

    //This is called in the above
   
    try {
		
		
        status = createHandle(pv, ccc, handle);
    }
    catch (CAFEException_pv & e) {
        CAFEException_open badpv;
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
 int  Connect::setPVAlias(unsigned int handle, const char * _pvAlias) throw(CAFEException_open){
 #define __METHOD__ "Connect::setPVAlias(unsigned int  * _handle, const char * _pv)"

     Helper helper;
     char pvAlias[PVNAME_SIZE];
     helper.removeLeadingAndTrailingSpaces(_pvAlias, pvAlias);

     _pvAlias=pvAlias;

     cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
     cafeConduit_set_by_handle::iterator it_handle;
     it_handle = handle_index.find(handle);

     if (it_handle != handle_index.end()) {

         ca_client_context * ccc = ca_current_context();

         //Need to make sure that Alias is NOT already in use as a PVName for another handle (would be strange!)
         unsigned int   pvHandle=handleHelper.getHandleFromPV(pvAlias,  ccc);

         if ( pvHandle > 0 &&  pvHandle != handle) {

             CAFEException_pv e;
             e = exceptionsHelper.prepareCAFEException_pv((*it_handle).pv.c_str(), _pvAlias,
                                        (*it_handle).getHandle(), (*it_handle).getChannelID(),
                                        ECAFE_PVALIAS_INVALID,__METHOD__, __LINE__);
             CAFEException_open badpv;
             badpv.pvEx=e;
             throw(badpv);
             return   ECAFE_PVALIAS_PREV_DEF;
         }


         pvHandle=handleHelper.getHandleFromPVAlias(pvAlias,  ccc);
	
         //Need to make sure that Alias is NOT already in use
         if ( pvHandle > 0 && pvHandle != handle) {

             CAFEException_pv e;
             e = exceptionsHelper.prepareCAFEException_pv((*it_handle).pv.c_str(), _pvAlias,
                                                          (*it_handle).getHandle(), (*it_handle).getChannelID(),
                                                          ECAFE_PVALIAS_PREV_DEF,__METHOD__, __LINE__);
             CAFEException_open badpv;
             badpv.pvEx=e;
             throw(badpv);
             return   ECAFE_PVALIAS_PREV_DEF;
         }


          if(MUTEX){cafeMutex.lock();}   //lock
         handle_index.modify(it_handle, change_pvAlias( _pvAlias));
          if(MUTEX){cafeMutex.unlock();} //unlock
	
      } else {
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
 int  Connect::closeChannels(unsigned int * handleArray, unsigned int nHandles) {
 #define __METHOD__ "Connect::closeChannels(unsigned int * handleArray, unsigned int nHandles)"

   	
    ca_client_context * cctLocal=  ca_current_context();

    if (cctLocal == NULL ) {
        // We do not know if this channel belongs to this context!
        //cs.erase(it_handle);
        return ECAFE_NULLCONTEXT;
    }
		
		
		//copy array to vector and remove dupliacte handles
		vector<unsigned int> vec;
		vec.reserve(nHandles);
		vec.insert(vec.end(), &handleArray[0], &handleArray[nHandles]);
	

		//Remove duplicate values
		sort( vec.begin(), vec.end() );
		vec.erase( unique( vec.begin(), vec.end() ), vec.end() );


    int  statusGroup = ECA_NORMAL;
    int  status      = ICAFE_NORMAL;

    bool   isClearChannel   =false;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
 

	for (unsigned int i=0; i<vec.size(); ++i) {
        it_handle = handle_index.find(vec[i]);

        if (it_handle != handle_index.end()) {
      

            if ((*it_handle).getClientContext() != cctLocal ) {         
              continue;
            }
						// Only clear channel if its ca_client_context is valid
       			if ((*it_handle).getChannelID() != NULL) {
            	status = ca_clear_channel((*it_handle).getChannelID());           
           		if (status != ECA_NORMAL){
                statusGroup = status;
            	}
            	isClearChannel=true;

            }
		}
	}
    
    // Even if Null Context remove handle. This could happen if a ca_context_destroy was called
    if (isClearChannel) {
        status = ca_pend_io(channelClosePolicy.getTimeout());
        if (status != ECA_NORMAL) {
            statusGroup = status;
        }
	}
   
	 
   // Loop through and identify valid elements and clear their databuffers
   // i.e., only for those channels within the ca_client_context

      			
				
	for (unsigned int i=0; i<vec.size(); ++i) {
        it_handle = handle_index.find(vec[i]);

        if (it_handle != handle_index.end()) {
      
	         // Only clear channel if its ca_client_context is valid
            if ((*it_handle).getChannelID() != NULL && (*it_handle).getClientContext() == cctLocal ) {         
                if(MUTEX){cafeMutex.lock();}
                handle_index.modify(it_handle, free_dataBuffers());
                if(MUTEX){cafeMutex.unlock();}
					
            }

			cs.erase(it_handle); //Moved to within loop; fixed May 2017
		}
	}
				
			
    return statusGroup; 

 #undef __METHOD__
 }





 /**
  *  \brief Closes connection for channel within a ca_client_context and deletes corresponding handle \n
  *  The handle must be within the same context. \n
  *  Note that this does NOT cause the channel's disconnect handler to be called. \n
  *  It does however invoke event handlers for subscriptions (i.e. monitors). \n
  *  The Conduit handle is erased
  *  \param  handle input
  *  \return ECA_NORMAL if all OK
  */
 int  Connect::close(unsigned int handle) {
 #define __METHOD__ "Connect::close"


   ca_client_context * cctLocal=  ca_current_context();

    if (cctLocal == NULL ) {
        // We do not know if this channel belongs to this context!
        //cs.erase(it_handle);
        return ECAFE_NULLCONTEXT;
    }

    status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(handle);

        if (it_handle != handle_index.end()) {

            if ((*it_handle).getClientContext() == NULL ) {
                //Channel already forcefully cleared
                cs.erase(it_handle);
                return ECAFE_NULLCONTEXT;
            }

            if ((*it_handle).getClientContext() != cctLocal ) {
                return ECAFE_WRONG_CA_CONTEXT;
            }

            if ((*it_handle).getChannelID() != NULL) {


                status = ca_clear_channel((*it_handle).getChannelID());

                if (status != ECA_NORMAL) {
                    return status; //ECA_BADCHID
                }

				        status = ca_pend_io(channelClosePolicy.getTimeout());
								
                if(MUTEX){cafeMutex.lock();}
                handle_index.modify(it_handle, free_dataBuffers());
                if(MUTEX){cafeMutex.unlock();}
            }

            cs.erase(it_handle);
            return status;
        }
        else {
            // Loop through all elements
            for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {

                if ((*itcs).handle == handle) {

                    if ((*itcs).getClientContext() == NULL ) {
                        //Channel already forcefully cleared
                        cs.erase(itcs);
                        return ECAFE_NULLCONTEXT;
                    }

                    if ((*itcs).getClientContext() != cctLocal ) {
                        return ECAFE_WRONG_CA_CONTEXT;
                    }

                    if ((*itcs).getChannelID() != NULL) {

                        status = ca_clear_channel((*itcs).getChannelID());
                        if (status != ECA_NORMAL) {
                            return status; //ECA_BADCHID
                        }


						            status = ca_pend_io(channelClosePolicy.getTimeout());
                        if(MUTEX){cafeMutex.lock();}
                        handle_index.modify(itcs, free_dataBuffers());
                        if(MUTEX){cafeMutex.unlock();}
                    }

                cs.erase(itcs);
                return status;
                }
            }
            return ECAFE_INVALID_HANDLE;
         }

    return status; // This line is not reached

 #undef __METHOD__
 }


 /**
  *  \brief Closes all channel connections within a ca_client_context and removes the corresponding handles.  \n
  *  Note that this does NOT cause the channel's disconnect handler to be called.  \n
  *  It does however invoke event subscriptions (for monitors).   \n
  *  Corresponding Conduit handles are erased.
  *  \return ECA_NORMAL if all OK; error if one or more channelIDs fail to close
  */
 int  Connect::closeChannels() {
 #define __METHOD__ "Connect::closeChannels"
     ca_client_context * cctLocal=  ca_current_context();

     if (cctLocal == NULL ) {
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
 int  Connect::closeChannels(ca_client_context * cctLocal) {
 #define __METHOD__ "Connect::closeChannelsca_client_context * cctLocal)"

    if (cctLocal == NULL ) {
        return ECAFE_NULLCONTEXT;
    }

    int  statusGroup = ICAFE_NORMAL;

    bool   isClearChannel=false;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {

        if ((*itcs).getClientContext() != cctLocal ) {         
            continue;
        }

        // Only clear channel if its ca_client_context is valid
        if ((*itcs).getChannelID() != NULL) {
            status = ca_clear_channel((*itcs).getChannelID());           
            if (status != ECA_NORMAL){
                statusGroup = status;
            }
            isClearChannel=true;
        }
    }

    // Even if Null Context remove handle. This could happen if a ca_context_destroy was called
    if (isClearChannel) {
        status = ca_pend_io(channelClosePolicy.getTimeout());
        if (status != ECA_NORMAL) {
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

        //cout << "--------------------------------------------------------------------" << endl;
        //cout << "CAFE HAS CLOSED ALL CHANNELS WITHIN THE CA_CLIENT_CONTEXT: " << ca_current_context()  << endl;
        //cout << "AND IS ERASING THEIR RESPECTIVE HANDLES:" << endl;
        //cout << "--------------------------------------------------------------------" << endl;

        if(MUTEX){cafeMutex.lock();}

        for (itcs = cs.begin(); itcs != cs.end(); ) {
            if ((*itcs).getChannelID() != NULL &&
                    ((*itcs).getClientContext() == cctLocal))
            {
                // good test of memory handling
                handle_index.modify(itcs, free_dataBuffers());               
                cs.erase(itcs++);
            }
            else {
                ++itcs;
            }
        }//for
        //cout << endl;
        //cout << "----------------------------------------------------------------" << endl << endl;
        if(MUTEX){cafeMutex.unlock();}
    }
    return statusGroup;

 #undef __METHOD__
 }


 /**
  *  \brief Closes channel connections (even if in other thread) and deletes handle.  \n
  *  Note that this does NOT cause the channel's disconnect handler to be called.  \n
  *  It does however invoke event handlers for subscriptions (i.e. monitors).  \n
  *  The Conduit handle is erased
  *  \param  handle input
  *  \return ECA_NORMAL if all OK
  */
 int  Connect::closeHandle(unsigned int handle) {
 #define __METHOD__ "Connect::closeHandle"

    //We can close handle irrespective of ca_current_context!
    ca_client_context * cctLocal=  ca_current_context();

    status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(handle);

        if (it_handle != handle_index.end()) {


            if ((*it_handle).getChannelID() != NULL &&  cctLocal != NULL ) {


                status = ca_clear_channel((*it_handle).getChannelID());



                if (status != ECA_NORMAL) {
                    return status; //ECA_BADCHID
                }

                status = ca_pend_io(channelClosePolicy.getTimeout());
                if(MUTEX){cafeMutex.lock();}
                handle_index.modify(it_handle, free_dataBuffers());
                if(MUTEX){cafeMutex.unlock();}
            }

            cs.erase(it_handle);
            return status;
        }
        else {
            // Loop through all elements
            for (itcs = cs.begin(); itcs != cs.end();) {
                //Conduit cc = *itcs;

                //operator overload prints handle, pv and nelem

                if ((*itcs).handle == handle) {
                    if ((*itcs).getChannelID() != NULL &&  cctLocal != NULL) {

                        status = ca_clear_channel((*itcs).getChannelID());
                        if (status != ECA_NORMAL) {
                            return status; //ECA_BADCHID
                        }

                        status = ca_pend_io(channelClosePolicy.getTimeout());
                        if(MUTEX){cafeMutex.lock();}
                        handle_index.modify(itcs, free_dataBuffers());
                        if(MUTEX){cafeMutex.unlock();}
                    }

                cs.erase(itcs++);
                return status;
                }
                else {
                    ++itcs;
                }
            }
            return ECAFE_INVALID_HANDLE;
         }

    return status; // This line is not reached

 #undef __METHOD__
 }


 /**
  *  \brief Closes given channel connections (even if in other threads) and deletes their handles.  \n
  *  Note that this does NOT cause the channel's disconnect handler to be called.  \n
  *  It does however invoke event subscriptions (for monitors).  \n
  *  All corresponding Conduit handles are erased.
  *  \param handleArray input: unsigned int *
  *  \param nHandles input: unsigned int
  *  \return ECA_NORMAL if all OK; error if one or more channelIDs fail to close
  */
 int  Connect::closeHandles(unsigned int * handleArray, unsigned int nHandles) {
 #define __METHOD__ "Connect::closeHandles(unsigned int *) "

		
		//copy array to vector and remove dupliacte handles
		vector<unsigned int> vec;
		vec.reserve(nHandles);
		vec.insert(vec.end(), &handleArray[0], &handleArray[nHandles]);
	

		//Remove duplicate values
		sort( vec.begin(), vec.end() );
		vec.erase( unique( vec.begin(), vec.end() ), vec.end() );

	
    int  statusGroup = ECA_NORMAL;
    int  status      = ICAFE_NORMAL;

    bool   isClearChannel   =false;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    if (ca_current_context() != NULL) {

        //for (unsigned int i=0; i<nHandles; ++i) {
            //it_handle = handle_index.find(handleArray[i]);

				for (unsigned int i=0; i<vec.size(); ++i) {
            it_handle = handle_index.find(vec[i]);


            if (it_handle != handle_index.end()) {

                if ((*it_handle).getChannelID() != NULL  ) {

                    status = ca_clear_channel((*it_handle).getChannelID());
                    if (status != ECA_NORMAL) {
                        statusGroup=status; //ECA_BADCHID
                    }

                }
                isClearChannel=true;
            }

            else {

                // Loop through all elements
                for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {

                    //if ((*itcs).getHandle()==handleArray[i]) {
										if ((*itcs).getHandle()==vec[i]) {

                        if ((*itcs).getChannelID() != NULL ) {
                           status = ca_clear_channel((*itcs).getChannelID());

                           //cout << "clear channel with handle: " << (*itcs).getHandle() << endl;

                           if (status != ECA_NORMAL){
                               statusGroup = status;
                           }
                           isClearChannel=true;
                        }
                        break;
                    }
                }
            }// else
        } //for
    }//if ca_current_context != NULL

    if (isClearChannel) {
        status = ca_pend_io(channelClosePolicy.getTimeout());
        if (statusGroup == ECA_NORMAL) {
            statusGroup = status;
        }
    }

		//for (unsigned int i=0; i<nHandles; ++i) {
        //it_handle = handle_index.find(handleArray[i]);

    for (unsigned int i=0; i<vec.size(); ++i) {
        it_handle = handle_index.find(vec[i]);

        if (it_handle != handle_index.end()) {
            if(MUTEX){cafeMutex.lock();}
            handle_index.modify(it_handle, free_dataBuffers());
            if(MUTEX){cafeMutex.unlock();}
            cs.erase(it_handle);
        }
        else {

            // Loop through all elements
            for (itcs = cs.begin(); itcs != cs.end();) {

                //if ((*itcs).handle == handleArray[i]) {
								if ((*itcs).handle == vec[i]) {
                     if(MUTEX){cafeMutex.lock();}
                    handle_index.modify(itcs, free_dataBuffers());
                     if(MUTEX){cafeMutex.unlock();}

                    cs.erase(itcs++);
                    break;
                }
                else {
                    ++itcs;
                }
            }         
        }

    }//for



    return statusGroup;

 #undef __METHOD__
 }



 /**
  *  \brief Closes all channel connections (even if in other threads) and deletes all handles.  \n
  *  Note that this does NOT cause the channel's disconnect handler to be called.  \n
  *  It does however invoke event subscriptions (for monitors).  \n
  *  All Conduit handles are erased.
  *  \return ECA_NORMAL if all OK; error if one or more channelIDs fail to close
  */
 int  Connect::closeHandles() {
 #define __METHOD__ "Connect::closeHandles "

    int  statusGroup = ECA_NORMAL;

    bool   isClearChannel   =false;
    bool   isCS             =false;

    if (ca_current_context() != NULL) {

        // Loop through all elements
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {

            if ((*itcs).getChannelID() != NULL) {
						   
                status = ca_clear_channel((*itcs).getChannelID());
 							
                if (status != ECA_NORMAL){
                    statusGroup = status;
                }
                isClearChannel=true;
            }
        isCS=true;
        }
    }

    if (isClearChannel) {
        status = ca_pend_io(channelClosePolicy.getTimeout());
        if (statusGroup == ECA_NORMAL) {
            statusGroup = status;
        }
    }

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();

    // Loop through all elements and clear databuffers
    if(MUTEX){cafeMutex.lock();}
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {
		  	//cout << " CLEAR   CHANNEL " << (*itcs).getHandle () << endl;
        handle_index.modify(itcs, free_dataBuffers());
				//cout << " CLEARED CHANNEL " << (*itcs).getHandle () << endl;
    }
    if(MUTEX){cafeMutex.unlock();}

    if(isCS) {
        cs.clear();
    }


    return statusGroup;

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
 int  Connect::contextDestroy(ca_client_context * cctLocal) {
 #define __METHOD__ "Connect::contextDestroy(ca_client_context * cctLocal)"

    int  statusCloseChannels=ICAFE_NORMAL;

    if (cctLocal == NULL) {
        return ECAFE_NULLCONTEXT;
    }

    //In case channels have not already been closed, then close them.
    //Closing Channels within this context first!
    statusCloseChannels = closeChannels(cctLocal);

    if (cctLocal==ca_current_context()){
        ca_context_destroy();
    }

    return statusCloseChannels;

 #undef __METHOD__
 }


 /**
  *   \brief Shuts down a channel access client context and frees allocated resources
  *  Not essential as resources are normally automatically released by the system.
  *  This invokes the connection_handler callback function
  *  \return ECA_NORMAL if all OK or ECAFE_NULLCONTEXT if ca client context does not exist
  */
 int  Connect::contextDestroy() {
 #define __METHOD__ "Connect::contextDestroy "

    // Determine ca-client context
    ca_client_context * ccc = ca_current_context();

    if (ccc == NULL) {
        return ECAFE_NULLCONTEXT;
    }

    //In case channels have not already been closed, then close them.
    //Closing Channels within this context first!
    closeChannels(ccc);

    ca_context_destroy();

    return ICAFE_NORMAL;

 #undef __METHOD__
 }





 /////////////////////////  Monitors         //////////////////////////////////////////////////


 /**
   *  \brief Monitor an array of PV with default MonitorPolicy object parameters
   *  \param handleV input: Array of handles to conduit object
   *  \param statusV output: Array of statuses
   *  \param monitorIDV output: Array of unique MonitorIDs
   *  \return overallStatus: ICAFE_NORMAL if all OK else the first ECAFE error encountered
   */
int  Connect::monitorStart(vector<unsigned int> handleV, vector<int> &statusV,
                  vector<unsigned int> &monitorIDV) {
#define __METHOD__ \
"monitorStart(vector<unsigned int> handleV, vector<int> &statusV,vector<unsigned int> &monitorIDV)"
    statusV.clear();
    statusV.reserve(handleV.size());
    monitorIDV.clear();
    monitorIDV.reserve(handleV.size());

    int  overallStatus=ICAFE_NORMAL; bool isGood=true;

	//ChangePolicy TO FLUSH ONLY AT END!! (not yet tested!)
	CAFENUM::ChannelWhenToFlushSendBufferPolicyKind previous=channelMonitorPolicy.getWhenToFlushSendBuffer() ;

	if (previous != FLUSH_DESIGNATED_TO_CLIENT) {
		channelMonitorPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
	}

    for (size_t i=0; i<handleV.size(); ++i) {

        unsigned int  monitorid;
        statusV.push_back(monitorStart(handleV[i], monitorid));
        monitorIDV.push_back(monitorid);

        if ( (statusV[i] != ICAFE_NORMAL) && isGood) {
            overallStatus=statusV[i];
            isGood=false;
        }
    }

    // flush now!
	//ca_flush_io();
	channelMonitorPolicy.flushSendBufferNow();

	if (channelMonitorPolicy.getWhenToFlushSendBuffer() ==
		FLUSH_DESIGNATED_TO_CLIENT) {
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
int  Connect::monitorStart(vector<unsigned int> handleV, vector<int> &statusV,
                        vector<MonitorPolicy> &mpV) {
#define __METHOD__ \
     "monitorStart(vector<unsigned int> handleV, vector<int> &statusV, vector<MonitorPolicy> &mpV) "

     if (mpV.size() < handleV.size()) {
         //WARNING....
         cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
         cout << "WARNING VECTOR SIZE MISMATCH! vector<MonitorPolicy> has size " << mpV.size()
                 << " while handle vector size is " << handleV.size() << endl;
         cout << "Adding default MonitorPolicy to handles: " << endl;
         for (size_t i=mpV.size(); i < handleV.size(); ++i) {
             MonitorPolicy mp;
             mpV.push_back(mp);
             cout << i << " " ;
         }
         cout << endl;
     }

     statusV.clear();
     statusV.reserve(handleV.size());

     int  overallStatus=ICAFE_NORMAL; bool isGood=true;

	 //ChangePolicy TO FLUSH ONLY AT END!! (not yet tested!)
	 CAFENUM::ChannelWhenToFlushSendBufferPolicyKind previous=channelMonitorPolicy.getWhenToFlushSendBuffer() ;

	 if (previous != FLUSH_DESIGNATED_TO_CLIENT) {
			channelMonitorPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
	 }

     for (size_t i=0; i<handleV.size(); ++i) {

         statusV.push_back(monitorStart(handleV[i], mpV[i]));

         if ( (statusV[i] != ICAFE_NORMAL) && isGood) {
             overallStatus=statusV[i];
             isGood=false;
         }

     }

     // flush now!
	 //ca_flush_io();
	 channelMonitorPolicy.flushSendBufferNow();

	 if (channelMonitorPolicy.getWhenToFlushSendBuffer() ==
		 FLUSH_DESIGNATED_TO_CLIENT) {
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
                            MonitorPolicy * mpArray){
#define __METHOD__ \
     "monitorStart(unsigned int  * handleArray, unsigned int  nelem, int  * statusArray, MonitorPolicy *  mpArray "
     int  overallStatus=ICAFE_NORMAL; bool isGood=true;

	 //ChangePolicy TO FLUSH ONLY AT END!
	 CAFENUM::ChannelWhenToFlushSendBufferPolicyKind previous=channelMonitorPolicy.getWhenToFlushSendBuffer() ;

	 if (previous != FLUSH_DESIGNATED_TO_CLIENT) {
			channelMonitorPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
	 }

     for (unsigned int  i=0; i<nelem; ++i) {
         statusArray[i]=monitorStart(handleArray[i], mpArray[i]);
         if ( (statusArray[i] != ICAFE_NORMAL) && isGood) {
             overallStatus=statusArray[i];
             isGood=false;
         }
     }
     // flush now!
	 //ca_flush_io();
	 channelMonitorPolicy.flushSendBufferNow();

	 if (channelMonitorPolicy.getWhenToFlushSendBuffer() ==
		 FLUSH_DESIGNATED_TO_CLIENT) {
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
                            unsigned int  *  monitorIDArray) {
 #define __METHOD__ \
     "monitorStart(unsigned int  * handleArray, unsigned int  nelem, int  * statusArray, unsigned int  *  monitorIDArray"
     int  overallStatus=ICAFE_NORMAL; bool isGood=true;

	 //ChangePolicy TO FLUSH ONLY AT END!! (not yet tested!)

	 CAFENUM::ChannelWhenToFlushSendBufferPolicyKind previous=channelMonitorPolicy.getWhenToFlushSendBuffer() ;

	 if (previous != FLUSH_DESIGNATED_TO_CLIENT) {
			channelMonitorPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
	 }


     for (unsigned int  i=0; i<nelem; ++i) {
         statusArray[i]=monitorStart(handleArray[i], monitorIDArray[i]);
         if ( (statusArray[i] != ICAFE_NORMAL) && isGood) {
             overallStatus=statusArray[i];
             isGood=false;
         }
     }
     // flush now!
	 //ca_flush_io();

	 channelMonitorPolicy.flushSendBufferNow();
	 if (channelMonitorPolicy.getWhenToFlushSendBuffer() ==
		 FLUSH_DESIGNATED_TO_CLIENT) {		
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
 int  Connect::monitorStart(unsigned int handle, MonitorPolicy &mp){
 #define __METHOD__ "monitorStart(unsigned int handle, MonitorPolicy &mp)"

     status = ICAFE_NORMAL;

     cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
     cafeConduit_set_by_handle::iterator it_handle;
     it_handle = handle_index.find(handle);

     if (it_handle != handle_index.end()) {

         vector<MonitorPolicy> _mpV =(*it_handle).getMonitorPolicyVector();
         vector<MonitorPolicy>::iterator it;

         for (it = _mpV.begin(); it != _mpV.end(); ++it) {
             if ( (*it).getID()==mp.getID()) {

                 cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                 cout << "MONITOR POLICY OBJECT ALREADY IN USE!! " << endl;
                 cafeStatus.report(ECAFE_NULLEVID);
                 status=ECAFE_NULLEVID;
                 return status;
             }
         }

         vector<MonitorPolicy> _mpiwV =(*it_handle).getMonitorPolicyInWaitingVector();
         vector<MonitorPolicy>::iterator itiw;

         for (itiw = _mpiwV.begin(); itiw != _mpiwV.end(); ++itiw) {
             if ( (*itiw).getID()==mp.getID()) {

                 cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                 cout << "MONITOR POLICY OBJECT ALREADY IN WAITING!! " << endl;
                 cafeStatus.report(ECAFE_NULLEVID);
                 status=ECAFE_NULLEVID;
                 return status;
             }
         }

         // Check how many monitors have already been started.
         if (_mpV.size()>= MAX_NO_MONITORS_PER_CHANNEL) {
             cout << " HEY DUDE, YOU ALREADY HAVE " <<  MAX_NO_MONITORS_PER_CHANNEL
                  << " MONITORS ON THIS CHANNEL " << endl;
             cout << " I AM NOT GOING TO START ANOTHER ONE FOR YOU!! " << endl;
             return ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED;
         }


         //IS CHANNEL CONNECTED?
         //IF NOT DELAY START OF MONITOR

         if (!(*it_handle).isConnected()) {
				 
			 				// Check how many monitors are in aiting
         			if (_mpiwV.size()>= MAX_NO_MONITORS_PER_CHANNEL) {
             		cout << " HEY DUDE, YOU ALREADY HAVE " <<  MAX_NO_MONITORS_PER_CHANNEL
                  << " MONITORS WAITING TO START FOR THIS CHANNEL " << endl;
             		cout << " I AM NOT GOING TO ADD ANOTHER ONE FOR YOU!! " << endl;
             		return ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED;
         			}


             //check in to vector<monitorMap>
              if(MUTEX){cafeMutex.lock();}; //lock
             handle_index.modify(it_handle, change_monitorPolicyInWaitingInsert(mp));
              if(MUTEX){cafeMutex.unlock();}; //unlock

             return ICAFE_MONITOR_DELAYED_AS_CONN_DOWN;
         }

         //check mp and replace default values!
         if (mp.getNelem()==0) {
             mp.setNelem((*it_handle).getChannelRegalia().getNelem());
         }
         else {
             mp.setNelem(min(mp.getNelem(), (*it_handle).getChannelRegalia().getNelem()));
         }

         if (mp.getDataType() == CAFE_NOT_REQUESTED) {
             mp.setDataType((*it_handle).getChannelRegalia().getDataType());
         }

         if (mp.getUserArgs() == NULL) {
             mp.setUserArgs((void *) (long long)(*it_handle).getHandle());
         }


         status=(*it_handle).monitorStart(mp);
         mp.setStatus(status);

         if (status==ICAFE_NORMAL) {

             //What is the POLICY!??
             //To Flush or to Pend this is the question!

             if (channelMonitorPolicy.getWhenToFlushSendBuffer()==
                 FLUSH_AFTER_EACH_CHANNEL_SUBSCRIPTION) {
                 channelMonitorPolicy.flushSendBufferNow();               
             }


             if (mp.getEventID()==NULL) {
                 cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                 cafeStatus.report(ECAFE_NULLEVID);
                 return ECAFE_NULLEVID;
             }

             //check in to vector<monitorMap>
              if(MUTEX){cafeMutex.lock();}; //lock
             handle_index.modify(it_handle, change_monitorPolicyInsert(mp));
              if(MUTEX){cafeMutex.unlock();}; //unlock

             _mpV =(*it_handle).getMonitorPolicyVector();

         }
     }
     else {
		 		 if (printErrorPolicy.getInvalidHandle()) {
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
 int  Connect::monitorStart(unsigned int handle, unsigned int  & monitorID) {
 #define __METHOD__ "monitorStart(unsigned int handle, unsigned int  & monitorID)"

     status = ICAFE_NORMAL;
     evid eventID = NULL;
     monitorID = (unsigned int) 0;

     cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
     cafeConduit_set_by_handle::iterator it_handle;
     it_handle = handle_index.find(handle);

     if (it_handle != handle_index.end()) {

         vector<MonitorPolicy> _mpV =(*it_handle).getMonitorPolicyVector();

         // Check how many monitors have already been started.
         if (_mpV.size()>= MAX_NO_MONITORS_PER_CHANNEL) {
             cout << " HEY DUDE, YOU ALREADY HAVE " <<  MAX_NO_MONITORS_PER_CHANNEL
                  << " MONITORS ON THIS CHANNEL " << endl;
             cout << " I AM NOT GOING TO START ANOTHER ONE FOR YOU!! " << endl;
             return ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED;
         }


 //IS CHANNEL CONNECTED?
         //IF NOT DELAY START OF MONITOR

        if (!(*it_handle).isConnected()) {

        	 vector<MonitorPolicy> _mpiwV =(*it_handle).getMonitorPolicyInWaitingVector();
        
			 		 // Check how many monitors are in aiting
        	 if (_mpiwV.size()>= MAX_NO_MONITORS_PER_CHANNEL) {
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
         if ((*it_handle).getChannelRegalia().getCafeConnectionState() !=  ICAFE_CS_NEVER_CONN ) {
             mp.setDataType((*it_handle).getChannelRegalia().getDataType());
             mp.setNelem((*it_handle).getChannelRegalia().getNelem());
         }

         mp.setUserArgs((void *) (long long) (*it_handle).getHandle());

         status=monitorStart(handle, mp);

         monitorID=mp.getID();

         if (status==ICAFE_NORMAL) {

             eventID=mp.getEventID();

             if (eventID==NULL) {               
                 return ECAFE_NULLEVID;
             }

         }
     }
     else {
		 		 if (printErrorPolicy.getInvalidHandle()) {
           cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
           cafeStatus.report(ECAFE_INVALID_HANDLE);
				 }	 
         return ECAFE_INVALID_HANDLE;
     }

     return status;
 #undef __METHOD__
 }



 /**
  *  \brief  Stop all monitors
  *  \return ICAFE_NORMAL if all OK else ECAFE error
  */
 int  Connect::monitorStop(){
 #define __METHOD__ "monitorStop()"

    int  statusGroup = ECA_NORMAL;

    bool   isClearChannel   =false;

    if (ca_current_context() != NULL) {

        // Loop through all elements
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {

            //HandleHelper::clearMonitorAction(unsigned int _handle)

            status=monitorStop ((*itcs).getHandle() );

            if (status != ECA_NORMAL){
                    statusGroup = status;
            }
            isClearChannel=true;
        }
    }

	  if (isClearChannel) {		
        status = ca_pend_io(channelClosePolicy.getTimeout());		
        if (statusGroup == ECA_NORMAL) {
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
 int  Connect::monitorStop(ca_client_context * cctLocal){
 #define __METHOD__ "monitorStop(ca_client_context * cccLocal)"

     if (cctLocal == NULL ) {
         return ECAFE_NULLCONTEXT;
     }

     int  statusGroup = ECA_NORMAL;
     bool   isClearChannel   =false;

     // Loop through all elements
     for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {

         if ((*itcs).getClientContext() != cctLocal ) {           
             continue;
         }

         status=monitorStop ((*itcs).getHandle() );

         if (status != ECA_NORMAL){
             statusGroup = status;
         }
         isClearChannel=true;
     }

     if (isClearChannel) {
         status = ca_pend_io(channelClosePolicy.getTimeout());

         if (statusGroup == ECA_NORMAL) {
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
 int  Connect::monitorStop(vector<unsigned int> handleV, vector<int> &statusV){
 #define __METHOD__ "monitorStop(vector<unsigned int> handleV, vector<int> statusV)"

    statusV.clear();
    statusV.reserve(handleV.size());

    int  overallStatus=ICAFE_NORMAL; bool isGood=true;
    for (unsigned int  i=0; i<handleV.size(); ++i) {

        statusV.push_back(monitorStop(handleV[i]));
        if ( (statusV[i] != ICAFE_NORMAL) && isGood) {
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
 int  Connect::monitorStop(unsigned int  * handleArray, unsigned int  nelem, int  * statusArray){
 #define __METHOD__ "monitorStop(unsigned int  * handleArray, unsigned int  nelem)"

    int  overallStatus=ICAFE_NORMAL; bool isGood=true;
    for (unsigned int  i=0; i<nelem; ++i) {

        statusArray[i]=monitorStop(handleArray[i]);
        if ( (statusArray[i] != ICAFE_NORMAL) && isGood) {
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
 int  Connect::monitorStop(unsigned int handle){
 #define __METHOD__ "monitorStop(unsigned int  _handle)"

     status = ICAFE_NORMAL;

     cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
     cafeConduit_set_by_handle::iterator it_handle;
     it_handle = handle_index.find(handle);

     if (it_handle != handle_index.end()) {

         evid eventID=NULL;
         unsigned int  monitorID= (unsigned int) NULL;

         //Loop all round those in waiting and those that started!

         vector<MonitorPolicy>  _mpiwV =(*it_handle).getMonitorPolicyInWaitingVector();
         vector<MonitorPolicy>::iterator itiw;

         for (itiw = _mpiwV.begin(); itiw != _mpiwV.end(); ++itiw) {
                 //erase vector<monitorMap>
                  if(MUTEX){cafeMutex.lock();}; //lock
                 monitorID=(*itiw).getMonitorID();
                 handle_index.modify(it_handle, change_monitorPolicyInWaitingErase(monitorID));
                  if(MUTEX){cafeMutex.unlock();}; //unlock
         }


         vector<MonitorPolicy>  _mpV =(*it_handle).getMonitorPolicyVector();
         vector<MonitorPolicy>::iterator it;

         for (it = _mpV.begin(); it != _mpV.end(); ++it) {
             eventID=(*it).getEventID();
             monitorID=(*it).getMonitorID();
             //monitor has been removed
             if (eventID==NULL) {
                 cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                 cout << "Monitor never started or previously stopped " << endl;
                 cafeStatus.report(ECAFE_NULLEVID);
                 status=ECAFE_NULLEVID;
                 continue;
             }

			 //cout << "MS eventID  " << eventID << " monitorID " << monitorID
			 //		  << " for h= " << handle  <<endl;
             status=(*it_handle).monitorStop(eventID);


             if (status==ICAFE_NORMAL) {

                //JC It appears that ca_clear_subscription does not need a flush!?
                if (channelMonitorPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_SUBSCRIPTION) {
                    channelMonitorPolicy.flushSendBufferNow();
                }

                 if(MUTEX){cafeMutex.lock();}; //lock
                handle_index.modify(it_handle, change_monitorPolicyErase(monitorID));
                 if(MUTEX){cafeMutex.unlock();}; //unlock
             }
         }//for
     }
     else {
		 	   if (printErrorPolicy.getInvalidHandle()) {
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
 int  Connect::monitorStop(unsigned int handle, MonitorPolicy mp){
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
 int  Connect::monitorStop (unsigned int handle, unsigned int  monitorID) {
 #define __METHOD__ "monitorStop(unsigned int handle, unsigned int  monitorID)"

     status = ICAFE_NORMAL;

     cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
     cafeConduit_set_by_handle::iterator it_handle;
     it_handle = handle_index.find(handle);

     if (it_handle != handle_index.end()) {

         evid eventID=NULL;

         //Monitor ID  in waiting?
         vector<MonitorPolicy>  _mpiwV =(*it_handle).getMonitorPolicyInWaitingVector();
         vector<MonitorPolicy>::iterator itiw;

         for (itiw = _mpiwV.begin(); itiw != _mpiwV.end(); ++itiw) {
             if ( (*itiw).getID()==monitorID) {

                  if(MUTEX){cafeMutex.lock();}; //lock
                 handle_index.modify(it_handle, change_monitorPolicyInWaitingErase(monitorID));
                  if(MUTEX){cafeMutex.unlock();}; //unlock

                 return status;
             }
         }//for


         vector<MonitorPolicy>  _mpV =(*it_handle).getMonitorPolicyVector();
         vector<MonitorPolicy>::iterator it;

         for (it = _mpV.begin(); it != _mpV.end(); ++it) {
             if ( (*it).getID()==monitorID) {
                 eventID=    (*it).getEventID();

                 if (eventID==NULL) {
                     cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                     cafeStatus.report(ECAFE_NULLEVID);
                     status=ECAFE_NULLEVID;
                     return status;
                 }
                 else {
                     break;
                 }
             }
         }

         //monitor has been removed
         if (eventID==NULL) {
             cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
             cout << "Monitor never started or previously stopped " << endl;
             cafeStatus.report(ECAFE_NULLEVID);
             status=ECAFE_NULLEVID;
             return status;
         }

         status=(*it_handle).monitorStop(eventID);

         if (status==ICAFE_NORMAL) {

             //JC It appears that ca_clear_subscription does not need a flush!?

             if (channelMonitorPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_CHANNEL_SUBSCRIPTION) {
                 channelMonitorPolicy.flushSendBufferNow();
             }
             //erase vector<monitorMap>
              if(MUTEX){cafeMutex.lock();}; //lock
             handle_index.modify(it_handle, change_monitorPolicyErase(monitorID));
              if(MUTEX){cafeMutex.unlock();}; //unlock
         }
     }
     else {
		 	   if (printErrorPolicy.getInvalidHandle())	{
         		cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
         		cafeStatus.report(ECAFE_INVALID_HANDLE);
				 }
         return ECAFE_INVALID_HANDLE;
     }

     return status;
 #undef __METHOD__
 }


 /**
  *  \brief print status information of given handle
  *  \param handle input: handle to Conduit object \n
  *  \param  status input: reporting status \n
  *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE
  */
 int  Connect::printStatus(unsigned int handle, int  status) {
     cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
     cafeConduit_set_by_handle::iterator it_handle; it_handle = handle_index.find(handle);
     if (it_handle != handle_index.end()) {

         cout << "====================================" << endl;
         cout << "Handle=" << handle << " PV=" << (*it_handle).getPV() << endl;
         if ( (strcmp((*it_handle).getPV(), (*it_handle).getPVAlias())!=0)  ) {
             cout << "PVAlias=" << (*it_handle).getPVAlias() << endl;
         }         
         cafeStatus.report(status);
         cout << "====================================" << endl;
         return ICAFE_NORMAL;}
     else { 
		 		if (printErrorPolicy.getInvalidHandle()) {
		 			cout << handle << " is an invalid handle " << endl;
          cafeStatus.report(status);
				}	
        return ECAFE_INVALID_HANDLE;}
 }


 /**
  *  \brief print status information of given handle only on error
  *  \param handle input: handle to Conduit object \n
  *  \param  status input: reporting status \n
  *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE
  */
 int  Connect::printStatusIfError(unsigned int handle, int  status) {
     if (status==ICAFE_NORMAL) {return ICAFE_NORMAL;};
     return Connect::printStatus(handle, status);
 }


 /**
  *  \brief print status information of given handles
  *  \param handleV input: vector of handles to conduit objects \n
  *  \param statusV input: vector of statuses \n
  *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE (if one or more handles are invalid)
  */
 int  Connect::printStatus(vector<unsigned int> handleV, vector<int> statusV) {
     int  overallStatus=ICAFE_NORMAL; bool isGood=true;
     int  localStatus=ICAFE_NORMAL;
	 for (unsigned int  i=0; i < min(handleV.size(),statusV.size()); ++i) {
			localStatus=Connect::printStatus(handleV[i], statusV[i]);
            if(isGood && localStatus!=ICAFE_NORMAL) {overallStatus=localStatus; isGood=false;}
     }
     return overallStatus;
 }

 /**
  *  \brief print status information of given handles only on error
  *  \param handleV input: vector of Handles to Conduit objects \n
  *  \param statusV input: vector of statuses \n
  *  \return ECA_NORMAL if all OK else ECAFE_INVALID_HANDLE (if one or more handles are invalid)
  */
 int  Connect::printStatusIfError(vector<unsigned int> handleV, vector<int>  statusV) {
     int  overallStatus=ICAFE_NORMAL; bool isGood=true;
     int  localStatus=ICAFE_NORMAL;
	 for (unsigned int  i=0; i <min(handleV.size(),statusV.size()); ++i) {
		if (statusV[i]!=ICAFE_NORMAL) {
			localStatus=Connect::printStatus(handleV[i], statusV[i]);
            if(isGood && localStatus!=ICAFE_NORMAL) {overallStatus=localStatus; isGood=false;}
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
 int  Connect::printStatus(unsigned int  * handleArray, unsigned int  nelem, int  *statusArray) {
	 int  overallStatus=ICAFE_NORMAL; bool isGood=true;
	 int  localStatus=ICAFE_NORMAL;
	 for (unsigned int  i=0; i <nelem; ++i) {
			localStatus=Connect::printStatus(handleArray[i], statusArray[i]);
			if(isGood && localStatus!=ICAFE_NORMAL) {overallStatus=localStatus; isGood=false;}
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
 int  Connect::printStatusIfError(unsigned int  * handleArray, unsigned int  nelem, int  *statusArray) {
	 int  overallStatus=ICAFE_NORMAL; bool isGood=true;
	 int  localStatus=ICAFE_NORMAL;
	 for (unsigned int  i=0; i <nelem; ++i) {
		if (statusArray[i]!=ICAFE_NORMAL) {
			localStatus=Connect::printStatus(handleArray[i], statusArray[i]);
			if(isGood && localStatus!=ICAFE_NORMAL) {overallStatus=localStatus; isGood=false;}
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
 int  Connect::terminate(ca_client_context * cctLocal) {

     int  statusTerminate=ICAFE_NORMAL;

     if(cctLocal==NULL) {
        cout << "Aborting attempt at releasing channel access resources due to:" << endl;
        cafeStatus.report(ECAFE_NULLCONTEXT);
        return statusTerminate=ECAFE_NULLCONTEXT;
     }
		    
	 cout << "Stopping Monitors (if any)" << endl;
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
     if(contextDestroy(cctLocal)==ECAFE_NULLCONTEXT) {
         cout << "Aborting attempt at releasing channel access resources due to:" << endl;
         cafeStatus.report(ECAFE_NULLCONTEXT);
         statusTerminate=ECAFE_NULLCONTEXT;
     }
     else {
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
	 	 	
 
	 cout << "The CAFEteria is now closed for this ca context" << endl;
     return statusTerminate;
 };


 /**
  *  \brief Closes all channels within a given context and all handles for all contexts \n
  *  Shuts down the local channel access client context and frees allocated resources \n
  *  contextDestroy is not essential as resources are normally automatically released by the system.
  *  \return ECA_NORMAL if all OK or ECAFE_NULLCONTEXT if ca client context does not exist
  */
 int  Connect::terminate() {
   int  statusTerminate=ICAFE_NORMAL;	 
	 cout << "\nStopping Monitors (if any)" << endl;
	 monitorStop();
     // i.e. let callbacks fire before context is destroyed
	 				 
		#if HAVE_BOOST_THREAD
	    boost::this_thread::sleep_for(boost::chrono::microseconds(30000));	
	    //cout << "Closing All Channels and Removing All Handles" << endl;
		#else	
			 #if HAVE_LINUX
	     usleep(30000);
      #endif
		#endif	
   
	
	 
	 //closeChannels(); //Within current context only!
	 closeHandles(); //All contexts

	 groupHandleErase(); //erase groupHandles
	 cout << "All Channels Closed and Handles Removed" << endl;
	 
     // Give a few ticks to avoid epics race conditions  
   	 
		 #if HAVE_BOOST_THREAD
		  boost::this_thread::sleep_for(boost::chrono::microseconds(30000));
		 #else	
		   #if HAVE_LINUX
	     usleep(30000); 	
	 	   #endif
		 #endif
		 		 
	   
		 
     if(contextDestroy()==ECAFE_NULLCONTEXT) {
         cout << "Aborting attempt at releasing channel access resources due to:" << endl;
         cafeStatus.report(ECAFE_NULLCONTEXT);
         statusTerminate=ECAFE_NULLCONTEXT;
     }
     else {
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
	
	 
	 PVGroupV.erase (PVGroupV.begin(),PVGroupV.end());
	 PVGroupPseudo.erase (PVGroupPseudo.begin(),PVGroupPseudo.end());
	 deviceCollectionV.erase (deviceCollectionV.begin(),deviceCollectionV.end());
	 groupPseudoMap.erase (groupPseudoMap.begin(), groupPseudoMap.end()  );
	 
	 globalChannelList.erase(globalChannelList.begin(), globalChannelList.end() );
	 
	 //Reset (for MATLAB)
	 ConduitGroup::groupHandleNext=0;
	 Conduit::handleNext=0;
	 MonitorPolicy::idNext=0xfff;
	 
//#if HAVE_PYTHON_H
  //if HAVE_PYCAFE_EXT
 	//Py_Finalize();
	//endif
//#endif
	 
	 
	 
	 
	   
   cout << "The CAFEteria is now closed" << endl;
   return statusTerminate;
 };









