///
/// \file    granules.cpp
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0

#include <granules.h>
#include <methodCallbacks.h>
//include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

/**
 *  \brief Contains methods that are used by Instant template:
 *  channelVerifyPut: preVerification of client's requested data
 *  \param _handle input: handle to Conduit Object
 *  \param _dbrType input: chtype
 *  \return ICAFE_NORMAL or ICAFE_CS_NEVER_CONN or ICAFE_CA_OP_CONN_DOWN or ECAFE_NOWTACCESS
 */
int  Granules::channelVerifyPut(const unsigned int  _handle, chtype _dbrType)
{
#define	__METHOD__  "Granules::channelVerifyPut"
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if ( (*it_handle).getChannelRegalia().getCafeConnectionState()==ICAFE_CS_NEVER_CONN)
        {
            return ICAFE_CS_NEVER_CONN;
        }
        else if ( (*it_handle).getChannelRegalia().getCafeConnectionState()==ICAFE_CS_CLOSED)
        {
            return ICAFE_CS_CLOSED;
        }


        if ( CHECK_CONSISTENCY_CA_STATE && !(*it_handle).isConnected())
        {
            status=helper.checkConsistency(_handle);
            /*
            if (status!=ICAFE_NORMAL)
            {
                return status;
            }
            */
        }

        //3+2
        //isConnected
        if ( !(*it_handle).isConnected())
        {
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            channelRequestStatusPut      = (*it_handle).getChannelRequestStatusPut(); //
            //Check if it was ever connected??
            channelRequestStatusPut.setPreRequestStatus (ICAFE_CA_OP_CONN_DOWN);
            handle_index.modify(it_handle, change_channelRequestStatusPut(channelRequestStatusPut));
            handle_index.modify(it_handle, change_status(ICAFE_CA_OP_CONN_DOWN)); //for return
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
            return ICAFE_CA_OP_CONN_DOWN;
        }

        //hasWriteAccess
        if ((*it_handle).getAccessWrite() == 0)
        {
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            channelRequestStatusPut      = (*it_handle).getChannelRequestStatusPut(); //
            channelRequestStatusPut.setPreRequestStatus (ECAFE_NOWTACCESS);
            handle_index.modify(it_handle, change_channelRequestStatusPut(channelRequestStatusPut));
            handle_index.modify(it_handle, change_status(ECAFE_NOWTACCESS)); //for return
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
            return ECAFE_NOWTACCESS;
        }

        //First steps OK
        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        channelRequestStatusPut      = (*it_handle).getChannelRequestStatusPut(); //
        channelRequestStatusPut.setPreRequestStatus (ICAFE_NORMAL);
        handle_index.modify(it_handle, change_channelRequestStatusPut(channelRequestStatusPut));
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }

        channelRequestMetaDataClient= (*it_handle).getChannelRequestMetaDataClient();  // 2

        //2
        chtype clientT = _dbrType%(LAST_TYPE+1);


        if (clientT != channelRequestMetaDataClient.getDataType() ||
                _dbrType != channelRequestMetaDataClient.getDbrDataType() )
        {

            channelRequestMetaDataClient.setDataType   (clientT);
            channelRequestMetaDataClient.setDbrDataType(_dbrType);

            channelRequestMetaDataClient.setCafeDbrType(
                (CAFENUM::DBR_TYPE) helper.convertToCAFEDbrTypeClass(_dbrType) );


            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            handle_index.modify(it_handle,
                                change_channelRequestMetaDataClient(channelRequestMetaDataClient));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

            channelRequestMetaDataClient= (*it_handle).getChannelRequestMetaDataClient();
        }
    }
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  _handle << endl;
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}


/**
 *  \brief channelPreparePut: prepare client's requested data for ca call
 *  \param _handle input: handle
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE;
 */
int  Granules::channelPreparePut(const unsigned int  _handle)
{
#define	__METHOD__  "Granules::channelPreparePut"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        channelRequestMetaDataClient  = (*it_handle).getChannelRequestMetaDataClient();   //
        channelRequestMetaPrimitive   = (*it_handle).getChannelRequestMetaPrimitive();    //
        channelRequestDataTypePolicy  = (*it_handle).getChannelRequestDataTypePolicy();   //

        bool changeChannelRequestMetaData=false;

        chtype clientRequestType = channelRequestMetaPrimitive.getDataType();
        CAFENUM::DBR_TYPE  cafeDbrType = channelRequestMetaPrimitive.getCafeDbrType();

        // Do we transfer data with the smaller sized type?
        if (channelRequestDataTypePolicy.getRequestKind()==CAFENUM::LOWEST_DATATYPE)
        {
            clientRequestType = convertMatrix((*it_handle).getChannelRegalia().getDataType(),
                                              channelRequestMetaDataClient.getDataType());
            channelRequestMetaPrimitive.setDataType(clientRequestType);

            changeChannelRequestMetaData=true;
        }


        if (channelRequestMetaDataClient.getCafeDbrType() > channelRequestMetaPrimitive.getCafeDbrType())
        {
            cafeDbrType = channelRequestMetaDataClient.getCafeDbrType();
            channelRequestMetaPrimitive.setCafeDbrType(cafeDbrType);
            changeChannelRequestMetaData=true;
        }

        if (changeChannelRequestMetaData)
        {
            //set dbrType according to cafeDbrType;
            switch (cafeDbrType)
            {
            case CAFENUM::DBR_TIME:
            case CAFENUM::DBR_STS:
            case CAFENUM::DBR_PRIMITIVE:
                channelRequestMetaPrimitive.setDbrDataType(dbf_type_to_DBR(clientRequestType));
                break;
            default:
                channelRequestMetaPrimitive.setDbrDataType(dbf_type_to_DBR(clientRequestType));
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "CAFE INTERNAL FUNNY!: Method does not support this DBR_TYPE: "
                     << dbr_type_to_text(channelRequestMetaPrimitive.getCafeDbrType()) << endl; //OK
                cout << "This line should never appear!" << endl;
                break;
            }
        }

        //Check the number of elements requested?
        if ((*it_handle).getChannelRegalia().getNelem()>1)   // or  channelRequestMetaDataClient.getNelem() >1
        {

            //What did the client request? What is native type? Transfer minimum of this.
            if (channelRequestMetaPrimitive.getNelem() !=  channelRequestMetaDataClient.getNelem())
            {
                channelRequestMetaPrimitive.setNelem( min((*it_handle).getChannelRegalia().getNelem(),
                                                      channelRequestMetaDataClient.getNelem()) );
                changeChannelRequestMetaData=true;
            }

        }

        if (changeChannelRequestMetaData==true)
        {
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            handle_index.modify(it_handle,
                                change_channelRequestMetaPrimitive(channelRequestMetaPrimitive));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
        };
    }
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief channelExecutePut: prepare client's requested data for ca call
 *  \param _handle input: handle
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE;
 */
