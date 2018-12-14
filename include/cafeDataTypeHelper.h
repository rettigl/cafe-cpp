///
/// \file    cafeDataTypeHelper.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef CAFEDATATYPEHELPER_H
#define CAFEDATATYPEHELPER_H

#include <cadef.h>
#include <iostream>
#include <string>
#include <map>
#include <cafeDataType.h>

class CafeDataTypeHelper {
private:
    CAFE_DATATYPE_UNION cdu;
    CAFE_DATATYPE  dataType;

public:
    CafeDataTypeHelper(CAFE_DATATYPE_UNION _cdu, CAFE_DATATYPE _dataType)
    {
        cdu=_cdu;
        dataType=_dataType;
    };

    ~CafeDataTypeHelper() {};

    CAFEConvert<double> renderDouble;
    CAFEConvert<float>  renderFloat;
    CAFEConvert<short>  renderShort;
    //CAFEConvert<int>  renderLong;
    CAFEConvert<unsigned short> renderEnum;
    CAFEConvert<unsigned char>  renderUChar;
    CAFEConvert<dbr_string_t>   renderString;

    CAFEConvert<char> renderChar;
    //CAFEConvert<unsigned int>  renderULong;
    CAFEConvert<long long>          renderLongLong;
    CAFEConvert<unsigned long long> renderULongLong;

    CAFEConvert<int>           renderInt;
    CAFEConvert<unsigned int>  renderUInt;

    std::string getAsString()
    {
        return (std::string) renderString.getString(dataType, cdu)[0];
    };
    dbr_string_t * getAsDbr_string_t()
    {
        return (dbr_string_t *) (renderString.getString(dataType, cdu)[0]);
    };
    double getAsDouble()
    {
        return (double) renderDouble.get(dataType, cdu)[0];
    };
    float  getAsFloat()
    {
        return (float) renderFloat.get (dataType, cdu)[0];
    };
    short  getAsShort()
    {
        return (short) renderShort.get(dataType, cdu)[0];
    };
    dbr_enum_t getAsEnum()
    {
        return (dbr_enum_t) renderEnum.get(dataType, cdu)[0];
    };
    unsigned short getAsUShort()
    {
        return (unsigned short) renderEnum.get(dataType, cdu)[0];
    };
    int  getAsInt()
    {
        return (int) renderInt.get(dataType, cdu)[0];
    };
    unsigned int  getAsUInt()
    {
        return (int) renderUInt.get(dataType, cdu)[0];
    };
    char getAsChar()
    {
        return (char) renderChar.get(dataType, cdu)[0];
    };
    unsigned char getAsUChar()
    {
        return (unsigned char) renderUChar.get(dataType, cdu)[0];
    };
    long long  getAsLongLong()
    {
        return (long long) renderLongLong.get(dataType, cdu)[0];
    };
    unsigned int  getAsULongLong()
    {
        return (unsigned long long) renderULongLong.get(dataType, cdu)[0];
    };
};

#endif
