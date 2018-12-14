///
/// \file    conduitFriends.h
/// \author  Jan Chrin, PSI
/// \date    November 2014
/// \version CAFE 1.0.0
///

#ifndef CONDUITFRIENDS_H
#define CONDUITFRIENDS_H

#include <cstdio>
#include <conduit.h>
#include <statusCodes.h>

/**
 *   Friend to Conduit/CAFEGroup permitting fast modification to STS recorded in hash table
 */
struct change_alarmStatus {
    change_alarmStatus (const dbr_short_t  & new_alarmStatus): new_alarmStatus(new_alarmStatus) {}

    void operator() (Conduit& c)
    {
        ///////////////////////////////////////////////////////////////////////////
        c.alarmStatus =  new_alarmStatus;
        ///////////////////////////////////////////////////////////////////////////
    }

private:
    dbr_short_t  new_alarmStatus;
};
struct change_alarmSeverity {
    change_alarmSeverity (const dbr_short_t  & new_alarmSeverity): new_alarmSeverity(new_alarmSeverity) {}

    void operator() (Conduit& c)
    {
        ///////////////////////////////////////////////////////////////////////////
        c.alarmSeverity =  new_alarmSeverity;
        ///////////////////////////////////////////////////////////////////////////
    }

private:
    dbr_short_t  new_alarmSeverity;
};
struct change_epicsTimeStamp {
    change_epicsTimeStamp (const epicsTimeStamp  & new_epicsTimeStamp): new_epicsTimeStamp(new_epicsTimeStamp) {}

    void operator() (Conduit& c)
    {
        ///////////////////////////////////////////////////////////////////////////
        c.ts =  new_epicsTimeStamp;
        //Do this to prevent overflow error in epicsTime time(ts) routines!
        //This bad number can occur in timeouts
        if(c.ts.nsec>1000000000) {
            c.ts.nsec=0;
        }
        ///////////////////////////////////////////////////////////////////////////
    }

private:
    epicsTimeStamp  new_epicsTimeStamp;
};


/**
 *   Friend to Conduit/CAFEGroup permitting fast modification to access rights state recorded in hash table
 */
struct change_accessRead {
    change_accessRead (const unsigned int  & new_accessRead): new_accessRead(new_accessRead) {}

    void operator() (Conduit& c)
    {
        ///////////////////////////////////////////////////////////////////////////
        c.channelRegalia.accessRead =  new_accessRead;
        ///////////////////////////////////////////////////////////////////////////
    }

private:
    unsigned int  new_accessRead;
};


/**
 *   Friend to Conduit/CAFEGroup permitting fast modification to access rights state recorded in hash table
 */
struct change_accessWrite {
    change_accessWrite (const unsigned int  & new_accessWrite): new_accessWrite(new_accessWrite) {}

    void operator() (Conduit& c)
    {
        ///////////////////////////////////////////////////////////////////////////
        c.channelRegalia.accessWrite =  new_accessWrite;
        ///////////////////////////////////////////////////////////////////////////
    }

private:
    unsigned int  new_accessWrite;
};


/**
 *   Friend toConduit/CAFEGroup permitting the access_rights_handler_args struct from callback fns
 *   to be recorded in hash table
 */
struct change_accessRightsHandlerArgs {
    change_accessRightsHandlerArgs (const struct access_rights_handler_args & new_accessRightsHandlerArgs):
        new_accessRightsHandlerArgs(new_accessRightsHandlerArgs) {}

    void operator() (Conduit& c)
    {

        ///////////////////////////////////////////////////////////////////////////
        c.channelRegalia.accessRead =  new_accessRightsHandlerArgs.ar.read_access;
        c.channelRegalia.accessWrite=  new_accessRightsHandlerArgs.ar.write_access;
        ///////////////////////////////////////////////////////////////////////////
    }

private:
    access_rights_handler_args new_accessRightsHandlerArgs;
};


/**
 *   Friend to Conduit permitting pulse_id f to be set in hash table
 */
struct change_beamEventNo {
    change_beamEventNo (const unsigned long long  & new_beamEventNo): new_beamEventNo(new_beamEventNo) {}

    void operator() (Conduit& c)
    {
        c.beamEventNo =  new_beamEventNo;
    }

private:
    unsigned long long new_beamEventNo;
};

/**
 *   Friend to Conduit/CAFEGroup records the channelRegalia class members in hash table
 */
