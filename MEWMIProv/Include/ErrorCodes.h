/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2009-2015 Intel Corporation
 */
/*++

@file: ErrorCodes.cpp

--*/

#ifndef __ERRORCODES_H
#define __ERRORCODES_H

#include <winerror.h>

// HECI/MEI FACILITY
const unsigned long FACILITY_MEI = 0x8086;

// PT/ME/AMT Status Faclity
const unsigned long FACILITY_ME_STATUS = 0x8087;

const unsigned long WMI_ERRORCODE_BASE = 0x3000;

const HRESULT WMI_E_HECI_CONNECTION = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_MEI, WMI_ERRORCODE_BASE + 1);
const HRESULT WMI_E_PTHI_CLIENT_CONNECTION = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_MEI, WMI_ERRORCODE_BASE + 2);
const HRESULT WMI_E_FWUPD_CLIENT_CONNECTION = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_MEI, WMI_ERRORCODE_BASE + 3);

const HRESULT WMI_E_MESTATUS_BASE = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ME_STATUS, WMI_ERRORCODE_BASE);



#endif __ERRORCODES_H

