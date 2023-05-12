#pragma once
#include "stdafx.h"
#include "Window.h"
namespace sg
{
	namespace core
	{
		class BaseApplication;
	}
	namespace graphics
	{
		class Render
		{
		public:
			friend class BaseApplication;
			Render();
			~Render();
			void Swap();
			void Draw();
			void Close();

		protected:
			int Init(sg::core::Window* window);
			sg::core::Window* m_window;
			D3D_DRIVER_TYPE m_driverType;
			D3D_FEATURE_LEVEL m_featureLevel;
			ID3D11Device* m_pd3dDevice;
			ID3D11DeviceContext* m_pImmediateContext;
			IDXGISwapChain* m_pSwapChain;
			ID3D11RenderTargetView* m_pRenderTargetView;
		};
	}
}


