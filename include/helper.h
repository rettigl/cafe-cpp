///
/// \file    helper.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef HELPER_H
#define HELPER_H
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>


#include <cmath>
#include <cadef.h>
#include <cafeEnum.h>
#include <defines.h>

class Helper
{
public:
    Helper() {};

    void          removeLeadingAndTrailingSpacesDbrString(const char * _pv, char pvStripped[MAX_STRING_SIZE]);
    void          removeLeadingAndTrailingSpacesPseudo(const char * _pv, char pvStripped[PVGROUP_PSEUDO_SIZE]);
    void          removeLeadingAndTrailingSpaces(const char * _pv, char pvStripped[PVNAME_SIZE]);
    unsigned int  convertToUniqueNumber(const char * pv, ca_client_context * ccc, unsigned int  ghs);
    unsigned int  convertToUniqueNumber(const char * pv, ca_client_context * ccc);
    CAFENUM::DBR_TYPE convertToCAFEDbrTypeClass(const chtype _chtype) const;

    std::string   concatToString(dbr_char_t * inChar, unsigned int nChar);
};

#endif // HELPER_H
