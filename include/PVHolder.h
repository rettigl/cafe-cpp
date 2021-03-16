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
class PVHolder
{

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

    CAFEGlobalAlarmCondition acond;
    CAFEGlobalAlarmSeverity  aseve;

    unsigned int  userNo; //e.g. add handle

    unsigned long long  beamEventNo;
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


    void verifyIndex(unsigned int  idx)
    {
        if(idx >= size)
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
    };

    bool isIndexOutOfRange (unsigned int  idx)
    {
        return (idx >= size) ? true:false;
    };


public:

    typedef boost::shared_ptr<CAFE_DATATYPE_UNION []> ValPtr;

    ValPtr val;

    boost::shared_ptr<std::vector<double> > ValVD_ptr;
    boost::shared_ptr<std::vector<float> > ValVF_ptr;
    boost::shared_ptr<std::vector<short> > ValVS_ptr;
    boost::shared_ptr<std::vector<int> > ValVI_ptr;
    boost::shared_ptr<std::vector<long> > ValVL_ptr;
    boost::shared_ptr<std::vector<unsigned long> > ValVUL_ptr;
    boost::shared_ptr<std::vector<long long> > ValVLL_ptr;
    boost::shared_ptr<std::vector<unsigned long long> > ValVULL_ptr;
    boost::shared_ptr<std::vector<unsigned char> > ValVC_ptr;
    boost::shared_ptr<std::vector<unsigned short> > ValVUS_ptr;
    boost::shared_ptr<std::vector<std::string> > ValVStr_ptr;

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

    void setUserNo(unsigned int un)
    {
        userNo=un;
        return;
    }

    unsigned int getUserNo()
    {
        return userNo;
    }


    void setRule(bool r)
    {
        rule=r;
        return;
    };

    void setBeamEventNo  (unsigned long long b)
    {
        beamEventNo=b;
    }
    void setPulseID      (unsigned long long b)
    {
        beamEventNo=b;
    }
    void setStatus       (int st)
    {
        status=st;   //for bsread
    }
    void setAlarmStatus  (int ast)
    {
        alarmStatus=ast;   //for bsread
    }
    void setAlarmSeverity(int asev)
    {
        alarmSeverity=asev;   //for bsread
    }

    const char  * getPV() const
    {
        return pv;
    };
    const char  * getPVName() const
    {
        return pv;
    };
    const char  * getPVAlias() const
    {
        return pvAlias;
    };
    const char  * getDevice() const
    {
        return device;
    };
    const char  * getAttribute() const
    {
        return attrib;
    };
    unsigned int  getNelem() const
    {
        return nelem;
    };
    //unsigned int  getNelemNative() const {return nelemNative;};
    const unsigned int  getSize() const
    {
        return size;
    };
    CAFE_DATATYPE_UNION_SEQ getVal() const
    {
        return val.get();
    };


    short getAlarmStatus()   const
    {
        return alarmStatus;
    };
    short getAlarmSeverity() const
    {
        return alarmSeverity;
    };

    std::string getAlarmStatusAsString()
    {
        return acond.asString(alarmStatus);
    };
    std::string getAlarmSeverityAsString()
    {
        return aseve.asString(alarmSeverity);
    };

    unsigned long long  getBeamEventNo() const
    {
        return beamEventNo;
    };
    unsigned long long  getPulseID() const
    {
        return beamEventNo;
    };
    bool getRule() const
    {
        return rule;
    };
    bool getHasAlarm() const
    {
        return hasAlarm;
    };
    int  getStatus() const
    {
        return status;
    };
    std::string getStatusAsString()
    {
        return cafeStatusCode.msgIDAsString(status);
    };
    CAFE_DATATYPE getDataTypeClient() const
    {
        return dataType;
    };
    CAFE_DATATYPE getDataType() const
    {
        return dataType;
    };

    CAFEStatusCode getStatusCode() const
    {
        return cafeStatusCode;
    };

    short  getNoEnumStrings () const
    {
        return noStr;
    };
    char * getEnumString(short indx) const
    {
        return (char *) strs[indx];
    }

    void setDataType(CAFE_DATATYPE cdt)
    {
        if (cdt > CAFE_DOUBLE || cdt < CAFE_STRING)
        {
            std::cout << "WARNING: INPUT VALUE NOT A VALID CAFE DATATYPE " << std::endl;
            return;
        }
        else
        {
            dataType=cdt;
            return;
        }
    };

    void set(double d)
    {
        val[0].d=d;
        dataType=CAFE_DOUBLE;
    };
    void set(float  f)
    {
        val[0].f=f;
        dataType=CAFE_FLOAT;
    };
    void set(short  s)
    {
        val[0].s=s;
        dataType=CAFE_SHORT;
    };
    void set(long long l)
    {
        if (l > std::numeric_limits<dbr_long_t>::max())
        {
            std::cout << "WARNING: INPUT VALUE GREATER THAN MAX LIMIT OF dbr_long_t " << std::endl;
            std::cout << "TYPE CASTING TO DOUBLE! " << std::endl;
            val[0].d= (double) l;
            dataType=CAFE_DOUBLE;
        }
        else
        {
            val[0].l= (int) l;
            dataType=CAFE_LONG;
        };
    }
    void set(int    l)
    {
        val[0].l=l;
        dataType=CAFE_LONG;
    };
    void set(unsigned long long  l)
    {
        if (l > (unsigned long long) std::numeric_limits<dbr_long_t>::max())
        {
            std::cout << "WARNING: INPUT VALUE GREATER THAN MAX LIMIT OF dbr_long_t " << std::endl;
            std::cout << "TYPE CASTING TO DOUBLE! " << std::endl;
            val[0].d= (double) l;
            dataType=CAFE_DOUBLE;
        }
        else
        {
            val[0].l= (int) l;
            dataType=CAFE_LONG;
        };
    }

