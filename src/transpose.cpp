///
/// \file    transpose.cpp
/// \author  Jan Chrin, PSI
/// \date    Relase: February 2015
/// \version CAFE 1.0.0
///

#ifndef TRANSPOSE_CC
#define TRANSPOSE_CC

#include <transpose.h>

using namespace std;

/**
 * \brief  Converts data from CTYPE to native type
 * in preparation for transmission to CA Server
 * \param  _handle  input: handel to Conduit object
 * \param val  input: Array of values of datatype dbr_string_t
 * \return ICAFE_NORMAL as local data conversion should not incur an error
 */
int  Transpose<dbr_string_t>::putString
(const unsigned int  _handle, dbr_string_t * val)
{
#define __METHOD__ "Transpose<CTYPE>::putString"

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        PVDataL  = (*it_handle).getPutBuffer  ();
        nelem    = (*it_handle).getChannelRequestMetaPrimitive().getNelem();

        dbrTypeRequest_DataBuffer = (*it_handle).getChannelRequestMetaPrimitive().getDbrDataType();

        dbr_ctrl_enum *  dataEnum;
        dbr_short_t noStrings =0;

        HandleHelper helper;

        bool allStringsAreValid=true;
        // Client is String
        // Native type is one of the following

        istringstream ss;

        switch(dbrTypeRequest_DataBuffer)
        {

        case DBR_STRING://0
            for (unsigned int  i=0; i<nelem; ++i)
            {
                strcpy( *((dbr_string_t *) (PVDataL) +i), val[i]);
            }
            break;

        case DBR_SHORT://1
            for (unsigned int  i=0; i<nelem; ++i)
            {
                dbr_short_t s=0;
                ss.clear();
                ss.str(val[i]);
                ss>>s;

                if ( !ss.fail())
                {
                    *((dbr_short_t *) (PVDataL) + i ) = s;
                }
                else
                {
                    cout << __FILE__<< "//" << __METHOD__ << "//" << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_SHORT CONVERSION for ELEMENT " << i
                         << " of " << nelem << " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i];
                    cout << " TO SHORT; PUT REQUEST NOT MADE!" << endl;
                    //AT THE MERCY OF THE CA SERVER!" << endl; //NOT MADE!" << endl;
                    allStringsAreValid=false;
                }
            }
            break;

        case DBR_FLOAT://2
            for (unsigned int  i=0; i<nelem; ++i)
            {
                dbr_float_t f=0;
                ss.clear();
                ss.str(val[i]);
                ss>>f;

                if ( !ss.fail())
                {
                    *((dbr_float_t *) (PVDataL) + i ) = f;
                }
                else
                {
                    cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_FLOAT CONVERSION for ELEMENT " << i << " of " << nelem << " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i];
                    cout << " TO FLOAT; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl; //NOT MADE! " << endl;
                    allStringsAreValid=false;
                }
            }
            break;

        case DBR_ENUM: //3
            // CHANGE THE STRING TO CORRESPONDING ENUM!
            dataEnum  =   (dbr_ctrl_enum *)  (*it_handle).getCtrlBuffer();
            char stig [MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE];

            noStrings           = ((struct dbr_ctrl_enum *) dataEnum)->no_str;

            memcpy( stig,  &(((struct dbr_ctrl_enum *) dataEnum)->strs),  sizeof(stig  )) ;


            for (unsigned int  i=0; i<nelem; ++i)
            {

                //Remove leading and blank spaces
                dbr_string_t  valTrimmed; //Previously referred to as varaible a;
                helper.removeLeadingAndTrailingSpaces((const char*) val[i], valTrimmed);

                /*
                        dbr_string_t a;

                        //sprintf(a, "%s", val[i]);

                        char *b = (char *)  val[i];
                        char *c;
                        // Remove leading blanks
                        c = b;
                        while (c != '\0' && *c == ' ') {
                            c++;
                        }
                        b = c;

                        // Remove trailing blanks
                        c = b + strlen (b) - 1;
                        while (c >= b)
                            {
                            if (*c == ' ') {
                                *c = '\0';
                                // This was reported at www.programmingforums.org/thread35790.html to cause a bus error!?
                            }
                            else {
                                break;
                            }
                            c--;
                        }

                        sprintf(a, "%s", b);
                						*/

                bool isValidString=false;
                for (int j=0; j<noStrings; ++j)
                {

                    if (strcmp((char *)valTrimmed, stig[j] ) ==0)
                    {

                        dbr_enum_t us= (unsigned short) j;
                        *((dbr_enum_t *) (PVDataL) + i ) = us;
                        isValidString=true;
                        //cout << "setting value " << j << " " << stig[j] << endl;
                        break;
                    }
                }

                // Check for ENUM values given in "string" form

                if (!isValidString)
                {

                    bool isValidStringAnew=false;

                    dbr_enum_t us=0;
                    ss.clear();
                    ss.str(valTrimmed);
                    ss>>us;

                    // Is this a valid number?
                    if ( !ss.fail())
                    {
                        if (us==0 || (us>0 && us <noStrings))
                        {
                            *((dbr_enum_t *) (PVDataL) + i ) = us;
                            isValidStringAnew=true;
                        }
                    }

                    if (!isValidStringAnew)
                    {

                        cout << __FILE__  << "//" << __METHOD__ << "//" << __LINE__ << endl;
                        cout << "***WARNING*** NO STRING TO DBR_ENUM MATCHING for ELEMENT " << i << " of " << nelem << " !! " << endl;
                        cout << "***WARNING*** COULD NOT CONVERT: '";
                        cout << valTrimmed; //val[i];
                        cout << "' TO A VALID ENUM INDEX; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl;
                        cout << "VALID ENUM OPTIONS ARE: " << endl;

                        for (int ij=0; ij<noStrings;   ++ij)
                        {
                            cout << "Index= " << ij << " '" << stig[ij] << "';  ";
                        }
                        cout << endl;

                        allStringsAreValid=false;
                    }

                }

            } //nelem for loop

            break;

        case DBR_CHAR: //4
            for (unsigned int  i=0; i<nelem; ++i)
            {
                dbr_char_t ch=0;
                ss.clear();
                ss.str(val[i]);
                ss>>ch;
                char *b = (char *)  val[i];
               
	        //cout << "Transpose putString: " << endl; 
                //cout << i << " " << val[i] << " " << (dbr_char_t) ch << endl; 
                //cout << b << " " << *b << endl;
                //cout << (unsigned short *) b << " " << (unsigned short) *b << endl;
          

                if ( !ss.fail())
                {
	
		  *((dbr_char_t *) (PVDataL) + i ) =(dbr_char_t) ch;
                }

                else //if (*b != '\0')
                {
                
		  *( (dbr_char_t *)(PVDataL) + i ) = (dbr_char_t) *b;

                }
                
                /*
                else if (*b == '\0')
                {

                    *((dbr_char_t *) (PVDataL) + i ) =  *b;

                }
                else
                {
                    cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_CHAR CONVERSION for ELEMENT index " << i << " in array of length " << nelem;
                    cout <<	", i.e., with index range [0-"	<< (nelem-1) << "] "	<<  " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i];
                    cout << " TO UNSIGNED CHAR; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl; //NOT MADE!" << endl;
                    allStringsAreValid=false;
                }
                */ 
            }
            break;

        case DBR_LONG: //5
            for (unsigned int  i=0; i<nelem; ++i)
            {
                dbr_long_t l=0;
                ss.clear();
                ss.str(val[i]);
                ss>>l;

                if ( !ss.fail())
                {
                    *((dbr_long_t *) (PVDataL) + i ) = l;
                }
                else
                {
                    cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_LONG  CONVERSION for ELEMENT " << i << " of " << nelem << " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i];
                    cout << " TO LONG; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl; //NOT MADE!" << endl;

                    allStringsAreValid=false;
                }
            }
            break;

        case DBR_DOUBLE: //6
            for (unsigned int  i=0; i<nelem; ++i)
            {
                dbr_double_t d=0;
                ss.clear();
                ss.str(val[i]);
                ss>>d;

                if ( !ss.fail())
                {
                    *((dbr_double_t *) (PVDataL) + i ) = d;
                }
                else
                {
                    cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION for ELEMENT " << i << " of " << nelem << " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i];
                    cout << " TO DOUBLE; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl;  //PUT REQUEST NOT MADE!" << endl;

                    allStringsAreValid=false;

                    // *((dbr_double_t *) (PVDataL) + i ) =   (dbr_double_t)  strtod(val[i],NULL);
                }

            }//for
            break;
        } //switch

        if(!allStringsAreValid)
        {
            return ECAFE_NO_CONVERT;
        }
        else
        {
            return ICAFE_NORMAL;
        }

    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}


