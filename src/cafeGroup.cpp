
///
/// \file   cafeGroup.cc
///
/// \author Jan Chrin, PSI
/// \date   January 2016
/// \version CAFE 1.0.0
///
#include "cafe.h"


//// Groups

/**
 *  Fills the PVGroup object indentified by the groupHandle from memory.\n
 *  Does NOT do a "get". Method used to fill the PVGroup class with meta-data.
 *  \param  _groupHandle input: handle to the group object
 *  \param   pvgroup     output: data pertaining to the object
 *  \return statusGroup
 */
int  CAFE::groupFetch(const unsigned int _groupHandle, PVGroup &pvgroup) {
#define __METHOD__ "groupFetch( _groupHandle,  &pvgroup)"

	string s = handleHelper.getGroupNameFromGroupHandle(_groupHandle);

	if (s!="") {
		 bool groupExists=false;

		 for (unsigned short i = 0; i < PVGroupV.size(); ++i) {
			 if (strcmp(PVGroupV[i].name, s.c_str()) == 0) {
				 pvgroup = PVGroupV[i];
				 pvgroup.groupHandle=_groupHandle;
				 groupExists=true;
				 break;
			 }
		 }
		 if (!groupExists) {
			 cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			 cafeStatus.report(ECAFE_UNKNOWN_GROUP);
			 return ECAFE_UNKNOWN_GROUP;
		 }
	}
	else {
		return ECAFE_UNKNOWN_GROUP;
	}
	return ICAFE_NORMAL;
#undef __METHOD__
};

/**
 *  Fills the PVGroup object indentified by the groupHandle from memory.\n
 *  Method used to fill the PVGroup class both with meta-data and actual data
 *  (from hash table)
 *  \param  _groupHandle input: handle to the group object
 *  \param   pvgroup     output: data pertaining to the object
 *  \return statusGroup
 */
int  CAFE::groupAttach(const unsigned int _groupHandle, PVGroup &pvgroup) {
#define __METHOD__ "groupAttach(_groupHandle,  &pvgroup)"

	string s = handleHelper.getGroupNameFromGroupHandle(_groupHandle);



	unsigned int iIdx;

	if (s!="") {
		 bool groupExists=false;

		 for (unsigned short i = 0; i < PVGroupV.size(); ++i) {
			 if (strcmp(PVGroupV[i].name, s.c_str()) == 0) {
				 pvgroup = PVGroupV[i];
				 iIdx=i;
				 groupExists=true;
				 break;
			 }
		 }

		 if (!groupExists) {
			 cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			 cafeStatus.report(ECAFE_UNKNOWN_GROUP);
			 return ECAFE_UNKNOWN_GROUP;
		 }
	}
	else {
		return ECAFE_UNKNOWN_GROUP;
	}

	//Fill group from current data in hash table:
	pvgroup.statusGroup=ICAFE_NORMAL;

	cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
	cafeGroup_set_by_groupHandle::iterator it_groupHandle;
	it_groupHandle = groupHandle_index.find(_groupHandle);


	if (it_groupHandle != groupHandle_index.end()) {

		unsigned int  nGroupMember = (*it_groupHandle).getNMember();

		if (pvgroup.npv != (*it_groupHandle).getNMember()) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cout << "A MISMATCH IN THE NUMBER OF GROUP MEMBERS DETECTED " << endl;
			cout << "CLIENT (pvgroup.npv) CLAIMS "  <<  pvgroup.npv
				 << " CONDUITGROUP OBJECT CLAIMS "  <<  (*it_groupHandle).getNMember() << endl;
			nGroupMember = min(pvgroup.npv,(*it_groupHandle).getNMember());
			cout << "TAKING SMALLER OF THE TWO NUMBERS " << endl;
		}

		cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
		cafeConduit_set_by_handle::iterator it_handle;


		for (unsigned int  iMember = 0; iMember < nGroupMember; ++iMember) {

			bool hf=false;
			//reset
			pvgroup.pvdata[iMember].alarmStatus    =-1;
			pvgroup.pvdata[iMember].alarmSeverity  =-1;
			pvgroup.pvdata[iMember].ts.secPastEpoch=0;
			pvgroup.pvdata[iMember].ts.nsec=0;
			pvgroup.pvdata[iMember].hasAlarm=true;
			pvgroup.pvdata[iMember].hasTS=true;
			pvgroup.pvdata[iMember].rule=true;
			pvgroup.pvdata[iMember].status=ICAFE_NORMAL;

			it_handle = handle_index.find((*it_groupHandle).mHandle[iMember]);

			if (it_handle != handle_index.end()) {
				hf=true;
			}

			if (!hf) {
				pvgroup.pvdata[iMember].status=ECAFE_INVALID_HANDLE;

				 if(MUTEX){cafeMutex.lock();}
					groupHandle_index.modify(it_groupHandle,
									change_sg_status(pvgroup.pvdata[iMember].status, iMember));
				 if(MUTEX){cafeMutex.unlock();}

				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(ECAFE_INVALID_HANDLE);
				cout <<  "Invalid handle = " << (*it_groupHandle).mHandle[iMember]
						<< " for groupMember [" << iMember << "] " << endl;

				pvgroup.statusGroup=ECAFE_INVALID_HANDLE;
				continue;
			}

			if (!(*it_handle).getChannelRegalia().getConnectFlag()) {

				if (pvgroup.statusGroup==ICAFE_NORMAL) {
					pvgroup.statusGroup=(*it_handle).getChannelRegalia().getCafeConnectionState();
				}
			}

			//this sets size and nelem to 1 if not connected
			//unsigned int a=
			pvgroup.pvdata[iMember].setNelem( (*it_handle).getChannelRegalia().getNelem() );

			//set Device, Attribute for handle

			ChannelDeviceAttribute channelDeviceAttribute;
			channelDeviceAttribute.init((*it_handle).pv, deviceAttributeDeliminator);

			if(MUTEX){cafeMutex.lock();}
			handle_index.modify(it_handle, change_channelDeviceAttribute(
					channelDeviceAttribute));
			if(MUTEX){cafeMutex.unlock();}

			//Above lines replace need for handleHelper
		   // handleHelper.setChannelDeviceAttribute(deviceAttributeDeliminator);


			(*it_handle).getPVDataHolder(pvgroup.pvdata[iMember]);

		} //for


		PVGroupV[iIdx]=pvgroup;
		pvgroup.groupHandle=_groupHandle;

		return pvgroup.statusGroup;
	} else {
		return ECAFE_INVALID_GROUP_HANDLE;
	}
