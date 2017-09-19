///
/// \file    PVHolder.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef PVHOLDER_H
#define PVHOLDER_H

#include <stdexcept>
#include <cstring>
#include <limits>
#include <cafeConvert.h>
#include <statusCodes.h>
#include <helper.h>
#include <defines.h>
#include <cafeDataTypeHelper.h>
#include <boost/smart_ptr/shared_ptr.hpp>
//include <boost/smart_ptr/make_unique.hpp> boost 1.57
//include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <vector>


/**
* \class PVHolder
* \brief The base class from which the PVDataHolder and PVCtrlHolder
* classes are derived
*/
class PVHolder {

protected:
    char  pv     [PVNAME_SIZE];
    char  pvAlias[PVNAME_SIZE];
    char  device [PVNAME_SIZE];
    char  attrib [PVNAME_SIZE];

    CAFE_DATATYPE       dataTypeNative; //enum
    CAFE_DATATYPE       dataType; //enum
    CAFEDataTypeCode    cafeDataTypeCode;   //class enum<->string mapping
    CAFEStatusCode      cafeStatusCode;

    chtype dbrDataType; //dbrTypeRequest_DataBuffer;

    unsigned int   size;
    unsigned int   nelem;
    //unsigned int   nelemNative;
    short alarmStatus;   //alarm.h 0-22 0=NO_ALARM
    short alarmSeverity; //alarm.h 0=NO_ALARM 1=MINOR 2=MAJOR 3=INVALID

    unsigned int  userNo; //e.g. add handle

    unsigned int  beamEventNo;
    //rule is used for synchronous groups only
    bool  rule;  // to set/get or not to set/get channel; default: true (i.e. set)
	  bool  hasAlarm;
    int   status;

    short noStr; // for enum
    char  strs [MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE];

    CAFEConvert<double> renderDouble;
    CAFEConvert<float>  renderFloat;
    CAFEConvert<short>  renderShort;
    CAFEConvert<int>    renderLong;
    CAFEConvert<unsigned short> renderEnum;
    CAFEConvert<unsigned char>  renderUChar;
    CAFEConvert<dbr_string_t>   renderString;
	
    CAFEConvert<char>           renderChar;
    CAFEConvert<unsigned int>   renderULong;
    CAFEConvert<long  long>     renderLongLong;
    CAFEConvert<unsigned long  long> renderULongLong;
	
    CAFEConvert<int>          renderInt;
    CAFEConvert<unsigned int> renderUInt;


    void verifyIndex(unsigned int  idx) {
        if(idx >= size) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
    };

    bool isIndexOutOfRange (unsigned int  idx) {
        return (idx >= size) ? true:false;
    };


public:

   typedef boost::shared_ptr<CAFE_DATATYPE_UNION []> ValPtr;

   ValPtr val;

   boost::shared_ptr<vector<double> > ValVD_ptr;
   boost::shared_ptr<vector<float> > ValVF_ptr;
   //boost::shared_ptr<vector<short> > ValVS_ptr;
   boost::shared_ptr<vector<int> > ValVI_ptr;
   //boost::shared_ptr<vector<unsigned char> > ValVC_ptr;
   //boost::shared_ptr<vector<unsigned short> > ValVUS_ptr;
   boost::shared_ptr<vector<string> > ValVStr_ptr;

   typedef boost::shared_ptr<double []> ValDPtr;
   typedef boost::shared_ptr<float []> ValFPtr;
   typedef boost::shared_ptr<short []> ValSPtr;
   typedef boost::shared_ptr<int []> ValIPtr;
   typedef boost::shared_ptr<unsigned char []> ValChPtr;
   typedef boost::shared_ptr<unsigned short []> ValUSPtr;
   typedef boost::shared_ptr<dbr_string_t []> ValStrPtr;

   ValDPtr ValD_ptr;
   ValFPtr ValF_ptr;
   ValSPtr ValS_ptr;
   ValIPtr ValI_ptr;
   ValChPtr ValCh_ptr;
   ValUSPtr ValUS_ptr;
   ValStrPtr ValStr_ptr;

   void setUserNo(unsigned int un) {
       userNo=un;
       return;
   }

   unsigned int getUserNo() {
       return userNo;
   }


    void setRule(bool r) {         
        rule=r;
        return;
    };



