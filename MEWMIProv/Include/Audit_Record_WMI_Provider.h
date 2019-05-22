/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2009-2015 Intel Corporation
 */
/*++

@file: Audit_Record_WMI_Provider.h

--*/

#pragma once
#include "WMIInfrastructure.h"
#include "PTHI_Commands.h"

class Audit_Record_WMI_Provider
{
	
	
public:

	std::wstring InstanceID;
	UINT8       ProvisioningTLSMode;
	bool		SecureDNS;
	bool        HostInitiated;
	UINT32  	SelectedHashType;	
	vector<UINT8> SelectedHashData;
	vector<UINT8> CACertificateSerial1;
	vector<UINT8> CACertificateSerial2;
	vector<UINT8> CACertificateSerial3;
	bool        AdditionalCaSerialNums;
	bool		IsOemDefault;
	bool		IsTimeValid;
	std::wstring     ProvServerIP; 
	std::wstring    TlsStartTime;
	std::wstring		ProvServerFQDN;
			
	static HRESULT Enumerate(
		IWbemServices* pNamespace,
		IWbemContext __RPC_FAR *pCtx,
		IWbemObjectSink __RPC_FAR *pResponseHandler);

	static HRESULT GetAudit_Record(
		IWbemServices* pNamespace,
		const BSTR strObjectPath,
		IWbemContext __RPC_FAR *pCtx,
		IWbemObjectSink __RPC_FAR *pResponseHandler);
	static HRESULT EnumerateAuditRecord(
		vector<Audit_Record_WMI_Provider>& auditVec, 
		uint32& ReturnValue);

	~Audit_Record_WMI_Provider() {};

private: 
	Audit_Record_WMI_Provider(MEAdminAudit MEAudit, std::wstring instanceID);
	
};