#undef __METHOD__
};


/**
 *  Fills the PVGroup object indentified by pvgroup.groupHandle \n
 *  \param   pvgroup    input/output: data pertaining to the object
 *  \return statusGroup
 */
int  CAFE::groupSet(PVGroup &pvgroup) {
#define __METHOD__ "groupSet(&pvgroup)"

	return CAFE::groupSet(pvgroup.groupHandle, pvgroup);

#undef __METHOD__
};

/**
 *  Fills the PVGroup object indentified by the groupHandle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param   pvgroup    input/output: data pertaining to the object
 *  \return statusGroup
 */
int  CAFE::groupSet(const unsigned int  _groupHandle, PVGroup &pvgroup) {
#define __METHOD__ "groupSet( _groupHandle,  &pvgroup)"

	if (pvgroup.npv==0) {
		cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
		cafeStatus.report(ECAFE_EMPTY_GROUP);
		return ECAFE_EMPTY_GROUP;
	}
	else if (pvgroup.groupHandle!=_groupHandle) {
		cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
		cout << "Input groupHandle " << _groupHandle << " does not match that within pvgroup "
				<< pvgroup.groupHandle << endl;
		cout << "groupSet operation NOT attempted. Client clarification required! " << endl;
		cafeStatus.report(ECAFE_PVGROUP_GROUPHANDLE_MISMATCH);
		return ECAFE_PVGROUP_GROUPHANDLE_MISMATCH;
	}

	int  gStatus=ICAFE_NORMAL;

	cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
	cafeGroup_set_by_groupHandle::iterator it_groupHandle;
	it_groupHandle = groupHandle_index.find(_groupHandle);

	if (it_groupHandle != groupHandle_index.end()) {

		unsigned int  nGroupMember = (*it_groupHandle).getNMember();

		if (pvgroup.npv != (*it_groupHandle).getNMember()) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cout << "A MISMATCH IN THE NUMBER OF GROUP MEMBERS DETECTED " << endl;
			cout << "CLIENT (pvgroup.npv) CLAIMS "  <<  pvgroup.npv
				 << " CONDUITGROUP OBJECT CLAIMS "  <<  (*it_groupHandle).getNMember() << endl;
			nGroupMember = min(pvgroup.npv,(*it_groupHandle).getNMember());
			cout << "TAKING SMALLER OF THE TWO NUMBERS " << endl;
		}

		cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
		cafeConduit_set_by_handle::iterator it_handle;

    bool * nelemPreviousFlag = new 	bool  [nGroupMember];
    unsigned int * nelemPrevious = new unsigned int [nGroupMember];


		for (unsigned int  iMember = 0; iMember < nGroupMember; ++iMember) {

			bool hf=false;
			//reset
			pvgroup.pvdata[iMember].alarmStatus=-1;
			pvgroup.pvdata[iMember].alarmSeverity=-1;
			pvgroup.pvdata[iMember].ts.secPastEpoch=0;
			pvgroup.pvdata[iMember].ts.nsec=0;
			pvgroup.pvdata[iMember].status=ICAFE_NORMAL;

			it_handle = handle_index.find((*it_groupHandle).mHandle[iMember]);

			if (it_handle != handle_index.end()) {
				hf=true;

				if (pvgroup.pvdata[iMember].getRule() != (*it_groupHandle).getRule(iMember)) {
					 if(MUTEX){cafeMutex.lock();}
						groupHandle_index.modify(it_groupHandle,
										change_sg_rule(pvgroup.pvdata[iMember].rule, iMember));
					 if(MUTEX){cafeMutex.unlock();}
				}
				if (!pvgroup.pvdata[iMember].getRule() ) {
					 pvgroup.pvdata[iMember].status=ICAFE_RULE_FALSE;
				}

				 if(MUTEX){cafeMutex.lock();}
					groupHandle_index.modify(it_groupHandle,
									change_sg_status(pvgroup.pvdata[iMember].status, iMember));
				 if(MUTEX){cafeMutex.unlock();}
			}

			if (!hf) {
			
			  nelemPrevious[iMember]=0;
	    	nelemPreviousFlag[iMember]=false;
			
				pvgroup.pvdata[iMember].status=ECAFE_INVALID_HANDLE;

				 if(MUTEX){cafeMutex.lock();}
					groupHandle_index.modify(it_groupHandle,
									change_sg_status(pvgroup.pvdata[iMember].status, iMember));
				 if(MUTEX){cafeMutex.unlock();}

				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(ECAFE_INVALID_HANDLE);
				cout <<  "Invalid handle = " << (*it_groupHandle).mHandle[iMember]
						<< " for groupMember [" << iMember << "] " << endl;
				continue;
			}

			unsigned int  handle=(*it_groupHandle).mHandle[iMember];

      
			nelemPrevious[iMember]=handleHelper.getNelemClient(handle);
	    nelemPreviousFlag[iMember]=false;

			//Find Native DataType
			if ( (pvgroup.pvdata[iMember].status=cafeGranules.channelVerifyPut(handle,
					  pvgroup.pvdata[iMember].dataType)) != ICAFE_NORMAL) {

				 if(MUTEX){cafeMutex.lock();}
					groupHandle_index.modify(it_groupHandle,
									change_sg_status(pvgroup.pvdata[iMember].status, iMember));
				 if(MUTEX){cafeMutex.unlock();}

				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cout << "CHANNEL: " << pvgroup.pvdata[iMember].pv << endl;
				cafeStatus.report(pvgroup.pvdata[iMember].status);

				continue;
			}


     
			
			// Examine No of elements and set accordingly....
			if (handleHelper.getNelemClient(handle) !=  pvgroup.pvdata[iMember].getNelem()) {
			  
				nelemPreviousFlag[iMember]=true;
				handleHelper.setNelem(handle, pvgroup.pvdata[iMember].getNelem());
				//cout << "INFO: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				//cout << "Changing handleHelper.getNelemClient(handle) to that given in  pvgroup.pvdata[iMember].nelem " << endl;
				//cout << "i.e., To: " << pvgroup.pvdata[iMember].nelem << " From: " << handleHelper.getNelemClient(handle)  << endl;		
						
			}
			
			

			if ( (pvgroup.pvdata[iMember].status=cafeGranules.channelPreparePut(handle))
				!= ICAFE_NORMAL) {
				 if(MUTEX){cafeMutex.lock();}
					groupHandle_index.modify(it_groupHandle,
									change_sg_status(pvgroup.pvdata[iMember].status, iMember));
				 if(MUTEX){cafeMutex.unlock();}
				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(pvgroup.pvdata[iMember].status);
				continue;
			}

			pvgroup.pvdata[iMember].dbrDataType=
					((*it_handle).getChannelRequestMetaPrimitive().getDbrDataType());


			switch ((*it_handle).getChannelRequestMetaPrimitive().getDbrDataType()) {

			case DBR_STRING:
				renderString.putString(handle, pvgroup.pvdata[iMember].val.get());
				break;
			case DBR_SHORT:
				renderShort.put(handle, pvgroup.pvdata[iMember].val.get(), pvgroup.pvdata[iMember].dataType);
				break;
			case DBR_FLOAT:
				renderFloat.put(handle, pvgroup.pvdata[iMember].val.get(), pvgroup.pvdata[iMember].dataType);
				break;
			case DBR_ENUM:
				renderEnum.put(handle, pvgroup.pvdata[iMember].val.get(), pvgroup.pvdata[iMember].dataType);
				break;
			case DBR_CHAR:
				renderChar.put(handle, pvgroup.pvdata[iMember].val.get(), pvgroup.pvdata[iMember].dataType);
				break;
			case DBR_LONG:
				renderLong.put(handle, pvgroup.pvdata[iMember].val.get(), pvgroup.pvdata[iMember].dataType);
				break;
			case DBR_DOUBLE:
				renderDouble.put(handle, pvgroup.pvdata[iMember].val.get(), pvgroup.pvdata[iMember].dataType);
				break;
			default:
				cout << "ERROR: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
				cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" <<
						pvgroup.pvdata[iMember].dataType <<endl;
				cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
				return ECAFE_INVALID_SWITCH_CASE;
				break;
			}
		} //for

		//Put Data into putBuffer

		gStatus=(*it_groupHandle).put();
		//this returns normal even if a channel is not connected

		channelTimeoutPolicySGPut = (*it_groupHandle).getChannelTimeoutPolicySGPut();

		if (gStatus!=ECA_NORMAL && channelTimeoutPolicySGPut.getSelfGoverningTimeout()) {
			cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cafeStatus.report(gStatus);
		}

		channelTimeoutPolicySGPut = (*it_groupHandle).getChannelTimeoutPolicySGPut();

		unsigned short ntries=0;
		double originalTimeOut=channelTimeoutPolicySGPut.getTimeout();


		//Withdraw this test for now; not required
		//while ( (ca_sg_test((*it_groupHandle).getGroupID()) == ECA_IOINPROGRESS
		//      || gStatus == ECA_TIMEOUT) && channelTimeoutPolicySGPut.getSelfGoverningTimeout()
		//                                && ntries<channelTimeoutPolicySGPut.getNtries()
		//  ) {




		while ( (gStatus == ECA_TIMEOUT) && channelTimeoutPolicySGPut.getSelfGoverningTimeout()
										  && ntries<channelTimeoutPolicySGPut.getNtries()
			) {
			++ntries;
			//CAFEStatus cafeStatus;

			//Withdraw this test for now; not required
			//if (ca_sg_test((*it_groupHandle).getGroupID()) == ECA_IOINPROGRESS ) {
			//    cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			//    cafeStatus.report(ECA_IOINPROGRESS);
			//}
			if (gStatus == ECA_TIMEOUT) {
				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(ECA_TIMEOUT);
			}

			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cout << "Changing SG PUT timeout from: " << channelTimeoutPolicySGPut.getTimeout()
					<< " to: "  <<
				(originalTimeOut + channelTimeoutPolicySGPut.getDeltaTimeout()*ntries)
				<< " seconds"  <<endl;


			channelTimeoutPolicySGPut.setTimeout( (originalTimeOut +
											  channelTimeoutPolicySGPut.getDeltaTimeout()*ntries));

			if(MUTEX){cafeMutex.lock();}  //lock
			groupHandle_index.modify(it_groupHandle,
									 change_channelTimeoutPolicySGPut(channelTimeoutPolicySGPut));
			if(MUTEX){cafeMutex.unlock();}  //unlock

			if (gStatus == ECA_TIMEOUT) {
				gStatus=(*it_groupHandle).put();
			}
		} //while

		if (ntries>0) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cout << "Changed SG PUT from intitial " << originalTimeOut << " to: "  <<
				(originalTimeOut +
				 channelTimeoutPolicySGPut.getDeltaTimeout()*ntries)
				<< " seconds after " << ntries << " sg_put calls " <<endl;
		}


		//Withdraw this test for now; not required
		//if (ca_sg_test((*it_groupHandle).getGroupID()) == ECA_IOINPROGRESS) {
		//    ca_sg_reset((*it_groupHandle).getGroupID());
		//    cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
		//    std::cout << " No. of ca_sg_test(gid) INSTIGATED: " << ntries+1 << std::endl;
		//    std::cout
		//    << " ca_sg_reset(gid) HAS BEEN FORCED SO THAT ca_sg_test() returns ECA_IODONE"
		//    <<std::endl;
		//    std::cout << " ***PLEASE** REPORT THIS OCCURRENCE TO jan.chrin@psi.ch THANKS!! "
		//    << std::endl;
		//}

		int  * mstatus =(*it_groupHandle).getStatus();

		bool groupStatusFlag = false;
		pvgroup.statusGroup= gStatus; //ICAFE_NORMAL;
		if (pvgroup.statusGroup==ECA_TIMEOUT) {
			groupStatusFlag = true;
		}

		for (unsigned int  iMember = 0; iMember< (*it_groupHandle).getNMember(); ++iMember) {

			if (pvgroup.pvdata[iMember].status == ECAFE_INVALID_HANDLE) {
				continue;
			 }


			it_handle = handle_index.find((*it_groupHandle).mHandle[iMember]);

			if (it_handle != handle_index.end()) {
				//do nothing as handle exists				
			
				if (nelemPreviousFlag[iMember]==true) {
				  handleHelper.setNelem((*it_groupHandle).mHandle[iMember], nelemPrevious[iMember]);
	    	}
				
			}
			else {
				//Should not get this far if invalid handle
				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(ECAFE_INVALID_HANDLE);
				cout <<  "Invalid handle = " << (*it_groupHandle).mHandle[iMember] << " for groupMember [" << iMember << "] " << endl;
				continue;
			}

			if(MUTEX){cafeMutex.lock();}
			handle_index.modify(it_handle, change_status(   mstatus[iMember] ));
			if(MUTEX){cafeMutex.unlock();}

			if (mstatus[iMember]!=ICAFE_NORMAL) {
				pvgroup.pvdata[iMember].status = mstatus[iMember];

				if (pvgroup.pvdata[iMember].status != ICAFE_NORMAL
						&& pvgroup.pvdata[iMember].status != ICAFE_RULE_FALSE
							&& !groupStatusFlag) {
					pvgroup.statusGroup = pvgroup.pvdata[iMember].status;
					groupStatusFlag = true;
				}
				continue;
			}

		} // for iMember

    delete [] nelemPreviousFlag;
    delete [] nelemPrevious;


		return pvgroup.statusGroup;
	} else {
		return ECAFE_INVALID_GROUP_HANDLE;
	}
