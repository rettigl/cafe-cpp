///
/// \file    conduitGroup.cc
/// \author  Jan Chrin, PSI
/// \date    November 2014
/// \version CAFE 1.0.0

#include <conduitGroup.h>
#include <hashConduitGroup.h>
#include <global.h>
#include <cadef.h>
//include <oldAccess.h>
//include <Python.h>

unsigned int ConduitGroup::groupHandleNext=0;

/**
 *   ConduitGroup destructor \n
 *   Good place to clean up! 
 */
ConduitGroup::~ConduitGroup(void){
};

/**
 *   ConduitGroup constructor \n
 *
 */
ConduitGroup::ConduitGroup(void){
};


/**
 *   ConduitGroup constructor has:\n
 *   \param _groupName      assigned group name
 *   \param _ccc            ca_client_context
 *   \param _groupID        unique group identifier (CA_SYNC_GID)
 *   \param _nMember	    number of members in group
 *   \param _mHandle        handlt to Conduit object
 */
ConduitGroup::ConduitGroup(const char * _groupName, ca_client_context * _ccc,  CA_SYNC_GID _groupID,
                        unsigned int _nMember,   unsigned int * _mHandle){

    ccc 	= _ccc;
    groupName 	= _groupName;
    groupID	= _groupID;
    nMember	= _nMember;
    mHandle     = _mHandle;

    timeout_sg_pend_io  = DEFAULT_TIMEOUT_SG_PEND_IO;

    channelTimeoutPolicySGGet.setTimeout(DEFAULT_TIMEOUT_SG_PEND_IO);
    channelTimeoutPolicySGPut.setTimeout(DEFAULT_TIMEOUT_SG_PEND_IO);
    
    channelTimeoutPolicySGGet.setDefaultTimeout(DEFAULT_TIMEOUT_SG_PEND_IO);
    channelTimeoutPolicySGPut.setDefaultTimeout(DEFAULT_TIMEOUT_SG_PEND_IO);


    ++groupHandleNext;
    groupHandle	      = groupHandleNext;

    mStatus = new int [nMember];
    mRule = new bool[nMember];
    for (unsigned int i=0; i<nMember; ++i) {
        mRule[i]=true;
        mStatus[i]=ICAFE_NORMAL;
    }
};



/**
 *  Retrieves PV data through channel access into the CAFEConduit::dataBuffer
 *  \return ICAFE_NORMAL if OK
 */
int ConduitGroup::get(void) const{
#define __METHOD__ "ConduitGroup::get(void) const"


    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    for (unsigned int i=0; i<nMember; ++i) {

        it_handle = handle_index.find(mHandle[i]);

        if (it_handle != handle_index.end()) {

            if (mStatus[i]==ICAFE_NORMAL) {
                //Returns ECA_NORMAL, ECA_BADSYNCGRP, ECA_BADCHID, ECA_BADCOUNT,
                //ECA_BADTYPE, ECA_GETFAIL, or 192 if not connected!
                mStatus[i]=ca_sg_array_get (groupID,
                                (*it_handle).channelRequestMetaData.dbrDataType,
                                (*it_handle).channelRequestMetaData.nelem,
                                (*it_handle).channelRegalia.channelID,
                                (*it_handle).dataBuffer);
            }
        }
    } //for


    //returns ECA_NORMAL, ECA_TIMEOUT, ECA_EVDISALLOW, ECA_BADSYNCGRP

	int groupStatus=ECA_NORMAL;

	groupStatus=ca_sg_block(groupID,  channelTimeoutPolicySGGet.getTimeout() ); //timeout_sg_pend_io );

	//Withdraw this test for now; not required
	//while ( (ca_sg_test((*it_groupHandle).getGroupID()) == ECA_IOINPROGRESS
	//       || gStatus == ECA_TIMEOUT) && channelTimeoutPolicySGGet.getSelfGoverningTimeout()
	//                                && ntries<channelTimeoutPolicySGGet.getNtries()
	//)

    if (groupStatus== ECA_TIMEOUT) {
        CAFEStatus cafeStatus;
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cafeStatus.report(ECA_TIMEOUT);
    }
    else if (groupStatus!= ECA_NORMAL) {
        CAFEStatus cafeStatus;
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << "groupID = " << groupID << " groupName = " << groupName << endl;
        cafeStatus.report(groupStatus);
    }

return groupStatus;
#undef __METHOD__
};


/**
 *  Sets PV data from the Conduit::dataBuffer through channel access
 *  \return ICAFE_NORMAL if OK
 */
int ConduitGroup::put(void) const  {
#define __METHOD__ "ConduitGroup::put(void) const"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    for (unsigned int i=0; i<nMember; ++i) {

        it_handle = handle_index.find(mHandle[i]);

        if (it_handle != handle_index.end()) {

            if (mStatus[i]==ICAFE_NORMAL) {
                //Returns ECA_NORMAL, ECA_BADSYNCGRP, ECA_BADCHID, ECA_BADCOUNT,
                //ECA_BADTYPE, ECA_GETFAIL, or 192 if not connected!
                mStatus[i]=ca_sg_array_put (groupID,
                                (*it_handle).channelRequestMetaPrimitive.dbrDataType,
                                (*it_handle).channelRequestMetaPrimitive.nelem,
                                (*it_handle).channelRegalia.channelID,
                                (*it_handle).putBuffer);

            }
        }
    } //for

    //returns ECA_NORMAL, ECA_TIMEOUT, ECA_EVDISALLOW, ECA-BADSYNCGRP


	int groupStatus=ECA_NORMAL;

	groupStatus=ca_sg_block(groupID, channelTimeoutPolicySGPut.getTimeout() ); // timeout_sg_pend_io );


	// epics code for ca_sg_block
	/*
	ca_client_context *pcac;
		int status = fetchClientContext ( &pcac );
		if ( status == ECA_NORMAL ) {
			CASG * pcasg;
			{
			epicsGuard < epicsMutex > guard ( pcac->mutex );
			  pcasg = pcac->lookupCASG ( guard, groupID );
			  if ( pcasg ) {
				status = pcasg->block (
					pcac->pCallbackGuard.get (), guard, channelTimeoutPolicySGPut.getTimeout()  );
			  }
			  else {
				  status = ECA_BADSYNCGRP;
			  }
			}
			if ( pcasg ) {
				sync_group_reset ( *pcac, *pcasg );
			}
		}
	*/
	//ca_poll();
	//ca_sg_test(groupID);
	//sleep(1);



    if (groupStatus== ECA_TIMEOUT) {
        CAFEStatus cafeStatus;
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cafeStatus.report(ECA_TIMEOUT);
    }
    //groupStatus is ECA_NORMAL even if one of the channels is disconnected

return groupStatus;
#undef __METHOD__
};
