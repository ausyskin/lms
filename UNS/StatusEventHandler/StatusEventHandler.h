/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2010-2019 Intel Corporation
 */
#ifndef __STATUSEVENTHANDLER_H_
#define __STATUSEVENTHANDLER_H_
#include "global.h"
#include "ace/Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/Task.h"
#include "ace/Svc_Handler.h"
#include "ace/Containers.h"
#include "ace/Map_Manager.h"
#include "ace/Synch.h"
#include "ace/Reactor_Notification_Strategy.h"
#include "ace/Message_Block.h"
#include "ace/Log_Msg.h"
#include "EventHandler.h"
#include "StatusEventFilter.h"
#include "AMT_COM_Interface_defs.h"
#include "GetProvisioningStateCommand.h"
#include "GetPlatformTypeCommand.h"
#include "HostBootReasonClient.h"
#include "STATUSEVENTHANDLER_export.h"
#include <vector>
#include <string>
#include <mutex>
enum DATA_NAME : unsigned int;

class STATUSEVENTHANDLER_Export StatusEventHandler : public EventHandler
{
	public:
					
		StatusEventHandler();
typedef enum _PUBLISHEVENTS
{
	SOL,
	IDER,
	SYSDEF,
	KVMSTATE,
	KVMACTIVITY,
	MANAGEMODE,
	MESTATE,
	HECISTATE,
	EACENABLE,
	IPSYNCENABLE,
	TIMESYNCENABLE,
	WIFIPROFILESYNCENABLE
} PUBLISHEVENTS;
typedef enum _UC_STATE
{
	UC_ENDED,
	UC_REQUESTED,
	UC_GRANTED
} UC_STATE;
typedef enum _OPT_IN_REQUIRED
{
	OPT_IN_REQUIRED_NONE = 0,
	OPT_IN_REQUIRED_KVM = 1,
	OPT_IN_REQUIRED_ALL = -1
} OPT_IN_REQUIRED;
typedef enum _OPT_IN_STATE
{
	OPT_IN_STATE_NOT_STARTED = 0,
	OPT_IN_STATE_REQUESTED = 1,
	OPT_IN_STATE_DISPLAYED = 2,
	OPT_IN_STATE_RECEIVED = 3,
	OPT_IN_STATE_IN_SESSION = 4
} OPT_IN_STATE;
typedef enum _KVM_REDIRECTION_SAP_STATE
{
	KVM_ENABLED_AND_CONNECTED = 2,
	KVM_DISABLED = 3,
	KVM_ENABLED_AND_DISCONNECTED = 6
} KVM_REDIRECTION_SAP_STATE;
typedef enum _KVM_STATE
{
	KVM_STOPPED,
	KVM_STARTED,
	KVM_REQUESTED,
	KVM_DATA_CHANNEL
} KVM_STATE;
typedef enum _WLAN_CONTROL_STATE
{
	ME_CONTROL = 1,
	HOST_CONTROL = 2
}WLAN_CONTROL_STATE;

typedef enum _WLAN_PROTECTION_STATE
{
	PROTECTION_OFF = 1,
	PROTECTION_PASSIVE = 2,
	PROTECTION_HIGH = 3,
	NOT_EXIST = 5
}WLAN_PROTECTION_STATE;

	void publishEvent(int action, PUBLISHEVENTS ex);
	//// Publish KVM state event while UNS was stopped
	void publishUCStateEvent(UC_STATE state);

	void PublishWlanProtectionEvent(WLAN_PROTECTION_STATE state);
	void PublishWlanControlEvent(WLAN_CONTROL_STATE state);
	//returns true if there is a current need for reboot after provisioning
	static bool getRebootAfterProvisioningNeed(); 


protected:
	virtual int init(int argc, ACE_TCHAR *argv[]);

	virtual int fini (void);

	virtual int suspend();

	virtual int resume();

	virtual int handle_event (MessageBlockPtr mbPtr);

		
	virtual const ACE_TString name();

	virtual std::shared_ptr<EventsFilter> getFilter()
	{
		return filter_;
	}

	private:

	bool SaveCurrentStatus(uint32_t status,DATA_NAME storageName);
	void NotifyConfigurator(int status, CONFIGURATION_TYPE RegValueName);
	void NotifyIPRefresh(int RegValueName);
	bool StatusChanged(DATA_NAME storageName, uint32_t state);
	void CheckForStatusChange(DATA_NAME storageName,bool state,PUBLISHEVENTS ev);
	void CheckForStatusChange(DATA_NAME storageName,Intel::MEI_Client::AMTHI_Client::AMT_PROVISIONING_STATE state);
	void CheckForStatusChange(DATA_NAME storageName,FEATURE_STATE state);
	void CheckForStatusChange(DATA_NAME storageName,UC_STATE state);
	void CheckForStatusChange(DATA_NAME storageName,KVM_STATE state);
	void CheckForStatusChange(DATA_NAME storageName, WLAN_PROTECTION_STATE state);
	void CheckForStatusChange(DATA_NAME storageName, WLAN_CONTROL_STATE state);
		
	bool GetProvisioningState(Intel::MEI_Client::AMTHI_Client::AMT_PROVISIONING_STATE& ProvState);
	bool GetAMTEnableState(bool& AMTState);
	bool GetSolIderState(bool& SOLState, bool& IDERState);
	bool GetSystemDefenseState(bool& SysDefState);
	bool GetMEState(bool& MEState);