#undef __METHOD__
}


/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  valV  output: vector of syting values
 *  \param  statusV  output: vector of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, vector<string> &valV, vector<int> &statusV) {
#define __METHOD__ "groupGetScalar( _groupHandle, vector<string> &valV, vector<int> &statusV)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::set(CAFE::getHandlesFromWithinGroupV(_groupHandle)[9],8.88);

		status=CAFE::groupGet(_groupHandle, pvgroup);


		valV.reserve(pvgroup.npv); statusV.reserve(pvgroup.npv);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
			cout << pvgroup.pvdata[iMember].getAsString(0)
					<< " ts " << pvgroup.pvdata[iMember].ts.secPastEpoch << " " << pvgroup.pvdata[iMember].ts.nsec
					<< " STATUS " << pvgroup.pvdata[iMember].status
					<< " iMember" << iMember << endl;
				valV.push_back(pvgroup.pvdata[iMember].getAsString(0));
				statusV.push_back(pvgroup.pvdata[iMember].status);
		}


		if (pvgroup.statusGroup==ECA_TIMEOUT) {
			cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cafeStatus.report(ECA_TIMEOUT);
			cout << "SG TIMEOUT OCCURRED. REVERTING TO ASYNCHRNOUS GET" << endl;
			return CAFE::getScalars(CAFE::getHandlesFromWithinGroupV(_groupHandle), valV, statusV);
		}
		else {
			return pvgroup.statusGroup;
		}