struct change_channelDeviceAttribute {
    change_channelDeviceAttribute (const ChannelDeviceAttribute  & new_channelDeviceAttribute):
        new_channelDeviceAttribute(new_channelDeviceAttribute) {}

    void operator() (Conduit& c)
    {
        c.channelDeviceAttribute =  new_channelDeviceAttribute;
    }

private:
    ChannelDeviceAttribute new_channelDeviceAttribute;
};



/**
 *   Friend to Conduit/CAFEGroup records the channelID in hash table
 */
struct change_channelID {
    change_channelID (const chid & new_channelID): new_channelID(new_channelID) {}

    void operator() (Conduit& c)
    {
        c.channelRegalia.channelID=new_channelID;
        c.channelID=new_channelID;
    }

private:
    chid new_channelID;
};



/**
 *   Friend to Conduit records the channelGetActionWhenMonitorPolicy in hash table
 */
struct change_channelGetActionWhenMonitorPolicy {
    change_channelGetActionWhenMonitorPolicy (
        const ChannelGetActionWhenMonitorPolicy & new_channelGetActionWhenMonitorPolicy):
        new_channelGetActionWhenMonitorPolicy(new_channelGetActionWhenMonitorPolicy) {}

    void operator() (Conduit& c)
    {
        //
        c.channelGetActionWhenMonitorPolicy =  new_channelGetActionWhenMonitorPolicy;
    }
private:
    ChannelGetActionWhenMonitorPolicy new_channelGetActionWhenMonitorPolicy;
};


/**
 *   Friend to Conduit records the channelGetCacheWaitPolicy in hash table
 */
struct change_channelGetCacheWaitPolicy {
    change_channelGetCacheWaitPolicy (
        const ChannelGetCacheWaitPolicy & new_channelGetCacheWaitPolicy):
        new_channelGetCacheWaitPolicy(new_channelGetCacheWaitPolicy) {}

    void operator() (Conduit& c)
    {
        //
        c.channelGetCacheWaitPolicy =  new_channelGetCacheWaitPolicy;
    }
private:
    ChannelGetCacheWaitPolicy new_channelGetCacheWaitPolicy;
};





/**
 *   Friend to Conduit permitting hasNewData flag to be set in hash table
 */
struct change_hasNewData {
    change_hasNewData (const bool  & new_hasNewData): new_hasNewData(new_hasNewData) {}

    void operator() (Conduit& c)
    {
        c.hasNewData =  new_hasNewData;
    }

private:
    bool new_hasNewData;
};



/**
 *   Friend to Conduit/CAFEGroup records the channelRegalia class members in hash table
 */
struct change_channelRegalia {
    change_channelRegalia (const ChannelRegalia  & new_channelRegalia): new_channelRegalia(new_channelRegalia) {}

    void operator() (Conduit& c)
    {
        c.channelRegalia =  new_channelRegalia;
    }

private:
    ChannelRegalia new_channelRegalia;
};


/**
 *   Friend to Conduit records the channelRequestDataTypePolicy in hash table
 */
struct change_channelRequestDataTypePolicy {
    change_channelRequestDataTypePolicy (
        const ChannelRequestDataTypePolicy & new_channelRequestDataTypePolicy):
        new_channelRequestDataTypePolicy(new_channelRequestDataTypePolicy) {}

    void operator() (Conduit& c)
    {
        //
        c.channelRequestDataTypePolicy =  new_channelRequestDataTypePolicy;
    }
private:
    ChannelRequestDataTypePolicy new_channelRequestDataTypePolicy;
};



/**
 *   Friend to Conduit records the channelRequestMetaCtrl in hash table
 */
struct change_channelRequestMetaCtrl {
    change_channelRequestMetaCtrl (const ChannelRequestMetaData & new_channelData):
        new_channelData(new_channelData) {}

    void operator() (Conduit& c)
    {
        //
        c.channelRequestMetaCtrl =  new_channelData;
    }
private:
    ChannelRequestMetaData new_channelData;
};



/**
 *   Friend to Conduit records the channelRequestMetaCtrlClient in hash table
 */
struct change_channelRequestMetaCtrlClient {
    change_channelRequestMetaCtrlClient (const ChannelRequestMetaDataClient & new_channelData):
        new_channelData(new_channelData) {}

    void operator() (Conduit& c)
    {
        //
        c.channelRequestMetaCtrlClient =  new_channelData;

    }
private:
    ChannelRequestMetaDataClient new_channelData;
};





/**
 *   Friend to Conduit records the channelRequestMetaData in hash table
 */
