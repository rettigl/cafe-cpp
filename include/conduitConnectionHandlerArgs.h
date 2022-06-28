///
/// \file    conduitConnectionHandlerArgs.h
/// \author  Jan Chrin, PSI
/// \date    November 2014
/// \version CAFE 1.0.0
///

#ifndef CONDUITCONNECTIONHANDLERARGS_H
#define CONDUITCONNECTIONHANDLERARGS_H

#include <cstdio>
#include <conduit.h>
#include <statusCodes.h>

/**
 *   Friend to Conduit/CAFEGroup records the connection_handler_args struct from callback fns
 *   in hash table
 *   Function is not invoked on closechannel by user
 */
struct change_connectionHandlerArgs
{
#define __METHOD__ "change_connectionHandlerArgs"
    change_connectionHandlerArgs (const struct connection_handler_args & new_connectionHandlerArgs):
        new_connectionHandlerArgs(new_connectionHandlerArgs) {}

    void operator() (Conduit& c)
    {
        chtype nativeDataType = ca_field_type(new_connectionHandlerArgs.chid);

        //c.connectionHandlerArgs = new_connectionHandlerArgs;
        c.channelRegalia.connectionState=new_connectionHandlerArgs.op;

        // Data type code will be one of DBF_. The constant TYPENOTCONN=-1 is
        // returned if the channel is not connected
        // but we do not overwrite it upon disconnect.

	//std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
      	//std::cout <<   new_connectionHandlerArgs.op << std::endl;
        //connectFlag
        if  (new_connectionHandlerArgs.op == CA_OP_CONN_UP)
        {

            //std::cout << " change_connectionHandlerArgs: bytesize UP " << c.channelRequestMetaData.byteSize << std::endl;
            //channelRegalia
            c.channelRegalia.nelem                  = ca_element_count(new_connectionHandlerArgs.chid);
            c.channelRegalia.connectFlag            = true;
            c.channelRegalia.hostName               = (const char *) ca_host_name (new_connectionHandlerArgs.chid);	    

            if (c.channelRegalia.channelID != new_connectionHandlerArgs.chid)
            {
                std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                std::cout <<  "Internal CAFE WARNING for handle : " <<  c.handle << std::endl;
                std::cout <<  "Channel ID has changed from " << c.channelRegalia.channelID
                          << " to " << new_connectionHandlerArgs.chid << "  " << std::endl;
                std::cout << "This is a rare occurence and happens when the ca message buffer " << std::endl;
                std::cout << "is flushed ahead of schedule (does that when full) and this callback is " << std::endl;
                std::cout << "consequently activated before channel ID is written to the hash table " << std::endl;
                c.channelRegalia.channelID              = new_connectionHandlerArgs.chid;
            }


            //Data type code will be one of DBF_. The constant TYPENOTCONN=-1 is
            //returned if the channel is not connected.
            //Does not get overwritten on channel disconnection
            c.channelRegalia.dataType               = nativeDataType;


            // DATA BUFFER ------------------------------------------------------------------
            // data buffer CLIENT
            // Check if c.channelRegalia.cafeConnectionState == ICAFE_CS_NEVER_CONN or not!


            if (c.channelRegalia.cafeConnectionState  == ICAFE_CS_NEVER_CONN )
            {
                c.channelRequestMetaDataClient.channelID   = new_connectionHandlerArgs.chid;
                c.channelRequestMetaDataClient.nelem       = c.channelRegalia.nelem;
                c.channelRequestMetaDataClient.nelemCache  = c.channelRegalia.nelem;
                c.channelRequestMetaDataClient.dataType    = nativeDataType;

                //cafeDbrType first filled with CAFENUM:DBR_TIME on initialization
                //but will be overwritten by whatever the client needs
                switch (c.channelRequestMetaDataClient.cafeDbrType)
                {
                case CAFENUM::DBR_TIME:
                    c.channelRequestMetaDataClient.dbrDataType = dbf_type_to_DBR_TIME(nativeDataType);
                    break;
                case CAFENUM::DBR_STS:
                    c.channelRequestMetaDataClient.dbrDataType = dbf_type_to_DBR_STS(nativeDataType);
                    break;
                case CAFENUM::DBR_PRIMITIVE:
                    c.channelRequestMetaDataClient.dbrDataType = dbf_type_to_DBR(nativeDataType);
                    break;
                default:
                    c.channelRequestMetaDataClient.dbrDataType   = dbf_type_to_DBR_TIME(nativeDataType);
                    c.channelRequestMetaDataClient.cafeDbrType   = CAFENUM::DBR_TIME;
                    break;
                }

                //What client is actutally requesting
                c.channelRequestMetaDataClient.byteSize=dbr_size_n(
                        c.channelRequestMetaDataClient.dbrDataType,
                        //dbf_type_to_DBR_TIME(nativeDataType),
                        c.channelRequestMetaDataClient.nelem);
            }

            //data Buffer requested by Cafe
            c.channelRequestMetaData.channelID   = new_connectionHandlerArgs.chid;
            c.channelRequestMetaData.nelem       = c.channelRegalia.nelem;
            c.channelRequestMetaData.nelemCache  = c.channelRegalia.nelem;
            c.channelRequestMetaData.dataType    = nativeDataType;

            //cafeDbrType first filled with CAFENUM:DBR_TIME on initialization
            //cafeDbrType can only be overwritten by an explicit method invocation
            switch (c.channelRequestMetaData.cafeDbrType)
            {
            case CAFENUM::DBR_TIME:
                c.channelRequestMetaData.dbrDataType = dbf_type_to_DBR_TIME(nativeDataType);
                break;
            case CAFENUM::DBR_STS:
                c.channelRequestMetaData.dbrDataType = dbf_type_to_DBR_STS(nativeDataType);
                break;
            case CAFENUM::DBR_PRIMITIVE:
                c.channelRequestMetaData.dbrDataType = dbf_type_to_DBR(nativeDataType);
                break;
            default:
                c.channelRequestMetaData.dbrDataType   = dbf_type_to_DBR_TIME(nativeDataType);
                c.channelRequestMetaData.cafeDbrType   = CAFENUM::DBR_TIME;
                break;
            }



            // CTRL BUFFER ------------------------------------------------------------------

            //No of elements for Ctrl Buffers
            unsigned int  nelem_ctrl_buffer=1;

            if ( c.channelRegalia.nelem > MAX_NELEM_FOR_CTRL_BUFFER)
            {
                nelem_ctrl_buffer         = DEFAULT_NELEM_FOR_CTRL_BUFFER;
            }
            else
            {
                nelem_ctrl_buffer  = c.channelRegalia.nelem;
            }

            //ctrl data CLIENT
            //Ctrl data requested by Client
            if (c.channelRegalia.cafeConnectionState  == ICAFE_CS_NEVER_CONN )
            {
                c.channelRequestMetaCtrlClient.channelID   = new_connectionHandlerArgs.chid;
                c.channelRequestMetaCtrlClient.nelem       = c.channelRegalia.nelem; //nelem_ctrl_buffer;
                c.channelRequestMetaCtrlClient.nelemCache  = c.channelRegalia.nelem;
                c.channelRequestMetaCtrlClient.dataType    = nativeDataType;

                //cafeDbrType first filled with CAFENUM:DBR_CTRL on initialization
                //but will be overwritten by whatever the client needs
                switch (c.channelRequestMetaCtrlClient.cafeDbrType)
                {
                case CAFENUM::DBR_CTRL:
                    c.channelRequestMetaCtrlClient.dbrDataType = dbf_type_to_DBR_CTRL(nativeDataType);
                    break;
                case CAFENUM::DBR_GR:
                    c.channelRequestMetaCtrlClient.dbrDataType = dbf_type_to_DBR_GR  (nativeDataType);
                    break;
                default:
                    c.channelRequestMetaCtrlClient.dbrDataType = dbf_type_to_DBR_CTRL(nativeDataType);
                    c.channelRequestMetaCtrlClient.cafeDbrType = CAFENUM::DBR_CTRL;
                    break;
                }

                c.channelRequestMetaCtrlClient.byteSize=dbr_size_n(
                        c.channelRequestMetaCtrlClient.dbrDataType,c.channelRequestMetaCtrlClient.nelem);
            }

            //ctrl Data requested by Cafe
            c.channelRequestMetaCtrl.channelID   = new_connectionHandlerArgs.chid;
            c.channelRequestMetaCtrl.nelem       = nelem_ctrl_buffer;
            c.channelRequestMetaCtrl.nelemCache  = nelem_ctrl_buffer;
            c.channelRequestMetaCtrl.dataType    = nativeDataType;
            //cafeDbrType first filled with CAFENUM:DBR_CTRL on initialization
            //cafeDbrType can only be overwritten by an explicit method invocation
            switch (c.channelRequestMetaCtrl.cafeDbrType)
            {
            case CAFENUM::DBR_CTRL:
                c.channelRequestMetaCtrl.dbrDataType = dbf_type_to_DBR_CTRL(nativeDataType);
                break;
            case CAFENUM::DBR_GR:
                c.channelRequestMetaCtrl.dbrDataType = dbf_type_to_DBR_GR  (nativeDataType);
                break;
            default:
                c.channelRequestMetaCtrl.dbrDataType = dbf_type_to_DBR_CTRL(nativeDataType);
                c.channelRequestMetaCtrl.cafeDbrType = CAFENUM::DBR_CTRL;
                break;
            }

            // STSACK BUFFER ------------------------------------------------------------------

            //No of elements for STSACK Buffers
            unsigned int  nelem_stsack_buffer;

            if ( c.channelRegalia.nelem > MAX_NELEM_FOR_STSACK_BUFFER)
            {
                nelem_stsack_buffer        = DEFAULT_NELEM_FOR_STSACK_BUFFER;
            }
            else
            {
                nelem_stsack_buffer = c.channelRegalia.nelem;
            }


            //STSACK Buffer Repository
            c.channelRequestMetaSTSACK.channelID     = new_connectionHandlerArgs.chid;
            c.channelRequestMetaSTSACK.nelem         = nelem_stsack_buffer;
            c.channelRequestMetaSTSACK.nelemCache      = nelem_stsack_buffer;
            c.channelRequestMetaSTSACK.dataType      = DBR_STRING;
            c.channelRequestMetaSTSACK.dbrDataType   = DBR_STSACK_STRING;
            c.channelRequestMetaSTSACK.cafeDbrType   = CAFENUM::DBR_STSACK;


            //PRIMITIVE Buffer Repository
            c.channelRequestMetaPrimitive.channelID  = new_connectionHandlerArgs.chid;
            c.channelRequestMetaPrimitive.nelem      = c.channelRegalia.nelem;
            c.channelRequestMetaPrimitive.dataType   = nativeDataType;;
            c.channelRequestMetaPrimitive.dbrDataType= dbf_type_to_DBR(nativeDataType);
            c.channelRequestMetaPrimitive.cafeDbrType= CAFENUM::DBR_PRIMITIVE;

            ///////////////////////////////////////////////////////////////////////////


            if ( c.channelRegalia.nelem>1)
            {
                double  tout=  ((unsigned int) (c.channelRegalia.nelem*0.000001)); // 1 sec per million
                c.channelRequestDataTypePolicy.setRequestKind(CAFENUM::LOWEST_DATATYPE);
                c.channelTimeoutPolicyGet.setTimeout(std::max(DEFAULT_TIMEOUT_PEND_IO_WF, tout));
                c.channelTimeoutPolicyPut.setTimeout(std::max(DEFAULT_TIMEOUT_PEND_IO_WF, tout));
                c.channelTimeoutPolicyGet.setDefaultTimeout(DEFAULT_TIMEOUT_PEND_IO_WF);
                c.channelTimeoutPolicyPut.setDefaultTimeout(DEFAULT_TIMEOUT_PEND_IO_WF);
            }

            c.channelRegalia.cafeConnectionState = ICAFE_CS_CONN;
            c.status = ICAFE_CA_OP_CONN_UP;
        }
        else
        {

            //nativeType not known on disconnect!!

            //Also callback done
            c.channelRequestStatusGet.setCallbackKind(false, true); //fake completion
            c.channelRequestStatusPut.setCallbackKind(false, true); //fake completion
            c.channelRegalia.cafeConnectionState   =ICAFE_CS_DISCONN;
            c.channelRegalia.connectFlag = false;
            c.status  = ICAFE_CA_OP_CONN_DOWN;

        }


    }

private:
    connection_handler_args new_connectionHandlerArgs;
#undef __METHOD__
};


