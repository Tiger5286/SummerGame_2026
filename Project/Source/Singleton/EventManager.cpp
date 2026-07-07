#include "EventManager.h"

void EventManager::CallEvent(const std::string& eventName)
{
	for (auto& data : m_eventDatas)
	{
		if (data.eventName == eventName)
		{
			data.func();
		}
	}
}

int EventManager::Register(const std::string& eventName, std::function<void()> func)
{
	m_newHandle++;
	EventData newData = {
		.handle = m_newHandle,
		.eventName = eventName,
		.func = func
	};
	m_eventDatas.push_back(newData);
	return newData.handle;
}

void EventManager::UnRegister(int handle)
{
	EventData removeData;
	for (auto& data : m_eventDatas)
	{
		if (data.handle == handle)
		{
			removeData = data;
			break;
		}
	}
	m_eventDatas.remove(removeData);
}