    const char  * getPV() const {return pv;};
    const char  * getPVName() const {return pv;};
    const char  * getPVAlias() const {return pvAlias;};
    const char  * getDevice() const {return device;};
    const char  * getAttribute() const {return attrib;};
    unsigned int  getNelem() const {return nelem;};
    //unsigned int  getNelemNative() const {return nelemNative;};
    const unsigned int  getSize() const {return size;};
    CAFE_DATATYPE_UNION_SEQ getVal() const {return val.get();};


    short getAlarmStatus() const {return alarmStatus;};
    short getAlarmSeverity() const {return alarmSeverity;};

    unsigned int  getBeamEventNo() const {return beamEventNo;};
    bool getRule() const {return rule;};
	bool getHasAlarm() const{return hasAlarm;};
    int  getStatus() const {return status;};
    CAFE_DATATYPE getDataTypeClient() const {return dataType;};
    CAFE_DATATYPE getDataType() const {return dataType;};

    CAFEStatusCode getStatusCode() const {return cafeStatusCode;};

    short  getNoEnumStrings () const {return noStr;};
    char * getEnumString(short indx) const {return (char *) strs[indx]; }

    void setDataType(CAFE_DATATYPE cdt){
        if (cdt > CAFE_DOUBLE || cdt < CAFE_STRING) {
            cout << "WARNING: INPUT VALUE NOT A VALID CAFE DATATYPE " << endl;
            return;
        } else {dataType=cdt; return;}};

    void set(double d) {val[0].d=d; dataType=CAFE_DOUBLE;};
    void set(float  f) {val[0].f=f; dataType=CAFE_FLOAT;};
    void set(short  s) {val[0].s=s; dataType=CAFE_SHORT;};
    void set(long long l)  {
        if (l > std::numeric_limits<dbr_long_t>::max()) {
            cout << "WARNING: INPUT VALUE GREATER THAN MAX LIMIT OF dbr_long_t " << endl;
            cout << "TYPE CASTING TO DOUBLE! " << endl;
            val[0].d= (double) l; dataType=CAFE_DOUBLE;}
        else {
            val[0].l= (int) l; dataType=CAFE_LONG;};
    }
    void set(int    l) {val[0].l=l; dataType=CAFE_LONG;};
    void set(unsigned long long  l)  {
        if (l > (unsigned long long) std::numeric_limits<dbr_long_t>::max()) {
            cout << "WARNING: INPUT VALUE GREATER THAN MAX LIMIT OF dbr_long_t " << endl;
            cout << "TYPE CASTING TO DOUBLE! " << endl;
            val[0].d= (double) l; dataType=CAFE_DOUBLE;}
        else {
            val[0].l= (int) l; dataType=CAFE_LONG;};
    }

    //For Cython
    void setString(std::string     str) {strcpy(val[0].str,str.c_str()); dataType=CAFE_STRING;};
    void setDouble(double d) {val[0].d=d; dataType=CAFE_DOUBLE;};
    void setInt(int    l) {val[0].l=l; dataType=CAFE_LONG;};
	  void setVString(vector<std::string>  Vstr) {
		   if(Vstr.size()!=nelem) {nelem=Vstr.size();}
		   for (unsigned int  i=0; i<nelem; ++ i) {
			 strcpy(val[i].str,Vstr[i].c_str());} dataType=CAFE_STRING;};
	  void setVDouble(vector<double> Vd) {
		  if(Vd.size()!=nelem) {nelem=Vd.size();}
		  for (unsigned int  i=0; i<nelem; ++ i) {
			val[i].d=Vd[i];} dataType=CAFE_DOUBLE;};
	  void setVInt   (vector<int>    Vl) {
		  if(Vl.size()!=nelem) {nelem=Vl.size();}
		  for (unsigned int  i=0; i<nelem; ++ i) {
			val[i].l=Vl[i];} dataType=CAFE_LONG;};

    void set(unsigned int    l)  {val[0].l= (int) l; dataType=CAFE_LONG;};
    void set(unsigned short  us) {val[0].us=us; dataType=CAFE_ENUM;};
    void set(unsigned char   ch) {val[0].ch=ch; dataType=CAFE_CHAR;};
    void set(dbr_string_t    str) {strcpy(val[0].str,str); dataType=CAFE_STRING;};
    void set(std::string     str) {strcpy(val[0].str,str.c_str()); dataType=CAFE_STRING;};

	
    void set(double * d) {for (unsigned int  i=0; i<nelem; ++ i) {val[i].d=d[i];} dataType=CAFE_DOUBLE;};
    void set(float  * f) {for (unsigned int  i=0; i<nelem; ++ i) {val[i].f=f[i];} dataType=CAFE_FLOAT;};
    void set(short  * s) {for (unsigned int  i=0; i<nelem; ++ i) {val[i].s=s[i];} dataType=CAFE_SHORT;};
    //Examine this!
    void set(long long * l) {
        for (unsigned int  i=0; i<nelem; ++ i) {val[i].l=(int)l[i];} dataType=CAFE_LONG;
    };