/**
 * \brief Converts data from CTYPE to native type
 * in preparation for transmission to CA Server
 * \param  _handle  input: handel to Conduit object
 * \param val  input: Array of values of datatype dbr_string_t
 * \return ICAFE_NORMAL as local data conversion should not incur an error
 */
int  Transpose<dbr_string_t>::putString(const unsigned int  _handle, CAFE_DATATYPE_UNION_SEQ val)
{
#define __METHOD__ "Transpose<CTYPE>::putString "

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        PVDataL  = (*it_handle).getPutBuffer  ();
        nelem    = (*it_handle).getChannelRequestMetaPrimitive().getNelem();


        dbrTypeRequest_DataBuffer = (*it_handle).getChannelRequestMetaPrimitive().getDbrDataType();


        HandleHelper helper;
        dbr_ctrl_enum *  dataEnum;
        dbr_short_t noStrings =0;

        bool allStringsAreValid=true;
        // Client is String
        // Native type is one of the following

        istringstream ss;

        switch(dbrTypeRequest_DataBuffer)
        {

        case DBR_STRING://0

            for (unsigned int  i=0; i<nelem; ++i)
            {
                strcpy( *((dbr_string_t *) (PVDataL) +i), val[i].str);
            }
            break;

        case DBR_SHORT://1
            for (unsigned int  i=0; i<nelem; ++i)
            {
                dbr_short_t s=0;
                ss.clear();
                ss.str(val[i].str);
                ss>>s;

                if ( !ss.fail())
                {
                    *((dbr_short_t *) (PVDataL) + i ) = s;
                }
                else
                {
                    cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_SHORT CONVERSION for ELEMENT " << i
                         << " of " << nelem << " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i].str;
                    cout << " TO SHORT; PUT REQUEST NOT MADE!" << endl;
                    //AT THE MERCY OF THE CA SERVER!" << endl; //NOT MADE!" << endl;
                    allStringsAreValid=false;
                }
            }
            break;

        case DBR_FLOAT://2
            for (unsigned int  i=0; i<nelem; ++i)
            {
                dbr_float_t f=0;
                ss.clear();
                ss.str(val[i].str);
                ss>>f;

                if ( !ss.fail())
                {
                    *((dbr_float_t *) (PVDataL) + i ) = f;
                }
                else
                {
                    cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_FLOAT CONVERSION for ELEMENT " << i << " of " << nelem << " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i].str;
                    cout << " TO FLOAT; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl; //NOT MADE! " << endl;
                    allStringsAreValid=false;
                }
            }
            break;

        case DBR_ENUM: //3
            // CHANGE THE STRING TO CORRESPONDING ENUM!
            dataEnum  =   (dbr_ctrl_enum *)  (*it_handle).getCtrlBuffer();
            char stig [MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE];

            noStrings           = ((struct dbr_ctrl_enum *) dataEnum)->no_str;

            memcpy( stig,  &(((struct dbr_ctrl_enum *) dataEnum)->strs),  sizeof(stig  )) ;


            for (unsigned int  i=0; i<nelem; ++i)
            {
                //dbr_string_t a;

                dbr_string_t  valTrimmed; //Previously referred to as varaible a;
                helper.removeLeadingAndTrailingSpaces((const char*) val[i].str, valTrimmed);

                /*
                //sprintf(a, "%s", val[i]);

                char *b = (char *)  val[i].str;
                char *c;
                // Remove leading blanks
                c = b;
                while (c != '\0' && *c == ' ') {
                    c++;
                }
                b = c;

                // Remove trailing blanks
                c = b + strlen (b) - 1;
                while (c >= b)
                    {
                    if (*c == ' ') {
                        *c = '\0';
                        // This was reported at www.programmingforums.org/thread35790.html to cause a bus error!?
                    }
                    else {
                        break;
                    }
                    c--;
                }

                sprintf(a, "%s", b);
                */
                bool isValidString=false;
                for (int j=0; j<noStrings; ++j)
                {

                    if (strcmp((char *)valTrimmed, stig[j] ) ==0)   // previously a
                    {

                        dbr_enum_t us= (unsigned short) j;
                        *((dbr_enum_t *) (PVDataL) + i ) = us;
                        isValidString=true;
                        //cout << "setting value " << j << " " << stig[j] << endl;
                        break;
                    }
                }

                // Check for ENUM values given in "string" form

                if (!isValidString)
                {

                    bool isValidStringAnew=false;

                    dbr_enum_t us=0;
                    ss.clear();
                    ss.str(valTrimmed); //
                    ss>>us;

                    // Is this a valid number?
                    if ( !ss.fail())
                    {
                        if (us==0 || (us>0 && us <noStrings))
                        {
                            *((dbr_enum_t *) (PVDataL) + i ) = us;
                            isValidStringAnew=true;
                        }
                    }

                    if (!isValidStringAnew)
                    {

                        cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
                        cout << "***WARNING*** NO STRING TO DBR_ENUM MATCHING for ELEMENT " << i << " of " << nelem << " !! " << endl;
                        cout << "***WARNING*** COULD NOT CONVERT: '";
                        cout << valTrimmed; //val[i];
                        cout << "' TO UNSIGNED SHORT; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl;
                        cout << "VALID ENUM OPTIONS ARE: " << endl;

                        for (int ij=0; ij<noStrings;   ++ij)
                        {
                            cout << "Index=" << ij << " '" << stig[ij] << "';  ";
                        }
                        cout << endl;

                        allStringsAreValid=false;
                    }

                }

            } //nelem for loop

            break;

        case DBR_CHAR: //4
            for (unsigned int  i=0; i<nelem; ++i)
            {
                dbr_char_t ch=0;
                ss.clear();
                ss.str(val[i].str);
                ss>>ch;

                char *b = (char *)  val[i].str;

                if ( !ss.fail())
                {
                    *((dbr_char_t *) (PVDataL) + i ) = ch;
                }
                else if (*b != '\0')
                {
                    *((dbr_char_t *) (PVDataL) + i ) =  *b;
                }
                else
                {
                    cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_CHAR CONVERSION for ELEMENT index " << i << " in array of length " << nelem;
                    cout <<	", i.e., with index range [0-"	<< (nelem-1) << "] "	<<  " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i].str;
                    cout << " TO UNSIGNED CHAR; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl; //NOT MADE!" << endl;
                    allStringsAreValid=false;
                }

            }
            break;

        case DBR_LONG: //5
            for (unsigned int  i=0; i<nelem; ++i)
            {
                dbr_long_t l=0;
                ss.clear();
                ss.str(val[i].str);
                ss>>l;

                if ( !ss.fail())
                {
                    *((dbr_long_t *) (PVDataL) + i ) = l;
                }
                else
                {
                    cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_int  CONVERSION for ELEMENT " << i << " of " << nelem << " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i].str;
                    cout << " TO LONG; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl; //NOT MADE!" << endl;

                    allStringsAreValid=false;
                }
            }
            break;

        case DBR_DOUBLE: //6
            for (unsigned int  i=0; i<nelem; ++i)
            {
                dbr_double_t d=0;
                ss.clear();
                ss.str(val[i].str);
                ss>>d;

                if ( !ss.fail())
                {
                    *((dbr_double_t *) (PVDataL) + i ) = d;
                }
                else
                {
                    cout << __FILE__ << "//" << __METHOD__<< "//"  << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION for ELEMENT " << i << " of " << nelem << " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i].str;
                    cout << " TO DOUBLE; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl;  //PUT REQUEST NOT MADE!" << endl;

                    allStringsAreValid=false;

                    // *((dbr_double_t *) (PVDataL) + i ) =   (dbr_double_t)  strtod(val[i],NULL);
                }

            }//for
            break;
        } //switch

        if(!allStringsAreValid)
        {
            return ECAFE_NO_CONVERT;
        }
        else
        {
            return ICAFE_NORMAL;
        }

    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}


