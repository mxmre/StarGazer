#include "Render.h"
using namespace sg::graphics;
using namespace sg::core;

sg::graphics::Render::Render() : m_window(nullptr),
	m_driverType(D3D_DRIVER_TYPE_NULL),
	m_featureLevel(D3D_FEATURE_LEVEL_11_0),
	m_pd3dDevice(nullptr),
	m_pImmediateContext(nullptr),
	m_pSwapChain(nullptr),
	m_pRenderTargetView(nullptr)
{
}

sg::graphics::Render::~Render()
{
}

void sg::graphics::Render::Swap()
{
}

void sg::graphics::Render::Draw()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //�������,�������,�����,�����
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pSwapChain->Present(0, 0);
}

void sg::graphics::Render::Close()
{
	if (m_pImmediateContext)
		m_pImmediateContext->ClearState();

	_RELEASE(m_pRenderTargetView);
	_RELEASE(m_pSwapChain);
	_RELEASE(m_pImmediateContext);
	_RELEASE(m_pd3dDevice);
}

int sg::graphics::Render::Init(sg::core::Window* window)
{
	HRESULT hr = S_OK;
	this->m_window = window;
	if (window == nullptr) return 0;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));	// ������� ���������
	sd.BufferCount = 1;			// � ��� ���� ������ �����
	sd.BufferDesc.Width = this->m_window->GetWindowSettings().Width();		// ������������� ������ ������
	sd.BufferDesc.Height = this->m_window->GetWindowSettings().Height();		// ������������� ������
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ������ �������
	sd.BufferDesc.RefreshRate.Numerator = 60; // ������� ���������� ������
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���������� ������
	sd.OutputWindow = this->m_window->GetWindowSettings().Handle();			// ����������� ����
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;			// ������������� ������� �����

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, m_driverType,
			NULL, createDeviceFlags,
			featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd,
			&m_pSwapChain, &m_pd3dDevice,
			&m_featureLevel, &m_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return 0;

	ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return false;

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	if(pBackBuffer) pBackBuffer->Release();

	if (FAILED(hr))
		return 0;
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)this->m_window->GetWindowSettings().Width();
	vp.Height = (FLOAT)this->m_window->GetWindowSettings().Height();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &vp);
}
