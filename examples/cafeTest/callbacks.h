//
//  callbacks.h
//


void callbackHandlerMonitor( struct event_handler_args args)  {
#define __METHOD__  "callbackHandlerMonitor"

    if (args.status !=ECA_NORMAL) {
        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        std::cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << std::endl;
        return;
    }

    unsigned int _handle = (unsigned long) ca_puser(args.chid);// args.usr; // ca_puser(args.chid);

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find((unsigned int)_handle);

    if (it_handle != handle_index.end()) {
        //std::cout << (*it_handle).getPV() << " " << (*it_handle).getHandle() << std::endl;
        if(MUTEX){cafeMutex.lock();}
        handle_index.modify(it_handle, change_eventHandlerArgs   (args));
        if(MUTEX){cafeMutex.unlock();}


				//**** ADD USER CODE HERE ***//
		
         if (args.type < DBR_GR_STRING) {
            PVDataHolder pvd(args.count);
            (*it_handle).getPVDataHolder(pvd);
            //pvd.print();
            //std::cout << "val/D//= " << pvd.getAsString(0) << std::endl;

         }
         else if (args.type < DBR_PUT_ACKT) {
            PVCtrlHolder pvc(args.count);
            (*it_handle).getPVCtrlHolder(pvc);
            //pvc.print();
            //std::cout << "val/C/= " << pvc.getAsString(0) << std::endl;
         }
				 
				  			
			  //std::cout <<  "args.usr = " << (unsigned int) args.usr << std::endl;
				//std::cout <<  "getUsrArgs() = " << (unsigned int) (*it_handle).getUsrArgs() << std::endl;
				 
				 
				/* 
				vector<MonitorPolicy> mpV = (*it_handle).getMonitorPolicyVector();
										
				for (int i=0; i<mpV.size(); ++i) {				
			  	std::cout << " i " << i << std::endl;
				  std::cout <<  mpV[i].getUserArgs() << std::endl;
					mpV[i].print();
				}
				*/
				//****  END USER CODE ***// 
		
    }
    else {
        std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
        std::cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << std::endl;
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
        std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
        std::cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << std::endl;
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
						
						//std::cout << __METHOD__ << " WITH_CALLBACK_USER_SUPLLIED for PUT done " << (*it_handle).getChannelRequestStatusPut().getCallbackProgressKind() << std::endl;

            if(MUTEX)cafeMutex.unlock();
				
				  	//**** ADD USER CODE HERE ***//
				
						
        }
        else {
       		 std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
        	 std::cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << std::endl;
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
        std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
        std::cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << std::endl;
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
						//std::cout << __METHOD__ << " GET CALLBACK DONE " << (*it_handle).getChannelRequestStatusGet().getCallbackProgressKind() << std::endl;
						if(MUTEX){cafeMutex.unlock();}
						
						
							//**** ADD USER CODE HERE ***//
						
						
         		if (args.type < DBR_GR_STRING) {
            	PVDataHolder pvd(args.count);
            	(*it_handle).getPVDataHolder(pvd);
            	//pvd.print();
            	//std::cout << "val/D//= " << pvd.getAsString(0) << std::endl;

         		}
         		else if (args.type < DBR_PUT_ACKT) {
            	PVCtrlHolder pvc(args.count);
            	(*it_handle).getPVCtrlHolder(pvc);
            	//pvc.print();
            	//std::cout << "val/C/= " << pvc.getAsString(0) << std::endl;
         		}
				 
						
							//****  END USER CODE ***// 
    }
		else {
				
        	  std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
        	  std::cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << std::endl;
    }


return;
#undef __METHOD__
};