int  Granules::channelExecutePut(const unsigned int  _handle)
{
#define	__METHOD__  "Granules::channelExecutePut"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {


        channelTimeoutPolicyPut = (*it_handle).getChannelTimeoutPolicyPut();
        channelRequestMetaPrimitive  = (*it_handle).getChannelRequestMetaPrimitive();
        channelRequestPolicyPut = (*it_handle).getChannelRequestPolicyPut();

        methodKind =channelRequestPolicyPut.getMethodKind();

        //IF NO WAIT - WE WANT THIS TO BE ASYNCHRONOUS
        if ( (channelRequestPolicyPut.getWaitKind() == NO_WAIT) && (methodKind == WITHOUT_CALLBACK))
        {
            methodKind=  WITH_CALLBACK_DEFAULT;
        }

        //using namespace boost::posix_time;

        int requestStatus=ICAFE_NORMAL;

        switch (methodKind)
        {
        case WITH_CALLBACK_DEFAULT:
        case WITH_CALLBACK_USER_SUPPLIED:

            //JC Do not wait for callback; better for e.g. sliders
            //JC if CAFENUM::COMPLETE, then still flush for piling set values
            //JC See setPutPrepare(h) followed by setPut(h)

            //cout << __METHOD__ << " Handle " << (*it_handle).getHandle() << endl;
            //cout << __METHOD__ << "ProgressKind " << (*it_handle).getChannelRequestStatusPut().getCallbackProgressKind()  << endl;

            //Last put action not yet reported
            if ((*it_handle).getChannelRequestStatusPut().getCallbackProgressKind() == CAFENUM::PENDING // CAFENUM::COMPLETE (2)
                    && (*it_handle).isConnected() )
            {


                //isConnected?

                // this is required for when flush not given by client!

                //What is the Policy? To flush now or later?
                //
                if (channelRequestPolicyPut.getWaitKind() == NO_WAIT &&
                        channelRequestPolicyPut.getWhenToFlushSendBuffer() == FLUSH_DESIGNATED_TO_CLIENT)
                {
                    ca_flush_io();//NON-BLOCKING Flush anyway! Flush anyway as sets are piling up!

                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
		    std::cout << (*it_handle).getPV() << ", Handle= " << (*it_handle).getHandle() << endl;
                    std::cout << "Handle configured for Aggregated, Non-Blocking, Asynchronous SET. Hence"  << std::endl;
                    std::cout << "CAFE enabling early flush for previous, pending set method(s) " << std::endl;
                    std::cout << "Current set method will be initiated by user (or by CAFE for repeated channels)" << std::endl;
         

                }



                //wait until previous request finishes//wait until previous request finishes
                /*
                status=waitForPutEvent(_handle, channelTimeoutPolicyPut.getTimeout() );
                if (status==ECAFE_TIMEOUT && channelTimeoutPolicyPut.getSelfGoverningTimeout()
                && (*it_handle).isConnected() ) {
                unsigned short ntries=0;
                while (status==ECAFE_TIMEOUT && ntries<channelTimeoutPolicyPut.getNtries()
                && (*it_handle).isConnected()) {
                status=waitForPutEvent(_handle, channelTimeoutPolicyPut.getTimeout() +
                          channelTimeoutPolicyPut.getDeltaTimeout()*(++ntries));
                }
                if ((*it_handle).isConnected()) {
                std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                std::cout << "No of waitForPutEvent tries=" << ntries << std::endl;

                if (status==ECAFE_TIMEOUT) {
                std::cout << "is the MAXIMUM allowed as configured through TimeoutPolicy! "  << std::endl;
                }


                std::cout << "Changing timeout for handle/pv "
                << _handle << "/" << (*it_handle).getPV() << " to: "  <<
                (channelTimeoutPolicyPut.getTimeout() +
                channelTimeoutPolicyPut.getDeltaTimeout()*ntries) << " seconds"  <<endl;

                //modify timeout for handle
                channelTimeoutPolicyPut.setTimeout( (channelTimeoutPolicyPut.getTimeout() +
                                          channelTimeoutPolicyPut.getDeltaTimeout()*ntries));
                if(MUTEX){cafeMutex.lock();}    //lock
                handle_index.modify(it_handle, change_channelTimeoutPolicyPut(channelTimeoutPolicyPut));
                if(MUTEX){cafeMutex.unlock();}  //unlock
                } //isConnected
                }
                */

            }


            if ( CHECK_CONSISTENCY_CA_STATE && !(*it_handle).isConnected())
            {
                status=helper.checkConsistency(_handle);
                /*
                if (status!=ICAFE_NORMAL)
                {
                    return status;
                }
                */
            }

            if (!(*it_handle).isConnected())
            {
                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                channelRequestStatusPut = (*it_handle).getChannelRequestStatusPut();
                channelRequestStatusPut.setCallbackKind(false, false);

                handle_index.modify(it_handle, change_channelRequestStatusPut(channelRequestStatusPut));
                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }
                return ICAFE_CS_DISCONN;
            }

            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }

            channelRequestStatusPut = (*it_handle).getChannelRequestStatusPut();
            channelRequestStatusPut.setCallbackKind(true, false); //PENDING NOT_TRIGGERED WAITING FOR FLUSH

            handle_index.modify(it_handle, change_channelRequestStatusPut(channelRequestStatusPut));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

            if (methodKind==WITH_CALLBACK_DEFAULT)
            {
                status=((*it_handle)).putWithCallback(CALLBACK_CAFE::handlerPut);

            }
            else   //WITH_CALLBACK_USER_SUPPLIED:
            {
                // Check getHandler is not NULL
                // If it is then use default callback!

                if (channelRequestPolicyPut.getHandler() != NULL)
                {
                    status=((*it_handle)).putWithCallback(channelRequestPolicyPut.getHandler());

                }
                else
                {
                    std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
                    std::cout << "NO CALLBACK FUNCTION FOR PUT SUPPLIED. USING DEFAULT CALLBACK_CAFE::handlerPut " << std::endl;
                    status=((*it_handle)).putWithCallback(CALLBACK_CAFE::handlerPut);
                }
            }


            requestStatus=status;
            //channelRequestStatusPut.setRequestStatus (status);


            if (status != ECA_NORMAL)
            {

                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                channelRequestStatusPut = (*it_handle).getChannelRequestStatusPut();
                channelRequestStatusPut.setRequestStatus (requestStatus);
                channelRequestStatusPut.setCallbackKind(false, false);      // NOT_INITIATED NOT_TRIGGERED
                handle_index.modify(it_handle, change_channelRequestStatusPut(channelRequestStatusPut));
                handle_index.modify(it_handle, change_status(status));
                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }
                return status;
            }


            //What is the policy to wait?
            if (channelRequestPolicyPut.getWaitKind() == WAIT)
            {


                ca_flush_io(); //NON-BLOCKING
                //time start here
                //ptime timeStart(microsec_clock::local_time());

                status=waitForPutEvent(_handle, channelTimeoutPolicyPut.getTimeout() );


                if (status==ECAFE_TIMEOUT && channelTimeoutPolicyPut.getSelfGoverningTimeout()
                        && (*it_handle).isConnected())
                {
                    unsigned short ntries=0;
                    while (status==ECAFE_TIMEOUT && ntries<channelTimeoutPolicyPut.getNtries()
                            && (*it_handle).isConnected())
                    {
                        status=waitForPutEvent(_handle, channelTimeoutPolicyPut.getTimeout() +
                                               channelTimeoutPolicyPut.getDeltaTimeout()*(++ntries));
                    }

                    if ((*it_handle).isConnected())
                    {

                        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                        std::cout << "No of waitForPutEvent (additional) tries=" << ntries << std::endl;

                        if (status==ECAFE_TIMEOUT)
                        {
                            std::cout << "is the MAXIMUM allowed as configured through TimeoutPolicy! "  << std::endl;

                            std::cout << " SELF-GOVERNING TIMEOUT FOR SET OPERATIONS FOR THIS CHANNEL"  << std::endl;
                            channelTimeoutPolicyPut.setSelfGoverningTimeout(false);
                            channelTimeoutPolicyPut.setTimeout( channelTimeoutPolicyPut.getTimeout() );
                        }
                        else
                        {
                            std::cout << "Changing timeout for handle/pv "
                                      << _handle << "/" << (*it_handle).getPV() << " to: "  <<
                                      (channelTimeoutPolicyPut.getTimeout() +
                                       channelTimeoutPolicyPut.getDeltaTimeout()*ntries) << " seconds"  <<endl;
                            channelTimeoutPolicyPut.setTimeout( (channelTimeoutPolicyPut.getTimeout() +
                                                                 channelTimeoutPolicyPut.getDeltaTimeout()*ntries));
                        }
                        if(MUTEX)
                        {
                            cafeMutex.lock();   //lock
                        }
                        handle_index.modify(it_handle, change_channelTimeoutPolicyPut(channelTimeoutPolicyPut));
                        if(MUTEX)
                        {
                            cafeMutex.unlock();   //unlock
                        }

                        if (status==ECAFE_TIMEOUT)
                        {
                            std::cout << "CURRENT STATUS OF HANDLE: " << std::endl;
                            helper.printHandle(_handle);
                        }
                    } //isConnected
                }


                //Check these in at end of routine


                if (status != ICAFE_NORMAL || !(*it_handle).isConnected())
                {


                    if(MUTEX)
                    {
                        cafeMutex.lock();   //lock
                    }
                    channelRequestStatusPut = (*it_handle).getChannelRequestStatusPut();
                    channelRequestStatusPut.setCallbackKind(false, false); // NOT_INITIATED NOT_TRIGGERED
                    channelRequestStatusPut.setCallbackStatus    (status);
                    channelRequestStatusPut.setRequestStatus (requestStatus);
                    handle_index.modify(it_handle,
                                        change_channelRequestStatusPut(channelRequestStatusPut));
                    handle_index.modify(it_handle, change_status(status));
                    if(MUTEX)
                    {
                        cafeMutex.unlock();   //unlock
                    }

                }

            } //IF WAIT
            else
            {

                //What is the Policy? To flush now or later?
                if (channelRequestPolicyPut.getWhenToFlushSendBuffer() == FLUSH_NOW)
                {
                    ca_flush_io();
                }


            }


            break;

        case WITHOUT_CALLBACK:
        default:

            status=(*it_handle).put();


            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }


            channelRequestStatusPut = (*it_handle).getChannelRequestStatusPut();
            channelRequestStatusPut.setRequestStatus    (status);
            handle_index.modify(it_handle, change_channelRequestStatusPut(channelRequestStatusPut));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

            if (status != ECA_NORMAL)
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
                return status;
            }

            status=ca_pend_io(channelTimeoutPolicyPut.getTimeout());

            if (status==ECA_TIMEOUT && channelTimeoutPolicyPut.getSelfGoverningTimeout())
            {
                unsigned short ntries=0;
                while (status==ECA_TIMEOUT && ntries<channelTimeoutPolicyPut.getNtries())
                {

                    status=(*it_handle).put();
                    if (status != ECA_NORMAL)
                    {
                        channelRequestStatusPut.setRequestStatus    (status);

                        if(MUTEX)
                        {
                            cafeMutex.lock();   //lock
                        }
                        handle_index.modify(it_handle,
                                            change_channelRequestStatusPut(channelRequestStatusPut));

                        handle_index.modify(it_handle, change_status(status));
                        if(MUTEX)
                        {
                            cafeMutex.unlock();   //unlock
                        }
                        return status;
                    }

                    status=ca_pend_io(channelTimeoutPolicyPut.getTimeout() +
                                      channelTimeoutPolicyPut.getDeltaTimeout()*(++ntries));
                }

                std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                std::cout << "No of ca_pend_io tries= " << ntries << std::endl;
                if (status==ECA_TIMEOUT)
                {
                    std::cout << "is the MAXIMUM allowed as configured through TimeoutPolicy! "  << std::endl;
                    std::cout << "TURNING OFF SELF-GOVERNING TIMEOUT FOR SET OPERATIONS FOR THIS CHANNEL"  << std::endl;
                    channelTimeoutPolicyPut.setSelfGoverningTimeout(false);
                    channelTimeoutPolicyPut.setTimeout( channelTimeoutPolicyPut.getTimeout());
                    std::cout << "AND TIMEOUT RESTORED TO START VALUE OF " << channelTimeoutPolicyPut.getTimeout() << endl;
                }

                else
                {

                    std::cout << "Changing timeout for handle/pv "
                              << _handle << "/" << (*it_handle).getPV() << " to: "  <<
                              (channelTimeoutPolicyPut.getTimeout() +
                               channelTimeoutPolicyPut.getDeltaTimeout()*ntries) << endl;
                    //modify handle
                    channelTimeoutPolicyPut.setTimeout( (channelTimeoutPolicyPut.getTimeout() +
                                                         channelTimeoutPolicyPut.getDeltaTimeout()*ntries));
                }



                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                handle_index.modify(it_handle, change_channelTimeoutPolicyPut(channelTimeoutPolicyPut));
                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }

                if (status==ECA_TIMEOUT)
                {
                    std::cout << "CURRENT STATUS OF HANDLE: " << std::endl;
                    helper.printHandle(_handle);
                }
            }


            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }

            channelRequestStatusPut = (*it_handle).getChannelRequestStatusPut();
            //Check these in at end of routine
            channelRequestStatusPut.setPendStatus    (status);
            handle_index.modify(it_handle, change_channelRequestStatusPut(channelRequestStatusPut));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

            break;
        }

    }
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }

    return status;
#undef __METHOD__
}

/**
 *  \brief Contains methods that are used by Instant template, namely
 *  verifyChannelGetMethod: preVerification of client's requested data
 *  \param _handle input: handle to Conduit Object
 *  \param _dbrType input: chtype
 *  \return ICAFE_NORMAL or ICAFE_CS_NEVER_CONN or ICAFE_CA_OP_CONN_DOWN or ECAFE_NORDACCESS
 */