/**
 * \brief Retrieves data transmitted by CA with dbrTypeRequest_DataBuffer
 * and then converts to dbr_string_t (char[40])
 * \param  _handle  input: handle to Conduit object
 * \param val output: array of CTYPE datatype
 * \param  alarmStatus   output: dbr_short_t
 * \param  alarmSeverity output: dbr_short_t
 * \param  ts output: epicsTimeStamp
 * \param isCacheRequest input: bool, set to true for cached data
 * \return ICAFE_NORMAL as local data conversion should not incur an error
 */
int  Transpose<dbr_string_t>::get(
    const unsigned int  _handle,
    dbr_string_t * val,
    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts,
    bool isCacheRequest)
{
#define __METHOD__ "Transpose<dbr_string_t>::get()"

  //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        PVDataL  = (*it_handle).getDataBuffer  ();
        offset   = (*it_handle).getChannelRequestMetaDataClient().getOffset();

        if(isCacheRequest)
        {
            offset = (*it_handle).getChannelRequestMetaData().getOffset( );
            nelem  = (*it_handle).getChannelRequestMetaData().getNelemCache(); //-(*it_handle).getChannelRequestMetaData().getOffset(); //-offset;
            //nelem  = min(nelem, (*it_handle).getChannelRequestMetaData().getNelem()-offset);
        }
        else
        {
            nelem  = (*it_handle).getChannelRequestMetaData().getNelem()-offset;

        }

        //Something wrong, just read last element
        if (nelem <=0)
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Something funny with the offset; just read last element! " << endl;
            cout << "Changing offset from=" << offset;

            offset = (*it_handle).getChannelRequestMetaData().getNelem()-1;
            if(isCacheRequest)
            {
                nelem  = (*it_handle).getChannelRequestMetaData().getNelemCache(); //-(*it_handle).getChannelRequestMetaData().getOffset(); //-offset;
            }
            else
            {
                nelem  = (*it_handle).getChannelRequestMetaData().getNelem()-offset;
            }

            cout << " to=" << offset << endl;
        }

        dbrTypeRequest_DataBuffer = (*it_handle).getChannelRequestMetaData().getDbrDataType();

        //std::cout << __METHOD__ << " // " <<  dbrTypeRequest_DataBuffer  << std::endl;

        dbr_ctrl_enum *  dataEnum;
        dbr_short_t noStrings =0;

        ts.secPastEpoch=0;
        ts.nsec        =0;
        alarmStatus    =0;
        alarmSeverity  =0;
        /// stringstream ssss;//create a stringstream

        //dbrTypeRequest_DataBuffer is the chtype used in ca_get
        //Only ever fille the Union with the native type as


        switch (dbrTypeRequest_DataBuffer)
        {
        case DBR_CHAR:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%u", (*(&((PVDataL)->charval)+i+offset)));
            }
            break;

        case DBR_FLOAT:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%f", (*(&((PVDataL)->fltval)+i+offset)));
            }
            break;

        case DBR_DOUBLE:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%.15f",(*(&((PVDataL)->doubleval)+i+offset)));
            }
            break;

        case DBR_SHORT:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%d",(*(&((PVDataL)->shrtval)+i+offset)));
            }
            break;

        case DBR_LONG:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%d",(*(&((PVDataL)->longval)+i+offset)));
            }
            break;

        case DBR_ENUM:
        {
            //Special treatment of ENUM: Transform VAL to String equivalent
            dataEnum   =   (dbr_ctrl_enum *)  (*it_handle).getCtrlBuffer();
            noStrings  =   ((struct dbr_ctrl_enum *) dataEnum)->no_str;

            memcpy( stig,  &(((struct dbr_ctrl_enum *) dataEnum)->strs),  sizeof(stig  )) ;

            int noEmptyStrings=0;
            //Check for empty strings:
            for (int j=0; j<noStrings; ++j)
            {
                if (strcmp(stig[j],"")==0)
                {
                    ++noEmptyStrings;
                }
            }
            if (noStrings==noEmptyStrings)
            {
                cout << "*** WARNING FROM " << __METHOD__ << " *** " << endl;
                cout << "ENUM STRING OPTIONS ARE ALL EMPTY! " << endl;
                cout << "BADLY CONFIGURED EPICS " <<  (*it_handle).getChannelRegalia().getClassName()  << " RECORD. " << endl;
                cout << "CHANNEL:  " <<  (*it_handle).getPV() << " (handle=" << (*it_handle).getHandle() <<") "  << endl;
            }


            for (unsigned int  i=0; i<nelem; ++i)
            {
                if ( ((*(&((PVDataL)->enmval)+i+offset)) <  noStrings) && (noStrings!=noEmptyStrings))
                {
                    sprintf(val[i], "%s", stig[(*(&((PVDataL)->enmval)+i+offset))] );
                }
                else
                {
                    sprintf(val[i], "%d", (*(&((PVDataL)->enmval)+i+offset)) );
                    if ( ((*(&((PVDataL)->enmval)+i+offset)) >=  noStrings) )
                    {
                        cout << "*** WARNING FROM " << __METHOD__ << " *** " << endl;
                        cout << "ENUM UNSIGNED SHORT VALUE IS GREATER THAN THE NO OF ENUMERATED TYPES" << endl;
                        cout << "VALUE (unsigned short) = " << (*(&((PVDataL)->enmval)+i+offset)) << endl;
                        cout << "NO OF ENUMERATED STRINGS = " << noStrings << " WITH VALUES: " << endl;
                        for (int j=0; j<noStrings; ++j)
                        {
                            cout << stig[j] << " ["  <<j << "]  ";
                        }
                        cout << endl;
                    }
                }
            }

            break;
        }
        case DBR_STRING:
            memcpy( val, &(&((PVDataL)->strval))[offset],  sizeof(dbr_string_t)*nelem) ;
            break;


        case DBR_STS_CHAR:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%u", (*(&((PVDataL)->schrval.value)+i+offset)));
            }
            alarmStatus   = ((struct dbr_sts_char *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_char *) PVDataL)->severity;
            break;

        case DBR_STS_FLOAT:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%f", (*(&((PVDataL)->sfltval.value)+i+offset)));
            }
            alarmStatus   = ((struct dbr_sts_float *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_float *) PVDataL)->severity;
            break;

        case DBR_STS_DOUBLE:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%.15f",(*(&((PVDataL)->sdblval.value)+i+offset)));
            }
            alarmStatus   = ((struct dbr_sts_double *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_double *) PVDataL)->severity;
            break;

        case DBR_STS_SHORT:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%d",(*(&((PVDataL)->sshrtval.value)+i+offset)));
            }
            alarmStatus   = ((struct dbr_sts_short *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_short *) PVDataL)->severity;
            break;

        case DBR_STS_LONG:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%d",(*(&((PVDataL)->slngval.value)+i+offset)));
            }
            alarmStatus   = ((struct dbr_sts_int  *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_int  *) PVDataL)->severity;
            break;

        case DBR_STS_ENUM:
        {
            //Special treatment of ENUM: Transform VAL to String equivalent
            dataEnum   =   (dbr_ctrl_enum *) (*it_handle).getCtrlBuffer();
            noStrings  =   ((struct dbr_ctrl_enum *) dataEnum)->no_str;
            memcpy( stig,  &(((struct dbr_ctrl_enum *) dataEnum)->strs),  sizeof(stig)) ;

            int noEmptyStrings=0;
            //Check for empty strings:
            for (int j=0; j<noStrings; ++j)
            {
                if (strcmp(stig[j],"")==0)
                {
                    ++noEmptyStrings;
                }
            }
            if (noStrings==noEmptyStrings)
            {
                cout << "*** WARNING FROM " << __METHOD__ << " *** " << endl;
                cout << "ENUM STRING OPTIONS ARE ALL EMPTY! " << endl;
                cout << "BADLY CONFIGURED EPICS " <<  (*it_handle).getChannelRegalia().getClassName()  << " RECORD. " << endl;
                cout << "CHANNEL:  " <<  (*it_handle).getPV() << " (handle=" << (*it_handle).getHandle() <<") "  << endl;
            }

            for (unsigned int  i=0; i<nelem; ++i)
            {
                if ( ((*(&((PVDataL)->senmval.value)+i+offset)) <  noStrings) && (noStrings!=noEmptyStrings))
                {
                    sprintf(val[i], "%s", stig[(*(&((PVDataL)->senmval.value)+i+offset))] );
                }
                else
                {
                    sprintf(val[i], "%d", (*(&((PVDataL)->senmval.value)+i+offset)) );
                    if ( ((*(&((PVDataL)->senmval.value)+i+offset)) >=  noStrings) )
                    {
                        cout << "*** WARNING FROM " << __METHOD__ << " *** " << endl;
                        cout << "ENUM UNSIGNED SHORT VALUE IS GREATER THAN THE NO OF ENUMERATED TYPES" << endl;
                        cout << "VALUE (unsigned short) = " << (*(&((PVDataL)->senmval.value)+i+offset)) << endl;
                        cout << "NO OF ENUMERATED STRINGS = " << noStrings << " WITH VALUES: " << endl;
                        for (int j=0; j<noStrings; ++j)
                        {
                            cout << stig[j] << " ["  <<j << "]  ";
                        }
                        cout << endl;
                    }
                }
            }

            alarmStatus   = ((struct dbr_sts_enum *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_enum *) PVDataL)->severity;
            break;
        }
        case DBR_STS_STRING:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                strcpy(val[i],   (*(&((PVDataL)->sstrval.value)+i+offset))) ;
            }


            alarmStatus   = ((struct dbr_sts_string *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_string *) PVDataL)->severity;
            break;


        case DBR_TIME_CHAR:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%u", (*(&((PVDataL)->tchrval.value)+i+offset)));
            }
            ts            = ((struct dbr_time_char *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_char *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_char *) PVDataL)->severity;
            break;

        case DBR_TIME_FLOAT:

            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%f", (*(&((PVDataL)->tfltval.value)+i+offset)));
            }
            ts            = ((struct dbr_time_float *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_float *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_float *) PVDataL)->severity;


            break;

        case DBR_TIME_DOUBLE:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%.15f", (*(&((PVDataL)->tdblval.value)+i+offset)));
            }
            ts            = ((struct dbr_time_double *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_double *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_double *) PVDataL)->severity;
            break;

        case DBR_TIME_SHORT:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%d", (*(&((PVDataL)->tshrtval.value)+i+offset)));
            }
            ts            = ((struct dbr_time_short *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_short *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_short *) PVDataL)->severity;
            break;

        case DBR_TIME_LONG:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                sprintf(val[i], "%d", (*(&((PVDataL)->tlngval.value)+i+offset)));
            }
            ts            = ((struct dbr_time_long  *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_long  *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_long  *) PVDataL)->severity;
            break;

        case DBR_TIME_ENUM:
        {

            //Special treatment of ENUM: Transform VAL to String equivalent
            dataEnum   =   (dbr_ctrl_enum *) (*it_handle).getCtrlBuffer();

            noStrings  =   ((struct dbr_ctrl_enum *) dataEnum)->no_str;

            memcpy( stig,  &(((struct dbr_ctrl_enum *) dataEnum)->strs),  sizeof(stig  )) ;

            //if native DataType is ENUM then overwrite call Enum method and then convert to string!
            //Check data type
            //cout << "Classname " << (*it_handle).getChannelRegalia().getClassName() << endl;

            int noEmptyStrings=0;
            //Check for empty strings:
            for (int j=0; j<noStrings; ++j)
            {
                if (strcmp(stig[j],"")==0)
                {
                    ++noEmptyStrings;
                }
            }
            if (noStrings==noEmptyStrings)
            {
                cout << "*** WARNING FROM " << __METHOD__ << " *** " << endl;
                cout << "ENUM STRING OPTIONS ARE ALL EMPTY! " << endl;
                cout << "BADLY CONFIGURED EPICS " <<  (*it_handle).getChannelRegalia().getClassName()  << " RECORD. " << endl;
                cout << "CHANNEL:  " <<  (*it_handle).getPV() << " (handle=" << (*it_handle).getHandle() <<") "  << endl;
            }

            for (unsigned int  i=0; i<nelem; ++i)
            {
                if ( ((*(&((PVDataL)->tenmval.value)+i+offset)) <  noStrings) && (noStrings!=noEmptyStrings))
                {
                    sprintf(val[i], "%s", stig[(*(&((PVDataL)->tenmval.value)+i+offset))] );
                }
                else
                {
                    sprintf(val[i], "%d", (*(&((PVDataL)->tenmval.value)+i+offset)) );
                    if ( ((*(&((PVDataL)->tenmval.value)+i+offset)) >=  noStrings) )
                    {
                        cout << "*** WARNING FROM " << __METHOD__ << " *** " << endl;
                        cout << "ENUM UNSIGNED SHORT VALUE IS GREATER THAN THE NO OF ENUMERATED TYPES" << endl;
                        cout << "VALUE (unsigned short) = " << (*(&((PVDataL)->tenmval.value)+i+offset)) << endl;
                        cout << "NO OF ENUMERATED STRINGS = " << noStrings << " WITH VALUES: " << endl;
                        for (int j=0; j<noStrings; ++j)
                        {
                            cout << stig[j] << " ["  <<j << "]  ";
                        }
                        cout << endl;
                    }
                }
            }
            ts            = ((struct dbr_time_enum *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_enum *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_enum *) PVDataL)->severity;
            break;
        }

        case DBR_TIME_STRING:

            for (unsigned int  i=0; i<nelem; ++i)
            {
                strcpy(val[i], (*(&((PVDataL)->tstrval.value)+i+offset)));
            }
            //std::cout << val[0] << std::endl;

            //dbr_string_t val2[4096];
            //std::cout << __METHOD__ << std::endl;

            //memcpy( val2, &(&((PVDataL)->tstrval.value))[offset], sizeof(dbr_string_t)*nelem);
            //std::cout << val2[0] << std::endl;

            ts            = ((struct dbr_time_string *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_string *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_string *) PVDataL)->severity;

            break;
        }

        //helper function to set TimeStamps!
        //cout <<__METHOD__ << endl;
        //cout << "_dbrType" << _dbrTypeRequest_DataBuffer<< endl;
        //cout << ts.secPastEpoch << " " << ts.nsec << endl;

        if(!isCacheRequest)
        {
            HandleHelper handleHelper;
            handleHelper.setSTS(_handle, alarmStatus, alarmSeverity, ts);
        }

    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }



    return ICAFE_NORMAL;
