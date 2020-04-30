/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2009-2020 Intel Corporation
 */
/*++

@file: EthernetPortSettings_WMI_Provider.cpp

--*/

#include "EthernetPortSettings_WMI_Provider.h"
#include "pthi_commands.h"
#include "WSmanCommands.h"
#include "WMIHelper.h"



HRESULT EthernetPortSettings_WMI_Provider::Enumerate(
								IWbemServices* pNamespace,
								IWbemContext __RPC_FAR *pCtx,
								IWbemObjectSink __RPC_FAR *pResponseHandler)
{
	USES_CONVERSION; 

	//_Module.logger.Detail(File,LOCATION, _T("SCS Server"), _T("Start function"),_T(""));
	//_Module.logger.Info(File,LOCATION, _T("Profile data"), _T("Enumerate started"),_T(""));
	//Get all keys in a colllection, from an internal function
	uint32 ReturnValue = 0;
	uint32 hr=0;
	int i=1;
	try
	{
		
		vector<EthernetPortSettings_WMI_Provider> ethernetPortList;
		hr = EnumerateEthernetPortSettings(ethernetPortList, ReturnValue);
		if (STATUS_SUCCESS == ReturnValue)
		{
			vector<EthernetPortSettings_WMI_Provider>::iterator entry;
			entry = ethernetPortList.begin();
			for (; entry != ethernetPortList.end(); entry++)
			{
				CComPtr<IWbemClassObject> obj;
				RETURNIF(WMIPutMember(pNamespace, &obj, L"AMT_EthernetPortSettings"));
				BREAKIF(WMIPut<1>(obj, L"ElementName", entry->ElementName));
				BREAKIF(WMIPut<1>(obj, L"InstanceID", entry->InstanceID));
				BREAKIF(WMIPut<1>(obj, L"MACAddress", entry->MACAddress));
				BREAKIF(WMIPut<1>(obj, L"LinkIsUp", entry->LinkIsUp));
				BREAKIF(WMIPut<1>(obj, L"DHCPEnabled", entry->DHCPEnabled));
				
				BREAKIF(WMIPut<1>(obj, L"IPAddress", entry->IPAddress ));
				BREAKIF(WMIPut<1>(obj, L"SubnetMask",entry->SubnetMask));
				BREAKIF(WMIPut<1>(obj, L"DefaultGateway",entry->DefaultGateway));
				BREAKIF(WMIPut<1>(obj, L"PrimaryDNS",entry->PrimaryDNS ));
				BREAKIF(WMIPut<1>(obj, L"SecondaryDNS",entry->SecondaryDNS));
		
				BREAKIF(pResponseHandler->Indicate(1, &obj.p));
			}
		}
		else
		{
			//WMIHandleSetStatus(pNamespace,pResponseHandler, ReturnValue);
			WMIHelper::PTHIHandleSetStatus(pNamespace, pResponseHandler, ReturnValue, hr);
			return hr;
		}


	}
	catch (...)
	{
		//_Module.logger.Error(File,LOCATION, _T("SCS Server"), _T(""), _T("Bad catch"));
		hr  = WBEM_E_PROVIDER_FAILURE;
	}
	WMIHandleSetStatus(pNamespace,pResponseHandler, hr);
	//Enumerate the collection, retrieving params and creating return instances
	//if (STATUS_SUCCESS == hr)
	//{
	//	_Module.logger.Info(File,LOCATION,  _T("Profile data"), _T("Enumerate finished successfully"),_T(""));
	//}
	//else
	//{
	//	_Module.logger.Info(File,LOCATION,  _T("Profile data"), _T("Enumerate failed with error code"),_T("StringUtilsNamespace::convertTowString(hr)"));
	//}

	return hr;

}