int  Granules::channelVerifyGet(const unsigned int  _handle, chtype _dbrType)
{
#define	__METHOD__  "Granules::channelVerifyGet"
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if ( (*it_handle).getChannelRegalia().getCafeConnectionState()==ICAFE_CS_NEVER_CONN)
        {
            return ICAFE_CS_NEVER_CONN;
        }
        else if ( (*it_handle).getChannelRegalia().getCafeConnectionState()==ICAFE_CS_CLOSED)
        {
            return ICAFE_CS_CLOSED;
        }



        if ( CHECK_CONSISTENCY_CA_STATE && !(*it_handle).isConnected())
        {
            status=helper.checkConsistency(_handle);
            /*
            if (status!=ICAFE_NORMAL)
            {
                return status;
            }
            */ 
        }


        //isConnected
        if ( !(*it_handle).isConnected())
        {

            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }

            //Check if it was ever connected??
            channelRequestStatusGet      = (*it_handle).getChannelRequestStatusGet(); //
            channelRequestStatusGet.setPreRequestStatus (ICAFE_CA_OP_CONN_DOWN);

            handle_index.modify(it_handle, change_channelRequestStatusGet(channelRequestStatusGet));

            handle_index.modify(it_handle, change_status(ICAFE_CA_OP_CONN_DOWN)); //for return
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
            return ICAFE_CA_OP_CONN_DOWN;
        }

        //hasReadAccess
        if ((*it_handle).getAccessRead() == 0)
        {
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            channelRequestStatusGet      = (*it_handle).getChannelRequestStatusGet(); //
            channelRequestStatusGet.setPreRequestStatus (ECAFE_NORDACCESS);

            handle_index.modify(it_handle, change_channelRequestStatusGet(channelRequestStatusGet));

            handle_index.modify(it_handle, change_status(ECAFE_NORDACCESS)); //for return
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
            return ECAFE_NORDACCESS;
        }

        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        //First steps OK
        channelRequestStatusGet      = (*it_handle).getChannelRequestStatusGet(); //
        channelRequestStatusGet.setPreRequestStatus (ICAFE_NORMAL);

        handle_index.modify(it_handle, change_channelRequestStatusGet(channelRequestStatusGet));
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }

        // This check of the datatype requested by client and that recorded in the hash table
        // is now done in advance of CAFENUM::GET_CACHE_NO_CHECK check made in getCache in instanst.cpp
        channelRequestMetaDataClient= (*it_handle).getChannelRequestMetaDataClient();  // 2

        //2
        chtype clientT = _dbrType%(LAST_TYPE+1);

        if (clientT != channelRequestMetaDataClient.getDataType() ||
                _dbrType != channelRequestMetaDataClient.getDbrDataType() )
        {

            channelRequestMetaDataClient.setDataType   (clientT);
            channelRequestMetaDataClient.setDbrDataType(_dbrType);

            channelRequestMetaDataClient.setCafeDbrType(
                (CAFENUM::DBR_TYPE) helper.convertToCAFEDbrTypeClass(_dbrType) );


            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            handle_index.modify(it_handle,
                                change_channelRequestMetaDataClient(channelRequestMetaDataClient));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

            channelRequestMetaDataClient= (*it_handle).getChannelRequestMetaDataClient();
        }
    }
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  _handle << endl;
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief channelPrepareGet: prepare client's requested data for ca call
 *  \param _handle input: handle
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE;
 */
int  Granules::channelPrepareGet(const unsigned int  _handle)
{
#define	__METHOD__  "Granules::channelPrepareGet"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        channelRequestMetaDataClient  = (*it_handle).getChannelRequestMetaDataClient();   //
        channelRequestMetaData        = (*it_handle).getChannelRequestMetaData();         //
        channelRequestDataTypePolicy  = (*it_handle).getChannelRequestDataTypePolicy();   //

        bool changeChannelRequestMetaData=false;

        chtype clientRequestType = channelRequestMetaData.getDataType();
        CAFENUM::DBR_TYPE  cafeDbrType = channelRequestMetaData.getCafeDbrType();


        // Do we transfer data with the smaller sized type?
        if (channelRequestDataTypePolicy.getRequestKind()==CAFENUM::LOWEST_DATATYPE)
        {

            clientRequestType = convertMatrix((*it_handle).getChannelRegalia().getDataType(),
                                              channelRequestMetaDataClient.getDataType());
            channelRequestMetaData.setDataType(clientRequestType);

            changeChannelRequestMetaData=true;

        }


        if (channelRequestMetaDataClient.getCafeDbrType() > channelRequestMetaData.getCafeDbrType()
                && channelRequestMetaDataClient.getCafeDbrType() <= CAFENUM::DBR_TIME )
        {
            cafeDbrType = channelRequestMetaDataClient.getCafeDbrType();
            channelRequestMetaData.setCafeDbrType(cafeDbrType);
            changeChannelRequestMetaData=true;
        }

        if (changeChannelRequestMetaData)
        {
            //set dbrType according to cafeDbrType;
            switch (cafeDbrType)
            {
            case CAFENUM::DBR_TIME:
                channelRequestMetaData.setDbrDataType(dbf_type_to_DBR_TIME(clientRequestType));
                break;
            case CAFENUM::DBR_STS:
                channelRequestMetaData.setDbrDataType(dbf_type_to_DBR_STS (clientRequestType));
                break;
            case CAFENUM::DBR_PRIMITIVE:
                channelRequestMetaData.setDbrDataType(dbf_type_to_DBR     (clientRequestType));
                break;
            default:
                channelRequestMetaData.setDbrDataType(dbf_type_to_DBR_TIME(clientRequestType));
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "Method does not support this DBR_TYPE: "
                     << dbr_type_to_text(channelRequestMetaData.getCafeDbrType()) << endl; //OK

                cout << "This line should never appear!" << endl;
                cout << "Handle=" << _handle << " PVNAME " << (*it_handle).pv << endl;
                cout << "The requested cafeDbrType was " << cafeDbrType << endl;
                cout << "Valid values are CAFENUM::DBR_PRIMITIVE  " <<  CAFENUM::DBR_PRIMITIVE << endl;
                cout << "Valid values are CAFENUM::DBR_STS  " <<  CAFENUM::DBR_STS << endl;
                cout << "Valid values are CAFENUM::DBR_TIME " <<  CAFENUM::DBR_TIME << endl;
                cout << "Valid values are CAFENUM::DBR_GR "   <<  CAFENUM::DBR_GR << endl;
                cout << "Valid values are CAFENUM::DBR_CTRL " <<  CAFENUM::DBR_CTRL << endl;
                cout << "Valid values are CAFENUM::DBR_PUT  " <<  CAFENUM::DBR_PUT << endl;
                cout << "Valid values are CAFENUM::DBR_STSACK " <<  CAFENUM::DBR_STSACK << endl;
                cout << "Valid values are CAFENUM::DBR_CLASS "  <<  CAFENUM::DBR_CLASS << endl;
                cout << "Valid values are CAFENUM::DBR_NONE "   <<  CAFENUM::DBR_NONE << endl;
                break;
            }
        }

        //Check the number of elements requested?


        if ((*it_handle).getChannelRegalia().getNelem()>1)   // or  channelRequestMetaDataClient.getNelem() >1
        {


            //cout << __METHOD__ << endl;
            //cout << " channelRequestMetaData.getNelem() " << channelRequestMetaData.getNelem() << endl;
            //cout << " channelRequestMetaDataClient.getNelem() " << channelRequestMetaDataClient.getNelem() << endl;
            //cout << " channelRequestMetaDataClient.getOffset() " <<  channelRequestMetaDataClient.getOffset() << endl;

            //What did the client request? What is native type? Transfer minimum of this.
            if (channelRequestMetaData.getNelem() !=  (channelRequestMetaDataClient.getNelem()- channelRequestMetaDataClient.getOffset()))
            {
                //channelRequestMetaDataClient.setNelem(channelRequestMetaDataClient.getNelem()+channelRequestMetaDataClient.getOffset());

                //if(MUTEX){cafeMutex.lock();}   //lock
                //handle_index.modify(it_handle,
                //		change_channelRequestMetaDataClient(channelRequestMetaDataClient));
                //if(MUTEX){cafeMutex.unlock();} //unlock

                unsigned int newNelem=min( ((*it_handle).getChannelRegalia().getNelem()),
                                           channelRequestMetaDataClient.getNelem()+channelRequestMetaDataClient.getOffset());
                channelRequestMetaData.setNelem(newNelem);
                //if ( newNelem < channelRequestMetaData.getNelemCache()){
                //	channelRequestMetaData.setNelemCache(newNelem);
                //}
                changeChannelRequestMetaData=true;
                //cout << "newElem for MetaData " <<  newNelem << endl;
            }

            /*
            //What did the client request? What is native type? Transfer minimum of this.
            if (channelRequestMetaData.getNelem() !=  channelRequestMetaDataClient.getNelem()) {
            					unsigned int newNelem=min((*it_handle).getChannelRegalia().getNelem(),
            				 																 channelRequestMetaDataClient.getNelem());
            					channelRequestMetaData.setNelem(newNelem);
            //if ( newNelem < channelRequestMetaData.getNelemCache()){
            //	channelRequestMetaData.setNelemCache(newNelem);
            //}
            changeChannelRequestMetaData=true;
            }
            	*/

        }




        if (changeChannelRequestMetaData==true)
        {
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            handle_index.modify(it_handle,
                                change_channelRequestMetaData(channelRequestMetaData));

            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }


        };


        channelRequestMetaData        = (*it_handle).getChannelRequestMetaData();


    }
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  _handle << endl;
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}


/**
 *  \brief channelExecuteGet: Executes ca_get method and wiats for response
 *  \param _handle input: handle
 *  \return ICAFE_NORMAL else ECAFE error
 */
