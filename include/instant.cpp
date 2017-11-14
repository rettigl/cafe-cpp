///
/// \file    instant.cpp
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef INSTANT_CPP
#define INSTANT_CPP

/**
 *  \brief Set values of data type CTYPE
 *  \param  _handle  input: handle to CAFEConduit object
 *  \param  _dbrType  input: chtype
 *  \param  _val  value/array of values of datatype const CTYPE
 *  \return ECA_NORMAL if all OK else CAFE_ or ECA_ error
 *  \return CAFE_ error if CAFEConduit::putCallback() operation is not to be executed:
 *  \return ECAFE_INVALID_HANDLE - Invalid reference handle
 *  \return ECAFE_NOWTACCESS - Write access denied
 *  \return ICAFE_CA_OP_CONN_DOWN - Channel is not connected
 *  \return ECA_ error from CAFEConduit::putCallback() operation:
 *  \return (*) should not arise due to pre-checks
 *  \return ECA_NORMAL       - Normal successful completion
 *  \return ECA_BADCHID      - Corrupted Channel Identifier (chid)
 *  \return ECA_BADTYPE (*)  - Invalid DBR_XXX type
 *  \return ECA_BADCOUNT (*) - Requested count larger than native element count
 *  \return ECA_NOWTACCESS (*) - Write access denied
 *  \return ECA_ALLOCMEM     - Unable to allocate memory
 *  \return ECA_DISCONN (*)  - Channel is disconnected
 */
template <class CTYPE> int  Instant<CTYPE>::set(const unsigned int  _handle, const chtype _dbrType,
                                                    const CTYPE * _val ) {
#define __METHOD__  "Instant::set(_handle, _dbrType, _val)"

     status=ICAFE_NORMAL;

     if (_dbrType > DBR_DOUBLE) {
         std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
         std::cout << "INTERNAL CAFE ERROR: HOW DID THIS PERCULIAR DATA TYPE: "
                 << dbr_type_to_text(_dbrType) << " GET THROUGH!" << endl;
     }

     cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
     cafeConduit_set_by_handle::iterator it_handle;
     it_handle = handle_index.find(_handle);

     if (it_handle != handle_index.end()) {

         if ( (status=cafeGranules.channelVerifyPut(_handle, _dbrType)) != ICAFE_NORMAL) {             
             return status;
         }

         if ( (status=cafeGranules.channelPreparePut(_handle)) != ICAFE_NORMAL) {             
             return status;
         }

         if ( (status=clientRequests(_handle, _dbrType, _val)) != ICAFE_NORMAL) {
            return status;
         }

         if ( (status=cafeGranules.channelExecutePut(_handle)) != ICAFE_NORMAL) {           
             return status;
         }

     }
     else {
         cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
         cafeStatus.report(ECAFE_INVALID_HANDLE);
         return ECAFE_INVALID_HANDLE;
     }

     if(MUTEX) {cafeMutex.lock();}; //lock
     handle_index.modify(it_handle, change_status(status));
     if(MUTEX) {cafeMutex.unlock();}; //unlock

     return status;

#undef __METHOD__
}

/**
 *  \brief Retrieve values of data type CTYPE
 *  \param  _handle  input:  handle to CAFEConduit object
 *  \param  _dbrType input:  chtype
 *  \param  _val     output: value/array of values of datatype const CTYPE
 *  \param  alarmStatus output
 *  \param  alarmSeverity output
 *  \param  ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else CAFE_ or ECA_ error
 *  \return CAFE_ error if CAFEConduit::get() operation is not to be executed:
 *  \return ECAFE_INVALID_HANDLE - Invalid reference handle
 *  \return ECAFE_NORDACCESS - Read access denied
 *  \return ICAFE_CA_OP_CONN_DOWN - Channel is not connected
 *  \return ECA_ error from CAFEConduit::get() operation:
 *  \return (*) should not arise due to pre-checks
 *  \return ECA_NORMAL       - Normal successful completion
 *  \return ECA_BADCHID      - Corrupted Channel Identifier (chid)
 *  \return ECA_BADTYPE (*)  - Invalid DBR_XXX type
 *  \return ECA_BADCOUNT (*) - Requested count larger than native element count
 *  \return ECA_NORDACCESS(*)- Read access denied
 *  \return ECA_ALLOCMEM     - Unable to allocate memory
 *  \return ECA_DISCONN (*)  - Channel is disconnected
 */
template <class CTYPE> int  Instant<CTYPE>::get(const unsigned int  _handle,
      const chtype _dbrType,  CTYPE * _val,
      dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) {
#define __METHOD__ "Instant::get(_handle, dbrType, _val, alarmStatus, alarmSeverity, ts) "

 		//cout << __FILE__ << " " << __LINE__ << " " << __METHOD__ << endl;

    status=ICAFE_NORMAL;

    if (_dbrType > DBR_CTRL_DOUBLE) {
        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        std::cout << "INTERNAL CAFE ERROR: HOW DID THIS PERCULIAR DATA TYPE: "
                << dbr_type_to_text(_dbrType) << " GET THROUGH!" << endl;          
    }
    else if (_dbrType > DBR_TIME_DOUBLE) {
        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        std::cout << "INTERNAL CAFE ERROR: METHOD NOT MEANT FOR THIS DATA TYPE: "
                << dbr_type_to_text(_dbrType) << endl;
        std::cout << "CAFE WILL THUS TAKE CONTROL OF ALLOCATING APPROPIATE DBR_TYPE" << endl;
    }

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;


    it_handle = handle_index.find(_handle);
		
	  if (it_handle != handle_index.end()) {

				

		    if ( (*it_handle).getChannelGetActionWhenMonitorPolicy().getActionKind() == CAFENUM::GET_FROM_CACHE) {
			    if ( helper.getNmonitorData(_handle) >0){
			      return Instant::getCache(_handle, _dbrType, _val, alarmStatus, alarmSeverity, ts);
			    }
		    }

        if ( (status=cafeGranules.channelVerifyGet(_handle, _dbrType)) != ICAFE_NORMAL) {
            return status;
        }

        //Here fill channelRequestMetaData

        if ( (status=cafeGranules.channelPrepareGet(_handle)) != ICAFE_NORMAL) {
            return status;
        }

        if ( (status=cafeGranules.channelExecuteGet(_handle)) != ICAFE_NORMAL) {
            return status;
        }

        channelRequestMetaDataClient = (*it_handle).getChannelRequestMetaDataClient();


        status=clientRequests( _handle,  channelRequestMetaDataClient.getDbrDataType(),
                       _val, alarmStatus, alarmSeverity, ts, false);

    }
    else {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }

    if(MUTEX) {cafeMutex.lock();}; //lock
    handle_index.modify(it_handle, change_status(status));
    if(MUTEX) {cafeMutex.unlock();}; //unlock

    return status;

#undef  __METHOD__
}


/**
 *  \brief Retrieve cached values of data type CTYPE
 *  \param  _handle  input:  handle to CAFEConduit object
 *  \param  _dbrType input:  chtype
 *  \param  _val     output: value/array of values of datatype const CTYPE
 *  \param  alarmStatus output
 *  \param  alarmSeverity output
 *  \param  ts output: epicsTimeStamp
 *  \return ECA_NORMAL if all OK else CAFE_ or ECA_ error
 *  \return CAFE_ error if CAFEConduit::get() operation is not to be executed:
 *  \return ECAFE_INVALID_HANDLE - Invalid reference handle
 *  \return ECAFE_NORDACCESS - Read access denied
 *  \return ICAFE_CA_OP_CONN_DOWN - Channel is not connected
 *  \return ECA_ error from CAFEConduit::get() operation:
 *  \return (*) should not arise due to pre-checks
 *  \return ECA_NORMAL       - Normal successful completion
 *  \return ECA_BADCHID      - Corrupted Channel Identifier (chid)
 *  \return ECA_BADTYPE (*)  - Invalid DBR_XXX type
 *  \return ECA_BADCOUNT (*) - Requested count larger than native element count
 *  \return ECA_NORDACCESS(*)- Read access denied
 *  \return ECA_ALLOCMEM     - Unable to allocate memory
 *  \return ECA_DISCONN (*)  - Channel is disconnected
 */