    //For Cython
    void setString(std::string     str)
    {
        strcpy(val[0].str,str.c_str());
        dataType=CAFE_STRING;
    };
    void setDouble(double d)
    {
        val[0].d=d;
        dataType=CAFE_DOUBLE;
    };
    void setInt(int    l)
    {
        val[0].l=l;
        dataType=CAFE_LONG;
    };


    void setVString(std::vector<std::string>  Vstr)
    {
        if(Vstr.size()!=nelem)
        {
            nelem=Vstr.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            strcpy(val[i].str,Vstr[i].c_str());
        }
        dataType=CAFE_STRING;
    };
    void setVDouble(std::vector<double> Vd)
    {
        if(Vd.size()!=nelem)
        {
            nelem=Vd.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].d=Vd[i];
        }
        dataType=CAFE_DOUBLE;
    };
    void setVInt   (std::vector<int>    Vl)
    {
        if(Vl.size()!=nelem)
        {
            nelem=Vl.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].l=Vl[i];
        }
        dataType=CAFE_LONG;
    };




    void set(std::vector<std::string>  Vstr)
    {
        if(Vstr.size()!=nelem)
        {
            nelem=Vstr.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            strcpy(val[i].str,Vstr[i].c_str());
        }
        dataType=CAFE_STRING;
    };
    void set(std::vector<double> Vd)
    {
        if(Vd.size()!=nelem)
        {
            nelem=Vd.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].d=Vd[i];
        }
        dataType=CAFE_DOUBLE;
    };
    void set(std::vector<float> Vf)
    {
        if(Vf.size()!=nelem)
        {
            nelem=Vf.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].f=Vf[i];
        }
        dataType=CAFE_FLOAT;
    };

    void set   (std::vector<int>    Vl)
    {
        if(Vl.size()!=nelem)
        {
            nelem=Vl.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].l=Vl[i];
        }
        dataType=CAFE_LONG;
    };

    void set   (std::vector<long>    Vl)
    {
        if(Vl.size()!=nelem)
        {
            nelem=Vl.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].l=Vl[i];
        }
        dataType=CAFE_LONG;
    };

    void set   (std::vector<unsigned long>    Vul)
    {
        if(Vul.size()!=nelem)
        {
            nelem=Vul.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].d=Vul[i];
        }
        dataType=CAFE_DOUBLE;
    };

    void set   (std::vector<long long>    Vll)
    {
        if(Vll.size()!=nelem)
        {
            nelem=Vll.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].d=Vll[i];
        }
        dataType=CAFE_DOUBLE;
    };

    void set   (std::vector<unsigned long long>    Vull)
    {
        if(Vull.size()!=nelem)
        {
            nelem=Vull.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].d=Vull[i];
        }
        dataType=CAFE_DOUBLE;
    };

    void set   (std::vector<short>    Vs)
    {
        if(Vs.size()!=nelem)
        {
            nelem=Vs.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].s=Vs[i];
        }
        dataType=CAFE_SHORT;
    };

    void set   (std::vector<unsigned short>    Vus)
    {
        if(Vus.size()!=nelem)
        {
            nelem=Vus.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].us=Vus[i];
        }
        dataType=CAFE_ENUM;
    };

    void set   (std::vector<unsigned char>    Vc)
    {
        if(Vc.size()!=nelem)
        {
            nelem=Vc.size();
        }
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].ch=Vc[i];
        }
        dataType=CAFE_CHAR;
    };



    void set(unsigned int    l)
    {
        val[0].l= (int) l;
        dataType=CAFE_LONG;
    };
    void set(unsigned short  us)
    {
        val[0].us=us;
        dataType=CAFE_ENUM;
    };
    void set(unsigned char   ch)
    {
        val[0].ch=ch;
        dataType=CAFE_CHAR;
    };
    void set(dbr_string_t    str)
    {
        strcpy(val[0].str,str);
        dataType=CAFE_STRING;
    };
    void set(std::string     str)
    {
        strcpy(val[0].str,str.c_str());
        dataType=CAFE_STRING;
    };


    void set(double * d)
    {
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].d=d[i];
        }
        dataType=CAFE_DOUBLE;
    };
    void set(float  * f)
    {
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].f=f[i];
        }
        dataType=CAFE_FLOAT;
    };
    void set(short  * s)
    {
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].s=s[i];
        }
        dataType=CAFE_SHORT;
    };
    //Examine this!
    void set(long long * ll)
    {
        if ( nelem==1)
        {
            set((long long) ll[0]);
        }
        else
        {
            for (unsigned int  i=0; i<nelem; ++ i)
            {
                val[i].d= ll[i];
            }
            dataType=CAFE_DOUBLE;
        }
    };
    void set(unsigned long long * ll)
    {
        if ( nelem==1)
        {
            set((unsigned long long) ll[0]);
        }
        else
        {
            for (unsigned int  i=0; i<nelem; ++ i)
            {
                val[i].d= ll[i];
            }
            dataType=CAFE_DOUBLE;
        }
    };

    void set(int    * l)
    {
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].l=l[i];
        }
        dataType=CAFE_LONG;
    };
    void set(unsigned int   * l)
    {
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].l= (int) l[i];
        }
        dataType=CAFE_LONG;
    };
    void set(unsigned short * us)
    {
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].us=us[i];
        }
        dataType=CAFE_ENUM;
    };
    void set(unsigned char  * ch)
    {
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            val[i].ch=ch[i];
        }
        dataType=CAFE_CHAR;
    };
    void set(dbr_string_t   * str)
    {
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            strcpy(val[i].str,str[i]);
        }
        dataType=CAFE_STRING;
    };
    void set(std::string    * str)
    {
        for (unsigned int  i=0; i<nelem; ++ i)
        {
            strcpy(val[i].str,str[i].c_str());
        }
        dataType=CAFE_STRING;
    };

    std::string concatToString()
    {
        std::string psWF = "";
        if (dataTypeNative==CAFE_CHAR)
        {
            for (unsigned int i=0; i<nelem; ++i)
            {
                if (val[i].ch != '\0')
                {
                    psWF.append(1, (dbr_char_t) val[i].ch);
                }
            }
        }
        return psWF;
    }

    std::string getWFAsString()
    {
        std::string psWF = "";
        if (dataTypeNative==CAFE_CHAR)
        {
            for (unsigned int i=0; i<nelem; ++i)
            {
                if (val[i].ch != '\0')
                {
                    psWF.append(1, (dbr_char_t) val[i].ch);
                }
            }
        }
        return psWF;
    }


    double   getDouble(unsigned int  idx)
    {
        return  (double)  val[idx].d;
    }
    float    getFloat (unsigned int  idx)
    {
        return  (float)   val[idx].f;
    }
    short    getShort (unsigned int  idx)
    {
        return  (short)   val[idx].s;
    }
    int      getInt   (unsigned int  idx)
    {
        return  (int)    val[idx].l;
    }
    int      getLong  (unsigned int  idx)
    {
        return  (int)    val[idx].l;
    }
    unsigned short  getEnum  (unsigned int  idx)
    {
        return  (unsigned short) val[idx].us;
    }
    unsigned short  getUShort(unsigned int  idx)
    {
        return  (unsigned short) val[idx].us;
    }
    unsigned char   getChar  (unsigned int  idx)
    {
        return  (unsigned char ) val[idx].ch;
    }



    dbr_string_t  * getString(unsigned int  idx)
    {
#define __METHOD__  "PVHolder::getString "
        if (dataType!=CAFE_STRING)
        {
            std::cout << "******* WARNING *******" << std::endl;
            std::cout << __METHOD__ << __LINE__ << std::endl;
            std::cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                      " hence getString method is invalid! " << std::endl;
            std::cout << "Use getAsString method if you wish to retrieve the data as a string! " << std::endl;
            std::cout << "**********************" << std::endl;
            //strcpy(val[idx].str, "");
            return (dbr_string_t *) "";
        }
        return  (dbr_string_t *) val[idx].str;
#undef __METHOD__
    }

    boost::shared_ptr<std::vector<double> > getAsVDouble()
    {
#define __METHOD__  "PVHolder::getAsVDouble "

        ValVD_ptr.reset(new std::vector<double>());
        ValVD_ptr->reserve(nelem);

        switch (dataType)
        {
        case CAFE_DOUBLE:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVD_ptr->push_back(val[i].d);
            }
            break;
        case CAFE_FLOAT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVD_ptr->push_back(val[i].f);
            }
            break;
        case CAFE_LONG:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVD_ptr->push_back(val[i].l);
            }
            break;
        case CAFE_SHORT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVD_ptr->push_back(val[i].s);
            }
            break;
        case CAFE_ENUM:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVD_ptr->push_back(val[i].us);
            }
            break;
        case CAFE_CHAR:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVD_ptr->push_back(val[i].ch);
            }
            break;
        case CAFE_STRING:
        default:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVD_ptr->push_back( getAsDouble(i));
            }
            break;
        }

        return  ValVD_ptr;
