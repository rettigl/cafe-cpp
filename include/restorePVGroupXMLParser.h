///
/// \file    restorePVGroupXMLParser.h
/// \author  Jan Chrin, G. Prekas, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#include <config.h>

#if HAVE_LIBQTXML

#ifndef RESTOREPVGROUPXMLPARSER_H
#define RESTOREPVGROUPXMLPARSER_H

#include <PVGroup.h>


#include <QtXml/QXmlDefaultHandler>


class restorePVGroupXMLParser : public QXmlDefaultHandler {
public:
    restorePVGroupXMLParser();
    virtual ~restorePVGroupXMLParser();

    bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts);
    bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName);
    bool characters(const QString& ch);

    PVGroup group;
private:
    enum {
        NotWaiting,
        WaitingForConfig,
        WaitingForNPV,
        WaitingForGroup,
        WaitingForDescription,
        WaitingForStatusGroup,
        WaitingForMember,
        WaitingForName,
        WaitingForNelem,
        WaitingForStatus,
        WaitingForRule,
        WaitingForVal,
        WaitingForSettable
    } state;


    PVDataHolder * pvd;
    bool settable;
    unsigned int  icount;

    const static QString& tagConfig;
    const static QString& tagGroup;
    const static QString& tagNPV;
    const static QString& tagDescription;
    const static QString& tagStatusGroup;
    const static QString& tagMember;
    const static QString& tagName;
    const static QString& tagNelem;
    const static QString& tagStatus;
    const static QString& tagRule;
    const static QString& tagVal;
    const static QString& tagSettable;

};



#endif /* RESTOREPVGROUPXMLPARSER_H */

#endif