template <class CTYPE> int  Instant<CTYPE>::getCache(const unsigned int  _handle,
      const chtype _dbrType,  CTYPE * _val,
      dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts) {
#define __METHOD__ "Instant::getCache(_handle, dbrType, _val, alarmStatus, alarmSeverity, ts) "

		//cout << __FILE__ << " " << __LINE__ << " " << __METHOD__ << endl;

	  status=ICAFE_NORMAL;

    ts.secPastEpoch= 0; // default value if cache does not have timeStamp
    ts.nsec        = 0; // default value if cache does not have timeStamp
    alarmStatus    =-1; // default value if cache does not have alarmStatus
    alarmSeverity  =-1; // default value if cache does not have alarmSeverity

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end()) {

    //cout << __METHOD__ << "__" << __LINE__ << endl;
    //cout <<  "(*it_handle).getChannelGetCacheWaitPolicy().getWaitKind()" <<
		//(*it_handle).getChannelGetCacheWaitPolicy().getWaitKind() << " 0=NO_CHECK 1=NO_WAIT 2=WAIT " << endl;

  
		//meant for use in callbacks in monitors!
		if ( (*it_handle).getChannelGetCacheWaitPolicy().getWaitKind()	== CAFENUM::GET_CACHE_NO_CHECK) {
			status=clientRequests(_handle,
						   _dbrType, _val, alarmStatus, alarmSeverity, ts, true);
					 
			return status;
		}

 
        //ifNeverConnected - return error
        if ( (*it_handle).getChannelRegalia().getCafeConnectionState() == ICAFE_CS_NEVER_CONN) {	
						
            return ICAFE_CS_NEVER_CONN;
        }
 				else if ( (*it_handle).getChannelRegalia().getCafeConnectionState()==ICAFE_CS_CLOSED)  {
            return ICAFE_CS_CLOSED;
        }
				
 
 

        if (_dbrType > DBR_CTRL_DOUBLE) {
            std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
            std::cout << "INTERNAL CAFE ERROR: HOW DID THIS PERCULIAR DATA TYPE: "
                    << dbr_type_to_text(_dbrType) << " GET THROUGH!" << endl;
        }
        else if (_dbrType > DBR_TIME_DOUBLE) {
            std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
            std::cout << "INTERNAL CAFE ERROR: METHOD NOT MEANT FOR THIS DATA TYPE: "
                    << dbr_type_to_text(_dbrType) << endl;
            std::cout << "CAFE WILL THUS TAKE CONTROL OF ALLOCATING APPROPIATE DBR_TYPE" << endl;
        }

        chtype dbrtype =(*it_handle).getChannelRequestMetaData().getDbrDataType();


        //MetaData will have true DBR_XXX_YYY type
        //Client is asking from cache for DBR_MMM_NNN
        //Therefore have to prepare DBR_MMM_NNN from DBR_XXX_YY
        CAFENUM::DBR_TYPE cafedbrtype= (*it_handle).getChannelRequestMetaData().getCafeDbrType();


        switch (cafedbrtype)
        {
        case CAFENUM::DBR_TIME:
            dbrtype = dbf_type_to_DBR_TIME(_dbrType%DBR_STS_STRING);
            break;
        case CAFENUM::DBR_STS:
            dbrtype = dbf_type_to_DBR_STS(_dbrType%DBR_STS_STRING);
            break;
        case CAFENUM::DBR_PRIMITIVE:
            dbrtype = dbf_type_to_DBR(_dbrType%DBR_STS_STRING);
            break;
        default:
            dbrtype  = (*it_handle).getChannelRequestMetaData().getDbrDataType();
            cout << __METHOD__ << "/" << __LINE__  <<endl;
            cout << "CAFE INTERNAL FUNNY: This line should not appear!" << endl;
            break;
        }


        status=ICAFE_NORMAL;

        //What's in the buffer and is the status OK?
        //Is using Callback finished??
        // If No return
        if ( (status=cafeGranules.channelVerifyGet(_handle, dbrtype)) != ICAFE_NORMAL) {

            //See what's in buffer anyway!
           clientRequests(_handle,
                      _dbrType, _val, alarmStatus, alarmSeverity, ts, true);
											
          
           return status;
        }



		//Test callbackProgressKind
		channelRequestStatusGet =(*it_handle).getChannelRequestStatusGet();

		if (channelRequestStatusGet.getCallbackProgressKind() == CAFENUM::PENDING
			&& helper.getNmonitorData(_handle)==0
			&& (*it_handle).getChannelRequestPolicyGet().getMethodKind()
						!= CAFENUM::WITH_CALLBACK_USER_SUPPLIED 
			&& (*it_handle).getChannelGetCacheWaitPolicy().getWaitKind()	== CAFENUM::GET_CACHE_WAIT					
			&& (*it_handle).isConnected()
			
			) {

            channelTimeoutPolicyGet  = (*it_handle).getChannelTimeoutPolicyGet();

            ca_flush_io();
			//usleep(500); //wait 0.5ms to give time for callback to complete
            status=cafeGranules.waitForGetEvent(_handle, channelTimeoutPolicyGet.getTimeout());

            if (status==ECAFE_TIMEOUT && channelTimeoutPolicyGet.getSelfGoverningTimeout()) {
                unsigned short ntries=0;
                while (status==ECAFE_TIMEOUT && ntries<channelTimeoutPolicyGet.getNtries()) {
                    status=cafeGranules.waitForGetEvent(_handle, channelTimeoutPolicyGet.getTimeout() +
                                      channelTimeoutPolicyGet.getDeltaTimeout()*(++ntries));
                }

                std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                std::cout << "No of waitForGetEvent tries=" << ntries << std::endl;

                if (status==ECAFE_TIMEOUT) {
                    std::cout << "is the MAXIMUM allowed as configured through TimeoutPolicy! "  << std::endl;
					std::cout << "TURNING OFF SELF-GOVERNING TIMEOUT FOR GET OPERATIONS FOR THIS CHANNEL"  << std::endl;
					channelTimeoutPolicyGet.setSelfGoverningTimeout(false);
					channelTimeoutPolicyGet.setTimeout( channelTimeoutPolicyGet.getTimeout()); //not needed same as the old one
					std::cout << "AND TIMEOUT RESTORED TO START VALUE OF " << channelTimeoutPolicyGet.getTimeout() << endl;
                }
								else {
										std::cout << "Changing timeout for handle/pv "
						 						 << _handle << "/" << (*it_handle).getPV() << " to: "  <<

                    (channelTimeoutPolicyGet.getTimeout() +
                     channelTimeoutPolicyGet.getDeltaTimeout()*ntries) << " seconds"  <<endl;

										//modify timeout for handle
					 						channelTimeoutPolicyGet.setTimeout( (channelTimeoutPolicyGet.getTimeout() +
                                                      channelTimeoutPolicyGet.getDeltaTimeout()*ntries));
									}
									
								if(MUTEX){cafeMutex.lock();};     //lock
                	handle_index.modify(it_handle, change_channelTimeoutPolicyGet(channelTimeoutPolicyGet));
                if(MUTEX){cafeMutex.unlock();};   //unlock

								if (status==ECAFE_TIMEOUT) {
									std::cout << "CURRENT STATUS OF HANDLE: " << std::endl;
									helper.printHandle(_handle);
								}
            }


            if (status != ICAFE_NORMAL) {
                //Check these in at end of routine
                channelRequestStatusGet.setCallbackStatus    (status);

                channelRequestStatusGet.setCallbackKind(false, false); // NOT_INITIATED NOT_TRIGGERED

                if(MUTEX){cafeMutex.lock();};   //lock
                handle_index.modify(it_handle,
                    change_channelRequestStatusGet(channelRequestStatusGet));

                if(MUTEX){cafeMutex.unlock();}; //unlock
            }

            if(MUTEX){cafeMutex.lock();};   //lock
            handle_index.modify(it_handle, change_status(status));
            if(MUTEX){cafeMutex.unlock();}; //unlock


            if (status != ICAFE_NORMAL) {
								cout << __METHOD__ << " status= " << status << endl;
                cout << "Returning ICAFE_WAITING_FOR_PREV_CALLBACK " << endl;
                //Why is this required - See what's in buffer anyway!
                clientRequests(_handle,
                          _dbrType, _val, alarmStatus, alarmSeverity, ts, true);
																					
                return 	ICAFE_WAITING_FOR_PREV_CALLBACK;
            }

            //ca_flush_io();
            //return ICAFE_CALLBACK_NOT_YET_INVOKED;
        }//if


        channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
        status=channelRequestStatusGet.getMessageStatus();

        if (status != ICAFE_NORMAL) {
            cafeStatus.report(status);
            //Why is this required - See what's in buffer anyway!
            clientRequests(_handle,
                      _dbrType, _val, alarmStatus, alarmSeverity, ts, true);
										
            return status;
        }

        status=clientRequests(_handle,
                       _dbrType, _val, alarmStatus, alarmSeverity, ts, true);
											        
    }

    else {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return status;


#undef  __METHOD__
}



/**
 *  \brief retrieve cached data from an array of handles
 *  \param handleArray input: Array of handles to conduit object
 *  \param nelem input: Size of array
 *  \param _dbrType input: chtype
 *  \param val  output: Array of doubles; if handle points to a wafeform only first element is read out
 *  \param statusArray output: Status array reporting from each get(handle)
 *  \param alarmStatus output
 *  \param alarmSeverity output
 *  \param ts output: epicsTimeStamp
 *  \return ICAFE_NORMAL if all OK else the first ECAFE error encountered
 */
template <class CTYPE> int  Instant<CTYPE>::getCache(const unsigned int *handleArray, const unsigned int  nelem,
const chtype _dbrType,  CTYPE * val, int  *statusArray,
dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts) {
#define __METHOD__ "Instant::getCache(handleArray, nelem, _dbrType, val, statusArray) "

    unsigned int  nelemPrevious=1;
    HandleHelper helper;
    int  gStatus=ICAFE_NORMAL;
    bool statusSet=false; 

    //cout << __FILE__ << " " << __LINE__ << " " << __METHOD__ << endl;

    for (unsigned int  i=0; i<nelem; ++i) {

			

				//Very possibly waveforems will need longer to report back!
        if(helper.getNelemNative(handleArray[i])!=1) {

            cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
            cafeConduit_set_by_handle::iterator it_handle;
            it_handle = handle_index.find(handleArray[i]);

            channelRequestStatusGet =(*it_handle).getChannelRequestStatusGet();

						if (channelRequestStatusGet.getCallbackProgressKind() == CAFENUM::PENDING
								&& helper.getNmonitorData(handleArray[i])==0 
								&& (*it_handle).getChannelRequestPolicyGet().getMethodKind()
									!= CAFENUM::WITH_CALLBACK_USER_SUPPLIED 
								&& (*it_handle).getChannelGetCacheWaitPolicy().getWaitKind() == CAFENUM::GET_CACHE_WAIT
								&& (*it_handle).isConnected()
						) {

                channelTimeoutPolicyGet  = (*it_handle).getChannelTimeoutPolicyGet();

                ca_flush_io();
                status=cafeGranules.waitForGetEvent(handleArray[i], channelTimeoutPolicyGet.getTimeout());


                if (status==ECAFE_TIMEOUT && channelTimeoutPolicyGet.getSelfGoverningTimeout()) {
                    unsigned short ntries=0;
                    while (status==ECAFE_TIMEOUT && ntries<channelTimeoutPolicyGet.getNtries()) {
                        status=cafeGranules.waitForGetEvent(handleArray[i], channelTimeoutPolicyGet.getTimeout() +
                                          channelTimeoutPolicyGet.getDeltaTimeout()*(++ntries));
                    }


                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << "No of waitForGetEvent tries=" << ntries << std::endl;

                    if (status==ECAFE_TIMEOUT) {
                        std::cout << "is the MAXIMUM allowed as configured through TimeoutPolicy! "  << std::endl;
												std::cout << "TURNING OFF SELF-GOVERNING TIMEOUT FOR GET OPERATIONS FOR THIS CHANNEL"  << std::endl;
												channelTimeoutPolicyGet.setSelfGoverningTimeout(false);
												channelTimeoutPolicyGet.setTimeout( channelTimeoutPolicyGet.getTimeout() ); //not needed really(!)
												std::cout << "AND TIMEOUT RESTORED TO START VALUE OF " << channelTimeoutPolicyGet.getTimeout() << endl;
                    }
								else {
										std::cout << "Changing timeout for handle/pv "
							 		 << handleArray[i] << "/" << (*it_handle).getPV() << " to: "  <<

                        (channelTimeoutPolicyGet.getTimeout() +
                         channelTimeoutPolicyGet.getDeltaTimeout()*ntries) << " seconds"  <<endl;

										//modify timeout for handle
											channelTimeoutPolicyGet.setTimeout( (channelTimeoutPolicyGet.getTimeout() +
                                                          channelTimeoutPolicyGet.getDeltaTimeout()*ntries));
								}
                   
								if(MUTEX){cafeMutex.lock();};     //lock
          			handle_index.modify(it_handle, change_channelTimeoutPolicyGet(channelTimeoutPolicyGet));
          			if(MUTEX){cafeMutex.unlock();};   //unlock
					
								if (status==ECAFE_TIMEOUT) {
									std::cout << "CURRENT STATUS OF HANDLE: " << std::endl;
									helper.printHandle(handleArray[i]);
								}
					 }

						if (status != ICAFE_NORMAL) {
                //Check these in at end of routine
                channelRequestStatusGet.setCallbackStatus    (status);

                channelRequestStatusGet.setCallbackKind(false, false); // NOT_INITIATED NOT_TRIGGERED

                if(MUTEX){cafeMutex.lock();};   //lock
                handle_index.modify(it_handle,
                        change_channelRequestStatusGet(channelRequestStatusGet));

								if(MUTEX){cafeMutex.unlock();}; //unlock
                }

								if(MUTEX){cafeMutex.lock();};   //lock
                handle_index.modify(it_handle, change_status(status));
                if(MUTEX){cafeMutex.unlock();}; //unlock


                if (status != ICAFE_NORMAL) {
                    cout << __METHOD__ << " status " << status << endl;
                    cout << "Returning ICAFE_WAITING_FOR_PREV_CALLBACK " << endl;
                    return ICAFE_WAITING_FOR_PREV_CALLBACK;
                }

                //ca_flush_io();
                //return ICAFE_CALLBACK_NOT_YET_INVOKED;
        		}//if status

            nelemPrevious=helper.getNelemNative(handleArray[i]);
            helper.setNelemToRetrieveFromCache(handleArray[i],1);
			}; //big if



        dbr_short_t  a1, a2; epicsTimeStamp ts1;

        statusArray[i]=Instant::getCache(handleArray[i], _dbrType, &val[i],
                                         a1, a2, ts1);


        chtype channelType= _dbrType;

        if (dbr_type_is_TIME(channelType)) {
            ts[i].secPastEpoch= ts1.secPastEpoch; // default value if cache does not have timeStamp
            ts[i].nsec        = ts1.nsec;        // default value if cache does not have timeStamp
        }
        if (dbr_type_is_TIME(channelType) ||  dbr_type_is_STS(channelType) ) {
            alarmStatus  [i]  = a1;   // default value if cache does not have alarmStatus
            alarmSeverity[i]  = a2;   // default value if cache does not have alarmSeverity
        }

        if (statusArray[i] != ICAFE_NORMAL && !statusSet) {
            gStatus=statusArray[i];
            statusSet=true;
        }
        if(helper.getNelemNative(handleArray[i])!=1) {
            helper.setNelemToRetrieveFromCache(handleArray[i],nelemPrevious);
        };

    } //for
    return gStatus;

#undef  __METHOD__
}

/**
 *  \brief Sends ca_put messages to IOC and waits until end before flushing
 *  \param handleArray input: Array of handles to conduit object
 *  \param nelem input: Size of array
 *  \param _dbrType input: chtype
 *  \param val input: Array of CTYPE
 *  \param statusArray output: Status array reporting from each get(handle)
 *  \return overallStatus ICAFE_NORMAL if all OK else the first ECAFE error encountered
 */
