//
//  callbacks.h
//


void callbackHandlerMonitor( struct event_handler_args args)  {
#define __METHOD__  "callbackHandlerMonitor"

    if (args.status !=ECA_NORMAL) {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << endl;
        return;
    }

    unsigned int _handle = (unsigned long) ca_puser(args.chid);// args.usr; // ca_puser(args.chid);

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find((unsigned int)_handle);

    if (it_handle != handle_index.end()) {
        //cout << (*it_handle).getPV() << " " << (*it_handle).getHandle() << endl;
        if(MUTEX){cafeMutex.lock();}
        handle_index.modify(it_handle, change_eventHandlerArgs   (args));
        if(MUTEX){cafeMutex.unlock();}


				//**** ADD USER CODE HERE ***//
		
         if (args.type < DBR_GR_STRING) {
            PVDataHolder pvd(args.count);
            (*it_handle).getPVDataHolder(pvd);
            //pvd.print();
            //cout << "val/D//= " << pvd.getAsString(0) << endl;

         }
         else if (args.type < DBR_PUT_ACKT) {
            PVCtrlHolder pvc(args.count);
            (*it_handle).getPVCtrlHolder(pvc);
            //pvc.print();
            //cout << "val/C/= " << pvc.getAsString(0) << endl;
         }
				 
				  			
			  //cout <<  "args.usr = " << (unsigned int) args.usr << endl;
				//cout <<  "getUsrArgs() = " << (unsigned int) (*it_handle).getUsrArgs() << endl;
				 
				 
				/* 
				vector<MonitorPolicy> mpV = (*it_handle).getMonitorPolicyVector();
										
				for (int i=0; i<mpV.size(); ++i) {				
			  	cout << " i " << i << endl;
				  cout <<  mpV[i].getUserArgs() << endl;
					mpV[i].print();
				}
				*/
				//****  END USER CODE ***// 
		
    }
    else {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
    }

return;
#undef __METHOD__
};


/**
 *  Callback function for when putWithCallback method is invoked
 *  \param args input: event handler_args structure
 */
void handlerPut( struct event_handler_args args)  {
#define __METHOD__  "handlerPut"

    if (args.status !=ECA_NORMAL) {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << endl;
        return;
    }

    unsigned int _handle = (unsigned long) args.usr;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find((unsigned int) _handle);

        if (it_handle != handle_index.end())  {

            if(MUTEX)cafeMutex.lock();

            ChannelRequestStatus  channelRequestStatusPut=(*it_handle).getChannelRequestStatusPut();

            channelRequestStatusPut.setCallbackKind(false, true);

            handle_index.modify(it_handle, change_channelRequestStatusPut(channelRequestStatusPut));
						
						//cout << __METHOD__ << " WITH_CALLBACK_USER_SUPLLIED for PUT done " << (*it_handle).getChannelRequestStatusPut().getCallbackProgressKind() << endl;

            if(MUTEX)cafeMutex.unlock();
				
				  	//**** ADD USER CODE HERE ***//
				
						
        }
        else {
       		 cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        	 cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
        }

				
				

return;
#undef __METHOD__
};



/**
 *  Callback function for getCallback method is invoked
 *  with a pointer to the retrieved value
 *  \param args input: event handler arguments
 */
void handlerGet( struct event_handler_args args)  {
#define __METHOD__  "handlerGet"

    if (args.status !=ECA_NORMAL) {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << endl;
        return;
    }

    unsigned int _handle = (unsigned long)  args.usr; // ca_puser(args.chid);


    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())  {

            ChannelRequestStatus  channelRequestStatusGet=(*it_handle).getChannelRequestStatusGet();
            channelRequestStatusGet.setCallbackKind(false, true);

						if(MUTEX){cafeMutex.lock();}
            handle_index.modify(it_handle, change_eventHandlerArgs   (args));
						handle_index.modify(it_handle, change_channelRequestStatusGet(channelRequestStatusGet));
						//cout << __METHOD__ << " GET CALLBACK DONE " << (*it_handle).getChannelRequestStatusGet().getCallbackProgressKind() << endl;
						if(MUTEX){cafeMutex.unlock();}
						
						
							//**** ADD USER CODE HERE ***//
						
						
         		if (args.type < DBR_GR_STRING) {
            	PVDataHolder pvd(args.count);
            	(*it_handle).getPVDataHolder(pvd);
            	//pvd.print();
            	//cout << "val/D//= " << pvd.getAsString(0) << endl;

         		}
         		else if (args.type < DBR_PUT_ACKT) {
            	PVCtrlHolder pvc(args.count);
            	(*it_handle).getPVCtrlHolder(pvc);
            	//pvc.print();
            	//cout << "val/C/= " << pvc.getAsString(0) << endl;
         		}
				 
						
							//****  END USER CODE ***// 
    }
		else {
				
        	  cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        	  cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
    }


return;
#undef __METHOD__
};


