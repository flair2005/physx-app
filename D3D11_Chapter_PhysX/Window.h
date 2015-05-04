#pragma once

#include "InputMgr.h"
class Framework;

	struct DescWindow {
		DescWindow() : 
					caption(L""),
					width(1024),
					height(768),
					posx(200),
					posy(20),
					resizing(true)
		{}

		int posx;
		int posy;
		std::wstring caption;						//��������� ����
		int width;									//������ ���������� �����
		int height;									//������ ���������� �����
		bool resizing;
	};

	class Window {
	private :
		static Window* m_wndthis;

		DescWindow m_desc;							//�������� ����
		InputMgr* m_inputMgr;
		HWND m_hwnd;								//���������� ����
		bool m_isExit;								//����, ���������� � ������ �� ����
		bool m_active;								//���� �������?
		bool m_minimized;
		bool m_maximized;
		bool m_isResize;							//���� ���� �������� ������

		void m_updateWindowState();
	public:
		Window();

		static Window* get() { return m_wndthis; }

		//������� ����
		bool create(const DescWindow& desc);
		//��������� ������� ����
		void runEvent();
		//������� ����
		void Close();

		void setInputMgr(InputMgr* inputmgr);

		HWND getHWND() const		{ return m_hwnd; }
		HINSTANCE getHInstance()	{ 
			HWND hwndC = GetConsoleWindow();
			HINSTANCE hInstC = GetModuleHandle(0);
			return hInstC;
		}
		int getLeft() const			{ return m_desc.posx; }
		int getTop() const			{ return m_desc.posy; }
		int getWidth() const		{ return m_desc.width; }
		int getHeidht() const		{ return m_desc.height; }
		//������� ��������� ����
		const std::wstring& getCaption() const { return m_desc.caption; }

		//��������, ���� �� ��������� � ������
		bool isExit() const { return m_isExit; }
		//�������� �� ���������� ����
		bool isActive() const { return m_active; }
		//�������� �� ��������� ����
		//����� ������ ��������� ���� �� ��������� �������
		bool isResize() { bool ret = m_isResize; m_isResize = false; return ret; }

		//��������� �������
		LRESULT WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	};

	static LRESULT CALLBACK staticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