HRESULT EthernetPortSettings_WMI_Provider::EnumerateEthernetPortSettings(vector<EthernetPortSettings_WMI_Provider>& settingsVec, uint32& ReturnValue)
{
	USES_CONVERSION; 

	//_Module.logger.Detail(File,LOCATION, _T("SCS Server"), _T("Start function"),_T(""));
	//_Module.logger.Info(File,LOCATION, _T("Profile data"), _T("Enumerate started"),_T(""));
	//Get all keys in a colllection, from an internal function
	ReturnValue = 0;

	uint32 hr=0;
	int i=1;
	try
	{
		vector<EthernetPortEntry> ethernetPortList;
		ReturnValue = GetPortList(ethernetPortList);
			if (STATUS_SUCCESS == ReturnValue)
			{	
				vector<EthernetPortEntry>::iterator entry;
				entry = ethernetPortList.begin();
				for (; entry != ethernetPortList.end(); entry++)
				{
					std::wstring elementName = L"Intel(r) AMT Ethernet Port Settings";
					WCHAR str[256];
					swprintf_s(str, 256, L"Intel(r) AMT Ethernet Port Settings %d",i++);
					std::wstring instanceID(str);
					EthernetPortSettings_WMI_Provider setting(*entry, instanceID, elementName);				
					settingsVec.push_back(setting);
				}
			}
	}
	catch (...)
	{
		//_Module.logger.Error(File,LOCATION, _T("SCS Server"), _T(""), _T("Bad catch"));
		hr  = WBEM_E_PROVIDER_FAILURE;
	}
	//Enumerate the collection, retrieving params and creating return instances
	//if (STATUS_SUCCESS == hr)
	//{
	//	_Module.logger.Info(File,LOCATION,  _T("Profile data"), _T("Enumerate finished successfully"),_T(""));
	//}
	//else
	//{
	//	_Module.logger.Info(File,LOCATION,  _T("Profile data"), _T("Enumerate failed with error code"),_T("StringUtilsNamespace::convertTowString(hr)"));
	//}

	return hr;
}

uint32 EthernetPortSettings_WMI_Provider::GetPortList(vector<EthernetPortEntry>& ethernetPortList)
{
	uint32 ReturnValue= 1; 
	PTHI_Commands pthic;
	vector<EthernetPortEntryWSMan> ethernetPortWSManList;
	WSmanCommands wsmc;

	// The wsman call requires adminSecurity credentials - Try and get admin account for this
	LOCAL_SYSTEM_ACCOUNT systemAccount;
	if (ReturnValue = pthic.GetLocalSystemAccount(&systemAccount) == STATUS_SUCCESS)
	{
		string userName(systemAccount.UserName);
		string password(systemAccount.Password); // This password is encrypted
		ReturnValue = wsmc.GetPortSettings(ethernetPortWSManList, userName, password);
	}
	/////////////////////////////////////////////////////////////////////////////////////////

	if (ReturnValue == STATUS_SUCCESS)
	{
		vector<EthernetPortEntryWSMan>::iterator portIterator;
		portIterator = ethernetPortWSManList.begin();	
		for (; portIterator != ethernetPortWSManList.end(); portIterator++)
		{
			EthernetPortEntry entry;
			entry.DefaultGateway = portIterator->DefaultGateway;
			entry.DHCPEnabled = portIterator->DHCPEnabled;
			entry.IPAddress = portIterator->IPAddress;
			entry.LinkIsUp = portIterator->LinkIsUp;
			entry.MACAddress = portIterator->MACAddress;
			entry.PrimaryDNS = portIterator->PrimaryDNS;
			entry.SecondaryDNS = portIterator->SecondaryDNS;
			entry.SubnetMask = portIterator->SubnetMask;
			ethernetPortList.push_back(entry);
		}
	}
	//if failed try using pthi
	else
	{
		ReturnValue = pthic.GetPortSettings(ethernetPortList);
	}
	return ReturnValue;
}

EthernetPortSettings_WMI_Provider::EthernetPortSettings_WMI_Provider(EthernetPortEntry port, std::wstring instanceID, std::wstring elementName)
{
	InstanceID = instanceID;
	ElementName = elementName;
	DefaultGateway = port.DefaultGateway;
	DHCPEnabled = port.DHCPEnabled;
	IPAddress = port.IPAddress;
	LinkIsUp = port.LinkIsUp;
	MACAddress = port.MACAddress;
	PrimaryDNS = port.PrimaryDNS;
	SecondaryDNS = port.SecondaryDNS;
	SubnetMask = port.SubnetMask;
}


