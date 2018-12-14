///
/// \file    helper.cpp
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0

//include "connect.h"
//include "conduit.h"
//#include "cafeConduitFriends.h"

#include "helper.h"


using namespace std;



/**
 *  \brief Removes leading and trailing blanks
 *  \param pv  input: process variable name
 *  \param pvStripped output: process variable name stripped of forward and trailing spaces
 */
void Helper::removeLeadingAndTrailingSpacesDbrString(const char * pv, char pvStripped[MAX_STRING_SIZE])
{
#define __METHOD__ "Helper::removeLeadingAndTrailingSpacesDbrString(const char * pv, char[MAX_STRING_SIZE])"
    // Remove leading and trailing blanks
    std::string pvS=pv;

    size_t found1  = pvS.find_first_not_of(" ");
    size_t found2  = pvS.find_last_not_of (" ");

    if (found1!=std::string::npos && found2 !=std::string::npos) {

        size_t found21 = std::min((int)((found2+1)-found1), (int) (MAX_STRING_SIZE-1));
        size_t length  = pvS.copy(pvStripped,found21,found1);

        pvStripped[length]='\0'; //required

    }
    else {
        std::strcpy(pvStripped,"");

    }

    return;
#undef __METHOD__
}


/**
 *  \brief Removes leading and trailing blanks
 *  \param pv  input: process variable name
 *  \param pvStripped output: process variable name stripped of forward and trailing spaces
 */
void Helper::removeLeadingAndTrailingSpacesPseudo(const char * pv, char pvStripped[PVGROUP_PSEUDO_SIZE])
{
#define __METHOD__ "Helper::removeLeadingAndTrailingSpacesPseudo(const char * pv, char[PVGROUP_PSEUDO_SIZE])"
    // Remove leading and trailing blanks
    std::string pvS=pv;

    size_t found1  = pvS.find_first_not_of(" ");
    size_t found2  = pvS.find_last_not_of (" ");

    if (found1!=std::string::npos && found2 !=std::string::npos) {

        size_t found21 = std::min((int)((found2+1)-found1), (int) (PVGROUP_PSEUDO_SIZE-1));
        size_t length  = pvS.copy(pvStripped,found21,found1);

        pvStripped[length]='\0'; //required

    }
    else {
        std::strcpy(pvStripped,"isEmpty");

    }

    return;
#undef __METHOD__
}


/**
 *  \brief Removes leading and trailing blanks
 *  \param pv  input: process variable name
 *  \param pvStripped output: process variable name stripped of forward and trailing spaces
 */
void Helper::removeLeadingAndTrailingSpaces(const char * pv, char pvStripped[PVNAME_SIZE])
{
#define __METHOD__ "Helper::removeLeadingAndTrailingSpaces(const char * pv, char[PVNAME_SIZE])"
    // Remove leading and trailing blanks
    std::string pvS=pv;

    size_t found1  = pvS.find_first_not_of(" ");
    size_t found2  = pvS.find_last_not_of (" ");

    if (found1!=std::string::npos && found2 !=std::string::npos) {

        size_t found21 = std::min((int)((found2+1)-found1), (int) (PVNAME_SIZE-1));
        size_t length  = pvS.copy(pvStripped,found21,found1);

        pvStripped[length]='\0'; //required

    }
    else {
        std::strcpy(pvStripped,"isEmpty");

    }

    return;
#undef __METHOD__
}


/**
 *  \brief Produces a unique identifier from the pvName, ca_client_context, group handle size \n
 *  for entry into hash table
 *  \param  pv  input: process variable name
 *  \param ccc  input: current context
 *  \param ghs  input: size of gs (group handle set)
 *  \return unique identifier
 */
unsigned int  Helper::convertToUniqueNumber(const char * pv, ca_client_context * ccc, unsigned int  ghs)
{
#define __METHOD__ "Helper::convertToUniqueNumber(const char * pv, ca_client_context * ccc, unsigned int  ghs)"

    std::string myString=pv;
    char s[12];

    sprintf(s,"%lu", (unsigned long) ccc);


    unsigned int  numberPV      = 0;
    unsigned int  numberContext =0 ;
    double dpow=0;
    unsigned int  ld=0;


    numberContext = atoi(s);

    unsigned int intValueIs=0;
    unsigned int iL=0;

    for (unsigned int  i=0; i< myString.size(); i++) {

        intValueIs= (unsigned int) myString[i];
        ld=1;

        if ( (intValueIs >47 && intValueIs < 58) ||
                (intValueIs >64 && intValueIs < 91) ||
                (intValueIs >97 && intValueIs < 123) ) {
            dpow=pow((float) 42, (int) iL%4);
            ld = static_cast<unsigned int>(dpow);
            ++iL;
        }
        else {
            iL=0;
        }

        numberPV += (intValueIs*ld*(i+1));
    }



    unsigned int  final=(numberPV+numberContext+((ghs+1)*10000+ghs));

    return final;
#undef __METHOD__
}

