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
		std::wstring caption;						//Заголовок окна
		int width;									//Ширина клиентской части
		int height;									//Высота клиентской части
		bool resizing;
	};

	class Window {
	private :
		static Window* m_wndthis;

		DescWindow m_desc;							//Описание окна
		InputMgr* m_inputMgr;
		HWND m_hwnd;								//Дескриптор окна
		bool m_isExit;								//Флаг, сообщающий о выходе из окна
		bool m_active;								//Окно активно?
		bool m_minimized;
		bool m_maximized;
		bool m_isResize;							//Если окно изменило размер

		void m_updateWindowState();
	public:
		Window();

		static Window* get() { return m_wndthis; }

		//Создать окно
		bool create(const DescWindow& desc);
		//Обработка событий окна
		void runEvent();
		//Закрыть окно
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
		//Вернуть заголовок окна
		const std::wstring& getCaption() const { return m_desc.caption; }

		//Сообщает, было ли сообщение о выходе
		bool isExit() const { return m_isExit; }
		//Сообщает об активности окна
		bool isActive() const { return m_active; }
		//Сообщает об изменении окна
		//После вызова оповещает окно об обработке события
		bool isResize() { bool ret = m_isResize; m_isResize = false; return ret; }

		//Обработка событий
		LRESULT WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	};

	static LRESULT CALLBACK staticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

