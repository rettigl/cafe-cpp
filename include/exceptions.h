///
/// \file    exceptions.h
/// \author  Jan Chrin, PSI
/// \date    November 2014
/// \version CAFE 1.0.0
///

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <cstring>
#include <iostream>
#include <defines.h>
#include <cafeDataType.h>

/**
 *  The CAFEException_pv struct for pv error reporting
 */
struct CAFEException_pv
{
    char            pv     [PVNAME_SIZE];
    char            pvAlias[PVNAME_SIZE];
    unsigned int    handle;
    CAFE_DATATYPE   dataTypeNative;
    std::string     dataTypeNativeText;
    int             statusCode;
    std::string     statusCodeText;
    std::string     statusMessage;
    std::string     source;
    unsigned int    ln;
};


/**
 *  The CAFEException_group struct for group error reporting
 */
struct CAFEException_group
{
    char            groupName [PVNAME_SIZE];
    unsigned int    groupHandle;
    int             statusCode;
    std::string     statusCodeText;
    std::string     statusMessage;
    std::string     source;
    unsigned int    ln;
};


/**
 *  The CAFEException_open class for ca open error reporting
 */
class CAFEException_open : public std::exception
{
private:
    std::string ewhat;  

public:
    CAFEException_open() {
        ewhat = "CAFEException_Open exception: Could not establish link to pv";
    };
    CAFEException_open(std::string _ewhat) {
        ewhat = _ewhat;
    };
    
    CAFEException_pv pvEx;  

    virtual const char* what() const throw()
    {
      return  ewhat.c_str();
    };

   
};

/**
 *  The CAFEException_groupOpen class for ca group open error reporting
 */
class CAFEException_groupOpen : public std::exception
{
private:
    std::string ewhat;  

public:
    CAFEException_groupOpen() {
        ewhat = "CAFEException_groupOpen exception: Could not establish link to group";
    };

    CAFEException_groupOpen(std::string _ewhat) {
        ewhat = _ewhat;
    };

    virtual const char* what() const throw()
    {
      return ewhat.c_str();
    };

    CAFEException_group groupEx;
};


/**
 *  The CAFEException_init
 */
class CAFEException_init: public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "CAFEException_init: \nChannel Access Error: ECA_ALLOCMEM when calling ca_context_create";
    };
};


/**
 *  The CAFEException_allocBufferMem
 */
class CAFEException_allocBufferMem: public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "CAFEException_allocBufferMem exception: \nCAFE ERROR: Cannot create space for pv data/ctrl buffer";
    };
};


#endif // EXCEPTION_H
