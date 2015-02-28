#include "Window.h"
#include "Framework.h"

Window* Window::m_wndthis = nullptr;

Window::Window(void) :
		m_inputMgr(nullptr),
		m_hwnd(0),
		m_isExit(false),
		m_active(true),
		m_minimized(false),
		m_maximized(false),
		m_isResize(false)	{

			if(!m_wndthis) {
				m_wndthis = this;
			} else {
				Log::get()->err("Window уже был создан");
			}

}

bool Window::create(const DescWindow& desc) {
	Log::get()->debug("Window Create");
	m_desc = desc;

	WNDCLASSEXW wnd;

	wnd.cbSize = sizeof(WNDCLASSEXW);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = staticWndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = 0;
	wnd.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wnd.hIconSm = wnd.hIcon;
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = L"D3D11F";
	wnd.cbSize = sizeof(WNDCLASSEX);

	if(!RegisterClassEx(&wnd)) {
		Log::get()->err("Не удалось зарегистрировать окно");
		return false;
	}

	RECT rect = {0, 0, m_desc.width, m_desc.height};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

	long lwidth = rect.right - rect.left;
	long lheight = rect.bottom - rect.top;

	long lleft = (long)m_desc.posx;
	long ltop = (long)m_desc.posy;

	m_hwnd = CreateWindowEx(NULL, L"D3D11F", m_desc.caption.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								lleft, ltop, lwidth, lheight, NULL, NULL, NULL, NULL);

	if(!m_hwnd) {
		Log::get()->err("Не удалось создать окно");
		return false;
	}

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	return true;
}

void Window::runEvent() {
	MSG msg;								//События окна
	//Просматриваем поступившие сообщения
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::Close() {
	if(m_hwnd) {
		DestroyWindow(m_hwnd);
	}
	m_hwnd = nullptr;

	Log::get()->debug("Window Close");
}

LRESULT Window::WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
	switch(nMsg) {
	case WM_CREATE:
		return 0;
	case WM_CLOSE:
		m_isExit = true;
		return 0;
	case WM_ACTIVATE:
		if(LOWORD(wParam) != WA_INACTIVE) {
			m_active = true;
		} else {
			m_active = false;
		}
		return 0;
	case WM_MOVE:
		m_desc.posx = LOWORD(lParam);
		m_desc.posy = HIWORD(lParam);
		m_updateWindowState();
		return 0;
	case WM_SIZE:
		if(!m_desc.resizing) {
			return 0;
		}
		m_desc.width = LOWORD(lParam);
		m_desc.height = HIWORD(lParam);
		m_isResize = true;
		if(wParam == SIZE_MINIMIZED) {
			m_active = false;
			m_minimized = true;
			m_maximized = false;
		} else if(wParam == SIZE_MAXIMIZED) {
			m_active = true;
			m_minimized = false;
			m_maximized = true;
		} else if(wParam == SIZE_RESTORED) {
			if(m_minimized) {
				m_active = true;
				m_minimized = false;
			} else if(m_maximized) {
				m_active = true;
				m_maximized = false;
			}
		}
		m_updateWindowState();
		return 0;
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MOUSEWHEEL:
	case WM_KEYDOWN:
	case WM_KEYUP:
		if(m_inputMgr) {
			m_inputMgr->run(nMsg, wParam, lParam);
		}
		return 0;
	}
	return DefWindowProcW(hwnd, nMsg, wParam, lParam);
}

void Window::setInputMgr(InputMgr* inputMgr) {
	m_inputMgr = inputMgr;
	m_updateWindowState();
}

void Window::m_updateWindowState() {
	RECT clientRect;
	clientRect.left = m_desc.posx;
	clientRect.top = m_desc.posy;
	clientRect.right = m_desc.width;
	clientRect.bottom = m_desc.height;
	if(m_inputMgr) {
		m_inputMgr->setWinRect(clientRect);
	}
}

LRESULT CALLBACK staticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
	return Window::get()->WndProc(hwnd, nMsg, wParam, lParam);
}

