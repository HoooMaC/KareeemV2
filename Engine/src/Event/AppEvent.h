#pragma once

#include "Event/Event.h"

namespace Karem {

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		// is equal with EVENT_CLASS_CATEGORY in Hazel
		int GetCategoryFlag() const override { return EventCategoryApplication; }

		// is equal with EVENT_CLASS_TYPE in Hazel
		static EventType GetStaticType() { return EventType::WindowClosed; }
		EventType GetEventType() const override { return EventType::WindowClosed; }
		const char* GetEventName() const override { return "Window Close Event"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Close Event";
			return ss.str();
		}
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {}

		// is equal with EVENT_CLASS_CATEGORY in Hazel
		int GetCategoryFlag() const override { return EventCategoryApplication; }

		// is equal with EVENT_CLASS_TYPE in Hazel 
		static EventType GetStaticType() { return EventType::WindowResized; }
		EventType GetEventType() const override { return EventType::WindowResized; }
		const char* GetEventName() const override { return "Window Resize"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Resize Width : " << m_Width << " Height : " << m_Height;
			return ss.str();
		}

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
	private:
		uint32_t m_Width, m_Height;
	};

}
