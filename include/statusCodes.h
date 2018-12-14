///
/// \file statusCodes.h
///
/// class StatusInfo - contains user defined messages (>ICAFE_STATUS_BASE)
///                    and ca_message() output string
///
/// class StatusMap  - contains ECA and ECAFE error strings plus ICAFE info strings
///
/// class Status     - contains both above classes by composition
///
/// \author  Jan Chrin, PSI
/// \date    Release: Feb. 2015
///          Revised: May  2017
///          Added CAFEGlobalAlarmStatus and CAFEGlobalAlarmSeverity
/// \version CAFE 1.1.0
///
/// JC, Dec. 2015, additional methods to CAFEStatus
///


#ifndef STATUSCODES_H
#define STATUSCODES_H
#include <cadef.h>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>

#include <cafeDataType.h>


const unsigned short ICAFE_STATUS_BASE  =  600;
const unsigned short ICAFE_STATUS_CS    = ICAFE_STATUS_BASE;
const unsigned short ICAFE_STATUS_CFT   =  700; // ca_field_type
const unsigned short ICAFE_STATUS_CA_OP =  800;
const unsigned short ICAFE_STATUS_ERROR = 1000;
const unsigned short ICAFE_FILE_ERROR   = 1100;
const unsigned short ICAFE_SERVICE_ERROR    =1200;
const unsigned short ICAFE_BITSHUFFLE_ERROR =1300;
const unsigned short ICAFE_BSREAD_BASE  = 1500;
const unsigned short ICAFE_ERRNO_BASE   = 5000;

const unsigned short ICAFE_SUCCESS      = ECA_NORMAL;
const unsigned short ICAFE_NORMAL       = ECA_NORMAL;



enum CAFE_CS_STATE {    ICAFE_CS_NEVER_CONN=ICAFE_STATUS_CS,
                        ICAFE_CS_PREV_CONN,
                        ICAFE_CS_CONN,
                        ICAFE_CS_CLOSED,
                        ICAFE_CS_DISCONN,
                        ICAFE_CS_UNKNOWN
                   };

enum CAFE_CFT_STATE {    ICAFE_TYPENOTCONN=ICAFE_STATUS_CFT,
                         ICAFE_RULE_FALSE,
                         ICAFE_BADCOUNT,
                         ICAFE_CALLBACK_NOT_YET_INVOKED,
                         ICAFE_WAITING_FOR_PREV_CALLBACK,
                         ICAFE_CACHE_EMPTY,
                         ICAFE_CHANNEL_BLOCKING_POLICY_CONFLICT,
                         ICAFE_MONITOR_DELAYED_AS_CONN_DOWN,
                         ICAFE_HAS_MONITOR_GET_DONE_FROM_CACHE,
                         ICAFE_SET_AND_GET_MISMATCH
                    };

enum CAFE_CC_STATE {    ICAFE_CA_OP_GET=ICAFE_STATUS_CA_OP,
                        ICAFE_CA_OP_PUT,
                        ICAFE_CA_OP_CREATE_CHANNEL,
                        ICAFE_CA_OP_ADD_EVENT,
                        ICAFE_CA_OP_CLEAR_EVENT,
                        ICAFE_CA_OP_OTHER,
                        ICAFE_CA_OP_CONN_UP,
                        ICAFE_CA_OP_CONN_DOWN
                   };

enum CAFE_ERROR_STATE { ECAFE_NODATA=ICAFE_STATUS_ERROR,
                        ECAFE_INVALID_TYPE,
                        ECAFE_BADCOUNT,
                        ECAFE_BADSTR,
                        ECAFE_BADTYPE,
                        ECAFE_NO_CONVERT,
                        ECAFE_NULLCONTEXT,
                        ECAFE_NULLCHID,
                        ECAFE_NULLEVID,
                        ECAFE_UNKNOWN_COLLECTION,
                        ECAFE_EMPTY_COLLECTION,
                        ECAFE_COLLECTION_PREV_DEF,
                        ECAFE_COLLECTION_INVALID_MEMBER,
                        ECAFE_RULE_FALSE,
                        ECAFE_UNKNOWN_GROUP,
                        ECAFE_EMPTY_GROUP,
                        ECAFE_GROUP_PREV_DEF,
                        ECAFE_INVALID_HANDLE,
                        ECAFE_INVALID_GROUP_HANDLE,
                        ECAFE_NORDACCESS,
                        ECAFE_NOWTACCESS,
                        ECAFE_TIMEOUT,
                        ECAFE_CANNOT_OPEN_FILE,
                        ECAFE_INVALID_SWITCH_CASE,
                        ECAFE_PVALIAS_PREV_DEF,
                        ECAFE_PVALIAS_INVALID,
                        ECAFE_PVNAME_PREV_DEF_AS_PVALIAS,
                        ECAFE_DEVICE_ATTRIB_NOT_FOUND,
                        ECAFE_HASH_UNIQUEID_EXISTS,
                        ECAFE_WRONG_CA_CONTEXT,
                        ECAFE_INVALID_CAFENUM_POLICY_TYPE,
                        ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED,
                        ECAFE_INVALID_ENUM_INDEX,
                        ECAFE_PVGROUP_GROUPHANDLE_MISMATCH,
                        ECAFE_TIMEOUT_SET_AND_MATCH,
                        ECAFE_HANDLE_MISMATCH_SET_AND_MATCH,
                        ECAFE_INCONSISTENT_CONTAINER_CORRECTED
                      };

//1100
enum CAFE_FILE_ERROR  { ECAFE_LOAD_COLLECTION=ICAFE_FILE_ERROR,
                        ECAFE_LOAD_GROUP
                      };

//1200
enum CAFE_SERVICE_ERROR { ECAFE_BPM_DATA_IS_INVALID=ICAFE_SERVICE_ERROR
                        };
//1300
enum CAFE_BITSHUFFLE_ERROR { ECAFE_BITSHUFF_DECOMPRESS=ICAFE_BITSHUFFLE_ERROR,
                             ECAFE_BITSHUFF_ALLOCMEM,
                             ECAFE_BITSHUFF_REALLOCMEM,
                             ECAFE_BITSHUFF_BADCOUNT
                           };

enum CAFE_BSREAD_ERROR {ECAFE_BSREAD_MULTIPART_MESS_NODATA=ICAFE_BSREAD_BASE,
                        ECAFE_BSREAD_PARSEFAIL_MAINHEADER,
                        ECAFE_BSREAD_PARSEFAIL_DATAHEADER,
                        ECAFE_BSREAD_ZMQSTREAM_NULL
                       };

//5000
//Used by zeromq
enum CAFE_ERRNO_ERROR { ERRNO_EINTR =ICAFE_ERRNO_BASE+4,
                        ERRNO_EAGAIN=ICAFE_ERRNO_BASE+11,
                        ERRNO_EFAULT=ICAFE_ERRNO_BASE+14,
                        ERRNO_EINVAL=ICAFE_ERRNO_BASE+22,
                        ERRNO_ENOTSOCK=ICAFE_ERRNO_BASE+88,
                        ERRNO_EPROTONOSUPPORT=ICAFE_ERRNO_BASE+93
                      };


