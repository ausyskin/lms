/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2010-2023 Intel Corporation
 */
/*++

@file: GetImageTypeCommand.cpp

--*/

#include "GetImageTypeCommand.h"

namespace Intel {
	namespace MEI_Client {
		namespace MKHI_Client {
			GetImageTypeCommand::GetImageTypeCommand()
			{
				m_request = std::make_shared<GetImageTypeRequest>();
				Transact();
			}

			MKHI_IMAGE_TYPE GetImageTypeCommand::getResponse()
			{
				return m_response.getResponse();
			}

			void GetImageTypeCommand::parseResponse(const std::vector<uint8_t>& buffer)
			{
				m_response = MKHICommandResponse<MKHI_IMAGE_TYPE>(buffer, RESPONSE_COMMAND_NUMBER, MKHI_GEN_GROUP_ID);
			}
		} // namespace MKHI_Client
	} // namespace MEI_Client
} // namespace Intel