/**
 *   Friend to Conduit/CAFEGroup permitting fast modification to the ctrlBuffer
 *   This is the ctrlBuffer for _CTRL data requested through ca_get
 */
struct change_dataBufferSize_CTRL
{
#define __METHOD__ "change_dataBufferSize_CTRL"
    change_dataBufferSize_CTRL (const chtype & new_ctrlTypeBuffer): new_ctrlTypeBuffer(new_ctrlTypeBuffer) {}

    void operator() (Conduit& c)
    {

        // Free buffer on re-connection
        // Check Byte size first!!!

        bool allocateMemory=false ;

        if(c.ctrlBuffer==NULL)
        {
            allocateMemory=true;
        }

        else if ( dbr_size_n(new_ctrlTypeBuffer,c.channelRequestMetaCtrl.getNelem()) > c.channelRequestMetaCtrl.getByteSize() )
        {
            std::cout << "ctrlBuffer already exists= " << c.ctrlBuffer << " for channel " << c.pv
                      << " with handle " << c.handle << std::endl;
            std::cout << "Freeing and reallocating ctrlBuffer" << std::endl;
            free(c.ctrlBuffer);
            allocateMemory=true;
        }

        if (allocateMemory)
        {
            //std::cout << "sizeof c.ctrlBuffer " << dbr_size_n(new_ctrlTypeBuffer,c.channelRequestMetaCtrl.getNelem())  << std::endl;
            c.ctrlBuffer = (db_access_val *) malloc ( dbr_size_n(new_ctrlTypeBuffer,c.channelRequestMetaCtrl.getNelem()) );
            c.channelRequestMetaCtrl.byteSize=dbr_size_n(new_ctrlTypeBuffer, c.channelRequestMetaCtrl.getNelem());
        }


        if (c.ctrlBuffer==0)
        {
            std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
            printf ("Virtual memory exhausted for channel %s ", ca_name(c.channelID));
            printf ("Exiting CAFE");
        }

    }

private:
    chtype new_ctrlTypeBuffer;
#undef __METHOD__
};


