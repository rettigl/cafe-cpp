///
/// \file    methodCallbacks.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015; Updated: July 2019
/// \version CAFE 1.9.0
///
///          Added handlerGetDescription

#ifndef METHODCALLBACKS_H
#define METHODCALLBACKS_H

#include <config.h>

namespace CALLBACK_CAFE
{
#if HAVE_PYTHON_H
void PyHandlerPut(struct event_handler_args args);
void PyHandlerGet(struct event_handler_args args);
#endif
void handlerPulseID(struct event_handler_args args);
void handlerPut(struct event_handler_args args);
void handlerGet(struct event_handler_args args);
void handlerGetCtrl(struct event_handler_args args);
void handlerGetCtrlAtConnect(struct event_handler_args args); //Python option embedded
void handlerGetSTSACK(struct event_handler_args args);
void handlerGetClassName(struct event_handler_args args);


void handlerGetDescription(struct event_handler_args args);

};

#endif // METHODCALLBACKS_H