#undef __METHOD__
    }

    boost::shared_ptr<std::vector<float> > getAsVFloat()
    {
#define __METHOD__  "PVHolder::getAsVFloat "

        ValVF_ptr.reset(new std::vector<float>());
        ValVF_ptr->reserve(nelem);

        switch (dataType)
        {
        case CAFE_DOUBLE:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVF_ptr->push_back((float) val[i].d);
            }
            break;
        case CAFE_FLOAT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVF_ptr->push_back(val[i].f);
            }
            break;
        case CAFE_LONG:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVF_ptr->push_back(val[i].l);
            }
            break;
        case CAFE_SHORT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVF_ptr->push_back(val[i].s);
            }
            break;
        case CAFE_ENUM:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVF_ptr->push_back(val[i].us);
            }
            break;
        case CAFE_CHAR:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVF_ptr->push_back(val[i].ch);
            }
            break;
        case CAFE_STRING:
        default:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVD_ptr->push_back( getAsFloat(i));
            }
            break;
        }

        //How to index shared pointer for <std::vector<float> >
        //std::vector<float> * vf= ValVF_ptr.get();
        //std::cout << "size/// " << vf[0].size() << std::endl;
        //std::cout << vf[0][0] << " val " << val[0].f << std::endl;
        //std::cout << vf[0][1] << " val " << val[1].f << std::endl;
        //std::vector<float>  vf= *ValVF_ptr.get();
        //std::cout << "size/// " << vf.size() << std::endl;
        //std::cout << vf[0] << " val " << val[0].f << std::endl;
        //std::cout << vf[1] << " val " << val[1].f << std::endl;

        return  ValVF_ptr;
