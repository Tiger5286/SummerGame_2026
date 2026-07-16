#include "EventManager.h"

EventManager& EventManager::GetInstance()
{
	static EventManager instance;
	return instance;
}

void EventManager::CallEvent(const std::string& eventName)
{
	// 対応した名前の関数を呼ぶ
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
	// 管理番号を更新
	m_newHandle++;
	// 登録するイベントデータを作成
	EventData newData = {
		.handle = m_newHandle,
		.eventName = eventName,
		.func = func
	};
	// 登録
	m_eventDatas.push_back(newData);
	// 新しいイベントのハンドルを返す
	return newData.handle;
}

void EventManager::UnRegister(int handle)
{
	// ハンドルが一致するイベントを削除
	for (auto& data : m_eventDatas)
	{
		if (data.handle == handle)
		{
			m_eventDatas.remove(data);
			break;
		}
	}
}
