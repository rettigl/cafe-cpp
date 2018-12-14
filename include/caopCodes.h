///
/// \file    caopCodes.h
/// \author  Jan Chrin, PSI
/// \date    Release: April 2015
/// \version CAFE 1.0.0
///

#ifndef CAOPCODES_H
#define CAOPCODES_H

#include <cadef.h>
#include <iostream>
#include <string>
#include <map>

/**
 *  Provides methods to convert between
 *  the CA_OP_xx and text equivalent
 */
class CAOPCodes {
    typedef std::map<int, std::string> mapIntString;
private:
    mapIntString   mapDataType;
    mapIntString::iterator pos;
public:
    CAOPCodes()
    {
        mapDataType.insert(std::make_pair((int) CA_OP_GET,   "CA_OP_GET" ));
        mapDataType.insert(std::make_pair((int) CA_OP_PUT,   "CA_OP_PUT" ));
        mapDataType.insert(std::make_pair((int) CA_OP_CREATE_CHANNEL, "CA_OP_CREATE_CHANNEL"  ));
        mapDataType.insert(std::make_pair((int) CA_OP_ADD_EVENT,      "CA_OP_ADD_EVENT"  ));
        mapDataType.insert(std::make_pair((int) CA_OP_CLEAR_EVENT,    "CA_OP_CLEAR_EVENT"  ));
        mapDataType.insert(std::make_pair((int) CA_OP_OTHER,          "CA_OP_OTHER"   ));
        mapDataType.insert(std::make_pair((int) CA_OP_CONN_UP,        "CA_OP_CONN_UP"   ));
        mapDataType.insert(std::make_pair((int) CA_OP_CONN_DOWN,      "CA_OP_CONN_DOWN"   ));
    };

    ~CAOPCodes() {};

    std::string message (int i)
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

    void show()
    {
        print();
    }

    void print ( )
    {
        std::cout << "------------------"   << std::endl;
        std::cout << "CA_OP_LIST" << std::endl;
        std::cout << "-----------------"   << std::endl;
        for (pos=mapDataType.begin(); pos != mapDataType.end(); ++pos) {
            std::cout << pos->first << " " << pos->second << std::endl;
        }
        std::cout << "-----------------" << std::endl;
    };

};


#endif