    void set(int    * l) {for (unsigned int  i=0; i<nelem; ++ i) {val[i].l=l[i];} dataType=CAFE_LONG;};
    void set(unsigned int   * l) {for (unsigned int  i=0; i<nelem; ++ i)
        {val[i].l= (int) l[i];} dataType=CAFE_LONG;};
    void set(unsigned short * us) {for (unsigned int  i=0; i<nelem; ++ i)
        {val[i].us=us[i];} dataType=CAFE_ENUM;};
    void set(unsigned char  * ch) {for (unsigned int  i=0; i<nelem; ++ i)
        {val[i].ch=ch[i];} dataType=CAFE_CHAR;};
    void set(dbr_string_t   * str) {for (unsigned int  i=0; i<nelem; ++ i)
        {strcpy(val[i].str,str[i]);} dataType=CAFE_STRING;};
    void set(std::string    * str) {for (unsigned int  i=0; i<nelem; ++ i)
        {strcpy(val[i].str,str[i].c_str());} dataType=CAFE_STRING;};

    std::string concatToString(){
        std::string psWF = "";     
        if (dataTypeNative==CAFE_CHAR) {
            for (unsigned int i=0; i<nelem; ++i) {
                if (val[i].ch != '\0') {
                    psWF.append(1, (dbr_char_t) val[i].ch);                
                }            
            }
        }
        return psWF;
    }
		
    std::string getWFAsString(){
        std::string psWF = "";     
        if (dataTypeNative==CAFE_CHAR) {
            for (unsigned int i=0; i<nelem; ++i) {
                if (val[i].ch != '\0') {
                    psWF.append(1, (dbr_char_t) val[i].ch);                
                }            
            }
        }
        return psWF;
    }
		

    double   getDouble(unsigned int  idx) {return  (double)  val[idx].d;}
    float    getFloat (unsigned int  idx) {return  (float)   val[idx].f;}
    short    getShort (unsigned int  idx) {return  (short)   val[idx].s;}
    int      getInt   (unsigned int  idx) {return  (int)    val[idx].l;}
    int      getLong  (unsigned int  idx) {return  (int)    val[idx].l;}
    unsigned short  getEnum  (unsigned int  idx)   {return  (unsigned short) val[idx].us;}
    unsigned short  getUShort(unsigned int  idx)   {return  (unsigned short) val[idx].us;}
    unsigned char   getChar  (unsigned int  idx)   {return  (unsigned char ) val[idx].ch;}



    dbr_string_t  * getString(unsigned int  idx) {
    #define __METHOD__  "PVHolder::getString "
        if (dataType!=CAFE_STRING) {
            cout << "******* WARNING *******" << endl;
            cout << __METHOD__ << __LINE__ << endl;
            cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                    " hence getString method is invalid! " << endl;
            cout << "Use getAsString method if you wish to retrieve the data as a string! " << endl;
            cout << "**********************" << endl;
            //strcpy(val[idx].str, "");
            return (dbr_string_t *) "";
        }
        return  (dbr_string_t *) val[idx].str;
    #undef __METHOD__
    }

    boost::shared_ptr<vector<double> > getAsVDouble(){
    #define __METHOD__  "PVHolder::getVDouble "

        ValVD_ptr.reset(new vector<double>());

        switch (dataType)
        {
            case CAFE_DOUBLE:
                for (unsigned i=0; i<nelem; ++i) {ValVD_ptr->push_back(val[i].d);}
                break;
            case CAFE_FLOAT:
                for (unsigned i=0; i<nelem; ++i) {ValVD_ptr->push_back(val[i].f);}
                break;
            case CAFE_LONG:
                for (unsigned i=0; i<nelem; ++i) {ValVD_ptr->push_back(val[i].l);}
                break;
            case CAFE_SHORT:
                for (unsigned i=0; i<nelem; ++i) {ValVD_ptr->push_back(val[i].s);}
                break;
            case CAFE_ENUM:
                for (unsigned i=0; i<nelem; ++i) {ValVD_ptr->push_back(val[i].us);}
                break;
            case CAFE_CHAR:
                for (unsigned i=0; i<nelem; ++i) {ValVD_ptr->push_back(val[i].ch);}
                break;
            case CAFE_STRING:
                default:
                for (unsigned i=0; i<nelem; ++i) {ValVD_ptr->push_back( getAsDouble(i));}
                break;
        }

    return  ValVD_ptr;
    #undef __METHOD__
    }

