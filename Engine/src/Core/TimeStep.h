#pragma once

namespace Karem {

	class TimeStep
	{
	public:
		TimeStep(float time)
			: m_Time(time) {}

		operator float() const { return GetSecond(); }

		float GetSecond() const { return m_Time; }
		float GetMiliSecond() const { return m_Time * 1000; }

	private:
		float m_Time;
	};

}
