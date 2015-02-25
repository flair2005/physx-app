#pragma once

#include "DefHeaders.h"

	class Log {
	private:
		static Log* m_instance;

		void m_init();
		void m_close();
		void m_print(const char* levtext, const char* text);

		FILE* m_file;
	public:
		Log();
		~Log();

		static Log* get() { return m_instance; }

		void print(const char* message, ...);
		void debug(const char* message, ...);
		void err(const char* message, ...);
	};