#undef __METHOD__
}


/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  valV  output: vector of double values
 *  \param  statusV  output: vector of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, vector<double> &valV, vector<int> &statusV) {
#define __METHOD__ "groupGetScalar( _groupHandle, vector<double> &valV, vector<int> &statusV)"
		PVGroup pvgroup;


		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		valV.reserve(pvgroup.npv); statusV.reserve(pvgroup.npv);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				valV.push_back(pvgroup.pvdata[iMember].getAsDouble(0));
				statusV.push_back(pvgroup.pvdata[iMember].status);
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}

/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  valV  output: vector of float values
 *  \param  statusV  output: vector of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, vector<float> &valV, vector<int> &statusV) {
#define __METHOD__ "groupGetScalar( _groupHandle, vector<float> &valV, vector<int> &statusV)"
		PVGroup pvgroup;


		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		valV.reserve(pvgroup.npv); statusV.reserve(pvgroup.npv);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				valV.push_back(pvgroup.pvdata[iMember].getAsFloat(0));
				statusV.push_back(pvgroup.pvdata[iMember].status);
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}


/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  valV  output: vector of short values
 *  \param  statusV  output: vector of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, vector<short> &valV, vector<int> &statusV) {
#define __METHOD__ "groupGetScalar( _groupHandle, vector<short> &valV, vector<int> &statusV)"
		PVGroup pvgroup;


		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		valV.reserve(pvgroup.npv); statusV.reserve(pvgroup.npv);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				valV.push_back(pvgroup.pvdata[iMember].getAsShort(0));
				statusV.push_back(pvgroup.pvdata[iMember].status);
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}

/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  valV  output: vector of long values
 *  \param  statusV  output: vector of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, vector<long> &valV, vector<int> &statusV) {
#define __METHOD__ "groupGetScalar( _groupHandle, vector<long> &valV, vector<int> &statusV)"
		PVGroup pvgroup;


		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		valV.reserve(pvgroup.npv); statusV.reserve(pvgroup.npv);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				valV.push_back(pvgroup.pvdata[iMember].getAsLong(0));
				statusV.push_back(pvgroup.pvdata[iMember].status);
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}


/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  valV  output: vector of long long values
 *  \param  statusV  output: vector of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, vector<long long> &valV, vector<int> &statusV) {
#define __METHOD__ "groupGetScalar( _groupHandle, vector<long long> &valV, vector<int> &statusV)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		valV.reserve(pvgroup.npv); statusV.reserve(pvgroup.npv);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				valV.push_back(pvgroup.pvdata[iMember].getAsLongLong(0));
				statusV.push_back(pvgroup.pvdata[iMember].status);
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}


/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  valV  output: vector of unsigned short values
 *  \param  statusV  output: vector of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, vector<unsigned short> &valV, vector<int> &statusV) {
#define __METHOD__ "groupGetScalar( _groupHandle, vector<unsigned short> &valV, vector<int> &statusV)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		valV.reserve(pvgroup.npv); statusV.reserve(pvgroup.npv);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				valV.push_back(pvgroup.pvdata[iMember].getAsUShort(0));
				statusV.push_back(pvgroup.pvdata[iMember].status);
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}


