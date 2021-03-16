///
/// \file    exceptionsHelper.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef EXCEPTIONSHELPER_H
#define EXCEPTIONSHELPER_H

#include <exceptions.h>
#include <statusCodes.h>
#include <defines.h>

class ExceptionsHelper
{
private:
    CAFEDataTypeCode cafeDataTypeCode;
    CAFEStatus cafeStatus;
public:
    CAFEException_pv prepareCAFEException_pv(
            const char pv[PVNAME_SIZE], const char pvAlias[PVNAME_SIZE],
            unsigned int  handle, chid pCh, int  status,
	    std::string source, unsigned int ln);

    CAFEException_group prepareCAFEException_group(
        const char groupName [PVNAME_SIZE],
        unsigned int    groupHandle,
        int             statusCode,
        std::string   source,
        unsigned int    ln);

    ExceptionsHelper(void) {};
    ~ExceptionsHelper(void) {};
    void printCAFEException_pv(CAFEException_pv & e);


};
#endif // EXCEPTIONSHELPER_H
