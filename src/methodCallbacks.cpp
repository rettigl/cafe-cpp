///
/// \file    methodCallbacks.cpp
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0

#include "conduitEventHandlerArgs.h"
#include "methodCallbacks.h"
#include "connect.h"

using namespace std;

//in methodCallbacks.h
//include <config.h>

//#if HAVE_PYTHON_H
//#include <PyCafe_api.h>
//#endif


#if HAVE_PYTHON_H

/**
 *  Callback function for when putWithCallback method is invoked
 *  \param args input: event handler_args structure
 */
void CALLBACK_CAFE::PyHandlerPut( struct event_handler_args args)
{
#define __METHOD__  "CALLBACK_CAFE::PyHandlerPut"

    if (args.status !=ECA_NORMAL) {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << endl;
        return;
    }


    // cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;

    unsigned int _handle = (unsigned long) args.usr;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find((unsigned int) _handle);

    if (it_handle != handle_index.end())  {

        if(MUTEX)cafeMutex.lock();

        //Change Channel Policy to NO_WAIT(?)

        ChannelRequestStatus  channelRequestStatusPut=(*it_handle).getChannelRequestStatusPut();

        channelRequestStatusPut.setCallbackKind(false, true);

        handle_index.modify(it_handle, change_channelRequestStatusPut(channelRequestStatusPut));

        if(MUTEX)cafeMutex.unlock();


        //if HAVE_PYTHON_H

        (*it_handle).PyPutHandler();

        //endif

    }
    else {

        bool internalFlag=false;
        cafeConduit_set::iterator itcs;
        // Loop through all elements and search for handle
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {

            if ( (*itcs).getHandle()==_handle) {
                cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cout << " INFORMATION to author: MATCHed Handle= " << _handle << " to PV= " << (*itcs).getPV() << endl;
                cout << " by looping through itcs::iterator, since the by_handle::iterator was NOT found! " << endl;

                if(MUTEX)cafeMutex.lock();
                ChannelRequestStatus  channelRequestStatusPut=(*itcs).getChannelRequestStatusPut();
                channelRequestStatusPut.setCallbackKind(false, true);
                handle_index.modify(itcs, change_channelRequestStatusPut(channelRequestStatusPut));
                if(MUTEX)cafeMutex.unlock();

                //if HAVE_PYTHON_H
                (*it_handle).PyPutHandler();
                //endif

                internalFlag=true;
                break;
            }
        }

        if (!internalFlag) {
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
        }

    }

    return;
#undef __METHOD__
};

//#endif


//#if HAVE_PYTHON_H

/**
 *  Callback function for getCallback method is invoked
 *  with a pointer to the retrieved value
 *  \param args input: event handler arguments
 */
void CALLBACK_CAFE::PyHandlerGet( struct event_handler_args args)
{

#define __METHOD__  "CALLBACK_CAFE::PyHandlerGet"


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

        if(MUTEX) {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_eventHandlerArgs   (args));
        handle_index.modify(it_handle, change_channelRequestStatusGet(channelRequestStatusGet));
        //cout << __METHOD__ << " CALLBACK DONE " << (*it_handle).getChannelRequestStatusGet().getCallbackProgressKind() << endl;
        if(MUTEX) {
            cafeMutex.unlock();
        }


        //if HAVE_PYTHON_H
        (*it_handle).PyGetHandler();
        //endif

    }
    else {

        bool internalFlag=false;
        cafeConduit_set::iterator itcs;
        // Loop through all elements and search for handle
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {

            if ( (*itcs).getHandle()==_handle) {
                cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cout << " INFORMATION to author: MATCHed Handle= " << _handle << " to PV= " << (*itcs).getPV() << endl;
                cout << " by looping through tcs::iterator, since the by_handle::iterator was NOT found! " << endl;

                ChannelRequestStatus  channelRequestStatusGet=(*itcs).getChannelRequestStatusGet();
                channelRequestStatusGet.setCallbackKind(false, true);
                if(MUTEX) {
                    cafeMutex.lock();
                }
                handle_index.modify(itcs, change_eventHandlerArgs   (args));
                handle_index.modify(itcs, change_channelRequestStatusGet(channelRequestStatusGet));
                if(MUTEX) {
                    cafeMutex.unlock();
                }


                //if HAVE_PYTHON_H
                (*it_handle).PyGetHandler();
                //endif


                internalFlag=true;
                break;
            }
        }

        if (!internalFlag) {
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
        }



    }


    return;
