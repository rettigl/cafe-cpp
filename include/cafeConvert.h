///
/// \file    cafeConvert.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef CAFECONVERT_H
#define CAFECONVERT_H

#include <cafeDataType.h>
#include <cstdlib>  // g++ 4.4.4
#include <cstdio>
#include <boost/math/special_functions/fpclassify.hpp>

/**
 * CAFEConvert Template \n
 * CTYPE is the input data type \n
 * PVDataHolder methods use CAFEConvert as follows: \n
 * method getAsDouble() converts CTYPE to double \n
 * method getAsFloat () converts CTYPE to float  \n
 * method getAsShort () converts CTYPE to short  \n
 * method getAsEnum  () converts CTYPE to enum   \n
 * method getAsChar  () converts CTYPE to char   \n
 * method getAsLong  () converts CTYPE to int   \n
 * method getAsString() converts CTYPE to string \n
 *
 */
template <class CTYPE> class CAFEConvert
{
private:
    CTYPE returnVal[1];
public:
    CAFEConvert (unsigned int nelem) {};
    CAFEConvert () {};
    ~CAFEConvert () {};
    CTYPE * get(unsigned int index, CAFE_DATATYPE dt, CAFE_DATATYPE_UNION_SEQ val);
    CTYPE * getString(unsigned int index, CAFE_DATATYPE dt, CAFE_DATATYPE_UNION_SEQ val);
    CTYPE * getStringFromEnum(unsigned int index, unsigned int noStr, CAFE_DATATYPE_UNION_SEQ val, char stig[MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE]);
    CTYPE * get(CAFE_DATATYPE dt, CAFE_DATATYPE_UNION val);
    CTYPE * getString(CAFE_DATATYPE dt, CAFE_DATATYPE_UNION val);
};


/**
 *  Converts from native type to CTYPE
 *  \param dt  input: CAFE_DATATYPE
 *  \param val input: CAFE_DATATYPE_UNION
 *  \return CTYPE
 */
template <class CTYPE> CTYPE * CAFEConvert<CTYPE>::get (CAFE_DATATYPE dt, CAFE_DATATYPE_UNION val)
{
#define __METHOD__ "CAFEConvert<CTYPE>::get(dt, val)"

    // (boost::math::isnan) calls the Boost version of the isnan  macro
    // Valid for all types that have numeric_limits support
    // (brackets required) to avoid compiler error should isnan also be a native macro

    switch (dt)
    {
    case CAFE_DOUBLE:
        if ( (boost::math::isnan)((CTYPE) val.d) )
        {
            returnVal[0]= (CTYPE) val.d;
        }
        else
        {
            returnVal[0]= (CTYPE) 0;
        }
        break;
    case CAFE_FLOAT:
        if ( (boost::math::isnan)((CTYPE) val.f) )
        {
            returnVal[0]= (CTYPE) val.f;
        }
        else
        {
            returnVal[0]= 0;
        }
        break;
    case CAFE_LONG:
        returnVal[0]= (CTYPE) val.l;
        break;
    case CAFE_SHORT:
        returnVal[0]= (CTYPE) val.s;
        break;
    case CAFE_ENUM:
        returnVal[0]= (CTYPE) val.us;
        break;
    case CAFE_CHAR:
        returnVal[0]= (CTYPE) val.ch;
        break;
    case CAFE_STRING:
        returnVal[0]= (CTYPE) strtod( val.str,  NULL);
        break;
    case CAFE_TYPENOTCONN:
        //std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
        //        <<  " CAFE_TYPENOTCONN: dataType: " << dt << " " << std::endl;
        returnVal[0]=0;
        break;
    case CAFE_NO_ACCESS:
        //std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
        //        << " CAFE_NO_ACCESS: dataType: " << dt << " " << std::endl;
        returnVal[0]=0;
        break;
    case CAFE_INVALID_DATATYPE:
        //std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
        //        << " CAFE_INVALID_DATATYPE: dataType: " << dt << " " << std::endl;
        returnVal[0]=0;
        break;
    case CAFE_NOT_REQUESTED:
        //std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
        //       << " CAFE_NOT_REQUESTED: dataType: " << dt << " " << std::endl;
        returnVal[0]=0;
        break;
    case CAFE_NOT_SHOWN:
        //std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
        //        << " CAFE_INVALID_DATATYPE: dataType: " << dt << " " << std::endl;
        returnVal[0]=0;
        break;
    default:
        std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
                  << " CAFE INTERNAL ERROR: Unknown dataType: " << dt << " " << std::endl;
        returnVal[0]=0;
        break;
    }

    return (CTYPE *) returnVal;

#undef __METHOD__
}

