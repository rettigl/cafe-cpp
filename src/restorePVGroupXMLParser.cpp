///
/// \file    restorePVGroupXMLParser.cpp
/// \author  Jan Chrin,  PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///



#include <restorePVGroupXMLParser.h>


#if HAVE_LIBQTXML


using namespace std;

std::vector<std::string> SplitString(const char *str, char c)
{
    vector<string> result;
    do
    {
        const char *begin = str;
        while(*str != c && *str)
        {
            str++;
        }
        result.push_back(string(begin, str));
    }
    while (0 != *str++);
    return result;
}


const QString&restorePVGroupXMLParser::tagConfig = "config";
const QString&restorePVGroupXMLParser::tagGroup = "group";
const QString&restorePVGroupXMLParser::tagNPV = "npv";
const QString&restorePVGroupXMLParser::tagDescription = "description";
const QString&restorePVGroupXMLParser::tagStatusGroup = "statusGroup";
const QString&restorePVGroupXMLParser::tagMember = "member";
const QString&restorePVGroupXMLParser::tagName = "name";
const QString&restorePVGroupXMLParser::tagNelem = "nelem";
const QString&restorePVGroupXMLParser::tagStatus = "status";
const QString&restorePVGroupXMLParser::tagRule = "rule";
const QString&restorePVGroupXMLParser::tagVal = "val";
const QString&restorePVGroupXMLParser::tagSettable = "settable";

restorePVGroupXMLParser::restorePVGroupXMLParser()
{
    icount=0;
}

restorePVGroupXMLParser::~restorePVGroupXMLParser()
{
}



bool restorePVGroupXMLParser::startElement(const QString& namespaceURI, const QString& localName,
        const QString& qName, const QXmlAttributes& atts)
{


    bool error = false;

    if (localName.compare(tagConfig, Qt::CaseInsensitive) == 0)
    {

        state = WaitingForConfig;
    }
    else if (localName.compare(tagGroup, Qt::CaseInsensitive) == 0)
    {
        group = PVGroup();
        group.setName ( atts.value("id").toLatin1().constData() );
        pvd = new PVDataHolder[500]; //read in npv
        state = WaitingForGroup;
    }
    else if (localName.compare(tagNPV, Qt::CaseInsensitive) == 0)
    {
        state = WaitingForNPV;
    }
    else if (localName.compare(tagDescription, Qt::CaseInsensitive) == 0)
    {
        state = WaitingForDescription;
    }
    else if (localName.compare(tagStatusGroup, Qt::CaseInsensitive) == 0)
    {
        state = WaitingForStatusGroup;
    }
    else if (localName.compare(tagMember, Qt::CaseInsensitive) == 0)
    {
        state = WaitingForMember;
    }
    else if (localName.compare(tagName, Qt::CaseInsensitive) == 0)
    {
        state = WaitingForName;
    }
    else if (localName.compare(tagNelem, Qt::CaseInsensitive) == 0)
    {
        state = WaitingForNelem;
    }
    else if (localName.compare(tagStatus, Qt::CaseInsensitive) == 0)
    {
        state = WaitingForStatus;
    }
    else if (localName.compare(tagRule, Qt::CaseInsensitive) == 0)
    {
        state = WaitingForRule;
    }
    else if (localName.compare(tagVal, Qt::CaseInsensitive) == 0)
    {

        state = WaitingForVal;
    }
    else if (localName.compare(tagSettable, Qt::CaseInsensitive) == 0)
    {
        state = WaitingForSettable;
    }
    else
    {
        error = true;
    }
    return !error;
}

bool restorePVGroupXMLParser::endElement(const QString& namespaceURI,
        const QString& localName, const QString& qName)
{
    if (localName.compare(tagGroup, Qt::CaseInsensitive) == 0)
    {
        group.setPVData(pvd);
        group.npv=icount;


    }
    else if (localName.compare(tagMember, Qt::CaseInsensitive) == 0)
    {
        if(settable)++icount;
    }
    else if (localName.compare(tagNPV, Qt::CaseInsensitive) == 0)
    {


    }

    return true;
}




bool restorePVGroupXMLParser::characters(const QString& ch)
{

    bool error = false;

    std::string data = ch.trimmed().toLatin1().constData();


    switch (state)
    {
    case WaitingForDescription:

    case WaitingForGroup:
    case WaitingForConfig:
    case WaitingForStatusGroup:

    case WaitingForMember:

        break;
    case WaitingForNPV:

        break;
    case WaitingForName:

        strcpy(pvd[icount].pv,data.c_str());
        break;
    case WaitingForNelem:
        pvd[icount].setNelem ( strtol(data.c_str(), NULL, 10) );
        break;
    case WaitingForStatus:

        break;
    case WaitingForRule:

        break;
    case WaitingForVal:

        //if elements > 1, then break up string
        //read no of elements and break up with space as deliminater!
        if ( pvd[icount].getNelem() >1)
        {
            //parse string

            vector<string> v;
            v.clear();
            v.reserve(pvd[icount].getNelem());
            v = SplitString(data.c_str(), ' ');
            dbr_string_t * arr = new dbr_string_t[ pvd[icount].getNelem()];

            for (size_t i=0; i<v.size(); ++i)
            {
                strcpy(arr[i], v[i].c_str());
            }

            pvd[icount].set(arr);
        }
        else
        {
            pvd[icount].set(data);
        }
        break;
    case WaitingForSettable:
        if ( strcmp(data.c_str(),"true")==0)
        {
            settable = true;
        }
        else
        {
            settable=false;
        }
        break;

    default:
        error = true;
        printf("Unexpected: '%s'\n", data.c_str());
        break;
    }
    return !error;
}

#endif