int  Granules::channelExecuteGet(const unsigned int  _handle)
{
#define	__METHOD__  "Granules::channelExecuteGet"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        channelTimeoutPolicyGet = (*it_handle).getChannelTimeoutPolicyGet();

        channelRequestMetaDataClient  = (*it_handle).getChannelRequestMetaDataClient();
        channelRequestPolicyGet = (*it_handle).getChannelRequestPolicyGet();

        switch (channelRequestPolicyGet.getMethodKind())
        {
        case WITH_CALLBACK_DEFAULT:
        case WITH_CALLBACK_USER_SUPPLIED:

            if (channelRequestPolicyGet.getMethodKind()==WITH_CALLBACK_DEFAULT)
            {
                status=((*it_handle)).getWithCallback(CALLBACK_CAFE::handlerGet);//CALLBACK_CAFE::callbackHandlerGet);
            }
            else   //WITH_CALLBACK_USER_SUPPLIED:
            {
                // Check getHandler is not NULL
                // If it is then use default callback!
                if (channelRequestPolicyGet.getHandler() != NULL)
                {
                    status=((*it_handle)).getWithCallback(channelRequestPolicyGet.getHandler());
                }
                else
                {
                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << "NO CALLBACK FUNCTION FOR GET SUPPLIED. USING DEFAULT CALLBACK_CAFE::handlerGet " << std::endl;
                    status=((*it_handle)).getWithCallback(CALLBACK_CAFE::handlerGet);
                }
            }


            if (status != ECA_NORMAL)
            {
                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
                channelRequestStatusGet.setRequestStatus (status);
                channelRequestStatusGet.setCallbackKind(false, false);      // NOT_INITIATED NOT_TRIGGERED

                handle_index.modify(it_handle, change_channelRequestStatusGet(channelRequestStatusGet));

                handle_index.modify(it_handle, change_status(status));
                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }
                return status;
            }


            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();

            channelRequestStatusGet.setCallbackKind(true, false);        //PENDING NOT_TRIGGERED

            handle_index.modify(it_handle, change_channelRequestStatusGet(channelRequestStatusGet));

            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

            ca_flush_io();

            status=waitForGetEvent(_handle, channelTimeoutPolicyGet.getTimeout() );

            if (status==ECAFE_TIMEOUT && channelTimeoutPolicyGet.getSelfGoverningTimeout()
                    && (*it_handle).isConnected() )
            {
                unsigned short ntries=0;
                while (status==ECAFE_TIMEOUT && ntries<channelTimeoutPolicyGet.getNtries()
                        && (*it_handle).isConnected() )
                {

                    status=waitForGetEvent(_handle, channelTimeoutPolicyGet.getTimeout() +
                                           channelTimeoutPolicyGet.getDeltaTimeout()*(++ntries));
                }
                if ((*it_handle).isConnected())
                {

                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << "No of waitForGetEvent (additional) tries=" << ntries << std::endl;

                    if (status==ECAFE_TIMEOUT)
                    {
                        std::cout << "is the MAXIMUM allowed as configured through TimeoutPolicy! "  << std::endl;
                        std::cout << "TURNING OFF SELF-GOVERNING TIMEOUT FOR GET OPERATIONS FOR THIS CHANNEL"  << std::endl;
                        channelTimeoutPolicyGet.setSelfGoverningTimeout(false);
                        channelTimeoutPolicyGet.setTimeout( channelTimeoutPolicyGet.getTimeout() );
                        std::cout << "AND TIMEOUT RESTORED TO START VALUE OF " << channelTimeoutPolicyGet.getTimeout() << endl;
                    }
                    else
                    {
                        std::cout << "Changing timeout for handle/pv "
                                  << _handle << "/" << (*it_handle).getPV() << " to: "  <<
                                  (channelTimeoutPolicyGet.getTimeout() +
                                   channelTimeoutPolicyGet.getDeltaTimeout()*ntries) << " seconds"  <<endl;

                        //modify timeout for handle
                        channelTimeoutPolicyGet.setTimeout( (channelTimeoutPolicyGet.getTimeout() +
                                                             channelTimeoutPolicyGet.getDeltaTimeout()*ntries));
                    }
                    if(MUTEX)
                    {
                        cafeMutex.lock();   //lock
                    }
                    handle_index.modify(it_handle, change_channelTimeoutPolicyGet(channelTimeoutPolicyGet));
                    if(MUTEX)
                    {
                        cafeMutex.unlock();   //unlock
                    }

                    if (status==ECAFE_TIMEOUT)
                    {
                        std::cout << "CURRENT STATUS OF HANDLE: " << std::endl;
                        helper.printHandle(_handle);
                    }

                } //if isConnected
            } //if

            else if (status==ECAFE_TIMEOUT)
            {
                std::cout << __METHOD__  << std::endl;
                std::cout << "TIMEOUT AFTER " << channelTimeoutPolicyGet.getTimeout() << " sec " << std::endl;
                std::cout << "CURRENT STATUS OF HANDLE/PV: " << std::endl;
                helper.printHandle(_handle);
            }

            if (!(*it_handle).isConnected() || status != ICAFE_NORMAL)
            {
                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
                channelRequestStatusGet.setCallbackStatus    (status);
                channelRequestStatusGet.setCallbackKind(false, false); // NOT_INITIATED NOT_TRIGGERED

                handle_index.modify(it_handle,
                                    change_channelRequestStatusGet(channelRequestStatusGet));

                if (!(*it_handle).isConnected())
                {
                    handle_index.modify(it_handle, change_status(ICAFE_CS_DISCONN));
                    if(MUTEX)
                    {
                        cafeMutex.unlock();   //unlock
                    }
                    return ICAFE_CS_DISCONN;
                }
                else
                {
                    handle_index.modify(it_handle, change_status(status));
                    if(MUTEX)
                    {
                        cafeMutex.unlock();   //unlock
                    }
                }

            }

            break;

        case WITHOUT_CALLBACK:
        default:

            status=(*it_handle).get();
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
            channelRequestStatusGet.setRequestStatus    (status);
            handle_index.modify(it_handle, change_channelRequestStatusGet(channelRequestStatusGet));

            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

            if (status != ECA_NORMAL)
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
                return status;
            }

            status=ca_pend_io(channelTimeoutPolicyGet.getTimeout());

            if (status==ECA_TIMEOUT && channelTimeoutPolicyGet.getSelfGoverningTimeout())
            {
                unsigned short ntries=0;
                while (status==ECA_TIMEOUT && ntries<channelTimeoutPolicyGet.getNtries())
                {

                    status=(*it_handle).get();
                    if (status != ECA_NORMAL)
                    {

                        if(MUTEX)
                        {
                            cafeMutex.lock();   //lock
                        }
                        channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
                        channelRequestStatusGet.setRequestStatus    (status);
                        handle_index.modify(it_handle,
                                            change_channelRequestStatusGet(channelRequestStatusGet));

                        handle_index.modify(it_handle, change_status(status));
                        if(MUTEX)
                        {
                            cafeMutex.unlock();   //unlock
                        }
                        return status;
                    }

                    status=ca_pend_io(channelTimeoutPolicyGet.getTimeout() +
                                      channelTimeoutPolicyGet.getDeltaTimeout()*(++ntries));
                }

                std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                std::cout << "No of ca_pend_io tries= " << ntries << std::endl;
                if (status==ECA_TIMEOUT)
                {
                    std::cout << "is the MAXIMUM allowed as configured through TimeoutPolicy! "  << std::endl;
                    std::cout << "TURNING OFF SELF-GOVERNING TIMEOUT FOR GET OPERATIONS FOR THIS CHANNEL"  << std::endl;
                    channelTimeoutPolicyGet.setSelfGoverningTimeout(false);
                    channelTimeoutPolicyGet.setTimeout( channelTimeoutPolicyGet.getTimeout() );
                    std::cout << "AND TIMEOUT RESTORED TO START VALUE OF " << channelTimeoutPolicyGet.getTimeout() << endl;
                }
                else
                {
                    std::cout << "Changing timeout for handle/pv "
                              << _handle << "/" << (*it_handle).getPV() << " to: "  <<
                              (channelTimeoutPolicyGet.getTimeout() +
                               channelTimeoutPolicyGet.getDeltaTimeout()*ntries) << endl;

                    //modify handle
                    channelTimeoutPolicyGet.setTimeout( (channelTimeoutPolicyGet.getTimeout() +
                                                         channelTimeoutPolicyGet.getDeltaTimeout()*ntries));
                }



                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }

                handle_index.modify(it_handle, change_channelTimeoutPolicyGet(channelTimeoutPolicyGet));

                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }


                cout << __FILE__ << " " << __METHOD__ << endl;
                cout << "status = " << status << endl;
                if (status==ECA_TIMEOUT)
                {
                    std::cout << "CURRENT STATUS OF HANDLE: " << std::endl;
                    helper.printHandle(_handle);
                }
            }


            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
            //Check these in at end of routine
            channelRequestStatusGet.setPendStatus    (status);
            handle_index.modify(it_handle,
                                change_channelRequestStatusGet(channelRequestStatusGet));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

            break;
        }



        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        //Important to place this here after the get is done!
        channelRequestMetaData  = (*it_handle).getChannelRequestMetaData();
        channelRequestMetaData.setOffset(channelRequestMetaDataClient.getOffset());
        handle_index.modify(it_handle, change_status(status));
        handle_index.modify(it_handle, change_channelRequestMetaData(channelRequestMetaData));
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }

    }
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  _handle << endl;
        return ECAFE_INVALID_HANDLE;
    }

    return status; //Could still give a timeout depending on policy
#undef __METHOD__
}



/**
 *  \brief channelExecuteGetNoWait: Executes ca_get method WITH CALLBACK! \n
 *  A channelRequestPolicyGet.getMethodKind()==WITHOUT_CALLBACK  setting is \n
 *  ignored; getMethodKind()=WITH_DEFAULT_CALLBACK is then used
 *  \param _handle input: handle
 *  \return ICAFE_NORMAL if all OK else ECAFE error
 */