	bool GetAlarmClockBootEvent(SX_STATES &previousSXState);

	// Publish Provisioning event while LMS was stopped
	void publishProvisioningEvent(Intel::MEI_Client::AMTHI_Client::AMT_PROVISIONING_STATE state);

	// Generate events according to register values while LMS was stopped
	void GenerateEvents();
	void GeneratePortFwrdRelatedEvents();
	void GenerateEACEvents(bool AMTstate);
	void GenerateSharedStaticIPEvents(bool AMTstate);
	void GenerateTimeSyncEvents(bool AMTstate);
	void GenerateSOLIDEREvents(bool AMTstate);
	void GenerateKVMRedirectionEvents(bool AMTstate);
	void GenerateSystemDefenseEvents(bool AMTstate);
	void GenerateProvisioningEvents(bool AMTstate);
	void GenerateUCEvents(bool AMTstate);
	void GenerateMEEvents();
	void GenerateWLANEvents();
	void GenerateWiFiProfileSyncEvents();

	bool GetManageabiltyAndFeaturesState(MENAGEABILTY_MODE* pManageMode,CUSTOMER_TYPE* pType,FEATURE_STATE* pAmtState,FEATURE_STATE* pRpatState,FEATURE_STATE* pKvmState,FEATURE_STATE* pTDTState);

	// Publish AMT enabled event
	void publishAMTEnabledEvent(bool enable);

	static FEATURE_STATE FeatureStateLogic(bool CapabilityBit, bool StateBit);
	static void MenageabiltyModeLogic(Intel::MEI_Client::MKHI_Client::MKHI_PLATFORM_TYPE platform, MENAGEABILTY_MODE* pMode);

	Intel::MEI_Client::AMTHI_Client::AMT_PROVISIONING_STATE UpdatePrevProvisioningState(Intel::MEI_Client::AMTHI_Client::AMT_PROVISIONING_STATE State);
	bool GetUserConsentState(short* pState, USER_CONSENT_POLICY* pPolicy);
	bool GetLocalProfileSynchronizationEnabled();
	void firstPullForEvents(); 
	void checkForBootReason();

	int handleStatusChanged(const GMS_AlertIndication *alert);
	void handleGeneralEvents(const GMS_AlertIndication *alert);
	void handleSystemDefenceEvents(const GMS_AlertIndication *alert);
	void handleRemoteDiagnosticEvents(const GMS_AlertIndication *alert);
	void handleKVMEvents(const GMS_AlertIndication *alert);
	void handleUNSEvents(const GMS_AlertIndication *alert);
	void handleIPSyncEvents(const GMS_AlertIndication *alert);
	void handleUserConsentEvents(const GMS_AlertIndication *alert);
	void handleProvisioningEvents(const GMS_AlertIndication *alert);
	void handleAgentPresenceEvents(const GMS_AlertIndication *alert);
	void handleWlanEvents(const GMS_AlertIndication *alert);
	void handleTimeSyncEvents(const GMS_AlertIndication *alert);

	void raiseGMS_AlertIndication(unsigned short category, unsigned long id, const std::string &Datetime,
		const ACE_TString &MessageID, const ACE_TString &Message, const std::vector<ACE_TString> &MessageArguments);
	void raiseGMS_AlertIndication(unsigned short category, unsigned long id, const std::string &Datetime,
		const ACE_TString &MessageID, const ACE_TString &Message);

	// Publish Remote Reboot event
	void publishRemoteRebootEvent();
	//publish Host boot by Alarm clock
	void publishAlarmClockBoot(SX_STATES previousSXState);
	ACE_TString GetSXState(SX_STATES previousSXState);

	void SafeSetProvisioningState(Intel::MEI_Client::AMTHI_Client::AMT_PROVISIONING_STATE State);
	// Get the network settings (need for populate the MAC address)
	bool GetEACEnabled(bool& enable);
	bool GetKVMRedirectionState(bool& enable,KVM_STATE& connected);
	void requestDisplaySettings(); 

	//check if reboot is required after provisioning for current FW and SKU
	bool isRebootAfterProvisioningRequired();
	//indiates that the system needs a reboot due to provisioning
	static bool s_rebootAfterProvsioningNeeded;	
	//semaphore for s_rebootAfterProvsioningNeeded (can be accessed by the service and IMSS)
	static std::mutex s_rebootAfterProvsioningSemaphore; 
	//sets the current state for reboot after provisioning
	void setRebootAfterProvisioningNeed(bool needed);

	short m_KVM;
	short m_SOL;
	short m_IDER;
	short m_prevUserConsentState;
	Intel::MEI_Client::AMTHI_Client::AMT_PROVISIONING_STATE m_prevProvState;
	USER_CONSENT_POLICY m_UserConsentPolicy;
	std::mutex m_semAMTEnabled;
	bool m_eacEnabled;
	FEATURE_STATE m_prevAmtState, m_prevRpatState, m_prevKvmState, m_prevTDTState;
	CUSTOMER_TYPE m_prevCustomerType;
	MENAGEABILTY_MODE m_prevManageMode;
	bool m_firstTime;
	std::shared_ptr<StatusEventFilter> filter_;
	};

#endif /* __STATUSEVENTHANDLER_H_ */
