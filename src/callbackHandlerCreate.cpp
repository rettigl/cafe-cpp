///
/// \file    callbackHandlerCreate.cc
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#include <cadef.h>
#include <global.h>
#include <policies.h>
#include <conduitFriends.h>
#include <conduitConnectionHandlerArgs.h>
//include <methodCallbacks.h>
#include <granules.h>
#include <handleHelper.h>


using namespace std;

/**
 *   \brief Callback function for ca_create_channel.
 *   Modifies Conduit object accordingly
 *   \param args returns connection handler parameters \n
 *   i.e. args.chid, args.op{CA_OP_CONN_UP, CA_OP_CONN_DOWN}
 */
void ChannelCreatePolicy::callbackHandlerCreate(struct connection_handler_args args) {
#define __METHOD__ "ChannelCreatePolicy::callbackHandlerCreate"

    //Use for debugging
	  //cout << __METHOD__ <<  endl;
	  
	
    unsigned int  _handle = (unsigned long) ca_puser(args.chid);

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find((unsigned int)_handle);

    if (it_handle != handle_index.end()) {
            //cout << (*it_handle).pv << endl; 
	        	//cout << "handle= " << (*it_handle).getHandle() << endl; cout << endl;

            //Modifies corresponding local variables
            if(MUTEX){cafeMutex.lock();}
            handle_index.modify(it_handle, change_connectionHandlerArgs(args));
            if(MUTEX){cafeMutex.unlock();}

            if (args.op == CA_OP_CONN_UP) {
                chtype buffer_TIME_Type      = dbf_type_to_DBR_TIME(ca_field_type(args.chid)); //DBF_STRING
                chtype buffer_CTRL_Type      = dbf_type_to_DBR_CTRL(ca_field_type(args.chid));
                chtype buffer_PRIMITIVE_TYPE = dbf_type_to_DBR     (ca_field_type(args.chid));

                //BUFFER FOR GET  has DBR_TIME size
                //BUFFER FOR PUT  has DBR      size
                //BUFFER FOR CTRL has DBR_CTRL size

                if(MUTEX){cafeMutex.lock();}
                handle_index.modify(it_handle, change_dataBufferSize_PRIMITIVE(buffer_PRIMITIVE_TYPE));
                handle_index.modify(it_handle, change_dataBufferSize_CTRL     (buffer_CTRL_Type));
                handle_index.modify(it_handle, change_dataBufferSize_TIME     (buffer_TIME_Type));
                handle_index.modify(it_handle, change_dataBufferSize_STSACK   ()); //fixed size DBR_STSACK_STRING

                if ( (*it_handle).getAccessRead() != ca_read_access(args.chid) ) {
                    //cout << " This will only occur if CAFE's access right handler is not used " << endl;
                    //cout << " accessRightsEventHandler not called BEFORE channelEventHandler! " << endl;
                    //cout << " FORCING MODIFICATION IN HASH TABLE FOR READ ACCESS=" << ca_read_access(args.chid) << endl;
                    handle_index.modify(it_handle, change_accessRead(ca_read_access(args.chid)));
                }
                if ( (*it_handle).getAccessWrite() != ca_write_access(args.chid) ) {
                    //cout << " This will only occur if CAFE's access right handler is not used " << endl;
                    //cout << " accessRightsEventHandler not called BEFORE channelEventHandler! "  << endl;
                    //cout << " FORCING MODIFICATION IN HASH TABLE FOR WRITE ACCESS=" << ca_write_access(args.chid) << endl;
                    handle_index.modify(it_handle, change_accessWrite(ca_write_access(args.chid)));
                }
                if(MUTEX){cafeMutex.unlock();}

                //Set prerequestStatus
                ChannelRequestStatus channelRequestStatusGet; int  status;

                //Call  callback methods for all types!!
                status=(*it_handle).getCtrlWithCallback(CALLBACK_CAFE::handlerGetCtrl);
                channelRequestStatusGet = (*it_handle).getChannelRequestStatusGetCtrl();
                channelRequestStatusGet.setRequestStatus (status);
                if (status != ECA_NORMAL) {
                    channelRequestStatusGet.setCallbackKind(false, false);      // NOT_INITIATED NOT_TRIGGERED
                    if(MUTEX){cafeMutex.lock();}
                    handle_index.modify(it_handle, change_status(status));
                    if(MUTEX){cafeMutex.unlock();}
                    cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                    cout << "getCtrlWithCallback(CALLBACK_CAFE::handlerGetCtrl) reported error:" << endl;
                    CAFEStatus cafeStatus; cafeStatus.report(status);
                }
                else {
                    channelRequestStatusGet.setCallbackKind(true, false);        //PENDING NOT_TRIGGERED
                }

                if(MUTEX){cafeMutex.lock();}
                handle_index.modify(it_handle, change_channelRequestStatusGetCtrl(channelRequestStatusGet));
                if(MUTEX){cafeMutex.unlock();}
                //ca_flush_io();

                status=(*it_handle).getSTSACKWithCallback(CALLBACK_CAFE::handlerGetSTSACK);
							
                channelRequestStatusGet = (*it_handle).getChannelRequestStatusGetSTSACK();
                channelRequestStatusGet.setRequestStatus (status);
                if (status != ECA_NORMAL) {
                    channelRequestStatusGet.setCallbackKind(false, false);      // NOT_INITIATED NOT_TRIGGERED
                    if(MUTEX){cafeMutex.lock();}
                    handle_index.modify(it_handle, change_status(status));
                    if(MUTEX){cafeMutex.unlock();}
                    cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                    cout << "getSTSACKWithCallback(CALLBACK_CAFE::handlerGetSTSACK) reported error:" << endl;
                    CAFEStatus cafeStatus; cafeStatus.report(status);
                }
                else {
                    channelRequestStatusGet.setCallbackKind(true, false);        //PENDING NOT_TRIGGERED                    
                }

                if(MUTEX){cafeMutex.lock();}
                handle_index.modify(it_handle, change_channelRequestStatusGetSTSACK(channelRequestStatusGet));
                if(MUTEX){cafeMutex.unlock();}
                //ca_flush_io();

                status=(*it_handle).getClassNameWithCallback(CALLBACK_CAFE::handlerGetClassName);

                if ( status != ECA_NORMAL)  {
                    cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                    cout << "getClassNameWithCallback(CALLBACK_CAFE::handlerGetClassName) reported error:" << endl;
                    CAFEStatus cafeStatus; cafeStatus.report(status);
                }

                status=(*it_handle).getWithCallback(CALLBACK_CAFE::handlerGet);
                channelRequestStatusGet = (*it_handle).getChannelRequestStatusGet();
                channelRequestStatusGet.setRequestStatus (status);
                if (status != ECA_NORMAL) {
                    channelRequestStatusGet.setCallbackKind(false, false);      // NOT_INITIATED NOT_TRIGGERED
                    if(MUTEX){cafeMutex.lock();}
                    handle_index.modify(it_handle, change_status(status));
                    if(MUTEX){cafeMutex.unlock();}
                    cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                    cout << "getWithCallback(CALLBACK_CAFE::handlerGet) reported error:" << endl;
                    CAFEStatus cafeStatus; cafeStatus.report(status);
                }
                else {
                    channelRequestStatusGet.setCallbackKind(true, false);        //PENDING NOT_TRIGGERED
                }

                if(MUTEX){cafeMutex.lock();}
                handle_index.modify(it_handle, change_channelRequestStatusGet(channelRequestStatusGet));
                if(MUTEX){cafeMutex.unlock();}
                //ca_flush_io();

                // Check if any monitors for this channel are to be started.....
                // If monitors are to be started, then start the monitors
                // pass on handler functions
                //
                // Loop thru monitor waiting list

                // monitor in waiting

                vector<MonitorPolicy> mpInWaitingV =(*it_handle).getMonitorPolicyInWaitingVector();
                vector<MonitorPolicy>::iterator it;

                //Iterate
                for (it = mpInWaitingV.begin(); it != mpInWaitingV.end(); ++it) {

                    //Check start values;
                    if ((*it).getNelem()==0) {
                        (*it).setNelem((*it_handle).getChannelRegalia().getNelem());
                    }

                    //Check start values;
                    if ((*it).getDataType()==CAFE_NOT_REQUESTED) {
                        (*it).setDataType((*it_handle).getChannelRegalia().getDataType());
                    }

					//Check setMask if MASK_CTRL!

                    status=(*it_handle).monitorStart((*it));
                    (*it).setStatus(status);

                    //Add to vector
                    //check in to vector<monitorMap>

                    if(MUTEX){cafeMutex.lock();}
                    handle_index.modify(it_handle, change_monitorPolicyInsert((*it)));
                    if(MUTEX){cafeMutex.unlock();}
                }
                //loop

                for (it = mpInWaitingV.begin(); it != mpInWaitingV.end(); ++it) {

                    unsigned int  ID=(*it).getID();
                    if(MUTEX){cafeMutex.lock();}
                    handle_index.modify(it_handle, change_monitorPolicyInWaitingErase(ID));
                    if(MUTEX){cafeMutex.unlock();}
                }
                //loop


            } else {

                // DIS-CONNECTION (i.e. from a previously connected state)
                // Setting Access Rights to boolean false. These will in any
                // case be overwritten to false by the access_rights_event_handler
                /*
                if ( (*it_handle).getAccessRead() != 0 ) {
                    handle_index.modify(it_handle, change_accessRead(0));
                }

                if ( (*it_handle).getAccessWrite() != 0 ) {
                    handle_index.modify(it_handle, change_accessWrite(0));
                }
                */
								// On disconnection invoke PyCafe Monitor callback
											
								
											
								#if HAVE_PYTHON_H
                // monitors
	           
						 	
						 
                vector<MonitorPolicy> mpV =(*it_handle).getMonitorPolicyVector();
                vector<MonitorPolicy>::iterator itmp;
								
							
																											
                //Iterate
                for (itmp = mpV.begin(); itmp != mpV.end(); ++itmp) {		
								 							    
										if(MUTEX){cafeMutex.lock();}
                    handle_index.modify(it_handle, change_usrArgs(  (unsigned long) (*itmp).getMonitorID() ));
                    if(MUTEX){cafeMutex.unlock();}
										 //Add datatypes etc... 
								     //cout << "(*it_handle).PyEventHandler();  " << (*itmp).getMonitorID()    << endl;								
										(*it_handle).PyEventHandler((*itmp).getMonitorID() );	
								}
								#endif
							
            }
	

	
        }
        else {

            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
                    << " called with an invalid CAFE handle:" << _handle << endl;         
            cout << "Actually this should not happen and this line should never appear!! " << endl;

        }


        return;
#undef __METHOD__
}


