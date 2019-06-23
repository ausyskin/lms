/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2009-2019 Intel Corporation
 */
/*++

@file: EtherenetSettingsWSManClient.h

--*/

#ifndef _ETHERNET_SETTINGS_WSMAN_CLIENT_H
#define _ETHERNET_SETTINGS_WSMAN_CLIENT_H

#include "AMT_EthernetPortSettings.h"
#include "BaseWSManClient.h"
#include <string>

class WSMAN_DLL_API EthernetSettingsWSManClient : public BaseWSManClient
{
public:
	static const unsigned int ERROR_UNKNOWN_ERROR = 1000;

	EthernetSettingsWSManClient();
	EthernetSettingsWSManClient(const std::string &userName, const std::string &password);
	virtual ~EthernetSettingsWSManClient();
	unsigned int Enumerate(std::vector<std::shared_ptr<Intel::Manageability::Cim::Typed::AMT_EthernetPortSettings>> &EthernetSettings);
private:
	bool m_isInit;
	static const std::string DEFAULT_USER;
	static const std::string DEFAULT_PASS;
	LOCK_BEFORE;
	Intel::Manageability::Cim::Typed::AMT_EthernetPortSettings m_service;
	UNLOCK_AFTER;
};

#endif //_ETHERNET_SETTINGS_WSMAN_CLIENT_H