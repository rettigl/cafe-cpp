///
/// \file    loadCollectionXMLParser.cpp
/// \author  Jan Chrin, G. Prekas, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///



#include <loadCollectionXMLParser.h>
#if HAVE_LIBQTXML

const QString& loadCollectionXMLParser::tagConfig = "config";
const QString& loadCollectionXMLParser::tagGroup = "collection";
const QString& loadCollectionXMLParser::tagDescription = "description";
const QString& loadCollectionXMLParser::tagAttributes = "attributes";
const QString& loadCollectionXMLParser::tagAttribute = "attribute";
const QString& loadCollectionXMLParser::tagMember = "member";
const QString& loadCollectionXMLParser::tagDevice = "device";

loadCollectionXMLParser::loadCollectionXMLParser()
{
}

loadCollectionXMLParser::~loadCollectionXMLParser()
{
}

bool loadCollectionXMLParser::startElement(const QString& namespaceURI, const QString& localName,
        const QString& qName, const QXmlAttributes& atts)
{

    bool error = false;

    if (localName.compare(tagConfig, Qt::CaseInsensitive) == 0) {
    }
    else if (localName.compare(tagGroup, Qt::CaseInsensitive) == 0) {
        devCollection = deviceCollection();
        devCollection.name = atts.value("id").toAscii().constData();
        state=NotWaiting;
    }
    else if (localName.compare(tagDescription, Qt::CaseInsensitive) == 0) {
        state = WaitingForDescription;
    }
    else if (localName.compare(tagAttributes, Qt::CaseInsensitive) == 0) {
        state=NotWaiting;
    }
    else if (localName.compare(tagAttribute, Qt::CaseInsensitive) == 0) {
        state=WaitingForAttribute;
    }
    else if (localName.compare(tagMember, Qt::CaseInsensitive) == 0) {
        cMember=collectionMember();
        cMember.devicePosition = atts.value("pos").toFloat();
        state=NotWaiting;
    }
    else if (localName.compare(tagDevice, Qt::CaseInsensitive) == 0) {
        state = WaitingForDevice;
    }
    else {
        error = true;
    }
    return !error;
}

bool loadCollectionXMLParser::endElement(const QString& namespaceURI, const QString& localName,
        const QString& qName)
{
    if (localName.compare(tagGroup, Qt::CaseInsensitive) == 0) {
        deviceCollectionV.push_back(devCollection);
    }
    else if (localName.compare(tagMember, Qt::CaseInsensitive) == 0) {
        devCollection.cMembers.push_back(cMember);
    }
    else if (localName.compare(tagAttribute, Qt::CaseInsensitive) == 0) {
        devCollection.attributes.push_back(attributeName);
    }
    return true;
}

bool loadCollectionXMLParser::characters(const QString& ch)
{
    bool error = false;

    std::string data = ch.trimmed().toAscii().constData();;


    switch (state) {

    case WaitingForDevice:
        cMember.deviceName = data;
        //printf( "Waiting for Device %s \n", data.c_str());
        break;
    case WaitingForDescription:
        //printf( "Waiting for Description %s \n", data.c_str());
        devCollection.description = data;
        break;
    case WaitingForAttribute:
        //printf( "Waiting for Attribute %s \n", data.c_str());
        attributeName = data;
        break;
    case NotWaiting:
        break;
    default:
        error = true;
        printf("Unexpected state in loadCollectionXMLParser::characters : '%s'\n", data.c_str());
        break;
    }
    return !error;
}

#endif