/**
 *   Friend to CAFEConduit/CAFEGroup permitting fast modification to the dataBuffer of type DBR (putBuffer)
 *   This is used in ca_put methods when input data that is not already in native data type is thus converted
 */
struct change_dataBufferSize_PRIMITIVE
{
#define __METHOD__ "change_dataBufferSize_PRIMITIVE"
    change_dataBufferSize_PRIMITIVE (const chtype & new_dataTypeBufferNative): new_dataTypeBufferNative(new_dataTypeBufferNative) {}

    void operator() (Conduit& c)
    {

        bool allocateMemory=false ;

        if(c.putBuffer==NULL)
        {
            allocateMemory=true;
        }
        else if ( dbr_size_n(c.channelRequestMetaPrimitive.getDbrDataType(),
                             c.channelRequestMetaPrimitive.getNelem())
                  > c.channelRequestMetaPrimitive.getByteSize() )
        {
            std::cout << "putBuffer already exists= " << c.putBuffer << " for channel " << c.pv
                      << " with handle " << c.handle << std::endl;
            std::cout << "Freeing and reallocating putBuffer" << std::endl;
            free(c.putBuffer);
            allocateMemory=true;
        }

        if (allocateMemory)
        {
            //std::cout << "sizeof c.putBuffer " << dbr_size_n(c.channelRequestMetaPrimitive.getDbrDataType(),
            //                                           c.channelRequestMetaPrimitive.getNelem())  << std::endl;
            c.putBuffer = (db_access_val *) malloc (dbr_size_n(c.channelRequestMetaPrimitive.getDbrDataType(),
                                                    c.channelRequestMetaPrimitive.getNelem()));

            c.channelRequestMetaPrimitive.byteSize
                =dbr_size_n(c.channelRequestMetaPrimitive.getDbrDataType(),
                            c.channelRequestMetaPrimitive.getNelem());
        }

        if (c.putBuffer==0)
        {
            std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
            printf ("Virtual memory exhausted for channel %s ", ca_name(c.channelID));
            printf ("Exiting CAFE");
            exit(0);
        }

    }

private:
    chtype  new_dataTypeBufferNative;
#undef __METHOD__
};



