#pragma once

#include "Event.h"

#include <string>

namespace Karem {

	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		// is equal with EVENT_CLASS_CATEGORY in Hazel
		int GetCategoryFlag() const override { return EventCategoryKeyboard | EventCategoryInput; }

	protected:
		KeyEvent(int keyCode)
			: m_KeyCode(keyCode) {}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		// is equal with EVENT_CLASS_TYPE in Hazel
		static EventType GetStaticType() { return EventType::KeyPressed; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetEventName() const override { return "KeyPressed"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Key Pressed Event " << m_KeyCode << " (" << m_RepeatCount << ")";
			return ss.str();
		}

	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		// is equal with EVENT_CLASS_TYPE in Hazel
		static EventType GetStaticType() { return EventType::KeyReleased; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetEventName() const override { return "KeyReleased"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Key Released Event " << m_KeyCode;
			return ss.str();
		}

	};
}
