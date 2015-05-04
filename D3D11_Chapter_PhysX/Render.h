#pragma once

#include "DefHeaders.h"
#include "macros.h"
#include "Log.h"
#include "Fps.h"
#include "Terrain.h"
#include "DataStructures.h"
#include "Camera.h"

class Shader;
class ObjectManager;
class Object;

using namespace physx;

class Render {
	private:		
		bool m_createDevice();
		bool m_createDepthStencil();
		bool m_createBlendingState();
		void m_initMatrix();
		void m_resize();

		//Указатели на интерфейсы DirectX11
		ID3D11Device *m_pd3dDevice;
		ID3D11DeviceContext *m_pImmediateContext;
		IDXGISwapChain *m_pSwapChain;
		ID3D11RenderTargetView *m_pRenderTargetView;
		ID3D11Texture2D* m_pDepthStencil;
		ID3D11DepthStencilView* m_pDepthStencilView;
		ID3D11DepthStencilState* m_pDepthStencilState;
		ID3D11DepthStencilState* m_pDepthDisabledStencilState;
		ID3D11BlendState* m_pAlphaEnableBlendingState;
		ID3D11BlendState* m_pAlphaDisableBlendingState;
		ID3D11Buffer* m_pIndexBuffer;
		ID3D11Buffer* m_pVertBuffer;
		ID3D11Buffer* m_pConstMatrixBuffer;
		ID3D11Buffer* m_pConstLightBuffer;
		XMMATRIX m_ortho;
		XMMATRIX m_projection;

		HWND m_hwnd;
		unsigned int m_width;
		unsigned int m_height;

		Light m_light;

		Shader* m_pShader;

		Terrain* m_pTerrain;
		Camera* m_pCamera;

		bool isInit;
		std::vector<Object*> objects;
		ObjectManager* m_pObjectManager;

		void m_renderTerrain();
	public:
		Render();
		virtual ~Render();

		bool hasInited()							{ return isInit;				}

		bool createDevice(HWND hwnd);
		void beginFrame(double dt);
		void endFrame();
		void shutdown();

		ID3D11Device* getDevice()					{ return m_pd3dDevice;			}
		ID3D11DeviceContext* getDeviceContext()		{ return m_pImmediateContext;	}
		Camera* getCamera()							{ return m_pCamera;				}
		unsigned int getWidth()						{ return m_width;				}
		unsigned int getHeight()					{ return m_height;				}
		XMMATRIX getOrtho()							{ return m_ortho;				}
		XMMATRIX getProjection()					{ return m_projection;			}

		bool init();
		bool initObjects(ObjectManager* objectManager);

		bool draw(double dt);
		
		void turnZBufferOn();
		void turnZBufferOff();

		//Вызываются, когда нужно отрендерить текст
		void turnOnAlphaBlending();					
		void turnOffAlphaBlending();

		void* operator new(size_t i)				{ return _aligned_malloc(i, 16);	}
		void operator delete(void* p)				{ _aligned_free(p);					}
	};