template <class CTYPE> int  Instant<CTYPE>::set(const unsigned int  *handleArray,  const unsigned int  nelem,
                                                const chtype _dbrType,  const  CTYPE * val, int  *statusArray) {
#define __METHOD__ \
"Instant::set(unsigned int  *handleArray,  unsigned int  nelem, const chtype _dbrType, dbr_double_t * val, int  *statusArray)"

    int  overallStatus=ICAFE_NORMAL; bool isGood=true;

    unsigned int  nelemPrevious=1;

    for (unsigned int  i=0; i<nelem; ++i) {

        if(helper.getNelemNative(handleArray[i])!=1) {
				
				    //cout << "Native =" << helper.getNelemNative(handleArray[i]) << endl;
						//cout << "Client =" << helper.getNelemClient(handleArray[i]) << endl;
						//cout << "Request =" << helper.getNelemRequest(handleArray[i]) << endl;
						//getNelemClient replaces getNelemNative, as this is required in e.g. SetScalarArray from MATLAB 
						//whenever an offset for a wf is given. e.g. offset 12, setnelm=20, gives client=20, request=32, native=256
            nelemPrevious=helper.getNelemClient(handleArray[i]); 
            helper.setNelem(handleArray[i],1);
        }



        //OveridePolicy
        //NO_WAIT, FLUSH_DESIGNATED_TO_CLIENT

        ChannelRequestPolicy originalPol;
        ChannelRequestPolicy pol;
        PolicyHelper policyHelper;
        policyHelper.getChannelRequestPolicyPut(handleArray[i], originalPol);
        policyHelper.getChannelRequestPolicyPut(handleArray[i], pol);
        pol.setWaitKind(NO_WAIT);
        pol.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
        policyHelper.setChannelRequestPolicyPut(handleArray[i], pol);
        statusArray[i]=Instant::set(handleArray[i],  _dbrType, &val[i]);
        if ( (statusArray[i] != ICAFE_NORMAL) && isGood) {
            overallStatus=statusArray[i];
            isGood=false;
        }
        //Return policy to previous
        policyHelper.setChannelRequestPolicyPut(handleArray[i], originalPol);

        if(helper.getNelemNative(handleArray[i])!=1) {
            helper.setNelem(handleArray[i],nelemPrevious);
        };
    }
    // flush now!

    ca_flush_io();

    return overallStatus;
#undef __METHOD__
};


/**
 *  \brief Data converted into datatype requested by client
 *  \param _handle input: handle
 *  \param _dbrType input: chtype
 *  \param _val input:array of CTYPE
 *  \param  alarmStatus output
 *  \param  alarmSeverity output
 *  \param  ts output: epicsTimeStamp
 *  \param isCacheRequest: input: bool true if cached data requested
 *  \return status ICAFE_NORMAL if all OK else the first ECAFE error encountered
 */
template <class CTYPE> int  Instant<CTYPE>::clientRequests(
      const unsigned int  _handle,
      const chtype _dbrType,  CTYPE * _val,
      dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts, bool isCacheRequest) {
#define __METHOD__ "Instant::clientRequests(*it_handle, _dbrType, _val, alarmStatus, alarmSeverity, ts) "

    ts.secPastEpoch= 0; // default value if cache does not have timeStamp
    ts.nsec        = 0; // default value if cache does not have timeStamp
    alarmStatus    =-1; // default value if cache does not have alarmStatus
    alarmSeverity  =-1; // default value if cache does not have alarmSeverity

    //this is what the client is requesting!
		
	  int status=ICAFE_NORMAL; //Use local value!!
		
    switch( _dbrType) {

    case DBR_TIME_STRING:  //0
        status = renderString.get(  _handle, (dbr_string_t *) _val, alarmStatus, alarmSeverity, ts, isCacheRequest);
        break;
    case DBR_TIME_SHORT:   //1
        status = renderShort.get (  _handle, (dbr_short_t *) _val, alarmStatus, alarmSeverity, ts, isCacheRequest);
        break;
    case DBR_TIME_FLOAT:   //2
        status = renderFloat.get ( _handle,  (dbr_float_t *) _val, alarmStatus, alarmSeverity, ts, isCacheRequest);
        break;
    case DBR_TIME_ENUM:    //3
        status = renderEnum.get  ( _handle,  (dbr_enum_t *) _val, alarmStatus, alarmSeverity, ts, isCacheRequest);
        break;
    case DBR_TIME_CHAR:    //4
        status = renderChar.get  ( _handle,  (dbr_char_t *) _val, alarmStatus, alarmSeverity, ts, isCacheRequest);
        break;
    case DBR_TIME_LONG:    //5
        status = renderLong.get  ( _handle,  (dbr_long_t *) _val, alarmStatus, alarmSeverity, ts, isCacheRequest);
        break;
    case DBR_TIME_DOUBLE:  //6
        status = renderDouble.get( _handle,  (dbr_double_t *) _val, alarmStatus, alarmSeverity, ts, isCacheRequest);
        break;
    case DBR_STS_STRING:  //0
        status = renderString.get( _handle, (dbr_string_t *) _val, alarmStatus, alarmSeverity, isCacheRequest);
        break;
    case DBR_STS_SHORT:   //1
        status = renderShort.get ( _handle,  (dbr_short_t *) _val, alarmStatus, alarmSeverity, isCacheRequest);
        break;
    case DBR_STS_FLOAT:   //2
        status = renderFloat.get ( _handle,  (dbr_float_t *) _val, alarmStatus, alarmSeverity, isCacheRequest);
        break;
    case DBR_STS_ENUM:    //3
        status = renderEnum.get  ( _handle,  (dbr_enum_t *) _val, alarmStatus, alarmSeverity, isCacheRequest);
        break;
    case DBR_STS_CHAR:    //4
        status = renderChar.get  ( _handle,  (dbr_char_t *) _val, alarmStatus, alarmSeverity, isCacheRequest);
        break;
    case DBR_STS_LONG:    //5
        status = renderLong.get  ( _handle,  (dbr_long_t *) _val, alarmStatus, alarmSeverity, isCacheRequest);
        break;
    case DBR_STS_DOUBLE:  //6
        status = renderDouble.get( _handle,  (dbr_double_t *) _val, alarmStatus, alarmSeverity, isCacheRequest);
        break;
    case DBR_STRING:  //0
        status = renderString.get( _handle,  (dbr_string_t *) _val, isCacheRequest);
        break;
    case DBR_SHORT:   //1
        status = renderShort.get ( _handle,   (dbr_short_t *) _val, isCacheRequest);
        break;
    case DBR_FLOAT:   //2
        status = renderFloat.get ( _handle,  (dbr_float_t *) _val, isCacheRequest);
        break;
    case DBR_ENUM:    //3
        status = renderEnum.get  ( _handle,  (dbr_enum_t *) _val, isCacheRequest);
        break;
    case DBR_CHAR:    //4
        status = renderChar.get  ( _handle,  (dbr_char_t *) _val, isCacheRequest);
        break;
    case DBR_LONG:    //5
        status = renderLong.get  ( _handle,  (dbr_long_t *) _val, isCacheRequest);
        break;
    case DBR_DOUBLE:  //6
        status = renderDouble.get( _handle,  (dbr_double_t *) _val, isCacheRequest);
        break;

    default:
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << "Method does not support this DBR_TYPE: " << dbr_type_to_text(_dbrType)
             << " This line should never appear!" << endl;
        break;
    }

return status;
#undef  __METHOD__
}




/**
 *  \brief Data converted from client dataType into native datatype required by put Method
 *  \param _handle input: handle
 *  \param _dbrType input: chtype
 *  \param _val input:array of CTYPE
 *  \return status ICAFE_NORMAL if all OK else the first ECAFE error encountered
 */

template <class CTYPE> int  Instant<CTYPE>::clientRequests(
    const unsigned int  _handle, const chtype _dbrType,  const CTYPE * _val) {
#define __METHOD__ "Instant::clientRequests(*it_handle, _dbrType, _val)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    status=ICAFE_NORMAL;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end()) {

        union db_access_val * PVDataL;
        unsigned int   nelem;
        //unsigned int   offset;
        chtype dbrTypeRequest_DataBuffer;

        PVDataL  = (*it_handle).getPutBuffer  ();
        nelem    = (*it_handle).getChannelRequestMetaPrimitive().getNelem();

        dbrTypeRequest_DataBuffer = (*it_handle).getChannelRequestMetaPrimitive().getDbrDataType();

        switch(_dbrType) {
        case DBR_STRING:
            status=renderString.putString(_handle, (dbr_string_t *) _val);
            break;
        case DBR_SHORT:
            status=renderShort.put(_handle, (dbr_short_t *) _val, _dbrType );
            break;
        case DBR_FLOAT:
            status=renderFloat.put(_handle, (dbr_float_t *)  _val, _dbrType);
            break;
        case DBR_ENUM:
            status=renderEnum.put(_handle, (dbr_enum_t *)  _val, _dbrType);
            break;
        case DBR_CHAR:
            status=renderChar.put(_handle,  (dbr_char_t *) _val, _dbrType);
            break;
        case DBR_LONG:
            status=renderLong.put(_handle,  (dbr_long_t *) _val, _dbrType);
            break;
        case DBR_DOUBLE:
            status=renderDouble.put(_handle, (dbr_double_t *) _val, _dbrType);
            break;
        default:
            cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
            cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" <<
                _dbrType <<endl;
            cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
            return ECAFE_INVALID_SWITCH_CASE;
            break;
        }
		/*
		epicsTimeStamp ts;
		ts.secPastEpoch= 0; // default value if cache does not have timeStamp
		ts.nsec        = 0; // default value if cache does not have timeStamp
		dbr_short_t alarmStatus    =-1; // default value if cache does not have alarmStatus
		dbr_short_t alarmSeverity  =-1; // default value if cache does not have alarmSeverity
		//helper function to set TimeStamps!
		helper.setSTS(_handle, alarmStatus, alarmSeverity, ts);
		*/
    }

    else {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
return status;
#undef  __METHOD__
}



/**
 *  \brief Set followed by an immediate get
 *  \param handleSet input: handle
 *  \param dbrType input: const chtype
 *  \param valSet input:  value to set
 *  \param valGet output: value to get
 *  \return ECA_NORMAL if all OK else first ECAFE error encountered, else ICAFE_SET_AND_GET_MISMATCH;
 */
template <class CTYPE> int  Instant<CTYPE>::setAndGet(const unsigned int handleSet, const chtype dbrType, CTYPE valSet, CTYPE &valGet) {
#define __METHOD__ "Instant<CTYPE>::setAndGet(const unsigned int handleSet,const chtype dbrType, CTYPE valSet, CTYPE &valGet"
        //CheckPolicy

        CTYPE valGetA[1];  CTYPE valSetA[1];

        valGetA[0]=0; valGet=0;

 				valSetA[0] = (CTYPE) valSet;

        status=Instant::set(handleSet, dbrType, valSetA);
		  if (status==ICAFE_NORMAL) {

			unsigned int  nelemPrevious, nelemRequestedCheck=0;
			unsigned int  nelemRequested=1;
			nelemPrevious=helper.getNelemClient(handleSet);
			//Check the number of elements requested?
			if (nelemPrevious>1) {
				nelemRequestedCheck = helper.setNelem(handleSet,nelemRequested);
				if (nelemRequestedCheck != nelemRequested) {
					cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
					cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
							<< nelemPrevious << endl;
					cout << "to: " << nelemRequested << " but got instead: "
							<< nelemRequestedCheck  << endl;
				}
			}
			status=Instant::get(handleSet, dbrType, valGetA);

			if (nelemPrevious>1) {
				helper.setNelem(handleSet,nelemPrevious);
			}

            valGet=valGetA[0];
        }
        else {
            return status;
        }
        if(valSet==valGet) {
            return ICAFE_NORMAL;
        }
		cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
		cout << "Process Variable = " << helper.getPVFromHandle(handleSet) << endl;
		cout << "Set Value: " << valSet << " Get Value: " << valGet << endl;
        return ICAFE_SET_AND_GET_MISMATCH;
#undef __METHOD__
}



