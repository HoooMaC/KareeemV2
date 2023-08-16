#include "Event/Event.h"

namespace Karem {

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_PosX(x), m_PosY(y) {}

		inline float GetX() const { return m_PosX; }
		inline float GetY() const { return m_PosY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Mouse Moved Event X : " << m_PosX << " Y : " << m_PosY;
			return ss.str();
		}

		// is equal with EVENT_CLASS_CATEGORY in Hazel
		int GetCategoryFlag() const override { return EventCategoryMouse; }

		// is equal with EVENT_CLASS_TYPE in Hazel
		static EventType GetStaticType() { return EventType::MouseMoved; }
		EventType GetEventType() const override { return EventType::MouseMoved; }
		const char* GetEventName() const override { return "Mouse Moved"; }

	private:
		float m_PosX, m_PosY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float x, float y)
			: m_OffsetX(x), m_OffsetY(y) {}

		inline float GetOffsetX() const { return m_OffsetX; }
		inline float GetOffsetY() const { return m_OffsetY; }

		// is equal with EVENT_CLASS_CATEGORY in Hazel
		int GetCategoryFlag() const override { return EventCategoryMouse | EventCategoryInput; }

		// is equal with EVENT_CLASS_TYPE in Hazel
		static EventType GetStaticType() { return EventType::MouseScroll; }
		EventType GetEventType() const override { return EventType::MouseScroll; }
		const char* GetEventName() const override { return "Mouse Scrolled"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Mouse Scrolled Event X : " << m_OffsetX << " Y : " << m_OffsetY;
			return ss.str();
		}

	private:
		float m_OffsetX, m_OffsetY;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		// is equal with EVENT_CLASS_CATEGORY in Hazel
		int GetCategoryFlag() const override { return EventCategoryMouse | EventCategoryInput; }
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		// is equal with EVENT_CLASS_TYPE in Hazel
		static EventType GetStaticType() { return EventType::MousePressed; }
		EventType GetEventType() const override { return EventType::MousePressed; }
		const char* GetEventName() const override { return "Mouse Pressed"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Mouse Button Pressed Event " << m_Button;
			return ss.str();
		}

	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		// is equal with EVENT_CLASS_TYPE in Hazel
		static EventType GetStaticType() { return EventType::MouseReleased; }
		EventType GetEventType() const override { return EventType::MouseReleased; }
		const char* GetEventName() const override { return "Mouse Released Event"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Mouse Button Released Event " << m_Button;
			return ss.str();
		}
	};

}