/**
 *  Converts from native type to CTYPE
 *  \param index input: index to val array
 *  \param dt  input: CAFE_DATATYPE
 *  \param val input: CAFE_DATATYPE_UNION_SEQ
 *  \return CTYPE
 */
template <class CTYPE> CTYPE * CAFEConvert<CTYPE>::get (unsigned int index, CAFE_DATATYPE dt, CAFE_DATATYPE_UNION_SEQ val)
{
#define __METHOD__ "CAFEConvert<CTYPE>::get(index, dt, val[])"

    switch (dt)
    {
    case CAFE_DOUBLE:
        returnVal[0]= (CTYPE) val[index].d;
        break;
    case CAFE_FLOAT:
        returnVal[0]= (CTYPE) val[index].f;
        break;
    case CAFE_LONG:
        returnVal[0]= (CTYPE) val[index].l;
        break;
    case CAFE_SHORT:
        returnVal[0]= (CTYPE) val[index].s;
        break;
    case CAFE_ENUM:
        returnVal[0]= (CTYPE) val[index].us;
        break;
    case CAFE_CHAR:
        returnVal[0]= (CTYPE) val[index].ch;
        break;
    case CAFE_STRING:
        returnVal[0]= (CTYPE) strtod( val[index].str,  NULL);
        break;
    case CAFE_TYPENOTCONN:
        //std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
        //        <<  " CAFE_TYPENOTCONN: dataType: " << dt << " " << std::endl;
        returnVal[0]=0;
        break;
    case CAFE_NO_ACCESS:
        //std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
        //        << " CAFE_NO_ACCESS: dataType: " << dt << " " << std::endl;
        returnVal[0]=0;
        break;
    case CAFE_INVALID_DATATYPE:
        //std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
        //        << " CAFE_INVALID_DATATYPE: dataType: " << dt << " " << std::endl;
        returnVal[0]=0;
        break;
    case CAFE_NOT_REQUESTED:
        //std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
        //       << " CAFE_NOT_REQUESTED: dataType: " << dt << " " << std::endl;
        returnVal[0]=0;
        break;
    case CAFE_NOT_SHOWN:
        //std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
        //        << " CAFE_INVALID_DATATYPE: dataType: " << dt << " " << std::endl;
        returnVal[0]=0;
        break;
    default:
        std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__
                  << " CAFE INTERNAL ERROR: Unknown dataType: " << dt << " " << std::endl;
        returnVal[0]=0;
        break;
    }

    return (CTYPE *) returnVal;

#undef __METHOD__
};




/**
 *  Converts from DBR_ENUM type to CTYPE=STRING
 *  \param index input: index to val array
 *  \param noStr input: number of enumerations
 *  \param val input: CAFE_DATATYPE_UNION_SEQ
 *  \param stig input: stig[MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE] an array of enum choices
 *  \return CTYPE
 */
