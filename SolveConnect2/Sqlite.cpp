#include "Sqlite.h"
#include <Windows.h>


Sqlite::Sqlite()
{
	
}

Sqlite::~Sqlite()
{
	closeDb();
	finalize();
}

int Sqlite::openDb(std::string fileName)
{
	return sqlite3_open(fileName.c_str(), &m_sql);
}

int Sqlite::closeDb()
{
	if (m_sql == 0)
		return 0;
	int result = sqlite3_close(m_sql);
	if (result == 0)
		m_sql = 0;
	return result;
}

int Sqlite::execute(std::string stmt)
{
	int result = prepare(stmt);
	if (result != 0)
		return 1;
	result = step();
	if (result != SQLITE_DONE)
		return 2;
	result = finalize();
	return result;
}

int Sqlite::prepare(std::string stmt)
{
	//stmt = strToUtf8(stmt); // not needed apparently
	return sqlite3_prepare(m_sql, stmt.c_str(), -1, &m_stmt, 0);
}

int Sqlite::preparev2(std::string stmt)
{
	return sqlite3_prepare_v2(m_sql, stmt.c_str(), -1, &m_stmt, 0);
}

int Sqlite::resetPreparedStmt()
{
	sqlite3_clear_bindings(m_stmt);
	sqlite3_reset(m_stmt);
	return 0; // eh
}

int Sqlite::step()
{
	return sqlite3_step(m_stmt);
}

int Sqlite::finalize()
{
	if (m_stmt == 0)
		return 0;
	int result = sqlite3_finalize(m_stmt);
	if (result == 0)
		m_stmt = 0;
	return result;
}

std::string Sqlite::strToUtf8(std::string str)
{
	int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(),
		str.length(), nullptr, 0);
	std::wstring utf16_str(size, '\0');
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(),
		str.length(), &utf16_str[0], size);

	int utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
		utf16_str.length(), nullptr, 0,
		nullptr, nullptr);
	std::string utf8_str(utf8_size, '\0');
	WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
		utf16_str.length(), &utf8_str[0], utf8_size,
		nullptr, nullptr);
	return utf8_str;
}