template <class CTYPE> int  Instant<CTYPE>::setMany(vector<unsigned int> handleSet, const chtype dbrType, vector<CTYPE> valSet, bool printFlag) {

#define __METHOD__ "Instant<CTYPE>::setMany(vector<unsigned int>handleSet, chtype, vector<CTYPE>valSet)"

    CAFEStatus cstat;

    CTYPE valSetA[1];

		status=ICAFE_NORMAL;
   
    if (handleSet.size() != valSet.size() ) {
		   return ECAFE_HANDLE_MISMATCH_SET_AND_MATCH;
		}
		
		for (size_t i=0; i< handleSet.size(); ++i) {

    	if (!helper.isChannelConnected(handleSet[i])) {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "NOT ALL CHANNELS CONNECTED: " << endl;
        if (!helper.isChannelConnected(handleSet[i])) {
				  helper.printHandle(handleSet[i]); 
				  status=helper.getStatus(handleSet[i]);
				}       
    	}		
	  
		}
	
		
		if (status!=ICAFE_NORMAL) {return status;}
	
	  if(printFlag) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
		}
	
	
	 	if (printFlag) {
		
			for (size_t i=0; i< handleSet.size(); ++i) {	
							
				if (dbrType==DBR_CHAR) {				
       		 cout << "SETTING  PV=" << helper.getPVFromHandle(handleSet[i]) << " to " << (unsigned short) valSet[i] << endl; 
				}       
				else {				
       	   cout << "SETTING  PV=" << helper.getPVFromHandle(handleSet[i]) << " to " << valSet[i] << endl;
				}
							
  	  } //for
   
		} //if
		
						
		for (size_t i=0; i< handleSet.size(); ++i) {
		
		
		
    	//set No of Elements to 1

    	unsigned int  nelemPrevious, nelemRequestedCheck=0;
    	unsigned int  nelemRequested=1;
					
    	nelemPrevious=helper.getNelemClient(handleSet[i]);
    	//Check the number of elements requested?
    	if (nelemPrevious>1) {
        nelemRequestedCheck = helper.setNelem(handleSet[i],nelemRequested);
        if (nelemRequestedCheck != nelemRequested) {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
                    << nelemPrevious << endl;
            cout << "to: " << nelemRequested << " but got instead: "
                    << nelemRequestedCheck  << endl;
        }
    	}
		
		
		
    	//policy set synchronous
    	ChannelRequestPolicy polPrevious, polNow;

    	policyHelper.getChannelRequestPolicyPut(handleSet[i], polPrevious);

   		polNow.setMethodKind(WITHOUT_CALLBACK);
    	polNow.setWaitKind(WAIT);
    	polNow.setWhenToFlushSendBuffer(FLUSH_AUTOMATIC);

    	policyHelper.setChannelRequestPolicyPut(handleSet[i], polNow);
     
      valSetA[0] = (CTYPE) valSet[i];

    	status=set(handleSet[i], dbrType, valSetA);

    	if (status!=ICAFE_NORMAL) {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cstat.report(status);
    	}

    	policyHelper.setChannelRequestPolicyPut(handleSet[i], polPrevious);
			
			
    	unsigned int  nelemPreviousCheck=nelemPrevious;
    	nelemRequested=1;
    	//Switch back to previous value
    	//if (nelemPrevious>1) {
    	if(helper.getNelemRequest(handleSet[i])!= nelemPrevious) {
        nelemPreviousCheck= helper.setNelem(handleSet[i],nelemPrevious);
        if (nelemPreviousCheck != nelemPrevious) {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Internal CAFE FUNNY: Wanted to re-set the no. elements from: "
                    << nelemRequested << endl;
            cout << "to the previous: " << nelemPrevious << " but got instead: "
                    << nelemPreviousCheck  << endl;
        }
    	}
			

		} //for size_t


return status;
} 
#undef __METHOD__
	


/**
 *  \brief Set Channels followed by a corresponding readback of Channels \n
 *  Method verifies whether or not the set/readback values agree within the given tolerance and timeout \n
 *  Method returns with ECA_NORMAL as soon as a match is reached
 *  \param handleSet input: vector of handleSet
 *  \param dbrType input: chtype of set value
 *  \param valSet input:  vector of CTYPE values to set
 *  \param handleMatch input: vector of handles of readback channel
 *  \param tolerance input: CTYPE of tolerance  margin
 *  \param timeout input:  double value; maximum time allowed for agreement to occur
 *  \param printFlag input: bool, set to true for diagnostics
 *  \return ECA_NORMAL if all OK, else first ECAFE error encountered, else ECAFE_TIMEOUT_SET_AND_MATCH
 */
 																										
template <class CTYPE> int  Instant<CTYPE>::compareAndMatchMany(vector<unsigned int> handleSet, const chtype dbrType, vector<CTYPE> valSet, vector<unsigned int> handleMatch, 
															CTYPE tolerance, double timeout, bool printFlag){ 
//template <class CTYPE> int  Instant<CTYPE>::setTriggerAndMatchMany(vector<unsigned int> handleSet, chtype dbrType, vector<CTYPE> valSet, 
//											vector<unsigned int> handleAction, vector<string> valAction, vector<unsigned int> handleMatch,
//                     CTYPE tolerance, double timeout, bool printFlag){
#define __METHOD__ "Instant<CTYPE>::compareAndMatchMany(chtype, vector<CTYPE>valSet, vector<unsigned int>handleMatch, tolerance, timeout, printFlag)"


    Connect connect;
    CAFEStatus cstat;

	CTYPE valGetA[1];  //CTYPE valSetA[1];

		status=ICAFE_NORMAL;

   
    if (handleMatch.size() != valSet.size() ) {
		   return ECAFE_HANDLE_MISMATCH_SET_AND_MATCH;
		}
		
		for (size_t i=0; i< handleMatch.size(); ++i) {

    	if (!helper.isChannelConnected(handleMatch[i])) {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "NOT ALL CHANNELS CONNECTED: " << endl;
               
                if (!helper.isChannelConnected(handleMatch[i])) {helper.printHandle(handleMatch[i]); status=helper.getStatus(handleMatch[i]);}
    	}		
	  
		}
	
		
		if (status!=ICAFE_NORMAL) {return status;}
	
	  if(printFlag) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
		}
	
		if (printFlag) {
		
		
			for (size_t i=0; i< handleMatch.size(); ++i) {	
		
						
				if (dbrType==DBR_CHAR) {				
       		cout << "SETTING  PV= " << helper.getPVFromHandle(handleSet[i]) << " to " << (unsigned short) valSet[i] << endl; 
				}       
				else {				
       	  cout << "SETTING  PV= " << helper.getPVFromHandle(handleSet[i]) << " to " << valSet[i] << endl;
				}
				
        cout << "READBACK PV=" << helper.getPVFromHandle(handleMatch[i])
             << " tolerance= " << fabs( (double) tolerance) << endl;
        cout << "TIME ALLOWED FOR MATCH IS " << timeout << " (sec) " << endl;
							
  	  } //for
   
		} //if

		
		vector<CTYPE> valSetUpper; vector<CTYPE> valSetLower;
	
		vector<unsigned int> monitorID; 
		vector<int> statMonitor;
		vector<unsigned short> nMonitors;
		vector<CTYPE> valGet;
	
			
		vector<unsigned int> nelemPreviousCache;	
							
		valSetUpper.reserve( valSet.size());
		
		
		valSetLower.reserve( valSet.size());
		monitorID.reserve( valSet.size());
		statMonitor.reserve( valSet.size());
		valGet.reserve( valSet.size());
		nMonitors.reserve( valSet.size() );
		
		
		nelemPreviousCache.reserve( valSet.size());
		
	
		for (size_t i=0; i< valSet.size(); ++i) {
		
					
    	switch (dbrType) {
    		case DBR_LONG :
    		case DBR_SHORT :
    		case DBR_ENUM:
        	valSetUpper[i]  = valSet[i] + abs((int)tolerance);
        	valSetLower[i]  = valSet[i] - abs((int)tolerance);
        break;
    		case DBR_CHAR :
        	valSetUpper[i]  = (unsigned short) valSet[i] + abs((unsigned short) tolerance);
        	valSetLower[i]  = (unsigned short) valSet[i] - abs((unsigned short) tolerance);
        break;
    		case DBR_FLOAT:
    		case DBR_DOUBLE:
    		default:
				
        	valSetUpper[i]  = (CTYPE) (valSet[i] + fabs((double)tolerance));
        	valSetLower[i]  = (CTYPE) (valSet[i] - fabs((double)tolerance));
					
        break;
    	}

	
	  	
		vector<MonitorPolicy> mpVMatch;
		mpVMatch.clear();
					
    	helper.getMonitorPolicyVector(handleMatch[i], mpVMatch);

     
    	nMonitors[i]=mpVMatch.size();
						
    	monitorID[i]=0;
			
    	////valGetA[0][i]=0; 
			valGet[i]=0;

    	statMonitor[i]=ICAFE_NORMAL;
				
		}
		
		
			
		for (size_t i=0; i< handleMatch.size(); ++i) {
			
    	//what is monitorpolicy??
   	  if (nMonitors[i]==0) {

			unsigned int  _nelemPrevious, _nelemRequestedCheck=0;
			unsigned int  _nelemRequested=1;
			_nelemPrevious=helper.getNelemClient(handleMatch[i]);
			//Check the number of elements requested?
			  if (_nelemPrevious>1) {
				  _nelemRequestedCheck = helper.setNelem(handleMatch[i],_nelemRequested); 
				  if (_nelemRequestedCheck != _nelemRequested) {
					  cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
					  cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
						<< _nelemPrevious << endl;
					  cout << "to: " << _nelemRequested << " but got instead: "
						<< _nelemRequestedCheck  << endl;
				  }
			  }

        //first do get to update cache before monitor starts
			  status=Instant::get(handleMatch[i], dbrType, valGetA);
					
			  valGet[i]=valGetA[0];

			  if (_nelemPrevious>1) {
				  helper.setNelem(handleMatch[i],_nelemPrevious);  
			  }


        if (status!=ICAFE_NORMAL) {
           cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
           cstat.report(status);
        }

        ChannelWhenToFlushSendBufferPolicyKind whenKind=
                connect.channelMonitorPolicy.getWhenToFlushSendBuffer();
        connect.channelMonitorPolicy.setWhenToFlushSendBuffer(FLUSH_AUTOMATIC);

        statMonitor[i]=connect.monitorStart(handleMatch[i], monitorID[i]);

        if (statMonitor[i]!=ICAFE_NORMAL) {
          cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
          cstat.report(statMonitor[i]);
        }
			  else {
					if(printFlag) {cout << "MONITOR NOW IN PLACE FOR READBACK CHANNEL " << helper.getPVFromHandle(handleMatch[i]) << " WITH ID=" << monitorID[i] << endl;}
			  }
        //revert to previous
        if (whenKind != FLUSH_AUTOMATIC) {
          connect.channelMonitorPolicy.setWhenToFlushSendBuffer(whenKind);
        }
   	 
		  } //if nMonitors
		 	
    	



			//No of elements to get from Cache

			//unsigned int  nelemPreviousCheck=0;
			unsigned int  nelemRequested=1; unsigned int  nelemRequestedCheck=0;
			nelemPreviousCache[i]=helper.getNelemRequest(handleMatch[i]);

			//Check the number of elements requested
			//See set and Match; this needs to be checked
			//Avoid problem when readback channel is the very same as the set(!)
			if (nelemPreviousCache[i]>0  && helper.getNelemNative(handleMatch[i])>1) {
				nelemRequestedCheck = helper.setNelemToRetrieveFromCache(handleMatch[i],nelemRequested);
				if (nelemRequestedCheck != nelemRequested) {
					cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
					cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
					<< nelemPreviousCache[i] << endl;
					cout << "to: " << nelemRequested << " but got instead: "
					<< nelemRequestedCheck  << endl;
				}
			}


	} //for size_t
	
	


	//start time

 


	double timeElapsed=0; double timeElapsed2=0;
	double timeElapsedBase=0;
	using namespace boost::posix_time;

	ptime timeStart(microsec_clock::local_time());


	for (size_t i=0; i< handleMatch.size(); ++i) {

  	valGetA[0]=0;

		status=Instant::getCache(handleMatch[i], dbrType, valGetA);

		valGet[i]=valGetA[0];


		if (status !=ICAFE_NORMAL) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cstat.report(status);
		}

		///valGet[i]=valGetA[0][i];
		if (dbrType==DBR_CHAR) {valGet[i] = (unsigned short) valGetA[0];}

		if(printFlag) {
			//cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			if (dbrType==DBR_CHAR) {
			cout << "Current Cached Value = " << (unsigned short) valGet[i] << endl;
			cout << "Lower/Upper Target Values = " << (unsigned short)  valSetLower[i] << " and " << (unsigned short)  valSetUpper[i] << endl;
			}
			else {
			cout << "Current Cached Value = " << valGet[i] << endl;
			cout << "Lower/Upper Target Values = " << valSetLower[i] << " and " << valSetUpper[i] << endl;
			}
		}

	} //for size_t
	
	
	
	ptime timeEnd(microsec_clock::local_time());
	time_duration duration(timeEnd-timeStart);
	timeElapsed= (double) duration.total_microseconds()/1000000.0;
	
	
	for (size_t i=0; i< valSet.size(); ++i) {
	

		while ( (valGet[i]>valSetUpper[i] || valGet[i]<valSetLower[i] )   
            && timeElapsed < timeout ) {

      valGetA[0]=0; 

			status=Instant::getCache(handleMatch[i], dbrType, valGetA);

      valGet[i]=valGetA[0];
			
			
			//cout <<"Cached value " << valGet[i] << endl;
			
						
			if (dbrType==DBR_CHAR) {valGet[i] = (unsigned short) valGetA[0];}
            
            //cout << valSetUpper[i] << " U/L " << valSetLower[i] << endl;

            ptime timeEnd(microsec_clock::local_time());
            time_duration duration(timeEnd-timeStart);
            timeElapsed= (double) duration.total_microseconds()/1000000.0;

            timeElapsed2=timeElapsed-timeElapsedBase;

            if (printFlag && timeElapsed2>1 && status==ICAFE_NORMAL) {
												
						
							for (size_t ij=0; ij< handleMatch.size(); ++ij) {							
								 status=Instant::getCache(handleMatch[ij], dbrType, valGetA);
      					 valGet[ij]=valGetA[0];
								 if (valGet[ij]>valSetUpper[ij] || valGet[ij]<valSetLower[ij]) { 
								 
								 					if (dbrType==DBR_CHAR) {
								 
													cout << "REPORTING " << helper.getPVFromHandle(handleSet[ij]) 
													<< " SET VALUE=" << (unsigned short) valSet[ij] 
													<< "; READBACK " << helper.getPVFromHandle(handleMatch[ij]) 
													<< " HAS CURRENT VALUE=" << (unsigned short) valGet[ij] 
               		  	    << " TimeElapsed " << timeElapsed << " (sec) " << endl;    
													}
													else {
													cout << "REPORTING "  << helper.getPVFromHandle(handleSet[ij]) 
													<< " SET VALUE=" << valSet[ij] 
													<< "; READBACK " << helper.getPVFromHandle(handleMatch[ij]) 
													<< " HAS CURRENT VALUE=" <<  valGet[ij] 
               		  	    << " TimeElapsed " << timeElapsed << " (sec) " << endl;   
													
													}
													      				
								 }
							}
							timeElapsedBase=timeElapsed;
						
            //cout << "REPORTING ON CHANNEL " << helper.getPVFromHandle(handleSet[i]) << " SET VALUE=" << valSet[i] << " // CURRENT READBACK VALUE=" << valGet[i] 
            //        << " TimeElapsed " << timeElapsed << " (sec) " << endl;
            //timeElapsedBase=timeElapsed;

            }
   
				 
			   #if HAVE_BOOST_THREAD
			   boost::this_thread::sleep_for(boost::chrono::microseconds(20));
				 #else
				  #if HAVE_LINUX
	        usleep(20); 	
			    #endif
			   #endif
		}
	

	
	} //for size_t
		
	

		if (timeout <=timeElapsed ) {
			cout << "*****TIMEOUT REACHED****** AFTER " << timeout << " (sec) " << endl;
			cout << "*****NOT ALL READBACK CHANNELS REACHED THEIR DESIRED VALUES WITHIN THE GIVEN TOLERANCE AND TIME PERIOD*****" << endl;
 			status=ECAFE_TIMEOUT_SET_AND_MATCH;					
		}
		else {
      			
					if (printFlag) {	
						
						for (size_t i=0; i< handleMatch.size(); ++i) {
				  	    		
					      valGetA[0]=0; 
								status=Instant::getCache(handleMatch[i], dbrType, valGetA);
								valGet[i]=valGetA[0];
								if (dbrType==DBR_CHAR) {
                cout << "GAME SET AND MATCH: " << helper.getPVFromHandle(handleSet[i]) << " SET VALUE= " << (unsigned short) valSet[i] 
								<< " // " << helper.getPVFromHandle(handleMatch[i])  << " READBACK VALUE= " << (unsigned short) valGet[i] //pvdMatch.getAsDouble()
                     << " TimeElapsed " << timeElapsed << " (sec) " << endl;
								}
								else {
								cout << "GAME SET AND MATCH: " << helper.getPVFromHandle(handleSet[i]) << " SET VALUE= " << valSet[i] 
								     << " // " << helper.getPVFromHandle(handleMatch[i])  << " READBACK VALUE= " << valGet[i] //pvdMatch.getAsDouble()
                     << " TimeElapsed " << timeElapsed << " (sec) " << endl;
								
								}
    		  	} //for
					
					} //printFlag
		}
