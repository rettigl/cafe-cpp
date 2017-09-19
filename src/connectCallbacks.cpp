///
/// \file    connectCallbacks.cc
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#include "connect.h"
#include "conduitFriends.h"

extern cafeConduit_set cs;
extern bool MUTEX;
extern epicsMutex cafeMutex;

/**
 *  Callback function for callbackHandlerAccessRights \n
 *  Modifies Conduit object accordingly \n
 *  Callback handler for the event of a change of R/W access rights \n
 *  R3.14 documentation states that the callback handler is called \n
 *  - whenever CA connects the channel, immediately BEFORE the channel's
 *  connection handler is called \n
 *  - whenever CA disconnects the channel, immediately AFTER the channel's
 *  disconnect callback handler is called \n
 *  - once immediately after installation if the channel is connected \n
 *  - whenever the access rights of a connected channel changes \n
 *   \param args output: access_rights_handler_args parameters \n
 */
void Connect::callbackHandlerAccessRights(struct access_rights_handler_args args) {
#define __METHOD__ "Connect::callbackHandlerAccessRights"

    //Note: At first connection the no of elements is unknown!
    //Therefore we cannot allocate memory for data buffers here!
    //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;

    unsigned int  _handle = (unsigned long) ca_puser(args.chid);

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find((unsigned int) _handle);

    if (it_handle != handle_index.end()) {
        if(MUTEX){cafeMutex.lock();}
        handle_index.modify(it_handle, change_accessRightsHandlerArgs(args));
        if(MUTEX){cafeMutex.unlock();}
    }

    return;
#undef __METHOD__
}



/**
 *   Callback function for callbackHandlerException
 *   Modifies CAFEConduit object accordingly
 *   \param args output: exception_handler_args parameters \n
 *
 */
void Connect::callbackHandlerException(struct exception_handler_args args) {
#define __METHOD__ "Connect::callbackHandlerException"

   // This routine is called on disconnect before the connection handler
   // cout << "-------------------------------------------------------------" << endl;
   // cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
   // cout << "Warning: Virtual circuit disconnect" << endl;
   // cout << "Exception Handler Args has status = " << args.stat << endl;

    unsigned int  _handle=0;

    char buf[512];
    char pName[PVNAME_SIZE];


    if (args.chid) {
        strcpy(pName , ca_name(args.chid));
        _handle = (unsigned long) ca_puser(args.chid);
        sprintf(buf,
         "%s with request handle=%d, channel=%s, op=%ld, datatype=%s, count=%ld. %s",
         args.ctx, (unsigned int) _handle, pName, args.op, dbr_type_to_text (args.type), args.count,
         "Possibly an IOC has been switched off or is rebooting.");

    }
    // This case is more usual(!)
    else {
        strcpy(pName , "unknown");
        sprintf(buf,
        "%s with channel=%s, op=%ld, datatype=%s, count=%ld. %s",
         args.ctx, pName, args.op, dbr_type_to_text (args.type), args.count,
         "Possibly an IOC has been switched off or is rebooting.\n");
    }

    ca_signal (args.stat, buf);

    return;
#undef __METHOD__
}