/**
 *  \brief Produces a unique identifier from the pvName and ca_client_context for entry into hash table
 *  \param  pv  input: process variable name
 *  \param ccc  input: current context
 *  \return unique identifier
 */
unsigned int  Helper::convertToUniqueNumber(const char * pv, ca_client_context * ccc)
{
#define __METHOD__ "Helper::convertToUniqueNumber(const char * pv, ca_client_context * ccc)"

    std::string myString=pv;
    char s[12];

    sprintf(s,"%lu", (unsigned long) ccc);

    unsigned int  numberPV      = 0;
    unsigned int  numberContext =0 ;
    double dpow=0;
    unsigned int  ld=0;

    numberContext = atoi(s);


    unsigned int intValueIs=0;
    unsigned int iL=0;

    for (unsigned int  i=0; i< myString.size(); i++) {

        intValueIs= (unsigned int) myString[i];
        ld=1;

        if ( (intValueIs >47 && intValueIs < 58) ||
                (intValueIs >64 && intValueIs < 91) ||
                (intValueIs >97 && intValueIs < 123) ) {
            dpow=pow((float)42, (int)iL%4);
            ld = static_cast<unsigned int>(dpow);
            ++iL;
        }
        else {
            iL=0;
        }

        numberPV += (intValueIs*ld*(i+1));

    }

    return (numberPV+numberContext);
#undef __METHOD__
}

/**
 *  \brief enum CAFENUM::DBR_TYPE {DBR_PRIMITIVE=333,DBR_STS,DBR_TIME,
 *                            DBR_GR,DBR_CTRL,DBR_NONE}; \n
 *  Extracts the CAFENUM::DBR_TYPE from the channel type
 *  \param _chtype  input: channel type
 *  \return CAFENUM::DBR_TYPE
 */
CAFENUM::DBR_TYPE Helper::convertToCAFEDbrTypeClass(const chtype _chtype) const
{
#define __METHOD__ "Helper::convertToCAFEDbrTypeClass(const chtype _chtype)"

    if (_chtype>=DBR_STRING && _chtype <= DBR_DOUBLE) {
        return CAFENUM::DBR_PRIMITIVE;
    }
    else if (_chtype>=DBR_STS_STRING && _chtype <= DBR_STS_DOUBLE) {
        return CAFENUM::DBR_STS;
    }
    else if (_chtype>=DBR_TIME_STRING && _chtype <= DBR_TIME_DOUBLE) {
        return CAFENUM::DBR_TIME;
    }
    else if (_chtype>=DBR_GR_STRING && _chtype <= DBR_GR_DOUBLE) {
        return CAFENUM::DBR_GR;
    }
    else if (_chtype>=DBR_CTRL_STRING && _chtype <= DBR_CTRL_DOUBLE) {
        return CAFENUM::DBR_CTRL;
    }
    else if (_chtype==DBR_PUT_ACKT || _chtype==DBR_PUT_ACKS) {
        return CAFENUM::DBR_PUT;
    }
    else if (_chtype==DBR_STSACK_STRING) {
        return CAFENUM::DBR_STSACK;
    }
    else if (_chtype==DBR_CLASS_NAME) {
        return CAFENUM::DBR_CLASS;
    }
    else {
        return CAFENUM::DBR_NONE;
    }

#undef __METHOD__
}

/**
 *  \brief Concatinates dbr_char_t(unsigned char) into a string
 *  \param inpChar input: Array of dbr_char_t data types
 *  \param nChar  input: size of array
 *  \return std:string The concatinated string
 */
std::string  Helper::concatToString(dbr_char_t * inpChar, unsigned int nChar)
{

    std::string psWF = "";

    for (unsigned int i=0; i<nChar; ++i) {
        if (inpChar[i] != '\0') {
            psWF.append(1, inpChar[i]);
        }
    }
    return psWF;
}
