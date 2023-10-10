#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Renderer/BaseRendererAPI.h"
#include "Renderer/BufferLayout.h"

namespace Karem {

	class Renderer2D;

	class RendererCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}
		inline static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewPort(x, y, width, height);
		}
		inline static void ClearColor(const glm::vec4& color)
		{
			s_RendererAPI->ClearColor(color);
		}
		inline static void ClearColor(std::string_view hexColor)
		{
			s_RendererAPI->ClearColor(hexColor);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		inline static void Draw(uint32_t count)
		{
			s_RendererAPI->Draw(count);
		}
	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};

}