//epicsAlarmConditionStrings = {"NO_ALARM","READ","WRITE","HIHI","HIGH",
//"LOLO","LOW","STATE","COS", "COMM","TIMEOUT","HWLIMIT","CALC","SCAN","LINK",
//"SOFT","BAD_SUB","UDF","DISABLE","SIMM","READ_ACCESS", "WRITE_ACCESS"};


enum EPICS_GLOBAL_ALARM_CONDITION {STAT_NO_ALARM=0, STAT_READ, STAT_WRITE, STAT_HIHI, STAT_HIGH,
                                   STAT_LOLO, STAT_LOW, STAT_STATE, STAT_COS, STAT_COMM, STAT_TIMEOUT, STAT_HWLIMIT,
                                   STAT_CALC, STAT_SCAN, STAT_LINK,
                                   STAT_SOFT, STAT_BAD_SUB, STAT_UDF, STAT_DISABLE, STAT_SIMM, STAT_READ_ACCESS, STAT_WRITE_ACCESS
                                  };

enum EPICS_GLOBAL_ALARM_SEVERITY {SEV_NO_ALARM=0, SEV_MINOR, SEV_MAJOR, SEV_INVALID};


class CAFEGlobalAlarmCondition {
    typedef std::map<int, std::string> mapIntString;

private:
    mapIntString mapAlarmCondition;
    mapIntString::iterator pos;

public:
    CAFEGlobalAlarmCondition()
    {
        mapAlarmCondition.insert(std::make_pair((int) STAT_NO_ALARM, "NO_ALARM"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_READ,     "READ"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_WRITE,    "WRITE"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_HIHI,     "HIHI"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_HIGH,     "HIGH"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_LOLO,     "LOLO"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_LOW,      "LOW"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_STATE,    "STATE"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_COS,      "COS"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_COMM,     "COMM"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_TIMEOUT,  "TIMEOUT"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_HWLIMIT,  "HWLIMIT"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_CALC,     "CALC"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_SCAN,     "SCAN"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_LINK,     "LINK"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_SOFT,     "SOFT"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_BAD_SUB,  "BAD_SUB"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_UDF,      "UDF"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_DISABLE,  "DISABLE"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_SIMM,     "SIMM"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_READ_ACCESS,   "READ_ACCESS"));
        mapAlarmCondition.insert(std::make_pair((int) STAT_WRITE_ACCESS,  "WRITE_ACCESS"));

    };

    ~CAFEGlobalAlarmCondition() {};

    std::string asString (int i)
    {

        pos = mapAlarmCondition.find(i);
        if (pos != mapAlarmCondition.end()) {
            return    pos->second;
        }

        std::ostringstream oss;
        if (i == -1) {
            oss << "NO_DATA" ;
            return oss.str();
        }
        std::cout << "------------------------------------" << std::endl;
        oss << "INVALID ENUM VALUE: " << i ; //<< --std::endl;
        std::cout << oss.str();
        std::cout << " VALID VALUES ARE: " << std::endl;
        printAll();
        std::cout << "--------------------------------------" << std::endl;
        return "NO_DATA"; //oss.str();
    };


    int asInt (std::string message)

    {
        for (pos=mapAlarmCondition.begin(); pos != mapAlarmCondition.end(); ++pos) {

            if (pos->second==message) return pos->first;
            // String searches such as s.find(s1) return string::npos on failure
            else if ( (pos->second).find(message)  != std::string::npos) return pos->first;
        }
        std::cout << "------------------------------------" << std::endl;
        std::cout << "INVALID INPUT: " << message << " VALID VALUES ARE: " << std::endl;
        printAll();
        std::cout << "------------------------------------" << std::endl;
        return  -1;
    };

    void getStates(std::vector<int> &vecI, std::vector<std::string> &vecS )
    {

        vecS.clear();
        vecS.reserve(mapAlarmCondition.size());
        vecI.clear();
        vecI.reserve(mapAlarmCondition.size());

        for (pos=mapAlarmCondition.begin(); pos != mapAlarmCondition.end(); ++pos) {
            vecI.push_back(pos->first);
            vecS.push_back(pos->second);
        }
        return;
    }


    void printAll()
    {
        std::cout << "-------------------" << std::endl;
        std::cout << "EPICS GLOBAL ALARM CONDITION LIST" << std::endl;
        std::cout << "-------------------" << std::endl;
        for (pos=mapAlarmCondition.begin(); pos != mapAlarmCondition.end(); ++pos) {
            if (pos->first < 10) {
                std::cout << " " << pos->first << " " << pos->second << std::endl;
            }
            else {
                std::cout << pos->first << " " << pos->second << std::endl;
            }
        }
        std::cout << "------------------" << std::endl;
    };

};



class CAFEGlobalAlarmSeverity {
    typedef std::map<int, std::string> mapIntString;

private:
    mapIntString mapAlarmSeverity;
    mapIntString::iterator pos;

public:
    CAFEGlobalAlarmSeverity()
    {
        mapAlarmSeverity.insert(std::make_pair((int) SEV_NO_ALARM, "NO_ALARM"));
        mapAlarmSeverity.insert(std::make_pair((int) SEV_MINOR, "MINOR"));
        mapAlarmSeverity.insert(std::make_pair((int) SEV_MAJOR, "MAJOR"));
        mapAlarmSeverity.insert(std::make_pair((int) SEV_INVALID, "INVALID"));
    };

    ~CAFEGlobalAlarmSeverity() {};

    std::string asString (int i)
    {

        pos = mapAlarmSeverity.find(i);
        if (pos != mapAlarmSeverity.end()) {
            return    pos->second;
        }


        std::ostringstream oss;
        if (i == -1) {
            oss << "NO_DATA" ;
            return oss.str();
        }
        std::cout << "--------------------------------------" << std::endl;
        oss << "INVALID ENUM VALUE: " << i ; //<< std::endl;
        std::cout << oss.str();
        std::cout << " VALID VALUES ARE: " << std::endl;
        printAll();
        std::cout << "--------------------------------------" << std::endl;
        return "NO_DATA"; //oss.str();
    };


    int asInt (std::string message)
    {
        for (pos=mapAlarmSeverity.begin(); pos != mapAlarmSeverity.end(); ++pos) {

            if (pos->second==message) return pos->first;
            // String searches such as s.find(s1) return string::npos on failure
            else if ( (pos->second).find(message)  != std::string::npos) return pos->first;
        }
        std::cout << "--------------------------------------" << std::endl;
        std::cout << "INVALID INPUT: " << message << " VALID VALUES ARE: " << std::endl;
        printAll();
        std::cout << "--------------------------------------" << std::endl;
        return  -1;
    };