//

		for (size_t i=0; i< handleMatch.size(); ++i) {
			unsigned int  nelemPreviousCheck; 
			unsigned int  nelemRequested=1;
			
		
	  	if (nelemPreviousCache[i]>1) {
            nelemPreviousCheck= helper.setNelemToRetrieveFromCache(handleMatch[i],nelemPreviousCache[i]);
            if (nelemPreviousCheck != nelemPreviousCache[i]) {
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "Internal CAFE FUNNY: Wanted to re-set the no. elements from: "
                        << nelemRequested << endl;
                cout << "to the previous: " << nelemPreviousCache[i] << " but got instead: "
                        << nelemPreviousCheck  << endl;
            }
	  	}



    	if (nMonitors[i]==0 && statMonitor[i]==ICAFE_NORMAL && printFlag) {
        cout << "MONITOR NOW STOPPED FOR READBACK CHANNEL " << helper.getPVFromHandle(handleMatch[i]) << " WITH ID =" << monitorID[i] << endl;
								int statm;
                statm=connect.monitorStop(handleMatch[i], monitorID[i]);
			          if (status==ICAFE_NORMAL) {status=statm;}
    	}

		} //for
		
		
return status;
#undef __METHOD__

}





/**
 *  \brief Set Channels followed by a corresponding readback of Channels \n
 *  Method verifies whether or not the set/readback values agree within the given tolerance and timeout \n
 *  Method returns with ECA_NORMAL as soon as a match is reached
 *  \param handleSet input: vector of handleSet
 *  \param dbrType input: chtype of set value
 *  \param valSet input:  vector of CTYPE values to set
 *  \param handleMatch input: vector of handles of readback channel
 *  \param tolerance input: CTYPE of tolerance  margin
 *  \param timeout input:  double value; maximum time allowed for agreement to occur
 *  \param printFlag input: bool, set to true for diagnostics
 *  \return ECA_NORMAL if all OK, else first ECAFE error encountered, else ECAFE_TIMEOUT_SET_AND_MATCH
 */
