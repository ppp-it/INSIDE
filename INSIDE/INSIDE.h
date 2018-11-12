/*!
 *
 * \file INSIDE.h
 * \brief создание меню и связывание пунктов меню с обработчиком
 *
 * Copyright (C) 2018 Pochepko PP.
 * Contact: ppp.it@hotmail.com
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source
 *    distribution.
 *
 */
 //-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif
#  pragma comment(lib, "User32.Lib")

#include "resource.h"		// main symbols
//#include <Shlwapi.h>
//#pragma comment (lib, "Shlwapi.lib")
#include <process.h>	/* _beginthread, _endthread */
using namespace std;
#include <vector>

UINT __cdecl WaitCreateFrameThreadProc(LPVOID lpParameter);


#define _delete(pointer) if (pointer) delete pointer;\
					   pointer = NULL
class CSMAPIex;
class CMFCWinApp;
class CMFCFrameWnd;
class CCustomMenu;
template <typename ClassWnd> class CMessageMap;
typedef CMessageMap<CMFCWinApp> WinAppMsgMap;

typedef unsigned    (_stdcall *PTHREAD_START) (void *);
typedef const AFX_MSGMAP* (PASCAL*  const LPCGETBASEMAP ) (void);
typedef AFX_MSGMAP* (PASCAL*  LPGETBASEMAP ) (void);
typedef const AFX_MSGMAP_ENTRY* LPCENTRIES;
typedef AFX_MSGMAP_ENTRY* LPENTRIES;

//typedef struct tag_WND_MESSAGE_MAP {
//	LPVOID pClass;
//	HWND   hWnd;
//	HANDLE hHeap;
//} TGWNDMM, *LPTGWNDMM;

#define chBEGINTHREADEX(psa, cbStack, pfnStartAddr, \
   pvParam, fdwCreate, pdwThreadID)                 \
      ((HANDLE) _beginthreadex(                     \
         (void *) (psa),                            \
         (unsigned) (cbStack),                      \
         (PTHREAD_START) (pfnStartAddr),            \
         (void *) (pvParam),                        \
         (unsigned) (fdwCreate),                    \
         (unsigned *) (pdwThreadID)))




/////////////////////////////////////////////////////////////////////////////
/// \brief CMessageMap - меняет карту сообщений.
template <typename ClassWnd>
class CMessageMap
{
	typedef ClassWnd* LPClassWnd;

public:
	CMessageMap();
	~CMessageMap();

public:

	void initialization(LPClassWnd pWnd);

	// добавим в карту сообщений обработку своих сообщений
	BOOL MapMessageEntry(AFX_MSGMAP* pMessageMapOld, AFX_MSGMAP* pMessageMapSrc);
	
	// добавим в карту сообщений обработку своих сообщений
	BOOL CreateMessageMap() {
		return ( MapMessageEntry(SmGetMessageMap(), GetThisMessageMap()) );
	};


protected:
	const AFX_MSGMAP* m_pSmMessageMap;

	// получим карту сообщений из самого приложения (из exe)
	AFX_MSGMAP* SmGetMessageMap() {
		return ( (AFX_MSGMAP*)m_pWnd->SmGetMessageMap() );
	};
	
	// получим карту сообщений созданную в этой длл
	AFX_MSGMAP* PASCAL GetThisMessageMap() {
		return ((AFX_MSGMAP*)ClassWnd::SmGetThisMessageMap());
	};

	// подсчитаем количество обрабатываемых событий в карте сообщений
	int GetMsgCount(AFX_MSGMAP* pMessageMap);

	// скопируем содержимое массива структур AFX_MSGMAP_ENTRY в новый массив карты сообщений
	void CopyMsgEntry(AFX_MSGMAP_ENTRY* pDest, const AFX_MSGMAP_ENTRY* pSrc, DWORD dwSizeToCopy);

	// распределение памяти для нового массива карты сообщений
	AFX_MSGMAP_ENTRY* AllocMemory (DWORD dwSize) {
		return (AFX_MSGMAP_ENTRY*) HeapAlloc (m_hHeap, HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, dwSize);
	};
	AFX_MSGMAP_ENTRY* ReAllocMemory (AFX_MSGMAP_ENTRY* &pointer, DWORD dwSize) {
		return (AFX_MSGMAP_ENTRY*) HeapReAlloc (m_hHeap, HEAP_GENERATE_EXCEPTIONS, pointer, dwSize);
	};