int  Granules::channelExecuteGetNoWait(const unsigned int  _handle)
{
#define	__METHOD__  "Granules::channelExecuteGetNoWait "

    status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        channelRequestPolicyGet = (*it_handle).getChannelRequestPolicyGet();


        channelRequestMetaDataClient  = (*it_handle).getChannelRequestMetaDataClient();

        if ((*it_handle).getChannelRequestStatusGet().getCallbackProgressKind() != CAFENUM::PENDING)
        {

            switch (channelRequestPolicyGet.getMethodKind())
            {
            case WITHOUT_CALLBACK:
            case WITH_CALLBACK_DEFAULT:
            case WITH_CALLBACK_USER_SUPPLIED:

                //Check these in at end of routine
                //channelRequestStatusGet.setCallbackKind(true, false);        //PENDING NOT_TRIGGERED
                //channelRequestStatusGet.setCallbackStatus    (status);

                if (channelRequestPolicyGet.getMethodKind()==WITH_CALLBACK_DEFAULT ||
                        channelRequestPolicyGet.getMethodKind()==WITHOUT_CALLBACK )
                {
                    status=((*it_handle)).getWithCallback(CALLBACK_CAFE::handlerGet);//CALLBACK_CAFE::callbackHandlerGet);

                }
                else   //WITH_CALLBACK_USER_SUPPLIED:
                {
                    // Check getHandler is not NULL
                    // If it is then use default callback!
                    if (channelRequestPolicyGet.getHandler() != NULL)
                    {
                        status=((*it_handle)).getWithCallback(channelRequestPolicyGet.getHandler());
                    }
                    else
                    {
                        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                        std::cout << "NO CALLBACK FUNCTION FOR GET SUPPLIED. USING DEFAULT CALLBACK_CAFE::handlerGet " << std::endl;
                        status=((*it_handle)).getWithCallback(CALLBACK_CAFE::handlerGet);
                    }
                }

                if (status != ECA_NORMAL)
                {
                    if(MUTEX)
                    {
                        cafeMutex.lock();   //lock
                    }
                    channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
                    channelRequestStatusGet.setCallbackKind(false, false);      // NOT_INITIATED NOT_TRIGGERED
                    channelRequestStatusGet.setRequestStatus (status);

                    handle_index.modify(it_handle, change_channelRequestStatusGet(channelRequestStatusGet));
                    handle_index.modify(it_handle, change_status(status));
                    if(MUTEX)
                    {
                        cafeMutex.unlock();   //unlock
                    }
                    return status;
                }

                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
                channelRequestStatusGet.setRequestStatus (status);
                channelRequestStatusGet.setCallbackKind(true, false);        //PENDING NOT_TRIGGERED

                handle_index.modify(it_handle,
                                    change_channelRequestStatusGet(channelRequestStatusGet));
                //cout << __METHOD__ << " CALLBACK STARTED " << (*it_handle).getChannelRequestStatusGet().getCallbackProgressKind() << endl;

                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }
                break;

            } //switch

            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }

            handle_index.modify(it_handle, change_status(status));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

        }
        else
        {

          //Wait for BundledEvents!! Sep. 2020
	  status = ICAFE_NORMAL;
	  //status = ICAFE_WAITING_FOR_PREV_CALLBACK;

            //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            //cafeStatus.report(status);

        } //if


        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        channelRequestMetaData  = (*it_handle).getChannelRequestMetaData();
        channelRequestMetaData.setOffset(channelRequestMetaDataClient.getOffset());
        handle_index.modify(it_handle, change_status(status));
        handle_index.modify(it_handle, change_channelRequestMetaData(channelRequestMetaData));
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }


    } //handle

    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  _handle << endl;
        return ECAFE_INVALID_HANDLE;
    }

    return status; //Could still give a timeout depending on policy
#undef __METHOD__
}


/**
 *  \brief waitForManyGetEvents: waits for all get callback to complete
 *  \param handleArray input: array of handles
 *  \param arrayLength input: size of array of handles
 *  \param bundleResponse output: map<unsigned int, int>
 *  \return ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  Granules::waitForManyGetEvents(const unsigned int * handleArray,  unsigned int arrayLength, std::map<unsigned int, int> & bundleResponse)
{
#define	__METHOD__  "Granules::waitForManyGetEvents"

    unsigned int nWaitActive=0;
    unsigned int nWaitActiveOver=0;
    //ChannelRequestPolicy channelGetMethodPolicy;
    //CAFEConduit cc;
    int status;
    int statusTransaction;
    int statusBundle= ICAFE_NORMAL;

    ca_flush_io();

    //Start Time
    using namespace boost::posix_time;
    ptime timeStart(microsec_clock::local_time());

    double timeoutHighestOriginal =0;
    double timeElapsed    =0;
    double timeoutHighest =0;
    unsigned short ntries =0;

    bundleResponse.clear();
    //first determine the number of handles

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    for (unsigned int i=0; i<arrayLength; ++i)
    {

        it_handle = handle_index.find(handleArray[i]);

        if (it_handle != handle_index.end())
        {

            channelRequestPolicyGet = (*it_handle).getChannelRequestPolicyGet();
            channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
            channelTimeoutPolicyGet = (*it_handle).getChannelTimeoutPolicyGet();
            statusTransaction= channelRequestStatusGet.getMessageStatus();

            //if (channelGetMethodPolicy.getWaitKind()==CAFENUM::NO_WAIT) {

            timeoutHighest = std::max(timeoutHighest,channelTimeoutPolicyGet.getTimeout());
            timeoutHighestOriginal = timeoutHighest;
            ++nWaitActive;
            //Next check for status
            //if (statusTransaction==ICAFE_NORMAL || statusTransaction==ICAFE_WAITING_FOR_PREV_CALLBACK) {
            //	++nWaitActual;
            //}
            //}

        }
    }

    //cout << "NHANDLE " << " nWaitActive " << nWaitActive << endl;
    //cout << "Timeout Period = " << timeoutHighest << endl;

    unsigned long nIterations=0;
    vector<unsigned int> isDone(arrayLength,0);

    while (nWaitActiveOver!=nWaitActive)
    {

        nWaitActiveOver=0;

        ///// ca_flush_io();
        //usleep(20);

#if HAVE_BOOST_THREAD
        boost::this_thread::sleep_for(boost::chrono::microseconds(20));
#else
#if HAVE_LINUX
        usleep(20);
#endif
#endif
        //cout << " (*it_handle).getChannelRequestStatusGet().getCallbackProgressKind() " << \
        //(*it_handle).getChannelRequestStatusGet().getCallbackProgressKind() << endl;

        for (unsigned int i=0; i<arrayLength; ++i)
        {

            if (isDone[i]==1)
            {
                nWaitActiveOver++;
                continue;
            }

            it_handle = handle_index.find(handleArray[i]);

            if (it_handle != handle_index.end())
            {

                if ((*it_handle).getChannelRequestStatusGet().getCallbackProgressKind() != CAFENUM::PENDING
                        ||  (*it_handle).isConnected()==false

                   )
                {
                    nWaitActiveOver++;

                    channelRequestPolicyGet = (*it_handle).getChannelRequestPolicyGet();
                    status=ICAFE_NORMAL;
                    channelRequestPolicyGet.setCallbackStatus(status);
                    if(MUTEX)
                    {
                        cafeMutex.lock();
                    };   //lock
                    handle_index.modify(it_handle, change_channelRequestPolicyGet(channelRequestPolicyGet));
                    if(MUTEX)
                    {
                        cafeMutex.unlock();
                    }; //unlock
                    //cout << __METHOD__ << " timeElapsed " << timeElapsed << " // allowed wait time " << timeoutHighest << endl;
                    isDone[i]=1;

                    continue;
                }

                channelRequestPolicyGet = (*it_handle).getChannelRequestPolicyGet();
                channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
                channelTimeoutPolicyGet = (*it_handle).getChannelTimeoutPolicyGet();
                statusTransaction= channelRequestStatusGet.getMessageStatus();

                if ( timeElapsed > timeoutHighest)
                {

                    //Increase Timeout

                    if (((*it_handle).getChannelTimeoutPolicyGet()).getSelfGoverningTimeout())
                    {

                        if (ntries<channelTimeoutPolicyGet.getNtries())
                        {

                            timeoutHighest=  timeoutHighestOriginal+
                                             channelTimeoutPolicyGet.getDeltaTimeout()*(++ntries);

                        }
                        else
                        {
                            //Latest Status
                            status=ECAFE_TIMEOUT;
                            statusBundle=status;
                            channelRequestPolicyGet.setCallbackStatus(status);
                            std::cout << "No of additional wait tries=" << ntries << std::endl;
                            std::cout << "is the MAXIMUM allowed as configured through TimeoutPolicy! "  << std::endl;
                            std::cout << "TURNING OFF SELF-GOVERNING TIMEOUT FOR GET OPERATIONS FOR CHANNEL:"  << std::endl;
                            std::cout << "Handle= " << (*it_handle).getHandle() << " pv=" << (*it_handle).getPV() << endl;

                            channelTimeoutPolicyGet.setSelfGoverningTimeout(false);
                            //channelTimeoutPolicyGet.setTimeout( channelTimeoutPolicyGet.getTimeout() );
                            std::cout << "AND TIMEOUT RESTORED TO START VALUE OF " << channelTimeoutPolicyGet.getTimeout() << endl;


                            if(MUTEX)
                            {
                                cafeMutex.lock();
                            };   //lock
                            handle_index.modify(it_handle, change_channelRequestPolicyGet(channelRequestPolicyGet));
                            handle_index.modify(it_handle, change_channelTimeoutPolicyGet(channelTimeoutPolicyGet));
                            handle_index.modify(it_handle, change_status(status));
                            if(MUTEX)
                            {
                                cafeMutex.unlock();
                            };   //unlock
                            nWaitActiveOver++;
                        }
                    }
                    else
                    {
                        //Latest Status
                        status=ECAFE_TIMEOUT;
                        statusBundle=status;
                        channelRequestPolicyGet.setCallbackStatus(status);
                        if(MUTEX)
                        {
                            cafeMutex.lock();
                        };   //lock
                        handle_index.modify(it_handle, change_channelRequestPolicyGet(channelRequestPolicyGet));
                        handle_index.modify(it_handle, change_status(status));
                        if(MUTEX)
                        {
                            cafeMutex.unlock();
                        };   //unlock
                        nWaitActiveOver++;
                    }
                } //if ( timeElapsed > timeoutHighest) {

            }//if handle
            else
            {
                //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  handleArray[i] << endl;

                status= ECAFE_INVALID_HANDLE;

            }
        }//for



        //Not required as will just out of loop for nWaitActiveOver==WaitActive
        //if (timeElapsed > timeoutHighest && nWaitActive != nWaitActiveOver) {
        //	cout << __METHOD__ << " timeElapsed " << timeElapsed << " //// exceeded final allowed wait time " << timeoutHighest << endl;
        //	break;
        //}

        ///cout << nWaitActiveOver << " nWaitActiveOver// " << nWaitActive << endl;

        //About to just loop!
        //if (nWaitActiveOver != nWaitActive) {

        //End time
        ptime timeEnd(microsec_clock::local_time());
        time_duration duration(timeEnd-timeStart);
        timeElapsed= (double) duration.total_microseconds()/1000000.0;


#if HAVE_BOOST_THREAD
        boost::this_thread::sleep_for(boost::chrono::microseconds(20));
#else
#if HAVE_LINUX
        usleep(20);
#endif
#endif
        //}
        ++nIterations;
        ///cout << "No iterations << " <<   nIterations << endl;
        ////if (nIterations>5000) break;
    }//while

    //Set Final Status of each handle


    //cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    //cafeConduit_set_by_handle::iterator it_handle;

    for (unsigned int i=0; i<arrayLength; ++i)
    {

        it_handle = handle_index.find(handleArray[i]);

        if (it_handle != handle_index.end())
        {

            channelRequestPolicyGet  = (*it_handle).getChannelRequestPolicyGet();

            //Will report Timeouts but Not Channel Disconect!
            if (!bundleResponse.insert(make_pair((*it_handle).getHandle(),
                                                 (*it_handle).getChannelRequestPolicyGet().getCallbackStatus())).second )
            {
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "bundleResponse insert failed due to duplicate handle " << endl;
                cout << "H=" << (*it_handle).getHandle() << " CB=" <<  (*it_handle).getChannelRequestPolicyGet().getCallbackStatus() << endl;
            }
            //cout << i << " BS=" << statusBundle <<  " TO=" << (*it_handle).getChannelTimeoutPolicyGet().getTimeout() << endl;
        }
    }

    //cout << __METHOD__ << " timeElapsed " << timeElapsed << " //end// allowed wait time " << timeoutHighest << endl;
    //cout << "NO OF ITERATIONS=" << nIterations << " /WAIT/= " << nWaitActive << " /O/= " << nWaitActiveOver << endl;


    return statusBundle;


#undef __METHOD__
}

/**
 *  \brief waitForGetEvent: waits for get callback to complete
 *  \param _handle input: handle
 *  \param _timeout input: wait time before method timeouts
 *  \return ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  Granules::waitForGetEvent(const unsigned int  _handle, double _timeout)
{
#define	__METHOD__  "Granules::waitForGetEvent"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        using namespace boost::posix_time;
        ptime timeStart(microsec_clock::local_time());

        double  timeElapsed=0;
        unsigned int  nPoll=0;

        channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();

        status= channelRequestStatusGet.getMessageStatus(); //Message==Request

        while (channelRequestStatusGet.getCallbackProgressKind() == CAFENUM::PENDING
                && timeElapsed < _timeout)
        {
            ca_flush_io();

#if HAVE_BOOST_THREAD
            boost::this_thread::sleep_for(boost::chrono::microseconds(20));
#else
#if HAVE_LINUX
            usleep(20);
#endif
#endif
            ++nPoll;

            ptime timeEnd(microsec_clock::local_time());
            time_duration duration(timeEnd-timeStart);
            timeElapsed= (double) duration.total_microseconds()/1000000.0;

            channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();

        }

        //cout << "npoll " << nPoll << endl;

        if (timeElapsed >= _timeout )
        {
            std::cout <<  __METHOD__ <<  __LINE__ << " TimeElapsed "  << timeElapsed <<   "  timeout_pend_io  " << _timeout<< std::endl;
            //std::cout << " End= "  <<  tv.tv_sec  << " " <<   tv.tv_usec << " Start =" << startTime_sec << " " << startTime_usec << std::endl;
        }

        if (channelRequestStatusGet.getCallbackProgressKind() != CAFENUM::PENDING)
        {

            return ICAFE_NORMAL; //or status?
        }
        else
        {
            return ECAFE_TIMEOUT;
        };
    }
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  _handle << endl;
        return ECAFE_INVALID_HANDLE;
    }

#undef __METHOD__
}


/**
 *  \brief isGetCallbackDone performs a ca_poll() and queries the Conduit data container
 *  to determine if the getWithCallback has been completed
 *  \param   _handle input: handle
 *  \return  bool true or false
 */
