/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2009-2024 Intel Corporation
 */
/*++

@file: heci.cpp

--*/

#include <ace/Log_Msg.h>
#include <vector>
#include <string>
#include <stdarg.h>
#include <sstream>
#include <chrono>
#include <thread>
#include <metee.h>
#include "heci.h"
#include "HECIException.h"

namespace Intel {
namespace MEI_Client {

	void HECI_Log(bool is_error, const char* fmt, ...)
	{
		const size_t DEBUG_MSG_LEN = 1024;
		char msg[DEBUG_MSG_LEN];
		va_list varl;
		va_start(varl, fmt);
		vsnprintf(msg, DEBUG_MSG_LEN, fmt, varl);
		va_end(varl);
		ACE_DEBUG(((is_error) ? LM_ERROR : LM_TRACE, ACE_TEXT("(%t)[%D][%-11M] %I %C"), msg));
	}

HECI::HECI(const GUID &guid, bool verbose) : _guid(guid), _initialized(false), _verbose(verbose), _bufSize(0), _handle(new _TEEHANDLE) {}

HECI::~HECI()
{
	Deinit();
}

void HECI::Init()
{
	TEESTATUS ret = TEE_INTERNAL_ERROR;
	bool client_not_found = false;

	if (_initialized)
		return;

#ifdef WIN32
	std::vector<struct tee_device_address> devices =
		{ { tee_device_address::TEE_DEVICE_TYPE_NONE, NULL } };
#else
	std::vector<struct tee_device_address> devices =
	{
		{ tee_device_address::TEE_DEVICE_TYPE_PATH, "/dev/mei0" },
		{ tee_device_address::TEE_DEVICE_TYPE_PATH, "/dev/mei1" },
		{ tee_device_address::TEE_DEVICE_TYPE_PATH, "/dev/mei2" },
		{ tee_device_address::TEE_DEVICE_TYPE_PATH, "/dev/mei3" },
	};
#endif // WIN32
	std::stringstream err;
	for (std::vector<struct tee_device_address>::const_iterator it = devices.begin();
	    it != devices.end(); it++) {
		ret = TeeInitFull(_handle.get(), &_guid, *it, TEE_LOG_LEVEL_VERBOSE, HECI_Log);
		if (!TEE_IS_SUCCESS(ret)) {
			err << ((it->data.path) ? it->data.path : "NULL") << " init " << ret << " ";
			continue;
		}

		const int HECI_MAX_CONNECT_RETRY = 3;
		int retry = 0;
		do {
			ret = TeeConnect(_handle.get());
			switch (ret) {
			case TEE_SUCCESS:
				_bufSize = _handle->maxMsgLen;
				_initialized = true;
				return;
			case TEE_UNABLE_TO_COMPLETE_OPERATION: /* windows return this error on busy */
			case TEE_BUSY:
				err << ((it->data.path) ? it->data.path : "NULL") << " connect " << ret << " ";
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				retry++;
				break;
			case TEE_CLIENT_NOT_FOUND:
				client_not_found = true;
				/*fallthrough*/
			default:
				err << ((it->data.path) ? it->data.path : "NULL") << " connect " << ret << " ";
				retry = HECI_MAX_CONNECT_RETRY; /* no need to retry */
				break;
			}
		} while (retry < HECI_MAX_CONNECT_RETRY);
		TeeDisconnect(_handle.get());
	}

	_initialized = false;

	if (client_not_found)
		throw HeciNoClientException("Client not found. " + err.str(), TEE_CLIENT_NOT_FOUND);
	else
		throw HECIException("Error in connecting to HECI. " + err.str(), ret);
}

void HECI::Deinit()
{
	if (_initialized) {
		TeeDisconnect(_handle.get());
		_initialized = false;
	}
}

size_t HECI::ReceiveHeciMessage(unsigned char *buffer, size_t len, unsigned long timeout)
{
	TEESTATUS ret;
	size_t numberOfBytesRead = 0;

	if (!_initialized)
		throw HECIException("Not initialized", TEE_INTERNAL_ERROR);

	ret = TeeRead(_handle.get(), buffer, len, &numberOfBytesRead, timeout);
	if (!TEE_IS_SUCCESS(ret))
		throw HECIException("Read failed", ret);

	return numberOfBytesRead;
}

size_t HECI::SendHeciMessage(const unsigned char *buffer, size_t len, unsigned long timeout)
{
	TEESTATUS ret;
	size_t numberOfBytesWritten = 0;

	if (!_initialized)
		throw HECIException("Not initialized", TEE_INTERNAL_ERROR);

#ifdef __linux__
	// Workaround for Linux kernels before 4.17
	// These kernels does not support select before write
	timeout = 0;
#endif // __linux__

	ret = TeeWrite(_handle.get(), buffer, len, &numberOfBytesWritten, timeout);
	if (!TEE_IS_SUCCESS(ret))
		throw HECIException("Write failed", ret);

	return numberOfBytesWritten;
}

void* HECI::GetHandle()
{
	if (!_initialized)
		throw HECIException("Not initialized", TEE_INTERNAL_ERROR);

	return reinterpret_cast<void*>(TeeGetDeviceHandle(_handle.get()));
}

void HECI::GetHeciDriverVersion(std::string& ver)
{
	if (!_initialized)
		throw HECIException("Not initialized", TEE_INTERNAL_ERROR);

	teeDriverVersion_t heciVersion = { 0 };
	TEESTATUS ret = GetDriverVersion(_handle.get(), &heciVersion);
	if (!TEE_IS_SUCCESS(ret))
		throw HECIException("GetDriverVersion failed", ret);

	std::stringstream ss;
	ss << heciVersion.major << "."
		<< heciVersion.minor << "."
		<< heciVersion.hotfix << "."
		<< heciVersion.build;
	ver = ss.str();
}

std::string heci_category_t::message(int ev) const {
#define TEE_ERR_STATE(state) case TEE_##state: return #state
	switch (ev) {
		TEE_ERR_STATE(SUCCESS);
		TEE_ERR_STATE(INTERNAL_ERROR);
		TEE_ERR_STATE(DEVICE_NOT_FOUND);
		TEE_ERR_STATE(DEVICE_NOT_READY);
		TEE_ERR_STATE(INVALID_PARAMETER);
		TEE_ERR_STATE(UNABLE_TO_COMPLETE_OPERATION);
		TEE_ERR_STATE(TIMEOUT);
		TEE_ERR_STATE(NOTSUPPORTED);
		TEE_ERR_STATE(CLIENT_NOT_FOUND);
		TEE_ERR_STATE(BUSY);
		TEE_ERR_STATE(DISCONNECTED);
		TEE_ERR_STATE(INSUFFICIENT_BUFFER);
	default:
		return std::to_string(ev);
	}
#undef TEE_ERR_STATE
}
}} // namespace
