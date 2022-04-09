#pragma once
#include <unordered_map>
#include <concepts>
#include <memory>
#include <atomic>
#include <functional>

namespace sy::event_sys
{
	template <typename... EventParams>
	class EventSystem;

	using EventID = uint64_t;
	constexpr EventID INVALID_EVENT_ID = 0;

	template <typename... EventParams>
	class EventSystem
	{
	public:
		using Event = std::function<void(EventParams...)>;

	public:
		EventID Subscribe(Event e)
		{
			static std::atomic<EventID> id = INVALID_EVENT_ID;
			lut[++id] = e;
			return id;
		}

		void Unsubscribe(EventID eventID)
		{
			if (eventID != INVALID_EVENT_ID)
			{
				if (auto itr = lut.find(eventID); itr != lut.end())
				{
					lut.erase(itr);
				}
			}
		}

		bool Contains(EventID eventID) const
		{
			return lut.find(eventID) != lut.end();
		}

		void Notify(EventParams&&... params)
		{
			for (auto callable : lut)
			{
				callable.second(std::forward<EventParams>(params)...);
			}
		}

	private:
		std::unordered_map<EventID, Event> lut;

	};
}