/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  valV  output: vector of unsigned char values
 *  \param  statusV  output: vector of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, vector<unsigned char> &valV, vector<int> &statusV) {
#define __METHOD__ "groupGetScalar( _groupHandle, vector<unsigned char> &valV, vector<int> &statusV)"
		PVGroup pvgroup;


		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		valV.reserve(pvgroup.npv); statusV.reserve(pvgroup.npv);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				valV.push_back(pvgroup.pvdata[iMember].getAsUChar(0));
				statusV.push_back(pvgroup.pvdata[iMember].status);
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}


/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  valV  output: vector of char values
 *  \param  statusV  output: vector of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, vector<char> &valV, vector<int> &statusV) {
#define __METHOD__ "groupGetScalar( _groupHandle, vector<char> &valV, vector<int> &statusV)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		valV.reserve(pvgroup.npv); statusV.reserve(pvgroup.npv);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				valV.push_back(pvgroup.pvdata[iMember].getAsChar(0));
				statusV.push_back(pvgroup.pvdata[iMember].status);
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}

/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  val  output: array of string values
 *  \param  statusArray  output: array of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, string * val, int * statusArray) {
#define __METHOD__ "groupGetScalar( _groupHandle,  string * val, int * statusArray)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				val[iMember]=pvgroup.pvdata[iMember].getAsString(0);
				statusArray[iMember]=pvgroup.pvdata[iMember].status;
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}

/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  val  output: array of double values
 *  \param  statusArray  output: array of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, double * val, int * statusArray) {
#define __METHOD__ "groupGetScalar( _groupHandle,  double * val, int * statusArray)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				val[iMember]=pvgroup.pvdata[iMember].getAsDouble(0);
				statusArray[iMember]=pvgroup.pvdata[iMember].status;
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}

/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  val  output: array of float values
 *  \param  statusArray  output: array of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, float * val, int * statusArray) {
#define __METHOD__ "groupGetScalar( _groupHandle,  float * val, int * statusArray)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				val[iMember]=pvgroup.pvdata[iMember].getAsFloat(0);
				statusArray[iMember]=pvgroup.pvdata[iMember].status;
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}

/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  val  output: array of short values
 *  \param  statusArray  output: array of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, short * val, int * statusArray) {
#define __METHOD__ "groupGetScalar( _groupHandle,  short * val, int * statusArray)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				val[iMember]=pvgroup.pvdata[iMember].getAsShort(0);
				statusArray[iMember]=pvgroup.pvdata[iMember].status;
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}

/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  val  output: array of long values
 *  \param  statusArray  output: array of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, long * val, int * statusArray) {
#define __METHOD__ "groupGetScalar( _groupHandle,  long * val, int * statusArray)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				val[iMember]=pvgroup.pvdata[iMember].getAsLong(0);
				statusArray[iMember]=pvgroup.pvdata[iMember].status;
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}

/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  val  output: array of long long values
 *  \param  statusArray  output: array of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, long long * val, int * statusArray) {
#define __METHOD__ "groupGetScalar( _groupHandle,  long long * val, int * statusArray)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				val[iMember]=pvgroup.pvdata[iMember].getAsLongLong(0);
				statusArray[iMember]=pvgroup.pvdata[iMember].status;
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}

/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  val  output: array of unsigned short values
 *  \param  statusArray  output: array of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, unsigned short * val, int * statusArray) {
#define __METHOD__ "groupGetScalar( _groupHandle, unsigned short * val, int * statusArray)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				val[iMember]=pvgroup.pvdata[iMember].getAsUShort(0);
				statusArray[iMember]=pvgroup.pvdata[iMember].status;
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}

/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  val  output: array of unsigned char values
 *  \param  statusArray  output: array of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, unsigned char * val, int * statusArray) {
#define __METHOD__ "groupGetScalar( _groupHandle,  unsigned char * val, int * statusArray)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				val[iMember]=pvgroup.pvdata[iMember].getAsUChar(0);
				statusArray[iMember]=pvgroup.pvdata[iMember].status;
		}

		//Asynchronous get
		//get()
		//waitForBundleEvents
		//getCache


		return pvgroup.statusGroup;
#undef __METHOD__
}

/**
 *
 *  Retrieves scalar value from the PVGroup object indentified by the group handle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param  val  output: array of char values
 *  \param  statusArray  output: array of int values reporting the status
 *  \return statusGroup
 */
int  CAFE::groupGetScalar(const unsigned int  _groupHandle, char * val, int * statusArray) {
#define __METHOD__ "groupGetScalar( _groupHandle,  char * val, int * statusArray)"
		PVGroup pvgroup;

		CAFE::groupAttach(_groupHandle, pvgroup);
		pvgroup.setHasAlarm(false);
		pvgroup.setHasTS(false);

		CAFE::groupGet(_groupHandle, pvgroup);
		for (unsigned int  iMember = 0; iMember < pvgroup.npv; ++iMember) {
				val[iMember]=pvgroup.pvdata[iMember].getAsChar(0);
				statusArray[iMember]=pvgroup.pvdata[iMember].status;
		}
		return pvgroup.statusGroup;
#undef __METHOD__
}


/**
 *
 *  Fills the PVGroup object indentified by the pvgroup.groupHandle \n
 *  \param   pvgroup     input/output: data pertaining to the object
 *  \return statusGroup
 */
int  CAFE::groupGet(PVGroup &pvgroup) {
#define __METHOD__ "groupGet(&pvgroup)"

	return CAFE::groupGet(pvgroup.groupHandle, pvgroup);

#undef __METHOD__
}


/**
 *  Fills the PVGroup object indentified by the groupHandle \n
 *  \param  _groupHandle input: handle to the group object
 *  \param   pvgroup     output: data pertaining to the object
 *  \return statusGroup
 */