template <class CTYPE> int  Instant<CTYPE>::setAndMatchMany(vector<unsigned int> handleSet, chtype dbrType, vector<CTYPE> valSet, vector<unsigned int> handleMatch,
                     CTYPE tolerance, double timeout, bool printFlag){
#define __METHOD__ "Instant<CTYPE>::setAndMatchMany(vector<unsigned int>handleSet, chtype, vector<CTYPE>valSet, vector<unsigned int>handleMatch, tolerance, timeout, printFlag)"

    Connect connect;
    CAFEStatus cstat;

    CTYPE valGetA[1];  CTYPE valSetA[1];

		status=ICAFE_NORMAL;

    if (handleSet.size() != handleMatch.size() ) {
		   return ECAFE_HANDLE_MISMATCH_SET_AND_MATCH;
		}
    
    if (handleSet.size() != valSet.size() ) {
		   return ECAFE_HANDLE_MISMATCH_SET_AND_MATCH;
		}
		
		for (size_t i=0; i< handleSet.size(); ++i) {

    	if (!helper.isChannelConnected(handleSet[i])
        || !helper.isChannelConnected(handleMatch[i])) {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "NOT ALL CHANNELS CONNECTED: " << endl;
                if (!helper.isChannelConnected(handleSet[i])) {helper.printHandle(handleSet[i]); status=helper.getStatus(handleSet[i]);}
                if (!helper.isChannelConnected(handleMatch[i])) {helper.printHandle(handleMatch[i]); status=helper.getStatus(handleMatch[i]);}
    	}		
	  
		}
	
		
		if (status!=ICAFE_NORMAL) {return status;}
	
	  if(printFlag) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
		}
	
		
		for (size_t i=0; i< handleSet.size(); ++i) {	
		
					
  	  if (printFlag) {
					
				if (dbrType==DBR_CHAR) {				
       		 cout << "SETTING  PV=" << helper.getPVFromHandle(handleSet[i]) << " to " << (unsigned short) valSet[i] << endl; 
				}       
				else {				
       	   cout << "SETTING  PV=" << helper.getPVFromHandle(handleSet[i]) << " to " << valSet[i] << endl;
				}
				
        cout << "READBACK PV=" << helper.getPVFromHandle(handleMatch[i])
             << " tolerance= " << fabs( (double) tolerance) << endl;
        cout << "TIME ALLOWED FOR MATCH IS " << timeout << " (sec) " << endl;
							
  	  }
   
		}

		
		vector<CTYPE> valSetUpper; vector<CTYPE> valSetLower;
	
		vector<unsigned int> monitorID; 
		vector<int> statMonitor;
		vector<unsigned short> nMonitors;
		vector<CTYPE> valGet;
	
			
		vector<unsigned int> nelemPreviousCache;	
							
		valSetUpper.reserve( handleSet.size());
		
		
		valSetLower.reserve( handleSet.size());
		monitorID.reserve( handleSet.size());
		statMonitor.reserve( handleSet.size());
		valGet.reserve( handleSet.size());
		nMonitors.reserve( handleSet.size() );
		
		
		nelemPreviousCache.reserve(handleSet.size());
		
	
		for (size_t i=0; i< handleSet.size(); ++i) {
		
					
    	switch (dbrType) {
    		case DBR_LONG :
    		case DBR_SHORT :
    		case DBR_ENUM:
        	valSetUpper[i]  = valSet[i] + abs((int)tolerance);
        	valSetLower[i]  = valSet[i] - abs((int)tolerance);
        break;
    		case DBR_CHAR :
        	valSetUpper[i]  = (unsigned short) valSet[i] + abs((unsigned short) tolerance);
        	valSetLower[i]  = (unsigned short) valSet[i] - abs((unsigned short) tolerance);
        break;
    		case DBR_FLOAT:
    		case DBR_DOUBLE:
    		default:
				
        	valSetUpper[i]  = (CTYPE) (valSet[i] + fabs((double)tolerance));
        	valSetLower[i]  = (CTYPE) (valSet[i] - fabs((double)tolerance));
					
        break;
    	}

	
	  	
			vector<MonitorPolicy> mpVMatch;
			mpVMatch.clear();
					
    	helper.getMonitorPolicyVector(handleMatch[i], mpVMatch);

     
    	nMonitors[i]=mpVMatch.size();
						
    	monitorID[i]=0;
			
    	////valGetA[0][i]=0; 
			valGet[i]=0;

    	statMonitor[i]=ICAFE_NORMAL;
				
		}
		
		
			
		for (size_t i=0; i< handleSet.size(); ++i) {
			
    	//what is monitorpolicy??
   	  if (nMonitors[i]==0) {

			unsigned int  _nelemPrevious, _nelemRequestedCheck=0;
			unsigned int  _nelemRequested=1;
			_nelemPrevious=helper.getNelemClient(handleMatch[i]);
			//Check the number of elements requested?
			  if (_nelemPrevious>1) {
				  _nelemRequestedCheck = helper.setNelem(handleMatch[i],_nelemRequested); //change handleSet to handleMatch 23/3/17
				  if (_nelemRequestedCheck != _nelemRequested) {
					  cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
					  cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
						<< _nelemPrevious << endl;
					  cout << "to: " << _nelemRequested << " but got instead: "
						<< _nelemRequestedCheck  << endl;
				  }
			  }

        //first do get to update cache before monitor starts
			  status=Instant::get(handleMatch[i], dbrType, valGetA);
					
			  valGet[i]=valGetA[0];

			  if (_nelemPrevious>1) {
				  helper.setNelem(handleMatch[i],_nelemPrevious);  //change handleSet to handleMatch 23/3/17
			  }


        if (status!=ICAFE_NORMAL) {
           cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
           cstat.report(status);
        }

        ChannelWhenToFlushSendBufferPolicyKind whenKind=
                connect.channelMonitorPolicy.getWhenToFlushSendBuffer();
        connect.channelMonitorPolicy.setWhenToFlushSendBuffer(FLUSH_AUTOMATIC);

        statMonitor[i]=connect.monitorStart(handleMatch[i], monitorID[i]);

        if (statMonitor[i]!=ICAFE_NORMAL) {
          cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
          cstat.report(statMonitor[i]);
        }
			  else {
					if(printFlag) {cout << "MONITOR NOW IN PLACE FOR READBACK CHANNEL " << helper.getPVFromHandle(handleMatch[i]) << " WITH ID=" << monitorID[i] << endl;}
			  }
        //revert to previous
        if (whenKind != FLUSH_AUTOMATIC) {
          connect.channelMonitorPolicy.setWhenToFlushSendBuffer(whenKind);
        }
   	 
		  } //if nMonitors
		 	
    	////valSetA[0][i]=valSet[i];

    	//set No of Elements to 1

    	unsigned int  nelemPrevious, nelemRequestedCheck=0;
    	unsigned int  nelemRequested=1;
					
    	nelemPrevious=helper.getNelemClient(handleSet[i]);
    	//Check the number of elements requested?
    	if (nelemPrevious>1) {
        nelemRequestedCheck = helper.setNelem(handleSet[i],nelemRequested);
        if (nelemRequestedCheck != nelemRequested) {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
                    << nelemPrevious << endl;
            cout << "to: " << nelemRequested << " but got instead: "
                    << nelemRequestedCheck  << endl;
        }
    	}
	

    	//policy set synchronous
    	ChannelRequestPolicy polPrevious, polNow;

    	policyHelper.getChannelRequestPolicyPut(handleSet[i], polPrevious);

   		polNow.setMethodKind(WITHOUT_CALLBACK);
    	polNow.setWaitKind(WAIT);
    	polNow.setWhenToFlushSendBuffer(FLUSH_AUTOMATIC);

    	policyHelper.setChannelRequestPolicyPut(handleSet[i], polNow);

     
      valSetA[0] = (CTYPE) valSet[i];

    	status=set(handleSet[i], dbrType, valSetA);

    	if (status!=ICAFE_NORMAL) {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cstat.report(status);
    	}

    	policyHelper.setChannelRequestPolicyPut(handleSet[i], polPrevious);

    	unsigned int  nelemPreviousCheck=nelemPrevious;
    	nelemRequested=1;
    	//Switch back to previous value
    	//if (nelemPrevious>1) {
    	if(helper.getNelemRequest(handleSet[i])!= nelemPrevious) {
        nelemPreviousCheck= helper.setNelem(handleSet[i],nelemPrevious);
        if (nelemPreviousCheck != nelemPrevious) {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Internal CAFE FUNNY: Wanted to re-set the no. elements from: "
                    << nelemRequested << endl;
            cout << "to the previous: " << nelemPrevious << " but got instead: "
                    << nelemPreviousCheck  << endl;
        }
    	}


			//No of elements to get from Cache

			nelemPreviousCheck=0;
			nelemRequested=1; nelemRequestedCheck=0;
			nelemPreviousCache[i]=helper.getNelemRequest(handleMatch[i]);

			//Check the number of elements requested
			//See set and Match; this needs to be checked
			//Avoid problem when readback channel is the very same as the set(!)
			if (nelemPreviousCache[i]>0  && helper.getNelemNative(handleMatch[i])>1) {
				nelemRequestedCheck = helper.setNelemToRetrieveFromCache(handleMatch[i],nelemRequested);
				if (nelemRequestedCheck != nelemRequested) {
					cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
					cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
					<< nelemPreviousCache[i] << endl;
					cout << "to: " << nelemRequested << " but got instead: "
					<< nelemRequestedCheck  << endl;
				}
			}


	} //for size_t

	//start time

 


	double timeElapsed=0; double timeElapsed2=0;
	double timeElapsedBase=0;
	using namespace boost::posix_time;

	ptime timeStart(microsec_clock::local_time());


	for (size_t i=0; i< handleSet.size(); ++i) {

 

  valGetA[0]=0;

	status=Instant::getCache(handleMatch[i], dbrType, valGetA);

	valGet[i]=valGetA[0];



		if (status !=ICAFE_NORMAL) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cstat.report(status);
		}

		///valGet[i]=valGetA[0][i];
		if (dbrType==DBR_CHAR) {valGet[i] = (unsigned short) valGetA[0];}

		if(printFlag) {
			//cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			if (dbrType==DBR_CHAR) {
			cout << "Current Cached Value = " << (unsigned short) valGet[i] << endl;
			cout << "Lower/Upper Target Values = " << (unsigned short)  valSetLower[i] << " and " << (unsigned short)  valSetUpper[i] << endl;
			}
			else {
			cout << "Current Cached Value = " << valGet[i] << endl;
			cout << "Lower/Upper Target Values = " << valSetLower[i] << " and " << valSetUpper[i] << endl;
			}
		}

	} //for size_t
	
	
	
	ptime timeEnd(microsec_clock::local_time());
	time_duration duration(timeEnd-timeStart);
	timeElapsed= (double) duration.total_microseconds()/1000000.0;
	
	
	for (size_t i=0; i< handleSet.size(); ++i) {
	

		while ( (valGet[i]>valSetUpper[i] || valGet[i]<valSetLower[i] )   
            && timeElapsed < timeout ) {

      valGetA[0]=0; 

			status=Instant::getCache(handleMatch[i], dbrType, valGetA);

      valGet[i]=valGetA[0];
			
			
			//cout <<"Cached value " << valGet[i] << endl;
			
						
			if (dbrType==DBR_CHAR) {valGet[i] = (unsigned short) valGetA[0];}
            
            //cout << valSetUpper[i] << " U/L " << valSetLower[i] << endl;

            ptime timeEnd(microsec_clock::local_time());
            time_duration duration(timeEnd-timeStart);
            timeElapsed= (double) duration.total_microseconds()/1000000.0;

            timeElapsed2=timeElapsed-timeElapsedBase;

            if (printFlag && timeElapsed2>1 && status==ICAFE_NORMAL) {
						
						
						
							for (size_t ij=0; ij< handleMatch.size(); ++ij) {							
								 status=Instant::getCache(handleMatch[ij], dbrType, valGetA);
      					 valGet[ij]=valGetA[0];
								 if (valGet[ij]>valSetUpper[ij] || valGet[ij]<valSetLower[ij]) { 
								 
								 					if (dbrType==DBR_CHAR) {
								 
													cout << "REPORTING " << helper.getPVFromHandle(handleSet[ij])  
													<< " SET VALUE=" << (unsigned short) valSet[ij] 
													<< "; READBACK " << helper.getPVFromHandle(handleMatch[ij]) 
													<< " HAS CURRENT VALUE=" << (unsigned short) valGet[ij] 
               		  	    << " TimeElapsed " << timeElapsed << " (sec) " << endl;    
													}
													else {
													cout << "REPORTING " << helper.getPVFromHandle(handleSet[ij])  
													<< " SET VALUE=" << valSet[ij] 
													<< "; READBACK " << helper.getPVFromHandle(handleMatch[ij]) 
													<< " HAS CURRENT VALUE=" <<  valGet[ij] 
               		  	    << " TimeElapsed " << timeElapsed << " (sec) " << endl;   
													
													}
													      				
								 }
							}
							timeElapsedBase=timeElapsed;
						
            //cout << "REPORTING ON CHANNEL " << helper.getPVFromHandle(handleSet[i]) << " SET VALUE=" << valSet[i] << " // CURRENT READBACK VALUE=" << valGet[i] 
            //        << " TimeElapsed " << timeElapsed << " (sec) " << endl;
            //timeElapsedBase=timeElapsed;

            }
   
				 
			   #if HAVE_BOOST_THREAD
			   boost::this_thread::sleep_for(boost::chrono::microseconds(20));
				 #else
				  #if HAVE_LINUX
	        usleep(20); 	
			    #endif
			   #endif
		}
	

	
	} //for size_t
		
	

		if (timeout <=timeElapsed ) {
			cout << "*****TIMEOUT REACHED****** AFTER " << timeout << " (sec) " << endl;
			cout << "*****NOT ALL READBACK CHANNELS REACHED THEIR DESIRED VALUES WITHIN THE GIVEN TOLERANCE AND TIME PERIOD*****" << endl;
 			status=ECAFE_TIMEOUT_SET_AND_MATCH;					
		}
		else {
      			
					if (printFlag) {	
						
						for (size_t i=0; i< handleSet.size(); ++i) {
				  	    		
					      valGetA[0]=0; 
								status=Instant::getCache(handleMatch[i], dbrType, valGetA);
								valGet[i]=valGetA[0];
								if (dbrType==DBR_CHAR) {
                cout << "GAME SET AND MATCH: " << helper.getPVFromHandle(handleSet[i]) << " SET VALUE= " << (unsigned short) valSet[i] 
								<< " // " << helper.getPVFromHandle(handleMatch[i])  << " READBACK VALUE= " << (unsigned short) valGet[i] //pvdMatch.getAsDouble()
                     << " TimeElapsed " << timeElapsed << " (sec) " << endl;
								}
								else {
								cout << "GAME SET AND MATCH: " << helper.getPVFromHandle(handleSet[i]) << " SET VALUE= " << valSet[i] 
								     << " // " << helper.getPVFromHandle(handleMatch[i])  << " READBACK VALUE= " << valGet[i] //pvdMatch.getAsDouble()
                     << " TimeElapsed " << timeElapsed << " (sec) " << endl;
								
								}
    		  	} //for
					
					} //printFlag
		}
//

		for (size_t i=0; i< handleSet.size(); ++i) {
			unsigned int  nelemPreviousCheck; 
			unsigned int  nelemRequested=1;
			
		
	  	if (nelemPreviousCache[i]>1) {
            nelemPreviousCheck= helper.setNelemToRetrieveFromCache(handleMatch[i],nelemPreviousCache[i]);
            if (nelemPreviousCheck != nelemPreviousCache[i]) {
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "Internal CAFE FUNNY: Wanted to re-set the no. elements from: "
                        << nelemRequested << endl;
                cout << "to the previous: " << nelemPreviousCache[i] << " but got instead: "
                        << nelemPreviousCheck  << endl;
            }
	  	}



    	if (nMonitors[i]==0 && statMonitor[i]==ICAFE_NORMAL && printFlag) {
        cout << "MONITOR NOW STOPPED FOR READBACK CHANNEL " << helper.getPVFromHandle(handleMatch[i]) << " WITH ID =" << monitorID[i] << endl;
								int statm;
                statm=connect.monitorStop(handleMatch[i], monitorID[i]);
			          if (status==ICAFE_NORMAL) {status=statm;}
    	}

		} //for
		
		
return status;
#undef __METHOD__
}



/**
 *  \brief Set Channel 1 followed by a readback of Channel 2 \n
 *  Method verifies whether or not the 2 values agree within the given tolerance and timeout \n
 *  Method returns with ECA_NORMAL as soon as a match is reached
 *  \param handleSet input: handleSet
 *  \param dbrType input: chtype of set value
 *  \param valSet input:  CTYPE value to set
 *  \param handleMatch input: handle of readback channel
 *  \param tolerance input: CTYPE of tolerance  margin
 *  \param timeout input:  double value; maximum time allowed for agreement to occur
 *  \param printFlag input: bool, set to true for diagnostics
 *  \return ECA_NORMAL if all OK else first ECAFE error encountered
 */
