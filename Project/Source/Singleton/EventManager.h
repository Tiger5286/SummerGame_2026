#pragma once
#include <string>
#include <functional>
#include <list>

class EventManager
{
public:
	static EventManager& GetInstance();
	~EventManager() = default;

	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;
private:
	EventManager() = default;

	struct EventData
	{
		int handle = -1;
		std::string eventName;
		std::function<void()> func;

		bool operator==(const EventData& data) const
		{
			return handle == data.handle;
		}
	};
public:

	/// <summary>
	/// 登録したイベントを発火させる。
	/// </summary>
	/// <param name="eventName">発火させるイベント名</param>
	void CallEvent(const std::string& eventName);

	/// <summary>
	/// イベント発火時に呼ばれる関数を登録する。
	/// </summary>
	/// <param name="eventName">イベント名</param>
	/// <param name="func">発火時に呼ばれる関数</param>
	/// <returns>管理番号</returns>
	int Register(const std::string& eventName, std::function<void()> func);

	/// <summary>
	/// 登録した関数を登録解除する。
	/// </summary>
	/// <param name="handle">登録した関数の管理番号</param>
	void UnRegister(int handle);

private:
	std::list<EventData> m_eventDatas;
	int m_newHandle = 0;
};