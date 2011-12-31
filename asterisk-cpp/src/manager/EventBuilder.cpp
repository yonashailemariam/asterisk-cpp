/*
 * EventBuilder.cpp
 *
 *  Created on: Jul 12, 2011
 *      Author: augcampos
 */

#include "asteriskcpp/manager/EventBuilder.h"
#include <stdlib.h>
#include <map>
#include "asteriskcpp/utils/StringUtils.h"

#include "asteriskcpp/manager/events/AgentCallbackLoginEvent.h"
#include "asteriskcpp/manager/events/AgentCallbackLogoffEvent.h"
#include "asteriskcpp/manager/events/AgentCalledEvent.h"
#include "asteriskcpp/manager/events/AgentCompleteEvent.h"
#include "asteriskcpp/manager/events/AgentConnectEvent.h"
#include "asteriskcpp/manager/events/AgentDumpEvent.h"
#include "asteriskcpp/manager/events/AgentLoginEvent.h"
#include "asteriskcpp/manager/events/AgentLogoffEvent.h"
#include "asteriskcpp/manager/events/AgentRingNoAnswer.h"
#include "asteriskcpp/manager/events/AgentsCompleteEvent.h"
#include "asteriskcpp/manager/events/AgiExecEvent.h"
#include "asteriskcpp/manager/events/AlarmClearEvent.h"
#include "asteriskcpp/manager/events/AlarmEvent.h"
#include "asteriskcpp/manager/events/AsyncAgiEvent.h"
#include "asteriskcpp/manager/events/BridgeEvent.h"
#include "asteriskcpp/manager/events/BridgeExecEvent.h"
#include "asteriskcpp/manager/events/CdrEvent.h"
#include "asteriskcpp/manager/events/ChannelReloadEvent.h"
#include "asteriskcpp/manager/events/ChannelUpdateEvent.h"
#include "asteriskcpp/manager/events/ConfbridgeEndEvent.h"
#include "asteriskcpp/manager/events/ConfbridgeJoinEvent.h"
#include "asteriskcpp/manager/events/ConfbridgeLeaveEvent.h"
#include "asteriskcpp/manager/events/ConfbridgeListCompleteEvent.h"
#include "asteriskcpp/manager/events/ConfbridgeListEvent.h"
#include "asteriskcpp/manager/events/ConfbridgeListRoomsCompleteEvent.h"
#include "asteriskcpp/manager/events/ConfbridgeListRoomsEvent.h"
#include "asteriskcpp/manager/events/ConfbridgeStartEvent.h"
#include "asteriskcpp/manager/events/ConfbridgeTalkingEvent.h"
#include "asteriskcpp/manager/events/ConnectEvent.h"
#include "asteriskcpp/manager/events/CoreShowChannelEvent.h"
#include "asteriskcpp/manager/events/CoreShowChannelsCompleteEvent.h"
#include "asteriskcpp/manager/events/DbGetResponseEvent.h"
#include "asteriskcpp/manager/events/DialEvent.h"
#include "asteriskcpp/manager/events/DisconnectEvent.h"
#include "asteriskcpp/manager/events/DndStateEvent.h"
#include "asteriskcpp/manager/events/DtmfEvent.h"
#include "asteriskcpp/manager/events/ExtensionStatusEvent.h"
#include "asteriskcpp/manager/events/FaxDocumentStatusEvent.h"
#include "asteriskcpp/manager/events/FaxReceivedEvent.h"
#include "asteriskcpp/manager/events/FaxStatusEvent.h"
#include "asteriskcpp/manager/events/FullyBootedEvent.h"
#include "asteriskcpp/manager/events/HangupEvent.h"
#include "asteriskcpp/manager/events/HoldEvent.h"
#include "asteriskcpp/manager/events/HoldedCallEvent.h"
#include "asteriskcpp/manager/events/JabberEventEvent.h"
#include "asteriskcpp/manager/events/JitterBufStatsEvent.h"
#include "asteriskcpp/manager/events/JoinEvent.h"
#include "asteriskcpp/manager/events/LeaveEvent.h"
#include "asteriskcpp/manager/events/LinkEvent.h"
#include "asteriskcpp/manager/events/ListDialplanEvent.h"
#include "asteriskcpp/manager/events/LogChannelEvent.h"
#include "asteriskcpp/manager/events/MasqueradeEvent.h"
#include "asteriskcpp/manager/events/MeetMeEndEvent.h"
#include "asteriskcpp/manager/events/MeetMeJoinEvent.h"

