#include "Render.h"
#include "Shader.h"
#include "Scene.h"
#include "Buffer.h"
#include "Object.h"
#include "ObjectManager.h"

/*

	������������� Direct3D:

	1. ������� ����������� swap chain, �������� ��������� DXGI_SWAP_CHAIN_DESC;
	2. �������� ���������� ID3D11Device � IDXGISwapChain, ��������� ������� D3D11CreateDeviceAndSwapChain;
	3. �������� render target ��� ������� ������ swap chain;
	4. �������� ����� �������/������� (stencil) � ������� ��� � depth/stencil view;
	5. ������������ render target � depth/stencil view � �������� ���������� (rendering pipeline), �����
	   Direct3D ��� �� ������������;
	6. ���������� ������� (viewport).

*/

Render::Render() {
	m_pd3dDevice = nullptr;
	m_pImmediateContext = nullptr;
	m_pSwapChain = nullptr;
	m_pRenderTargetView = nullptr;
	m_pDepthStencil = nullptr;
	m_pDepthStencilView = nullptr;
	m_pDepthStencilState = nullptr;
	m_pDepthDisabledStencilState = nullptr;
	m_pAlphaEnableBlendingState = nullptr;
	m_pAlphaDisableBlendingState = nullptr;
	m_pIndexBuffer = nullptr;
	m_pVertBuffer = nullptr;
	m_pConstMatrixBuffer = nullptr;
	m_pConstLightBuffer = nullptr;
	m_pShader = nullptr;
	m_pTerrain = nullptr;
	m_pCamera = nullptr;
	isInit = false;
}

Render::~Render() {

}


void Render::m_resize() {
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	m_width = rc.right - rc.left;
	m_height = rc.bottom - rc.top;
}

bool Render::createDevice(HWND hwnd) {
	m_hwnd = hwnd;

	m_resize();

	if(!m_createDevice()) {
		Log::get()->err("�� ������� ������� DirecX Device");
		return false;
	}

	if(!m_createDepthStencil()) {
		Log::get()->err("�� ������� ������� ����� �������");
		return false;
	}

	if(!m_createBlendingState()) {
		Log::get()->err("�� ������� ������� Blending State");
		return false;
	}

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)m_width;
	vp.Height = (FLOAT) m_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &vp);

	m_initMatrix();

	return init();
}

bool Render::m_createDevice() {
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = m_width;
	sd.BufferDesc.Height = m_height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, 
										featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd,
										&m_pSwapChain, &m_pd3dDevice, NULL, &m_pImmediateContext);
	if(FAILED(hr)) {
		return false;
	}

	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if(FAILED(hr)) {
		return false;
	}

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	_RELEASE(pBackBuffer);
	if(FAILED(hr)) {
		return false;
	}

	return true;
}

bool Render::m_createDepthStencil() {
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = m_width;
	descDepth.Height = m_height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = m_pd3dDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);
	if(FAILED(hr)) {
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc,sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	hr = m_pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	if(FAILED(hr)) {
		return false;
	}

	depthStencilDesc.DepthEnable = false;
	hr = m_pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthDisabledStencilState);
	if(FAILED(hr)) {
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
	if(FAILED(hr)) {
		return false;
	}

	return true;
}

bool Render::m_createBlendingState() {
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	HRESULT hr = m_pd3dDevice->CreateBlendState(&blendStateDescription, &m_pAlphaEnableBlendingState);
	if(FAILED(hr)) {
		return false;
	}

	blendStateDescription.RenderTarget[0].BlendEnable = false;
	hr = m_pd3dDevice->CreateBlendState(&blendStateDescription, &m_pAlphaDisableBlendingState);
	if(FAILED(hr)) {
		return false;
	}

	return true;
}

void Render::m_initMatrix() {
	float aspect = (float)m_width/(float)m_height;
	m_projection = XMMatrixPerspectiveFovLH(0.4f*3.14f, aspect, 1.0f, 1000.0f);
	m_ortho = XMMatrixOrthographicLH((float)m_width, (float)m_height, 0.0f, 1.0f);
}

bool Render::init() {
	m_pShader = new Shader();
	m_pTerrain = new Terrain();


	if(!m_pShader) {
		return false;
	}

	if(!m_pShader->loadTexture(L"Res\\grass.jpg", m_pd3dDevice)) {
		return false;
	}

	m_pShader->addInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	m_pShader->addInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	m_pShader->addInputElementDesc("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	if(!m_pShader->createShader(L"Shaders\\pointlight.vs", L"Shaders\\pointlight.ps", m_pd3dDevice)) {
		return false;
	}

	m_pConstMatrixBuffer = Buffer::createConstantBuffer(m_pd3dDevice, sizeof(cbMatrixData), false);
	m_pConstLightBuffer = Buffer::createConstantBuffer(m_pd3dDevice, sizeof(cbLightData), false);

	m_light.dir = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	
	if(!m_pTerrain->create(&std::string("D:\\RESTORE_FILES\\Projects\\D3D11Lessons\\D3D11_Chapter_PhysX\\D3D11_Chapter_PhysX\\Res\\heightmap3.bmp"),
											m_pd3dDevice)) {
		Log::get()->err("Render::init() failed: unable to create Terrain");
		return false;
	}
	XMVECTOR tmp = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	m_pCamera = new Camera();

	m_pCamera->init(XMVectorSet(0.0f, 60.0f, -60.0f, 0.0f));

	isInit = true;
	return true;
}

