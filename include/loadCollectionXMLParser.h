///
/// \file    loadCollectionXMLParser.h
/// \author  Jan Chrin, George Prekas, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///


#include <config.h>

#if HAVE_LIBQTXML

#ifndef LOADCOLLECTIONXMLPARSER_H
#define LOADCOLLECTIONXMLPARSER_H

#include "deviceCollection.h"

#include <QtXml/QXmlDefaultHandler>


class loadCollectionXMLParser : public QXmlDefaultHandler {
public:
    loadCollectionXMLParser();
    virtual ~loadCollectionXMLParser();
    bool startElement(const QString& namespaceURI,
                      const QString& localName,
                      const QString& qName,
                      const QXmlAttributes& atts);
    bool endElement  (const QString& namespaceURI,
                      const QString& localName,
                      const QString& qName);
    bool characters  (const QString& ch);

    std::vector<deviceCollection> deviceCollectionV;


private:
    enum {
        NotWaiting,
        WaitingForDescription,
        WaitingForDevice,
        WaitingForAttribute
    } state;

    deviceCollection devCollection;
    collectionMember cMember;

    std::string      attributeName;
    static const QString& tagConfig;
    static const QString& tagGroup;
    static const QString& tagDescription;
    static const QString& tagAttributes;
    static const QString& tagAttribute;
    static const QString& tagMember;
    static const QString& tagDevice;
};

#endif


#endif // LOADCOLLECTIONXMLPARSER_H
