///
/// \file    transpose.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef TRANSPOSE_H
#define TRANSPOSE_H

#include <cadef.h>
#include <global.h>
#include <statusCodes.h>
#include <handleHelper.h>

using namespace std;

/**
 * Transpose Template \n
 * CTYPE is the type of data to be rendered to/from the client \n
 * method get() will receive data transmitted by CA with dataTypeRequest \n
 *              and then convert to CTYPE
 * method getString() will receive data transmitted by CA with dataTypeRequest \n
 *              and then convert to a string
 * method put() will convert data of type CTYPE to the native type  \n
 *              and then transmit to CA
 * method putString() will convert a string type to the native type  \n
 *              and then transmit to CA
 */
template <class CTYPE> class Transpose {

public:
    Transpose (){};
    ~Transpose (){};

	HandleHelper handleHelper;

    int  put(const unsigned int  _handle, const CTYPE * val,  const chtype cdt);
    int  put(const unsigned int  _handle, CAFE_DATATYPE_UNION_SEQ val, CAFE_DATATYPE cdt);
    int  putString (const unsigned int  _handle, dbr_string_t * val);

    int  get(
             const unsigned int  _handle, CTYPE * val,
              dbr_short_t & alarmStatus,  dbr_short_t &alarmSeverity, epicsTimeStamp &ts, bool isCacheRequest);


    int  get( //union db_access_val * PVDataL,
             //ChannelRequestMetaDataClient    channelRequestMetaDataClient,
             //ChannelRepositoryMetaData       channelRepositoryMetaData,
             const unsigned int  _handle, CTYPE * val,
              dbr_short_t & alarmStatus,  dbr_short_t &alarmSeverity, bool isCacheRequest) {

        epicsTimeStamp ts;
        alarmStatus    =0;
        alarmSeverity  =0;

        return get (//PVDataL,
                    //channelRequestMetaDataClient,
                    //channelRepositoryMetaData,
                    _handle, val, alarmStatus, alarmSeverity, ts, isCacheRequest);
    };


    int  get( //union db_access_val * PVDataL,
             //ChannelRequestMetaDataClient    channelRequestMetaDataClient,
             //ChannelRepositoryMetaData       channelRepositoryMetaData,
            const unsigned int  _handle, CTYPE * val,  bool isCacheRequest) {
        epicsTimeStamp ts;
        dbr_short_t alarmStatus;
        dbr_short_t alarmSeverity;

        ts.secPastEpoch=0;
        ts.nsec        =0;
        alarmStatus    =0;
        alarmSeverity  =0;

        return  get(//PVDataL,
                    //channelRequestMetaDataClient,
                    //channelRepositoryMetaData,
                    _handle, val, alarmStatus, alarmSeverity, ts, isCacheRequest);
    };


    int  getCtrl ( //const CAFEConduit &cc, union db_access_val * PVDataL,
                  //ChannelRequestMetaDataClient    channelRequestMetaDataClient,
                  //ChannelRepositoryMetaData       channelRepositoryMetaData,

                  const unsigned int  _handle, CTYPE * val,
        dbr_short_t &alarmStatus,  dbr_short_t &alarmSeverity, dbr_short_t &precision,
        CTYPE   &RISC_pad,
        CTYPE   &upperDispLimit,    CTYPE  &lowerDispLimit,
        CTYPE   &upperAlarmLimit,   CTYPE  &upperWarningLimit,
        CTYPE   &lowerWarningLimit, CTYPE  &lowerAlarmLimit,
        CTYPE   &upperCtrlLimit,    CTYPE  &lowerCtrlLimit,
        char units[MAX_UNITS_SIZE], short &noStr, char strs [MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE], bool isCacheRequest);



private:
  CTYPE * val;
  int  status;
  CAFEStatus   cafeStatus;
  CAFEDataTypeCode cafeDataTypeCode;
  union db_access_val * PVDataL;
  unsigned int   nelem;
  unsigned int   offset;
  chtype dbrTypeRequest_DataBuffer;
  chtype dbrTypeRequest_CtrlBuffer;
  chtype _dataTypeClient;
  char stig [MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE];

};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * Transpose Template Specialization for dbr_string_t char[40]\n
 */
template <> class Transpose <dbr_string_t> {
    //dbr_string_t * val;
public:
    Transpose (){};
    ~Transpose (){};
  int  putString (const unsigned int  _handle, dbr_string_t * val);
  int  putString (const unsigned int  _handle, CAFE_DATATYPE_UNION_SEQ val); //dbr_string_t * val);

  int  get(//union db_access_val * PVDataL,
           //ChannelRequestMetaDataClient    channelRequestMetaDataClient,
           //ChannelRepositoryMetaData       channelRepositoryMetaData,
           const unsigned int  _handle, dbr_string_t * val,
           dbr_short_t & alarmStatus,  dbr_short_t &alarmSeverity, epicsTimeStamp &ts, bool isCacheRequest );


  int  get(//union db_access_val * PVDataL,
           //ChannelRequestMetaDataClient    channelRequestMetaDataClient,
           //ChannelRepositoryMetaData       channelRepositoryMetaData,
          const unsigned int  _handle, dbr_string_t * val,
            dbr_short_t & alarmStatus,  dbr_short_t &alarmSeverity, bool isCacheRequest ) {

      epicsTimeStamp ts;
      alarmStatus    =0;
      alarmSeverity  =0;

      return get (//PVDataL,
                  //channelRequestMetaDataClient,
                  //channelRepositoryMetaData,
                 _handle, val, alarmStatus, alarmSeverity, ts,  isCacheRequest);
  };


  int  get(//union db_access_val * PVDataL,
           //ChannelRequestMetaDataClient    channelRequestMetaDataClient,
           //ChannelRepositoryMetaData       channelRepositoryMetaData,
          const unsigned int  _handle, dbr_string_t * val, bool isCacheRequest) {
      epicsTimeStamp ts;
      dbr_short_t alarmStatus;
      dbr_short_t alarmSeverity;

      ts.secPastEpoch=0;
      ts.nsec        =0;
      alarmStatus    =0;
      alarmSeverity  =0;

      return get ( //PVDataL,
                   //channelRequestMetaDataClient,
                   //channelRepositoryMetaData,
                    _handle, val, alarmStatus, alarmSeverity, ts, isCacheRequest );
  };


  int  getCtrl(//const CAFEConduit & cc,
               const unsigned int  _handle, dbr_string_t * val,
           dbr_short_t & alarmStatus,  dbr_short_t &alarmSeverity, bool isCacheRequest );

private:
  int  status;
  CAFEStatus   cafeStatus;
  union db_access_val * PVDataL;
  unsigned int   nelem;
  unsigned int   offset;
  chtype dbrTypeRequest_DataBuffer;
  chtype dbrTypeRequest_CtrlBuffer;
  chtype _dataTypeClient;
  char stig [MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE];
};




//////////////////////////////////////////////////////////////////////////////////////////////////////////////



/**
 * Transpose Template Specialization for CAFE_DATATYPE_UNION_SEQ\n
 */
template <> class Transpose <CAFE_DATATYPE_UNION> {
   // CAFE_DATATYPE_UNION_SEQ val;
public:
    Transpose (){};
    ~Transpose (){};

    //long put(const unsigned int  _handle, CAFE_DATATYPE_UNION_SEQ val, CAFE_DATATYPE cdt);
    //long putString (const unsigned int  _handle, dbr_string_t * val) { return ICAFE_NORMAL;}


    int  get(//const CAFEConduit & cc,
          const unsigned int  _handle, CAFE_DATATYPE_UNION_SEQ val,
           dbr_short_t & alarmStatus,  dbr_short_t &alarmSeverity, epicsTimeStamp &ts, bool isCacheRequest);

    int  get(//const CAFEConduit & cc,
          const unsigned int  _handle, CAFE_DATATYPE_UNION_SEQ val,
           dbr_short_t & alarmStatus,  dbr_short_t &alarmSeverity, bool isCacheRequest) {

      epicsTimeStamp ts;
      alarmStatus    =0;
      alarmSeverity  =0;

      return get (_handle, val, alarmStatus, alarmSeverity, ts, isCacheRequest);
    };


    int  get(//const CAFEConduit & cc,
           const unsigned int  _handle,  CAFE_DATATYPE_UNION_SEQ val, bool isCacheRequest) {
      epicsTimeStamp ts;
      dbr_short_t alarmStatus;
      dbr_short_t alarmSeverity;

      ts.secPastEpoch=0;
      ts.nsec        =0;
      alarmStatus    =0;
      alarmSeverity  =0;

      return get (_handle, val, alarmStatus, alarmSeverity, ts, isCacheRequest);
    };

private:
    int  status;
    CAFEStatus   cafeStatus;
    union db_access_val * PVDataL;
    unsigned int   nelem;
    unsigned int   offset;
    chtype dbrTypeRequest_DataBuffer;
    chtype _dataTypeClient;
    char stig [MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE];
};


/**
 * \brief Transforms data from CTYPE * val to PutBuffer
 * \param  _handle  input: Conduit object
 * \param  val input: CTYPE datatype
 * \param _dbrType input: chtype
 * \return ICAFE_NORMAL as local data conversion should not incur an error
 */