int  CAFE::groupGet(const unsigned int  _groupHandle, PVGroup &pvgroup) {
#define __METHOD__ "groupGet( _groupHandle,  &pvgroup)"


	int  gStatus=ICAFE_NORMAL;

	if (pvgroup.npv==0 || pvgroup.groupHandle==0) {
		gStatus=groupAttach(_groupHandle, pvgroup);
		if (gStatus != ICAFE_NORMAL) {return gStatus;}
	};

	if (pvgroup.groupHandle != _groupHandle) {
		return ECAFE_PVGROUP_GROUPHANDLE_MISMATCH;
	}


	cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
	cafeGroup_set_by_groupHandle::iterator it_groupHandle;
	it_groupHandle = groupHandle_index.find(_groupHandle);

	if (it_groupHandle != groupHandle_index.end()) {

		unsigned int  nGroupMember = (*it_groupHandle).getNMember();

		if (pvgroup.npv != (*it_groupHandle).getNMember()) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cout << "A MISMATCH IN THE NUMBER OF GROUP MEMBERS DETECTED " << endl;
			cout << "CLIENT (pvgroup.npv) CLAIMS "  <<  pvgroup.npv
				 << " CONDUITGROUP OBJECT CLAIMS "  <<  (*it_groupHandle).getNMember() << endl;
			nGroupMember = min(pvgroup.npv,(*it_groupHandle).getNMember());
			cout << "TAKING SMALLER OF THE TWO NUMBERS " << endl;
		}

		cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
		cafeConduit_set_by_handle::iterator it_handle;

		for (unsigned int  iMember = 0; iMember < nGroupMember; ++iMember) {

			bool hf=false;
			//reset
			pvgroup.pvdata[iMember].alarmStatus=-1;
			pvgroup.pvdata[iMember].alarmSeverity=-1;
			pvgroup.pvdata[iMember].ts.secPastEpoch=0;
			pvgroup.pvdata[iMember].ts.nsec=0;
			pvgroup.pvdata[iMember].status=ICAFE_NORMAL;


			it_handle = handle_index.find((*it_groupHandle).mHandle[iMember]);

			if (it_handle != handle_index.end()) {
				hf=true;
				if (pvgroup.pvdata[iMember].getRule() != (*it_groupHandle).getRule(iMember)) {
					 if(MUTEX){cafeMutex.lock();}
						groupHandle_index.modify(it_groupHandle,
										change_sg_rule(pvgroup.pvdata[iMember].rule, iMember));
					 if(MUTEX){cafeMutex.unlock();}
				}
				if (!pvgroup.pvdata[iMember].getRule() ) {
					 pvgroup.pvdata[iMember].status=ICAFE_RULE_FALSE;
				}

				 if(MUTEX){cafeMutex.lock();}
					groupHandle_index.modify(it_groupHandle,
									change_sg_status(pvgroup.pvdata[iMember].status, iMember));
				 if(MUTEX){cafeMutex.unlock();}
			}

			if (!hf) {
				pvgroup.pvdata[iMember].status=ECAFE_INVALID_HANDLE;

				if(MUTEX){cafeMutex.lock();}
					groupHandle_index.modify(it_groupHandle,
									change_sg_status(pvgroup.pvdata[iMember].status, iMember));
				if(MUTEX){cafeMutex.unlock();}

				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(ECAFE_INVALID_HANDLE);
				cout <<  "Invalid handle = " << (*it_groupHandle).mHandle[iMember]
					 << " for groupMember [" << iMember << "] " << endl;
				continue;
			}

			unsigned int  handle=(*it_groupHandle).mHandle[iMember];

			//May happen that channel has just connected and native datatype not yet entered

			CAFE_DATATYPE cdt = pvgroup.pvdata[iMember].dataType;

			//Check groupDataType
			if (pvgroup.pvdata[iMember].dataType<CAFE_STRING ||
				pvgroup.pvdata[iMember].dataType>CAFE_DOUBLE) {
				cdt=CAFE_STRING;
			}



			//Let us take a look at hasAlarm and hasTS flags to determine if these are retrieved from SC
			chtype _chtype= cdt;


			if (!pvgroup.pvdata[iMember].hasAlarm && !pvgroup.pvdata[iMember].hasTS) {
				handleHelper.setCafeDbrType(handle, CAFENUM::DBR_PRIMITIVE);
			}
			else if (pvgroup.pvdata[iMember].hasAlarm && !pvgroup.pvdata[iMember].hasTS) {
				_chtype=dbf_type_to_DBR_STS (cdt);
				handleHelper.setCafeDbrType(handle, CAFENUM::DBR_STS) ;

			}
			else if (pvgroup.pvdata[iMember].hasTS) {
				_chtype=dbf_type_to_DBR_TIME (cdt);
				 handleHelper.setCafeDbrType(handle, CAFENUM::DBR_TIME) ;
			}


			//Always Return to Native DataType for now
			if ( (pvgroup.pvdata[iMember].status=cafeGranules.channelVerifyGet(handle,
				 _chtype)) != ICAFE_NORMAL) {
				 //dbf_type_to_DBR_TIME (cdt))) != ICAFE_NORMAL) {

				 if(MUTEX){cafeMutex.lock();}
					groupHandle_index.modify(it_groupHandle,
									change_sg_status(pvgroup.pvdata[iMember].status, iMember));
				 if(MUTEX){cafeMutex.unlock();}

				//May not be connected - wish to report this here
				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cout << "CHANNEL: " << pvgroup.pvdata[iMember].pv << endl;
				cafeStatus.report(pvgroup.pvdata[iMember].status);
				continue;
			}


			// Examine the No of elements and set accordingly....
			 if ( handleHelper.getNelemClient(handle) !=  pvgroup.pvdata[iMember].getNelem()) {
				//handleHelper.setNelem(handle, pvgroup.pvdata[iMember].getNelem()); //Change made 17 March 2017
				
				cout << "INFO: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cout << "Changing pvgroup.pvdata[iMember].nelem to that given in handleHelper.getNelemClient(handle)" << endl;
				cout << "i.e., From: " << pvgroup.pvdata[iMember].nelem << " To: " << handleHelper.getNelemClient(handle)  << endl;
				pvgroup.pvdata[iMember].nelem=handleHelper.getNelemClient(handle);
			 }


			if ( (pvgroup.pvdata[iMember].status=cafeGranules.channelPrepareGet(handle))
				!= ICAFE_NORMAL) {
				 if(MUTEX){cafeMutex.lock();}
					groupHandle_index.modify(it_groupHandle,
									change_sg_status(pvgroup.pvdata[iMember].status, iMember));
				 if(MUTEX){cafeMutex.unlock();}
				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(pvgroup.pvdata[iMember].status);
				continue;
			}

		} //for




		gStatus=(*it_groupHandle).get();

		channelTimeoutPolicySGGet = (*it_groupHandle).getChannelTimeoutPolicySGGet();

	   //this returns normal even if a channel is not connected

		if (gStatus!=ECA_NORMAL && channelTimeoutPolicySGGet.getSelfGoverningTimeout()) {
			cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;

			cafeStatus.report(gStatus);
			if (gStatus == ECA_TIMEOUT) {
				cout << "ENABLING AUTO- CORRECTIVE PROCEDURE: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			}
		}



		unsigned short ntries=0;
		double originalTimeOut=channelTimeoutPolicySGGet.getTimeout();

		//Withdraw this test for now; not required
		//if (ca_sg_test((*it_groupHandle).getGroupID()) == ECA_IOINPROGRESS ) {
		//    cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
		//    cafeStatus.report(ECA_IOINPROGRESS);
		//}

		//Withdraw this test for now; not required
		//while ( (ca_sg_test((*it_groupHandle).getGroupID()) == ECA_IOINPROGRESS
		//       || gStatus == ECA_TIMEOUT) && channelTimeoutPolicySGGet.getSelfGoverningTimeout()
		//                                 && ntries<channelTimeoutPolicySGGet.getNtries()
		//   ) {

		while ( (gStatus == ECA_TIMEOUT) && channelTimeoutPolicySGGet.getSelfGoverningTimeout()
										  && ntries<channelTimeoutPolicySGGet.getNtries()
			) {

			++ntries;
			//Withdraw this test for now; not required
			//if (ca_sg_test((*it_groupHandle).getGroupID()) == ECA_IOINPROGRESS ) {
			//    cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			//    cafeStatus.report(ECA_IOINPROGRESS);
			//}
			//if (gStatus == ECA_TIMEOUT) {
			//    cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			//    cafeStatus.report(ECA_TIMEOUT);
			//}

			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cout << "Changing SG GET timeout from: " << channelTimeoutPolicySGGet.getTimeout()
				 << " to: "  << (originalTimeOut + channelTimeoutPolicySGGet.getDeltaTimeout()*ntries)
				 << " seconds"  << endl;

			channelTimeoutPolicySGGet.setTimeout( (originalTimeOut +
											  channelTimeoutPolicySGGet.getDeltaTimeout()*ntries));

			if(MUTEX){cafeMutex.lock();}  //lock
			groupHandle_index.modify(it_groupHandle,
									 change_channelTimeoutPolicySGGet(channelTimeoutPolicySGGet));
			if(MUTEX){cafeMutex.unlock();}  //unlock

			if (gStatus == ECA_TIMEOUT) {
				gStatus=(*it_groupHandle).get();
			}

		} //while

		if (ntries>0) {
			cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
			cout << "Changed SG GET from intitial " << originalTimeOut << " to: "  <<
				(originalTimeOut +
				 channelTimeoutPolicySGGet.getDeltaTimeout()*ntries)
				<< " seconds after " << ntries << " sg_get calls " <<endl;
		}

		//Withdraw this test for now; not required
		//if (ca_sg_test((*it_groupHandle).getGroupID()) == ECA_IOINPROGRESS) {
		//    ca_sg_reset((*it_groupHandle).getGroupID());
		//    cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
		//    std::cout << " No. of ca_sg_test(gid) INSTIGATED: " << ntries+1 << std::endl;
		//    std::cout
		//    << " ca_sg_reset(gid) HAS BEEN FORCED SO THAT ca_sg_test() returns ECA_IODONE"
		//    <<std::endl;
		//    std::cout << " ***PLEASE** REPORT THIS OCCURRENCE TO jan.chrin@psi.ch THANKS!! "
		//    <<std::endl;
		//}

		int  * mstatus =(*it_groupHandle).getStatus();

		bool groupStatusFlag = false;
		pvgroup.statusGroup= gStatus; //ICAFE_NORMAL;
		if ( pvgroup.statusGroup==ECA_TIMEOUT) {
			groupStatusFlag = true;
		}


		for (unsigned int  iMember = 0; iMember< (*it_groupHandle).getNMember(); ++iMember) {

			if (pvgroup.pvdata[iMember].status == ECAFE_INVALID_HANDLE) {
				continue;
			 }
			it_handle = handle_index.find((*it_groupHandle).mHandle[iMember]);

			if (it_handle != handle_index.end()) {
				//do nothing as handle exists
			}
			else {
				//Should not get this far if invalid handle
				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(ECAFE_INVALID_HANDLE);
				cout <<  "Invalid handle = " << (*it_groupHandle).mHandle[iMember]
						<< " for groupMember [" << iMember << "] " << endl;
				continue;
			}

			if(MUTEX){cafeMutex.lock();}
			handle_index.modify(it_handle, change_status(mstatus[iMember]));
			if(MUTEX){cafeMutex.unlock();}

			if (mstatus[iMember]!=ICAFE_NORMAL) {
				pvgroup.pvdata[iMember].status = mstatus[iMember];

				if (pvgroup.pvdata[iMember].status != ICAFE_NORMAL
						&& pvgroup.pvdata[iMember].status != ICAFE_RULE_FALSE
							&& !groupStatusFlag) {
					pvgroup.statusGroup = pvgroup.pvdata[iMember].status;
					groupStatusFlag = true;
				}

				continue;
			}

			//cout <<  __METHOD__  << " / " << iMember << " " << nGroupMember << endl;
			(*it_handle).getPVDataHolder(pvgroup.pvdata[iMember]);
			//cout <<  __METHOD__  << " / " << mstatus[iMember] << endl;
			//check back alarms and timestamps into conduit
			//handleHelper.setSTS((*it_groupHandle).mHandle[iMember],pvgroup.pvdata[iMember].alarmStatus, \
								//pvgroup.pvdata[iMember].alarmSeverity,	pvgroup.pvdata[iMember].ts);

			if(MUTEX){cafeMutex.lock();}
			handle_index.modify(it_handle, change_alarmStatus(pvgroup.pvdata[iMember].alarmStatus));
			handle_index.modify(it_handle, change_alarmSeverity(pvgroup.pvdata[iMember].alarmSeverity));			
			handle_index.modify(it_handle, change_epicsTimeStamp(pvgroup.pvdata[iMember].ts));			
			if(MUTEX){cafeMutex.unlock();}


		} // for iMember

		return pvgroup.statusGroup;

	} else {
		return ECAFE_INVALID_GROUP_HANDLE;
	}

#undef __METHOD__
}