struct change_channelRequestMetaData {
    change_channelRequestMetaData (const ChannelRequestMetaData & new_channelData):
        new_channelData(new_channelData) {}

    void operator() (Conduit& c)
    {
        //

        c.channelRequestMetaData =  new_channelData;

    }

private:
    ChannelRequestMetaData new_channelData;
};



/**
 *   Friend to Conduit records the channelRequestMetaDataClient in hash table
 */
struct change_channelRequestMetaDataClient {
    change_channelRequestMetaDataClient (const ChannelRequestMetaDataClient & new_channelData):
        new_channelData(new_channelData) {}

    void operator() (Conduit& c)
    {
        //

        c.channelRequestMetaDataClient =  new_channelData;
    }
private:
    ChannelRequestMetaDataClient new_channelData;
};



/**
 *   Friend to Conduit records the channelRequestMetaPrimitive in hash table
 */
struct change_channelRequestMetaPrimitive {
    change_channelRequestMetaPrimitive (const ChannelRequestMetaData & new_channelData):
        new_channelData(new_channelData) {}

    void operator() (Conduit& c)
    {
        //
        c.channelRequestMetaPrimitive =  new_channelData;
    }
private:
    ChannelRequestMetaData new_channelData;
};


/**
 *   Friend to Conduit records the channelRequestPolicyGet in hash table
 */
struct change_channelRequestPolicyGet {
    change_channelRequestPolicyGet (const ChannelRequestPolicy & new_ChannelRequestPolicy):
        new_ChannelRequestPolicy(new_ChannelRequestPolicy) {}

    void operator() (Conduit& c)
    {
        //This does a deep copy!
        c.channelRequestPolicyGet =  new_ChannelRequestPolicy;
    }
private:
    ChannelRequestPolicy new_ChannelRequestPolicy;
};


/**
 *   Friend to Conduit records the channelRequestPolicyGetCtrl in hash table
 */
struct change_channelRequestPolicyGetCtrl {
    change_channelRequestPolicyGetCtrl (const ChannelRequestPolicy & new_ChannelRequestPolicy):
        new_ChannelRequestPolicy(new_ChannelRequestPolicy) {}

    void operator() (Conduit& c)
    {
        //This does a deep copy!
        c.channelRequestPolicyGetCtrl =  new_ChannelRequestPolicy;
    }
private:
    ChannelRequestPolicy new_ChannelRequestPolicy;
};



/**
 *   Friend to Conduit records the channelRequestPolicyPut in hash table
 */
struct change_channelRequestPolicyPut {
    change_channelRequestPolicyPut (const ChannelRequestPolicy & new_ChannelRequestPolicy):
        new_ChannelRequestPolicy(new_ChannelRequestPolicy) {}

    void operator() (Conduit& c)
    {
        //This does a deep copy!
        c.channelRequestPolicyPut =  new_ChannelRequestPolicy;
    }
private:
    ChannelRequestPolicy new_ChannelRequestPolicy;
};




/**
 *   Friend to Conduit records the channelRequestStatusGet in hash table
 */
struct change_channelRequestStatusGet {
    change_channelRequestStatusGet (const ChannelRequestStatus & new_ChannelRequestStatus):
        new_ChannelRequestStatus(new_ChannelRequestStatus) {}

    void operator() (Conduit& c)
    {
        //This does a deep copy!

        c.channelRequestStatusGet =  new_ChannelRequestStatus;
    }

private:
    ChannelRequestStatus new_ChannelRequestStatus;
};



/**
 *   Friend to Conduit records the channelRequestStatusGetCtrl in hash table
 */
struct change_channelRequestStatusGetCtrl {
    change_channelRequestStatusGetCtrl (const ChannelRequestStatus & new_ChannelRequestStatus):
        new_ChannelRequestStatus(new_ChannelRequestStatus) {}

    void operator() (Conduit& c)
    {
        //This does a deep copy!
        c.channelRequestStatusGetCtrl =  new_ChannelRequestStatus;
    }
private:
    ChannelRequestStatus new_ChannelRequestStatus;
};


/**
 *   Friend to Conduit records the channelRequestStatusGetSTSACK in hash table
 */
struct change_channelRequestStatusGetSTSACK {
    change_channelRequestStatusGetSTSACK (const ChannelRequestStatus & new_ChannelRequestStatus):
        new_ChannelRequestStatus(new_ChannelRequestStatus) {}

    void operator() (Conduit& c)
    {
        //This does a deep copy!
        c.channelRequestStatusGetSTSACK =  new_ChannelRequestStatus;
    }
private:
    ChannelRequestStatus new_ChannelRequestStatus;
};