    void getStates(std::vector<int> &vecI, std::vector<std::string> &vecS )
    {

        vecS.clear();
        vecS.reserve(mapAlarmSeverity.size());
        vecI.clear();
        vecI.reserve(mapAlarmSeverity.size());

        for (pos=mapAlarmSeverity.begin(); pos != mapAlarmSeverity.end(); ++pos) {

            vecI.push_back(pos->first);
            vecS.push_back(pos->second);
        }
        return;
    }

    void printAll()
    {
        std::cout << "-------------------" << std::endl;
        std::cout << "EPICS GLOBAL ALARM SEVERITY LIST" << std::endl;
        std::cout << "-------------------" << std::endl;
        for (pos=mapAlarmSeverity.begin(); pos != mapAlarmSeverity.end(); ++pos) {
            std::cout << pos->first << " " << pos->second << std::endl;
        }
        std::cout << "------------------" << std::endl;
    };

};



/* class StatusInfo provides information for user defined error/status codes
* and ca defined error/status codes \n
*    1 ECA_NORMAL \n
*   48 ECA_ALLOCMEM \n
*   72 ECA_TOLARGE \n
*   80 ECA_TIMEOUT \n
*  114 ECA_BADTYPE \n
*  142 ECA_INTERNAL \n
*  152 ECA_GETFAIL \n
*  160 ECA_PUTFAIL \n
*  176 ECA_BADCOUNT \n
*  186 ECA_BADSTR \n
*  192 ECA_DISCONN \n
*  200 ECA_DBLCHNL \n
*  210 ECA_EVDISALLOW \n
*  242 ECA_BADMONID \n
*  330 ECA_BADMASK \n
*  339 ECA_IODONE \n
*  347 ECA_IOINPROGRESS \n
*  354 ECA_BADSYNCGRP \n
*  362 ECA_PUTCBINPROG \n
*  368 ECA_NORDACCESS \n
*  376 ECA_NOWTACCESS \n
*  386 ECA_ANACHRONISM \n
*  392 ECA_NOSEARCHADDR \n
*  400 ECA_NOCONVERT \n
*  410 ECA_BADCHID \n
*  418 ECA_BADFUNCPTR \n
*  424 ECA_ISATTACHED \n
*  432 ECA_UNAVAILINSERV \n
*  440 ECA_CHANDESTROY \n
*  450 ECA_BADPRIORITY \n
*  458 ECA_NOTTHREADED \n
*  464 ECA_16KARRAYCLIENT \n
*  472 ECA_CONNSEQTMO \n
*  480 ECA_UNRESPTMO \n
*  600 ICAFE_CS_NEVER_CONN \n
*  601 ICAFE_CS_PREV_CONN \n
*  602 ICAFE_CS_CONN \n
*  603 ICAFE_CS_CLOSED \n
*  604 ICAFE_CS_DISCONN, \n
*  605 ICAFE_CS_UNKNOWN \n
*  700 ICAFE_TYPENOTCONN \n
*  701 ICAFE_RULE_FALSE \n
*  702 ICAFE_BADCOUNT \n
*  703 ICAFE_CALLBACK_NOT_YET_INVOKED \n
*  704 ICAFE_WAITING_FOR_PREV_CALLBACK \n
*  705 ICAFE_CACHE_EMPTY \n
*  706 ICAFE_CHANNEL_BLOCKING_POLICY_CONFLICT \n
*  707 ICAFE_MONITOR_DELAYED_AS_CONN_DOWN \n
*  708 ICAFE_HAS_MONITOR_GET_DONE_FROM_CACHE \n
*  709 ICAFE_SET_AND_GET_MISMATCH \n
*  806 ICAFE_CA_OP_CONN_UP \n
*  807 ICAFE_CA_OP_CONN_DOWN \n
* 1000 ECAFE_NODATA \n
* 1001 ECAFE_INVALID_TYPE \n
* 1002 ECAFE_BADCOUNT \n
* 1003 ECAFE_BADSTR \n
* 1004 ECAFE_BADTYPE \n
* 1005 ECAFE_NO_CONVERT \n
* 1006 ECAFE_NULLCONTEXT \n
* 1007 ECAFE_NULLCHID \n
* 1008 ECAFE_NULLEVID \n
* 1009 ECAFE_UNKNOWN_COLLECTION \n
* 1010 ECAFE_EMPTY_COLLECTION \n
* 1011 ECAFE_COLLECTION_PREV_DEF \n
* 1012 ECAFE_COLLECTION_INVALID_MEMBER \n
* 1013 ECAFE_RULE_FALSE \n
* 1014 ECAFE_UNKNOWN_GROUP \n
* 1015 ECAFE_EMPTY_GROUP \n
* 1016 ECAFE_GROUP_PREV_DEF \n
* 1017 ECAFE_INVALID_HANDLE \n
* 1018 ECAFE_INVALID_GROUP_HANDLE \n
* 1019 ECAFE_NORDACCESS \n
* 1020 ECAFE_NOWTACCESS \n
* 1021 ECAFE_TIMEOUT \n
* 1022 ECAFE_CANNOT_OPEN_FILE \n
* 1023 ECAFE_INVALID_SWITCH_CASE \n
* 1024 ECAFE_PVALIAS_INVALID \n
* 1025 ECAFE_PVALIAS_PREV_DEF \n
* 1026 ECAFE_PVNAME_PREV_DEF_AS_PVALIAS \n
* 1027 ECAFE_DEVICE_ATTRIB_NOT_FOUND \n
* 1028 ECAFE_HASH_UNIQUEID_EXISTS \n
* 1029 ECAFE_WRONG_CA_CONTEXT \n
* 1030 ECAFE_INVALID_CAFENUM_POLICY_TYPE \n
* 1031 ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED \n
* 1032 ECAFE_INVALID_ENUM_INDEX \n
* 1033 ECAFE_PVGROUP_GROUPHANDLE_MISMATCH \n
* 1034 ECAFE_TIMEOUT_SET_AND_MATCH \n
* 1035 ECAFE_HANDLE_MISMATCH_SET_AND_MATCH \n
* 1036 ECAFE_INCONSISTENT_CONTAINER_CORRECTED \n
* 1100 ECAFE_LOAD_COLLECTION \n
* 1101 ECAFE_LOAD_GROUP \n
* 1200 ECAFE_BPM_DATA_IS_INVALID \n
* 1300 ECAFE_BITSHUFF_DECOMPRESS \n
* 1301 ECAFE_BITSHUFF_ALLOCMEM \n
* 1302 ECAFE_BITSHUFF_REALLOCMEM \n
* 1302 ECAFE_BITSHUFF_BADCOUNT \n
* 1500 ECAFE_BSREAD_MULTIPART_MESS_NODATA \n
* 1501 ECAFE_BSREAD_PARSEFAIL_MAINHEADER \n
* 1502 ECAFE_BSREAD_PARSEFAIL_DATAHEADER \n
* 1503 ECAFE_BSREAD_ZMQSTREAM_NULL \n
* 5004 ERRNO_EINTR \n
* 5011 ERRNO_EGAIN \n
* 5014 ERRNO_EFAULT \n
* 5022 ERRNO_EINVAL Invalid argument \n
* 5088 ERRNO_ENOTSOCK \n
* 5093 ERRNO_EPROTONOSUPPORT \n
*/
class CAFEStatusInfo {
    typedef std::map<int, std::string> mapLongString;
private:
    mapLongString mapStatusInfo;
    mapLongString::iterator posStatusInfo;

public:
    CAFEStatusInfo()
    {
        //maximum message length is 80 characters

        mapStatusInfo.insert(std::make_pair((int)  ICAFE_CA_OP_CONN_UP,     "Status of channel connection: channel UP " ));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_CA_OP_CONN_DOWN,   "Status of channel connection: channel DOWN! " ));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_CS_NEVER_CONN,     "Valid chid; server not found or unavailable " ));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_CS_PREV_CONN,      "Valid chid; previously connected to server " ));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_CS_CONN,           "Valid chid; connected to server " ));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_CS_CLOSED,         "Channel closed by user " ));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_CS_DISCONN,        "Channel disconnected " ));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_CS_UNKNOWN,        "Unknown state!!! " ));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_TYPENOTCONN,       "ca_field_type() channel is disconnected " ));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_RULE_FALSE,        "CAFE collection rule for channel set to false by user " ));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_BADCOUNT,          "nelemClient exceeds nelemNative (max. allowed)! Set nelemClient/nelemRequest to nelemNative "));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_CALLBACK_NOT_YET_INVOKED,  "Callback for get/set operation has not yet been invoked "));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_WAITING_FOR_PREV_CALLBACK, "Callback for previous get/set operation not yet invoked "));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_CACHE_EMPTY,        "Data buffer is empty; get message to IOC not yet invoked"));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_CHANNEL_BLOCKING_POLICY_CONFLICT, "User supplied callback function not provided when expected "));

        mapStatusInfo.insert(std::make_pair((int)  ICAFE_MONITOR_DELAYED_AS_CONN_DOWN, "Channel disconnected. Monitor will be started on connection "));
        mapStatusInfo.insert(std::make_pair((int)  ICAFE_HAS_MONITOR_GET_DONE_FROM_CACHE, "Handle has monitor, hence data retrieved from cache "));

        mapStatusInfo.insert(std::make_pair((int)  ICAFE_SET_AND_GET_MISMATCH, "Set and Get values from SetAndGetMethod do not match"));


        mapStatusInfo.insert(std::make_pair((int)  ECAFE_NODATA,            "Requested data transfer is of zero length! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_INVALID_TYPE,      "Invalid data type! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_BADCOUNT,          "nelemClient exceeds nelemNative (max. allowed)! Set nelemClient/nelemRequest to nelemNative "));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_BADSTR,            "Empty string for process variable name! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_BADTYPE,           "Invalid datatype! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_NO_CONVERT,        "set()/get() cannot convert to/from native datatype! " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_NULLCONTEXT,  "Error: current context is null; CA not initialized. " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_NULLCHID,     "Error: chid is null!!  " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_NULLEVID,                 "Monitor() evid is null! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_UNKNOWN_COLLECTION,       "CAFE collection is unknown/unregistered or invalid! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_EMPTY_COLLECTION,         "CAFE collection is empty; has no members! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_COLLECTION_PREV_DEF,      "CAFE collection with this name already exists! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_COLLECTION_INVALID_MEMBER,"Device is not a member of the CAFE collection! " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_RULE_FALSE,          "CAFE rule for channel set to false by user " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_UNKNOWN_GROUP,  "CAFE group is unknown/unregistered or invalid! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_EMPTY_GROUP,    "CAFE group is empty; has no members! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_GROUP_PREV_DEF, "CAFE group with this name already exists! " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_INVALID_HANDLE,  "Handle does not exist! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_INVALID_GROUP_HANDLE,  "Group handle does not exist! " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_NORDACCESS,  "Channel does not have READ access!  " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_NOWTACCESS,  "Channel does not have WRITE access! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_TIMEOUT,     "Callback function not activated within specified timeout period " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_CANNOT_OPEN_FILE,          "Cannot open file! " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_INVALID_SWITCH_CASE, "Internal CAFE Error: Invalid option for switch case " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_PVALIAS_INVALID,     "Requested PVAlias is another handle's PV name. Bizarre! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_PVALIAS_PREV_DEF,    "Requested PVAlias already exists! " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_PVNAME_PREV_DEF_AS_PVALIAS, "PVName clashes with previously defined PVAlias!" ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_DEVICE_ATTRIB_NOT_FOUND, "Deliminator - hence dev/attrib - not found in PV name" ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_HASH_UNIQUEID_EXISTS,"cafeConduit object already inserted (handle exists) "));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_WRONG_CA_CONTEXT,"Operation cannot be carried out for this ca_client_context "));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_INVALID_CAFENUM_POLICY_TYPE, "Not a valid CAFENUM type for this policy. See policies.h "));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED, "See define.sh to change the allowed maximum "));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_INVALID_ENUM_INDEX, "DBR_ENUM value exceeds the number of enum string options " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_PVGROUP_GROUPHANDLE_MISMATCH, "PVGroup previously assigned to another group handle " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_TIMEOUT_SET_AND_MATCH, "Readback channel did not reach set value within specified timeout period "));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_HANDLE_MISMATCH_SET_AND_MATCH, "Number of set/readback handles do not match"));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_INCONSISTENT_CONTAINER_CORRECTED, "Boost container required update after consistency check"));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_LOAD_COLLECTION,     "CAFE collection could not be loaded from xml configuration file " ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_LOAD_GROUP,          "CAFE group could not be loaded from group xml configuration file " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_BPM_DATA_IS_INVALID, "CAFE BPM Service: Data Validity channel reports BPM data is INVALID " ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_BITSHUFF_DECOMPRESS,  "Bitshuffle decompress failed within CAFE BS read" ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_BITSHUFF_ALLOCMEM,    "Bitshuffle memory allocation failed within CAFE BS read" ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_BITSHUFF_REALLOCMEM,  "Bitshuffle memory reallocation failed within CAFE BS read" ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_BITSHUFF_BADCOUNT,    "Bitshuffle data size is too small (<BSREAD_PREBLOB_BYTES)" ));

        mapStatusInfo.insert(std::make_pair((int)  ECAFE_BSREAD_MULTIPART_MESS_NODATA, "No data for this channel from BS read" ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_BSREAD_PARSEFAIL_MAINHEADER, "Main header in BS read could not be parsed" ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_BSREAD_PARSEFAIL_DATAHEADER, "Data header in BS read could not be parsed" ));
        mapStatusInfo.insert(std::make_pair((int)  ECAFE_BSREAD_ZMQSTREAM_NULL, "ZMQ stream not created. Check configuration parameters"));
        mapStatusInfo.insert(std::make_pair((int)  ERRNO_EINTR,    "C ERRNO CODE: Interrupted system call" ));
        mapStatusInfo.insert(std::make_pair((int)  ERRNO_EAGAIN,   "C ERRNO CODE: Try again" ));
        mapStatusInfo.insert(std::make_pair((int)  ERRNO_EFAULT,   "C ERRNO CODE: Bad address" ));
        mapStatusInfo.insert(std::make_pair((int)  ERRNO_EINVAL,   "C ERROR CODE: Invalid argument" ));
        mapStatusInfo.insert(std::make_pair((int)  ERRNO_ENOTSOCK, "C ERRNO CODE: Socket operation on non-socket" ));
        mapStatusInfo.insert(std::make_pair((int)  ERRNO_EPROTONOSUPPORT,   "C ERRNO CODE: Protocol not supported" ));
    }

    ~CAFEStatusInfo() {};


    std::string message (int i)
    {

        if (i<ICAFE_STATUS_BASE) {
            std::string c= ca_message(i);
            return c;
        }
        else {
            posStatusInfo = mapStatusInfo.find(i);
            if (posStatusInfo != mapStatusInfo.end()) {
                return    posStatusInfo->second;
            }

            std::ostringstream oss;
            oss << "INVALID ERROR CODE: " << i ; //<< std::endl;
            //std::cout << oss.str();

            return oss.str();

        }
    };



    void printAll()
    {
        std::cout << "-----------------------" << std::endl;
        std::cout << "CAFE ERROR MESSAGE LIST" << std::endl;
        std::cout << "-----------------------" << std::endl;
        for (posStatusInfo=mapStatusInfo.begin(); posStatusInfo != mapStatusInfo.end(); ++posStatusInfo) {
            std::cout << posStatusInfo->first << " " << posStatusInfo->second << std::endl;
        }
        std::cout << "------------------" << std::endl;
    };
};