#undef __METHOD__
}


/**
 * \brief Retrieves ctrl data from buffer
 * \param _handle input: handle to Conduit object
 * \param val input: dbr_string_t
 * \param alarmStatus   input: dbr_short_t
 * \param alarmSeverity input: dbr_short_t
 * \param isCacheRequest input: bool, set to true for cached data
 * \return ICAFE_NORMAL
 */
int  Transpose<dbr_string_t>::getCtrl(
    const unsigned int  _handle,      dbr_string_t * val,
    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity,
    bool isCacheRequest)
{
#define __METHOD__ "Transpose<dbr_string_t>::getCtrl"

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);


    if (it_handle != handle_index.end())
    {

        PVDataL  = (*it_handle).getCtrlBuffer  ();

        offset   = (*it_handle).getChannelRequestMetaCtrlClient().getOffset();

        if (isCacheRequest)
        {
            offset = (*it_handle).getChannelRequestMetaCtrl().getOffset( );
            nelem  = (*it_handle).getChannelRequestMetaCtrl().getNelemCache(); //-(*it_handle).getChannelRequestMetaCtrl().getOffset();   //-offset;
            //nelem  = min(nelem, (*it_handle).getChannelRequestMetaCtrl().getNelem()-offset);

        }
        else
        {
            nelem    = (*it_handle).getChannelRequestMetaCtrl().getNelem()-offset;
        }
        //Something wrong, just read last element
        if (nelem <=0)
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Something funny with the offset; just read last element! " << endl;
            cout << "Changing offset from=" << offset;

            offset = (*it_handle).getChannelRequestMetaCtrl().getNelem()-1;
            if(isCacheRequest)
            {
                nelem  = (*it_handle).getChannelRequestMetaCtrl().getNelemCache(); //-(*it_handle).getChannelRequestMetaCtrl().getOffset(); //-offset;
            }
            else
            {
                nelem  = (*it_handle).getChannelRequestMetaCtrl().getNelem()-offset;
            }
            cout << " to=" << offset << endl;
        }

        dbrTypeRequest_CtrlBuffer = (*it_handle).getChannelRequestMetaCtrl().getDbrDataType();


        //cout << __METHOD__ << endl;
        //cout <<  " dbrTypeRequest_CtrlBuffer " << dbrTypeRequest_CtrlBuffer << endl;



        alarmStatus     = -1;
        alarmSeverity   = -1;

        switch (dbrTypeRequest_CtrlBuffer)
        {
        case DBR_CTRL_STRING:
            memcpy(val, &(&((PVDataL)->cstrval.value))[offset],  sizeof(dbr_string_t)*nelem);
            alarmStatus   = ((struct dbr_sts_string *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_string *) PVDataL)->severity;
            break;

        default:
            break;
        }

    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * \brief Retrieves data transmitted by CA with dbrTypeRequest_DataBuffer
 * and then converts to CAFE_DATATYPE_UNION_SEQ
 * \param  _handle input: handle to Conduit object
 * \param  val output: CTYPE datatype
 * \param  ts output: epicsTimeStamp
 * \param  alarmStatus   output: dbr_short_t
 * \param  alarmSeverity output: dbr_short_t
 * \param  ts output: epicsTimeStamp
 * \param  isCacheRequest input: bool, set to true for cached data
 * \return ICAFE_NORMAL as local data conversion should not incur an error
 */