/**
 *   Friend to Conduit records the channelRequestStatusGetClassName in hash table
 */
struct change_channelRequestStatusGetClassName {
    change_channelRequestStatusGetClassName (const ChannelRequestStatus & new_ChannelRequestStatus):
        new_ChannelRequestStatus(new_ChannelRequestStatus) {}

    void operator() (Conduit& c)
    {
        //This does a deep copy!
        c.channelRequestStatusGetClassName  =  new_ChannelRequestStatus;
    }
private:
    ChannelRequestStatus new_ChannelRequestStatus;
};


/**
 *   Friend to Conduit records the channelRequestStatusPut in hash table
 */
struct change_channelRequestStatusPut {
    change_channelRequestStatusPut (const ChannelRequestStatus & new_ChannelRequestStatus):
        new_ChannelRequestStatus(new_ChannelRequestStatus) {}

    void operator() (Conduit& c)
    {
        //This does a deep copy!
        c.channelRequestStatusPut =  new_ChannelRequestStatus;
    }
private:
    ChannelRequestStatus new_ChannelRequestStatus;
};




/**
 *   Friend to Conduit records the channelTimeoutPolicyGet in hash table
 */
struct change_channelTimeoutPolicyGet {
    change_channelTimeoutPolicyGet (const ChannelTimeoutPolicy & new_channelTimeoutPolicy):
        new_channelTimeoutPolicy(new_channelTimeoutPolicy) {}

    void operator() (Conduit& c)
    {
        //This does a deep copy!
        c.channelTimeoutPolicyGet =  new_channelTimeoutPolicy;
    }
private:
    ChannelTimeoutPolicy new_channelTimeoutPolicy;
};


/**
 *   Friend to Conduit records the channelTimeoutPolicyPut in hash table
 */
struct change_channelTimeoutPolicyPut {
    change_channelTimeoutPolicyPut (const ChannelTimeoutPolicy & new_channelTimeoutPolicy):
        new_channelTimeoutPolicy(new_channelTimeoutPolicy) {}

    void operator() (Conduit& c)
    {
        //This does a deep copy!
        c.channelTimeoutPolicyPut =  new_channelTimeoutPolicy;
    }
private:
    ChannelTimeoutPolicy new_channelTimeoutPolicy;
};



/**
 *   Friend to Conduit permitting pvdata to be entered in queue
 */
struct change_dequePulseID {
    change_dequePulseID (const PVDataHolder  & new_pvd): new_pvd(new_pvd) {}

    void operator() (Conduit& c)
    {



        try {
            c.dequePulseID.push_back(new_pvd);

            if (c.dequePulseID.size() > c.mapPulseIDBufferSize) {
                c.dequePulseID.pop_front();
            }

        }
        catch (std::bad_alloc &e) {

            std::cout << "change_dequeuPulseID" << " //" << e.what() << std:: endl;
            exit(1);

        }



    }

private:
    PVDataHolder new_pvd;
};



/**
 *   Friend to Conduit permitting pulse_id/pvdata to be set in hash table
 */
struct change_mapPulseID {
    change_mapPulseID (const PVDataHolder  & new_pvd): new_pvd(new_pvd) {}

    void operator() (Conduit& c)
    {

        try {

            //std::pair<unsigned long long, PVDataHolder> p = std::make_pair(new_pvd.getPulseID(), new_pvd);

            //Check if make_pair is a success

            c.mapPulseID.insert(std::make_pair(new_pvd.getPulseID(), new_pvd));


            if (c.mapPulseID.size() > c.mapPulseIDBufferSize) {

                std::map<unsigned long long, PVDataHolder>::iterator pos=c.mapPulseID.begin();
                if (pos != c.mapPulseID.end()) {
                    c.mapPulseID.erase(pos);
                }
                else {
                    std::cout << "ITERATOR NOT FOUND in change_mapPulseID" << std::endl;
                }
            }

        }
        catch (std::bad_alloc &e) {

            std::cout << "change_mapPulseID" << " //" << e.what() << std:: endl;
            exit(1);

        }

    }

private:
    PVDataHolder new_pvd;
};




/**
 *   Friend to Conduit permitting size of map<pulseID, pvdata> size to be set;
 *   if new buffer size if smaller than previous, then erase excess elements
 */
struct change_mapPulseIDBufferSize {
    change_mapPulseIDBufferSize (const unsigned short  & new_mapPulseIDBufferSize): new_mapPulseIDBufferSize(new_mapPulseIDBufferSize) {}