class CAFEStatusCode {
    typedef std::map<int, std::string> mapLongString;

private:
    mapLongString mapStatusCode;
    mapLongString::iterator pos;

public:
    CAFEStatusCode()
    {
        mapStatusCode.insert(std::make_pair(ECA_NORMAL,        "SUCCESS: ICAFE_NORMAL"));
        mapStatusCode.insert(std::make_pair(ECA_ALLOCMEM,      "WARNING: ECA_ALLOCMEM"));
        mapStatusCode.insert(std::make_pair(ECA_TOLARGE,       "WARNING: ECA_TOLARGE"));
        mapStatusCode.insert(std::make_pair(ECA_TIMEOUT,       "WARNING: ECA_TIMEOUT"));
        mapStatusCode.insert(std::make_pair(ECA_BADTYPE,         "ERROR: ECA_BADTYPE"));
        mapStatusCode.insert(std::make_pair(ECA_INTERNAL,        "FATAL: ECA_INTERNAL"));
        mapStatusCode.insert(std::make_pair(ECA_GETFAIL,       "WARNING: ECA_GETFAIL"));
        mapStatusCode.insert(std::make_pair(ECA_PUTFAIL,       "WARNING: ECA_PUTFAIL"));
        mapStatusCode.insert(std::make_pair(ECA_BADCOUNT,      "WARNING: ECA_BADCOUNT"));
        mapStatusCode.insert(std::make_pair(ECA_BADSTR,          "ERROR: ECA_BADSTR"));
        mapStatusCode.insert(std::make_pair(ECA_DISCONN,       "WARNING: ECA_DISCONN"));
        mapStatusCode.insert(std::make_pair(ECA_DBLCHNL,       "WARNING: ECA_DBLCHNL"));
        mapStatusCode.insert(std::make_pair(ECA_EVDISALLOW,      "ERROR: ECA_EVDISALLOW"));
        mapStatusCode.insert(std::make_pair(ECA_BADMONID,        "ERROR: ECA_BADMONID"));
        mapStatusCode.insert(std::make_pair(ECA_BADMASK,         "ERROR: ECA_BADMASK"));
        mapStatusCode.insert(std::make_pair(ECA_IODONE,          "INFO: ECA_IODONE "));
        mapStatusCode.insert(std::make_pair(ECA_IOINPROGRESS,     "INFO: ECA_IOINPROGRESS"));
        mapStatusCode.insert(std::make_pair(ECA_BADSYNCGRP,      "ERROR: ECA_BADSYNCGRP"));
        mapStatusCode.insert(std::make_pair(ECA_PUTCBINPROG,     "ERROR: ECA_PUTCBINPROG"));
        mapStatusCode.insert(std::make_pair(ECA_NORDACCESS,    "WARNING: ECA_NORDACCESS"));
        mapStatusCode.insert(std::make_pair(ECA_NOWTACCESS,    "WARNING: ECA_NOWTACCESS"));
        mapStatusCode.insert(std::make_pair(ECA_ANACHRONISM,     "ERROR: ECA_ANACHRONISM"));
        mapStatusCode.insert(std::make_pair(ECA_NOSEARCHADDR,  "WARNING: ECA_NOSEARCHADDR"));
        mapStatusCode.insert(std::make_pair(ECA_NOCONVERT,     "WARNING: ECA_NOCONVERT"));
        mapStatusCode.insert(std::make_pair(ECA_BADCHID,         "ERROR: ECA_BADCHID"));
        mapStatusCode.insert(std::make_pair(ECA_BADFUNCPTR,      "ERROR: ECA_BADFUNCPTR"));
        mapStatusCode.insert(std::make_pair(ECA_ISATTACHED,    "WARNING: ECA_ISATTACHED"));
        mapStatusCode.insert(std::make_pair(ECA_UNAVAILINSERV, "WARNING: ECA_UNAVAILINSERV"));
        mapStatusCode.insert(std::make_pair(ECA_CHANDESTROY,   "WARNING: ECA_CHANDESTROY"));
        mapStatusCode.insert(std::make_pair(ECA_BADPRIORITY,     "ERROR: ECA_BADPRIORITY"));
        mapStatusCode.insert(std::make_pair(ECA_NOTTHREADED,     "ERROR: ECA_NOTTHREADED"));
        mapStatusCode.insert(std::make_pair(ECA_16KARRAYCLIENT,"WARNING: ECA_16KARRAYCLIENT"));
        mapStatusCode.insert(std::make_pair(ECA_CONNSEQTMO,    "WARNING: ECA_CONNSEQTMO"));
        mapStatusCode.insert(std::make_pair(ECA_UNRESPTMO,    "WARNING: ECA_UNRESPTMO"));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_CS_NEVER_CONN,   "CHANNEL STATE: ICAFE_CS_NEVER_CONN" ));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_CS_PREV_CONN,    "CHANNEL STATE: ICAFE_CS_PREV_CONN " ));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_CS_CONN,         "CHANNEL STATE: ICAFE_CS_CONN" ));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_CS_CLOSED,       "CHANNEL STATE: ICAFE_CS_CLOSED" ));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_CS_DISCONN,      "CHANNEL STATE: ICAFE_CS_DISCONN" ));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_CS_UNKNOWN,      "CHANNEL STATE: ICAFE_CS_UNKNOWN" ));

        mapStatusCode.insert(std::make_pair((int)  ICAFE_TYPENOTCONN,     "CHANNEL FIELD TYPE: ICAFE_CFT_TYPENOTCONN"));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_RULE_FALSE,      "CAFE INFO: ICAFE_RULE_FALSE"));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_BADCOUNT,          "CAFE INFO: ICAFE_BADCOUNT"));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_CALLBACK_NOT_YET_INVOKED, "CAFE_INFO: ICAFE_CALLBACK_NOT_YET_INVOKED"));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_WAITING_FOR_PREV_CALLBACK, "CAFE_INFO: ICAFE_WAITING_FOR_PREV_CALLBACK"));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_CACHE_EMPTY,     "CAFE_INFO:ICAFE_CACHE_EMPTY"));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_CHANNEL_BLOCKING_POLICY_CONFLICT, "CAFE_INFO: ICAFE_CHANNEL_BLOCKING_POLICY_CONFLICT"));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_CA_OP_CONN_UP,   "CHANNEL CONNECTION: ICAFE_CC_OP_CONN_UP" ));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_CA_OP_CONN_DOWN, "CHANNEL CONNECTION: ICAFE_CC_OP_CONN_DOWN" ));

        mapStatusCode.insert(std::make_pair((int)  ICAFE_MONITOR_DELAYED_AS_CONN_DOWN, "CHANNEL CONNECTION: ICAFE_MONITOR_DELAYED_AS_CONN_DOWN"));
        mapStatusCode.insert(std::make_pair((int)  ICAFE_HAS_MONITOR_GET_DONE_FROM_CACHE, "CAFE_INFO: ICAFE_HAS_MONITOR_GET_DONE_FROM_CACHE"));

        mapStatusCode.insert(std::make_pair((int)  ICAFE_SET_AND_GET_MISMATCH, "CAFE_INFO: ICAFE_SET_AND_GET_MISMATCH"));

        mapStatusCode.insert(std::make_pair((int)  ECAFE_NODATA,          "CAFE ERROR: ECAFE_NODATA"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_BADCOUNT,        "CAFE ERROR: ECAFE_BADCOUNT"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_BADSTR,          "CAFE ERROR: ECAFE_BADSTR"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_BADTYPE,         "CAFE ERROR: ECAFE_BADTYPE"));

        mapStatusCode.insert(std::make_pair((int)  ECAFE_NO_CONVERT,           "CAFE ERROR: ECAFE_NO_CONVERT"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_NULLCONTEXT,          "CAFE ERROR: ECAFE_NULLCONTEXT"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_NULLCHID,             "CAFE ERROR: ECAFE_NULLCHID"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_NULLEVID,             "CAFE ERROR: ECAFE_NULLEVID"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_UNKNOWN_COLLECTION,   "CAFE ERROR: ECAFE_UNKNOWN_COLLECTION"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_EMPTY_COLLECTION,     "CAFE ERROR: ECAFE_EMPTY_COLLECTION"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_COLLECTION_PREV_DEF,  "CAFE ERROR: ECAFE_COLLECTION_PREV_DEF"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_COLLECTION_INVALID_MEMBER,  "CAFE ERROR: ECAFE_COLLECTION_INVALID_MEMBER"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_RULE_FALSE,           "CAFE ERROR/INFO: ECAFE_RULE_FALSE"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_UNKNOWN_GROUP,        "CAFE ERROR: ECAFE_UNKNOWN_GROUP"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_EMPTY_GROUP,          "CAFE ERROR: ECAFE_EMPTY_GROUP"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_GROUP_PREV_DEF,       "CAFE ERROR: ECAFE_GROUP_PREV_DEF"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_INVALID_HANDLE,       "CAFE ERROR: ECAFE_INVALID_HANDLE"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_INVALID_GROUP_HANDLE, "CAFE ERROR: ECAFE_INVALID_GROUP_HANDLE"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_NORDACCESS,       "CAFE ERROR: ECAFE_NORDACCESS"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_NOWTACCESS,       "CAFE ERROR: ECAFE_NOWTACCESS"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_TIMEOUT,          "CAFE ERROR: ECAFE_TIMEOUT"));

        mapStatusCode.insert(std::make_pair((int)  ECAFE_CANNOT_OPEN_FILE,     "CAFE ERROR: ECAFE_CANNOT_OPEN_FILE"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_INVALID_SWITCH_CASE,  "CAFE ERROR: ECAFE_INVALID_SWITCH_CASE"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_PVALIAS_INVALID,      "CAFE ERROR: ECAFE_PVALIAS_INVALID"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_PVALIAS_PREV_DEF,     "CAFE ERROR: ECAFE_PVALIAS_PREV_DEF"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_PVNAME_PREV_DEF_AS_PVALIAS,"CAFE ERROR: ECAFE_PVNAME_PREV_DEF_AS_PVALIAS"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_DEVICE_ATTRIB_NOT_FOUND,"CAFE ERROR: ECAFE_DEVICE_ATTRIB_NOT_FOUND"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_HASH_UNIQUEID_EXISTS, "CAFE ERROR: ECAFE_HASH_UNIQUEID_EXISTS"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_WRONG_CA_CONTEXT,     "CAFE ERROR: ECAFE_WRONG_CA_CONTEXT"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_INVALID_CAFENUM_POLICY_TYPE, "CAFE ERROR: ECAFE_INVALID_CAFENUM_POLICY_TYPE"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED, "CAFE_ERROR: ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_INVALID_ENUM_INDEX,   "CAFE_ERROR: ECAFE_INVALID_ENUM_INDEX"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_PVGROUP_GROUPHANDLE_MISMATCH, "CAFE ERROR:ECAFE_PVGROUP_GROUPHANDLE_MISMATCH"));

        mapStatusCode.insert(std::make_pair((int)  ECAFE_TIMEOUT_SET_AND_MATCH, "CAFE ERROR: CAFE_TIMEOUT_SET_AND_MATCH"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_HANDLE_MISMATCH_SET_AND_MATCH, "CAFE ERROR: CAFE_HANDLE_MISMATCH_SET_AND_MATCH"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_INCONSISTENT_CONTAINER_CORRECTED,"CAFE ERROR: ECAFE_INCONSISTENT_CONTAINER_CORRECTED"));

        mapStatusCode.insert(std::make_pair((int)  ECAFE_LOAD_COLLECTION,      "CAFE ERROR: ECAFE_LOAD_COLLECTION"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_LOAD_GROUP,           "CAFE ERROR: ECAFE_LOAD_GROUP"));

        mapStatusCode.insert(std::make_pair((int)  ECAFE_BPM_DATA_IS_INVALID, "CAFE_ERROR: ECAFE_BPM_DATA_IS_INVALID" ));

        mapStatusCode.insert(std::make_pair((int)  ECAFE_BITSHUFF_DECOMPRESS,  "CAFE_ERROR: ECAFE_BITSHUF_DECOMPRESS" ));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_BITSHUFF_REALLOCMEM,  "CAFE_WARN: ECAFE_BITSHUFF_REALLOCMEM" ));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_BITSHUFF_ALLOCMEM,    "CAFE_ERROR: ECAFE_BITSHUFF_ALLOCMEM" ));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_BITSHUFF_BADCOUNT,    "CAFE_ERROR: ECAFE_BITSHUFF_BADCOUNT" ));

        mapStatusCode.insert(std::make_pair((int)  ECAFE_BSREAD_MULTIPART_MESS_NODATA, "CAFE_WARN: ECAFE_BSREAD_MULTIPART_MESS_NODATA"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_BSREAD_PARSEFAIL_MAINHEADER,  "CAFE_ERROR: ECAFE_BSREAD_PARSEFAIL_MAINHEADER"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_BSREAD_PARSEFAIL_DATAHEADER,  "CAFE_ERROR: ECAFE_BSREAD_PARSEFAIL_DATAHEADER"));
        mapStatusCode.insert(std::make_pair((int)  ECAFE_BSREAD_ZMQSTREAM_NULL, "CAFE_WARN: ECAFE_BSREAD_ZMQSTREAM_NULL"));

        mapStatusCode.insert(std::make_pair((int)  ERRNO_EINTR,    "CAFE_ERROR: ERRNO_EINTR" ));
        mapStatusCode.insert(std::make_pair((int)  ERRNO_EAGAIN,   "CAFE_WARN: ERRNO_EAGAIN" ));
        mapStatusCode.insert(std::make_pair((int)  ERRNO_EFAULT,   "CAFE_ERROR: ERRNO_EFAULT" ));
        mapStatusCode.insert(std::make_pair((int)  ERRNO_EINVAL,   "CAFE_ERROR: ERRNO_EINVAL" ));
        mapStatusCode.insert(std::make_pair((int)  ERRNO_ENOTSOCK, "CAFE_ERROR: ERRNO_ENOTSOCK" ));
        mapStatusCode.insert(std::make_pair((int)  ERRNO_EPROTONOSUPPORT,   "CAFE_ERROR: ERRNO_EPROTONOSUPPORT" ));

    };

    ~CAFEStatusCode() {};

    std::string message (int i)
    {

        pos = mapStatusCode.find(i);
        if (pos != mapStatusCode.end()) {
            return    pos->second;
        }

        std::ostringstream oss;
        oss << "INVALID ERROR CODE: " << i ; //<< std::endl;
        //std::cout << oss.str();

        return oss.str();
    };


    void getStates(std::vector<int> &vecI, std::vector<std::string> &vecS )
    {

        vecS.clear();
        vecS.reserve(mapStatusCode.size());
        vecI.clear();
        vecI.reserve(mapStatusCode.size());

        for (pos=mapStatusCode.begin(); pos != mapStatusCode.end(); ++pos) {
            std::size_t found = (pos->second).find(": ");
            if (found!=std::string::npos) {

                vecS.push_back((pos->second).substr(found+2,(pos->second).length()));
            }
            else {
                vecS.push_back(pos->second);
            }
            vecI.push_back(pos->first);
        }
        return;
    };

    std::string msgIDAsString (int i)
    {

        pos = mapStatusCode.find(i);
        if (pos != mapStatusCode.end()) {
            std::size_t found = (pos->second).find(": ");
            if (found!=std::string::npos) {

                return (pos->second).substr(found+2,(pos->second).length());
            }
            else {
                return    pos->second;
            }
        }

        std::ostringstream oss;
        oss << "INVALID ERROR CODE: " << i ; //<< std::endl;
        //std::cout << oss.str();

        return oss.str();
    };


    int enumIs (std::string message)
    {
        for (pos=mapStatusCode.begin(); pos != mapStatusCode.end(); ++pos) {

            if (pos->second==message) return pos->first;
            // String searches such as s.find(s1) return string::npos on failure
            else if ( (pos->second).find(message)  != std::string::npos) return pos->first;
        }
        return  -1;
    };

    bool isTimeout(int statusCodeToCheck)
    {
        if (statusCodeToCheck==ECA_TIMEOUT || statusCodeToCheck==ECAFE_TIMEOUT) {
            return true;
        }
        else {
            return false;
        }
    }

    void printAll()
    {
        std::cout << "-------------------" << std::endl;
        std::cout << "CAFE ERROR CODE LIST" << std::endl;
        std::cout << "-------------------" << std::endl;
        for (pos=mapStatusCode.begin(); pos != mapStatusCode.end(); ++pos) {
            std::cout << pos->first << " " << pos->second << std::endl;
        }
        std::cout << "------------------" << std::endl;
    };

};