	AFX_MSGMAP_ENTRY* m_pEntryArrayNew;

private:
	// указатель на класс, карту соббщений которого надо изменить
	LPClassWnd m_pWnd;
	// Дескриптор к куче.
	HANDLE m_hHeap;
};




///////////////////////////////////////////////////////////////////////////////
/// \brief - указатель на производный класс CWinApp приложения.
///
/// CWinApp - класс приложения, которое исполняется в exe-файле\n
/// куда планируем внедрить dll и в которое (приложение) добавим свое меню из dll.
///
class CMFCWinApp : public CWinApp
{
public:
	// получим карту сообщений основного окна приложения
	AFX_MSGMAP* SmGetMessageMap() {
		return ( (AFX_MSGMAP*)GetMessageMap() );
	};
	// получим карту сообщений этого класса
	static const AFX_MSGMAP* PASCAL SmGetThisMessageMap(){
		return (CMFCWinApp::GetThisMessageMap()); 
	};

// Overrides
public:
	//{{AFX_MSG(CMFCFrameWnd)
	afx_msg void OnSubMenu_1();						// Событие моего меню "Подменю 1"
    // сдесь описываем обработчики для пунктов менюшки
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////
// Main Window
//   used as the context for running all the tests

class CMFCFrameWnd : public CWnd	//CFrameWnd
{
public:
	// добавляет меню из длл в приложение (exe, куда внедрили dll)
	DWORD AttachCustomMenu(CSMAPIex* pApp);

protected:
	CCustomMenu*  m_CustomMenu;

// Implementation
public:
	//{{AFX_MSG(CMFCFrameWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CustomMenu - used for custom menu test
//   included here to show how it should be embedded as a member of the
//   main frame window that uses it.  The implementation is in custmenu.cpp

class CCustomMenu : public CMenu
{
public:
// Operations

// Implementation
	CCustomMenu();
	virtual ~CCustomMenu();
}; 



/////////////////////////////////////////////////////////////////////////////
// CSMAPIex

class CSMAPIex
{
	//typedef vector <LPTGWNDMM> TGWNDMMVECTOR;
	//typedef vector <LPTGWNDMM>::iterator ITERATOR;

public:
	CSMAPIex();           // protected constructor used by dynamic creation
	virtual ~CSMAPIex();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// Поток, ожидающий запуск главного окна приложения
	CWinThread* CreateThread();
	CWinThread* WaitCreateFrame() const throw() { return m_pWaitCreateFrameThread; };
	CCustomMenu* GetCustomMenu()  const throw() { return m_MainMenu; };

	DWORD GetAndCreateMenu(LPCTSTR strWindowCaption = NULL);

//protected:
//	TGWNDMMVECTOR m_MsgMappWnd;

private:
	WinAppMsgMap m_WinAppMsgMap;
	CMFCWinApp* m_pMFCWinApp;
	CMFCFrameWnd* m_pMFCFrameWnd;
	CCustomMenu*  m_MainMenu;
	CWinThread *m_pWaitCreateFrameThread;

	friend class CMFCFrameWnd;
};



/*
struct AFX_MSGMAP_ENTRY;       // declared below after CWnd

struct AFX_MSGMAP
{
	const AFX_MSGMAP* (PASCAL* pfnGetBaseMap)();
	const AFX_MSGMAP_ENTRY* lpEntries;
};

struct AFX_MSGMAP_ENTRY
{
	UINT nMessage;   // windows message
	UINT nCode;      // control code or WM_NOTIFY code
	UINT nID;        // control ID (or 0 for windows messages)
	UINT nLastID;    // used for entries specifying a range of control id's
	UINT_PTR nSig;       // signature type (action) or pointer to message #
	AFX_PMSG pfn;    // routine to call (or special value)

#define BEGIN_MESSAGE_MAP(theClass, baseClass) \
	PTM_WARNING_DISABLE \
	const AFX_MSGMAP* theClass::GetMessageMap() const \
		{ return GetThisMessageMap(); } \
	const AFX_MSGMAP* PASCAL theClass::GetThisMessageMap() \
	{ \
		typedef theClass ThisClass;						   \
		typedef baseClass TheBaseClass;					   \
		static const AFX_MSGMAP_ENTRY _messageEntries[] =  \
		{

#define END_MESSAGE_MAP() \
		{0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 } \
		}; \
		static const AFX_MSGMAP messageMap = \
		{ &TheBaseClass::GetThisMessageMap, &_messageEntries[0] }; \
		return &messageMap; \
	}								  \
	PTM_WARNING_RESTORE
};
	*/

///////////////////////////////// End of File /////////////////////////////////