bool Granules::isGetCallbackDone(const unsigned int  _handle)
{
#define __METHOD__  "Granules::isGetCallbackDone(_handle)"


    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if ((*it_handle).getChannelRequestStatusGet().getCallbackProgressKind() == CAFENUM::PENDING)
        {

            ca_poll();   // just in case a flush was forgotten
        }

        if ( (*it_handle).getChannelRequestStatusGet().getCallbackProgressKind() == CAFENUM::PENDING)
        {
            return false;
        }
        else
        {
            return true;
        }


    }

    cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
    cout << " Handle " << _handle << " does not exist! " << endl;
    cout << " Function for Handle " << _handle << " will return true to keep things going! " << endl;

    return true;

#undef __METHOD__
};




/**
 *  \brief waitForPutEvent: waits for put callback to complete
 *  \param _handle input: handle
 *  \param _timeout input: wait time before method timeouts
 *  \return ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  Granules::waitForPutEvent(const unsigned int  _handle, double _timeout)
{
#define	__METHOD__  "Granules::waitForPutEvent"

    //using namespace boost::posix_time;


    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        using namespace boost::posix_time;
        ptime timeStart(microsec_clock::local_time());

        double  timeElapsed=0;
        unsigned int  nPoll=0;

        //ca_flush_io already done - no need to do it again!

        channelRequestStatusPut = (*it_handle).getChannelRequestStatusPut();
        status= channelRequestStatusPut.getMessageStatus(); //Message==Request

        while (channelRequestStatusPut.getCallbackProgressKind() == CAFENUM::PENDING
                && timeElapsed < _timeout)
        {

            ++nPoll;

            ptime timeEnd(microsec_clock::local_time());
            time_duration duration(timeEnd-timeStart);
            timeElapsed= (double) duration.total_microseconds()/1000000.0;


            channelRequestStatusPut = (*it_handle).getChannelRequestStatusPut();
        }


        if (timeElapsed >= _timeout )
        {
            std::cout <<  __METHOD__ <<  __LINE__ << " TimeElapsed "  << timeElapsed <<   "  timeout_pend_io  " << _timeout<< std::endl;
        }

        if (channelRequestStatusPut.getCallbackProgressKind() != CAFENUM::PENDING)
        {
            return ICAFE_NORMAL; //status
        }
        else
        {
            return ECAFE_TIMEOUT;
        };
    }
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  _handle << endl;
        return ECAFE_INVALID_HANDLE;
    }


#undef __METHOD__
}





/**
 *  \brief isPutCallbackDone performs a ca_poll() and queries the Conduit data container
 *  to determine if the putWithCallback has been completed
 *  \param   _handle input: handle
 *  \return  bool true or false
 */
bool Granules::isPutCallbackDone(const unsigned int  _handle)
{
#define __METHOD__  "Granules::isPutCallbackDone(_handle)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        if ((*it_handle).getChannelRequestStatusPut().getCallbackProgressKind() == CAFENUM::PENDING)
        {

            ca_poll();   // just in case a flush was forgotten

        }
        if ( (*it_handle).getChannelRequestStatusPut().getCallbackProgressKind() == CAFENUM::PENDING)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << " Handle " << _handle << " does not exist! " << endl;
        cout << " Function for Handle " << _handle << " will return true to keep things going! " << endl;
    }

    return true;

#undef __METHOD__
};

//////////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief channelVerifyGetCtrl: preVerification of client's requested data
 *  \param _handle input: handle
 *  \param _dbrType input: chtype
 *  \return ICAFE_NORMAL or ICAFE_CA_OP_CONN_DOWN or ECAFE_NORDACCESS
 */
int  Granules::channelVerifyGetCtrl(const unsigned int  _handle, chtype _dbrType)
{
#define	__METHOD__  "Granules::channelVerifyGetCtrl"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {



        if ( (*it_handle).getChannelRegalia().getCafeConnectionState()==ICAFE_CS_NEVER_CONN)
        {
            return ICAFE_CS_NEVER_CONN;
        }
        else if ( (*it_handle).getChannelRegalia().getCafeConnectionState()==ICAFE_CS_CLOSED)
        {
            return ICAFE_CS_CLOSED;
        }



        if ( CHECK_CONSISTENCY_CA_STATE && !(*it_handle).isConnected())
        {
            status=helper.checkConsistency(_handle);
            /*
            if (status!=ICAFE_NORMAL)
            {
                return status;
            }
            */
        }


        //isConnected
        if ( !(*it_handle).isConnected())
        {

            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            channelRequestStatusGetCtrl  = (*it_handle).getChannelRequestStatusGetCtrl();
            channelRequestStatusGetCtrl.setPreRequestStatus (ICAFE_CA_OP_CONN_DOWN);

            handle_index.modify(it_handle, change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));

            handle_index.modify(it_handle, change_status(ICAFE_CA_OP_CONN_DOWN)); //for return
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
            return ICAFE_CA_OP_CONN_DOWN;
        }

        //hasReadAccess
        if ((*it_handle).getAccessRead() == 0)
        {
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            channelRequestStatusGetCtrl  = (*it_handle).getChannelRequestStatusGetCtrl();
            channelRequestStatusGetCtrl.setPreRequestStatus (ECAFE_NORDACCESS);


            handle_index.modify(it_handle, change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));

            handle_index.modify(it_handle, change_status(ECAFE_NORDACCESS)); //for return
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
            return ECAFE_NORDACCESS;
        }

        //First steps OK
        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        channelRequestStatusGetCtrl  = (*it_handle).getChannelRequestStatusGetCtrl();
        channelRequestStatusGetCtrl.setPreRequestStatus (ICAFE_NORMAL);

        handle_index.modify(it_handle, change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }

        channelRequestMetaCtrlClient = (*it_handle).getChannelRequestMetaCtrlClient();  // 2

        //2
        chtype clientT = _dbrType%(LAST_TYPE+1);
        channelRequestMetaCtrlClient.setDataType   (clientT);
        channelRequestMetaCtrlClient.setDbrDataType(_dbrType);



        //This will always be DBR_CTRL
        channelRequestMetaCtrlClient.setCafeDbrType(
            //                       (CAFENUM::DBR_TYPE) (*it_handle).convertToDbrTypeClass(_dbrType) );
            (CAFENUM::DBR_TYPE) helper.convertToCAFEDbrTypeClass(_dbrType) );



        //All ok so far

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
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  _handle << endl;
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief channelPrepareGetCtrl: prepare client's requested data for ca call
 *  \param _handle input: handle
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE;
 */