template <class CTYPE> int  Transpose<CTYPE>::put(const unsigned int  _handle,
                                                   const CTYPE * val, const  chtype _dbrType)
{
#define __METHOD__ "Transpose<CTYPE>::put(_handle,  CTYPE * val, const chtype cdt)"

    status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;


    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end()) {


        PVDataL  = (*it_handle).getPutBuffer  ();
        nelem    = (*it_handle).getChannelRequestMetaPrimitive().getNelem();


        dbrTypeRequest_DataBuffer = (*it_handle).getChannelRequestMetaPrimitive().getDbrDataType();

        //dbrTypeRequest_DataBuffer is the chtype used in ca_get
        //Only ever fille the Union with the native type as

        //Need to first check if this is really an enum!?


        if (dbrTypeRequest_DataBuffer==DBR_ENUM) {

            dbr_ctrl_enum * dataEnum  =  (dbr_ctrl_enum *)  (*it_handle).getCtrlBuffer();
            dbr_short_t noStrings     =  ((struct dbr_ctrl_enum *) dataEnum)->no_str;

            if(noStrings >0) {

                for (unsigned int  i=0; i<nelem; ++i) {

                    if ( (dbr_enum_t) val[i] >= noStrings ) {

                        memcpy( stig,  &(((struct dbr_ctrl_enum *) dataEnum)->strs),  sizeof(stig  )) ;


                        cout << __FILE__ << "//" << __LINE__ << "//" <<__METHOD__ << endl;
                        cafeStatus.report(ECAFE_INVALID_ENUM_INDEX);
                        cout << val[i] << " is not a a valid enum index " << endl;
                        cout << "VALID STRING [ENUM] OPTIONS ARE: " << endl;
                        for (dbr_short_t ij=0; ij<noStrings; ++ij) {
                            cout << "'" << stig[ij] << "' " << "[" << ij << "]" << "; ";
                        }
                        cout << endl;
                        return ECAFE_INVALID_ENUM_INDEX;
                    } //if
                }//for
            }//if noStrings>0

        }//if
				
				
				//cout << "dbrTypeRequest_DataBuffer " << dbrTypeRequest_DataBuffer << endl;
				//cout << "_dbrType " << _dbrType << endl;
				
				//cout << "nelem " << nelem << endl;

        switch (dbrTypeRequest_DataBuffer)
        {
        case DBR_STRING: // 0
            switch(_dbrType){
            case CAFE_SHORT:
                for (unsigned int  i=0; i<nelem; ++i) {
                    sprintf(*((dbr_string_t *) (PVDataL) + i ),
                        "%d",  *reinterpret_cast<const short*>(&val[i]));
                }
                break;
            case CAFE_FLOAT:
                for (unsigned int  i=0; i<nelem; ++i) {
                    sprintf(*((dbr_string_t *) (PVDataL) + i ) ,
                        "%f",  *reinterpret_cast<const float*>(&val[i]));
                }
                break;
            case CAFE_ENUM:
                for (unsigned int  i=0; i<nelem; ++i) {
                    sprintf(*((dbr_string_t *) (PVDataL) + i ) ,
                        "%u",  *reinterpret_cast<const unsigned short*>(&val[i]));
                }
                break;
            case CAFE_CHAR:
                for (unsigned int  i=0; i<nelem; ++i) {
                    sprintf(*((dbr_string_t *) (PVDataL) + i ) ,
                        "%u",  *reinterpret_cast<const char*>(&val[i]));
                }
                break;
            case CAFE_LONG:
                for (unsigned int  i=0; i<nelem; ++i) {
                    sprintf(*((dbr_string_t *) (PVDataL) + i ) ,
                        "%d", *reinterpret_cast<const int*>(&val[i]));
                }
                break;
            case CAFE_DOUBLE:
                for (unsigned int  i=0; i<nelem; ++i) {
                    sprintf(*((dbr_string_t *) (PVDataL) + i ) ,
                        "%.15f", *reinterpret_cast<const double*>(&val[i]));
                }
                break;
            case CAFE_STRING:
                memcpy( PVDataL, val, sizeof(dbr_string_t)*nelem);

                break;
            default:
                cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
                cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" << _dbrType <<endl;
                cout << cafeDataTypeCode.message(_dbrType) << endl;
                cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
                cout << "LIKELY THAT THE DATA_TYPE WAS NOT MADE KNOWN IN THE INPUT PARAMETERS" << endl;
                //cout << "CAFE INTERNAL FUNNY: THIS LINE SHOULD NOT APPEAR" << endl;
                return ECAFE_INVALID_SWITCH_CASE;
                break;
            }

            break;

        case DBR_SHORT:  // 1
            switch(_dbrType) {
            case DBR_SHORT:
                memcpy( PVDataL, val, sizeof(dbr_short_t)*nelem);

                break;
            case DBR_FLOAT: //2
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_short_t *) (PVDataL) + i )  =   (dbr_short_t)   val[i] ;
                }
                break;
            case DBR_ENUM:  //3
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_short_t *) (PVDataL) + i )   =   (dbr_short_t)   val[i] ;
                }
                break;
            case DBR_CHAR:  //4
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_short_t *) (PVDataL) + i )   =   (dbr_short_t)   val[i] ;
                }
                break;
            case DBR_LONG: //5
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_short_t *) (PVDataL) + i )   =   (dbr_short_t)   val[i] ;
                }
                break;
            case DBR_DOUBLE:  //6
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_short_t *) (PVDataL) + i ) =   (dbr_short_t)   val[i] ;
                }
                break;
            case DBR_STRING:  //0
                 status=putString(_handle, (dbr_string_t *) val);

                break;
            default:
                cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
                cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" << _dbrType <<endl;
                cout << cafeDataTypeCode.message(_dbrType) << endl;
                cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
                cout << "LIKELY THAT THE DATA_TYPE WAS NOT MADE KNOWN IN THE INPUT PARAMETERS" << endl;
                //cout << "CAFE INTERNAL FUNNY: THIS LINE SHOULD NOT APPEAR" << endl;
                return ECAFE_INVALID_SWITCH_CASE;
                break;
            }
            break;
        case DBR_FLOAT:  // 2
            switch(_dbrType) {
            case DBR_SHORT:
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_float_t *) (PVDataL) + i )  =   (dbr_float_t)   val[i] ;
                }
                break;
            case DBR_FLOAT: //2
                memcpy( PVDataL, val, sizeof(dbr_float_t)*nelem);

                break;
            case DBR_ENUM:  //3
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_float_t *) (PVDataL) + i )   =   (dbr_float_t)   val[i] ;
                }
                break;
            case DBR_CHAR:  //4
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_float_t *) (PVDataL) + i )   =   (dbr_float_t)   val[i] ;
                }
                break;
            case DBR_LONG: //5
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_float_t *) (PVDataL) + i )   =   (dbr_float_t)   val[i] ;
                }
                break;
            case DBR_DOUBLE:  //6
						   
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_float_t *) (PVDataL) + i ) =   (dbr_float_t)   val[i] ;
										
                }
								
                break;
            case DBR_STRING:  //0
                status=putString(_handle, (dbr_string_t *) val);

                break;
            default:
                cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
                cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" << _dbrType <<endl;
                cout << cafeDataTypeCode.message(_dbrType) << endl;
                cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
                cout << "LIKELY THAT THE DATA_TYPE WAS NOT MADE KNOWN IN THE INPUT PARAMETERS" << endl;
                //cout << "CAFE INTERNAL FUNNY: THIS LINE SHOULD NOT APPEAR" << endl;
                return ECAFE_INVALID_SWITCH_CASE;
                break;
            }
            break;
        case DBR_ENUM:   // 3


            switch(_dbrType) {
            case DBR_SHORT:
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_enum_t *) (PVDataL) + i )  =   (dbr_enum_t)   val[i] ;
                }
                break;
            case DBR_FLOAT: //2
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_enum_t *) (PVDataL) + i )  =   (dbr_enum_t)   val[i] ;
                }
                break;
            case DBR_ENUM:  //3
                memcpy( PVDataL, val, sizeof(dbr_enum_t)*nelem);

                break;
            case DBR_CHAR:  //4
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_enum_t *) (PVDataL) + i )   =   (dbr_enum_t)   val[i] ;
                }
                break;
            case DBR_LONG: //5
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_enum_t *) (PVDataL) + i )   =   (dbr_enum_t)   val[i] ;
                }
                break;
            case DBR_DOUBLE:  //6
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_enum_t *) (PVDataL) + i ) =   (dbr_enum_t)   val[i] ;
                }
                break;
            case DBR_STRING:  //0
                status=putString(_handle, (dbr_string_t *) val);

                break;
            default:
                cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
                cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" << _dbrType <<endl;
                cout << cafeDataTypeCode.message(_dbrType) << endl;
                cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
                cout << "LIKELY THAT THE DATA_TYPE WAS NOT MADE KNOWN IN THE INPUT PARAMETERS" << endl;
                //cout << "CAFE INTERNAL FUNNY: THIS LINE SHOULD NOT APPEAR" << endl;
                return ECAFE_INVALID_SWITCH_CASE;
                break;
            }

            break;

        case DBR_CHAR:   // 4
            switch(_dbrType) {
            case DBR_SHORT:
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_char_t *) (PVDataL) + i )  =   (dbr_char_t)   val[i] ;
                }
                break;
            case DBR_FLOAT: //2
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_char_t *) (PVDataL) + i )  =   (dbr_char_t)   val[i] ;
                }
                break;
            case DBR_ENUM:  //3
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_char_t *) (PVDataL) + i )   =   (dbr_char_t)   val[i] ;
                }
                break;
            case DBR_CHAR:  //4
                memcpy( PVDataL, val, sizeof(dbr_char_t)*nelem);

                break;
            case DBR_LONG: //5
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_char_t *) (PVDataL) + i )   =   (dbr_char_t)   val[i] ;
                }
                break;
            case DBR_DOUBLE:  //6
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_char_t *) (PVDataL) + i ) =   (dbr_char_t)   val[i] ;
                }
                break;
            case DBR_STRING:  //0
                status=putString(_handle, (dbr_string_t *) val);

                break;
            default:
                cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
                cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" << _dbrType <<endl;
                cout << cafeDataTypeCode.message(_dbrType) << endl;
                cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
                cout << "LIKELY THAT THE DATA_TYPE WAS NOT MADE KNOWN IN THE INPUT PARAMETERS" << endl;
                //cout << "CAFE INTERNAL FUNNY: THIS LINE SHOULD NOT APPEAR" << endl;
                return ECAFE_INVALID_SWITCH_CASE;
                break;
            }
            break;

        case DBR_LONG:   // 5
            switch(_dbrType) {
            case DBR_SHORT:
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_long_t *) (PVDataL) + i )  =   (dbr_long_t)   val[i] ;
                }
                break;
            case DBR_FLOAT: //2
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_long_t *) (PVDataL) + i )  =   (dbr_long_t)   val[i] ;
                }
                break;
            case DBR_ENUM:  //3
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_long_t *) (PVDataL) + i )   =   (dbr_long_t)   val[i] ;
                }
                break;
            case DBR_CHAR:  //4
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_long_t *) (PVDataL) + i )   =   (dbr_long_t)   val[i] ;
                }
                break;
            case DBR_LONG: //5
                memcpy( PVDataL, val, sizeof(dbr_long_t)*nelem);

                break;
            case DBR_DOUBLE:  //6
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_long_t *) (PVDataL) + i ) =   (dbr_long_t)   val[i] ;
                }
                break;
            case DBR_STRING:  //0
                status=putString(_handle, (dbr_string_t *) val);

                break;
            default:
                cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
                cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" << _dbrType <<endl;
                cout << cafeDataTypeCode.message(_dbrType) << endl;
                cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
                cout << "LIKELY THAT THE DATA_TYPE WAS NOT MADE KNOWN IN THE INPUT PARAMETERS" << endl;
                //cout << "CAFE INTERNAL FUNNY: THIS LINE SHOULD NOT APPEAR" << endl;
                return ECAFE_INVALID_SWITCH_CASE;
                break;
            }
            break;


        case DBR_DOUBLE: // 6



            switch(_dbrType) {

            case DBR_SHORT:
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_double_t *) (PVDataL) + i )  =   (dbr_double_t)   val[i] ;
                }

                break;

            case DBR_FLOAT: //2
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_double_t *) (PVDataL) + i )  =   (dbr_double_t)   val[i] ;
                }


                break;
            case DBR_ENUM:  //3
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_double_t *) (PVDataL) + i )   =   (dbr_double_t)   val[i] ;
                }
                break;
            case DBR_CHAR:  //4
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_double_t *) (PVDataL) + i )   =   (dbr_double_t)   val[i] ;
                }
                break;
            case DBR_LONG: //5
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((dbr_double_t *) (PVDataL) + i )   =   (dbr_double_t)   val[i] ;
                }
                break;
            case DBR_DOUBLE:  //6

                memcpy( PVDataL, val, sizeof(dbr_double_t)*nelem);



                break;
            case DBR_STRING:  //0
                status=putString(_handle, (dbr_string_t *) val);

                break;

            default:
                cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
                cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" << _dbrType <<endl;
                cout << cafeDataTypeCode.message(_dbrType) << endl;
                cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
                cout << "LIKELY THAT THE DATA_TYPE WAS NOT MADE KNOWN IN THE INPUT PARAMETERS" << endl;
                //cout << "CAFE INTERNAL FUNNY: THIS LINE SHOULD NOT APPEAR" << endl;
                return ECAFE_INVALID_SWITCH_CASE;
                break;
            }
            break;
        default:
            cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
            cout << "SWITCH STATMENT NOT MEANT FOR DATATYPE=" << dbrTypeRequest_DataBuffer <<endl;
            cout << cafeDataTypeCode.message(dbrTypeRequest_DataBuffer) << endl;
            cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
            cout << "LIKELY THAT THE DATA_TYPE WAS NOT MADE KNOWN IN THE INPUT PARAMETERS" << endl;
            //cout << "CAFE INTERNAL FUNNY: THIS LINE SHOULD NOT APPEAR" << endl;
            return ECAFE_INVALID_SWITCH_CASE;
            break;
        }
    }
    else {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }

    cout << "status " << status << endl;

    return status;

