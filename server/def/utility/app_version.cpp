/********************************************************************
	created:	2014/07/26
	created:	26:7:2014   16:12
	file base:	app_version
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include <string>

// #ifdef WIN32
// #ifndef WIN32_LEAN_AND_MEAN
// #define WIN32_LEAN_AND_MEAN
// #endif

#include <winsock2.h>
#include <windows.h>
#include <tchar.h> 
#include "logic/type_def.hpp"

#pragma comment (lib, "version.lib")


namespace faith
{
#ifdef UNICODE
	std::wstring 
#else
	xstring 
#endif // UNICODE
	app_get_pgroduct_version_string()
	{
		ANSICHAR szpathname[MAX_PATH]={0};
		ANSICHAR strversion[MAX_PATH]={0};
		::GetModuleFileName(NULL, szpathname, sizeof(szpathname));

		DWORD dwverinfosize;
		DWORD dwhnd;
		void* pbuffer;
		VS_FIXEDFILEINFO* pfixedinfo;
		UINT uversionlen;
		dwverinfosize = ::GetFileVersionInfoSize(szpathname, &dwhnd);
		if( dwverinfosize > 0 )
		{
			pbuffer = malloc(dwverinfosize);
			if( pbuffer )
			{
				::GetFileVersionInfo( szpathname, dwhnd, dwverinfosize, pbuffer );
				::VerQueryValue(pbuffer, TEXT("\\"), (VOID**)&pfixedinfo, (UINT*)&uversionlen);
				_stprintf_s( strversion, TEXT("%u.%u.%u.%u")
					, HIWORD(pfixedinfo->dwProductVersionMS)
					, LOWORD(pfixedinfo->dwProductVersionMS)
					, HIWORD(pfixedinfo->dwProductVersionLS)
					, LOWORD(pfixedinfo->dwProductVersionLS) );

				free( pbuffer );
			}
		}
		return strversion;
	}

	void app_set_console_title(void)
	{
		ANSICHAR title[1024] = {0};
		sprintf_s( title, sizeof(title), "command:\"%s\"    pid:[%d]    ver:[%s]"
			, ::GetCommandLine(), ::GetCurrentProcessId(), app_get_pgroduct_version_string().c_str() );
		::SetConsoleTitleA( title );
	}

}

//#endif 
