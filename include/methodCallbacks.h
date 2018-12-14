///
/// \file    methodCallbacks.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef METHODCALLBACKS_H
#define METHODCALLBACKS_H

#include <config.h>

namespace CALLBACK_CAFE {
#if HAVE_PYTHON_H
void PyHandlerPut( struct event_handler_args args);
void PyHandlerGet( struct event_handler_args args);
#endif
void handlerPulseID( struct event_handler_args args) ;
void handlerPut( struct event_handler_args args) ;
void handlerGet( struct event_handler_args args) ;
void handlerGetCtrl( struct event_handler_args args) ;
void handlerGetSTSACK( struct event_handler_args args) ;
void handlerGetClassName( struct event_handler_args args) ;


};

#endif // METHODCALLBACKS_H