    boost::shared_ptr<vector<float> > getAsVFloat(){
    #define __METHOD__  "PVHolder::getVFloat "

        ValVF_ptr.reset(new vector<float>());

        switch (dataType)
        {
            case CAFE_DOUBLE:
                for (unsigned i=0; i<nelem; ++i) {ValVF_ptr->push_back((float) val[i].d);}
                break;
            case CAFE_FLOAT:
                for (unsigned i=0; i<nelem; ++i) {ValVF_ptr->push_back(val[i].f);}
                break;
            case CAFE_LONG:
                for (unsigned i=0; i<nelem; ++i) {ValVF_ptr->push_back(val[i].l);}
                break;
            case CAFE_SHORT:
                for (unsigned i=0; i<nelem; ++i) {ValVF_ptr->push_back(val[i].s);}
                break;
            case CAFE_ENUM:
                for (unsigned i=0; i<nelem; ++i) {ValVF_ptr->push_back(val[i].us);}
                break;
            case CAFE_CHAR:
                for (unsigned i=0; i<nelem; ++i) {ValVF_ptr->push_back(val[i].ch);}
                break;
            case CAFE_STRING:
                default:
                for (unsigned i=0; i<nelem; ++i) {ValVD_ptr->push_back( getAsFloat(i));}
                break;
        }

        //How to index shared pointer for <vector<float> >
        //vector<float> * vf= ValVF_ptr.get();
        //cout << "size/// " << vf[0].size() << endl;
        //cout << vf[0][0] << " val " << val[0].f << endl;
        //cout << vf[0][1] << " val " << val[1].f << endl;

    return  ValVF_ptr;
    #undef __METHOD__
    }


    boost::shared_ptr<vector<int> > getAsVInt(){
    #define __METHOD__  "PVHolder::getVInt "

        ValVI_ptr.reset(new vector<int>());

        switch (dataType)
        {
            case CAFE_DOUBLE:
                for (unsigned i=0; i<nelem; ++i) {ValVI_ptr->push_back((int) val[i].d);}
                break;
            case CAFE_FLOAT:
                for (unsigned i=0; i<nelem; ++i) {ValVI_ptr->push_back((int) val[i].f);}
                break;
            case CAFE_LONG:
                for (unsigned i=0; i<nelem; ++i) {ValVI_ptr->push_back(val[i].l);}
                break;
            case CAFE_SHORT:
                for (unsigned i=0; i<nelem; ++i) {ValVI_ptr->push_back(val[i].s);}
                break;
            case CAFE_ENUM:
                for (unsigned i=0; i<nelem; ++i) {ValVI_ptr->push_back(val[i].us);}
                break;
            case CAFE_CHAR:
                for (unsigned i=0; i<nelem; ++i) {ValVI_ptr->push_back(val[i].ch);}
                break;
            case CAFE_STRING:
                default:
                for (unsigned i=0; i<nelem; ++i) {ValVI_ptr->push_back( getAsInt(i));}
                break;
        }

    return  ValVI_ptr;
    #undef __METHOD__
    }

    boost::shared_ptr<vector<string> > getAsVString(){
    #define __METHOD__  "PVHolder::getVString "

        ValVStr_ptr.reset(new vector<string>());
        for (unsigned i=0; i<nelem; ++i) {ValVStr_ptr->push_back( getAsString(i));}

    return  ValVStr_ptr;
    #undef __METHOD__
    }

    ValDPtr getDouble(){
    #define __METHOD__  "PVHolder::getDouble "
        if (dataType!=CAFE_DOUBLE) {
           cout << "******* WARNING *******" << endl;
           cout <<  __METHOD__ << __LINE__  << endl;
           cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                   " hence getDouble method is invalid! " << endl;
           cout << "Use getAsDouble method if you wish to retrieve the data as a double! " << endl;
           cout << "**********************" << endl;         
        }

        ValD_ptr.reset(new double[nelem]);

        for (unsigned i=0; i<nelem; ++i) {
            ValD_ptr[i] = val[i].d;
        }

        return  ValD_ptr;
    #undef __METHOD__
    }

