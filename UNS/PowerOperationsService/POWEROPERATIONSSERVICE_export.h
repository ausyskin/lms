/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2013-2015 Intel Corporation
 */
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl POWEROPERATIONSSERVICE
// ------------------------------
#ifndef POWEROPERATIONSSERVICE_EXPORT_H
#define POWEROPERATIONSSERVICE_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (POWEROPERATIONSSERVICE_HAS_DLL)
#  define POWEROPERATIONSSERVICE_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && POWEROPERATIONSSERVICE_HAS_DLL */

#if !defined (POWEROPERATIONSSERVICE_HAS_DLL)
#  define POWEROPERATIONSSERVICE_HAS_DLL 1
#endif /* ! POWEROPERATIONSSERVICE_HAS_DLL */

#if defined (POWEROPERATIONSSERVICE_HAS_DLL) && (POWEROPERATIONSSERVICE_HAS_DLL == 1)
#  if defined (POWEROPERATIONSSERVICE_BUILD_DLL)
#    define POWEROPERATIONSSERVICE_Export ACE_Proper_Export_Flag
#    define POWEROPERATIONSSERVICE_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define POWEROPERATIONSSERVICE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* POWEROPERATIONSSERVICE_BUILD_DLL */
#    define POWEROPERATIONSSERVICE_Export ACE_Proper_Import_Flag
#    define POWEROPERATIONSSERVICE_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define POWEROPERATIONSSERVICE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* POWEROPERATIONSSERVICE_BUILD_DLL */
#else /* POWEROPERATIONSSERVICE_HAS_DLL == 1 */
#  define POWEROPERATIONSSERVICE_Export
#  define POWEROPERATIONSSERVICE_SINGLETON_DECLARATION(T)
#  define POWEROPERATIONSSERVICE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* POWEROPERATIONSSERVICE_HAS_DLL == 1 */

// Set POWEROPERATIONSSERVICE_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (POWEROPERATIONSSERVICE_NTRACE)
#  if (ACE_NTRACE == 1)
#    define POWEROPERATIONSSERVICE_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define POWEROPERATIONSSERVICE_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !POWEROPERATIONSSERVICE_NTRACE */

#if (POWEROPERATIONSSERVICE_NTRACE == 1)
#  define POWEROPERATIONSSERVICE_TRACE(X)
#else /* (POWEROPERATIONSSERVICE_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define POWEROPERATIONSSERVICE_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (POWEROPERATIONSSERVICE_NTRACE == 1) */

#endif /* POWEROPERATIONSSERVICE_EXPORT_H */

// End of auto generated file.
