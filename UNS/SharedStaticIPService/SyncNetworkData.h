/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2009-2018 Intel Corporation
 */
/*++

@file: SyncNetworkData.h

--*/

#ifndef SYNC_NETDATA_H
#define SYNC_NETDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

class SyncNetworkData
{
public:	
	SyncNetworkData();	
	bool getSharedStaticIpState(bool* state);	
	bool SyncNetworkConfiguration();
	bool SyncDNSData();
	bool CheckNetworkData(bool &need_sync, bool &isEmptyAddress, bool &IPv4Enabled);
	bool ValidateLinkStatus();
	void ResetSettings();
	~SyncNetworkData(void);

	std::string		m_MacAddress;
	bool		m_DHCPEnabled;
	bool		m_ValidationFailed;

private:		
	std::string		m_IpAddress;
	std::string		m_SubnetMask;
	std::string		m_DefaultGateway;
	std::string		m_PrimaryDNS;
	std::string		m_SecondaryDNS;
#ifdef WIN32
	unsigned int			m_NICindex;
#else
	int m_NICindex;
#endif // WIN32

};

#endif /*SYNC_NETDATA_H*/
