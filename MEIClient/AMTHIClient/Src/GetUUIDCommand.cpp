/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2010-2023 Intel Corporation
 */
/*++

@file: GetUUIDCommand.cpp

--*/

#include "GetUUIDCommand.h"

namespace Intel {
	namespace MEI_Client {
		namespace AMTHI_Client {
			GetUUIDCommand::GetUUIDCommand()
			{
				m_request = std::make_shared<GetUUIDRequest>();
				Transact();
			}

			GET_UUID_RESPONSE GetUUIDCommand::getResponse()
			{
				return m_response.getResponse();
			}

			void GetUUIDCommand::parseResponse(const std::vector<uint8_t>& buffer)
			{
				m_response = AMTHICommandResponse<GET_UUID_RESPONSE>(buffer, RESPONSE_COMMAND_NUMBER);
			}
		} // namespace AMTHI_Client
	} // namespace MEI_Client
} // namespace Intel