///
/// \file    cafeXML.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef CAFEXML_H
#define CAFEXML_H

#include <config.h>

#if HAVE_LIBQTXML
int  loadCollectionsFromXML(const char * collectionsFile);
int  loadGroupsFromXML     (const char * groupsFile);
int  restoreFromXML(const char * snapshotFile);
#endif

int  snapshot2XML (PVGroup pg);
void openGroupXMLFile(string fileName);
void closeGroupXMLFile(string fileName);
int  group2XML (const char * grpName, string fileName);

#endif // CAFEXML_H
