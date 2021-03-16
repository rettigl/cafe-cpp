///
/// \file    cafeDataType.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef CAFEDATATYPE_H
#define CAFEDATATYPE_H

#include <cadef.h>
#include <iostream>
#include <string>
#include <map>

enum CAFE_DATATYPE
{
    CAFE_TYPENOTCONN  = TYPENOTCONN, // -1 val returned by ca_field_type when channel not connected
    CAFE_STRING       = DBF_STRING,  // 0
    CAFE_SHORT        = DBF_SHORT,   // 1
    CAFE_INT          = DBF_INT,     // 1
    CAFE_FLOAT        = DBF_FLOAT,   // 2
    CAFE_ENUM         = DBF_ENUM,    // 3
    CAFE_USHORT       = DBF_ENUM,    // 3
    CAFE_CHAR         = DBF_CHAR,    // 4
    CAFE_LONG         = DBF_LONG,    // 5
    CAFE_DOUBLE       = DBF_DOUBLE,  // 6
    CAFE_NO_ACCESS    = DBF_NO_ACCESS, //7
    CAFE_INVALID_DATATYPE =   8,
    CAFE_NOT_REQUESTED    = 100,
    CAFE_NOT_SHOWN        = 101   // (in stop monitor)
};

/**
 *  Provides methods to convert between
 *  the CAFE_DATATYPES and text equivalent
 */
class CAFEDataTypeCode
{
    typedef std::map<int, std::string> mapLongString;
private:
    mapLongString   mapDataType;
    mapLongString::iterator pos;
public:
    CAFEDataTypeCode()
    {
        mapDataType.insert(std::make_pair((int) CAFE_TYPENOTCONN,       "CAFE_TYPENOTCONN" ));
        mapDataType.insert(std::make_pair((int) CAFE_STRING,            "DBF_STRING" ));
        mapDataType.insert(std::make_pair((int) CAFE_SHORT,             "DBF_SHORT"  ));
        mapDataType.insert(std::make_pair((int) CAFE_INT,               "DBF_SHORT"  ));
        mapDataType.insert(std::make_pair((int) CAFE_FLOAT,             "DBF_FLOAT"  ));
        mapDataType.insert(std::make_pair((int) CAFE_ENUM,              "DBF_ENUM"   ));
        mapDataType.insert(std::make_pair((int) CAFE_CHAR,              "DBF_CHAR"   ));
        mapDataType.insert(std::make_pair((int) CAFE_LONG,              "DBF_LONG"   ));
        mapDataType.insert(std::make_pair((int) CAFE_DOUBLE,            "DBF_DOUBLE" ));
        mapDataType.insert(std::make_pair((int) CAFE_NO_ACCESS,         "DBF_NO_ACCESS"   ));
        mapDataType.insert(std::make_pair((int) CAFE_INVALID_DATATYPE,  "CAFE_INVALID_DATATYPE"));
        mapDataType.insert(std::make_pair((int) CAFE_NOT_REQUESTED,     "CAFE_NOT_REQUESTED"));
        mapDataType.insert(std::make_pair((int) CAFE_NOT_SHOWN,         "CAFE_NOT_SHOWN (IN STOP MONITOR)"));
    };

    ~CAFEDataTypeCode() {};

    std::string message (int i)
    {
        pos = mapDataType.find(i);
        if (pos != mapDataType.end()) return    pos->second;
        return  "CAFE_DATATYPE_UNKNOWN";
    };


    std::string asString (int i)
    {
        pos = mapDataType.find(i);
        if (pos != mapDataType.end()) return    pos->second;
        return  "CAFE_DATATYPE_UNKNOWN";
    };

    int enumIs (std::string message)
    {
        for (pos=mapDataType.begin(); pos != mapDataType.end(); ++pos)
            if (pos->second==message) return pos->first;
        return  -1;
    };


    int asEnum (std::string message)
    {
        for (pos=mapDataType.begin(); pos != mapDataType.end(); ++pos)
            if (pos->second==message) return pos->first;
        return  -1;
    };

    void show()
    {
        print();
    }

    void print ( )
    {
        std::cout << "------------------"   << std::endl;
        std::cout << "CAFE_DATATYPE LIST"   << std::endl;
        std::cout << "------------------"   << std::endl;
        for (pos=mapDataType.begin(); pos != mapDataType.end(); ++pos)
        {
            std::cout << pos->first << " " << pos->second << std::endl;
        }
        std::cout << "------------------" << std::endl;
    };

};

/**
 *  A union of CAFE primitive datatypes
 */
union CAFE_DATATYPE_UNION
{
    dbr_string_t   str;
    dbr_short_t  s;
    dbr_float_t  f;
    dbr_enum_t  us;  //unsigned short us;
    dbr_char_t  ch;  //unsigned char  ch;
    dbr_long_t   l;  //int l;
    dbr_double_t d;
};

typedef CAFE_DATATYPE_UNION * CAFE_DATATYPE_UNION_SEQ;




#endif