/**
 *   Friend to CAFEConduit/CAFEGroup permitting fast modification to the ctrlBuffer
 *   This is the ctrlBuffer for _CTRL data requested through ca_get
 */
struct change_dataBufferSize_STSACK
{
#define __METHOD__ "change_dataBufferSize_STSACK"
    change_dataBufferSize_STSACK () {}

    void operator() (Conduit& c)
    {

        bool allocateMemory=false ;

        if(c.stsackBuffer==NULL)
        {

            allocateMemory=true;
        }

        else if ( dbr_size_n(c.channelRequestMetaSTSACK.getDbrDataType(),
                             c.channelRequestMetaSTSACK.getNelem())
                  > c.channelRequestMetaSTSACK.getByteSize() )
        {
            std::cout << "stsackBuffer already exists= " << c.stsackBuffer << " for channel " << c.pv
                      << " with handle " << c.handle << std::endl;
            std::cout << "Freeing and reallocating putBuffer" << std::endl;
            free(c.stsackBuffer);
            allocateMemory=true;
        }

        if (allocateMemory)
        {
            //std::cout << "sizeof c.stsackBuffer " << dbr_size_n(c.channelRequestMetaSTSACK.getDbrDataType(),
            //                                           c.channelRequestMetaSTSACK.getNelem())  << std::endl;
            c.stsackBuffer = (db_access_val *) malloc (dbr_size_n(c.channelRequestMetaSTSACK.getDbrDataType(),
                             c.channelRequestMetaSTSACK.getNelem()));

            c.channelRequestMetaSTSACK.byteSize
                =dbr_size_n(c.channelRequestMetaSTSACK.getDbrDataType(),
                            c.channelRequestMetaSTSACK.getNelem());
        }

    }
#undef __METHOD__
};