    ValFPtr getFloat() {
    #define __METHOD__  "PVHolder::getFloat "
        if (dataType!=CAFE_FLOAT) {
           cout << "******* WARNING *******" << endl;
           cout <<  __METHOD__ << __LINE__  << endl;
           cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                   " hence getFloat method is invalid! " << endl;
           cout << "Use getAsFloat method if you wish to retrieve the data as a float! " << endl;
           cout << "**********************" << endl;
        }
        ValF_ptr.reset(new float[nelem]);
            for (unsigned i=0; i<nelem; ++i) {
                ValF_ptr[i] = val[i].f;
            }
        return  ValF_ptr;
        //return (float *) val.get();
    #undef __METHOD__
    }

    ValSPtr getShort()          {
    #define __METHOD__  "PVHolder::getShort "
        if (dataType==CAFE_SHORT) {
            cout << "******* WARNING *******" << endl;
            cout <<  __METHOD__ << __LINE__  << endl;
            cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
               " hence getShort method is invalid! " << endl;
            cout << "Use getAsShort method if you wish to retrieve the data as a short! " << endl;
            cout << "**********************" << endl;
        }
        ValS_ptr.reset(new short[nelem]);
            for (unsigned i=0; i<nelem; ++i) {
                ValS_ptr[i] = val[i].s;
            }
        return  ValS_ptr;

    #undef __METHOD__
    }

    ValIPtr getInt(){
    #define __METHOD__  "PVHolder::getInt (meaning dbr_long_t)  "
        if (dataType!=CAFE_LONG) {
            cout << "******* WARNING *******" << endl;
            cout <<  __METHOD__ << __LINE__  << endl;
            cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                   " hence getInt  method is invalid! " << endl;
            cout << "Use getAsInt  method if you wish to retrieve the data as a int! " << endl;
            cout << "**********************" << endl;
        }
        ValI_ptr.reset(new int[nelem]);
            for (unsigned i=0; i<nelem; ++i) {
                ValI_ptr[i] = val[i].l;
            }
        return  ValI_ptr;
    #undef __METHOD__
    }

    ValIPtr getLong(){
    #define __METHOD__  "PVHolder::getLong (meaning dbr_long_t)  "
        if (dataType!=CAFE_LONG) {
            cout << "******* WARNING *******" << endl;
            cout <<  __METHOD__ << __LINE__  << endl;
            cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                   " hence getLong  method is invalid! " << endl;
            cout << "Use getAsLong  method if you wish to retrieve the data as a dbr_long_t! " << endl;
            cout << "**********************" << endl;
        }
        ValI_ptr.reset(new int[nelem]);
            for (unsigned i=0; i<nelem; ++i) {
                ValI_ptr[i] = val[i].l;
            }
        return  ValI_ptr;
    #undef __METHOD__
    }

    ValUSPtr getEnum(){
    #define __METHOD__  "PVHolder::getEnum "
        if (dataType!=CAFE_ENUM) {
            cout << "******* WARNING *******" << endl;
            cout <<  __METHOD__ << __LINE__  << endl;
            cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                   " hence getEnum method is invalid! " << endl;
            cout << "Use getAsEnum method if you wish to retrieve the data as an enum (unsigned short)! " << endl;
            cout << "**********************" << endl;
        }
        ValUS_ptr.reset(new unsigned short[nelem]);
            for (unsigned i=0; i<nelem; ++i) {
                ValUS_ptr[i] = val[i].us;
            }
        return  ValUS_ptr;
    #undef __METHOD__
    }

    ValUSPtr getUShort(){
    #define __METHOD__  "PVHolder::getUShort "
        if (dataType!=CAFE_USHORT) {
            cout << "******* WARNING *******" << endl;
            cout <<  __METHOD__ << __LINE__  << endl;
            cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                   " hence getUShort method is invalid! " << endl;
            cout << "Use getAsUSHort method if you wish to retrieve the data as an unsigned short! " << endl;
            cout << "**********************" << endl;
        }
        ValUS_ptr.reset(new unsigned short[nelem]);
            for (unsigned i=0; i<nelem; ++i) {
                ValUS_ptr[i] = val[i].us;
            }
        return  ValUS_ptr;
    #undef __METHOD__
    }

