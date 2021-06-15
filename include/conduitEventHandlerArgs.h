///
/// \file    conduitEventHandlerArgs.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef CONDUITEVENTHANDLERARGS_H
#define CONDUITEVENTHANDLERARGS_H

#include <cadef.h>
#include <conduit.h>
#include <helper.h>


/**
 *   Friend to Conduit/CAFEGroup permitting the event_handler_args.dbr data from callback fn
 *   to be recorded in hash table
 */
struct change_eventHandlerArgs
{
#define __METHOD__ "change_eventHandlerArgs"
    change_eventHandlerArgs (const struct event_handler_args & new_eventHandlerArgs) :
        new_eventHandlerArgs(new_eventHandlerArgs) {}

    void operator() (Conduit& c)
    {

      //c.eventHandlerArgs = new_eventHandlerArgs;   
        c.status= new_eventHandlerArgs.status;

        if(c.status!=ECA_NORMAL) {
	  std::cout << __METHOD__ << " STATUS IS " << c.status << std::endl;
          std::cout << __METHOD__ << " return from method ***NOT*** enforced " << std::endl;
          //return;
        }
	//std::cout << __METHOD__ << " STATUS IS " << c.status << std::endl;

        Helper helper;

        chtype requestedT= new_eventHandlerArgs.type%DBR_STS_STRING;
        chtype bufferType= new_eventHandlerArgs.type; //default
        //Fill with what actually came back!
        //Let is do comparison!
        CAFENUM::DBR_TYPE dbrTypeClass=helper.convertToCAFEDbrTypeClass(new_eventHandlerArgs.type);

        c.usrArgs = new_eventHandlerArgs.usr;
        c.dataType= requestedT;
        c.dbrDataType= bufferType;
        c.cafeDbrType= dbrTypeClass;

        c.hasNewData=true; //flag used by getMonitorAction for CAFE extensions!


        if (new_eventHandlerArgs.type < DBR_GR_STRING)
        {
            bufferType = dbf_type_to_DBR_TIME(requestedT);
        }
        else if (new_eventHandlerArgs.type < DBR_PUT_ACKT)
        {
            bufferType = dbf_type_to_DBR_CTRL(requestedT);
        }
        else if (new_eventHandlerArgs.type < (LAST_BUFFER_TYPE+1))
        {
            // keep default
        }
        else
        {
            std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
            std::cout << "CAFE INTERNAL FUNNY: UNKNOWN event_handler_args.type= "
                      <<  new_eventHandlerArgs.type << std::endl;
        }

        // A safety check that the buffer size is indeed large enough to hold the data

        unsigned int nrequired  = dbr_size_n(bufferType, new_eventHandlerArgs.count);
        unsigned int navailable = nrequired;


        switch(dbrTypeClass)
        {
        case CAFENUM::DBR_STSACK:
            navailable =   c.channelRequestMetaSTSACK.byteSize;
            c.channelRequestMetaSTSACK.nelem        =  new_eventHandlerArgs.count;
            c.channelRequestMetaSTSACK.dataType     =  requestedT;
            c.channelRequestMetaSTSACK.dbrDataType  =  new_eventHandlerArgs.type;
            c.channelRequestMetaSTSACK.cafeDbrType  =  dbrTypeClass;
            c.channelRequestMetaSTSACK.usrArg       =  new_eventHandlerArgs.usr; //c.channelRequestMetaDataClient.usrArg;
            break;
        case CAFENUM::DBR_PRIMITIVE:
        case CAFENUM::DBR_STS:
        case CAFENUM::DBR_TIME:
            navailable =   c.channelRequestMetaData.byteSize;

            c.channelRequestMetaData.nelem        =  new_eventHandlerArgs.count;
            c.channelRequestMetaData.dataType     =  requestedT;
            c.channelRequestMetaData.dbrDataType  =  new_eventHandlerArgs.type;
            c.channelRequestMetaData.cafeDbrType  =  dbrTypeClass;
            c.channelRequestMetaData.usrArg       =  new_eventHandlerArgs.usr; //c.channelRequestMetaDataClient.usrArg;

            //Do this check here already
            if (nrequired > navailable)
            {
                c.channelRequestMetaData.byteSize     =  nrequired;

            }
            break;
        case CAFENUM::DBR_GR:
        case CAFENUM::DBR_CTRL:
            navailable =   c.channelRequestMetaCtrl.byteSize;
            c.channelRequestMetaCtrl.nelem        =  new_eventHandlerArgs.count;
            c.channelRequestMetaCtrl.dataType     =  requestedT;
            c.channelRequestMetaCtrl.dbrDataType  =  new_eventHandlerArgs.type;
            c.channelRequestMetaCtrl.cafeDbrType  =  dbrTypeClass;
            c.channelRequestMetaCtrl.usrArg       =  new_eventHandlerArgs.usr; //c.channelRequestMetaDataClient.usrArg;
            if (nrequired > navailable)
            {
                c.channelRequestMetaCtrl.byteSize     =  nrequired;
            }
            break;
        case CAFENUM::DBR_CLASS:
            break;
        case CAFENUM::DBR_NONE:
            break;
        default:
            std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
            std::cout << "CAFE INTERNAL FUNNY: UNKNOWN CAFENUM::DBR_TYPE= " <<  dbrTypeClass  << std::endl;
            break;
        }


        if (nrequired > navailable)
        {

            std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
            std::cout << "CHANGE OF BUFFER SIZE: FROM " << navailable << " bytes TO " << nrequired << " bytes" << std::endl;
            std::cout << "Channel-" << c.pv << " ca-name=" << ca_name(new_eventHandlerArgs.chid)
                      << " type="  <<  new_eventHandlerArgs.type
                      << " type="  << dbr_type_to_text(new_eventHandlerArgs.type) << std::endl;

            //check DataBuffers
            switch(dbrTypeClass)
            {
            case CAFENUM::DBR_STSACK: //value is of type dbr_string_t

                if (c.stsackBuffer!=NULL)
                {
                    free(c.stsackBuffer);
                }
                else
                {
                    std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
                    std::cout << "CAFE INTERNAL FUNNY: HOW CAN stsackBuffer NOT ALREADY EXIST?" << std::endl;
                    std::cout << "CREATING stsackBuffer " << std::endl;
                }

                c.stsackBuffer = (db_access_val *) malloc (nrequired);


                if (c.stsackBuffer==NULL)
                {
                    std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
                    printf ("Virtual memory exhausted for channel %s ", ca_name(c.channelID));
                    printf ("Exiting CAFE");
                    exit(0);
                }

                c.channelRequestMetaSTSACK.byteSize=nrequired;



                break;

            case CAFENUM::DBR_PRIMITIVE:
            case CAFENUM::DBR_STS:
            case CAFENUM::DBR_TIME:

                // Check that buffer is large enough! Do not expect this part ever to be invoked
                if (c.dataBuffer != NULL)
                {

                    // Only re-allocate buffer if the number of native elements has increased without the
                    // the callback function first being called. i.e. nelemNative was changed on the fly!
                    // Don't believe this is possible - oder?

                    free(c.dataBuffer);
                }
                else
                {
                    std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
                    std::cout << "CAFE INTERNAL FUNNY: HOW CAN dataBuffer NOT ALREADY EXIST?" << std::endl;
                    std::cout << "CREATING dataBuffer " << std::endl;
                }

                c.dataBuffer = (db_access_val *) malloc (nrequired);

                if (c.dataBuffer==NULL)
                {
                    std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
                    printf ("Virtual memory exhausted for channel %s ", ca_name(c.channelID));
                    printf ("Exiting CAFE");
                    exit(0);
                }
                c.channelRequestMetaData.byteSize=nrequired;


                break;

            case CAFENUM::DBR_CTRL:

                // Check that buffer is large enough! Do not expect this part ever to be invoked
                if (c.ctrlBuffer != NULL)
                {

                    // Only re-allocate buffer if the number of native elements has increased without the
                    // the callback function first being called. i.e. nelemNative was changed on the fly!
                    // Don't believe this is possible - oder?

                    free(c.ctrlBuffer);
                }
                else
                {
                    std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
                    std::cout << "CAFE INTERNAL FUNNY: HOW CAN dataBuffer NOT ALREADY EXIST?" << std::endl;
                    std::cout << "CREATING dataBuffer " << std::endl;
                }

                c.ctrlBuffer = (db_access_val *) malloc (nrequired);

                if (c.ctrlBuffer==NULL)
                {
                    std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
                    printf ("Virtual memory exhausted for channel %s ", ca_name(c.channelID));
                    printf ("Exiting CAFE");
                    exit(0);
                }

                c.channelRequestMetaCtrl.byteSize=nrequired;

                //if (new_eventHandlerArgs.count >  (long) c.channelRequestMetaCtrl.nelem) {
                //    c.channelRequestMetaCtrl.nelem = new_eventHandlerArgs.count;
                //}

                break;

            case CAFENUM::DBR_CLASS:
            default:
                break;
            } //switch
        } // if new buffer size required


        if (new_eventHandlerArgs.count >  0)
        {
            if ((unsigned int) new_eventHandlerArgs.count >   c.channelRegalia.nelem)
            {
                c.channelRegalia.nelem = (unsigned int) new_eventHandlerArgs.count;
            }
        }



        switch(dbrTypeClass)
        {
        case CAFENUM::DBR_PRIMITIVE:
            //c.hasAlarmStatus  =false;
            //c.hasAlarmSeverity=false;
            //c.hasTimeStamp    =false;
            //c.hasValue        =true;
            c.alarmStatus     =-1;
            c.alarmSeverity   =-1;
            c.ts.secPastEpoch =0;
            c.ts.nsec         =0;
            break;
        case CAFENUM::DBR_STS:
            //c.hasAlarmStatus  =true;
            //c.hasAlarmSeverity=true;
            //c.hasTimeStamp    =false;
            //c.hasValue        =true;
            c.ts.secPastEpoch =0;
            c.ts.nsec         =0;
            break;
        case CAFENUM::DBR_TIME:
            //c.hasAlarmStatus  =true;
            //c.hasAlarmSeverity=true;
            //c.hasTimeStamp    =true;
            //c.hasValue        =true;
            break;
        case CAFENUM::DBR_CTRL:
            // c.hasAlarmStatus  =true;
            // c.hasAlarmSeverity=true;
            // c.hasTimeStamp    =false;
            // c.hasValue        =true;
            break;
        case CAFENUM::DBR_STSACK:
            // c.hasAlarmStatus  =true;
            // c.hasAlarmSeverity=true;
            // c.hasTimeStamp    =false;
            // c.hasValue        =true;
            break;
        case CAFENUM::DBR_CLASS:
            // c.hasAlarmStatus  =false;
            // c.hasAlarmSeverity=false;
            // c.hasTimeStamp    =false;
            // c.hasValue        =false;
            break;
        case CAFENUM::DBR_PUT:
            break;
        case CAFENUM::DBR_NONE:
        default:
            // c.hasAlarmStatus  =false;
            // c.hasAlarmSeverity=false;
            // c.hasTimeStamp    =false;
            // c.hasValue        =false;
            std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
            std::cout << "CAFE INTERNAL ERROR. Datatype not catered for: "
                      << dbf_type_to_text(new_eventHandlerArgs.type) << std::endl;
            std::cout << new_eventHandlerArgs.type << std::endl;
            break;
        }

        //Now fill buffers

        switch(new_eventHandlerArgs.type)
        {
        case DBR_DOUBLE: //6

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->doubleval)+i))
                    = (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->doubleval)+i));
            }

            break;

        case DBR_FLOAT: // 2

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->fltval)+i))
                    =  (dbr_float_t) (*(&( ( (union db_access_val *)  new_eventHandlerArgs.dbr)->fltval)+i));
            }


            break;

        case DBR_LONG: // 5

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->longval)+i))
                    = (*(&(( (union db_access_val *) new_eventHandlerArgs.dbr)->longval)+i));
            }
            break;

        case DBR_SHORT: // 1

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->shrtval)+i))
                    = (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->shrtval)+i));
            }
            break;

        case DBR_STRING: // 0

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                strcpy ((*(&((c.dataBuffer)->strval)+i)),
                        (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->strval)+i)) );
            }
            break;

        case DBR_ENUM: // 3

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->enmval)+i))
                    = (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->enmval)+i));
            }
            break;

        case DBR_CHAR:  // 4

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->charval)+i))
                    = (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->charval)+i));
            }
            break;


        case DBR_STS_DOUBLE: // 13

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->sdblval.value)+i))
                    = (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->sdblval.value)+i));
            }
            (*(&((c.dataBuffer)->sdblval.status)))    = ((struct dbr_sts_double *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->sdblval.severity))) = ((struct dbr_sts_double *) new_eventHandlerArgs.dbr)->severity;

            c.alarmStatus  =((struct dbr_sts_double *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_sts_double *) new_eventHandlerArgs.dbr)->severity;

            break;

        case DBR_STS_FLOAT: // 9

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->sfltval.value)+i))
                    =  (dbr_float_t) (*(&( ( (union db_access_val *)  new_eventHandlerArgs.dbr)->sfltval.value)+i));
            }

            (*(&((c.dataBuffer)->sfltval.status))) = ((struct dbr_sts_float *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->sfltval.severity))) = ((struct dbr_sts_float *) new_eventHandlerArgs.dbr)->severity;

            c.alarmStatus  =((struct dbr_sts_float *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_sts_float *) new_eventHandlerArgs.dbr)->severity;

            break;

        case DBR_STS_LONG: // 12

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->slngval.value)+i))
                    = (*(&(( (union db_access_val *) new_eventHandlerArgs.dbr)->slngval.value)+i));
            }

            (*(&((c.dataBuffer)->slngval.status))) = ((struct dbr_sts_long *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->slngval.severity))) = ((struct dbr_sts_long *) new_eventHandlerArgs.dbr)->severity;

            c.alarmStatus  =((struct dbr_sts_long *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_sts_long *) new_eventHandlerArgs.dbr)->severity;

            break;

        case DBR_STS_SHORT: // 8

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->sshrtval.value)+i))
                    = (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->sshrtval.value)+i));
            }
            (*(&((c.dataBuffer)->sshrtval.status))) = ((struct dbr_sts_short *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->sshrtval.severity))) = ((struct dbr_sts_short *) new_eventHandlerArgs.dbr)->severity;

            c.alarmStatus  =((struct dbr_sts_short *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_sts_short *) new_eventHandlerArgs.dbr)->severity;
            break;

        case DBR_STS_STRING: // 7

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                strcpy ((*(&((c.dataBuffer)->sstrval.value)+i)),
                        (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->sstrval.value)+i)) );
            }
            (*(&((c.dataBuffer)->sstrval.status))) = ((struct dbr_sts_string *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->sstrval.severity))) = ((struct dbr_sts_string *) new_eventHandlerArgs.dbr)->severity;

            c.alarmStatus  =((struct dbr_sts_string *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_sts_string *) new_eventHandlerArgs.dbr)->severity;

            break;

        case DBR_STS_ENUM: // 10

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->senmval.value)+i))
                    = (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->senmval.value)+i));
            }
            (*(&((c.dataBuffer)->senmval.status))) = ((struct dbr_sts_enum *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->senmval.severity))) = ((struct dbr_sts_enum *) new_eventHandlerArgs.dbr)->severity;

            c.alarmStatus  =((struct dbr_sts_enum *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_sts_enum *) new_eventHandlerArgs.dbr)->severity;

            break;

        case DBR_STS_CHAR: // 11

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->schrval.value)+i))
                    = (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->schrval.value)+i));
            }

            (*(&((c.dataBuffer)->schrval.status))) = ((struct dbr_sts_char *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->schrval.severity))) = ((struct dbr_sts_char *) new_eventHandlerArgs.dbr)->severity;

            c.alarmStatus  =((struct dbr_sts_char *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_sts_char *) new_eventHandlerArgs.dbr)->severity;

            break;

        case DBR_TIME_DOUBLE: // 20

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->tdblval.value)+i))
                    = (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->tdblval.value)+i));
            }

            (*(&((c.dataBuffer)->tdblval.status))) = ((struct dbr_time_double *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->tdblval.severity))) = ((struct dbr_time_double *) new_eventHandlerArgs.dbr)->severity;
            (*(&((c.dataBuffer)->tdblval.stamp))) = ((struct dbr_time_double *) new_eventHandlerArgs.dbr)->stamp;

            c.alarmStatus  =((struct dbr_time_double *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_time_double *) new_eventHandlerArgs.dbr)->severity;
            c.ts =((struct dbr_time_double *) new_eventHandlerArgs.dbr)->stamp;


            break;

        case DBR_TIME_FLOAT: //16

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {

                (*(&((c.dataBuffer)->tfltval.value)+i))
                    =  (dbr_float_t) (*(&( ( (union db_access_val *)  new_eventHandlerArgs.dbr)->tfltval.value)+i));


            }

            (*(&((c.dataBuffer)->tfltval.status))) = ((struct dbr_time_float *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->tfltval.severity))) = ((struct dbr_time_float *) new_eventHandlerArgs.dbr)->severity;
            (*(&((c.dataBuffer)->tfltval.stamp))) = ((struct dbr_time_float *) new_eventHandlerArgs.dbr)->stamp;

            c.alarmStatus  =((struct dbr_time_float *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_time_float *) new_eventHandlerArgs.dbr)->severity;
            c.ts           =((struct dbr_time_float *) new_eventHandlerArgs.dbr)->stamp;

            break;

        case DBR_TIME_LONG: //19

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->tlngval.value)+i))
                    = (*(&(( (union db_access_val *) new_eventHandlerArgs.dbr)->tlngval.value)+i));
            }

            (*(&((c.dataBuffer)->tlngval.status)))   = ((struct dbr_time_long *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->tlngval.severity))) = ((struct dbr_time_long *) new_eventHandlerArgs.dbr)->severity;
            (*(&((c.dataBuffer)->tlngval.stamp)))    = ((struct dbr_time_long *) new_eventHandlerArgs.dbr)->stamp;

            c.alarmStatus  =((struct dbr_time_long *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_time_long *) new_eventHandlerArgs.dbr)->severity;
            c.ts           =((struct dbr_time_long *) new_eventHandlerArgs.dbr)->stamp;
            break;

        case DBR_TIME_SHORT:
            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->tshrtval.value)+i))
                    = (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->tshrtval.value)+i));
            }

            (*(&((c.dataBuffer)->tshrtval.status))) = ((struct dbr_time_short *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->tshrtval.severity))) = ((struct dbr_time_short *) new_eventHandlerArgs.dbr)->severity;
            (*(&((c.dataBuffer)->tshrtval.stamp))) = ((struct dbr_time_short *) new_eventHandlerArgs.dbr)->stamp;

            c.alarmStatus  =((struct dbr_time_short *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_time_short *) new_eventHandlerArgs.dbr)->severity;
            c.ts        =((struct dbr_time_short *) new_eventHandlerArgs.dbr)->stamp;

            break;

        case DBR_TIME_STRING: //14

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                strcpy ((*(&((c.dataBuffer)->tstrval.value)+i)),
                        (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->tstrval.value)+i)) );
            }


            (*(&((c.dataBuffer)->tstrval.status))) = ((struct dbr_time_string *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->tstrval.severity))) = ((struct dbr_time_string *) new_eventHandlerArgs.dbr)->severity;
            (*(&((c.dataBuffer)->tstrval.stamp))) = ((struct dbr_time_string *) new_eventHandlerArgs.dbr)->stamp;

            c.alarmStatus  =((struct dbr_time_string *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_time_string *) new_eventHandlerArgs.dbr)->severity;
            c.ts           =((struct dbr_time_string *) new_eventHandlerArgs.dbr)->stamp;

            break;

        case DBR_TIME_ENUM: //17

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->tenmval.value)+i))
                    = (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->tenmval.value)+i));
            }


            (*(&((c.dataBuffer)->tenmval.status))) = ((struct dbr_time_enum *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->tenmval.severity))) = ((struct dbr_time_enum *) new_eventHandlerArgs.dbr)->severity;
            (*(&((c.dataBuffer)->tenmval.stamp))) = ((struct dbr_time_enum *) new_eventHandlerArgs.dbr)->stamp;

            c.alarmStatus  =((struct dbr_time_enum *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_time_enum *) new_eventHandlerArgs.dbr)->severity;
            c.ts           =((struct dbr_time_enum *) new_eventHandlerArgs.dbr)->stamp;

            break;

        case DBR_TIME_CHAR: //18

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.dataBuffer)->tchrval.value)+i))
                    = (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->tchrval.value)+i));

            }
            (*(&((c.dataBuffer)->tchrval.status))) = ((struct dbr_time_char *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.dataBuffer)->tchrval.severity))) = ((struct dbr_time_char *) new_eventHandlerArgs.dbr)->severity;
            (*(&((c.dataBuffer)->tchrval.stamp))) = ((struct dbr_time_char *) new_eventHandlerArgs.dbr)->stamp;

            c.alarmStatus  =((struct dbr_time_char *) new_eventHandlerArgs.dbr)->status;
            c.alarmSeverity=((struct dbr_time_char *) new_eventHandlerArgs.dbr)->severity;
            c.ts           =((struct dbr_time_char *) new_eventHandlerArgs.dbr)->stamp;
            break;

        case DBR_CTRL_DOUBLE: //34

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.ctrlBuffer)->cdblval.value)+i)) =
                    (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->cdblval.value)+i));
            }

            (*(&((c.ctrlBuffer)->cdblval.status)))    =
                ((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->cdblval.severity)))  =
                ((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->severity;
            (*(&((c.ctrlBuffer)->cdblval.precision))) =
                ((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->precision;
            (*(&((c.ctrlBuffer)->cdblval.RISC_pad0))) =
                (dbr_short_t) ((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->RISC_pad0; //dbr_short_t
            memcpy((*(&((c.ctrlBuffer)->cdblval.units))),
                   &(((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->units),
                   sizeof(char[MAX_UNITS_SIZE]));
            (*(&((c.ctrlBuffer)->cdblval.upper_disp_limit))) =
                ((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->upper_disp_limit;
            (*(&((c.ctrlBuffer)->cdblval.lower_disp_limit))) =
                ((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->lower_disp_limit;
            (*(&((c.ctrlBuffer)->cdblval.upper_alarm_limit))) =
                ((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->upper_alarm_limit;
            (*(&((c.ctrlBuffer)->cdblval.upper_warning_limit))) =
                ((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->upper_warning_limit;
            (*(&((c.ctrlBuffer)->cdblval.lower_warning_limit))) =
                ((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->lower_warning_limit;
            (*(&((c.ctrlBuffer)->cdblval.lower_alarm_limit))) =
                ((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->lower_alarm_limit;
            (*(&((c.ctrlBuffer)->cdblval.upper_ctrl_limit))) =
                ((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->upper_ctrl_limit;
            (*(&((c.ctrlBuffer)->cdblval.lower_ctrl_limit))) =
                ((struct dbr_ctrl_double *) new_eventHandlerArgs.dbr)->lower_ctrl_limit;

            break;

        case DBR_CTRL_LONG: //33

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.ctrlBuffer)->clngval.value)+i)) =
                    (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->clngval.value)+i));
            }

            (*(&((c.ctrlBuffer)->clngval.status)))   =
                ((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->clngval.severity))) =
                ((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->severity;

            // struct dbr_ctrl_long does not have the precision member
            // struct dbr_ctrl_long does not have the RISC_pad member
            memcpy((*(&((c.ctrlBuffer)->clngval.units))),
                   &(((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->units),
                   sizeof(char[MAX_UNITS_SIZE]));

            (*(&((c.ctrlBuffer)->clngval.upper_disp_limit))) =
                ((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->upper_disp_limit;
            (*(&((c.ctrlBuffer)->clngval.lower_disp_limit))) =
                ((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->lower_disp_limit;
            (*(&((c.ctrlBuffer)->clngval.upper_alarm_limit))) =
                ((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->upper_alarm_limit;
            (*(&((c.ctrlBuffer)->clngval.upper_warning_limit))) =
                ((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->upper_warning_limit;
            (*(&((c.ctrlBuffer)->clngval.lower_warning_limit))) =
                ((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->lower_warning_limit;
            (*(&((c.ctrlBuffer)->clngval.lower_alarm_limit))) =
                ((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->lower_alarm_limit;
            (*(&((c.ctrlBuffer)->clngval.upper_ctrl_limit))) =
                ((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->upper_ctrl_limit;
            (*(&((c.ctrlBuffer)->clngval.lower_ctrl_limit))) =
                ((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->lower_ctrl_limit;

            //c.alarmStatus  =((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->status;
            //c.alarmSeverity=((struct dbr_ctrl_long *) new_eventHandlerArgs.dbr)->severity;

            break;


        case DBR_CTRL_CHAR: //32

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.ctrlBuffer)->gchrval.value)+i)) =
                    (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->cchrval.value)+i));
            }

            (*(&((c.ctrlBuffer)->cchrval.status)))   =
                ((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->cchrval.severity))) =
                ((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->severity;

            // struct dbr_ctrl_char does not have the precision member
            // struct dbr_ctrl_char does not have the RISC_pad member
            memcpy((*(&((c.ctrlBuffer)->cchrval.units))),
                   &(((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->units),
                   sizeof(char[MAX_UNITS_SIZE]));

            (*(&((c.ctrlBuffer)->cchrval.upper_disp_limit))) =
                ((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->upper_disp_limit;
            (*(&((c.ctrlBuffer)->cchrval.lower_disp_limit))) =
                ((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->lower_disp_limit;
            (*(&((c.ctrlBuffer)->cchrval.upper_alarm_limit))) =
                ((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->upper_alarm_limit;
            (*(&((c.ctrlBuffer)->cchrval.upper_warning_limit))) =
                ((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->upper_warning_limit;
            (*(&((c.ctrlBuffer)->cchrval.lower_warning_limit))) =
                ((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->lower_warning_limit;
            (*(&((c.ctrlBuffer)->cchrval.lower_alarm_limit))) =
                ((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->lower_alarm_limit;
            (*(&((c.ctrlBuffer)->cchrval.upper_ctrl_limit))) =
                ((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->upper_ctrl_limit;
            (*(&((c.ctrlBuffer)->cchrval.lower_ctrl_limit))) =
                ((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->lower_ctrl_limit;

            // c.alarmStatus  =((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->status;
            //c.alarmSeverity=((struct dbr_ctrl_char *) new_eventHandlerArgs.dbr)->severity;

            break;


        case DBR_CTRL_ENUM: //31

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.ctrlBuffer)->cenmval.value)+i)) =
                    (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->cenmval.value)+i));
            }

            (*(&((c.ctrlBuffer)->cenmval.status)))   =
                ((struct dbr_ctrl_enum *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->cenmval.severity))) =
                ((struct dbr_ctrl_enum *) new_eventHandlerArgs.dbr)->severity;

            // struct dbr_ctrl_enum does not have the precision member
            // struct dbr_ctrl_enum does not have the RISC_pad member
            // struct dbr_ctrl_enum does not have the disp/alarm/warning/ctrl _limits

            (*(&((c.ctrlBuffer)->cenmval.no_str)))   =
                ((struct dbr_ctrl_enum *) new_eventHandlerArgs.dbr)->no_str;

            memcpy( (*(&((c.ctrlBuffer)->cenmval.strs))),
                    &(((struct dbr_ctrl_enum *) new_eventHandlerArgs.dbr)->strs),
                    sizeof(char)*MAX_ENUM_STRING_SIZE*MAX_ENUM_STATES) ;



            memcpy(c.channelEnumStrings.options, &(((struct dbr_ctrl_enum *) new_eventHandlerArgs.dbr)->strs),
                   sizeof(c.channelEnumStrings.options));
            c.channelEnumStrings.noOptions=((struct dbr_ctrl_enum *) new_eventHandlerArgs.dbr)->no_str;
            //std::cout << "=====================" << c.channelEnumStrings.noOptions << std::endl;
            //for (int i=0; i<c.channelEnumStrings.noOptions; ++i) std::cout << c.channelEnumStrings.options[i] << std::endl;

            break;


        case DBR_CTRL_FLOAT: //30

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.ctrlBuffer)->cfltval.value)+i)) =
                    (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->cfltval.value)+i));
            }

            (*(&((c.ctrlBuffer)->cfltval.status)))   =
                ((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->cfltval.severity))) =
                ((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->severity;


            (*(&((c.ctrlBuffer)->cfltval.precision))) =
                ((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->precision;
            (*(&((c.ctrlBuffer)->cfltval.RISC_pad))) = (dbr_short_t)
                    ((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->RISC_pad; //dbr_short_t

            memcpy((*(&((c.ctrlBuffer)->cfltval.units))),
                   &(((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->units),
                   sizeof(char[MAX_UNITS_SIZE]));

            (*(&((c.ctrlBuffer)->cfltval.upper_disp_limit))) =
                ((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->upper_disp_limit;
            (*(&((c.ctrlBuffer)->cfltval.lower_disp_limit))) =
                ((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->lower_disp_limit;
            (*(&((c.ctrlBuffer)->cfltval.upper_alarm_limit))) =
                ((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->upper_alarm_limit;
            (*(&((c.ctrlBuffer)->cfltval.upper_warning_limit))) =
                ((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->upper_warning_limit;
            (*(&((c.ctrlBuffer)->cfltval.lower_warning_limit))) =
                ((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->lower_warning_limit;
            (*(&((c.ctrlBuffer)->cfltval.lower_alarm_limit))) =
                ((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->lower_alarm_limit;
            (*(&((c.ctrlBuffer)->cfltval.upper_ctrl_limit))) =
                ((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->upper_ctrl_limit;
            (*(&((c.ctrlBuffer)->cfltval.lower_ctrl_limit))) =
                ((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->lower_ctrl_limit;


            //c.alarmStatus  =((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->status;
            //c.alarmSeverity=((struct dbr_ctrl_float *) new_eventHandlerArgs.dbr)->severity;

            break;


        case DBR_CTRL_SHORT: //29

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.ctrlBuffer)->gshrtval.value)+i)) =
                    (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->cshrtval.value)+i));
            }

            (*(&((c.ctrlBuffer)->cshrtval.status)))   =
                ((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->cshrtval.severity))) =
                ((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->severity;

            // struct dbr_ctrl_short does not have the precision member
            // struct dbr_ctrl_short does not have the RISC_pad member
            memcpy((*(&((c.ctrlBuffer)->cshrtval.units))),
                   &(((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->units),
                   sizeof(char[MAX_UNITS_SIZE]));

            (*(&((c.ctrlBuffer)->cshrtval.upper_disp_limit))) =
                ((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->upper_disp_limit;
            (*(&((c.ctrlBuffer)->cshrtval.lower_disp_limit))) =
                ((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->lower_disp_limit;
            (*(&((c.ctrlBuffer)->cshrtval.upper_alarm_limit))) =
                ((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->upper_alarm_limit;
            (*(&((c.ctrlBuffer)->cshrtval.upper_warning_limit))) =
                ((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->upper_warning_limit;
            (*(&((c.ctrlBuffer)->cshrtval.lower_warning_limit))) =
                ((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->lower_warning_limit;
            (*(&((c.ctrlBuffer)->cshrtval.lower_alarm_limit))) =
                ((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->lower_alarm_limit;
            (*(&((c.ctrlBuffer)->cshrtval.upper_ctrl_limit))) =
                ((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->upper_ctrl_limit;
            (*(&((c.ctrlBuffer)->cshrtval.lower_ctrl_limit))) =
                ((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->lower_ctrl_limit;


            //c.alarmStatus  =((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->status;
            //c.alarmSeverity=((struct dbr_ctrl_short *) new_eventHandlerArgs.dbr)->severity;

            break;


        case DBR_CTRL_STRING: //28


            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                strcpy( (*(&((c.ctrlBuffer)->cstrval.value)+i)),
                        (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->cstrval.value)+i)));
            }

            (*(&((c.ctrlBuffer)->cstrval.status)))   =
                ((struct dbr_sts_string *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->cstrval.severity))) =
                ((struct dbr_sts_short *) new_eventHandlerArgs.dbr)->severity;

            //c.alarmStatus  =((struct dbr_sts_string *) new_eventHandlerArgs.dbr)->status;
            //c.alarmSeverity=((struct dbr_sts_string *) new_eventHandlerArgs.dbr)->severity;


            break;

        case DBR_GR_DOUBLE: //27

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.ctrlBuffer)->gdblval.value)+i)) =
                    (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->gdblval.value)+i));
            }

            (*(&((c.ctrlBuffer)->gdblval.status)))    =
                ((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->gdblval.severity)))  =
                ((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->severity;

            (*(&((c.ctrlBuffer)->gdblval.precision))) =
                ((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->precision;

            (*(&((c.ctrlBuffer)->gdblval.RISC_pad0))) =
                (dbr_short_t) ((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->RISC_pad0; //dbr_short_t

            memcpy((*(&((c.ctrlBuffer)->gdblval.units))),
                   &(((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->units),
                   sizeof(char[MAX_UNITS_SIZE]));

            (*(&((c.ctrlBuffer)->gdblval.upper_disp_limit))) =
                ((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->upper_disp_limit;
            (*(&((c.ctrlBuffer)->gdblval.lower_disp_limit))) =
                ((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->lower_disp_limit;
            (*(&((c.ctrlBuffer)->gdblval.upper_alarm_limit))) =
                ((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->upper_alarm_limit;
            (*(&((c.ctrlBuffer)->gdblval.upper_warning_limit))) =
                ((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->upper_warning_limit;
            (*(&((c.ctrlBuffer)->gdblval.lower_warning_limit))) =
                ((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->lower_warning_limit;
            (*(&((c.ctrlBuffer)->gdblval.lower_alarm_limit))) =
                ((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->lower_alarm_limit;

            //c.alarmStatus  =((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->status;
            //c.alarmSeverity=((struct dbr_gr_double *) new_eventHandlerArgs.dbr)->severity;

            break;

        case DBR_GR_LONG: //26

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.ctrlBuffer)->glngval.value)+i)) =
                    (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->glngval.value)+i));
            }

            (*(&((c.ctrlBuffer)->glngval.status)))   =
                ((struct dbr_gr_long *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->glngval.severity))) =
                ((struct dbr_gr_long *) new_eventHandlerArgs.dbr)->severity;

            // struct dbr_gr_long does not have the precision member
            // struct dbr_gr_long does not have the RISC_pad member
            memcpy((*(&((c.ctrlBuffer)->glngval.units))),
                   &(((struct dbr_gr_long *) new_eventHandlerArgs.dbr)->units),
                   sizeof(char[MAX_UNITS_SIZE]));

            (*(&((c.ctrlBuffer)->glngval.upper_disp_limit))) =
                ((struct dbr_gr_long *) new_eventHandlerArgs.dbr)->upper_disp_limit;
            (*(&((c.ctrlBuffer)->glngval.lower_disp_limit))) =
                ((struct dbr_gr_long *) new_eventHandlerArgs.dbr)->lower_disp_limit;
            (*(&((c.ctrlBuffer)->glngval.upper_alarm_limit))) =
                ((struct dbr_gr_long *) new_eventHandlerArgs.dbr)->upper_alarm_limit;
            (*(&((c.ctrlBuffer)->glngval.upper_warning_limit))) =
                ((struct dbr_gr_long *) new_eventHandlerArgs.dbr)->upper_warning_limit;
            (*(&((c.ctrlBuffer)->glngval.lower_warning_limit))) =
                ((struct dbr_gr_long *) new_eventHandlerArgs.dbr)->lower_warning_limit;
            (*(&((c.ctrlBuffer)->glngval.lower_alarm_limit))) =
                ((struct dbr_gr_long *) new_eventHandlerArgs.dbr)->lower_alarm_limit;

            //c.alarmStatus  =((struct dbr_gr_long *) new_eventHandlerArgs.dbr)->status;
            //c.alarmSeverity=((struct dbr_gr_long *) new_eventHandlerArgs.dbr)->severity;

            break;


        case DBR_GR_CHAR: //25

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.ctrlBuffer)->gchrval.value)+i)) =
                    (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->gchrval.value)+i));
            }

            (*(&((c.ctrlBuffer)->gchrval.status)))   =
                ((struct dbr_gr_char *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->gchrval.severity))) =
                ((struct dbr_gr_char *) new_eventHandlerArgs.dbr)->severity;

            // struct dbr_gr_char does not have the precision member
            // struct dbr_gr_char does not have the RISC_pad member
            memcpy((*(&((c.ctrlBuffer)->gchrval.units))),
                   &(((struct dbr_gr_char *) new_eventHandlerArgs.dbr)->units),
                   sizeof(char[MAX_UNITS_SIZE]));

            (*(&((c.ctrlBuffer)->gchrval.upper_disp_limit))) =
                ((struct dbr_gr_char *) new_eventHandlerArgs.dbr)->upper_disp_limit;
            (*(&((c.ctrlBuffer)->gchrval.lower_disp_limit))) =
                ((struct dbr_gr_char *) new_eventHandlerArgs.dbr)->lower_disp_limit;
            (*(&((c.ctrlBuffer)->gchrval.upper_alarm_limit))) =
                ((struct dbr_gr_char *) new_eventHandlerArgs.dbr)->upper_alarm_limit;
            (*(&((c.ctrlBuffer)->gchrval.upper_warning_limit))) =
                ((struct dbr_gr_char *) new_eventHandlerArgs.dbr)->upper_warning_limit;
            (*(&((c.ctrlBuffer)->gchrval.lower_warning_limit))) =
                ((struct dbr_gr_char *) new_eventHandlerArgs.dbr)->lower_warning_limit;
            (*(&((c.ctrlBuffer)->gchrval.lower_alarm_limit))) =
                ((struct dbr_gr_char *) new_eventHandlerArgs.dbr)->lower_alarm_limit;

            //c.alarmStatus  =((struct dbr_gr_char *) new_eventHandlerArgs.dbr)->status;
            //c.alarmSeverity=((struct dbr_gr_char *) new_eventHandlerArgs.dbr)->severity;

            break;


        case DBR_GR_ENUM: //24

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.ctrlBuffer)->genmval.value)+i)) =
                    (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->genmval.value)+i));
            }

            (*(&((c.ctrlBuffer)->genmval.status)))   =
                ((struct dbr_gr_enum *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->genmval.severity))) =
                ((struct dbr_gr_enum *) new_eventHandlerArgs.dbr)->severity;

            // struct dbr_gr_enum does not have the precision member
            // struct dbr_gr_enum does not have the RISC_pad member
            // struct dbr_gr_enum does not have the disp/alarm/warning/ctrl _limits

            (*(&((c.ctrlBuffer)->genmval.no_str)))   =
                ((struct dbr_gr_enum *) new_eventHandlerArgs.dbr)->no_str;

            memcpy( (*(&((c.ctrlBuffer)->genmval.strs))),
                    &(((struct dbr_gr_enum *) new_eventHandlerArgs.dbr)->strs),
                    sizeof(char)*MAX_ENUM_STRING_SIZE*MAX_ENUM_STATES) ;

            // c.alarmStatus  =((struct dbr_gr_enum *) new_eventHandlerArgs.dbr)->status;
            //c.alarmSeverity=((struct dbr_gr_enum *) new_eventHandlerArgs.dbr)->severity;

            memcpy(c.channelEnumStrings.options, &(((struct dbr_gr_enum *) new_eventHandlerArgs.dbr)->strs),
                   sizeof(c.channelEnumStrings.options));
            c.channelEnumStrings.noOptions=((struct dbr_gr_enum *) new_eventHandlerArgs.dbr)->no_str;
            //std::cout << "=====================" << c.channelEnumStrings.noOptions << std::endl;
            //for (int i=0; i<c.channelEnumStrings.noOptions; ++i) std::cout << c.channelEnumStrings.options[i] << std::endl;

            break;


        case DBR_GR_FLOAT: //23

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.ctrlBuffer)->gfltval.value)+i)) =
                    (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->gfltval.value)+i));
            }

            (*(&((c.ctrlBuffer)->gfltval.status)))   =
                ((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->gfltval.severity))) =
                ((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->severity;


            (*(&((c.ctrlBuffer)->gfltval.precision))) =
                ((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->precision;
            (*(&((c.ctrlBuffer)->gfltval.RISC_pad0))) =
                (dbr_short_t) ((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->RISC_pad0; //dbr_short_t

            memcpy((*(&((c.ctrlBuffer)->gfltval.units))),
                   &(((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->units),
                   sizeof(char[MAX_UNITS_SIZE]));

            (*(&((c.ctrlBuffer)->gfltval.upper_disp_limit))) =
                ((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->upper_disp_limit;
            (*(&((c.ctrlBuffer)->gfltval.lower_disp_limit))) =
                ((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->lower_disp_limit;
            (*(&((c.ctrlBuffer)->gfltval.upper_alarm_limit))) =
                ((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->upper_alarm_limit;
            (*(&((c.ctrlBuffer)->gfltval.upper_warning_limit))) =
                ((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->upper_warning_limit;
            (*(&((c.ctrlBuffer)->gfltval.lower_warning_limit))) =
                ((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->lower_warning_limit;
            (*(&((c.ctrlBuffer)->gfltval.lower_alarm_limit))) =
                ((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->lower_alarm_limit;


            // c.alarmStatus  =((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->status;
            //c.alarmSeverity=((struct dbr_gr_float *) new_eventHandlerArgs.dbr)->severity;

            break;


        case DBR_GR_SHORT: //22

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                (*(&((c.ctrlBuffer)->gshrtval.value)+i)) =
                    (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->gshrtval.value)+i));
            }

            (*(&((c.ctrlBuffer)->gshrtval.status)))   =
                ((struct dbr_gr_short *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->gshrtval.severity))) =
                ((struct dbr_gr_short *) new_eventHandlerArgs.dbr)->severity;

            // struct dbr_gr_short does not have the precision member
            // struct dbr_gr_short does not have the RISC_pad member
            memcpy((*(&((c.ctrlBuffer)->gshrtval.units))),
                   &(((struct dbr_gr_short *) new_eventHandlerArgs.dbr)->units),
                   sizeof(char[MAX_UNITS_SIZE]));

            (*(&((c.ctrlBuffer)->gshrtval.upper_disp_limit))) =
                ((struct dbr_gr_short *) new_eventHandlerArgs.dbr)->upper_disp_limit;
            (*(&((c.ctrlBuffer)->gshrtval.lower_disp_limit))) =
                ((struct dbr_gr_short *) new_eventHandlerArgs.dbr)->lower_disp_limit;
            (*(&((c.ctrlBuffer)->gshrtval.upper_alarm_limit))) =
                ((struct dbr_gr_short *) new_eventHandlerArgs.dbr)->upper_alarm_limit;
            (*(&((c.ctrlBuffer)->gshrtval.upper_warning_limit))) =
                ((struct dbr_gr_short *) new_eventHandlerArgs.dbr)->upper_warning_limit;
            (*(&((c.ctrlBuffer)->gshrtval.lower_warning_limit))) =
                ((struct dbr_gr_short *) new_eventHandlerArgs.dbr)->lower_warning_limit;
            (*(&((c.ctrlBuffer)->gshrtval.lower_alarm_limit))) =
                ((struct dbr_gr_short *) new_eventHandlerArgs.dbr)->lower_alarm_limit;

            // c.alarmStatus  =((struct dbr_gr_short *) new_eventHandlerArgs.dbr)->status;
            // c.alarmSeverity=((struct dbr_gr_short *) new_eventHandlerArgs.dbr)->severity;

            break;


        case DBR_GR_STRING: //21

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                strcpy( (*(&((c.ctrlBuffer)->gstrval.value)+i)),
                        (*(&( ((union db_access_val *)  new_eventHandlerArgs.dbr)->gstrval.value)+i)));
            }

            (*(&((c.ctrlBuffer)->gstrval.status)))   =
                ((struct dbr_sts_string *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.ctrlBuffer)->gstrval.severity))) =
                ((struct dbr_sts_short *) new_eventHandlerArgs.dbr)->severity;

            // c.alarmStatus  =((struct dbr_sts_string *) new_eventHandlerArgs.dbr)->status;
            // c.alarmSeverity=((struct dbr_sts_string *) new_eventHandlerArgs.dbr)->severity;

            break;

        case DBR_STSACK_STRING: // 37

            // std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;


            // std::cout << "no of events = " << new_eventHandlerArgs.count << std::endl;
            // std::cout << "Is databuffer large enough? " << std::endl;
            // Need to cast as (struct dbr_stsack_string *) rather than (union db_access_val *)
            //

            //CHECK BUFFER SIZE!!!

            // std::cout <<  "VALUE (struct) = " <<
            //        (*(&(((struct dbr_stsack_string *) new_eventHandlerArgs.dbr)->value)+0))
            //        << std::endl;

            for (long i=0; i<new_eventHandlerArgs.count; ++i)
            {
                strcpy ((*(&((c.stsackBuffer)->sastrval.value)+i)),
                        (*(&(((struct dbr_stsack_string *) new_eventHandlerArgs.dbr)->value)+i)) );
                //std::cout << " VAL  " << (*(&((c.stsackBuffer)->sastrval.value)+i)) << " [" << i << "] ";
            }

            //std::cout << std::endl;

            (*(&((c.stsackBuffer)->sastrval.status)))   = (dbr_ushort_t) ((struct dbr_stsack_string *) new_eventHandlerArgs.dbr)->status;
            (*(&((c.stsackBuffer)->sastrval.severity))) = (dbr_ushort_t) ((struct dbr_stsack_string *) new_eventHandlerArgs.dbr)->severity;




            //std::cout << "Value STSACK " <<  (*(&((c.stsackBuffer)->sastrval.value)+0)) << std::endl;
            //std::cout << "Value STSACK " <<  (*(&(((struct dbr_stsack_string *) new_eventHandlerArgs.dbr)->value)+0))<< std::endl;
            //std::cout << "ALARM STATUS  " <<  (*(&((c.stsackBuffer)->sastrval.status))) << std::endl;
            //std::cout << "ALARM SEVERITY " <<  (*(&((c.stsackBuffer)->sastrval.severity))) << std::endl;

            //std::cout << "VAL ACKT " <<  (dbr_ushort_t) ( (struct dbr_stsack_string *) new_eventHandlerArgs.dbr)->ackt << std::endl;

            //std::cout << "VAL ACKS " <<  (dbr_ushort_t)  ( (struct dbr_stsack_string *) new_eventHandlerArgs.dbr)->acks << std::endl;



            // c.alarmStatus  =((struct dbr_stsack_string *) new_eventHandlerArgs.dbr)->status;
            // c.alarmSeverity=((struct dbr_stsack_string *) new_eventHandlerArgs.dbr)->severity;


            break;

        case DBR_CLASS_NAME: // 37

            // Need to cast as (struct dbr_stsack_string *) rather than (union db_access_val *)
            //

            // strcpy ((c.dataBuffer)->classname ,
            // (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->classname)+0)) );


            // std::cout << "Value CLASSNAME " <<  strlen((*(&((c.dataBuffer)->classname)+0)) )<< std::endl;
            // std::cout << "Value CLASSNAME " <<  ((*(&((c.dataBuffer)->classname)+0)) )<< std::endl;
            // std::cout << "Value CLASSNAME " <<   strlen((*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->classname)+0)))<< "/"<<std::endl;


            ///strcpy( c.className, (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->classname)+0)));
            strcpy( c.channelRegalia.className,
                    (*(&(((union db_access_val *) new_eventHandlerArgs.dbr)->classname)+0)));

            //std::cout << "ClassName=" << c.channelRegalia.className << std::endl;

            break;


        default:
            std::cout << "CAFE INTERNAL ERROR: This line should never appear! " << std::endl;
            std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
            std::cout << "Method does not support this DBR_TYPE: "
                      << dbr_type_to_text(new_eventHandlerArgs.type) << std::endl;

            //break;

        } //switch


        //Do this to prevent overflow error in epicsTime time(ts) routines!
        //This bad number can occur in timeouts
        if(c.ts.nsec>1000000000)
        {
            c.ts.nsec=0;
        }


    } // void



private:
    event_handler_args new_eventHandlerArgs;
#undef __METHOD__
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif // CONDUITEVENTHANDLERARGS_H