/**
 *  Starts a monitor on all PVs within the PVGroup object indentified by the groupHandle \n
 *  \param  groupHandle input: handle to the group object
 *  \param  statusV output: status of individual monitors
 *  \param  mpV output: vector of MonitorPolicy
 *  \return ECAFE_NORMAL if all OK else first error encountered
 */
int  CAFE::groupMonitorStart(const unsigned int  groupHandle, vector<int> &statusV,
					   vector<MonitorPolicy> &mpV){
#define __METHOD__ "groupMonitorStart(groupHandle, &statusV, &mpV)"

	// open groupHandle
	cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
	cafeGroup_set_by_groupHandle::iterator it_groupHandle;
	it_groupHandle = groupHandle_index.find(groupHandle);

	if (it_groupHandle != groupHandle_index.end()) {

		unsigned int  nGroupMember = (*it_groupHandle).getNMember();

		vector<unsigned int> handleV;
		handleV.reserve(nGroupMember);

		cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
		cafeConduit_set_by_handle::iterator it_handle;

		// fill vector of member handles
		for (unsigned int  iMember = 0; iMember < nGroupMember; ++iMember) {

			it_handle = handle_index.find((*it_groupHandle).mHandle[iMember]);

			if (it_handle != handle_index.end()) {
				handleV.push_back((*it_groupHandle).mHandle[iMember]);
			}
			else {
				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(ECAFE_INVALID_HANDLE);
				cout <<  "Invalid handle = " << (*it_groupHandle).mHandle[iMember]
						<< " for groupMember [" << iMember << "] " << endl;
				continue;
			}
		}
		// issue monitorStart command
		return monitorStart(handleV, statusV, mpV);
	}
	else {
		return ECAFE_INVALID_GROUP_HANDLE;
	}

#undef __METHOD__
}