#undef __METHOD__

};


#endif



/**
 *  Callback function for pulseID PV method is invoked
 *  \param args input: event handler_args structure
 */
void CALLBACK_CAFE::handlerPulseID( struct event_handler_args args)
{
#define __METHOD__  "CALLBACK_CAFE::handlerPulseID"

    if (args.status !=ECA_NORMAL) {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << endl;
        return;
    }

    unsigned int _handle = (unsigned long) args.usr;
    unsigned long long pulse_id=0;


    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;


    it_handle = handle_index.find((unsigned int) _handle);

    if (it_handle != handle_index.end()) {
        //std::cout << (*it_handle).getPV() << " / " << (*it_handle).getHandle() ;
        if(MUTEX) {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_eventHandlerArgs   (args));

        PVDataHolder pvd;
        (*it_handle).getPVDataHolder(pvd);
        pulse_id = pvd.getAsULongLong();
        //std::cout << " / pulse ID " << pulse_id << std::endl;
        handle_index.modify(it_handle, change_beamEventNo(pulse_id));
        if(MUTEX) {
            cafeMutex.unlock();
        }

    }
    else {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
        return;
    }





    /*
    PVDataHolder pvd;
    (*it_handle).getPVDataHolder(pvd);

    */


    /*
    cafeConduit_set::iterator itcs;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {
    if ((*itcs).getChannelID() != NULL) {
    		if(MUTEX){cafeMutex.lock();}
    handle_index.modify(itcs, change_beamEventNo(pulse_id));
    if(MUTEX){cafeMutex.unlock();}
    }
    }
    */


    return;
#undef __METHOD__
};







/**
 *  Callback function for when putWithCallback method is invoked
 *  \param args input: event handler_args structure
 */
void CALLBACK_CAFE::handlerPut( struct event_handler_args args)
{
#define __METHOD__  "CALLBACK_CAFE::handlerPut"

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

        //Change Channel Policy to NO_WAIT(?)

        ChannelRequestStatus  channelRequestStatusPut=(*it_handle).getChannelRequestStatusPut();

        channelRequestStatusPut.setCallbackKind(false, true);

        handle_index.modify(it_handle, change_channelRequestStatusPut(channelRequestStatusPut));

        if(MUTEX)cafeMutex.unlock();


    }
    else {

        bool internalFlag=false;
        cafeConduit_set::iterator itcs;
        // Loop through all elements and search for handle
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {


            if ( (*itcs).getHandle()==_handle) {
                cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cout << " INFORMATION to author: MATCHed Handle= " << _handle << " to PV= " << (*itcs).getPV() << endl;
                cout << " by looping through itcs::iterator, since the by_handle::iterator was NOT found! " << endl;

                if(MUTEX)cafeMutex.lock();
                ChannelRequestStatus  channelRequestStatusPut=(*itcs).getChannelRequestStatusPut();
                channelRequestStatusPut.setCallbackKind(false, true);
                handle_index.modify(itcs, change_channelRequestStatusPut(channelRequestStatusPut));
                if(MUTEX)cafeMutex.unlock();


                internalFlag=true;
                break;
            }
        }

        if (!internalFlag) {
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
        }

    }

    return;
#undef __METHOD__
};



/**
 *  Callback function for getCallback method is invoked
 *  with a pointer to the retrieved value
 *  \param args input: event handler arguments
 */