/**
 *   Friend to CAFEConduit/CAFEGroup permitting fast modification to the dataBuffer of type DBR_TIME (dataBuffer)
 *   This is the main dataBuffer for data requested through ca_get
 */
struct change_dataBufferSize_TIME
{
#define __METHOD__ "change_dataBufferSize_PRIMITIVE"

    change_dataBufferSize_TIME (const chtype & new_dataTypeBuffer): new_dataTypeBuffer(new_dataTypeBuffer) {}

    void operator() (Conduit& c)
    {

        // Free buffer on re-connection
        // Check Byte size first!!!

        bool allocateMemory=false ;


        if(c.dataBuffer==NULL)
        {

            allocateMemory=true;
        }

        else if ( dbr_size_n(new_dataTypeBuffer,c.channelRegalia.getNelem()) > c.channelRequestMetaData.getByteSize() )
        {
            std::cout << "dataBuffer already exists= " << c.dataBuffer << " for channel " << c.pv
                      << " with handle " << c.handle << std::endl;
            std::cout << "Freeing and reallocating dataBuffer" << std::endl;

            std::cout << dbr_size_n(new_dataTypeBuffer,c.channelRegalia.getNelem()) << " VERSUS "
                      << c.channelRequestMetaData.getByteSize() << std::endl;
            free(c.dataBuffer);
            allocateMemory=true;

        }

        if (allocateMemory)
        {
            //std::cout << "sizeof c.dataBuffer " << dbr_size_n(new_dataTypeBuffer,c.channelRegalia.getNelem())  << std::endl;
            c.dataBuffer = (db_access_val *) malloc ( dbr_size_n(new_dataTypeBuffer,c.channelRegalia.getNelem()) );
            c.channelRequestMetaData.byteSize=dbr_size_n(new_dataTypeBuffer,c.channelRequestMetaData.getNelem());

        }

        if (c.dataBuffer==NULL)
        {
            std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
            printf ("Virtual memory exhausted for channel %s ", ca_name(c.channelID));
            printf ("Exiting CAFE");

            exit(0);
        }
    }

private:
    chtype  new_dataTypeBuffer;
#undef __METHOD__
};


/**
 *   Friend to CAFEConduit/CAFEGroup - releases resources
 */
struct free_dataBuffers
{
    free_dataBuffers () {}

    void operator() (Conduit& c)
    {

        //std::cout << "c.handle=" << c.handle << " " << c.pv << std::endl;

        if(c.dataBuffer)
        {
            free(c.dataBuffer); // _TIME data buffer for ca_get
        }

        if(c.ctrlBuffer)
        {
            free(c.ctrlBuffer); // _CTRL data buffer for ca_get
        }

        if(c.stsackBuffer)
        {
            free(c.stsackBuffer); // _STSACK_STRING data buffer for ca_get
        }

        if(c.putBuffer)
        {
            free(c.putBuffer);  // data buffer for ca_put
        }

    }
};


#endif // CONDUITCONNECTIONHANDLERARGS_H