/**
 *  Starts a monitor on all PVs within the PVGroup object indentified by the groupHandle \n
 *  \param  groupHandle input: handle to the group object
 *  \param  statusV output: status of individual monitors
 *  \param  monitorIDV output: vector of monitor ID (unsigned int)
 *  \return ECAFE_NORMAL if all OK else first error encountered
 */
int  CAFE::groupMonitorStart(const unsigned int  groupHandle, vector<int> &statusV,
					   vector<unsigned int> &monitorIDV) {
#define __METHOD__ "groupMonitorStart(groupHandle, &statusV, &monitorIDV)"

	// open groupHandle
	cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
	cafeGroup_set_by_groupHandle::iterator it_groupHandle;
	it_groupHandle = groupHandle_index.find(groupHandle);

	if (it_groupHandle != groupHandle_index.end()) {

		unsigned int  nGroupMember = (*it_groupHandle).getNMember();

		vector<unsigned int> handleV;
		handleV.reserve(nGroupMember);

		cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
		cafeConduit_set_by_handle::iterator it_handle;

		// fill vector of member handles
		for (unsigned int  iMember = 0; iMember < nGroupMember; ++iMember) {

			it_handle = handle_index.find((*it_groupHandle).mHandle[iMember]);

			if (it_handle != handle_index.end()) {
				handleV.push_back((*it_groupHandle).mHandle[iMember]);
			}
			else {
				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(ECAFE_INVALID_HANDLE);
				cout <<  "Invalid handle = " << (*it_groupHandle).mHandle[iMember]
						<< " for groupMember [" << iMember << "] " << endl;
				continue;
			}
		}

		// issue monitorStart command
		return monitorStart(handleV, statusV, monitorIDV);

	}
	else {
		return ECAFE_INVALID_GROUP_HANDLE;
	}
#undef __METHOD__
}


/**
 *  Stops a monitor on all PVs within the PVGroup object indentified by the groupHandle \n
 *  \param  groupHandle input: handle to the group object
 *  \param  statusV output: status of individual monitor stop operations
 *  \return ECAFE_NORMAL if all OK else first error encountered
 */
int  CAFE::groupMonitorStop (const unsigned int  groupHandle, vector<int> &statusV) {
#define __METHOD__ "groupMonitorStop(groupHandle, &statusV)"

	// open groupHandle
	cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
	cafeGroup_set_by_groupHandle::iterator it_groupHandle;
	it_groupHandle = groupHandle_index.find(groupHandle);

	if (it_groupHandle != groupHandle_index.end()) {

		unsigned int  nGroupMember = (*it_groupHandle).getNMember();

		vector<unsigned int> handleV;
		handleV.reserve(nGroupMember);

		cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
		cafeConduit_set_by_handle::iterator it_handle;

		// fill vector of member handles
		for (unsigned int  iMember = 0; iMember < nGroupMember; ++iMember) {

			it_handle = handle_index.find((*it_groupHandle).mHandle[iMember]);

			if (it_handle != handle_index.end()) {
				handleV.push_back((*it_groupHandle).mHandle[iMember]);
			}
			else {
				cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
				cafeStatus.report(ECAFE_INVALID_HANDLE);
				cout <<  "Invalid handle = " << (*it_groupHandle).mHandle[iMember]
						<< " for groupMember [" << iMember << "] " << endl;
				continue;
			}
		}

		// issue monitorStart command
		return monitorStop(handleV, statusV);
	}
	else {
		return ECAFE_INVALID_GROUP_HANDLE;
	}

#undef __METHOD__
}