HRESULT EthernetPortSettings_WMI_Provider::Get_PortSettings(
									 IWbemServices* pNamespace,
									 const BSTR strObjectPath,
									 IWbemContext __RPC_FAR *pCtx,
									 IWbemObjectSink __RPC_FAR *pResponseHandler)
{
	USES_CONVERSION; 

	//_Module.logger.Detail(File,LOCATION, _T("SCS Server"), _T("Start function"),_T(""));

	uint32 hr = 0;
	uint32 ReturnValue = 0;
	map <std::wstring, CComVariant> keyList;
	map <std::wstring, CComVariant>::const_iterator it ;

	try
	{
		GetKeysList(keyList, strObjectPath);
		it = keyList.find(L"InstanceID");
		if (it == keyList.end())
		{
			//_Module.logger.Error(File,LOCATION, _T("WiFi Profile data"), _T("AddWiFiProfile"),_T("WBEM_E_INVALID_METHOD_PARAMETERS"));
			//_Module.SetLastErrorString(GETSCSMESSAGE1(ERROR_IN_PARAMETER,_T("InstanceID")));
			return  WBEM_E_INVALID_METHOD_PARAMETERS;
		}
		std::wstring val = (it->second).bstrVal;
		const WCHAR* str = val.c_str();
		unsigned int num = _wtoi(str+wcslen(L"Intel(r) AMT Ethernet Port Settings"));
		//_Module.logger.Info(File,LOCATION, _T("Profile data"), _T("Get profile object started"),_T("Profile:")+ StringUtilsNamespace::convertTowString(id));

		//SCS_Profile profile;
		do 
		{
			vector<EthernetPortEntry> ethernetPortList;
			ReturnValue = GetPortList(ethernetPortList);

			if (ReturnValue != S_OK)
			{
				WMIHelper::PTHIHandleSetStatus(pNamespace, pResponseHandler, ReturnValue, hr);
				return hr;
			}
			
			if (num < 1 || num > ethernetPortList.size())
			{
				hr = WBEM_E_PROVIDER_FAILURE;
				break;
			}
			
			
			if (ReturnValue == STATUS_SUCCESS)
			{
			const EthernetPortEntry &entry = ethernetPortList.at(num-1);
			CComPtr<IWbemClassObject> obj;
			RETURNIF(WMIPutMember(pNamespace, &obj, L"AMT_EthernetPortSettings"));

			BREAKIF(WMIPut<1>(obj, L"ElementName", L"Intel(r) AMT Ethernet Port Settings"));
			WCHAR str[256];
			swprintf_s(str, 256, L"Intel(r) AMT Ethernet Port Settings %d",num);
			BREAKIF(WMIPut<1>(obj, L"InstanceID", str));

			BREAKIF(WMIPut<1>(obj, L"MACAddress", entry.MACAddress));
			BREAKIF(WMIPut<1>(obj, L"LinkIsUp", entry.LinkIsUp));

			BREAKIF(WMIPut<1>(obj, L"DHCPEnabled", entry.DHCPEnabled));
			BREAKIF(WMIPut<1>(obj, L"IPAddress", entry.IPAddress));
			BREAKIF(WMIPut<1>(obj, L"SubnetMask", entry.SubnetMask));
			BREAKIF(WMIPut<1>(obj, L"DefaultGateway", entry.DefaultGateway));
			BREAKIF(WMIPut<1>(obj, L"PrimaryDNS", entry.PrimaryDNS));
			BREAKIF(WMIPut<1>(obj, L"SecondaryDNS", entry.SecondaryDNS));

			
			BREAKIF(pResponseHandler->Indicate(1, &obj.p));
			}
		}while(0);

		WMIHandleSetStatus(pNamespace,pResponseHandler, hr);

		
		if (STATUS_SUCCESS == hr)
		{
			//_Module.logger.Info(File,LOCATION,  _T("Profile data"), _T("Get profile  finished successfully"),_T(""));
		}
		else
		{
			//_Module.logger.Error(File,LOCATION,  _T("Profile data"), _T("Get profile failed with error code"),_T("StringUtilsNamespace::convertTowString(hr)"));
		}
	}
	catch (...)
	{
		//_Module.SetLastErrorString(GETSCSMESSAGE(ERROR_EXCEPTION_IN_SERVICE));
		//_Module.logger.Error(File,LOCATION, _T("SCS Server"), _T(""), _T("Bad catch"));
		hr  = WBEM_E_PROVIDER_FAILURE;
	}


	return hr;

}