int  Granules::channelPrepareGetCtrl(const unsigned int  _handle)
{
#define	__METHOD__  "Granules::channelPrepareGetCtrl"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {


        channelRequestMetaCtrlClient  = (*it_handle).getChannelRequestMetaCtrlClient();   //
        channelRequestMetaCtrl        = (*it_handle).getChannelRequestMetaCtrl();         //
        channelRequestDataTypePolicy  = (*it_handle).getChannelRequestDataTypePolicy();   //

        bool changeChannelRequestMetaCtrl=false;

        chtype clientRequestType       = channelRequestMetaCtrl.getDataType();
        CAFENUM::DBR_TYPE  cafeDbrType = channelRequestMetaCtrl.getCafeDbrType();

        // Do we transfer data with the smaller sized type?
        if (channelRequestDataTypePolicy.getRequestKind()==CAFENUM::LOWEST_DATATYPE)
        {

            clientRequestType = convertMatrix((*it_handle).getChannelRegalia().getDataType(),
                                              channelRequestMetaCtrlClient.getDataType());
            channelRequestMetaCtrl.setDataType(clientRequestType);
            changeChannelRequestMetaCtrl=true;

        }

        if (channelRequestMetaCtrlClient.getCafeDbrType() > channelRequestMetaCtrl.getCafeDbrType())
        {
            cafeDbrType = channelRequestMetaCtrlClient.getCafeDbrType();
            channelRequestMetaCtrl.setCafeDbrType(cafeDbrType);
            changeChannelRequestMetaCtrl=true;
        }



        if (changeChannelRequestMetaCtrl)
        {
            //set dbrType according to cafeDbrType;
            switch (cafeDbrType)
            {
            case CAFENUM::DBR_CTRL:
                channelRequestMetaCtrl.setDbrDataType(dbf_type_to_DBR_CTRL(clientRequestType));
                break;
            case CAFENUM::DBR_GR:
                channelRequestMetaCtrl.setDbrDataType(dbf_type_to_DBR_GR (clientRequestType));
                break;
            default:
                channelRequestMetaCtrl.setDbrDataType(dbf_type_to_DBR_CTRL(clientRequestType));
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "Method does not support this DBR_TYPE: "
                     << dbr_type_to_text(channelRequestMetaData.getCafeDbrType()) << endl;//OK
                cout << "This line should never appear!" << endl;
                break;
            }
        }


        //Check the number of elements requested?
        if ((*it_handle).getChannelRegalia().getNelem()>1)
        {

            //What did the client request? What is native type? Transfer minimum of this.
            if (channelRequestMetaCtrl.getNelem() !=  channelRequestMetaCtrlClient.getNelem())
            {


                unsigned int  nelem_new = min(MAX_NELEM_FOR_CTRL_BUFFER,
                                              min((*it_handle).getChannelRegalia().getNelem(),
                                                  channelRequestMetaCtrlClient.getNelem()));



                if (nelem_new != channelRequestMetaCtrl.getNelem() )
                {
                    channelRequestMetaCtrl.setNelem( nelem_new);
                    changeChannelRequestMetaCtrl=true;
                }

            }
        }

        if (changeChannelRequestMetaCtrl==true)
        {
            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            handle_index.modify(it_handle,
                                change_channelRequestMetaCtrl(channelRequestMetaCtrl));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
        };

    }
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  _handle << endl;
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}


/**
 *  \brief channelExecuteGetCtrl: Executes ca_get method
 *  \param _handle input: handle
 *  \return ICAFE_NORMAL
 */
int  Granules::channelExecuteGetCtrl(const unsigned int  _handle)
{
#define	__METHOD__  "Granules::channelExecuteGetCtrl"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {


        channelTimeoutPolicyGet      = (*it_handle).getChannelTimeoutPolicyGet(); //

        channelRequestPolicyGetCtrl  = (*it_handle).getChannelRequestPolicyGetCtrl();  // 1

        switch (channelRequestPolicyGetCtrl.getMethodKind())
        {
        case WITH_CALLBACK_DEFAULT:
        case WITH_CALLBACK_USER_SUPPLIED:

            if (channelRequestPolicyGetCtrl.getMethodKind()==WITH_CALLBACK_DEFAULT)
            {
                status=((*it_handle)).getCtrlWithCallback(CALLBACK_CAFE::handlerGetCtrl);//CALLBACK_CAFE::callbackHandlerGetCtrl);
            }
            else   //WITH_CALLBACK_USER_SUPPLIED:
            {
                // If it is then use default callback!
                if (channelRequestPolicyGetCtrl.getHandler() != NULL)
                {
                    status=((*it_handle)).getCtrlWithCallback(channelRequestPolicyGetCtrl.getHandler());
                }
                else
                {
                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << "NO CALLBACK FUNCTION FOR GET SUPPLIED." << std::endl;
                    std::cout << "USING DEFAULT CALLBACK_CAFE::handlerGetCtrl " << std::endl;
                    status=((*it_handle)).getCtrlWithCallback(CALLBACK_CAFE::handlerGetCtrl);
                }
            }



            if (status != ECA_NORMAL)
            {
                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                channelRequestStatusGetCtrl   = (*it_handle).getChannelRequestStatusGetCtrl();  //
                channelRequestStatusGetCtrl.setCallbackKind(false, false);      // NOT_INITIATED
                channelRequestStatusGetCtrl.setRequestStatus (status);
                handle_index.modify(it_handle, change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));
                handle_index.modify(it_handle, change_status(status));
                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }
                return status;
            }


            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            channelRequestStatusGetCtrl   = (*it_handle).getChannelRequestStatusGetCtrl();  //
            channelRequestStatusGetCtrl.setCallbackKind(true, false);        //PENDING
            channelRequestStatusGetCtrl.setRequestStatus (status);

            handle_index.modify(it_handle, change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));

            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

            ca_flush_io();

            status=waitForGetCtrlEvent(_handle, channelTimeoutPolicyGet.getTimeout() );

            if (status==ECAFE_TIMEOUT && channelTimeoutPolicyGet.getSelfGoverningTimeout()
                    && (*it_handle).isConnected() )
            {
                unsigned short ntries=0;
                while (status==ECAFE_TIMEOUT && ntries<channelTimeoutPolicyGet.getNtries()
                        && (*it_handle).isConnected() )
                {
                    status=waitForGetCtrlEvent(_handle, channelTimeoutPolicyGet.getTimeout() +
                                               channelTimeoutPolicyGet.getDeltaTimeout()*(++ntries));
                }

                if ((*it_handle).isConnected())
                {

                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << "No of waitForGetEvent tries=" << ntries << std::endl;
                    if (status==ECAFE_TIMEOUT)
                    {
                        std::cout << "is the MAXIMUM allowed as configured through TimeoutPolicy! "  << std::endl;
                        std::cout << "TURNING OFF SELF-GOVERNING TIMEOUT FOR GET OPERATIONS FOR THIS CHANNEL"  << std::endl;
                        channelTimeoutPolicyGet.setSelfGoverningTimeout(false);
                    }
                    else
                    {
                        std::cout <<  "Changing timeout for handle/pv "
                                  << _handle << "/" << (*it_handle).getPV() << " to: "  <<
                                  (channelTimeoutPolicyGet.getTimeout() +
                                   channelTimeoutPolicyGet.getDeltaTimeout()*ntries) << endl;

                        //modify timeout for handle
                        channelTimeoutPolicyGet.setTimeout( (channelTimeoutPolicyGet.getTimeout() +
                                                             channelTimeoutPolicyGet.getDeltaTimeout()*ntries));
                    }

                    if(MUTEX)
                    {
                        cafeMutex.lock();   //lock
                    }
                    handle_index.modify(it_handle, change_channelTimeoutPolicyGet(channelTimeoutPolicyGet));
                    if(MUTEX)
                    {
                        cafeMutex.unlock();   //unlock
                    }

                    if (status==ECAFE_TIMEOUT)
                    {
                        std::cout << "CURRENT STATUS OF HANDLE: " << std::endl;
                        helper.printHandle(_handle);
                    }

                }//if connected
            } //if


            if (!(*it_handle).isConnected() || status != ICAFE_NORMAL)
            {
                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                channelRequestStatusGetCtrl = (*it_handle).getChannelRequestStatusGetCtrl();
                channelRequestStatusGetCtrl.setCallbackStatus    (status);
                channelRequestStatusGetCtrl.setCallbackKind(false, false); // NOT_INITIATED NOT_TRIGGERED

                handle_index.modify(it_handle,
                                    change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));

                if (!(*it_handle).isConnected())
                {
                    handle_index.modify(it_handle, change_status(ICAFE_CS_DISCONN));
                    if(MUTEX)
                    {
                        cafeMutex.unlock();   //unlock
                    }
                    return ICAFE_CS_DISCONN;
                }
                else
                {
                    handle_index.modify(it_handle, change_status(status));
                    if(MUTEX)
                    {
                        cafeMutex.unlock();   //unlock
                    }
                }
            }
            break;

        case WITHOUT_CALLBACK:
        default:

            status=(*it_handle).getCtrl();

            if (status != ECA_NORMAL)
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
                return status;
            }

            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            channelRequestStatusGetCtrl   = (*it_handle).getChannelRequestStatusGetCtrl();  //
            channelRequestStatusGetCtrl.setRequestStatus    (status);
            handle_index.modify(it_handle, change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));

            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }


            status=ca_pend_io(channelTimeoutPolicyGet.getTimeout());

            if (status==ECA_TIMEOUT && channelTimeoutPolicyGet.getSelfGoverningTimeout())
            {
                unsigned short ntries=0;
                while (status==ECA_TIMEOUT && ntries<channelTimeoutPolicyGet.getNtries())
                {

                    status=(*it_handle).getCtrl();
                    if (status != ECA_NORMAL)
                    {
                        if(MUTEX)
                        {
                            cafeMutex.lock();   //lock
                        }
                        channelRequestStatusGetCtrl   = (*it_handle).getChannelRequestStatusGetCtrl();  //
                        channelRequestStatusGetCtrl.setRequestStatus(status);

                        handle_index.modify(it_handle,
                                            change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));

                        handle_index.modify(it_handle, change_status(status));
                        if(MUTEX)
                        {
                            cafeMutex.unlock();   //unlock
                        }
                        return status;
                    }

                    status=ca_pend_io(channelTimeoutPolicyGet.getTimeout() +
                                      channelTimeoutPolicyGet.getDeltaTimeout()*(++ntries));
                }




                std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                std::cout << "No of ca_pend_io tries=" << ntries << std::endl;
                if (status==ECA_TIMEOUT)
                {
                    std::cout << "is the MAXIMUM allowed as configured through TimeoutPolicy! "  << std::endl;
                    std::cout << "TURNING OFF SELF-GOVERNING TIMEOUT FOR GET OPERATIONS FOR THIS CHANNEL"  << std::endl;
                    channelTimeoutPolicyGet.setSelfGoverningTimeout(false);
                    channelTimeoutPolicyGet.setTimeout( channelTimeoutPolicyGet.getTimeout() );
                    std::cout << "AND TIMEOUT RESTORED TO START VALUE OF " << channelTimeoutPolicyGet.getTimeout() << endl;
                }
                else
                {
                    std::cout <<  "Changing timeout for handle/pv "
                              << _handle << "/" << (*it_handle).getPV() << " to: "  <<
                              (channelTimeoutPolicyGet.getTimeout() +
                               channelTimeoutPolicyGet.getDeltaTimeout()*ntries) << endl;


                    //modify timeout for handle
                    channelTimeoutPolicyGet.setTimeout( (channelTimeoutPolicyGet.getTimeout() +
                                                         channelTimeoutPolicyGet.getDeltaTimeout()*ntries));
                }
                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                handle_index.modify(it_handle, change_channelTimeoutPolicyGet(channelTimeoutPolicyGet));
                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }

                if (status==ECA_TIMEOUT)
                {
                    std::cout << "CURRENT STATUS OF HANDLE: " << std::endl;
                    helper.printHandle(_handle);
                }
            }

            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            channelRequestStatusGetCtrl   = (*it_handle).getChannelRequestStatusGetCtrl();  //

            channelRequestStatusGetCtrl.setPendStatus    (status);
            handle_index.modify(it_handle,
                                change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

            break;
        }




        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        //Important to place this after getCtrl has completed
        channelRequestMetaCtrl = (*it_handle).getChannelRequestMetaCtrl();
        channelRequestMetaCtrl.setOffset(channelRequestMetaCtrlClient.getOffset());
        handle_index.modify(it_handle, change_channelRequestMetaCtrl(channelRequestMetaCtrl));

        handle_index.modify(it_handle, change_status(status));
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }

    }
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  _handle << endl;
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}