#undef __METHOD__
    }


    boost::shared_ptr<std::vector<int> > getAsVInt()
    {
#define __METHOD__  "PVHolder::getAsVInt "

        ValVI_ptr.reset(new std::vector<int>());
        ValVI_ptr->reserve(nelem);

        switch (dataType)
        {
        case CAFE_DOUBLE:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVI_ptr->push_back((int) val[i].d);
            }
            break;
        case CAFE_FLOAT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVI_ptr->push_back((int) val[i].f);
            }
            break;
        case CAFE_LONG:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVI_ptr->push_back(val[i].l);
            }
            break;
        case CAFE_SHORT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVI_ptr->push_back(val[i].s);
            }
            break;
        case CAFE_ENUM:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVI_ptr->push_back(val[i].us);
            }
            break;
        case CAFE_CHAR:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVI_ptr->push_back(val[i].ch);
            }
            break;
        case CAFE_STRING:
        default:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVI_ptr->push_back( getAsInt(i));
            }
            break;
        }

        return  ValVI_ptr;
#undef __METHOD__
    }



    boost::shared_ptr<std::vector<long> > getAsVLong()
    {
#define __METHOD__  "PVHolder::getAsVInt "

        ValVL_ptr.reset(new std::vector<long>());
        ValVL_ptr->reserve(nelem);

        switch (dataType)
        {
        case CAFE_DOUBLE:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVL_ptr->push_back((long) val[i].d);
            }
            break;
        case CAFE_FLOAT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVL_ptr->push_back((long) val[i].f);
            }
            break;
        case CAFE_LONG:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVL_ptr->push_back((long) val[i].l);
            }
            break;
        case CAFE_SHORT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVL_ptr->push_back((long) val[i].s);
            }
            break;
        case CAFE_ENUM:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVL_ptr->push_back((long) val[i].us);
            }
            break;
        case CAFE_CHAR:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVL_ptr->push_back((long) val[i].ch);
            }
            break;
        case CAFE_STRING:
        default:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVL_ptr->push_back( getAsLong(i));
            }
            break;
        }

        return  ValVL_ptr;
#undef __METHOD__
    }


    boost::shared_ptr<std::vector<unsigned long> > getAsVULong()
    {
#define __METHOD__  "PVHolder::getAsVInt "

        ValVUL_ptr.reset(new std::vector<unsigned long>());
        ValVUL_ptr->reserve(nelem);

        switch (dataType)
        {
        case CAFE_DOUBLE:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUL_ptr->push_back((unsigned long) val[i].d);
            }
            break;
        case CAFE_FLOAT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUL_ptr->push_back((unsigned long) val[i].f);
            }
            break;
        case CAFE_LONG:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUL_ptr->push_back((unsigned long) val[i].l);
            }
            break;
        case CAFE_SHORT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUL_ptr->push_back((unsigned long) val[i].s);
            }
            break;
        case CAFE_ENUM:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUL_ptr->push_back((unsigned long) val[i].us);
            }
            break;
        case CAFE_CHAR:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUL_ptr->push_back((unsigned long) val[i].ch);
            }
            break;
        case CAFE_STRING:
        default:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUL_ptr->push_back( getAsULong(i));
            }
            break;
        }

        return  ValVUL_ptr;
#undef __METHOD__
    }




    boost::shared_ptr<std::vector<long long> > getAsVLongLong()
    {
#define __METHOD__  "PVHolder::getAsVLongLong "

        ValVLL_ptr.reset(new std::vector<long long>());
        ValVLL_ptr->reserve(nelem);

        switch (dataType)
        {
        case CAFE_DOUBLE:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVLL_ptr->push_back((long long) val[i].d);
            }
            break;
        case CAFE_FLOAT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVLL_ptr->push_back((long long) val[i].f);
            }
            break;
        case CAFE_LONG:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVLL_ptr->push_back((long long) val[i].l);
            }
            break;
        case CAFE_SHORT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVLL_ptr->push_back((long long) val[i].s);
            }
            break;
        case CAFE_ENUM:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVLL_ptr->push_back((long long) val[i].us);
            }
            break;
        case CAFE_CHAR:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVLL_ptr->push_back((long long) val[i].ch);
            }
            break;
        case CAFE_STRING:
        default:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVLL_ptr->push_back( getAsLongLong(i));
            }
            break;
        }

        return  ValVLL_ptr;
