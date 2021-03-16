///
/// \file    tmDateMap.h
/// \author  Jan Chrin, PSI
/// \date    Release: August 2018
/// \version CAFE 1.7.0
///

#ifndef TMDATEMAP_H
#define TMDATEMAP_H

#include <string>
#include <map>

enum TM_WDAY
{
    TM_SUNDAY  = 0,
    TM_MONDAY  = 1,
    TM_TUESDAY = 2,
    TM_WEDNESDAY=3,
    TM_THURSDAY =4,
    TM_FRIDAY   =5,
    TM_SATURDAY =6
};


enum TM_MONTHP
{
    TM_JAN = 1,
    TM_FEB = 2,
    TM_MAR = 3,
    TM_APR = 4,
    TM_MAY = 5,
    TM_JUN = 6,
    TM_JUL = 7,
    TM_AUG = 8,
    TM_SEP = 9,
    TM_OCT =10,
    TM_NOV =11,
    TM_DEC =12
};


/**
 *  Provides methods to convert between
 *  the TM_WDAY and text equivalent
 */
class TMwdayText
{
    typedef std::map<int, std::string> mapLongString;
private:
    mapLongString   mapDataType;
    mapLongString::iterator pos;
public:
    TMwdayText()
    {
        mapDataType.insert(std::make_pair((int) TM_SUNDAY,       "Sun" ));
        mapDataType.insert(std::make_pair((int) TM_MONDAY,       "Mon" ));
        mapDataType.insert(std::make_pair((int) TM_TUESDAY,      "Tue" ));
        mapDataType.insert(std::make_pair((int) TM_WEDNESDAY,    "Wed" ));
        mapDataType.insert(std::make_pair((int) TM_THURSDAY,     "Thu" ));
        mapDataType.insert(std::make_pair((int) TM_FRIDAY,       "Fri" ));
        mapDataType.insert(std::make_pair((int) TM_SATURDAY,     "Sat" ));
    };

    ~TMwdayText() {};

    std::string message (int i)
    {
        pos = mapDataType.find(i);
        if (pos != mapDataType.end()) return    pos->second;
        return  "Day Unknown";
    };


    std::string asString (int i)
    {
        pos = mapDataType.find(i);
        if (pos != mapDataType.end()) return    pos->second;
        return  "Day Unknown";
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
        std::cout << "TM_WDAY LIST" << std::endl;
        std::cout << "------------------"   << std::endl;
        for (pos=mapDataType.begin(); pos != mapDataType.end(); ++pos)
        {
            std::cout << pos->first << " " << pos->second << std::endl;
        }
        std::cout << "-----------------" << std::endl;
    };

};





/**
 *  Provides methods to convert between
 *  the TM_WDAY and text equivalent
 */
class TMmonthpText
{
    typedef std::map<int, std::string> mapLongString;
private:
    mapLongString   mapDataType;
    mapLongString::iterator pos;
public:
    TMmonthpText()
    {
        mapDataType.insert(std::make_pair((int) TM_JAN,       "Jan" ));
        mapDataType.insert(std::make_pair((int) TM_FEB,       "Feb" ));
        mapDataType.insert(std::make_pair((int) TM_MAR,       "Mar" ));
        mapDataType.insert(std::make_pair((int) TM_APR,       "Apr" ));
        mapDataType.insert(std::make_pair((int) TM_MAY,       "May" ));
        mapDataType.insert(std::make_pair((int) TM_JUN,       "Jun" ));
        mapDataType.insert(std::make_pair((int) TM_JUL,       "Jul" ));
        mapDataType.insert(std::make_pair((int) TM_AUG,       "Aug" ));
        mapDataType.insert(std::make_pair((int) TM_SEP,       "Sep" ));
        mapDataType.insert(std::make_pair((int) TM_OCT,       "Oct" ));
        mapDataType.insert(std::make_pair((int) TM_NOV,       "Nov" ));
        mapDataType.insert(std::make_pair((int) TM_DEC,       "Dec" ));

    };

    ~TMmonthpText() {};

    std::string message (int i)
    {
        pos = mapDataType.find(i);
        if (pos != mapDataType.end()) return    pos->second;
        return  "Month Unknown";
    };


    std::string asString (int i)
    {
        pos = mapDataType.find(i);
        if (pos != mapDataType.end()) return    pos->second;
        return  "Month Unknown";
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
        std::cout << "TM_MONTH(+1) LIST" << std::endl;
        std::cout << "------------------"   << std::endl;
        for (pos=mapDataType.begin(); pos != mapDataType.end(); ++pos)
        {
            std::cout << pos->first << " " << pos->second << std::endl;
        }
        std::cout << "-----------------" << std::endl;
    };

};




#endif