/**
 *  \brief channelExecuteGetCtrl: Executes ca_get method
 *  \param _handle input: handle
 *  \return ICAFE_NORMAL
 */
int  Granules::channelExecuteGetCtrlNoWait(const unsigned int  _handle)
{
#define	__METHOD__  "Granules::channelExecuteGetCtrlNoWait"

    status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        channelRequestPolicyGetCtrl   = (*it_handle).getChannelRequestPolicyGetCtrl();  // 1

        if ((*it_handle).getChannelRequestStatusGetCtrl().getCallbackProgressKind() != CAFENUM::PENDING)
        {

            switch (channelRequestPolicyGetCtrl.getMethodKind())
            {
            case WITHOUT_CALLBACK:
            case WITH_CALLBACK_DEFAULT:
            case WITH_CALLBACK_USER_SUPPLIED:

                if (channelRequestPolicyGetCtrl.getMethodKind()==WITH_CALLBACK_DEFAULT ||
                        channelRequestPolicyGetCtrl.getMethodKind()==WITHOUT_CALLBACK )
                {
                    status=((*it_handle)).getCtrlWithCallback(CALLBACK_CAFE::handlerGetCtrl);//CALLBACK_CAFE::callbackHandlerGetCtrl);
                }
                else   //WITH_CALLBACK_USER_SUPPLIED:
                {
                    // If it is then use default callback!
                    if (channelRequestPolicyGetCtrl.getHandler() != NULL)
                    {
                        status=((*it_handle)).getCtrlWithCallback(channelRequestPolicyGetCtrl.getHandler());
                    }
                    else
                    {
                        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                        std::cout << "NO CALLBACK FUNCTION FOR GET SUPPLIED." << std::endl;
                        std::cout << "USING DEFAULT CALLBACK_CAFE::handlerGetCtrl " << std::endl;
                        status=((*it_handle)).getCtrlWithCallback(CALLBACK_CAFE::handlerGetCtrl);
                    }
                }

                //channelRequestStatusGetCtrl.setRequestStatus (status);


                if (status != ECA_NORMAL)
                {
                    if(MUTEX)
                    {
                        cafeMutex.lock();   //lock
                    }
                    channelRequestStatusGetCtrl   = (*it_handle).getChannelRequestStatusGetCtrl();  //
                    channelRequestStatusGetCtrl.setCallbackKind(false, false);      // NOT_INITIATED
                    channelRequestStatusGetCtrl.setRequestStatus (status);
                    handle_index.modify(it_handle, change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));
                    handle_index.modify(it_handle, change_status(status));
                    if(MUTEX)
                    {
                        cafeMutex.unlock();   //unlock
                    }
                    return status;
                }


                if(MUTEX)
                {
                    cafeMutex.lock();   //lock
                }
                channelRequestStatusGetCtrl   = (*it_handle).getChannelRequestStatusGetCtrl();  //
                channelRequestStatusGetCtrl.setCallbackKind(true, false);        //PENDING
                channelRequestStatusGetCtrl.setRequestStatus (status);

                handle_index.modify(it_handle, change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));

                if(MUTEX)
                {
                    cafeMutex.unlock();   //unlock
                }

                break;

            } //switch


        }
        else
        {
            status= ICAFE_WAITING_FOR_PREV_CALLBACK;

            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(status);

        } //if pending


    }
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  _handle << endl;
        return ECAFE_INVALID_HANDLE;
    }



    if(MUTEX)
    {
        cafeMutex.lock();   //lock
    }
    channelRequestMetaCtrl = (*it_handle).getChannelRequestMetaCtrl();
    channelRequestMetaCtrl.setOffset(channelRequestMetaCtrlClient.getOffset());
    handle_index.modify(it_handle, change_channelRequestMetaCtrl(channelRequestMetaCtrl));
    handle_index.modify(it_handle, change_status(status));
    if(MUTEX)
    {
        cafeMutex.unlock();   //unlock
    }

    return status; //Could still give a timeout depending on policy or ICAFE_NORMAL;
#undef __METHOD__
}


/**
 *  \brief waitForGetCtrlEvent: waits for get callback to complete
 *  \param _handle input: handle
 *  \param _timeout input: wait time before method timeouts
 *  \return ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  Granules::waitForGetCtrlEvent(const unsigned int  _handle, double _timeout)
{
#define	__METHOD__  "Granules::waitForGetEvent"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        using namespace boost::posix_time;
        ptime timeStart(microsec_clock::local_time());

        double  timeElapsed=0;
        unsigned int  nPoll=0;


        channelRequestStatusGetCtrl = (*it_handle).getChannelRequestStatusGetCtrl();
        status= channelRequestStatusGetCtrl.getMessageStatus(); //Message==Request

        while (channelRequestStatusGetCtrl.getCallbackProgressKind() == PENDING
                && timeElapsed < _timeout)
        {
            //usleep(20);
            ++nPoll;

            ptime timeEnd(microsec_clock::local_time());
            time_duration duration(timeEnd-timeStart);
            timeElapsed= (double) duration.total_microseconds()/1000000.0;

            channelRequestStatusGetCtrl = (*it_handle).getChannelRequestStatusGetCtrl();

        }


        if (channelRequestStatusGetCtrl.getCallbackProgressKind() != CAFENUM::PENDING)
        {

            return ICAFE_NORMAL; //or status
        }
        else
        {
            return ECAFE_TIMEOUT;
        };
    }
    else
    {
        //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        //cout <<  "Internal CAFE ERROR!  Invalid handle : " <<  _handle << endl;
        return ECAFE_INVALID_HANDLE;
    }

#undef __METHOD__
}


/**
 *  \brief isGetXtrlCallbackDone performs a ca_poll() and queries the Conduit data container
 *  to determine if the getCtrlWithCallback has been completed
 *  \param   _handle input: handle
 *  \return  bool true or false
 */
bool Granules::isGetCtrlCallbackDone(const unsigned int  _handle)
{
#define __METHOD__  "CAFEGranule::isGetCtrlCallbackDone(_handle)"


    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        if ((*it_handle).getChannelRequestStatusGetCtrl().getCallbackProgressKind() == CAFENUM::PENDING)
        {

            ca_poll();   // just in case a flush was forgotten

        }
        if ( (*it_handle).getChannelRequestStatusGetCtrl().getCallbackProgressKind() == CAFENUM::PENDING)
        {
            return false;
        }
        else
        {
            return true;
        }

    }

    cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
    cout << " Handle " << _handle << " does not exist! " << endl;
    cout << " Function for Handle " << _handle << " will return true to keep things going! " << endl;

    return true;

#undef __METHOD__
};


/**
 *  \brief convertMatrix determines datatype for message transmission.
 *  The datatype will be the smaller of the native datatype and that requested by client.
 *  \param   nativeType input: native datatype
 *  \param   clientT input: datatype is that requested by client
 *  \return  clientRequestType is datatype for message transmission
 */
chtype Granules::convertMatrix(const chtype nativeType, const chtype clientT)
{
#define __METHOD__ "Granules::convertMatrix(const CAFEConduit cc, chtype clientT)"

    chtype clientRequestType=clientT;


    switch (nativeType)
    {
    case DBR_STRING:
        //convert to smaller type
        break;

    case DBR_DOUBLE:
        //convert is larger than a double
        if ( clientT == DBR_STRING )
        {
            clientRequestType=DBR_DOUBLE;
        }
        break;
    case DBR_FLOAT:
        switch(clientT)
        {
        case DBR_STRING:
        case DBR_DOUBLE:
            clientRequestType=DBR_FLOAT;
            break;
        default:
            break;
        }
        break;
    case DBR_LONG:
        switch( clientT)
        {
        case DBR_STRING:
        case DBR_DOUBLE:
            clientRequestType=DBR_LONG;
            break;
        default:
            break;
        }
        break;
    case DBR_SHORT:
        switch( clientT)
        {
        case DBR_STRING:
        case DBR_DOUBLE:
        case DBR_FLOAT:
        case DBR_LONG:
            clientRequestType=DBR_SHORT;
            break;
        default:
            break;
        }
        break;
    case DBR_ENUM:
        switch( clientT)
        {
        case DBR_STRING:
        case DBR_DOUBLE:
        case DBR_FLOAT:
        case DBR_LONG:
            clientRequestType=DBR_ENUM;
            break;
        default:
            break;
        }
        break;
    case DBR_CHAR:
        clientRequestType=DBR_CHAR;
        break;
    default:
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "CAFE INTERNAL ERROR. Datatype is not an option for this switch case: "
             << dbr_type_to_text(nativeType) << endl;
        break;
    }

    return clientRequestType;
#undef __METHOD__
}
