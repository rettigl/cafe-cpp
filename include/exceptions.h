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
#include <defines.h>
#include <cafeDataType.h>

using namespace std;

/**
 *  The CAFEException_pv struct for pv error reporting
 */
struct CAFEException_pv
{
    char            pv     [PVNAME_SIZE];
    char            pvAlias[PVNAME_SIZE]; 
    unsigned int    handle;
    CAFE_DATATYPE   dataTypeNative;
    const  char *   dataTypeNativeText;
    int             statusCode;
    const  char *   statusCodeText;
    const  char *   statusMessage;
    const  char *   source;
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
    const  char *   statusCodeText;
    const  char *   statusMessage;
    const  char *   source;
    unsigned int    ln;
}; 


/**
 *  The CAFEException_open class for ca open error reporting
 */
class CAFEException_open : public exception
{
    public:
    virtual const char* what() const throw()
    {
        return "CAFEException_open exception: Could not establish link to pv";
    };

    CAFEException_pv pvEx;
};

/**
 *  The CAFEException_groupOpen class for ca group open error reporting
 */
class CAFEException_groupOpen : public exception
{
    public:
    virtual const char* what() const throw()
    {
        return "CAFEException_groupOpen exception: Could not establish link to group";
    };
    
    CAFEException_group groupEx;
};


/**
 *  The CAFEException_init
 */
class CAFEException_init: public exception
{
    public:
    virtual const char* what() const throw()
    {
        return "CAFEException_init exception: \nChannel Access Error: ECA_ALLOCMEM when calling ca_context_create";
    };
};
		  

/**
 *  The CAFEException_allocBufferMem
 */
class CAFEException_allocBufferMem: public exception
{
    public:
    virtual const char* what() const throw()
    {
        return "CAFEException_allocBufferMem exception: \nCAFE ERROR: Cannot create space for pv data/ctrl buffer";
    };
};


#endif // EXCEPTION_H
