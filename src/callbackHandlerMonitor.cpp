//
/// \file    callbackHandlerMonitor.cpp
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#include <cadef.h>
#include <global.h>
#include <policies.h>
#include <conduitFriends.h>
#include <conduitConnectionHandlerArgs.h>
#include <conduitEventHandlerArgs.h>
#include <methodCallbacks.h>
#include <granules.h>


using namespace std;
using namespace boost::posix_time;

/**
 *  \brief Callback function for monitor method is invoked
 *  with a pointer to the retrieved value
 *  \param args input: event handler arguments
 */
void MonitorPolicy::callbackHandlerMonitor( struct event_handler_args args)
{
#define __METHOD__  "MonitorPolicy::callbackHandlerMonitor"

    if (args.status !=ECA_NORMAL)
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << endl;
        //return;
    }

    unsigned int _handle = (unsigned long) ca_puser(args.chid);// args.usr; // ca_puser(args.chid);

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find((unsigned int)_handle);

    if (it_handle != handle_index.end())
    {
        //cout << (*it_handle).getPV() << " " << (*it_handle).getHandle() << endl;
        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_eventHandlerArgs   (args));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

        /*
         if (args.type < DBR_GR_STRING) {
             PVDataHolder pvd(args.count);
             (*it_handle).getPVDataHolder(pvd);
            //// pvd.print();
             cout << "val/D//= " << pvd.getAsString(0) << endl;

          }
          else if (args.type < DBR_PUT_ACKT) {
             PVCtrlHolder pvc(args.count);
             (*it_handle).getPVCtrlHolder(pvc);
             ////pvc.print();
             cout << "val/C/= " << pvc.getAsString(0) << endl;
          }
         */
    }
    else
    {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
        return;
    }

#if HAVE_ZEROMQ
    if (SF_WITH_PULSE_ID)
    {

        cafeConduit_set::iterator itcs;
        unsigned long long pulseID=0;
        PVDataHolder pvd;
        cafeConduit_set_by_pv & pv_index = cs.get<by_pv> ();
        cafeConduit_set_by_pv::iterator it_pv;
        it_pv = pv_index.find(SF_PULSE_ID_PV);

        // Two possibilities of getting a match!
        if (it_pv != pv_index.end())
        {
            if ((*it_pv).getChannelID() != NULL)
            {
                (*it_pv).getPVDataHolder(pvd);
                pulseID = (unsigned long long) pvd.getAsLongLong();
            }
        }
        else
        {
            // Loop through all elements and search for pv match
            for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
            {
                if ((*itcs).getChannelID() != NULL && (*itcs).getPV()==SF_PULSE_ID_PV)
                {
                    (*itcs).getPVDataHolder(pvd);
                    pulseID = (unsigned long long) pvd.getAsLongLong();
                    break;
                }
            }
        }
        if(MUTEX)
        {
            cafeMutex.lock();
        }

        //std::cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << std::endl;
        //std::cout << "Pulse Id " << pulseID << std::endl;

        handle_index.modify(it_handle, change_beamEventNo(pulseID));
        (*it_handle).getPVDataHolder(pvd);
        //handle_index.modify(it_handle, change_mapPulseID(pvd));
        handle_index.modify(it_handle, change_dequePulseID(pvd));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

   } // if SF_WITH_PULSE_ID
#endif

    return;
#undef __METHOD__
};



/**
 *  \brief Callback function for monitor method is invoked
 *  with a pointer to the retrieved value; also fires a user supplied Python callback.
 *  \param args input: event handler arguments
 */
