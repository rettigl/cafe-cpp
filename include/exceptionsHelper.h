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

class ExceptionsHelper {
private:
    CAFEDataTypeCode cafeDataTypeCode;
    CAFEStatus cafeStatus;
public:
    CAFEException_pv prepareCAFEException_pv(const char *pv, const char *pvAlias,
            unsigned int  handle, chid pCh, int  status,
            const char * source, unsigned int  ln);

    CAFEException_group prepareCAFEException_group(
        char groupName [PVNAME_SIZE],
        unsigned int    groupHandle,
        int             statusCode,
        const  char *   source,
        unsigned int    ln);

    ExceptionsHelper(void) {};
    ~ExceptionsHelper(void) {};
    void printCAFEException_pv(CAFEException_pv & e);


};
#endif // EXCEPTIONSHELPER_H