template <class CTYPE> CTYPE * CAFEConvert<CTYPE>::getStringFromEnum (unsigned int index,  unsigned int noStr,  CAFE_DATATYPE_UNION_SEQ val,
        char stig[MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE])
{
#define __METHOD__ 	"CAFEConvert<CTYPE>::getStringFromEnum(indx, noStr, val, stig)"


    unsigned int noEmptyStrings=0;
    //Check for empty strings:
    for (unsigned int j=0; j<noStr; ++j)
    {
        if (strcmp(stig[j],"")==0)
        {
            ++noEmptyStrings;
        }
    }

    // Some developers do not populate enum string values
    //if (noStr==noEmptyStrings)
    //{
    //    std::cout << "*** WARNING FROM " << __METHOD__ << " *** " << std::endl;
    //    std::cout << "ENUM STRING OPTIONS ARE ALL EMPTY! " << std::endl;
    //    std::cout << "BADLY CONFIGURED EPICS RECORD. " << std::endl;
    //}


    if (index < noStr && noStr!=noEmptyStrings)
    {
        sprintf(returnVal[0], "%s", stig[val[index].us] );
    }
    else
    {
        sprintf(returnVal[0], "%d", val[index].us );
        if (  val[index].us>= noStr)
        {
            std::cout << "*** WARNING FROM " << __METHOD__ << " *** " << std::endl;
            std::cout << "ENUM UNSIGNED SHORT VALUE IS GREATER THAN THE NO OF ENUMERATED TYPES" << std::endl;
            std::cout << "VALUE (unsigned short) = " <<  val[index].us << std::endl;
            std::cout << "NO OF ENUMERATED STRINGS = " << noStr << " WITH VALUES: " << std::endl;
            for (unsigned int j=0; j<noStr; ++j)
            {
                std::cout << stig[j] << " ["  <<j << "]  ";
            }
            std::cout << std::endl;
        }
    }

    return (CTYPE *) returnVal;

#undef __METHOD__
};

/**
 *  Converts from native type to CTYPE=STRING
 *  \param index input: index to val array
 *  \param dt  input: CAFE_DATATYPE
 *  \param val input: CAFE_DATATYPE_UNION_SEQ
 *  \return CTYPE
 */
template <class CTYPE> CTYPE * CAFEConvert<CTYPE>::getString (unsigned int index,  CAFE_DATATYPE dt, CAFE_DATATYPE_UNION_SEQ val)
{
#define __METHOD__  "CAFEConvert<CTYPE>::getString(nelem, dt, val[])"

    switch (dt)
    {
    case CAFE_STRING:
        sprintf(returnVal[0], "%s",  val[index].str);
        break;
    case CAFE_CHAR:
        sprintf(returnVal[0], "%u",  val[index].ch);
        break;
    case CAFE_FLOAT:
        sprintf(returnVal[0], "%f",  val[index].f); //floats offer a precision of 6
        break;
    case CAFE_DOUBLE:
        sprintf(returnVal[0], "%.15f", val[index].d); //double offer a precicion of 15
        break;
    case CAFE_SHORT:
        sprintf(returnVal[0], "%d",  val[index].s);
        break;
    case CAFE_LONG:
        sprintf(returnVal[0], "%d", val[index].l);
        break;
    case CAFE_ENUM:
        sprintf(returnVal[0], "%u",  val[index].us);
        break;
    case CAFE_TYPENOTCONN:
        //std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__  << std::endl;
        //std::cout << " ERROR CAFE_TYPENOTCONN: dataType: " << dt  << " " << std::endl;
        sprintf(returnVal[0], "%s",  "0"); //CAFE_TYPENOTCONN");
        break;
    case CAFE_NO_ACCESS:
        //std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        //std::cout << " CAFE_NO_ACCESS: dataType: " << dt << " " << std::endl;
        sprintf(returnVal[0], "%s",  "0"); //"CAFE_NO_ACCESS");
        break;
    case CAFE_INVALID_DATATYPE:
        //std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        //std::cout<<  " CAFE_INVALID_DATATYPE: dataType: " << dt << " " << std::endl;
        sprintf(returnVal[0], "%s",  "0"); //"CAFE_INVALID_DATATYPE");
        break;
    case CAFE_NOT_REQUESTED:
        //std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        //std::cout<< " CAFE_NOT_REQUESTED: dataType: " << dt << " " << std::endl;
        sprintf(returnVal[0], "%s",   "0"); //"");
        break;
    case CAFE_NOT_SHOWN:
        //std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        //std::cout<< " CAFE_NOT_SHOWN: dataType: " << dt << " " << std::endl;
        sprintf(returnVal[0], "%s",  "0"); // "CAFE_NOT_SHOWN");
        break;
    default:
        //std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        //std::cout<< " CAFE INTERNAL ERROR: Unknown dataType: "<< dt << " " << std::endl;
        sprintf(returnVal[0], "%s",  "0");// "Unknown dataType");
        break;

    }

    return (CTYPE *) returnVal;

#undef __METHOD__
};