namespace asteriskcpp {

EventBuilder::EventBuilder() {
	RegisterClass<AgentCallbackLoginEvent>();
	RegisterClass<AgentCallbackLogoffEvent>();
	RegisterClass<AgentCalledEvent>();
	RegisterClass<AgentCompleteEvent>();
	RegisterClass<AgentConnectEvent>();
	RegisterClass<AgentDumpEvent>();
	RegisterClass<AgentLoginEvent>();
	RegisterClass<AgentLogoffEvent>();
	RegisterClass<AgentRingNoAnswer>();
	RegisterClass<AgentsCompleteEvent>();
	RegisterClass<AgiExecEvent>();
	RegisterClass<AlarmClearEvent>();
	RegisterClass<AlarmEvent>();
	RegisterClass<AsyncAgiEvent>();
	RegisterClass<BridgeEvent>();
	RegisterClass<BridgeExecEvent>();
	RegisterClass<CdrEvent>();
	RegisterClass<ChannelReloadEvent>();
	RegisterClass<ChannelUpdateEvent>();
	RegisterClass<ConfbridgeEndEvent>();
	RegisterClass<ConfbridgeJoinEvent>();
	RegisterClass<ConfbridgeLeaveEvent>();
	RegisterClass<ConfbridgeListCompleteEvent>();
	RegisterClass<ConfbridgeListEvent>();
	RegisterClass<ConfbridgeListRoomsCompleteEvent>();
	RegisterClass<ConfbridgeListRoomsEvent>();
	RegisterClass<ConfbridgeStartEvent>();
	RegisterClass<ConfbridgeTalkingEvent>();
	RegisterClass<ConnectEvent>();
	RegisterClass<CoreShowChannelEvent>();
	RegisterClass<CoreShowChannelsCompleteEvent>();
	RegisterClass<DbGetResponseEvent>();
	RegisterClass<DialEvent>();
	RegisterClass<DisconnectEvent>();
	RegisterClass<DndStateEvent>();
	RegisterClass<DtmfEvent>();
	RegisterClass<ExtensionStatusEvent>();
	RegisterClass<FaxDocumentStatusEvent>();
	RegisterClass<FaxReceivedEvent>();
	RegisterClass<FaxStatusEvent>();
	RegisterClass<FullyBootedEvent>();
	RegisterClass<HangupEvent>();
	RegisterClass<HoldEvent>();
	RegisterClass<HoldedCallEvent>();
	RegisterClass<JabberEventEvent>();
	RegisterClass<JitterBufStatsEvent>();
	RegisterClass<JoinEvent>();
	RegisterClass<LeaveEvent>();
	RegisterClass<LinkEvent>();
	RegisterClass<ListDialplanEvent>();
	RegisterClass<LogChannelEvent>();
	RegisterClass<MasqueradeEvent>();
	RegisterClass<MeetMeEndEvent>();
	RegisterClass<MeetMeJoinEvent>();

}

EventBuilder::~EventBuilder() {

}

static std::string extractEventName(const std::string& eventStr) {
	size_t endl = eventStr.find("\r\n");
	if (endl != eventStr.npos) {
		size_t sep = eventStr.find("Event: ");
		if (sep != eventStr.npos) {
			return (eventStr.substr(sep + 7, endl - (sep + 7)));
		}
	}
	return ("");
}

ManagerEvent* EventBuilder::buildEvent(const std::string& eventStr) {

	std::string eventName = extractEventName(eventStr);
	if (!eventName.empty()) {

		EnventFactoryMapType::const_iterator it = eventFactoryMap.find(eventName);
		if (it != eventFactoryMap.end()) {
			return ((it->second)->create(eventStr));
		} else {
			LOG_WARN_STR("EVENT:INVALID OR NOT IMPLEMENTED \n"+ str2Log(eventStr))
			return (new ManagerEvent(eventStr));
		}
	}

	return (NULL);
}

} /* namespace asterisk_cpp */