#undef __METHOD__
    }


    boost::shared_ptr<std::vector<unsigned long long> > getAsVULongLong()
    {
#define __METHOD__  "PVHolder::getAsVLongLong "

        ValVULL_ptr.reset(new std::vector<unsigned long long>());
        ValVULL_ptr->reserve(nelem);

        switch (dataType)
        {
        case CAFE_DOUBLE:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVULL_ptr->push_back((unsigned long long) val[i].d);
            }
            break;
        case CAFE_FLOAT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVULL_ptr->push_back((unsigned long long) val[i].f);
            }
            break;
        case CAFE_LONG:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVULL_ptr->push_back((unsigned long long) val[i].l);
            }
            break;
        case CAFE_SHORT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVULL_ptr->push_back((unsigned long long) val[i].s);
            }
            break;
        case CAFE_ENUM:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVULL_ptr->push_back((unsigned long long) val[i].us);
            }
            break;
        case CAFE_CHAR:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVULL_ptr->push_back((unsigned long long) val[i].ch);
            }
            break;
        case CAFE_STRING:
        default:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVULL_ptr->push_back( getAsULongLong(i));
            }
            break;
        }

        return  ValVULL_ptr;
#undef __METHOD__
    }





    boost::shared_ptr<std::vector<short> > getAsVShort()
    {
#define __METHOD__  "PVHolder::getAsVShort "

        ValVS_ptr.reset(new std::vector<short>());
        ValVS_ptr->reserve(nelem);

        switch (dataType)
        {
        case CAFE_DOUBLE:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVS_ptr->push_back((short) val[i].d);
            }
            break;
        case CAFE_FLOAT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVS_ptr->push_back((short) val[i].f);
            }
            break;
        case CAFE_LONG:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVS_ptr->push_back((short) val[i].l);
            }
            break;
        case CAFE_SHORT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVS_ptr->push_back(val[i].s);
            }
            break;
        case CAFE_ENUM:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVS_ptr->push_back((short) val[i].us);
            }
            break;
        case CAFE_CHAR:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVS_ptr->push_back((unsigned short) val[i].ch);
            }
            break;
        case CAFE_STRING:
        default:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVS_ptr->push_back( getAsShort(i));
            }
            break;
        }

        return  ValVS_ptr;
#undef __METHOD__
    }



    boost::shared_ptr<std::vector<unsigned short> > getAsVUShort()
    {
#define __METHOD__  "PVHolder::getAsVUShort "

        ValVUS_ptr.reset(new std::vector<unsigned short>());
        ValVUS_ptr->reserve(nelem);

        switch (dataType)
        {
        case CAFE_DOUBLE:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUS_ptr->push_back((unsigned short) val[i].d);
            }
            break;
        case CAFE_FLOAT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUS_ptr->push_back((unsigned short) val[i].f);
            }
            break;
        case CAFE_LONG:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUS_ptr->push_back((unsigned short) val[i].l);
            }
            break;
        case CAFE_SHORT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUS_ptr->push_back((unsigned short)val[i].s);
            }
            break;
        case CAFE_ENUM:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUS_ptr->push_back( val[i].us);
            }
            break;
        case CAFE_CHAR:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUS_ptr->push_back((unsigned short) val[i].ch);
            }
            break;
        case CAFE_STRING:
        default:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVUS_ptr->push_back( getAsUShort(i));
            }
            break;
        }

        return  ValVUS_ptr;
#undef __METHOD__
    }


    boost::shared_ptr<std::vector<unsigned char> > getAsVUChar()
    {
#define __METHOD__  "PVHolder::getAsVUChar "

        ValVC_ptr.reset(new std::vector<unsigned char>());
        ValVC_ptr->reserve(nelem);

        switch (dataType)
        {
        case CAFE_DOUBLE:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVC_ptr->push_back((unsigned char) val[i].d);
            }
            break;
        case CAFE_FLOAT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVC_ptr->push_back((unsigned char) val[i].f);
            }
            break;
        case CAFE_LONG:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVC_ptr->push_back((unsigned char) val[i].l);
            }
            break;
        case CAFE_SHORT:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVC_ptr->push_back((unsigned char)val[i].s);
            }
            break;
        case CAFE_ENUM:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVC_ptr->push_back( (unsigned char)val[i].us);
            }
            break;
        case CAFE_CHAR:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVC_ptr->push_back((unsigned char) val[i].ch);
            }
            break;
        case CAFE_STRING:
        default:
            for (unsigned i=0; i<nelem; ++i)
            {
                ValVC_ptr->push_back( (unsigned char) getAsChar(i));
            }
            break;
        }

        return  ValVC_ptr;