#undef __METHOD__
};




/**
 * Transforms data from CAFE_DATATYPE_UNION_SEQ to PutBuffer
 * \param  _handle  input: Conduit object
 * \param  val input: CAFE_DATATYPE_UNION_SEQ
 * \param  cdt input:  CAFE_DATATYPE
 * \return ICAFE_NORMAL as local data conversion should not incur an error
 */
//long Transpose<CAFE_DATATYPE_UNION>::put( const unsigned int  _handle,
 //                                         CAFE_DATATYPE_UNION_SEQ val, CAFE_DATATYPE cdt)

template <class CTYPE> int  Transpose<CTYPE>::put( const unsigned int  _handle,
                                                     CAFE_DATATYPE_UNION_SEQ val, CAFE_DATATYPE cdt)
{
#define __METHOD__ "Transpose<CTYPE>::put()"

    status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end()) {

        PVDataL  = (*it_handle).getPutBuffer  ();
        nelem    = (*it_handle).getChannelRequestMetaPrimitive().getNelem();


        dbrTypeRequest_DataBuffer = (*it_handle).getChannelRequestMetaPrimitive().getDbrDataType();

        //dbrTypeRequest_DataBuffer is the chtype used in ca_get
        //Only ever fille the Union with the native type as

        dbr_ctrl_enum *  dataEnum; 
        dbr_short_t noStrings =0;


        switch (dbrTypeRequest_DataBuffer)
        {
        case DBR_STRING: // 0
            switch(cdt){
            case CAFE_SHORT:
                for (unsigned int  i=0; i<nelem; ++i) {
                    sprintf(*((dbr_string_t *) (PVDataL) + i ),
                        "%d",  *reinterpret_cast<const short*>(&val[i].s));
                }
                break;
            case CAFE_FLOAT:
                for (unsigned int  i=0; i<nelem; ++i) {
                    sprintf(*((dbr_string_t *) (PVDataL) + i ) ,
                        "%f",  *reinterpret_cast<const float*>(&val[i].f));
                }
                break;
            case CAFE_ENUM:
                for (unsigned int  i=0; i<nelem; ++i) {
                    sprintf(*((dbr_string_t *) (PVDataL) + i ) ,
                        "%u",  *reinterpret_cast<const unsigned short*>(&val[i].us));
                }
                break;
            case CAFE_CHAR:
                for (unsigned int  i=0; i<nelem; ++i) {
                    sprintf(*((dbr_string_t *) (PVDataL) + i ) ,
                        "%u",  *reinterpret_cast<const char*>(&val[i].ch));
                }
                break;
            case CAFE_LONG:
                for (unsigned int  i=0; i<nelem; ++i) {
                    sprintf(*((dbr_string_t *) (PVDataL) + i ) ,
                        "%d", *reinterpret_cast<const int*>(&val[i].l));
                }
                break;
            case CAFE_DOUBLE:
                for (unsigned int  i=0; i<nelem; ++i) {
                    sprintf(*((dbr_string_t *) (PVDataL) + i ) ,
                        "%.15lf", *reinterpret_cast<const double*>(&val[i].d));
                }
                break;
            case CAFE_STRING:
                for (unsigned int  i=0; i<nelem; ++i) {
                   // cout << "before " << val[i].str << endl;
                    sprintf(*((dbr_string_t *) (PVDataL) + i ) ,
                        "%s", *reinterpret_cast<char * const *>(&val[i].str));
                    //cout << "after " << *((dbr_string_t *) (PVDataL) + i) << endl;
                }
                break;
            default:
                cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
                cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" << cdt <<endl;
                cout << cafeDataTypeCode.message(cdt) << endl;
                cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
                cout << "LIKELY THAT THE DATA_TYPE WAS NOT MADE KNOWN IN THE INPUT PARAMETERS" << endl;
                //cout << "CAFE INTERNAL FUNNY: THIS LINE SHOULD NOT APPEAR" << endl;
                return ECAFE_INVALID_SWITCH_CASE;
                break;
            }

            break;
        case DBR_ENUM:   // 3


            // CHANGE THE STRING TO CORRESPONDING ENUM!
            dataEnum  =   (dbr_ctrl_enum *)  (*it_handle).getCtrlBuffer();
           

            noStrings           = ((struct dbr_ctrl_enum *) dataEnum)->no_str;

            memcpy( stig,  &(((struct dbr_ctrl_enum *) dataEnum)->strs),  sizeof(stig  )) ;


                switch(cdt) {

                case CAFE_SHORT:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        *((CTYPE *) (PVDataL) + i )  =   (CTYPE)   val[i].s ;
                        if (  val[i].s < 0 || (dbr_enum_t) val[i].s >= (dbr_enum_t) noStrings) {
                            status=ECAFE_INVALID_ENUM_INDEX;
                        }
                    }

                    break;

                case CAFE_FLOAT: //2
                    for (unsigned int  i=0; i<nelem; ++i) {
                        *((CTYPE *) (PVDataL) + i )  =   (CTYPE)   val[i].f ;
                        if (  val[i].f < 0 ||  (dbr_enum_t) val[i].f >= (dbr_enum_t) noStrings) {
                            status=ECAFE_INVALID_ENUM_INDEX;
                        }
                    }


                    break;
                case CAFE_ENUM:  //3
                    for (unsigned int  i=0; i<nelem; ++i) {
                        *((CTYPE *) (PVDataL) + i )   =   (CTYPE)   val[i].us ;
                        if ( (dbr_enum_t) val[i].us >= (dbr_enum_t) noStrings) {
                            status=ECAFE_INVALID_ENUM_INDEX;
                        }
                    }
                    break;
                case CAFE_CHAR:  //4
                    for (unsigned int  i=0; i<nelem; ++i) {
                        *((CTYPE *) (PVDataL) + i )   =   (CTYPE)   val[i].ch ;
                        if ( (dbr_enum_t) val[i].ch >= (dbr_enum_t) noStrings) {
                            status=ECAFE_INVALID_ENUM_INDEX;
                        }
                    }
                    break;
                case CAFE_LONG: //5
                    for (unsigned int  i=0; i<nelem; ++i) {
                        *((CTYPE *) (PVDataL) + i )   =   (CTYPE)   val[i].l ;
                        if (  val[i].l < 0 || (dbr_enum_t) val[i].l >= (dbr_enum_t)noStrings) {
                            status=ECAFE_INVALID_ENUM_INDEX;
                        }
                    }
                    break;
                case CAFE_DOUBLE:  //6
                    for (unsigned int  i=0; i<nelem; ++i) {
                        *((CTYPE *) (PVDataL) + i ) =   (CTYPE)   val[i].d ;
                        if (  val[i].d < 0 || (dbr_enum_t) val[i].d >= (dbr_enum_t)noStrings) {
                            status=ECAFE_INVALID_ENUM_INDEX;
                        }
                    }


                    break;
                case CAFE_STRING:  //0
                    status=putString(_handle, (dbr_string_t *) val);

                    break;

                default:
                    cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                    cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
                    cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" << cdt <<endl;
                    cout << cafeDataTypeCode.message(cdt) << endl;
                    cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
                    cout << "LIKELY THAT THE DATA_TYPE WAS NOT MADE KNOWN IN THE INPUT PARAMETERS" << endl;
                    //cout << "CAFE INTERNAL FUNNY: THIS LINE SHOULD NOT APPEAR" << endl;
                    return ECAFE_INVALID_SWITCH_CASE;
                    break;
                }


                break;

        case DBR_SHORT:  // 1
        case DBR_FLOAT:  // 2


        case DBR_CHAR:   // 4
        case DBR_LONG:   // 5
        case DBR_DOUBLE: // 6

            switch(cdt) {

            case CAFE_SHORT:
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((CTYPE *) (PVDataL) + i )  =   (CTYPE)   val[i].s ;
                }

                break;

            case CAFE_FLOAT: //2
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((CTYPE *) (PVDataL) + i )  =   (CTYPE)   val[i].f ;
                }


                break;
            case CAFE_ENUM:  //3
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((CTYPE *) (PVDataL) + i )   =   (CTYPE)   val[i].us ;
                }
                break;
            case CAFE_CHAR:  //4
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((CTYPE *) (PVDataL) + i )   =   (CTYPE)   val[i].ch ;
                }
                break;
            case CAFE_LONG: //5
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((CTYPE *) (PVDataL) + i )   =   (CTYPE)   val[i].l ;
                }
                break;
            case CAFE_DOUBLE:  //6
                for (unsigned int  i=0; i<nelem; ++i) {
                    *((CTYPE *) (PVDataL) + i ) =   (CTYPE)   val[i].d ;
                }

                break;
            case CAFE_STRING:  //0
                status=putString(_handle, (dbr_string_t *) val);

                break;

            default:
                cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
                cout << "SWITCH STATMENT NOT MEANT FOR CAFE DATATYPE=" << cdt <<endl;
                cout << cafeDataTypeCode.message(cdt) << endl;
                cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
                cout << "LIKELY THAT THE DATA_TYPE WAS NOT MADE KNOWN IN THE INPUT PARAMETERS" << endl;
                //cout << "CAFE INTERNAL FUNNY: THIS LINE SHOULD NOT APPEAR" << endl;
                return ECAFE_INVALID_SWITCH_CASE;
                break;
            }
            break;
        default:
            cout << "ERROR: " << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_SWITCH_CASE);
            cout << "SWITCH STATMENT NOT MEANT FOR DATATYPE=" << dbrTypeRequest_DataBuffer <<endl;
            cout << cafeDataTypeCode.message(dbrTypeRequest_DataBuffer) << endl;
            cout << "ABORTING DATA CONVERTION TO LOCAL putBuffer" << endl;
            cout << "LIKELY THAT THE DATA_TYPE WAS NOT MADE KNOWN IN THE INPUT PARAMETERS" << endl;

            //cout << "CAFE INTERNAL FUNNY: THIS LINE SHOULD NOT APPEAR" << endl;
            return ECAFE_INVALID_SWITCH_CASE;
            break;
        }
    }
    else {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }

    return status;