void CALLBACK_CAFE::handlerGet( struct event_handler_args args)
{
#define __METHOD__  "CALLBACK_CAFE::handlerGet"

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

        if(MUTEX) {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_eventHandlerArgs   (args));
        handle_index.modify(it_handle, change_channelRequestStatusGet(channelRequestStatusGet));
        //cout << __METHOD__ << " CALLBACK DONE " << (*it_handle).getChannelRequestStatusGet().getCallbackProgressKind() << endl;
        if(MUTEX) {
            cafeMutex.unlock();
        }
    }
    else {

        bool internalFlag=false;
        cafeConduit_set::iterator itcs;
        // Loop through all elements and search for handle
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {

            if ( (*itcs).getHandle()==_handle) {
                cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cout << " INFORMATION to author: MATCHed Handle= " << _handle << " to PV= " << (*itcs).getPV() << endl;
                cout << " by looping through tcs::iterator, since the by_handle::iterator was NOT found! " << endl;

                ChannelRequestStatus  channelRequestStatusGet=(*itcs).getChannelRequestStatusGet();
                channelRequestStatusGet.setCallbackKind(false, true);
                if(MUTEX) {
                    cafeMutex.lock();
                }
                handle_index.modify(itcs, change_eventHandlerArgs   (args));
                handle_index.modify(itcs, change_channelRequestStatusGet(channelRequestStatusGet));
                if(MUTEX) {
                    cafeMutex.unlock();
                }

                internalFlag=true;
                break;
            }
        }

        if (!internalFlag) {
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
        }
    }


    return;
#undef __METHOD__
};


/**
 *  Callback function for handlerGetCtrl method is invoked
 *  with a pointer to the retrieved value
 *  \param args input: event handler arguments
 */
void CALLBACK_CAFE::handlerGetCtrl( struct event_handler_args args)
{
#define __METHOD__  "CALLBACK_CAFE::handlerGetCtrl "


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

        if(MUTEX) {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_eventHandlerArgs   (args));
        if(MUTEX) {
            cafeMutex.unlock();
        }
        ChannelRequestStatus  channelRequestStatusGetCtrl=(*it_handle).getChannelRequestStatusGetCtrl();
        channelRequestStatusGetCtrl.setCallbackKind(false, true);
        if(MUTEX) {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));
        if(MUTEX) {
            cafeMutex.unlock();
        }

    }
    else {

        bool internalFlag=false;
        cafeConduit_set::iterator itcs;
        // Loop through all elements and search for handle
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {

            if ( (*itcs).getHandle()==_handle) {
                cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cout << " INFORMATION to author: MATCHed Handle= " << _handle << " to PV= " << (*itcs).getPV() << endl;
                cout << " by looping through itcs::iterator, since the by_handle::iterator was NOT found! " << endl;
                if(MUTEX) {
                    cafeMutex.lock();
                }
                handle_index.modify(itcs, change_eventHandlerArgs   (args));
                if(MUTEX) {
                    cafeMutex.unlock();
                }
                ChannelRequestStatus  channelRequestStatusGetCtrl=(*itcs).getChannelRequestStatusGetCtrl();
                channelRequestStatusGetCtrl.setCallbackKind(false, true);
                if(MUTEX) {
                    cafeMutex.lock();
                }
                handle_index.modify(itcs, change_channelRequestStatusGetCtrl(channelRequestStatusGetCtrl));
                if(MUTEX) {
                    cafeMutex.unlock();
                }

                internalFlag=true;
                break;
            }
        }

        if (!internalFlag) {
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
        }
    }

    return;
#undef __METHOD__
};



/**
 *  Callback function for handlerSTSACK method is invoked
 *  with a pointer to the retrieved value
 *  \param args input: event handler arguments
 */
