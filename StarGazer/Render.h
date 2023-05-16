#pragma once
#include "stdafx.h"
#include "GraphicalBaseObjects.h"
namespace sg
{
	namespace core
	{
		class Window;
		//class BaseApplication;
	}
	namespace graphics
	{
		class Render
		{
		public:
			//friend core::Window;
			//friend void Bind(Window& refApplicationWindow, Render& refRender);
			Render();
			~Render();
			bool Init();
			void ClearBuffers(const fColorRGBA color = {0.f, 0.f, 0.f, 0.f});
			bool IsInit() const;
			void BindWindow(core::Window* pWindow);
		protected:
			bool EnumerateAdapters();
			std::vector <std::shared_ptr< IDXGIAdapter1 >> vAdapters_;
			core::Window* pWindow_;

			ID3D11Device* d3dDevice_;
			ID3D11DeviceContext* d3dContext_;
			IDXGISwapChain* swapChain_;
			ID3D11RenderTargetView* backBufferTarget_;

			D3D_FEATURE_LEVEL featureLevel_;
		};
	}
}