#undef __METHOD__
    }

    boost::shared_ptr<std::vector<std::string> > getAsVString()
    {
#define __METHOD__  "PVHolder::getAsVString "

        ValVStr_ptr.reset(new std::vector<std::string>());
        ValVStr_ptr->reserve(nelem);
        for (unsigned i=0; i<nelem; ++i)
        {
            ValVStr_ptr->push_back( getAsString(i));
        }

        return  ValVStr_ptr;
#undef __METHOD__
    }



    std::vector<std::string> getAsVectorString()
    {
        boost::shared_ptr<std::vector<std::string> >  spVs = getAsVString();
        return *spVs.get();
    }
    std::vector<float> getAsVectorFloat()
    {
        boost::shared_ptr<std::vector<float> >  spVf = getAsVFloat();
        return *spVf.get();
    }
    std::vector<double> getAsVectorDouble()
    {
        boost::shared_ptr<std::vector<double> >  spVd = getAsVDouble();
        return *spVd.get();
    }
    std::vector<int> getAsVectorInt()
    {
        boost::shared_ptr<std::vector<int> >  spVi = getAsVInt();
        return *spVi.get();
    }

    std::vector<long> getAsVectorLong()
    {
        boost::shared_ptr<std::vector<long> >  spVl = getAsVLong();
        return *spVl.get();
    }

    std::vector<unsigned long> getAsVectorULong()
    {
        boost::shared_ptr<std::vector<unsigned long> >  spVul = getAsVULong();
        return *spVul.get();
    }

    std::vector<long long> getAsVectorLongLong()
    {
        boost::shared_ptr<std::vector<long long> >  spVll = getAsVLongLong();
        return *spVll.get();
    }

    std::vector<unsigned long long> getAsVectorULongLong()
    {
        boost::shared_ptr<std::vector<unsigned long long> >  spVull = getAsVULongLong();
        return *spVull.get();
    }


    std::vector<short> getAsVectorShort()
    {
        boost::shared_ptr<std::vector<short> >  spVsh = getAsVShort();
        return *spVsh.get();
    }

    std::vector<unsigned short> getAsVectorUShort()
    {
        boost::shared_ptr<std::vector<unsigned short> >  spVus = getAsVUShort();
        return *spVus.get();
    }
    std::vector<unsigned char> getAsVectorUChar()
    {
        boost::shared_ptr<std::vector<unsigned char> >  spVc = getAsVUChar();
        return *spVc.get();
    }


    ValDPtr getDouble()
    {
#define __METHOD__  "PVHolder::getDouble "
        if (dataType!=CAFE_DOUBLE)
        {
            std::cout << "******* WARNING *******" << std::endl;
            std::cout <<  __METHOD__ << __LINE__  << std::endl;
            std::cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                      " hence getDouble method is invalid! " << std::endl;
            std::cout << "Use getAsDouble method if you wish to retrieve the data as a double! " << std::endl;
            std::cout << "**********************" << std::endl;
        }

        ValD_ptr.reset(new double[nelem]);

        for (unsigned i=0; i<nelem; ++i)
        {
            ValD_ptr[i] = val[i].d;
        }

        return  ValD_ptr;
#undef __METHOD__
    }

    ValFPtr getFloat()
    {
#define __METHOD__  "PVHolder::getFloat "
        if (dataType!=CAFE_FLOAT)
        {
            std::cout << "******* WARNING *******" << std::endl;
            std::cout <<  __METHOD__ << __LINE__  << std::endl;
            std::cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                      " hence getFloat method is invalid! " << std::endl;
            std::cout << "Use getAsFloat method if you wish to retrieve the data as a float! " << std::endl;
            std::cout << "**********************" << std::endl;
        }
        ValF_ptr.reset(new float[nelem]);
        for (unsigned i=0; i<nelem; ++i)
        {
            ValF_ptr[i] = val[i].f;
        }
        return  ValF_ptr;
        //return (float *) val.get();
#undef __METHOD__
    }

    ValSPtr getShort()
    {
#define __METHOD__  "PVHolder::getShort "
        if (dataType==CAFE_SHORT)
        {
            std::cout << "******* WARNING *******" << std::endl;
            std::cout <<  __METHOD__ << __LINE__  << std::endl;
            std::cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                      " hence getShort method is invalid! " << std::endl;
            std::cout << "Use getAsShort method if you wish to retrieve the data as a short! " << std::endl;
            std::cout << "**********************" << std::endl;
        }
        ValS_ptr.reset(new short[nelem]);
        for (unsigned i=0; i<nelem; ++i)
        {
            ValS_ptr[i] = val[i].s;
        }
        return  ValS_ptr;

#undef __METHOD__
    }

    ValIPtr getInt()
    {
#define __METHOD__  "PVHolder::getInt (meaning dbr_long_t)  "
        if (dataType!=CAFE_LONG)
        {
            std::cout << "******* WARNING *******" << std::endl;
            std::cout <<  __METHOD__ << __LINE__  << std::endl;
            std::cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                      " hence getInt  method is invalid! " << std::endl;
            std::cout << "Use getAsInt  method if you wish to retrieve the data as a int! " << std::endl;
            std::cout << "**********************" << std::endl;
        }
        ValI_ptr.reset(new int[nelem]);
        for (unsigned i=0; i<nelem; ++i)
        {
            ValI_ptr[i] = val[i].l;
        }
        return  ValI_ptr;
#undef __METHOD__
    }

    ValIPtr getLong()
    {
#define __METHOD__  "PVHolder::getLong (meaning dbr_long_t)  "
        if (dataType!=CAFE_LONG)
        {
            std::cout << "******* WARNING *******" << std::endl;
            std::cout <<  __METHOD__ << __LINE__  << std::endl;
            std::cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                      " hence getLong  method is invalid! " << std::endl;
            std::cout << "Use getAsLong  method if you wish to retrieve the data as a dbr_long_t! " << std::endl;
            std::cout << "**********************" << std::endl;
        }
        ValI_ptr.reset(new int[nelem]);
        for (unsigned i=0; i<nelem; ++i)
        {
            ValI_ptr[i] = val[i].l;
        }
        return  ValI_ptr;
#undef __METHOD__
    }

    ValUSPtr getEnum()
    {
#define __METHOD__  "PVHolder::getEnum "
        if (dataType!=CAFE_ENUM)
        {
            std::cout << "******* WARNING *******" << std::endl;
            std::cout <<  __METHOD__ << __LINE__  << std::endl;
            std::cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                      " hence getEnum method is invalid! " << std::endl;
            std::cout << "Use getAsEnum method if you wish to retrieve the data as an enum (unsigned short)! " << std::endl;
            std::cout << "**********************" << std::endl;
        }
        ValUS_ptr.reset(new unsigned short[nelem]);
        for (unsigned i=0; i<nelem; ++i)
        {
            ValUS_ptr[i] = val[i].us;
        }
        return  ValUS_ptr;
#undef __METHOD__
    }

    ValUSPtr getUShort()
    {
#define __METHOD__  "PVHolder::getUShort "
        if (dataType!=CAFE_USHORT)
        {
            std::cout << "******* WARNING *******" << std::endl;
            std::cout <<  __METHOD__ << __LINE__  << std::endl;
            std::cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                      " hence getUShort method is invalid! " << std::endl;
            std::cout << "Use getAsUSHort method if you wish to retrieve the data as an unsigned short! " << std::endl;
            std::cout << "**********************" << std::endl;
        }
        ValUS_ptr.reset(new unsigned short[nelem]);
        for (unsigned i=0; i<nelem; ++i)
        {
            ValUS_ptr[i] = val[i].us;
        }
        return  ValUS_ptr;
#undef __METHOD__
    }

    ValChPtr getChar()
    {
#define __METHOD__  "PVHolder::getChar "
        if (dataType!=CAFE_CHAR)
        {
            std::cout << "******* WARNING *******" << std::endl;
            std::cout <<  __METHOD__ << __LINE__  << std::endl;
            std::cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                      " hence getChar method is invalid! " << std::endl;
            std::cout << "Use getAsChar method if you wish to retrieve the data as a char! " << std::endl;
            std::cout << "**********************" << std::endl;
        }
        ValCh_ptr.reset(new unsigned char[nelem]);
        for (unsigned i=0; i<nelem; ++i)
        {
            ValCh_ptr[i] = val[i].ch;
        }
        return  ValCh_ptr;
#undef __METHOD__
    }

    ValStrPtr getString()
    {
#define __METHOD__  "PVHolder::getString "
        if (dataType!=CAFE_STRING)
        {
            std::cout << "******* WARNING *******" << std::endl;
            std::cout << __METHOD__ << __LINE__ << std::endl;
            std::cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                      " hence getString method is invalid! " << std::endl;
            std::cout << "Use getAsString method if you wish to retrieve the data as a string! " << std::endl;
            std::cout << "**********************" << std::endl;
            //return (dbr_string_t *) "";
        }
        ValStr_ptr.reset(new dbr_string_t[nelem]);
        for (unsigned i=0; i<nelem; ++i)
        {
            strcpy(ValStr_ptr[i], val[i].str);
        }
        return  ValStr_ptr;
#undef __METHOD__
    }

    double getAsDouble ()
    {
        return (double)  renderDouble.get(0, dataType, val.get())[0];
    }

    double getAsDouble(unsigned int  idx) noexcept(false) //throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (double)  renderDouble.get(idx, dataType, val.get())[0];
    }

    float getAsFloat ()
    {
        return (float)  renderFloat.get(0, dataType, val.get())[0];
    }

    float getAsFloat(unsigned int  idx) noexcept(false) //throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (float) renderFloat.get(idx, dataType, val.get())[0];
    }

    short getAsShort ()
    {
        return (short)  renderShort.get(0, dataType, val.get())[0];
    }

    short getAsShort(unsigned int  idx)  noexcept(false) //throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (short) renderShort.get(idx, dataType, val.get())[0];
    }

    int  getAsLong  ()
    {
        return (int)  renderLong.get(0, dataType, val.get())[0];
    }

    int  getAsLong(unsigned int  idx) noexcept(false) // throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (int) renderLong.get(idx, dataType, val.get())[0];
    }

    unsigned short getAsEnum ()
    {
        return (unsigned short)  renderEnum.get(0, dataType, val.get())[0];
    }

    unsigned short  getAsEnum(unsigned int  idx)  noexcept(false) //throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (unsigned short) renderEnum.get(idx, dataType, val.get())[0];
    }

    unsigned short getAsUShort ()
    {
        return (unsigned short)  renderEnum.get(0, dataType, val.get())[0];
    }

    unsigned short getAsUShort(unsigned int  idx)  noexcept(false) //throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (unsigned short) renderEnum.get(idx, dataType, val.get())[0];
    }

    unsigned char getAsChar ()
    {
        return (char) renderChar.get(0, dataType, val.get())[0];
    }

    unsigned char getAsChar(unsigned int  idx)  noexcept(false) //throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (char) renderChar.get(idx, dataType, val.get())[0];
    }

    unsigned char getAsUChar ()
    {
        return (unsigned char) renderUChar.get(0, dataType, val.get())[0];
    }

    unsigned char getAsUChar(unsigned int  idx) noexcept(false) // throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (unsigned char) renderUChar.get(idx, dataType, val.get())[0];
    }

    unsigned int  getAsULong  ()
    {
        return (unsigned int)  renderULong.get(0, dataType, val.get())[0];
    }

    unsigned int  getAsULong(unsigned int  idx) noexcept(false) // throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (unsigned int) renderULong.get(idx, dataType, val.get())[0];
    }

    long long  getAsLongLong  ()
    {
        return (long  long)  renderLongLong.get(0, dataType, val.get())[0];
    }

    long long  getAsLongLong(unsigned int  idx) noexcept(false) // throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (long  long) renderLongLong.get(idx, dataType, val.get())[0];
    }

    unsigned long long  getAsULongLong  ()
    {
        return (unsigned long  long)  renderULongLong.get(0, dataType, val.get())[0];
    }

    unsigned long  long  getAsULongLong(unsigned int  idx) noexcept(false) // throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (unsigned long  long) renderULongLong.get(idx, dataType, val.get())[0];
    }

    int getAsInt ()
    {
        return (int)  renderInt.get(0, dataType, val.get())[0];
    }

    int getAsInt(unsigned int  idx) noexcept(false) // throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (int) renderInt.get(idx, dataType, val.get())[0];
    }

    unsigned int getAsUInt ()
    {
        return (unsigned int)  renderInt.get(0, dataType, val.get())[0];
    }

    unsigned int getAsUInt(unsigned int  idx)  noexcept(false) //throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        return (unsigned int) renderInt.get(idx, dataType, val.get())[0];
    }

    //getEnumAsNumberedString
    std::string getEnumIntegerValueAsString()
    {
#define __METHOD__  "PVHolder::getEnumIntegerValueAsString"
        if (dataType!=CAFE_ENUM)
        {
            std::cout << "******* WARNING *******" << std::endl;
            std::cout << __METHOD__ << __LINE__ << std::endl;
            std::cout << "DataType is " << (cafeDataTypeCode.message((CAFE_DATATYPE)dataType)).c_str() <<
                      " hence getEnumIntegerValueAsString method is invalid! " << std::endl;
            std::cout << "Use getAsString method if you want the string equilavent of the ENUM value!" << std::endl;
            std::cout << "**********************" << std::endl;
        }
        return (std::string) renderString.getString(0, dataType, val.get())[0];
#undef __METHOD__
    }

    std::string getAsString()
    {
        if(dataTypeNative==DBR_ENUM && dataType==CAFE_ENUM)
        {

            return  (std::string) renderString.getStringFromEnum(0,  noStr, val.get(), strs)[0];
        }
        else
        {
            return (std::string) renderString.getString(0, dataType, val.get())[0];
        }
    }

    std::string getAsString(unsigned int  idx) noexcept(false) //throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        if(dataTypeNative==DBR_ENUM && dataType==CAFE_ENUM)
        {
            return  (std::string) renderString.getStringFromEnum(idx,  noStr, val.get(), strs)[0];
        }
        else
        {
            return (std::string) renderString.getString(idx, dataType, val.get())[0];
        }
    }


    char * getAsDbr_string_t()
    {
        if(dataTypeNative==DBR_ENUM && dataType==CAFE_ENUM)
        {
            return (char *) renderString.getStringFromEnum(0,  noStr, val.get(), strs)[0];

        }
        else
        {
            return (char *) renderString.getString(0, dataType, val.get())[0];
        }
    }


    char *  getAsDbr_string_t(unsigned int  idx) noexcept(false) // throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to PVHolder method is out of range. Valid range is from 0 to " << size-1;
            throw std::out_of_range(oss.str());
        }
        if(dataTypeNative==DBR_ENUM && dataType==CAFE_ENUM)
        {
            return  (char *) renderString.getStringFromEnum(idx, noStr, val.get(), strs)[0];
        }
        else
        {
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
    double  getAsDouble(CAFE_DATATYPE_UNION  cdu)
    {
        return (double) renderDouble.get(0, dataType, &cdu)[0];
    }

    float   getAsFloat(CAFE_DATATYPE_UNION cdu)
    {
        return (float) renderFloat.get(0, dataType, &cdu)[0];
    }

    short   getAsShort(CAFE_DATATYPE_UNION cdu)
    {
        return (short) renderShort.get(0, dataType, &cdu)[0];
    }

    int     getAsLong(CAFE_DATATYPE_UNION  cdu)
    {
        return (int) renderLong.get(0, dataType, &cdu)[0];
    }

    unsigned short  getAsEnum(CAFE_DATATYPE_UNION cdu)
    {
        return (unsigned short) renderEnum.get(0, dataType, &cdu)[0];
    }

    unsigned short  getAsUShort(CAFE_DATATYPE_UNION cdu)
    {
        return (unsigned short) renderEnum.get(0, dataType, &cdu)[0];
    }

    char   getAsChar(CAFE_DATATYPE_UNION cdu)
    {
        char  * ans = renderChar.get(0, dataType, &cdu);
        return (char ) ans[0];
    }
    unsigned char   getAsUChar(CAFE_DATATYPE_UNION cdu)
    {
        unsigned char  * ans = renderUChar.get(0, dataType, &cdu);
        return (unsigned char ) ans[0];
    }

    std::string getAsString(CAFE_DATATYPE_UNION cdu)
    {
        return (std::string) renderString.getString(dataType, cdu)[0];
    }

    char * getAsDbr_string_t(CAFE_DATATYPE_UNION cdu)
    {
        return (char *) renderString.getString(0, dataType, &cdu)[0];
    }
};


#endif //PVHOLDER_H
