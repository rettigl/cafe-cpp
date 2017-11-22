///
/// \file    defines.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0

#ifndef DEFINES_H
#define DEFINES_H

#include <cadef.h>
#include <iostream>


#define EV      EPICSVERSION(HAVE_EPICS)

#if (EPICS_MAJOR==3) && (EPICS_MINOR==14) && (EPICS_PATCH>=11)
#define MASK_CTRL DBE_VALUE | DBE_LOG | DBE_ALARM | DBE_PROPERTY
#elif (EPICS_MAJOR==3) && (EPICS_MINOR>14) 
#define MASK_CTRL DBE_VALUE | DBE_LOG | DBE_ALARM | DBE_PROPERTY
#else
#define MASK_CTRL DBE_VALUE | DBE_LOG | DBE_ALARM
#endif

//To match alarmString.h in EPICS
const unsigned short ALARM_SEVERITY_STRING_LENGTH =  4; // "NO_ALARM","MINOR","MAJOR", "INVALID"
const unsigned short ALARM_STATUS_STRING_LENGTH   = 22; // "NO_ALARM","READ","WRITE",
//"HIHI","HIGH","LOLO","LOW","STATE","COS","COMM","TIMEOUT","HWLIMIT","CALC","SCAN","LINK",
//"SOFT","BAD_SUB","UDF","DISABLE","SIMM","READ_ACCESS","WRITE_ACCESS"


/**
 *  Define pCallback \n
 */
typedef void (*pCallback) (struct event_handler_args ARGS);
typedef void (*pCallbackConnection) (struct connection_handler_args ARGS);


const bool ADD_EXCEPTION_EVENT=false; //set to false for use with MATLAB
// Standard exception event is good enough
/**
 *  Size of char * array to hold pv and pvAlias names \n
 */
const short PVNAME_SIZE=MAX_STRING_SIZE +20; //from epicsTypes.h 40;
const short PVGROUP_PSEUDO_SIZE=1024; //determines max size of groupNameEntry for Pseudo groups

//List all possible deliminators; code will separate dev/attriby when first of this list is found;
const std::string DEFAULT_DEVICE_ATTRIBUTE_DELIMINATOR=":";  //Only one deliminator

typedef char pv_string_t[PVNAME_SIZE]; 

/**
 *  Default configuration parameters. Recall that if pend_event is too short \n
 *  then create callback function will still be called after the specified period
 */
const bool              DEFAULT_SELF_GOVERNING_TIMEOUT    = true;

const double            DEFAULT_TIMEOUT_PEND_IO_WF        =  5.0;
const double                    TIMEOUT_PEND_IO_MIN       =  0.0001;
const double                    TIMEOUT_PEND_IO_MAX       = 20.0; // max timeout for first attempt (ntries=1)
const unsigned short    DEFAULT_PEND_IO_NO_TRIES          =  2;
const unsigned short            PEND_IO_MAX_TRIES         = 10;   // safety net
const double            DEFAULT_PEND_IO_INCREMENT_TIME    =  2.0;
const double                    PEND_IO_INCREMENT_TIME_MIN=  0.05;
const double                    PEND_IO_INCREMENT_TIME_MAX= 10.0;
const double            DEFAULT_TIMEOUT_PEND_EVENT        =  0.4;
const double            DEFAULT_TIMEOUT_PEND_IO           =  0.4;  // previously 3.0
const double            DEFAULT_TIMEOUT_SG_PEND_EVENT     =  0.5; // plus nMember/NMEMBER_PER_SEC_SG_PEND_EVENT
const double            NMEMBER_PER_SEC_SG_PEND_EVENT     =  400; // 
const double            DEFAULT_TIMEOUT_SG_PEND_IO        =  5.0;

// Otherwise too many large databuffers are created
const unsigned int          MAX_NELEM_FOR_CTRL_BUFFER   = 8192;
// Keep at 1, else becomes problematic when wf is made a syn. group member (ch disconnect occurs for STSACK cb)
const unsigned int          MAX_NELEM_FOR_STSACK_BUFFER = 1;  
const unsigned int  	DEFAULT_NELEM_FOR_CTRL_BUFFER     = 256; // if nelem exceeds above
const unsigned int  	DEFAULT_NELEM_FOR_STSACK_BUFFER   = 1;   // if nelem exceeds above

const unsigned short        MAX_NO_MONITORS_PER_CHANNEL = 4;   // Can't start more than 4; 1 is more usual.

#endif // DEFINES_H
