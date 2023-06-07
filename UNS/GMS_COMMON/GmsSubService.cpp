/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2010-2022 Intel Corporation
 */

#include "GmsSubService.h"
#include <ace/Get_Opt.h>
#include "Tools.h"

#include <sstream>

int GmsSubService::init (int argc, ACE_TCHAR *argv[])
{
	initSubService(argc,argv);
	startSubService();
	return 0;
}

int GmsSubService::initSubService(int argc, ACE_TCHAR *argv[])
{
	FuncEntryExit<void> fee(this, L"initSubService");
	static const ACE_TCHAR options[] = ACE_TEXT (":g:");
	ACE_Get_Opt cmd_opts(argc, argv, options, 0);
	int option;
	while ((option = cmd_opts()) != EOF)
	{
		switch (option)
		{
		case 'g':
		{
			unsigned long long pointer = ACE_OS::strtoull(cmd_opts.opt_arg(), NULL, 10);
			if (pointer == ULLONG_MAX || pointer == 0)
			{
				ACE_ERROR_RETURN
				((LM_ERROR, ACE_TEXT("-%c wrong argument\n"),
					cmd_opts.opt_opt()),
					-1);
			}
			m_mainService = (GmsService*)pointer;
		}
		break;
		case ':':
			ACE_ERROR_RETURN
			((LM_ERROR, ACE_TEXT("-%c requires an argument\n"),
				cmd_opts.opt_opt()), -1);
		default:
			ACE_ERROR_RETURN
			((LM_ERROR, ACE_TEXT("Parse error.\n")), -1);
		}
	}
	this->reactor(ACE_Reactor::instance());
	this->notifier_.reactor(this->reactor());
	this->notifier_.event_handler(this);
	this->msg_queue()->notification_strategy(&this->notifier_);
	m_serviceIsClosed=false;
	UNS_DEBUG(L"%s\n",name().c_str());
	return 0;
}

void GmsSubService::sendStatusChanged(SERVICE_STATUS_TYPE type)
{
	MessageBlockPtr mbPtr(new ACE_Message_Block(), deleteMessageBlockPtr);
	mbPtr->data_block(new ServiceStatus(name(), type));
	mbPtr->msg_type(MB_SERVICE_STATUS_CHANGED);
	m_mainService->sendMessage(GMS_CONFIGURATOR, mbPtr);
}

int GmsSubService::closeSubService()
{
	FuncEntryExit<void> fee(this, L"closeSubService");

	sendStatusChanged(SERVICE_STATUS_TYPE::UNLOADCOMPLETE);

	UNS_DEBUG(L"%s\n",name().c_str());
	return 0;
}

int GmsSubService::suspendSubService()
{
	sendStatusChanged(SERVICE_STATUS_TYPE::SUSPENDCOMPLETE);

	UNS_DEBUG(L"%s suspendSubService()\n",name().c_str());
	return 0;
}

int GmsSubService::startSubService()
{
	FuncEntryExit<void> fee(this, L"startSubService");

	sendStatusChanged(SERVICE_STATUS_TYPE::LOADCOMPLETE);

	UNS_DEBUG(L"%s, 0x%X\n",name().c_str(), this);
	return 0;
}

int GmsSubService::fini (void)
{
	UNS_DEBUG(L"%s service finalized\n",name().c_str());
	return 0;
}

int GmsSubService::suspend() 
{
	UNS_DEBUG(L"%s service suspended\n",name().c_str());
	return 0;
}

int GmsSubService::resume() 
{
	sendStatusChanged(SERVICE_STATUS_TYPE::RESUMECOMPLETE);

	UNS_DEBUG(L"%s service Resumed\n",name().c_str());
	return 0;
}

int GmsSubService::handle_output(ACE_HANDLE fd)
{
	FuncEntryExit<void> fee(this, L"handle_output");
	UNS_DEBUG(L"handle_output: %s\n",name().c_str());

	ACE_Message_Block *mb = 0;
	while (! this->msg_queue()->is_empty() && ShouldPass())
	{	
		/*int qcount = */this->getq(mb);
		MessageBlockPtr mbPtr(mb, deleteMessageBlockPtr);
		if (mb != 0)
		{
			int type=mbPtr->msg_type();
			HandleAceMessage(type, mbPtr);
		}
	}
	return true;
}

void GmsSubService::HandleAceMessage( int type, MessageBlockPtr &mbPtr )
{
	FuncEntryExit<decltype(type)> fee(this, L"GmsSubService::HandleAceMessage", type);

	switch (type) {
	case MB_STOP_SERVICE:
		{
			StopServiceDataBlock *event = dynamic_cast<StopServiceDataBlock*>(mbPtr->data_block());
			bool meiEnabled = false;
			if (event != nullptr)
				meiEnabled = event->m_meiEnabled;
			PreStop(type, meiEnabled);
			closeSubService();
			m_serviceIsClosed=true;
			break;
		}
	case MB_SUSPEND_SERVICE:
		{
			suspendSubService();
			break;
		}
	default:
		UNS_ERROR(L"%s Error: Unexpected message type: %d \n", name().c_str(), type);
		break;
	}
}

bool GmsSubService::sendAlertIndicationMessage(unsigned short category, unsigned long id,
	const ACE_TString &Message, const ACE_TString &MessageArgument)
{
	auto svc = theService::instance();
	if (svc == nullptr)
		return false;
	
	MessageBlockPtr mbPtr(new ACE_Message_Block(), deleteMessageBlockPtr);
	mbPtr->data_block(new GMS_AlertIndication(category, id, getDateTime(), ACTIVE_MESSAGEID, Message, MessageArgument));
	mbPtr->msg_type(MB_PUBLISH_EVENT);
	return svc->sendMessage(EVENT_MANAGER, mbPtr);
}