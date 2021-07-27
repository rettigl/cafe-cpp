///
/// \file    instantT.h
/// \author  Jan Chrin, PSI
/// \date    Release: July 2021
/// \version CAFE 1.14.2
///


#ifndef INSTANT_T_H
#define INSTANT_T_H

#include <instant.h>

template <class CTYPE> int  Instant<CTYPE>::set(const unsigned int  _handle, 
        const chtype _dbrType, const CTYPE * _val );

template <class CTYPE> int  Instant<CTYPE>::get(const unsigned int  _handle,
        const chtype _dbrType,  CTYPE * _val,
	dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);

template <class CTYPE> int  Instant<CTYPE>::getCache(const unsigned int  _handle,
const chtype _dbrType,  CTYPE * _val,
						     dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);

template <class CTYPE> int  Instant<CTYPE>::getCache(const unsigned int *handleArray, const unsigned int  nelem,
const chtype _dbrType,  CTYPE * val, int  *statusArray,
						     dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts);

template <class CTYPE> int  Instant<CTYPE>::set(const unsigned int  *handleArray,  const unsigned int  nelem,
						onst chtype _dbrType,  const  CTYPE * val, int  *statusArray);

template <class CTYPE> int  Instant<CTYPE>::clientRequests(
 const unsigned int  _handle,
    const chtype _dbrType,  CTYPE * _val,
 dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts, bool isCacheRequest);


template <class CTYPE> int  Instant<CTYPE>::clientRequests(
							   const unsigned int  _handle, const chtype _dbrType,  const CTYPE * _val);


template <class CTYPE> int  Instant<CTYPE>::setAndGet(const unsigned int handleSet, const chtype dbrType, CTYPE valSet, CTYPE &valGet);
template <class CTYPE> int  Instant<CTYPE>::setMany(std::vector<unsigned int> handleSet, const chtype dbrType, std::vector<CTYPE> valSet, bool printFlag);
template <class CTYPE> int  Instant<CTYPE>::compareAndMatchMany(std::vector<unsigned int> handleSet, const chtype dbrType, std::vector<CTYPE> valSet, std::vector<unsigned int> handleMatch,
 CTYPE tolerance, double timeout, bool printFlag);

//template <class CTYPE> int  Instant<CTYPE>::setTriggerAndMatchMany(std::vector<unsigned int> handleSet, chtype dbrType, std::vector<CTYPE> valSet,
template <class CTYPE> int  Instant<CTYPE>::setAndMatchMany(std::vector<unsigned int> handleSet, chtype dbrType, std::vector<CTYPE> valSet, std::vector<unsigned int> handleMatch,
CTYPE tolerance, double timeout, bool printFlag);

template <class CTYPE> int  Instant<CTYPE>::setAndMatch(const unsigned int handleSet, const chtype dbrType, CTYPE valSet, const unsigned int handleMatch,
							CTYPE tolerance, double timeout, bool printFlag);

template <class CTYPE> int  Instant<CTYPE>::match(const chtype dbrType, CTYPE valSet, unsigned int handleMatch,
						  CTYPE tolerance, double timeout, bool printFlag);


template <class CTYPE> int  Instant<CTYPE>::matchMany(const chtype dbrType, std::vector<CTYPE> valSet, std::vector<unsigned int> handleMatch, CTYPE tolerance, double timeout, bool printFlag);


#endif