int  Transpose<CAFE_DATATYPE_UNION>::get(//const CAFEConduit &cc,
    const unsigned int  _handle,   CAFE_DATATYPE_UNION_SEQ val,
    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts, bool isCacheRequest)
{
#define __METHOD__ "Transpose<CAFE_DATATYPE_UNION_SEQ>::get()"

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);


    if (it_handle != handle_index.end())
    {

        PVDataL  = (*it_handle).getDataBuffer  ();

        offset   = (*it_handle).getChannelRequestMetaDataClient().getOffset();



        if(isCacheRequest)
        {
            offset = (*it_handle).getChannelRequestMetaData().getOffset( );
            nelem  = (*it_handle).getChannelRequestMetaData().getNelemCache(); //-offset;
            //nelem  = min(nelem, (*it_handle).getChannelRequestMetaData().getNelem()-offset);

        }
        else
        {
            nelem  = (*it_handle).getChannelRequestMetaData().getNelem()-offset;
        }


        //Something wrong, just read last element
        if (nelem <=0)
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Something funny with the offset; just read last element! " << endl;
            cout << "Changing offset from=" << offset;
            //offset = cc.getNelemRequest()-1;
            //nelem  = cc.getNelemRequest()-offset;
            offset = (*it_handle).getChannelRequestMetaData().getNelem()-1;
            if(isCacheRequest)
            {
                nelem  = (*it_handle).getChannelRequestMetaData().getNelemCache(); //-offset;
            }
            else
            {
                nelem  = (*it_handle).getChannelRequestMetaData().getNelem()-offset;
            }
            cout << " to=" << offset << endl;
        }

        dbrTypeRequest_DataBuffer = (*it_handle).getChannelRequestMetaData().getDbrDataType();

        ts.secPastEpoch=0;
        ts.nsec        =0;
        alarmStatus    =-1;
        alarmSeverity  =-1;


        //dbrTypeRequest_DataBuffer is the chtype used in ca_get
        //Only ever fille the Union with the native type as

        switch (dbrTypeRequest_DataBuffer)
        {
        case DBR_CHAR:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].ch =   (*(&((PVDataL)->charval)+i+offset));
            }
            break;

        case DBR_FLOAT:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].f  =   (*(&((PVDataL)->fltval)+i+offset));
            }

            break;

        case DBR_DOUBLE:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].d  =   (*(&((PVDataL)->doubleval)+i+offset));
            }
            break;

        case DBR_SHORT:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].s  =   (*(&((PVDataL)->shrtval)+i+offset));
            }
            break;

        case DBR_LONG:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].l  =   (*(&((PVDataL)->longval)+i+offset));
            }
            break;

        case DBR_ENUM:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].us =   (*(&((PVDataL)->enmval)+i+offset));
            }
            break;

        case DBR_STRING:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                strcpy(val[i].str, (*(&((PVDataL)->strval)+i+offset)));
            }
            break;


        case DBR_STS_CHAR:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].ch =   (*(&((PVDataL)->schrval.value)+i+offset));
            }
            alarmStatus   = ((struct dbr_sts_char *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_char *) PVDataL)->severity;
            break;

        case DBR_STS_FLOAT:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].f =   (*(&((PVDataL)->sfltval.value)+i+offset));
            }
            alarmStatus   = ((struct dbr_sts_float *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_float *) PVDataL)->severity;
            break;

        case DBR_STS_DOUBLE:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].d =   (*(&((PVDataL)->sdblval.value)+i+offset));
            }
            alarmStatus   = ((struct dbr_sts_double *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_double *) PVDataL)->severity;
            break;

        case DBR_STS_SHORT:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].s =   (*(&((PVDataL)->sshrtval.value)+i+offset));
            }
            alarmStatus   = ((struct dbr_sts_short *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_short *) PVDataL)->severity;
            break;

        case DBR_STS_LONG:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].l =   (*(&((PVDataL)->slngval.value)+i+offset));
            }
            alarmStatus   = ((struct dbr_sts_int  *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_int  *) PVDataL)->severity;
            break;

        case DBR_STS_ENUM:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].us =   (*(&((PVDataL)->senmval.value)+i+offset));
            }
            alarmStatus   = ((struct dbr_sts_enum *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_enum *) PVDataL)->severity;
            break;

        case DBR_STS_STRING:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                strcpy(val[i].str,   (*(&((PVDataL)->sstrval.value)+i+offset))) ;
            }
            alarmStatus   = ((struct dbr_sts_string *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_string *) PVDataL)->severity;
            break;


        case DBR_TIME_CHAR:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].ch =   (*(&((PVDataL)->tchrval.value)+i+offset)) ;
            }
            ts            = ((struct dbr_time_char *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_char *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_char *) PVDataL)->severity;
            break;

        case DBR_TIME_FLOAT:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].f =   (*(&((PVDataL)->tfltval.value)+i+offset)) ;
            }
            ts            = ((struct dbr_time_float *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_float *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_float *) PVDataL)->severity;
            break;

        case DBR_TIME_DOUBLE:

            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].d =   (*(&((PVDataL)->tdblval.value)+i+offset)) ;

            }
            ts            = ((struct dbr_time_double *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_double *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_double *) PVDataL)->severity;
            break;

        case DBR_TIME_SHORT:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].s =   (*(&((PVDataL)->tshrtval.value)+i+offset));
            }
            ts            = ((struct dbr_time_short *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_short *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_short *) PVDataL)->severity;
            break;

        case DBR_TIME_LONG:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].l =   (*(&((PVDataL)->tlngval.value)+i+offset));
            }
            ts            = ((struct dbr_time_long *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_long *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_long *) PVDataL)->severity;
            break;

        case DBR_TIME_ENUM:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                val[i].us =   (*(&((PVDataL)->tenmval.value)+i+offset));
            }
            ts            = ((struct dbr_time_enum *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_enum *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_enum *) PVDataL)->severity;
            break;

        case DBR_TIME_STRING:
            for (unsigned int  i=0; i<nelem; ++i)
            {
                strcpy(val[i].str, (*(&((PVDataL)->tstrval.value)+i+offset)));
            }
            ts            = ((struct dbr_time_string *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_string *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_string *) PVDataL)->severity;
            break;
        }
    }
    //helper function to set TimeStamps!
    //cout <<__METHOD__ << endl;
    //cout << "_dbrType" << _dbrTypeRequest_DataBuffer<< endl;
    //cout << ts.secPastEpoch << " " << ts.nsec << endl;

    if(!isCacheRequest)
    {
        HandleHelper handleHelper;
        handleHelper.setSTS(_handle, alarmStatus, alarmSeverity, ts);
    }

    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
};


#endif