template <class CTYPE> int  Instant<CTYPE>::setAndMatch(const unsigned int handleSet, const chtype dbrType, CTYPE valSet, const unsigned int handleMatch,
                      CTYPE tolerance, double timeout, bool printFlag){
#define __METHOD__ "Instant<CTYPE>::setAndMatch(handleSet, dbrType, valSet, handleMatch, tolerance, timeout, printFlag)"
    //check both handles exists before proceeding

    Connect connect;
    CAFEStatus cstat;

    if (!helper.isChannelConnected(handleSet)
        || !helper.isChannelConnected(handleMatch)) {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "NOT ALL CHANNELS CONNECTED: " << endl;
                if (!helper.isChannelConnected(handleSet)) {helper.printHandle(handleSet); return helper.getStatus(handleSet);}
                if (!helper.isChannelConnected(handleMatch)) {helper.printHandle(handleMatch); return helper.getStatus(handleMatch);}
    }

    if (printFlag) {
		
		    if (dbrType==DBR_CHAR) {
          cout << "SETTING  PV=" << helper.getPVFromHandle(handleSet) << " to " << (unsigned short) valSet << endl;
				}
				else {
				  cout << "SETTING  PV=" << helper.getPVFromHandle(handleSet) << " to " << valSet << endl;
				}
        cout << "READBACK PV=" << helper.getPVFromHandle(handleMatch)
             << " tolerance= " << fabs((double)tolerance) << endl;
        cout << "TIME ALLOWED FOR MATCH " << timeout << " (sec) " << endl;
    }

    vector<MonitorPolicy> mpVMatch;
    helper.getMonitorPolicyVector(handleMatch, mpVMatch);

    unsigned short nMonitors=mpVMatch.size();
   

    CTYPE valSetUpper; CTYPE valSetLower;
		
		
    switch (dbrType) {
    case DBR_LONG :
    case DBR_SHORT :
    case DBR_ENUM:
        valSetUpper  = valSet + abs((int)tolerance);
        valSetLower  = valSet - abs((int)tolerance);
        break;
    case DBR_CHAR :
        valSetUpper  = (unsigned short) valSet + abs((unsigned short) tolerance);
        valSetLower  = (unsigned short) valSet - abs((unsigned short) tolerance);
        break;
    case DBR_FLOAT:
    case DBR_DOUBLE:
    default:
        valSetUpper  = (CTYPE) (valSet + fabs((double)tolerance));
        valSetLower  = (CTYPE) (valSet - fabs((double)tolerance));
        break;
    }


    CTYPE valGetA[1]={0}; 
		CTYPE valGet=valGetA[0];

    int statMonitor=ICAFE_NORMAL;

    unsigned int monitorIDis=0;
		
    //what is monitorpolicy??
    if (nMonitors==0) {

			unsigned int  _nelemPrevious, _nelemRequestedCheck=0;
			unsigned int  _nelemRequested=1;
			_nelemPrevious=helper.getNelemClient(handleMatch);
			//Check the number of elements requested?
			if (_nelemPrevious>1) {
				_nelemRequestedCheck = helper.setNelem(handleMatch,_nelemRequested); ////change handleSet to handleMatch 23/3/17
				if (_nelemRequestedCheck != _nelemRequested) {
					cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
					cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
						<< _nelemPrevious << endl;
					cout << "to: " << _nelemRequested << " but got instead: "
						<< _nelemRequestedCheck  << endl;
				}
			}

        //first do get to update cache before monitor starts
			status=Instant::get(handleMatch, dbrType, valGetA);
			//valGet=valGetA[0];


			if (_nelemPrevious>1) {
				helper.setNelem(handleMatch,_nelemPrevious);  ////change handleSet to handleMatch 23/3/17
			}

      if (status!=ICAFE_NORMAL) {
          cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
          cstat.report(status);
      }

      ChannelWhenToFlushSendBufferPolicyKind whenKind=
                connect.channelMonitorPolicy.getWhenToFlushSendBuffer();
      connect.channelMonitorPolicy.setWhenToFlushSendBuffer(FLUSH_AUTOMATIC);
				
      statMonitor=connect.monitorStart(handleMatch, monitorIDis);

      if (statMonitor!=ICAFE_NORMAL) {
          cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
          cstat.report(statMonitor);
      }
		  else {
				  if(printFlag) {cout << "MONITOR STARTED WITH ID=" << monitorIDis<< endl;}
			}
      //revert to previous
      if (whenKind != FLUSH_AUTOMATIC) {
          connect.channelMonitorPolicy.setWhenToFlushSendBuffer(whenKind);
      }
    }
		
    CTYPE valSetA[1]; valSetA[0]=valSet;

    //set No of Elements to 1

    unsigned int  nelemPrevious, nelemRequestedCheck=0;
    unsigned int  nelemRequested=1;
    nelemPrevious=helper.getNelemClient(handleSet);
    //Check the number of elements requested?
    if (nelemPrevious>1) {
        nelemRequestedCheck = helper.setNelem(handleSet,nelemRequested);
        if (nelemRequestedCheck != nelemRequested) {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
                    << nelemPrevious << endl;
            cout << "to: " << nelemRequested << " but got instead: "
                    << nelemRequestedCheck  << endl;
        }
    }

    //policy set synchronous
    ChannelRequestPolicy polPrevious, polNow;

    policyHelper.getChannelRequestPolicyPut(handleSet, polPrevious);

    polNow.setMethodKind(WITHOUT_CALLBACK);
    polNow.setWaitKind(WAIT);
    polNow.setWhenToFlushSendBuffer(FLUSH_AUTOMATIC);

    policyHelper.setChannelRequestPolicyPut(handleSet, polNow);

    status=set(handleSet, dbrType, valSetA);

    if (status!=ICAFE_NORMAL) {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cstat.report(status);
    }

    policyHelper.setChannelRequestPolicyPut(handleSet, polPrevious);

    unsigned int  nelemPreviousCheck=nelemPrevious;
    nelemRequested=1;
    //Switch back to previous value
    //if (nelemPrevious>1) {
    if(helper.getNelemRequest(handleSet)!= nelemPrevious) {
        nelemPreviousCheck= helper.setNelem(handleSet,nelemPrevious);
        if (nelemPreviousCheck != nelemPrevious) {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Internal CAFE FUNNY: Wanted to re-set the no. elements from: "
                    << nelemRequested << endl;
            cout << "to the previous: " << nelemPrevious << " but got instead: "
                    << nelemPreviousCheck  << endl;
        }
    }


	//start time

	double timeElapsed=0; double timeElapsed2=0;
	double timeElapsedBase=0;
	using namespace boost::posix_time;

	ptime timeStart(microsec_clock::local_time());

	// PVDataHolder pvdMatch;
	// status=getCache(handleMatch, dbrType,  pvdMatch);


	//No of elements to get from Cache

	nelemPreviousCheck=0;
	nelemRequested=1; nelemRequestedCheck=0;
	nelemPrevious=helper.getNelemRequest(handleMatch);
	


	//Check the number of elements requested
	if (nelemPrevious>0) {
		nelemRequestedCheck = helper.setNelemToRetrieveFromCache(handleMatch,nelemRequested);
		if (nelemRequestedCheck != nelemRequested) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
					<< nelemPrevious << endl;
			cout << "to: " << nelemRequested << " but got instead: "
					<< nelemRequestedCheck  << endl;
		}
	}

	

  

	status=Instant::getCache(handleMatch, dbrType, valGetA);
	
	

	
	///if (dbrType==DBR_CHAR) {valGet = (unsigned short) valGetA[0];}
	


	if (status !=ICAFE_NORMAL) {
		cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
		cstat.report(status);
	}

	
	

	if(printFlag) {
		//cout << __METHOD__ << endl;
		cout << "Current Cached Value = " ;
		if (dbrType==DBR_CHAR) {
		cout << (unsigned short) valGetA[0] << endl;
		cout << "Lower/Upper Target Values = " << (unsigned short) valSetLower << " and " << (unsigned short) valSetUpper << endl;
		}
		else {
		cout << valGetA[0] << endl;
		cout << "Lower/Upper Target Values = " << valSetLower << " and " << valSetUpper << endl;
		}
	}



	ptime timeEnd(microsec_clock::local_time());
	time_duration duration(timeEnd-timeStart);
	timeElapsed= (double) duration.total_microseconds()/1000000.0;


	
	if (dbrType==DBR_CHAR) {valGet= (unsigned short) valGetA[0];}
	
	else { valGet=valGetA[0];}
	
	

	while ( (valGet>valSetUpper || valGet<valSetLower)

        //while ( (pvdMatch.getAsDouble()>valSetUpper || pvdMatch.getAsDouble()<valSetLower)
            && timeElapsed < timeout ) {
            //status=getCache(handleMatch, dbrType, pvdMatch);

            /*
            //Check the number of elements requested - again; in case overwritten by Monitor
            if (nelemPrevious>1) {
                nelemRequestedCheck = helper.setNelemToRetrieveFromCache(handleMatch,nelemRequested);
                if (nelemRequestedCheck != nelemRequested) {
                    cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                    cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
                            << nelemPrevious << endl;
                    cout << "to: " << nelemRequested << " but got instead: "
                            << nelemRequestedCheck  << endl;
                }
            }
            */

			status=Instant::getCache(handleMatch, dbrType, valGetA);

      valGet=valGetA[0];
						
			if (dbrType==DBR_CHAR) {valGet = (unsigned short) valGetA[0];}
            //cout << "getCache(handleMatch, dbrType, valGetA) = " << valGet << endl;
            //cout << valSetUpper << " U/L " << valSetLower << endl;

            ptime timeEnd(microsec_clock::local_time());
            time_duration duration(timeEnd-timeStart);
            timeElapsed= (double) duration.total_microseconds()/1000000.0;

            timeElapsed2=timeElapsed-timeElapsedBase;

            if (printFlag && timeElapsed2>1 && status==ICAFE_NORMAL) {
						
						if (dbrType==DBR_CHAR) {					
            cout << "SET VALUE " << (unsigned short) valSet << " CURRENT VALUE " << (unsigned short) valGetA[0] 
                    << " TimeElapsed " << timeElapsed << " (sec) " << endl;
						} else {
						cout << "SET VALUE " << valSet << " CURRENT VALUE " << valGetA[0] //pvdMatch.getAsDouble()
                    << " TimeElapsed " << timeElapsed << " (sec) " << endl;
						}				
										
										
            timeElapsedBase=timeElapsed;

            }
              
													
							#if HAVE_BOOST_THREAD
							boost::this_thread::sleep_for(boost::chrono::microseconds(20));
							#else
							#if HAVE_LINUX
	            usleep(20);
							#endif
							#endif
	}

	
	if (timeout <=timeElapsed ) {
		cout << "*****TIMEOUT REACHED****** AFTER " << timeout << " (sec) " << endl;
		
                        status=ECAFE_TIMEOUT_SET_AND_MATCH;
	}
	else {          		
            if (printFlag) {
				        status=Instant::getCache(handleMatch, dbrType, valGetA);
								if (dbrType==DBR_CHAR) {
                  cout << "GAME SET AND MATCH: SET VALUE= " << (unsigned short) valSet 
									     << " LAST VALUE READ= " << (unsigned short) valGetA[0] //pvdMatch.getAsDouble()
                       << " TimeElapsed " << timeElapsed << " (sec) " << endl;
								}
								else {
								  cout << "GAME SET AND MATCH: SET VALUE= " << valSet << " LAST VALUE READ= " << valGetA[0] //pvdMatch.getAsDouble()
                       << " TimeElapsed " << timeElapsed << " (sec) " << endl;
								}
										 
            }
	}


	

	if (nelemPrevious>1) {
            nelemPreviousCheck= helper.setNelemToRetrieveFromCache(handleMatch,nelemPrevious);
            if (nelemPreviousCheck != nelemPrevious) {
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "Internal CAFE FUNNY: Wanted to re-set the no. elements from: "
                        << nelemRequested << endl;
                cout << "to the previous: " << nelemPrevious << " but got instead: "
                        << nelemPreviousCheck  << endl;
            }
	}



	
    if (nMonitors==0 && statMonitor==ICAFE_NORMAL) {
			if (monitorIDis !=0 ) {
       	cout << "STOPPED MONITOR WITH ID =" << monitorIDis << endl;
			 	int statm;
        statm=connect.monitorStop(handleMatch, monitorIDis);
				
				if (status==ICAFE_NORMAL) {status=statm;}
			}
    }

	return status;
#undef __METHOD__
}


/**
 *  \brief Set Channel followed by a corresponding readback of Channel \n
 *  Method verifies whether or not the set/readback values agree within the given tolerance and timeout \n
 *  Method returns with ECA_NORMAL as soon as a match is reached
 *  \param dbrType input: chtype of set value
 *  \param valSet input: CTYPE value to set
 *  \param handleMatch input: handle of readback channel
 *  \param tolerance input: CTYPE of tolerance  margin
 *  \param timeout input:  double value; maximum time allowed for agreement to occur
 *  \param printFlag input: bool, set to true for diagnostics
 *  \return ECA_NORMAL if all OK else first ECAFE error encountered
 */
template <class CTYPE> int  Instant<CTYPE>::match(const chtype dbrType, CTYPE valSet, unsigned int handleMatch,
                     CTYPE tolerance, double timeout, bool printFlag){
#define __METHOD__ "Instant<CTYPE>::match(const chtype dbrType, CTYPE valSet, unsigned int handleMatch, \
       CTYPE tolerance,double  timeout, bool printFlag)"
			 
 	vector<CTYPE> valSetV;
  vector<unsigned int>handleMatchV;
  valSetV.reserve(1);
	handleMatchV.reserve(1);
	valSetV.push_back(valSet);
	handleMatchV.push_back(handleMatch);
	
	return matchMany(dbrType, valSetV, handleMatchV, tolerance, timeout, printFlag);
#undef __METHOD__
}


