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
	public:

		virtual EventType getEventType() const { return m_Type; }
		virtual const char* getEventName() const = 0;
		virtual int getCategoryFlag() const = 0;
		virtual std::string ToString() const { getEventName(); }
		inline bool isInCategory(EventCategory category)
		{
			return getCategoryFlag() & category;
		} 

	protected:

		Event(EventType type)
			: m_Type(type), m_Handled(false) {}

		EventType m_Type;
		bool m_Handled;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename T>
		bool Dispatch(bool(*func)(T&))
		{
			if (m_Event.getEventType() == T::getStatictype)
			{
				m_Event.m_Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};
}