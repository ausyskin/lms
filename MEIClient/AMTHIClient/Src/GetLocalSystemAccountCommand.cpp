/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2010-2023 Intel Corporation
 */
/*++

@file: GetLocalSystemAccountCommand.cpp

--*/

#include "GetLocalSystemAccountCommand.h"

namespace Intel {
	namespace MEI_Client {
		namespace AMTHI_Client {
			GetLocalSystemAccountCommand::GetLocalSystemAccountCommand()
			{
				std::shared_ptr<MEICommandRequest> tmp(new GetLocalSystemAccountRequest());
				m_request = tmp;
				Transact();
			}

			GET_LOCAL_SYSTEM_ACCOUNT_RESPONSE GetLocalSystemAccountCommand::getResponse()
			{
				return m_response.getResponse();
			}

			void GetLocalSystemAccountCommand::parseResponse(const std::vector<uint8_t>& buffer)
			{
				m_response = AMTHICommandResponse<GET_LOCAL_SYSTEM_ACCOUNT_RESPONSE>(buffer, RESPONSE_COMMAND_NUMBER);
			}

			std::vector<uint8_t> GetLocalSystemAccountRequest::SerializeData()
			{
				std::vector<uint8_t> output(40, 0); //"Reserved"
				return output;
			}
		} // namespace AMTHI_Client
	} // namespace MEI_Client
} // namespace Intel