#undef __METHOD__
};





/**
 * Converts data from CTYPE to native type
 * in preparation for transmission to CA Server
 * \param  _handle  input: handel to Conduit object
 * \param val  input: Array of values of datatype dbr_string_t
 * \return ICAFE_NORMAL else ECAFE_NO_CONVERT
 */
template <class CTYPE> int  Transpose<CTYPE>::putString
        (const unsigned int  _handle, dbr_string_t * val)
{
#define __METHOD__ "Transpose<CTYPE>::putString"

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end()) {

        PVDataL  = (*it_handle).getPutBuffer  ();
        nelem    = (*it_handle).getChannelRequestMetaPrimitive().getNelem();

        dbrTypeRequest_DataBuffer = (*it_handle).getChannelRequestMetaPrimitive().getDbrDataType();

        dbr_ctrl_enum *  dataEnum; 
        dbr_short_t noStrings =0;

        bool allStringsAreValid=true;
        // Client is String
        // Native type is one of the following

        istringstream ss;

        switch(dbrTypeRequest_DataBuffer) {

        case DBR_STRING://0


            for (unsigned int  i=0; i<nelem; ++i)  {
                strcpy( *((dbr_string_t *) (PVDataL) +i), val[i]);
            }
            break;

        case DBR_SHORT://1
            for (unsigned int  i=0; i<nelem; ++i)  {
                dbr_short_t s=0;
                ss.clear();
                ss.str(val[i]);
                ss>>s;

                if ( !ss.fail()) {
                    *((dbr_short_t *) (PVDataL) + i ) = s;
                }
                else {
                    cout << __METHOD__ << __LINE__ << endl;
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
            for (unsigned int  i=0; i<nelem; ++i)  {
                dbr_float_t f=0;
                ss.clear();
                ss.str(val[i]);
                ss>>f;

                if ( !ss.fail()) {
                    *((dbr_float_t *) (PVDataL) + i ) = f;
                }
                else {
                    cout << __METHOD__ << __LINE__ << endl;
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


            noStrings           = ((struct dbr_ctrl_enum *) dataEnum)->no_str;

            memcpy( stig,  &(((struct dbr_ctrl_enum *) dataEnum)->strs),  sizeof(stig  )) ;


            for (unsigned int  i=0; i<nelem; ++i) {
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

                bool isValidString=false;
                for (int j=0; j<noStrings; ++j) {

                    if (strcmp((char *)a, stig[j] ) ==0) {

                        dbr_enum_t us= (unsigned short) j;
                        *((dbr_enum_t *) (PVDataL) + i ) = us;
                        isValidString=true;
                        //cout << "setting value " << j << " " << stig[j] << endl;
                        break;
                    }
                }

                // Check for ENUM values given in "string" form

                if (!isValidString) {

                    bool isValidStringAnew=false;

                    dbr_enum_t us=0;
                    ss.clear();
                    ss.str(a);
                    ss>>us;

                    // Is this a valid number?
                    if ( !ss.fail()) {
                        if (us==0 || (us>0 && us <noStrings)) {
                            *((dbr_enum_t *) (PVDataL) + i ) = us;
                            isValidStringAnew=true;
                        }
                        else {
                            status=ECAFE_INVALID_ENUM_INDEX;
                        }
                    }

                    if (!isValidStringAnew) {

                        cout << __METHOD__ << __LINE__ << endl;
                        cout << "***WARNING*** NO STRING TO DBR_ENUM MATCHING for ELEMENT " << i << " of " << nelem << " !! " << endl;
                        cout << "***WARNING*** COULD NOT CONVERT: '";
                        cout << a; //val[i];
                        cout << "' TO A VALID ENUM; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl;
                        cout << "VALID STRING [ENUM] OPTIONS ARE: " << endl;


                        for (dbr_short_t ij=0; ij<noStrings; ++ij) {
                            cout << "'" << stig[ij] << "' " << "[" << ij << "]" << "; ";
                        }
                        cout << endl;


                        allStringsAreValid=false;
                    }

                }

            } //nelem for loop



            break;

        case DBR_CHAR: //4
            for (unsigned int  i=0; i<nelem; ++i)  {
                dbr_char_t ch=0;
                ss.clear();
                ss.str(val[i]);
                ss>>ch;

								char *b = (char *)  val[i];
          

                if ( !ss.fail()) {
                    *((dbr_char_t *) (PVDataL) + i ) = ch;
                }
								
								else if (b != '\0') {
								
										*((dbr_char_t *) (PVDataL) + i ) =  *b;	
								
								}
                else {
                    cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_CHAR CONVERSION for ELEMENT index " << i << " in array of length " << nelem;
										cout <<	", i.e., with index range [0-"	<< (nelem-1) << "] "	<<  " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i];
                    cout << " TO UNSIGNED CHAR; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl; //NOT MADE!" << endl;
                     allStringsAreValid=false;
                }

            }
            break;

        case DBR_LONG: //5
            for (unsigned int  i=0; i<nelem; ++i)  {
                dbr_long_t l=0;
                ss.clear();
                ss.str(val[i]);
                ss>>l;

                if ( !ss.fail()) {
                    *((dbr_long_t *) (PVDataL) + i ) = l;
                }
                else {
                    cout << __METHOD__ << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_LONG CONVERSION for ELEMENT " << i << " of " << nelem << " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i];
                    cout << " TO LONG; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl; //NOT MADE!" << endl;

                    allStringsAreValid=false;
                }
            }
            break;

        case DBR_DOUBLE: //6
            for (unsigned int  i=0; i<nelem; ++i)  {
                dbr_double_t d=0;
                ss.clear();
                ss.str(val[i]);
                ss>>d;

                if ( !ss.fail()) {
                    *((dbr_double_t *) (PVDataL) + i ) = d;
                }
                else {
                    cout << __METHOD__ << __LINE__ << endl;
                    cout << "***WARNING*** NO STRING TO DBR_DOUBLE CONVERSION for ELEMENT " << i << " of " << nelem << " !! " << endl;
                    cout << "***WARNING*** COULD NOT CONVERT: ";
                    cout << val[i];
                    cout << " TO DOUBLE; PUT REQUEST NOT MADE!" << endl; //AT THE MERCY OF THE CA SERVER!" << endl;  //PUT REQUEST NOT MADE!" << endl;

                    allStringsAreValid=false;
                }

            }//for
            break;
        } //switch


        if(!allStringsAreValid) {
            if (status==ECAFE_INVALID_ENUM_INDEX) {return status;}
            return ECAFE_NO_CONVERT;}
        else {return ICAFE_NORMAL;}

    }
    else {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}



/**
 * \brief Retrieves data transmitted by CA with dbrTypeRequest_DataBuffer
 * and then converts to CTYPE i.e.  _dataTypeClient
 * \param  _handle  input: handle to Conduit object
 * \param val output: CTYPE datatype
 * \param  alarmStatus   output: dbr_short_t
 * \param  alarmSeverity output: dbr_short_t
 * \param  ts output: epicsTimeStamp
 * \param isCacheRequest input: bool
 * \return ICAFE_NORMAL as local data conversion should not incur an error
**/
template <class CTYPE> int  Transpose<CTYPE>::get(
                        const unsigned int  _handle, CTYPE * val,
          dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts, bool isCacheRequest)
{
#define __METHOD__ "Transpose<CTYPE>::get()"

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end()) {

        PVDataL  = (*it_handle).getDataBuffer();
        offset   = (*it_handle).getChannelRequestMetaDataClient().getOffset();
				//cout << "offset client " << offset << endl;
				//cout << "offset client too...... " << handleHelper.getOffset(_handle) << endl;
				//cout << "offsetLast  ...... " << handleHelper.getOffsetLast(_handle) << endl;
				
				// Difference between using cache and not
				// when not cache offset is that requested by client..
				// when not cache nelem is no of elements requested by client plus the offset requested by client
				// when     cache offset is actual offset in last transaction 
				// when     cache nelem is no of elements from cache 
				
        if(isCacheRequest) {		
						
						offset = (*it_handle).getChannelRequestMetaData().getOffset( );   // equivalent to handleHelper.getOffsetLast(_handle);					
            nelem  = (*it_handle).getChannelRequestMetaData().getNelemCache();// + offset;  // not (*it_handle).getChannelRequestMetaData().getOffset()
						
						//Could be that the number of elements in the data buffer is actually less!   
						//cout << "nelem1 + data offset " << nelem << " " << (*it_handle).getChannelRequestMetaData().getNelemCache() << " + " << 
						//(*it_handle).getChannelRequestMetaData().getOffset() << endl; 
						//nelem  = min(nelem, (*it_handle).getChannelRequestMetaData().getNelemCache()+ offset);
						//cout << "nelem2 " << nelem << " " << (*it_handle).getChannelRequestMetaData().getNelemCache() << " + " << offset << endl; 			
						//cout << __METHOD__ << __LINE__ << endl;
						//cout << "nelem = " << (*it_handle).getChannelRequestMetaData().getNelem() << endl;
						//cout << "nelemCache = " << (*it_handle).getChannelRequestMetaData().getNelemCache() << endl;
						//cout << "offset = " << (*it_handle).getChannelRequestMetaData().getOffset() << endl;
						//cout << "offsetLast  ...... " << handleHelper.getOffsetLast(_handle) << endl;
						//cout << "nelem = " << nelem << endl;
						
        }
        else {
            nelem    = (*it_handle).getChannelRequestMetaData().getNelem()-offset;
						//cout << __METHOD__ << " nelem " << nelem << endl;
        		//cout <<  "getChannelRequestMetaData().getNelem()=" << (*it_handle).getChannelRequestMetaData().getNelem() << endl;
					  //cout <<  "getChannelRequestMetaDataClient().offset " << offset << endl;
												
				}
        //Something wrong, just read last element
        if (nelem <=0) {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "Something funny with the offset; hence will read last element only!"<< endl;
            cout << "Changing offset from=" << offset;

            offset = (*it_handle).getChannelRequestMetaData().getNelem()-1;
            if(isCacheRequest) {
						    //this does not mean much for cache
                nelem  = (*it_handle).getChannelRequestMetaData().getNelemCache(); //(*it_handle).getChannelRequestMetaData().getOffset(); //add offset for cache
            }
            else {
                nelem  = (*it_handle).getChannelRequestMetaData().getNelem()-offset;
            }

            cout << " to=" << offset << endl;
        }

    dbrTypeRequest_DataBuffer = (*it_handle).getChannelRequestMetaData().getDbrDataType();
    _dataTypeClient           = (*it_handle).getChannelRequestMetaDataClient().getDataType();

    //cout << "WHAT IS ON THE DATA BUFFER = " << dbrTypeRequest_DataBuffer  << endl;
    //cout << "WHAT THE CLIENT WANTS    = " << _dataTypeClient << endl;

    ts.secPastEpoch=0;
    ts.nsec        =0;
    alarmStatus    =-1;
    alarmSeverity  =-1;


    if (_dataTypeClient < DBR_STRING || _dataTypeClient > DBR_DOUBLE) {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << "Datatype requested " << dbr_type_to_text(_dataTypeClient)
                << " is not within allowed range: " << endl;
        cout << "DBR_STRING <= dataType <= DBR_DOUBLE" << endl;
        cout << "CAFE ERROR: ECAFE_INVALID_SWITCH_CASE" << endl;
        return ECAFE_INVALID_SWITCH_CASE;
    }

    //dbrTypeRequest_DataBuffer is the chtype used in ca_get
    //_dataTypeCLient is the chtype requested by client

    switch (dbrTypeRequest_DataBuffer)
    {
        case DBR_CHAR:
            switch(_dataTypeClient)
            {
                case DBR_CHAR:
                    memcpy( val, &(&((PVDataL)->charval))[offset], sizeof(dbr_char_t)*nelem);
                    break;

            default:
                for (unsigned int  i=0; i<nelem; ++i) {
                    val[i] =  (CTYPE) (*(&((PVDataL)->charval)+i+offset));
                }
                break;
            }
            break;

        case DBR_FLOAT:
            switch(_dataTypeClient)
            {
                case DBR_FLOAT:
                    memcpy( val, &(&((PVDataL)->fltval))[offset], sizeof(dbr_float_t)*nelem);
                    break;
                default:
                    for (unsigned int  i=0; i<nelem; ++i){
                        val[i] =  (CTYPE) (*(&((PVDataL)->fltval)+i+offset));
                    }
                    break;
            }

            break;

        case DBR_DOUBLE:
            switch(_dataTypeClient)
            {
                case DBR_DOUBLE:
                    memcpy( val, &(&((PVDataL)->doubleval))[offset], sizeof(dbr_double_t)*nelem);
                    break;
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->doubleval)+i+offset));
                    }
                    break;
            }
            break;

        case DBR_SHORT:
            switch(_dataTypeClient)
            {
                case DBR_SHORT:
                    memcpy( val, &(&((PVDataL)->shrtval))[offset], sizeof(dbr_short_t)*nelem);
                    break;
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->shrtval)+i+offset));
                    }
                    break;
            }
            break;

        case DBR_LONG:
            switch(_dataTypeClient)
            {
                case DBR_LONG:
                    memcpy( val, &(&((PVDataL)->longval))[offset], sizeof(dbr_long_t)*nelem);
                    break;
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->longval)+i+offset));
                    }
                    break;
            }
            break;

        case DBR_ENUM:
            switch(_dataTypeClient)
            {
                case DBR_ENUM:
                    memcpy( val, &(&((PVDataL)->enmval))[offset], sizeof(dbr_enum_t)*nelem);
                    break;
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->enmval)+i+offset));
                    }
                    break;
            }
            break;

        case DBR_STRING:
            switch(_dataTypeClient)
            {
                case DBR_STRING:
                    memcpy( val, &(&((PVDataL)->strval))[offset], sizeof(dbr_string_t)*nelem);
                    break;

                case DBR_SHORT:
                case DBR_LONG:
                case DBR_ENUM:
                case DBR_CHAR:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i]=  (CTYPE) strtol(  (*(&((PVDataL)->strval)+i+offset)), NULL, 0);
                    }
                    break;
                case DBR_FLOAT:
                case DBR_DOUBLE:
                default:
                   // If no valid conversion could be performed, the function returns zero (0.0).
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i]=  (CTYPE) strtod(  (*(&((PVDataL)->strval)+i+offset)), NULL);
                    }
                    break;
            }
            break;

        case DBR_STS_CHAR:
            switch(_dataTypeClient)
            {
                case DBR_CHAR:
                    memcpy( val, &(&((PVDataL)->schrval.value))[offset],  sizeof(dbr_char_t)*nelem);
                    break;
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->schrval.value)+i+offset));
                    }
                    break;
            }
            alarmStatus   = ((struct dbr_sts_char *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_char *) PVDataL)->severity;
            break;

        case DBR_STS_FLOAT:
            switch(_dataTypeClient)
            {
                case DBR_FLOAT:
                    memcpy( val, &(&((PVDataL)->sfltval.value))[offset], sizeof(dbr_float_t)*nelem);
                    break;
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->sfltval.value)+i+offset));
                    }
                    break;
                }
            alarmStatus   = ((struct dbr_sts_float *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_float *) PVDataL)->severity;
            break;

        case DBR_STS_DOUBLE:
            switch(_dataTypeClient)
            {              
                case DBR_DOUBLE:
                    memcpy( val, &(&((PVDataL)->sdblval.value))[offset], sizeof(dbr_double_t)*nelem);
                    break;
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->sdblval.value)+i+offset));
                    }
                    break;
                }
            alarmStatus   = ((struct dbr_sts_double *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_double *) PVDataL)->severity;
            break;

        case DBR_STS_SHORT:
            switch(_dataTypeClient)
            {
                case DBR_SHORT:
                    memcpy( val, &(&((PVDataL)->sshrtval.value))[offset], sizeof(dbr_short_t)*nelem);
                    break;
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->sshrtval.value)+i+offset));
                    }
                   break;
            }
            alarmStatus   = ((struct dbr_sts_short *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_short *) PVDataL)->severity;
            break;

        case DBR_STS_LONG:
            switch(_dataTypeClient)
            {
                case DBR_LONG:
                    memcpy( val, &(&((PVDataL)->slngval.value))[offset], sizeof(dbr_long_t)*nelem);
                    break;
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->slngval.value)+i+offset));
                    }
                    break;
            }
            alarmStatus   = ((struct dbr_sts_long *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_long *) PVDataL)->severity;
            break;

        case DBR_STS_ENUM:
            switch(_dataTypeClient)
            {
                case DBR_ENUM:
                    memcpy( val, &(&((PVDataL)->senmval.value))[offset], sizeof(dbr_enum_t)*nelem);
                    break;
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->senmval.value)+i+offset));
                    }
                    break;
            }
            alarmStatus   = ((struct dbr_sts_enum *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_enum *) PVDataL)->severity;
            break;

        case DBR_STS_STRING:
            switch(_dataTypeClient)
            {
                case DBR_STRING:
                    memcpy( val, &(&((PVDataL)->sstrval.value))[offset], sizeof(dbr_string_t)*nelem);
                    break;
                case DBR_SHORT:
                case DBR_LONG:
                case DBR_ENUM:
                case DBR_CHAR:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) strtol(  (*(&((PVDataL)->sstrval.value)+i+offset)), NULL, 0);
                    }
                    break;
                case DBR_FLOAT:
                case DBR_DOUBLE:
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) strtod(  (*(&((PVDataL)->sstrval.value)+i+offset)), NULL);
                    }
                    break;
                }
            alarmStatus   = ((struct dbr_sts_string *) PVDataL)->status;
            alarmSeverity = ((struct dbr_sts_string *) PVDataL)->severity;
            break;

        case DBR_TIME_CHAR:
            switch(_dataTypeClient)
            {
                case DBR_CHAR:
                    memcpy( val, &(&((PVDataL)->tchrval.value))[offset], sizeof(dbr_char_t)*nelem);
                    break;
                case DBR_SHORT:
                case DBR_LONG:
                case DBR_ENUM:
                case DBR_FLOAT:
                case DBR_DOUBLE:
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->tchrval.value)+i+offset));
                    }
                    break;
                // strings are handled by getString - but are handled HERE when calling getCache()!!
                //case DBR_STRING:
                //	for (unsigned int  i=0; i<nelem; ++i) sprintf(val[i], "%u",   (dbr_char_t) (*(&((PVDataL)->tchrval.value)+i)));
            }
            ts            = ((struct dbr_time_char *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_char *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_char *) PVDataL)->severity;
            break;

        case DBR_TIME_FLOAT:
            switch(_dataTypeClient)
            {
                case DBR_FLOAT:
                    memcpy( val, &(&((PVDataL)->tfltval.value))[offset], sizeof(dbr_float_t)*nelem);
                    break;
                case DBR_SHORT:
                case DBR_LONG:
                case DBR_ENUM:
                case DBR_CHAR:
                case DBR_DOUBLE:
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->tfltval.value)+i+offset));
                     }
                    break;
            }
            ts            = ((struct dbr_time_float *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_float *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_float *) PVDataL)->severity;
            break;

        case DBR_TIME_DOUBLE:
            switch(_dataTypeClient)
            {
                case DBR_DOUBLE:
                    memcpy( val, &(&((PVDataL)->tdblval.value))[offset], sizeof(dbr_double_t)*nelem);
                    break;
                case DBR_SHORT:
                case DBR_LONG:
                case DBR_ENUM:
                case DBR_CHAR:
                case DBR_FLOAT:
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->tdblval.value)+i+offset));
                    }
                    break;
            }
            ts            = ((struct dbr_time_double *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_double *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_double *) PVDataL)->severity;

            break;

        case DBR_TIME_SHORT:
            switch(_dataTypeClient)
            {
                case DBR_SHORT:
                    memcpy( val, &(&((PVDataL)->tshrtval.value))[offset], sizeof(dbr_short_t)*nelem);
                    break;
                case DBR_DOUBLE:
                case DBR_LONG:
                case DBR_ENUM:
                case DBR_CHAR:
                case DBR_FLOAT:
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->tshrtval.value)+i+offset));
                    }
                    break;
                //case DBR_STRING:
                //	for (unsigned int  i=0; i<nelem; ++i) sprintf(val[i], "%d",(*(&((PVDataL)->tshrtval.value)+i)));
            }
            ts            = ((struct dbr_time_short *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_short *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_short *) PVDataL)->severity;
            break;

        case DBR_TIME_LONG:
            switch(_dataTypeClient){
                case DBR_LONG:
                    memcpy( val, &(&((PVDataL)->tlngval.value))[offset], sizeof(dbr_long_t)*nelem);
                    break;
                case DBR_DOUBLE:
                case DBR_SHORT:
                case DBR_ENUM:
                case DBR_CHAR:
                case DBR_FLOAT:
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->tlngval.value)+i+offset));
                    }
                    break;
                //case DBR_STRING:
                //	for (unsigned int  i=0; i<nelem; ++i) sprintf(val[i], "%ld", (*(&((PVDataL)->tlngval.value)+i)));
                //break;
            }
            ts            = ((struct dbr_time_long *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_long *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_long *) PVDataL)->severity;
            break;

        case DBR_TIME_ENUM:
            switch(_dataTypeClient)
            {
                case DBR_ENUM:
                    memcpy( val, &(&((PVDataL)->tenmval.value))[offset], sizeof(dbr_enum_t)*nelem);
                    break;
                case DBR_DOUBLE:
                case DBR_SHORT:
                case DBR_LONG:
                case DBR_CHAR:
                case DBR_FLOAT:
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i] =  (CTYPE) (*(&((PVDataL)->tenmval.value)+i+offset));
                    }
                    break;
                //case DBR_STRING:
                //	for (unsigned int  i=0; i<nelem; ++i) sprintf(val[i], "%d", (*(&((PVDataL)->tenmval.value)+i)));
                //break;
            }
            ts            = ((struct dbr_time_enum *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_enum *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_enum *) PVDataL)->severity;
            break;


        case DBR_TIME_STRING:
        default:
            switch(_dataTypeClient){
                case DBR_STRING:
                    memcpy( val, &(&((PVDataL)->strval))[offset], sizeof(dbr_string_t)*nelem);
                    break;
                case DBR_SHORT:
                case DBR_LONG:
                case DBR_ENUM:
                case DBR_CHAR:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i]=  (CTYPE) strtol(  (*(&((PVDataL)->tstrval.value)+i+offset)), NULL, 0);
                    }
                    break;
                case DBR_FLOAT:
                case DBR_DOUBLE:
                default:
                    for (unsigned int  i=0; i<nelem; ++i) {
                        val[i]=  (CTYPE) strtod(  (*(&((PVDataL)->tstrval.value)+i+offset)), NULL);
                    }
                    break;
            }
            ts            = ((struct dbr_time_string *) PVDataL)->stamp;
            alarmStatus   = ((struct dbr_time_string *) PVDataL)->status;
            alarmSeverity = ((struct dbr_time_string *) PVDataL)->severity;
            break;
        }
		//helper function to set TimeStamps!
		//cout <<__METHOD__ << endl;
		//cout << "_dbrType" << _dbrTypeRequest_DataBuffer<< endl;
		//cout << ts.secPastEpoch << " " << ts.nsec << endl;

		if(!isCacheRequest) {
			handleHelper.setSTS(_handle, alarmStatus, alarmSeverity, ts);
		}

    }
    else {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;


#undef __METHOD__
}


