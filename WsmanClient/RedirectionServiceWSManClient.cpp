/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2009-2019 Intel Corporation
 */
/*++

@file: RedirectionServiceWSManClient.cpp

--*/

#include "RedirectionServiceWSManClient.h"
#include "UNSDebug.h"
#include "WsmanClientCatch.h"

using namespace Intel::Manageability::Cim::Typed;
using namespace std;

const std::string RedirectionServiceWSManClient::DEFAULT_USER = "$$uns";
const std::string RedirectionServiceWSManClient::DEFAULT_PASS = "$$uns";

RedirectionServiceWSManClient::RedirectionServiceWSManClient(): BaseWSManClient(DEFAULT_USER, DEFAULT_PASS)
{
	m_isInit = false;
}

RedirectionServiceWSManClient::~RedirectionServiceWSManClient()
{
}

bool RedirectionServiceWSManClient::RedirectionState(unsigned short *state)
{
/*#ifdef _DEBUG
	if (GetFromRegistry("DebugData", "KVMRedirectionState", state) == true)
	{
		return true;
	}
#endif*/
	
	if (!Init(true))
		return false;
	if (m_service.EnabledStateExists())
	{
		*state = m_service.EnabledState();
		return true;
	}
	return false;
}

bool RedirectionServiceWSManClient::Init(bool forceGet)
{
	if (!forceGet && m_isInit)
	{
		return true;
	}
	m_isInit = false;
	
	try
	{
		if (!m_endpoint)
			SetEndpoint(false);
		//Lock WsMan to prevent reentry
		std::lock_guard<std::mutex> lock(WsManSemaphore());
		m_service.WsmanClient(m_client.get());
		m_service.Get();
		m_isInit = true;
	}
	CATCH_exception("RedirectionServiceWSManClient::Init")
	
	return m_isInit;
}