void CALLBACK_CAFE::handlerGetSTSACK( struct event_handler_args args)
{
#define __METHOD__  "CALLBACK_CAFE::handlerGetSTSACK "


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

        if(MUTEX) {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_eventHandlerArgs   (args));
        if(MUTEX) {
            cafeMutex.unlock();
        }
        ChannelRequestStatus  channelRequestStatusGetSTSACK=(*it_handle).getChannelRequestStatusGetSTSACK();

        channelRequestStatusGetSTSACK.setCallbackKind(false, true);
        if(MUTEX) {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestStatusGetSTSACK(channelRequestStatusGetSTSACK));
        if(MUTEX) {
            cafeMutex.unlock();
        }

    }
    else {

        bool internalFlag=false;
        cafeConduit_set::iterator itcs;
        // Loop through all elements and search for handle
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {

            if ( (*itcs).getHandle()==_handle) {
                cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cout << " INFORMATION to author: MATCHed Handle= " << _handle << " to PV= " << (*itcs).getPV() << endl;
                cout << " by looping through itcs::iterator, since the by_handle::iterator was NOT found! " << endl;

                if(MUTEX) {
                    cafeMutex.lock();
                }
                handle_index.modify(itcs, change_eventHandlerArgs   (args));
                if(MUTEX) {
                    cafeMutex.unlock();
                }
                ChannelRequestStatus  channelRequestStatusGetSTSACK=(*itcs).getChannelRequestStatusGetSTSACK();
                channelRequestStatusGetSTSACK.setCallbackKind(false, true);
                if(MUTEX) {
                    cafeMutex.lock();
                }
                handle_index.modify(itcs, change_channelRequestStatusGetSTSACK(channelRequestStatusGetSTSACK));
                if(MUTEX) {
                    cafeMutex.unlock();
                }
                internalFlag=true;
                break;
            }
        }

        if (!internalFlag) {
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
        }
    }


    return;
#undef __METHOD__
};



/**
 *  Callback function for handlerClassName method is invoked
 *  with a pointer to the retrieved value
 *  \param args input: event handler arguments
 */
void CALLBACK_CAFE::handlerGetClassName( struct event_handler_args args)
{
#define __METHOD__  "CALLBACK_CAFE::handlerGetClassName "


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

        if(MUTEX) {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_eventHandlerArgs   (args));
        if(MUTEX) {
            cafeMutex.unlock();
        }

        ChannelRequestStatus  channelRequestStatusGetClassName=(*it_handle).getChannelRequestStatusGetClassName();

        channelRequestStatusGetClassName.setCallbackKind(false, true);

        if(MUTEX) {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestStatusGetClassName(channelRequestStatusGetClassName));
        if(MUTEX) {
            cafeMutex.unlock();
        }

    }
    else {

        bool internalFlag=false;
        cafeConduit_set::iterator itcs;
        // Loop through all elements and search for handle
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs) {

            if ( (*itcs).getHandle()==_handle) {
                cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cout << " INFORMATION to author: MATCHed Handle= " << _handle << " to PV= " << (*itcs).getPV() << endl;
                cout << " by looping through tcs::iterator, since the by_handle::iterator was NOT found! " << endl;

                if(MUTEX) {
                    cafeMutex.lock();
                }
                handle_index.modify(itcs, change_eventHandlerArgs (args));
                if(MUTEX) {
                    cafeMutex.unlock();
                }
                ChannelRequestStatus  channelRequestStatusGetClassName=(*it_handle).getChannelRequestStatusGetClassName();
                channelRequestStatusGetClassName.setCallbackKind(false, true);
                if(MUTEX) {
                    cafeMutex.lock();
                }
                handle_index.modify(itcs, change_channelRequestStatusGetClassName(channelRequestStatusGetClassName));
                if(MUTEX) {
                    cafeMutex.unlock();
                }

                internalFlag=true;
                break;
            }
        }

        if (!internalFlag) {
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << endl;
        }
    }

    return;
#undef __METHOD__
};







