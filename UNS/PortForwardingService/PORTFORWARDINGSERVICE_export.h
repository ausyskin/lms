/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2013-2015 Intel Corporation
 */
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl PORTFORWARDINGSERVICE
// ------------------------------
#ifndef PORTFORWARDINGSERVICE_EXPORT_H
#define PORTFORWARDINGSERVICE_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (PORTFORWARDINGSERVICE_HAS_DLL)
#  define PORTFORWARDINGSERVICE_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && PORTFORWARDINGSERVICE_HAS_DLL */

#if !defined (PORTFORWARDINGSERVICE_HAS_DLL)
#  define PORTFORWARDINGSERVICE_HAS_DLL 1
#endif /* ! PORTFORWARDINGSERVICE_HAS_DLL */

#if defined (PORTFORWARDINGSERVICE_HAS_DLL) && (PORTFORWARDINGSERVICE_HAS_DLL == 1)
#  if defined (PORTFORWARDINGSERVICE_BUILD_DLL)
#    define PORTFORWARDINGSERVICE_Export ACE_Proper_Export_Flag
#    define PORTFORWARDINGSERVICE_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define PORTFORWARDINGSERVICE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* PORTFORWARDINGSERVICE_BUILD_DLL */
#    define PORTFORWARDINGSERVICE_Export ACE_Proper_Import_Flag
#    define PORTFORWARDINGSERVICE_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define PORTFORWARDINGSERVICE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* PORTFORWARDINGSERVICE_BUILD_DLL */
#else /* PORTFORWARDINGSERVICE_HAS_DLL == 1 */
#  define PORTFORWARDINGSERVICE_Export
#  define PORTFORWARDINGSERVICE_SINGLETON_DECLARATION(T)
#  define PORTFORWARDINGSERVICE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* PORTFORWARDINGSERVICE_HAS_DLL == 1 */

// Set PORTFORWARDINGSERVICE_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (PORTFORWARDINGSERVICE_NTRACE)
#  if (ACE_NTRACE == 1)
#    define PORTFORWARDINGSERVICE_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define PORTFORWARDINGSERVICE_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !PORTFORWARDINGSERVICE_NTRACE */

#if (PORTFORWARDINGSERVICE_NTRACE == 1)
#  define PORTFORWARDINGSERVICE_TRACE(X)
#else /* (PORTFORWARDINGSERVICE_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define PORTFORWARDINGSERVICE_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (PORTFORWARDINGSERVICE_NTRACE == 1) */

#endif /* PORTFORWARDINGSERVICE_EXPORT_H */

// End of auto generated file.