/**
 * \brief  Retrieves ctrl data from buffer
 * \param  _handle  input: handle to Conduit Object
 * \param  val output: CTYPE
 * \param  alarmStatus   output: dbr_short_t
 * \param  alarmSeverity output: dbr_short_t
 * \param  precision     output: dbr_short_t
 * \param  RISC_pad     deprecated!
 * \param  upperDispLimit  output
 * \param  lowerDispLimit  output
 * \param  upperAlarmLimit output
 * \param  upperWarningLimit output
 * \param  lowerWarningLimit output
 * \param  lowerAlarmLimit   output
 * \param  upperCtrlLimit    output
 * \param  lowerCtrlLimit    output
 * \param  units             output: units
 * \param  noStr             output: No of ENUM Choices
 * \param  strs              output: ENUM Choices
 * \param isCacheRequest     input:  bool true or false
 * \return ICAFE_NORMAL
 */
template <class CTYPE> int  Transpose<CTYPE>::getCtrl (
        //const CAFEConduit &cc, union db_access_val * PVDataL,
    //ChannelRequestMetaDataClient    channelRequestMetaDataClient,
    //ChannelRepositoryMetaData       channelRepositoryMetaData,
    const unsigned int  _handle, CTYPE * val,
    dbr_short_t &alarmStatus,  dbr_short_t &alarmSeverity, dbr_short_t &precision,
    CTYPE   &RISC_pad,
    CTYPE   &upperDispLimit,    CTYPE  &lowerDispLimit,
    CTYPE   &upperAlarmLimit,   CTYPE  &upperWarningLimit,
    CTYPE   &lowerWarningLimit, CTYPE  &lowerAlarmLimit,
    CTYPE   &upperCtrlLimit,    CTYPE  &lowerCtrlLimit,
    char units[MAX_UNITS_SIZE], short &noStr, char strs [MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE],
    bool isCacheRequest)  {
#define __METHOD__ "Transpose<CTYPE>::getCtrl()"

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);


    if (it_handle != handle_index.end()) {

    PVDataL  = (*it_handle).getCtrlBuffer  ();

    offset   = (*it_handle).getChannelRequestMetaCtrlClient().getOffset();
		
				
		
    if(isCacheRequest) {
		   	offset = (*it_handle).getChannelRequestMetaCtrl().getOffset( ); 
        nelem  = (*it_handle).getChannelRequestMetaCtrl().getNelemCache();; // + (*it_handle).getChannelRequestMetaCtrl().getOffset();
				//nelem  = min(nelem, (*it_handle).getChannelRequestMetaCtrl().getNelem() + offset);
				
    }
    else {
        nelem   = (*it_handle).getChannelRequestMetaCtrl().getNelem()-offset;
    }
    //Something wrong, just read last element
    if (nelem <=0) {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "Something funny with the offset; just read last element! " << endl;
        cout << "Changing offset from=" << offset;

        offset = (*it_handle).getChannelRequestMetaCtrl().getNelem()-1;
        if(isCacheRequest) {
            nelem  = (*it_handle).getChannelRequestMetaCtrl().getNelemCache(); // + (*it_handle).getChannelRequestMetaCtrl().getOffset();  //-offset;
        }
        else {
            nelem  = (*it_handle).getChannelRequestMetaCtrl().getNelem()-offset;
        }
        cout << " to=" << offset << endl;
    }


    dbrTypeRequest_CtrlBuffer = (*it_handle).getChannelRequestMetaCtrl().getDbrDataType();
    _dataTypeClient           = (*it_handle).getChannelRequestMetaDataClient().getDataType();


    alarmStatus     = -1;
    alarmSeverity   = -1;

    if (_dataTypeClient < DBR_STRING || _dataTypeClient > DBR_DOUBLE) {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cout << "Datatype requested " << dbr_type_to_text(_dataTypeClient)
             << " is not within allowed range: " << endl;
        cout << "DBR_STRING <= dataType <= DBR_DOUBLE" << endl;
        cout << "CAFE ERROR: ECAFE_INVALID_SWITCH_CASE" << endl;
        return ECAFE_INVALID_SWITCH_CASE;
    }


    noStr=0;

    //cout << __METHOD__ << endl;
		//cout <<  " dbrTypeRequest_CtrlBuffer " << dbrTypeRequest_CtrlBuffer << endl;
		//cout <<  " _dataTypeClient " << _dataTypeClient << endl;


    switch (dbrTypeRequest_CtrlBuffer) {

    case DBR_CTRL_CHAR:

        switch(_dataTypeClient){
        case DBR_CHAR:
            memcpy(val, &(&((PVDataL)->cchrval.value))[offset], sizeof(dbr_char_t)*nelem);
            break;
        case DBR_DOUBLE:
        case DBR_SHORT:
        case DBR_ENUM:
        case DBR_LONG:
        case DBR_FLOAT:
            for (unsigned int  i=0; i<nelem; ++i) {
                val[i] =  (CTYPE) (*(&((PVDataL)->cchrval.value)+i+offset));
            }
            break;
            // strings are handled by a specialized template
        case DBR_STRING:
            default:
            break;
        }

        precision     = 0;  // struct dbr_ctrl_char does not have the precision member
        RISC_pad      = ((struct dbr_ctrl_char *) PVDataL)->RISC_pad;
        alarmStatus   = ((struct dbr_ctrl_char *) PVDataL)->status;
        alarmSeverity = ((struct dbr_ctrl_char *) PVDataL)->severity;

        memcpy(units, &(((struct dbr_ctrl_char *) PVDataL)->units), sizeof(char[MAX_UNITS_SIZE]));

        upperDispLimit    = (CTYPE) ((struct dbr_ctrl_char *) PVDataL)->upper_disp_limit;
        lowerDispLimit    = (CTYPE) ((struct dbr_ctrl_char *) PVDataL)->lower_disp_limit;
        upperAlarmLimit   = (CTYPE) ((struct dbr_ctrl_char *) PVDataL)->upper_alarm_limit;
        upperWarningLimit = (CTYPE) ((struct dbr_ctrl_char *) PVDataL)->upper_warning_limit;
        lowerWarningLimit = (CTYPE) ((struct dbr_ctrl_char *) PVDataL)->lower_warning_limit;
        lowerAlarmLimit   = (CTYPE) ((struct dbr_ctrl_char *) PVDataL)->lower_alarm_limit;
        upperCtrlLimit    = (CTYPE) ((struct dbr_ctrl_char *) PVDataL)->upper_ctrl_limit;
        lowerCtrlLimit    = (CTYPE) ((struct dbr_ctrl_char *) PVDataL)->lower_ctrl_limit;

        break;

    case DBR_CTRL_ENUM:
        switch(_dataTypeClient){
        case DBR_ENUM:
            memcpy(val, &(&((PVDataL)->cenmval.value))[offset], sizeof(dbr_enum_t)*nelem);
            //for (unsigned int  i=0; i<nelem; ++i) std::cout << val[i] << " /[" << i << "]/ "; 	cout << endl;
            break;
        case DBR_DOUBLE:
        case DBR_SHORT:
        case DBR_CHAR:
        case DBR_LONG:
        case DBR_FLOAT:
            for (unsigned int  i=0; i<nelem; ++i) {
                    val[i] =  (CTYPE) (*(&((PVDataL)->cenmval.value)+i));
            }
            break;
            // strings are handled by a specialized template
        case DBR_STRING:
        default:
            break;
        }

        precision     = 0;  // struct dbr_ctrl_enum does not have the precision member
        RISC_pad      = 0;  // struct dbr_ctrl_char does not have the RISC_pad member
        alarmStatus   = ((struct dbr_ctrl_enum *) PVDataL)->status;
        alarmSeverity = ((struct dbr_ctrl_enum *) PVDataL)->severity;
        noStr         = ((struct dbr_ctrl_enum *) PVDataL)->no_str;

        memcpy(strs , &(((struct dbr_ctrl_enum *) PVDataL)->strs),
                sizeof(char)*MAX_ENUM_STRING_SIZE*MAX_ENUM_STATES);

        //for (unsigned int  i=0; i<noStr; ++i)	cout << i << " " << strs[i] << endl;

        //no units
        memcpy(units,"",sizeof(char[MAX_UNITS_SIZE]));

        upperDispLimit    = (CTYPE)  0;
        lowerDispLimit    = (CTYPE)  0;
        upperAlarmLimit   = (CTYPE)  0;
        upperWarningLimit = (CTYPE)  0;
        lowerWarningLimit = (CTYPE)  0;
        lowerAlarmLimit   = (CTYPE)  0;
        upperCtrlLimit    = (CTYPE)  0;
        lowerCtrlLimit    = (CTYPE)  0;

    break;

    case DBR_CTRL_FLOAT:
        switch(_dataTypeClient){
        case DBR_FLOAT:
            memcpy(val, &(&((PVDataL)->cfltval.value))[offset], sizeof(dbr_float_t)*nelem);

            break;
        case DBR_SHORT:
        case DBR_LONG:
        case DBR_ENUM:
        case DBR_CHAR:
        case DBR_DOUBLE:
            for (unsigned int  i=0; i<nelem; ++i) {
                val[i] =  (CTYPE) (*(&((PVDataL)->cfltval.value)+i+offset)) ;
            }
            break;
        // strings are handled by a specialized template
        case DBR_STRING:
            default:
            break;
        }

        precision     = ((struct dbr_ctrl_float *) PVDataL)->precision;
        RISC_pad      = ((struct dbr_ctrl_float *) PVDataL)->RISC_pad;

        //std::cout << " RISC_PAD " << ((struct dbr_ctrl_float *) PVDataL)->RISC_pad << std::endl;
        //std::cout << " RISC_PAD " << (short) ((struct dbr_ctrl_float *) PVDataL)->RISC_pad << std::endl;

        alarmStatus   = ((struct dbr_ctrl_float *) PVDataL)->status;
        alarmSeverity = ((struct dbr_ctrl_float *) PVDataL)->severity;

        memcpy(units, &(((struct dbr_ctrl_float *) PVDataL)->units), sizeof(char[MAX_UNITS_SIZE]));

        upperDispLimit    = (CTYPE)  ((struct dbr_ctrl_float *) PVDataL)->upper_disp_limit;
        lowerDispLimit    = (CTYPE)  ((struct dbr_ctrl_float *) PVDataL)->lower_disp_limit;
        upperAlarmLimit   = (CTYPE)  ((struct dbr_ctrl_float *) PVDataL)->upper_alarm_limit;
        upperWarningLimit = (CTYPE)  ((struct dbr_ctrl_float *) PVDataL)->upper_warning_limit;
        lowerWarningLimit = (CTYPE)  ((struct dbr_ctrl_float *) PVDataL)->lower_warning_limit;
        lowerAlarmLimit   = (CTYPE)  ((struct dbr_ctrl_float *) PVDataL)->lower_alarm_limit;
        upperCtrlLimit    = (CTYPE)  ((struct dbr_ctrl_float *) PVDataL)->upper_ctrl_limit;
        lowerCtrlLimit    = (CTYPE)  ((struct dbr_ctrl_float *) PVDataL)->lower_ctrl_limit;

        //cout << "upperCtrlLimit= " << upperCtrlLimit << " upperDispLimit=" << upperDispLimit << endl;
        // cout << "value " << val[0] << endl;

        break;

    case DBR_CTRL_DOUBLE:
        switch(_dataTypeClient){
        case DBR_DOUBLE:
            memcpy(val, &(&((PVDataL)->cdblval.value))[offset], sizeof(dbr_double_t)*nelem);

            break;
        case DBR_SHORT:
        case DBR_LONG:
        case DBR_ENUM:
        case DBR_CHAR:
        case DBR_FLOAT:
            for (unsigned int  i=0; i<nelem; ++i) {
                val[i] =  (CTYPE) (*(&((PVDataL)->cdblval.value)+i+offset));
            }
            break;
        // strings are handled by a specialized template
        case DBR_STRING:
            default:
            break;
        }

        precision = ((struct dbr_ctrl_double *) PVDataL)->precision;
        RISC_pad      = (short) ((struct dbr_ctrl_double *) PVDataL)->RISC_pad0; //short
        alarmStatus   = ((struct dbr_ctrl_double *) PVDataL)->status;
        alarmSeverity = ((struct dbr_ctrl_double *) PVDataL)->severity;

        memcpy(units, &(((struct dbr_ctrl_double *) PVDataL)->units), sizeof(char[MAX_UNITS_SIZE]));

        upperDispLimit    = (CTYPE)  ((struct dbr_ctrl_double *) PVDataL)->upper_disp_limit;
        lowerDispLimit    = (CTYPE)  ((struct dbr_ctrl_double *) PVDataL)->lower_disp_limit;
        upperAlarmLimit   = (CTYPE)  ((struct dbr_ctrl_double *) PVDataL)->upper_alarm_limit;
        upperWarningLimit = (CTYPE)  ((struct dbr_ctrl_double *) PVDataL)->upper_warning_limit;
        lowerWarningLimit = (CTYPE)  ((struct dbr_ctrl_double *) PVDataL)->lower_warning_limit;
        lowerAlarmLimit   = (CTYPE)  ((struct dbr_ctrl_double *) PVDataL)->lower_alarm_limit;
        upperCtrlLimit    = (CTYPE)  ((struct dbr_ctrl_double *) PVDataL)->upper_ctrl_limit;
        lowerCtrlLimit    = (CTYPE)  ((struct dbr_ctrl_double *) PVDataL)->lower_ctrl_limit;

        break;

    case DBR_CTRL_SHORT:
        switch(_dataTypeClient){
        case DBR_SHORT:
            memcpy(val, &(&((PVDataL)->cshrtval.value))[offset], sizeof(dbr_short_t)*nelem);

            break;
        case DBR_DOUBLE:
        case DBR_LONG:
        case DBR_ENUM:
        case DBR_CHAR:
        case DBR_FLOAT:
            for (unsigned int  i=0; i<nelem; ++i) {
                val[i] =  (CTYPE) (*(&((PVDataL)->cshrtval.value)+i+offset));
            }
            break;
        // strings are handled by a specialized template
        case DBR_STRING:
            default:
            break;
        }


       precision     = 0;  // struct dbr_ctrl_shrt does not have the precision member
       RISC_pad      = 0;  // struct dbr_ctrl_shrt does not have the RISC_pad member
       alarmStatus   = ((struct dbr_ctrl_short *) PVDataL)->status;
       alarmSeverity = ((struct dbr_ctrl_short *) PVDataL)->severity;

       memcpy(units, &(((struct dbr_ctrl_short *) PVDataL)->units), sizeof(char[MAX_UNITS_SIZE]));

       upperDispLimit    = (CTYPE)  ((struct dbr_ctrl_short *) PVDataL)->upper_disp_limit;
       lowerDispLimit    = (CTYPE)  ((struct dbr_ctrl_short *) PVDataL)->lower_disp_limit;
       upperAlarmLimit   = (CTYPE)  ((struct dbr_ctrl_short *) PVDataL)->upper_alarm_limit;
       upperWarningLimit = (CTYPE)  ((struct dbr_ctrl_short *) PVDataL)->upper_warning_limit;
       lowerWarningLimit = (CTYPE)  ((struct dbr_ctrl_short *) PVDataL)->lower_warning_limit;
       lowerAlarmLimit   = (CTYPE)  ((struct dbr_ctrl_short *) PVDataL)->lower_alarm_limit;
       upperCtrlLimit    = (CTYPE)  ((struct dbr_ctrl_short *) PVDataL)->upper_ctrl_limit;
       lowerCtrlLimit    = (CTYPE)  ((struct dbr_ctrl_short *) PVDataL)->lower_ctrl_limit;

       break;


    case DBR_CTRL_LONG:
        switch(_dataTypeClient){
        case DBR_LONG:
            memcpy(val, &(&((PVDataL)->clngval.value))[offset], sizeof(dbr_long_t)*nelem);
            break;
        case DBR_DOUBLE:
        case DBR_SHORT:
        case DBR_ENUM:
        case DBR_CHAR:
        case DBR_FLOAT:
            for (unsigned int  i=0; i<nelem; ++i) {
                val[i] =  (CTYPE) (*(&((PVDataL)->clngval.value)+i+offset));
            }
            break;
        // strings are handled by a specialized template
        case DBR_STRING:
            default:
            break;
        }

        precision     = 0;  // struct dbr_ctrl_long does not have the precision member
        RISC_pad      = 0;  // struct dbr_ctrl_long does not have the RISC_pad member

        alarmStatus   = ((struct dbr_ctrl_long *) PVDataL)->status;
        alarmSeverity = ((struct dbr_ctrl_long *) PVDataL)->severity;

        memcpy(units, &(((struct dbr_ctrl_long *) PVDataL)->units),sizeof(char[MAX_UNITS_SIZE])) ;

        upperDispLimit    = (CTYPE)  ((struct dbr_ctrl_long *) PVDataL)->upper_disp_limit;
        lowerDispLimit    = (CTYPE)  ((struct dbr_ctrl_long *) PVDataL)->lower_disp_limit;
        upperAlarmLimit   = (CTYPE)  ((struct dbr_ctrl_long *) PVDataL)->upper_alarm_limit;
        upperWarningLimit = (CTYPE)  ((struct dbr_ctrl_long *) PVDataL)->upper_warning_limit;
        lowerWarningLimit = (CTYPE)  ((struct dbr_ctrl_long *) PVDataL)->lower_warning_limit;
        lowerAlarmLimit   = (CTYPE)  ((struct dbr_ctrl_long *) PVDataL)->lower_alarm_limit;
        upperCtrlLimit    = (CTYPE)  ((struct dbr_ctrl_long *) PVDataL)->upper_ctrl_limit;
        lowerCtrlLimit    = (CTYPE)  ((struct dbr_ctrl_long *) PVDataL)->lower_ctrl_limit;

        break;
    }

    }
    else {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }

return ICAFE_NORMAL;
#undef __METHOD__
}


#endif // TRANSPOSE_H






