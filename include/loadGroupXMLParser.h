///
/// \file    loadGroupXMLParser.h
/// \author  Jan Chrin, G. Prekas, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#include <config.h>

#if HAVE_LIBQTXML

#ifndef LOADGROUPXMLPARSER_H
#define LOADGROUPXMLPARSER_H

#include "deviceCollection.h"

#include <QtXml/QXmlDefaultHandler>

class loadGroupXMLParser : public QXmlDefaultHandler {
public:
    loadGroupXMLParser();
    virtual ~loadGroupXMLParser();
    bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts);
    bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName);
    bool characters(const QString& ch);

    std::vector<deviceGroup> groups;
private:
    enum {
        NotWaiting,
        WaitingForDescription,
        WaitingForStatusGroup,
        WaitingForMember,
        WaitingForName,
        WaitingForNelem,
        WaitingForStatus,
        WaitingForRule,
        WaitingForDataType,
        WaitingForId,
        WaitingForAttrib,
        WaitingForCollectiveType
    } state;
    deviceGroup group;
    //collectionMember member;
    collectionInGroup collection;
    std::string xmlMem;

    const static QString& tagCollection_list;
    const static QString& tagGroup;
    const static QString& tagDescription;
    const static QString& tagStatusGroup;
    const static QString& tagMember;
    const static QString& tagName;
    const static QString& tagNelem;
    const static QString& tagStatus;
    const static QString& tagRule;
    const static QString& tagDataType;
    const static QString& tagCollection;
    const static QString& tagId;
    const static QString& tagAttrib;
    const static QString& tagCollectiveType;
};

#endif /* LOADGROUPXMLPARSER_H */

#endif