class CAFEStatusSeverity {
    typedef std::map<int, std::string> mapLongString;

private:
    mapLongString mapStatusSeverity;
    mapLongString::iterator pos;

public:
    CAFEStatusSeverity()
    {
        mapStatusSeverity.insert(std::make_pair(ECA_NORMAL,        "INFO"));
        mapStatusSeverity.insert(std::make_pair(ECA_ALLOCMEM,      "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_TOLARGE,       "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_TIMEOUT,       "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_BADTYPE,       "ERROR"));
        mapStatusSeverity.insert(std::make_pair(ECA_INTERNAL,      "FATAL"));
        mapStatusSeverity.insert(std::make_pair(ECA_GETFAIL,       "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_PUTFAIL,       "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_BADCOUNT,      "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_BADSTR,        "ERROR"));
        mapStatusSeverity.insert(std::make_pair(ECA_DISCONN,       "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_DBLCHNL,       "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_EVDISALLOW,    "ERROR"));
        mapStatusSeverity.insert(std::make_pair(ECA_BADMONID,      "ERROR"));
        mapStatusSeverity.insert(std::make_pair(ECA_BADMASK,       "ERROR"));
        mapStatusSeverity.insert(std::make_pair(ECA_IODONE,       "INFO"));
        mapStatusSeverity.insert(std::make_pair(ECA_IOINPROGRESS,  "INFO"));
        mapStatusSeverity.insert(std::make_pair(ECA_BADSYNCGRP,    "ERROR"));
        mapStatusSeverity.insert(std::make_pair(ECA_PUTCBINPROG,   "ERROR"));
        mapStatusSeverity.insert(std::make_pair(ECA_NORDACCESS,    "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_NOWTACCESS,    "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_ANACHRONISM,   "ERROR"));
        mapStatusSeverity.insert(std::make_pair(ECA_NOSEARCHADDR,  "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_NOCONVERT,     "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_BADCHID,       "ERROR"));
        mapStatusSeverity.insert(std::make_pair(ECA_BADFUNCPTR,    "ERROR"));
        mapStatusSeverity.insert(std::make_pair(ECA_ISATTACHED,    "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_UNAVAILINSERV, "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_CHANDESTROY,   "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_BADPRIORITY,   "ERROR"));
        mapStatusSeverity.insert(std::make_pair(ECA_NOTTHREADED,   "ERROR"));
        mapStatusSeverity.insert(std::make_pair(ECA_16KARRAYCLIENT,"WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_CONNSEQTMO,    "WARN"));
        mapStatusSeverity.insert(std::make_pair(ECA_UNRESPTMO,    "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_CS_NEVER_CONN,   "WARN" ));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_CS_PREV_CONN,    "INFO" ));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_CS_CONN,         "INFO" ));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_CS_CLOSED,       "INFO" ));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_CS_DISCONN,      "WARN" ));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_CS_UNKNOWN,      "ERROR" ));

        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_TYPENOTCONN,      "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_RULE_FALSE,      "INFO"));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_BADCOUNT,        "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_CALLBACK_NOT_YET_INVOKED,  "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_WAITING_FOR_PREV_CALLBACK, "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_CACHE_EMPTY,     "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_CHANNEL_BLOCKING_POLICY_CONFLICT, "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_CA_OP_CONN_UP,   "INFO" ));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_CA_OP_CONN_DOWN, "WARN" ));

        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_MONITOR_DELAYED_AS_CONN_DOWN, "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_HAS_MONITOR_GET_DONE_FROM_CACHE, "INFO"));

        mapStatusSeverity.insert(std::make_pair((int)  ICAFE_SET_AND_GET_MISMATCH, "WARN"));

        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_NODATA,          "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_BADCOUNT,        "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_BADSTR,          "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_BADTYPE,         "ERROR"));

        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_NO_CONVERT,           "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_NULLCONTEXT,          "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_NULLCHID,             "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_NULLEVID,             "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_UNKNOWN_COLLECTION,   "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_EMPTY_COLLECTION,     "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_COLLECTION_PREV_DEF,  "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_COLLECTION_INVALID_MEMBER,  "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_RULE_FALSE,           "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_UNKNOWN_GROUP,        "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_EMPTY_GROUP,          "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_GROUP_PREV_DEF,       "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_INVALID_HANDLE,       "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_INVALID_GROUP_HANDLE, "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_NORDACCESS,           "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_NOWTACCESS,           "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_TIMEOUT,              "WARN"));


        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_CANNOT_OPEN_FILE,     "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_INVALID_SWITCH_CASE,  "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_PVALIAS_INVALID,      "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_PVALIAS_PREV_DEF,     "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_PVNAME_PREV_DEF_AS_PVALIAS,"ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_DEVICE_ATTRIB_NOT_FOUND,"ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_HASH_UNIQUEID_EXISTS, "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_WRONG_CA_CONTEXT,     "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_INVALID_CAFENUM_POLICY_TYPE, "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_MAX_MONITORS_PER_CHAN_EXCEEDED, "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_INVALID_ENUM_INDEX,   "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_PVGROUP_GROUPHANDLE_MISMATCH, "ERROR"));

        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_TIMEOUT_SET_AND_MATCH, "WARN"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_HANDLE_MISMATCH_SET_AND_MATCH, "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_INCONSISTENT_CONTAINER_CORRECTED, "WARN"));

        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_LOAD_COLLECTION,      "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_LOAD_GROUP,           "ERROR"));

        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_BPM_DATA_IS_INVALID,  "WARN" ));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_BITSHUFF_DECOMPRESS,  "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_BITSHUFF_ALLOCMEM,    "ERROR" ));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_BITSHUFF_REALLOCMEM,  "WARN" ));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_BITSHUFF_BADCOUNT,    "ERROR" ));

        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_BSREAD_MULTIPART_MESS_NODATA, "WARN" ));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_BSREAD_PARSEFAIL_MAINHEADER,  "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_BSREAD_PARSEFAIL_DATAHEADER,  "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ECAFE_BSREAD_ZMQSTREAM_NULL,        "WARN"));

        mapStatusSeverity.insert(std::make_pair((int)  ERRNO_EINTR,     "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ERRNO_EAGAIN,    "WARN" ));
        mapStatusSeverity.insert(std::make_pair((int)  ERRNO_EFAULT,    "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ERRNO_EINVAL,    "ERROR" ));
        mapStatusSeverity.insert(std::make_pair((int)  ERRNO_ENOTSOCK,  "ERROR"));
        mapStatusSeverity.insert(std::make_pair((int)  ERRNO_EPROTONOSUPPORT, "ERROR"  ));

    };

    ~CAFEStatusSeverity() {};

    std::string message (int i)
    {
        pos = mapStatusSeverity.find(i);
        if (pos != mapStatusSeverity.end()) {
            return    pos->second;
        }

        std::ostringstream oss;
        oss << "INVALID ERROR CODE: " << i ; //<< std::endl;
        //std::cout << oss.str();

        return oss.str();
    };

    int enumIs (std::string message)
    {
        for (pos=mapStatusSeverity.begin(); pos != mapStatusSeverity.end(); ++pos) {

            if (pos->second==message) return pos->first;
            // String searches such as s.find(s1) return string::npos on failure
            else if ( (pos->second).find(message)  != std::string::npos) return pos->first;
        }
        return  -1;
    };

    void printAll()
    {
        std::cout << "-------------------" << std::endl;
        std::cout << "CAFE STATUS SEVERITY LIST" << std::endl;
        std::cout << "-------------------" << std::endl;
        for (pos=mapStatusSeverity.begin(); pos != mapStatusSeverity.end(); ++pos) {
            std::cout << pos->first << " " << pos->second << std::endl;
        }
        std::cout << "------------------" << std::endl;
    };

};

//Example
//For statusCode=0, we have
//severity: INFO
//asString: ECA_NORMAL or ICAFE_NORMAL as from Feb. 2018)
//msgID:    ECA_NORMAL
//code:     SUCCESS: ECA_NORMAL
//info:     Normal successful completion
//message:  SUCCESS: ECA_NORMAL Normal successful completion
class CAFEStatus {

public:
    CAFEStatusSeverity css;
    CAFEStatusInfo csi;
    CAFEStatusCode csc;
    CAFEStatus() {};

    std::string severity(int i)
    {
        return (std::string) css.message(i);
    }

    std::string asString(int i)
    {
        return (std::string) csc.msgIDAsString(i);
    }

    std::string msgID(int i)
    {
        return (std::string) csc.msgIDAsString(i);
    }

    std::string code(int i)
    {
        return (std::string) csc.message(i);
    }

    std::string info(int i)
    {
        return (std::string) csi.message(i);
    }

    std::string message(int i)
    {
        std::string strRet="";
        strRet.append(csc.message(i));
        strRet.append(" ");
        strRet.append(csi.message(i));
        return (std::string) strRet;
    }

    bool isTimeout(int i)
    {
        return csc.isTimeout(i);
    }

    void report (int i)
    {
        //std::cout << "------------------" << std::endl;
        //std::cout << "CAFE STATUS REPORT" << std::endl;
        std::cout << "------------------------------------" << std::endl;
        std::cout << csc.message(i)       << std::endl;
        std::cout << csi.message(i)       << std::endl;
        std::cout << "------------------------------------" << std::endl;
    };
};

#endif // STATUSCODES_H