    ValChPtr getChar(){
    #define __METHOD__  "PVHolder::getChar "
        if (dataType!=CAFE_CHAR) {
            cout << "******* WARNING *******" << endl;
            cout <<  __METHOD__ << __LINE__  << endl;
            cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                   " hence getChar method is invalid! " << endl;
            cout << "Use getAsChar method if you wish to retrieve the data as a char! " << endl;
            cout << "**********************" << endl;
        }
        ValCh_ptr.reset(new unsigned char[nelem]);
            for (unsigned i=0; i<nelem; ++i) {
                ValCh_ptr[i] = val[i].ch;
            }
        return  ValCh_ptr;
    #undef __METHOD__
    }

    ValStrPtr getString() {
    #define __METHOD__  "PVHolder::getString "
        if (dataType!=CAFE_STRING) {
            cout << "******* WARNING *******" << endl;
            cout << __METHOD__ << __LINE__ << endl;
            cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                    " hence getString method is invalid! " << endl;
            cout << "Use getAsString method if you wish to retrieve the data as a string! " << endl;
            cout << "**********************" << endl;
            //return (dbr_string_t *) "";
        }
        ValStr_ptr.reset(new dbr_string_t[nelem]);
            for (unsigned i=0; i<nelem; ++i) {
                strcpy(ValStr_ptr[i] , val[i].str);
            }
        return  ValStr_ptr;
    #undef __METHOD__
    }	

    double getAsDouble () {
        return (double)  renderDouble.get(0, dataType, val.get())[0];
    }
	