bool Render::initObjects(ObjectManager* objectManager) {
	m_pObjectManager = objectManager;

	m_pIndexBuffer = Buffer::createIndexBuffer(m_pd3dDevice, sizeof(DWORD)*m_pObjectManager->totalIndices(),
																	false, m_pObjectManager->getIndices());
	m_pVertBuffer = Buffer::createVertexBuffer(m_pd3dDevice, sizeof(Vertex)*m_pObjectManager->totalVertices(),
																	false, m_pObjectManager->getVertices());
	return true;
}

bool Render::draw(double dt) {
	m_pCamera->update(dt);
	//m_light.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//m_light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	cbLightData cblgh;
	cblgh.light = m_light;
	m_pImmediateContext->UpdateSubresource(m_pConstLightBuffer, 0 , NULL, &cblgh, 0, 0);
	m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pConstLightBuffer);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertBuffer, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cbMatrixData cbMat;
	XMMATRIX WVP;

	objects.clear();
	m_pObjectManager->getObjects(objects);
	XMMATRIX translation;
	XMMATRIX rotation;
	XMMATRIX scale;

	for(int i = 0; i < objects.size(); i++) {
		XMMATRIX translation = XMMatrixTranslation(objects[i]->getPosition().x, objects[i]->getPosition().y, objects[i]->getPosition().z);
		XMMATRIX rotation = XMMatrixRotationQuaternion(XMVectorSet(objects[i]->getRotation().w, objects[i]->getRotation().x,
																	objects[i]->getRotation().y, objects[i]->getRotation().z));
		XMMATRIX scale = XMMatrixScaling(objects[i]->getScale().x, objects[i]->getScale().y, objects[i]->getScale().z);
		//������� ���������� ������ � ����� �������
		WVP = rotation * translation * scale * m_pCamera->getView() * m_projection;
		cbMat.world = XMMatrixTranspose(translation);
		cbMat.WVP = XMMatrixTranspose(WVP);
		m_pImmediateContext->UpdateSubresource(m_pConstMatrixBuffer, 0, NULL, &cbMat, 0, 0);
		m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstMatrixBuffer);
	
		m_pShader->draw(m_pImmediateContext);

		m_pImmediateContext->DrawIndexed(objects[i]->getIndices().size(), 0, 0);
	}

	m_renderTerrain();

	return true;
}

void Render::m_renderTerrain() {
	if(!m_pTerrain) {
		return;
	}

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, m_pTerrain->getVertexBuffer(), &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(m_pTerrain->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	cbMatrixData cbMat;
	XMMATRIX WVP;
	XMMATRIX translation;
	XMMATRIX scale;

	WVP = XMMatrixIdentity();
	translation = XMMatrixTranslation(-100.0f, -100.0f, -100.0f);
	scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	WVP =  translation * scale * m_pCamera->getView() * m_projection;
	cbMat.world = XMMatrixTranspose(translation);
	cbMat.WVP = XMMatrixTranspose(WVP);

	m_pImmediateContext->UpdateSubresource(m_pConstMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstMatrixBuffer);

	m_pShader->draw(m_pImmediateContext);
	m_pImmediateContext->DrawIndexed(m_pTerrain->getIndexCount(), 0, 0);
}

void Render::beginFrame(double dt) {
	float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Render::endFrame() {
	m_pSwapChain->Present(0, 0);
}

void Render::shutdown() {
	m_pSwapChain->SetFullscreenState(false, NULL);

	if(m_pImmediateContext) {
		m_pImmediateContext->ClearState();
	}

	_CLOSE(m_pShader);

	_DELETE(m_pTerrain);

	_RELEASE(m_pAlphaEnableBlendingState);
	_RELEASE(m_pAlphaDisableBlendingState);
	_RELEASE(m_pDepthStencil);
	_RELEASE(m_pDepthStencilView);
	_RELEASE(m_pDepthStencilState);
	_RELEASE(m_pDepthDisabledStencilState);
	_RELEASE(m_pRenderTargetView);
	_RELEASE(m_pSwapChain);
	_RELEASE(m_pImmediateContext);
	_RELEASE(m_pd3dDevice);
}

void Render::turnZBufferOn() {
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
}

void Render::turnZBufferOff() {
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthDisabledStencilState, 1);
}

void Render::turnOnAlphaBlending() {
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	m_pImmediateContext->OMSetBlendState(m_pAlphaEnableBlendingState, blendFactor, 0xffffffff);
}

void Render::turnOffAlphaBlending() {
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	m_pImmediateContext->OMSetBlendState(m_pAlphaDisableBlendingState, blendFactor, 0xffffffff);
}

