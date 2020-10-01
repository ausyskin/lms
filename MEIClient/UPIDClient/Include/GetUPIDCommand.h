/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2020 Intel Corporation
 */
/*++

@file: GetUPIDCommand.h

--*/

#ifndef __GET_UPID_COMMAND_H__
#define __GET_UPID_COMMAND_H__

#include "UPIDCommand.h"
#include "MEIparser.h"


namespace Intel
{
	namespace MEI_Client
	{
		namespace UPID_Client
		{
			typedef enum
			{
				UPID_OEM_PLATFORM_ID_TYPE_NOT_SET = 0,
				UPID_OEM_PLATFORM_ID_TYPE_BINARY = 1,
				UPID_OEM_PLATFORM_ID_TYPE_PRINTABLE_STRING = 2,
			}   UPID_OEM_PLATFORM_ID_TYPE;

			typedef struct
			{
				uint32_t                           PlatformIdType; //UPID_OEM_PLATFORM_ID_TYPE
				uint8_t                            OEMPlatformId[UPID_LEN];
				uint8_t                            CSMEPlatformId[UPID_LEN];

				void parse(std::vector<uint8_t>::const_iterator& itr, const std::vector<uint8_t>::const_iterator &end)
				{
					parseData(PlatformIdType, itr, end);
					parseData(OEMPlatformId, itr, end);
					parseData(CSMEPlatformId, itr, end);
				}

			}  UPID_PLATFORM_ID_GET_Response;


			class GetUPIDRequest;
			class GetUPIDCommand : public UPIDCommand
			{
			public:

				GetUPIDCommand();
				virtual ~GetUPIDCommand() {}

				UPID_PLATFORM_ID_GET_Response getResponse();

			private:
				virtual void parseResponse(const std::vector<uint8_t>& buffer);

				std::shared_ptr<UPIDCommandResponse<UPID_PLATFORM_ID_GET_Response>> m_response;
			};


			class GetUPIDRequest : public UPIDRequest
			{
			public:
				GetUPIDRequest() {}
				virtual ~GetUPIDRequest() {}

			private:
				virtual uint8_t requestHeaderFeatureID()
				{
					return UPID_COMMAND_FEATURE_PLATFORM_ID;
				}
				virtual uint8_t requestHeaderCommandID()
				{
					return UPID_COMMAND_PLATFORM_ID_GET;
				}
				virtual uint16_t requestDataSize()
				{
					return 0;
				}
				virtual std::vector<uint8_t> SerializeData();
			};
		} // namespace UPID_Client
	} // namespace MEI_Client
} // namespace Intel

#endif //__GET_UPID_COMMAND_H__