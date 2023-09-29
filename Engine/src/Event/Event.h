#pragma once

#include "Core/Kareeem.h"

#define BIT(x) (1 << x)

namespace Karem {

	enum class EventType : uint8_t
	{
		None = 0,
		WindowClosed, WindowResized, WindowFocus, WindowLostFocus, WindowMove,
		KeyPressed, KeyReleased, KeyTyped,
		MousePressed, MouseReleased, MouseMoved, MouseScroll
	};

	enum EventCategory : uint8_t
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput       = BIT(1),
		EventCategoryKeyboard    = BIT(2),
		EventCategoryMouse       = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	class Event
	{
		friend class EventDispatcher;
	public:
		virtual int GetCategoryFlag() const = 0;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetEventName() const = 0;

		virtual std::string ToString() const {  return GetEventName(); }

		inline bool isInCategory(EventCategory category)
		{
			return GetCategoryFlag() & category;
		} 

	protected:

		Event()
			: m_Handled(false) {}

		bool m_Handled;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFN = std::function<bool(T&)>;
		//bool(*func)(T&)
	public:

		EventDispatcher(Event& event)
			: m_Event(event) {}


		template<typename T>
		bool Dispatch(EventFN<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}