    void operator() (Conduit& c)
    {
        ipos=0;
        if (c.mapPulseIDBufferSize > new_mapPulseIDBufferSize) {
            std::map<unsigned long long, PVDataHolder>::iterator pos;
            for (pos=c.mapPulseID.begin();  pos != c.mapPulseID.end();) {
                ++ipos;
                if (ipos > new_mapPulseIDBufferSize) {
                    c.mapPulseID.erase(pos++);
                }
                else {
                    ++pos;
                }
            }
        }
        c.mapPulseIDBufferSize=new_mapPulseIDBufferSize;
    }

private:
    unsigned short new_mapPulseIDBufferSize;
    unsigned short ipos;
};


/**
 *   Friend to Conduit permitting fast modification to std::vector<std::string> monitorAction
 *   (for CAFE Extensions)
 */
struct change_monitorAction {
    change_monitorAction (std::string  &new_monitorAction): new_monitorAction(new_monitorAction) {}

    void operator() (Conduit& c)
    {
        c.monitorAction.push_back(new_monitorAction);
    }
private:
    std::string new_monitorAction;
};



/**
 *   Friend to Conduit permitting fast removal of all elements in std::vector<std::string> monitorAction
 *   (for CAFE Extensions)
 */
struct change_monitorActionClear {
    change_monitorActionClear () {}

    void operator() (Conduit& c)
    {
        c.monitorAction.clear();
    }

};



/**
 *   Friend to Conduit permitting fast removal of an entry in std::vector<std::string> monitorAction
 *   (for CAFE Extensions)
 */
struct change_monitorActionErase {
    change_monitorActionErase (std::string  &new_monitorAction): new_monitorAction(new_monitorAction) {}

    void operator() (Conduit& c)
    {
        bool maFound=false;
        std::vector<std::string>::iterator it;


        for (it = c.monitorAction.begin(); it != c.monitorAction.end(); ) {
            if( (*it)==new_monitorAction) {
                it = c.monitorAction.erase(it);
                maFound=true;
            }
            else {
                ++it;
            }
        }

        if (!maFound) {
            std::cout << "monitorAction " << new_monitorAction << " NOT FOUND! " << std::endl;
            std::cout << "Could not delete entry!" << std::endl;
        }

    }
private:
    std::string new_monitorAction;
};


/**
 *   Friend to Conduit permitting fast removal of an entry in the  monitorPolicy vector
 *
 */
struct change_monitorPolicyErase {
    change_monitorPolicyErase (unsigned int  & new_evid): new_evid(new_evid) {}

    void operator() (Conduit& c)
    {

        bool evidFound=false;
        std::vector<MonitorPolicy>::iterator it;
        //Iterate
        for (it = c.mpV.begin(); it != c.mpV.end();) {
            //std::cout << "ID " << (*it).getID() << " " << std::endl;

            if ( (*it).getID()==new_evid) {
                evidFound=true;
                it=(c.mpV).erase(it);
                break;
            }
            else {
                ++it;
            }
        }

        if (!evidFound) {
            std::cout << "evid " << new_evid << " NOT FOUND! " << std::endl;
            std::cout << "Could not delete entry!" << std::endl;
        }
    }

private:
    unsigned int  new_evid;
};




/**
 *   Friend to Conduit permitting fast insertion into the monitorPolicy vector
 *
 */
struct change_monitorPolicyInsert {
    change_monitorPolicyInsert (class MonitorPolicy & new_monitorPolicy): new_monitorPolicy(new_monitorPolicy) {}

    void operator() (Conduit& c)
    {


        //insert into mpV
        (c.mpV).push_back(new_monitorPolicy);

        if (c.mpV.size()>(MAX_NO_MONITORS_PER_CHANNEL/2)) {
            std::cout << "HEY DUDE - YOU NOW HAVE " << c.mpV.size() << " MONITORS " << std::endl;
            std::cout << "for channel " << c.pv << " with handle " << c.handle << std::endl;
        }


    }

private:
    class MonitorPolicy new_monitorPolicy;
};



/**
 *   Friend to Conduit permitting fast removal of an entry in the  monitorPolicyInWaiting vector
 *
 */
struct change_monitorPolicyInWaitingErase {
    change_monitorPolicyInWaitingErase (unsigned int  & new_evid): new_evid(new_evid) {}