void MonitorPolicy::PyCallbackHandlerMonitorData (struct event_handler_args args)
{
#define __METHOD__  "MonitorPolicy::PyCallbackHandlerMonitorData"

  //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
  //  cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << endl;

    if (args.status !=ECA_NORMAL)
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << endl;
        //  return;
    }

    unsigned int _handle = (unsigned long) ca_puser(args.chid);// args.usr; // ca_puser(args.chid);

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find((unsigned int)_handle);

    if (it_handle != handle_index.end())
    {

        if(MUTEX)
        {
            cafeMutex.lock();
        }

        handle_index.modify(it_handle, change_eventHandlerArgs   (args));

        if(MUTEX)
        {
            cafeMutex.unlock();
        }

	
#if HAVE_PYTHON_H

        //For monitors args.usr is the id of the monitorpolicy class  
	//std::cout << "usrArgs == " << args.usr << std::endl;
	//Place timecheck here
	//Get present timestamp
        unsigned long monid = (unsigned long) (*it_handle).getUsrArgs(); 
        //std::cout << "monitorID " << monid << std::endl;
	MonitorPolicy mp = (*it_handle).getMonitorPolicy(monid);
	//std::cout << "monitorID " << mp.getNotifyDeltaMilliSeconds() <<  std::endl;
	//mp.setNotifyDeltaMilliSeconds(1000);


        ptime timeNow(microsec_clock::local_time());

      	//Get time of last update as recorded in cache
	ptime timePrevious =  mp.getLastUpdate(); //(*it_handle).getCallback_ptime();

	//Compare times and if delta is greater than requested - fire event
	time_duration duration(timeNow-timePrevious);
	double timeElapsedMilliseconds= (double) duration.total_milliseconds();
	double updateDeltaMillioseconds = mp.getNotifyDeltaMilliSeconds(); //1000; //1Hz

	
	//if ( (*it_handle).pv == "SINSB03-DSCR110:ENERGY-OP" ) {

	//std::cout << (*it_handle).pv <<" time elapses " << timeElapsedMilliseconds << std::endl;
	//std::cout << "monitorID " << monid << " wait" << mp.getNotifyDeltaMilliSeconds() <<  std::endl;
	//}
	

	if (timeElapsedMilliseconds < updateDeltaMillioseconds*0.998) {
	   return;
	}
	else {
	  //update cache with timeNow
	  // handle_index.modify(it_handle, change_callback_ptime(timeNow));
	   mp.setLastUpdate(timeNow);
	   handle_index.modify(it_handle, change_monitorPolicy(mp));
	   //std::cout << (*it_handle).pv <<" time elapses " << timeElapsedMilliseconds << std::endl;
	}
	
        if (args.type < DBR_GR_STRING)
        {
	  //(*it_handle).PyDataEventHandler();
            (*it_handle).CyDataEventHandler();
	    //std::cout << (*it_handle).pv <<" time elapses " << timeElapsedMilliseconds << std::endl;
        }
        else if (args.type < DBR_PUT_ACKT)
        {
	  //(*it_handle).PyCtrlEventHandler();
            (*it_handle).CyCtrlEventHandler();
        }
#endif
    }
    else
    {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
    }

    return;
#undef __METHOD__
};


/**
 *  \brief Callback function for monitor method is invoked
 *  with a pointer to the retrieved value; also fires a user supplied Python callback.
 *  \param args input: event handler arguments
 */
void MonitorPolicy::PyCallbackHandlerMonitor (struct event_handler_args args)
{
#define __METHOD__  "MonitorPolicy::PyCallbackHandlerMonitor"


    if (args.status !=ECA_NORMAL)
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << endl;
        // return;
    }

    unsigned int _handle = (unsigned long) ca_puser(args.chid);// args.usr; // ca_puser(args.chid);

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find((unsigned int)_handle);

    if (it_handle != handle_index.end())
    {

        if(MUTEX)
        {
            cafeMutex.lock();
        }

        handle_index.modify(it_handle, change_eventHandlerArgs   (args));

        if(MUTEX)
        {
            cafeMutex.unlock();
        }

#if HAVE_PYTHON_H

        unsigned long monid = (unsigned long) (*it_handle).getUsrArgs(); 
	MonitorPolicy mp = (*it_handle).getMonitorPolicy(monid);
        ptime timeNow(microsec_clock::local_time());

      	//Get time of last update as recorded in cache
	ptime timePrevious =  mp.getLastUpdate(); //(*it_handle).getCallback_ptime();

	//Compare times and if delta is greater than requested - fire event
	time_duration duration(timeNow-timePrevious);
	double timeElapsedMilliseconds= (double) duration.total_milliseconds();
	double updateDeltaMillioseconds = mp.getNotifyDeltaMilliSeconds();

	


	if (timeElapsedMilliseconds < updateDeltaMillioseconds*0.998) {
	   return;
	}
	else {
	   mp.setLastUpdate(timeNow);
	   handle_index.modify(it_handle, change_monitorPolicy(mp));
	}	

        //(*it_handle).PyEventHandler();
        (*it_handle).CyEventHandler();
#endif

    }
    else
    {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
    }


    return;
#undef __METHOD__
}