/**
 *  Converts from native type to CTYPE=STRING
 *  \param dt  input: CAFE_DATATYPE
 *  \param val CAFE_DATATYPE_UNION input
 *  \return CTYPE
 */
template <class CTYPE> CTYPE * CAFEConvert<CTYPE>::getString (CAFE_DATATYPE dt, CAFE_DATATYPE_UNION val)
{
#define __METHOD__  "CAFEConvert<CTYPE>::getString(dt, val[])"

    switch (dt)
    {
    case CAFE_STRING:
        sprintf(returnVal[0], "%s",  val.str);
        break;
    case CAFE_CHAR:
        sprintf(returnVal[0], "%u",  val.ch);
        break;
    case CAFE_FLOAT:
        sprintf(returnVal[0], "%f",  val.f); //floats offer a precision of 6
        break;
    case CAFE_DOUBLE:
        sprintf(returnVal[0], "%.15f", val.d); //double offer a precicion of 15
        break;
    case CAFE_SHORT:
        sprintf(returnVal[0], "%d",  val.s);
        break;
    case CAFE_LONG:
        sprintf(returnVal[0], "%d", val.l);
        break;
    case CAFE_ENUM:
        sprintf(returnVal[0], "%u",  val.us);
        break;
    case CAFE_TYPENOTCONN:
        //std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__  << std::endl;
        //std::cout << " ERROR CAFE_TYPENOTCONN: dataType: " << dt  << " " << std::endl;
        sprintf(returnVal[0], "%s",  "0"); //CAFE_TYPENOTCONN");
        break;
    case CAFE_NO_ACCESS:
        //std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        //std::cout << " CAFE_NO_ACCESS: dataType: " << dt << " " << std::endl;
        sprintf(returnVal[0], "%s",  "0"); //"CAFE_NO_ACCESS");
        break;
    case CAFE_INVALID_DATATYPE:
        //std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        //std::cout<<  " CAFE_INVALID_DATATYPE: dataType: " << dt << " " << std::endl;
        sprintf(returnVal[0], "%s",  "0"); //"CAFE_INVALID_DATATYPE");
        break;
    case CAFE_NOT_REQUESTED:
        //std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        //std::cout<< " CAFE_NOT_REQUESTED: dataType: " << dt << " " << std::endl;
        sprintf(returnVal[0], "%s",   "0"); //"");
        break;
    case CAFE_NOT_SHOWN:
        //std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        //std::cout<< " CAFE_NOT_SHOWN: dataType: " << dt << " " << std::endl;
        sprintf(returnVal[0], "%s",  "0"); // "CAFE_NOT_SHOWN");
        break;
    default:
        //std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        //std::cout<< " CAFE INTERNAL ERROR: Unknown dataType: "<< dt << " " << std::endl;
        sprintf(returnVal[0], "%s",  "0");// "Unknown dataType");
        break;

    }

    return (CTYPE *) returnVal;

#undef __METHOD__
};



#endif