    void operator() (Conduit& c)
    {

        bool evidFound=false;
        std::vector<MonitorPolicy>::iterator it;
        //Iterate
        for (it = c.mpInWaitingV.begin(); it != c.mpInWaitingV.end(); ++it) {

            if ( (*it).getID()==new_evid) {
                evidFound=true;
                (c.mpInWaitingV).erase(it);
                break;
            }
        }

        if (!evidFound) {
            std::cout << "evid " << new_evid << " NOT FOUND! " << std::endl;
            std::cout << "Could not delete entry!" << std::endl;
        }
    }

private:
    unsigned int  new_evid;
};



/**
 *   Friend to Conduit permitting fast insertion into the monitorPolicyInWaiting vector
 *
 */
struct change_monitorPolicyInWaitingInsert {
    change_monitorPolicyInWaitingInsert (class MonitorPolicy & new_monitorPolicy): new_monitorPolicy(new_monitorPolicy) {}

    void operator() (Conduit& c)
    {

        //insert into mpnWaitingV
        (c.mpInWaitingV).push_back(new_monitorPolicy);

        if (c.mpInWaitingV.size()>6) {
            std::cout << "HEY DUDE - YOU NOW HAVE " << c.mpInWaitingV.size()
                      << " MONITORS IN WAITING" << std::endl;
            std::cout << "for channel " << c.pv << " with handle " << c.handle << std::endl;
        }
    }

private:
    class MonitorPolicy new_monitorPolicy;
};








/**
 *   Friend to Conduit/CAFEGroup permitting fast modification to pvAlias in hash table
 */
struct change_pvAlias {
    change_pvAlias (const char * & new_pvAlias): new_pvAlias(new_pvAlias) {}

    void operator() (Conduit& c)
    {
        c.pvAlias =  new_pvAlias;
        //No longer supporting c,pvd
        ///strcpy(c.pvd.pvAlias, new_pvAlias);
        //strcpy(c.pvc.pvAlias, new_pvAlias);
    }

private:
    const char * new_pvAlias;
};




/**
 *   Friend to Conduit permitting pyCafeFlag flag to be set in hash table
 *
struct change_pyCafeFlag
{
        change_pyCafeFlag (const bool  & new_pyCafeFlag): new_pyCafeFlag(new_pyCafeFlag){}

        void operator() (Conduit& c) {c.pyCafeFlag =  new_pyCafeFlag;}

private:
        bool new_pyCafeFlag;
};
*/


/**
 *   Friend to Conduit/CAFEGroup permitting fast modification to the status as given by the last method invocation
 */
struct change_status {
    change_status (const int  & new_status): new_status(new_status) {}
    void operator() (Conduit& c)
    {
        c.status =  new_status;
    }

private:
    int  new_status;
};





/**
 *   Friend to Conduit permitting associated alarm Severity levels together with channel description to be entered into container
 */
struct change_supplementHandle {
    change_supplementHandle (const alarmSeverityStruct  & new_alarmSeverity, const std::string & new_desc): new_alarmSeverity(new_alarmSeverity), new_desc(new_desc) {}

    void operator() (Conduit& c)
    {

      c.aSevStruct.hhsv=new_alarmSeverity.hhsv;
      c.aSevStruct.hsv=new_alarmSeverity.hsv;
			c.aSevStruct.lsv=new_alarmSeverity.lsv;
      c.aSevStruct.llsv=new_alarmSeverity.llsv;
			c.desc=new_desc;
			c.hasDesc=true;
			c.hasAlarmSevStruct=true;
			 
    }

private:
    alarmSeverityStruct new_alarmSeverity;
		std::string new_desc;
};






/**
 *   Friend to Conduit/CAFEGroup permitting fast modification to the usrArgs as given by the last method invocation
 */
struct change_usrArgs {
    change_usrArgs (const unsigned int  & new_usrArgs): new_usrArgs(new_usrArgs) {}
    void operator() (Conduit& c)
    {
        c.usrArgs = (void *) (unsigned long) new_usrArgs;   // APRIL2018 - added to remove C++11 compiler warning
    }


private:
    unsigned int  new_usrArgs;
};


/**
 *   friend to Conduit/CAFEGroup permitting the groupHandle to which the channel belongs to be recorded in hash table
 */
struct change_groupHandle {
    change_groupHandle (const unsigned int  & new_groupHandle): new_groupHandle(new_groupHandle) {}

    void operator() (Conduit& c)
    {
        c.groupHandle =  new_groupHandle;
    }

private:
    unsigned int  new_groupHandle;
};



#endif // CONDUITFRIENDS_H
