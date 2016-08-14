#pragma once

#include "Texture2D.hpp"

namespace VermeerRender
{
	class VermeerRender
	{
	public:
		VermeerRender() {}

		void Render();
		void SaveImage(const char* filepath)
		{
			m_renderTexture.SaveImage(filepath);
		}
		Texture2D m_renderTexture{ 512, 512 };
	};
}