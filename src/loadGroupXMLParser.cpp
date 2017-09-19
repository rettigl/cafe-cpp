///
/// \file    loadGroupXMLParser.cc
/// \author  Jan Chrin, G. Prekas, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///



#include "loadGroupXMLParser.h"
#if HAVE_LIBQTXML 
 
#include <stdio.h>
#include <stdlib.h>

const QString&loadGroupXMLParser::tagCollection_list = "config";
const QString&loadGroupXMLParser::tagGroup = "group";
const QString&loadGroupXMLParser::tagDescription = "description";
const QString&loadGroupXMLParser::tagStatusGroup = "statusGroup";
const QString&loadGroupXMLParser::tagMember = "member";
const QString&loadGroupXMLParser::tagName = "name";
const QString&loadGroupXMLParser::tagNelem = "nelem";
const QString&loadGroupXMLParser::tagStatus = "status";
const QString&loadGroupXMLParser::tagRule = "rule";
const QString&loadGroupXMLParser::tagDataType = "datatype";
const QString&loadGroupXMLParser::tagCollection = "collection";
const QString&loadGroupXMLParser::tagId = "id";
const QString&loadGroupXMLParser::tagAttrib = "attribute";
const QString&loadGroupXMLParser::tagCollectiveType = "collectivetype";

loadGroupXMLParser::loadGroupXMLParser() {
}

loadGroupXMLParser::~loadGroupXMLParser() {
}

bool loadGroupXMLParser::startElement(const QString& namespaceURI, const QString& localName, 
				      const QString& qName, const QXmlAttributes& atts) {

    bool error = false;

    if (localName.compare(tagCollection_list, Qt::CaseInsensitive) == 0) {

    }else if (localName.compare(tagGroup, Qt::CaseInsensitive) == 0) {
        group = deviceGroup();
        group.id = atts.value("id").toAscii().constData();

    } else if (localName.compare(tagDescription, Qt::CaseInsensitive) == 0) {
        state = WaitingForDescription;
    } else if (localName.compare(tagStatusGroup, Qt::CaseInsensitive) == 0) {
        state = WaitingForStatusGroup;
    } else if (localName.compare(tagMember, Qt::CaseInsensitive) == 0) {
        state = WaitingForMember;
    } else if (localName.compare(tagName, Qt::CaseInsensitive) == 0) {
        state = WaitingForName;
    } else if (localName.compare(tagNelem, Qt::CaseInsensitive) == 0) {
        state = WaitingForNelem;
    } else if (localName.compare(tagStatus, Qt::CaseInsensitive) == 0) {
        state = WaitingForStatus;
    } else if (localName.compare(tagRule, Qt::CaseInsensitive) == 0) {
        state = WaitingForRule;
    } else if (localName.compare(tagDataType, Qt::CaseInsensitive) == 0) {
        state = WaitingForDataType;
    } else if (localName.compare(tagCollection, Qt::CaseInsensitive) == 0) {
        collection = collectionInGroup();
    } else if (localName.compare(tagId, Qt::CaseInsensitive) == 0) {
        state = WaitingForId;
    } else if (localName.compare(tagAttrib, Qt::CaseInsensitive) == 0) {
        state = WaitingForAttrib;
    } else if (localName.compare(tagCollectiveType, Qt::CaseInsensitive) == 0) {
        state = WaitingForCollectiveType;
    } else {
        error = true;

    }
    return !error;
}

bool loadGroupXMLParser::endElement(const QString& namespaceURI, 
const QString& localName, const QString& qName) {
    if (localName.compare(tagGroup, Qt::CaseInsensitive) == 0) {
        groups.push_back(group);
    } else if (localName.compare(tagMember, Qt::CaseInsensitive) == 0) {
        group.xmlMembers.push_back(xmlMem);
    } else if (localName.compare(tagCollection, Qt::CaseInsensitive) == 0) {
        group.collections.push_back(collection);
    }
    return true;
}

bool loadGroupXMLParser::characters(const QString& ch) {
    bool error = false;

    std::string data = ch.trimmed().toAscii().constData();

    switch (state) {
	case WaitingForDescription:
            group.description = data;
            break;
	case WaitingForStatusGroup:
  case WaitingForMember:
            break;
						
	case WaitingForName:
            xmlMem=data;
            break;
	case WaitingForNelem:

            break;
	case WaitingForStatus:

            break;
	case WaitingForRule:

            break;
	case WaitingForDataType:

            break;
	case WaitingForId:
            collection.id = data;
            break;
	case WaitingForAttrib:
            collection.attrib = data;
            break;
	case WaitingForCollectiveType:              
            break;
	default:
            error = true;
            printf("Unexpected state in loadGroupXMLParser::characters: '%s'\n", data.c_str());
            break;
	}
	return !error;
}

#endif