/**
 *  \brief Set Channels followed by a corresponding readback of Channels \n
 *  Method verifies whether or not the set/readback values agree within the given tolerance and timeout \n
 *  Method returns with ECA_NORMAL as soon as a match is reached
 *  \param dbrType input: chtype of set value
 *  \param valSet input: vector of CTYPE values to set
 *  \param handleMatch input: vector of handles of readback channel
 *  \param tolerance input: CTYPE of tolerance  margin
 *  \param timeout input:  double value; maximum time allowed for agreement to occur
 *  \param printFlag input: bool, set to true for diagnostics
 *  \return ECA_NORMAL if all OK else first ECAFE error encountered
 */
template <class CTYPE> int  Instant<CTYPE>::matchMany(const chtype dbrType, vector<CTYPE> valSet, vector<unsigned int> handleMatch,
                     CTYPE tolerance, double timeout, bool printFlag){
#define __METHOD__ "Instant<CTYPE>::matchMany(const chtype dbrType, vector<CTYPE>valSet, vector<unsigned int>handleMatch, \
			CTYPE tolerance, double timeout, bool printFlag)"

   
    Connect connect;
    CAFEStatus cstat;

    CTYPE valGetA[1];  
		////CTYPE valSetA[1];

		status=ICAFE_NORMAL;

    
    if (handleMatch.size() != valSet.size() ) {
		   return ECAFE_HANDLE_MISMATCH_SET_AND_MATCH;
		}
		
		for (size_t i=0; i< handleMatch.size(); ++i) {
    	if ( !helper.isChannelConnected(handleMatch[i])) {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "NOT ALL CHANNELS CONNECTED: " << endl;
                
        helper.printHandle(handleMatch[i]); 
				status=helper.getStatus(handleMatch[i]);
    	}			  
		}
	
		
		if (status!=ICAFE_NORMAL) {return status;}
	
		
		for (size_t i=0; i< handleMatch.size(); ++i) {		
  	  if (printFlag) {
        cout << "SET VALUE IS = " << valSet[i] << endl;
        cout << "READBACK PV=" << helper.getPVFromHandle(handleMatch[i])
             << "  tolerance= " << fabs((double)tolerance) << endl;
        cout << "TIME ALLOWED FOR MATCH IS " << timeout << " (sec) " << endl;		
  	  }  
		}

		
		vector<CTYPE> valSetUpper; vector<CTYPE> valSetLower;
	
		vector<unsigned int> monitorID; 
		vector<int> statMonitor;
		vector<unsigned short> nMonitors;
		vector<CTYPE> valGet;
			
		vector<unsigned int> nelemPreviousCache;	
							
		valSetUpper.reserve( handleMatch.size());			
		valSetLower.reserve( handleMatch.size());
		monitorID.reserve( handleMatch.size());
		statMonitor.reserve( handleMatch.size());
		valGet.reserve( handleMatch.size());
		nMonitors.reserve( handleMatch.size() );		
		
		nelemPreviousCache.reserve(handleMatch.size());
		
		for (size_t i=0; i< handleMatch.size(); ++i) {
							
    	switch (dbrType) {
    		case DBR_LONG :
    		case DBR_SHORT :
    		case DBR_ENUM:
        	valSetUpper[i]  = valSet[i] + abs((int)tolerance);
        	valSetLower[i]  = valSet[i] - abs((int)tolerance);
        break;
    		case DBR_CHAR :
        	valSetUpper[i]  = (unsigned short) valSet[i] + abs((unsigned short) tolerance);
        	valSetLower[i]  = (unsigned short) valSet[i] - abs((unsigned short) tolerance);
        break;
    		case DBR_FLOAT:
    		case DBR_DOUBLE:
    		default:
				
        	valSetUpper[i]  = (CTYPE) (valSet[i] + fabs((double)tolerance));
        	valSetLower[i]  = (CTYPE) (valSet[i] - fabs((double)tolerance));
					
        break;
    	}

		  	
			vector<MonitorPolicy> mpVMatch;
			mpVMatch.clear();
					
    	helper.getMonitorPolicyVector(handleMatch[i], mpVMatch);
    
    	nMonitors[i]=mpVMatch.size();
						
    	monitorID[i]=0;
			
    	////valGetA[0][i]=0; 
			valGet[i]=0;

    	statMonitor[i]=ICAFE_NORMAL;
				
		}
		
		
			
		for (size_t i=0; i< handleMatch.size(); ++i) {
			
    	//what is monitorpolicy??
   	 if (nMonitors[i]==0) {

			unsigned int  _nelemPrevious, _nelemRequestedCheck=0;
			unsigned int  _nelemRequested=1;
			_nelemPrevious=helper.getNelemClient(handleMatch[i]);
			//Check the number of elements requested?
			if (_nelemPrevious>1) {
				_nelemRequestedCheck = helper.setNelem(handleMatch[i],_nelemRequested);
				if (_nelemRequestedCheck != _nelemRequested) {
					cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
					cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
						<< _nelemPrevious << endl;
					cout << "to: " << _nelemRequested << " but got instead: "
						<< _nelemRequestedCheck  << endl;
				}
			}

      //first do get to update cache before monitor starts
			status=Instant::get(handleMatch[i], dbrType, valGetA);
					
			valGet[i]=valGetA[0];

			if (_nelemPrevious>1) {
				helper.setNelem(handleMatch[i],_nelemPrevious);
			}


      if (status!=ICAFE_NORMAL) {
         cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
         cstat.report(status);
      }

      ChannelWhenToFlushSendBufferPolicyKind whenKind=
                connect.channelMonitorPolicy.getWhenToFlushSendBuffer();
      connect.channelMonitorPolicy.setWhenToFlushSendBuffer(FLUSH_AUTOMATIC);

      statMonitor[i]=connect.monitorStart(handleMatch[i], monitorID[i]);

      if (statMonitor[i]!=ICAFE_NORMAL) {
          cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
          cstat.report(statMonitor[i]);
      }
			else {
					if(printFlag) {cout << "MONITOR STARTED WITH ID=" << monitorID[i] << endl;}
			}
      //revert to previous
      if (whenKind != FLUSH_AUTOMATIC) {
          connect.channelMonitorPolicy.setWhenToFlushSendBuffer(whenKind);
      }
   	 
		 } //if nMonitors
		 	
    	////valSetA[0][i]=valSet[i];

    	//set No of Elements to 1

    	unsigned int  nelemPrevious, nelemRequestedCheck=0;
    	unsigned int  nelemRequested=1;
			
			
    	nelemPrevious=helper.getNelemClient(handleMatch[i]);
    	//Check the number of elements requested?
    	if (nelemPrevious>1) {
        nelemRequestedCheck = helper.setNelem(handleMatch[i],nelemRequested);
        if (nelemRequestedCheck != nelemRequested) {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
                    << nelemPrevious << endl;
            cout << "to: " << nelemRequested << " but got instead: "
                    << nelemRequestedCheck  << endl;
        }
    	}
	
		//No of elements to get from Cache

		////unsigned int nelemPreviousCheck=0;
		nelemRequested=1; nelemRequestedCheck=0;
		nelemPreviousCache[i]=helper.getNelemRequest(handleMatch[i]);

		//Check the number of elements requested
		//See set and Match; this needs to be checked
		//Avoid problem when readback channel is the very same as the set(!)
		if (nelemPreviousCache[i]>0  && helper.getNelemNative(handleMatch[i])>1) {
			nelemRequestedCheck = helper.setNelemToRetrieveFromCache(handleMatch[i],nelemRequested);
			if (nelemRequestedCheck != nelemRequested) {
				cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
					<< nelemPreviousCache[i] << endl;
				cout << "to: " << nelemRequested << " but got instead: "
					<< nelemRequestedCheck  << endl;
			}
		}

	} //if size_t

	//start time

	double timeElapsed=0; double timeElapsed2=0;
	double timeElapsedBase=0;
	using namespace boost::posix_time;

	ptime timeStart(microsec_clock::local_time());


	for (size_t i=0; i< handleMatch.size(); ++i) {

  	valGetA[0]=0;

		status=Instant::getCache(handleMatch[i], dbrType, valGetA);

		valGet[i]=valGetA[0];

		if (status !=ICAFE_NORMAL) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cstat.report(status);
		}

		///valGet[i]=valGetA[0][i];
		if (dbrType==DBR_CHAR) {valGet[i] = (unsigned short) valGet[i];}

		if(printFlag) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cout << "Current Cached Value = " << valGet[i] << endl;
			cout << "Lower/Upper Target Values = " << valSetLower[i] << " and " << valSetUpper[i] << endl;
		}

	} //for size_t
	
	
	
	ptime timeEnd(microsec_clock::local_time());
	time_duration duration(timeEnd-timeStart);
	timeElapsed= (double) duration.total_microseconds()/1000000.0;
	
	
	for (size_t i=0; i< handleMatch.size(); ++i) {
	

		while ( (valGet[i]>valSetUpper[i] || valGet[i]<valSetLower[i] )   
            && timeElapsed < timeout ) {

      valGetA[0]=0; 

			status=Instant::getCache(handleMatch[i], dbrType, valGetA);
      valGet[i]=valGetA[0];
			
			
			//cout <<"Cached value " << valGet[i] << endl;
			
						
			if (dbrType==DBR_CHAR) {valGet[i] = (unsigned short) valGet[i];}
            
            //cout << valSetUpper[i] << " U/L " << valSetLower[i] << endl;

            ptime timeEnd(microsec_clock::local_time());
            time_duration duration(timeEnd-timeStart);
            timeElapsed= (double) duration.total_microseconds()/1000000.0;

            timeElapsed2=timeElapsed-timeElapsedBase;

            if (printFlag && timeElapsed2>1 && status==ICAFE_NORMAL) {
						
							for (size_t ij=0; ij< handleMatch.size(); ++ij) {							
								 status=Instant::getCache(handleMatch[ij], dbrType, valGetA);
      					 valGet[ij]=valGetA[0];
								 if (valGet[ij]>valSetUpper[ij] || valGet[ij]<valSetLower[ij]) { 
													cout << "REPORTING ON: " << helper.getPVFromHandle(handleMatch[ij]) << " SET VALUE= " << valSet[ij] \
													     <<" WHILE CURRENT READBACK VALUE=" << valGet[ij] 
							<< " TimeElapsed " << timeElapsed << " (sec) " << endl;

								 }
							}
							timeElapsedBase=timeElapsed;

            }
       
			 						
			 #if HAVE_BOOST_THREAD
			   boost::this_thread::sleep_for(boost::chrono::microseconds(20));
			 #else	 
				#if HAVE_LINUX
	       usleep(20); 	 
			  #endif
			 #endif
		}
	

	
	} //for size_t
		
	

		if (timeout <=timeElapsed ) {
			cout << "*****TIMEOUT REACHED****** AFTER " << timeout << " (sec) " << endl;
      status=ECAFE_TIMEOUT_SET_AND_MATCH;
		}
		else {
      			
			for (size_t i=0; i< handleMatch.size(); ++i) {
			     
					//if (printFlag) {cout << "MONITOR STARTED WITH ID/2-/=" << monitorID[i] << endl;}
      		if (printFlag) {
					      valGetA[0]=0; 
								status=Instant::getCache(handleMatch[i], dbrType, valGetA);
								valGet[i]=valGetA[0];
                cout << "GAME SET AND MATCH: "   << " SET VALUE= " << valSet[i] << " // " << helper.getPVFromHandle(handleMatch[i])  << " READBACK VALUE= " << valGet[i] //pvdMatch.getAsDouble()
                     << " TimeElapsed " << timeElapsed << " (sec) " << endl;
    		  }
					
			} //for
		}


		for (size_t i=0; i< handleMatch.size(); ++i) {
			unsigned int  nelemPreviousCheck; 
			unsigned int  nelemRequested=1;
			
		
	  	if (nelemPreviousCache[i]>1) {
            nelemPreviousCheck= helper.setNelemToRetrieveFromCache(handleMatch[i],nelemPreviousCache[i]);
            if (nelemPreviousCheck != nelemPreviousCache[i]) {
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "Internal CAFE FUNNY: Wanted to re-set the no. elements from: "
                        << nelemRequested << endl;
                cout << "to the previous: " << nelemPreviousCache[i] << " but got instead: "
                        << nelemPreviousCheck  << endl;
            }
	  	}



    	if (nMonitors[i]==0 && statMonitor[i]==ICAFE_NORMAL) {
            cout << "STOPPING MONITOR WITH ID =" << monitorID[i] << endl;
						int statm;
            statm=connect.monitorStop(handleMatch[i], monitorID[i]);
						if(status==ICAFE_NORMAL){status=statm;}
    	}

		} //for
		
		
return status;
#undef __METHOD__
}





#endif // instant.cpp