    double getAsDouble(unsigned int  idx) throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (double)  renderDouble.get(idx, dataType, val.get())[0];
    }
	
    float getAsFloat () {
        return (float)  renderFloat.get(0, dataType, val.get())[0];
    }
		
    float getAsFloat(unsigned int  idx) throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());           
        }
        return (float) renderFloat.get(idx, dataType, val.get())[0];
    }
   
    short getAsShort () {
        return (short)  renderShort.get(0, dataType, val.get())[0];
    }

    short getAsShort(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (short) renderShort.get(idx, dataType, val.get())[0];
    }

    int  getAsLong  () {
        return (int)  renderLong.get(0, dataType, val.get())[0];
    }
    
    int  getAsLong(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (int) renderLong.get(idx, dataType, val.get())[0];
    }
	
    unsigned short getAsEnum () {
        return (unsigned short)  renderEnum.get(0, dataType, val.get())[0];
    }
	
    unsigned short  getAsEnum(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (unsigned short) renderEnum.get(idx, dataType, val.get())[0];
    }

    unsigned short getAsUShort () {
        return (unsigned short)  renderEnum.get(0, dataType, val.get())[0];
    }
	
    unsigned short getAsUShort(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (unsigned short) renderEnum.get(idx, dataType, val.get())[0];
    }
     
    unsigned char getAsChar () {
        return (char) renderChar.get(0, dataType, val.get())[0];
    }
		
    unsigned char getAsChar(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (char) renderChar.get(idx, dataType, val.get())[0];
    }
  	
    unsigned char getAsUChar () {
        return (unsigned char) renderUChar.get(0, dataType, val.get())[0];
    }
		
    unsigned char getAsUChar(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (unsigned char) renderUChar.get(idx, dataType, val.get())[0];
    }
  	
    unsigned int  getAsULong  () {
        return (unsigned int)  renderULong.get(0, dataType, val.get())[0];
    }
    
    unsigned int  getAsULong(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (unsigned int) renderULong.get(idx, dataType, val.get())[0];
    }
	
    long long  getAsLongLong  () {
        return (long  long)  renderLongLong.get(0, dataType, val.get())[0];
    }
    
    long long  getAsLongLong(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (long  long) renderLongLong.get(idx, dataType, val.get())[0];
    }
	
    unsigned long long  getAsULongLong  () {
        return (unsigned long  long)  renderULongLong.get(0, dataType, val.get())[0];
    }
    
    unsigned long  long  getAsULongLong(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (unsigned long  long) renderULongLong.get(idx, dataType, val.get())[0];
    }
    
    int getAsInt () {
        return (int)  renderInt.get(0, dataType, val.get())[0];
    }
    
    int getAsInt(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (int) renderInt.get(idx, dataType, val.get())[0];
    }
      
    unsigned int getAsUInt () {
        return (unsigned int)  renderInt.get(0, dataType, val.get())[0];
    }
    
    unsigned int getAsUInt(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (unsigned int) renderInt.get(idx, dataType, val.get())[0];
    }
    
   //getEnumAsNumberedString
    string getEnumIntegerValueAsString() {
    #define __METHOD__  "PVHolder::getEnumIntegerValueAsString"
        if (dataType!=CAFE_ENUM) {
            cout << "******* WARNING *******" << endl;
            cout << __METHOD__ << __LINE__ << endl;
            cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                    " hence getEnumIntegerValueAsString method is invalid! " << endl;
            cout << "Use getAsString method if you want the string equilavent of the ENUM value!" << endl;
            cout << "**********************" << endl;
        }
    return (string) renderString.getString(0, dataType, val.get())[0];
    #undef __METHOD__
    }

    string getAsString()  {
        if(dataTypeNative==DBR_ENUM && dataType==DBR_ENUM) {
				
            return  (string) renderString.getStringFromEnum(0,  noStr, val.get(), strs)[0];
        }
        else {
            return (string) renderString.getString(0, dataType, val.get())[0];
        }
    }
    
    string getAsString(unsigned int  idx) throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        if(dataTypeNative==DBR_ENUM && dataType==CAFE_ENUM) {
            return  (string) renderString.getStringFromEnum(idx,  noStr, val.get(), strs)[0];
        }
        else {
            return (string) renderString.getString(idx, dataType, val.get())[0];
        }
    }
    

    char * getAsDbr_string_t() { 
        if(dataTypeNative==DBR_ENUM && dataType==CAFE_ENUM) {          
            return (char *) renderString.getStringFromEnum(0,  noStr, val.get(), strs)[0];

        }
        else {
            return (char *) renderString.getString(0, dataType, val.get())[0];
        }
    }
    

    char *  getAsDbr_string_t(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        if(dataTypeNative==DBR_ENUM && dataType==CAFE_ENUM) {
            return  (char *) renderString.getStringFromEnum(idx, noStr, val.get(), strs)[0];
        }
        else {
            return (char *) renderString.getString(idx, dataType, val.get())[0];
        }
    }

    /*
    dbr_string_t *  getAsDbr_string_t(unsigned int  idx)  throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                    << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        if(dataTypeNative==DBR_ENUM && dataType==CAFE_ENUM) {
            return  (dbr_string_t *) renderString.getStringFromEnum(idx,  val.get(), strs);
        }
        else {
            return (dbr_string_t *) renderString.getString(idx, dataType, val.get());
        }
    }
    */

    //used by PVCtrlHolder
    double  getAsDouble(CAFE_DATATYPE_UNION  cdu) {
        return (double) renderDouble.get(0, dataType, &cdu)[0];
    }

    float   getAsFloat(CAFE_DATATYPE_UNION cdu) {
        return (float) renderFloat.get(0, dataType, &cdu)[0];
    }

    short   getAsShort(CAFE_DATATYPE_UNION cdu) {
        return (short) renderShort.get(0, dataType, &cdu)[0];
    }

    int     getAsLong(CAFE_DATATYPE_UNION  cdu) {
        return (int) renderLong.get(0, dataType, &cdu)[0];
    }

    unsigned short  getAsEnum(CAFE_DATATYPE_UNION cdu) {
        return (unsigned short) renderEnum.get(0, dataType, &cdu)[0];
    }

    unsigned short  getAsUShort(CAFE_DATATYPE_UNION cdu) {
        return (unsigned short) renderEnum.get(0, dataType, &cdu)[0];
    }

    char   getAsChar(CAFE_DATATYPE_UNION cdu) {
        char  * ans = renderChar.get(0, dataType, &cdu);
        return (char ) ans[0];
    }
    unsigned char   getAsUChar(CAFE_DATATYPE_UNION cdu) {
        unsigned char  * ans = renderUChar.get(0, dataType, &cdu);
        return (unsigned char ) ans[0];
    }

    string getAsString(CAFE_DATATYPE_UNION cdu) {
        return (string) renderString.getString(dataType, cdu)[0];
    }

    char * getAsDbr_string_t(CAFE_DATATYPE_UNION cdu) {
        return (char *) renderString.getString(0, dataType, &cdu)[0];
    }
};


#endif //PVHOLDER_H
