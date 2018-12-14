//
/// \file    callbackHandlerMonitor.cpp
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#include <cadef.h>
#include "global.h"
#include "policies.h"
#include "conduitFriends.h"
#include "conduitConnectionHandlerArgs.h"
#include "conduitEventHandlerArgs.h"
#include "methodCallbacks.h"
#include "granules.h"


using namespace std;


/**
 *  \brief Callback function for monitor method is invoked
 *  with a pointer to the retrieved value
 *  \param args input: event handler arguments
 */
void MonitorPolicy::callbackHandlerMonitor( struct event_handler_args args)
{
#define __METHOD__  "MonitorPolicy::callbackHandlerMonitor"

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
        if(MUTEX) {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_eventHandlerArgs   (args));
        if(MUTEX) {
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
    else {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
        return;
    }

#if HAVE_ZEROMQ
    if (SF_WITH_PULSE_ID) {

        cafeConduit_set::iterator itcs;
        unsigned long long pulseID=0;
        PVDataHolder pvd;
        cafeConduit_set_by_pv & pv_index = cs.get<by_pv> ();
        cafeConduit_set_by_pv::iterator it_pv;
        it_pv = pv_index.find(SF_PULSE_ID_PV);

        // Three possibilities of getting a match!
        if (it_pv != pv_index.end()) {
            if ((*it_pv).getChannelID() != NULL) {
                (*it_pv).getPVDataHolder(pvd);
                pulseID = (unsigned long long) pvd.getAsLongLong();
            }
        }
        else {
            // Loop through all elements and search for pv match
            for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {
                if ((*itcs).getChannelID() != NULL && (*itcs).getPV()==SF_PULSE_ID_PV) {
                    (*itcs).getPVDataHolder(pvd);
                    pulseID = (unsigned long long) pvd.getAsLongLong();
                    break;
                }
            }
        }
        if(MUTEX) {
            cafeMutex.lock();
        }

        handle_index.modify(it_handle, change_beamEventNo(pulseID));
        (*it_handle).getPVDataHolder(pvd);
        //handle_index.modify(it_handle, change_mapPulseID(pvd));
        handle_index.modify(it_handle, change_dequePulseID(pvd));
        if(MUTEX) {
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

        if(MUTEX) {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_eventHandlerArgs   (args));
        if(MUTEX) {
            cafeMutex.unlock();
        }

#if HAVE_PYTHON_H

        if (args.type < DBR_GR_STRING) {

            (*it_handle).PyDataEventHandler();

        }
        else if (args.type < DBR_PUT_ACKT) {

            (*it_handle).PyCtrlEventHandler();
        }
#endif


    }
    else {
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

        if(MUTEX) {
            cafeMutex.lock();
        }

        handle_index.modify(it_handle, change_eventHandlerArgs   (args));

        if(MUTEX) {
            cafeMutex.unlock();
        }

#if HAVE_PYTHON_H

        (*it_handle).PyEventHandler();

#endif

    }
    else {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
    }


    return;
#undef __METHOD__
}



