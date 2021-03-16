///
/// \file    exceptionsHelper.cpp
/// \author  Jan Chrin, PSI
/// \date    Release, February 2015
/// \version CAFE 1.0.0
///

#include <exceptionsHelper.h>

using namespace std;

/**
 *  \brief Populates the CAFEException_pv struct; precedes throw(e)
 *  \param pv  input: process variable
 *  \param pvAlias input: process variable alias
 *  \param handle input: handle to reference object
 *  \param pCh input: channel identifier (chid)
 *  \param status input: the error/status code
 *  \param source input: method name
 *  \param ln input: line number of file from where the error originates
 *  \return struct CAFEException_pv
 */
CAFEException_pv ExceptionsHelper::prepareCAFEException_pv (
        const char pv[PVNAME_SIZE], const char pvAlias[PVNAME_SIZE],
        unsigned int  handle, chid pCh, int  status,
        string source, unsigned int  ln)
{
#define __METHOD__ "Connect::prepareCAFEException_pv"

    CAFEStatus cafeStatus;

    CAFEException_pv e;
    // handle, pv, pvAlias
    e.handle=handle;
    
    
    
    if (pv != NULL)
    {
        strcpy(e.pv, (char *) pv);
    }
    else
    {
        strcpy(e.pv, "");
    }
    if (pvAlias != NULL)
    {
        strcpy(e.pvAlias, (char *) pvAlias);
    }
    else
    {
        strcpy(e.pvAlias, "");
    }
    
    // native datatype
    if (pCh == NULL)
    {
        e.dataTypeNative = (CAFE_DATATYPE) CAFE_NO_ACCESS;
    }
    else if (status == ECAFE_RULE_FALSE || status == ICAFE_RULE_FALSE)
    {
        e.dataTypeNative = (CAFE_DATATYPE) CAFE_NOT_REQUESTED;
    }
    else
    {
        e.dataTypeNative = (CAFE_DATATYPE) dbf_type_to_DBR(ca_field_type(pCh));
    }
    e.dataTypeNativeText = cafeDataTypeCode.message(e.dataTypeNative).c_str();
    // status code, message, description
    e.statusCode     = status;

    std::cout << status << " " << (const char*) cafeStatus.csc.message(status).c_str() << std::endl;
    std::cout << status << " " << (const char*) cafeStatus.csi.message(status).c_str() << std::endl;
    e.statusCodeText = cafeStatus.csc.message(status);
    e.statusMessage  = cafeStatus.csi.message(status);

   
    // method and line no of error source
    e.source = source;
    e.ln = ln;
    return e;

#undef __METHOD__
}

/**
 *  \brief Prints CAFEException_pv to std out
 *  \param e input: struct CAFEException
 */
void ExceptionsHelper::printCAFEException_pv(CAFEException_pv & e)
{
#define __METHOD__ "ExceptionsHelper::printCAFEException_pv"

    cout << "------------------------------------" << endl;
    cout << __METHOD__ << endl;
    cout << "------------------------------------" << endl;
    cout << "Handle               : " << e.handle << endl;
    cout << "Process Variable (PV): " << e.pv << endl;
    if ( strcmp(e.pv,e.pvAlias) )
    {
        cout << "PV Alias             : " << e.pvAlias << endl;
    }
    cout << "PV Native Type       : " << e.dataTypeNative << " ("
         << e.dataTypeNativeText << ") " << endl;
    cout << "Status Code          : " << e.statusCode << endl;
    cout << "Status Message       : " << e.statusCodeText << endl;
    cout << "Error Description    : " << e.statusMessage << endl;
    cout << "Source Method/Line   : " << e.source << "/" << e.ln << endl;
    cout << "------------------------------------" << endl;
    return;

#undef __METHOD__
}


/**
 *  \brief Populates the CAFEException_group struct; precedes throw(e)
 *  \param groupName  input: name of group
 *  \param ghandle input: group handle to reference object
 *  \param status input: the error/status code
 *  \param source input: method name
 *  \param ln input: line number of file from where the error originates
 *  \return struct CAFEException_group
 */
CAFEException_group ExceptionsHelper::prepareCAFEException_group(const char groupName[PVNAME_SIZE],
        unsigned int  ghandle, int  status,
	std::string source, unsigned int ln)
{
#define __METHOD__ "Connect::prepareCAFEExceptionGroup"

    CAFEException_group e;
    // handle, pv, pvAlias
    e.groupHandle=ghandle;
    
    if (groupName != NULL)
    {
      strcpy(e.groupName, groupName);   
    }
    else
    {
      strcpy(e.groupName, "");     
    }
    
    e.statusCode     = status;
    e.statusCodeText = cafeStatus.csc.message(status);
    e.statusMessage  = cafeStatus.csi.message(status);
    // method and line no of error source
    e.source =  source;
    e.ln = ln;
    return e;

#undef __METHOD__
}
