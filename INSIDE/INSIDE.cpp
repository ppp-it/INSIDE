/*!
 *
 * \file INSIDE.cpp
 * \brief создание меню и связывание пунктов меню с обработчиком
 * 
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
 // INSIDE.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "INSIDE.h"
#include <afxdllx.h>

#ifdef _MANAGED
#error Please read instructions in SMAPIex.cpp to compile with /clr
// If you want to add /clr to your project you must do the following:
//	1. Remove the above include for afxdllx.h
//	2. Add a .cpp file to your project that does not have /clr thrown and has
//	   Precompiled headers disabled, with the following text:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static AFX_EXTENSION_MODULE SMAPIexDLL = { NULL, NULL };

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//CCustomMenu* CMFCFrameWnd::m_CustomMenu;
CSMAPIex theApp;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("INSIDE.DLL Initializing!\n");

		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SMAPIexDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(SMAPIexDLL);
		
		theApp.InitInstance();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("INSIDE.DLL Terminating!\n");

		theApp.ExitInstance();

		// Terminate the library before destructors are called
		AfxTermExtensionModule(SMAPIexDLL);
	}
	return 1;   // ok
}

// SMAPIex.cpp : implementation file
////////////////////////////////////////////////////////////////////////////////
// CSMAPIex

CSMAPIex::CSMAPIex()
{
	m_pMFCWinApp = NULL;
	m_pMFCFrameWnd = NULL;
	m_MainMenu = NULL;
	m_pWaitCreateFrameThread = NULL;
}

CSMAPIex::~CSMAPIex()
{
	if (m_MainMenu)
		_delete(m_MainMenu);
}

BOOL CSMAPIex::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	HANDLE hTread = NULL;
	// Запустите поток для создания меню.
	hTread = chBEGINTHREADEX(NULL, 0, WaitCreateFrameThreadProc, (LPVOID)this, 0, NULL);
	if ( hTread )
		m_MainMenu = new CCustomMenu();
	else
		::AfxMessageBox(_T("Error run thread"), MB_OK || MB_ICONERROR);

	return TRUE;
}

int CSMAPIex::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	_delete(m_MainMenu);

	//ITERATOR _Element;
	//for (_Element = m_MsgMappWnd.begin(); _Element != m_MsgMappWnd.end(); _Element++)	{
	//	if (*_Element)		_delete (*_Element);
	//}
	//m_MsgMappWnd.clear();

	return TRUE;
}

// Поток, ожидающий запуск главного окна приложения
CWinThread* CSMAPIex::CreateThread()
{
	// Запустите поток для создания меню.
	m_pWaitCreateFrameThread = AfxBeginThread((AFX_THREADPROC)WaitCreateFrameThreadProc, (LPVOID)this);//, 0, NULL, CREATE_SUSPENDED);
	if ( !m_pWaitCreateFrameThread )
		::AfxMessageBox(_T("Error run thread"), MB_OK | MB_ICONERROR);
	//else
	//	m_pWaitCreateFrameThread->ResumeThread();
	
	return m_pWaitCreateFrameThread;
}

DWORD CSMAPIex::GetAndCreateMenu(LPCTSTR strWindowCaption /*= NULL*/)
{
	DWORD dwRes = -1;

#ifdef _DEBUG
	::Sleep( 2000 );
#endif

	AFX_MANAGE_STATE( AfxGetAppModuleState() );
	
	m_pMFCWinApp = static_cast<CMFCWinApp*>(AfxGetApp());/*m_pModState->m_pCurrentWinApp*/

	if (m_pMFCWinApp)
		m_pMFCFrameWnd = static_cast<CMFCFrameWnd*>(AfxGetApp()->GetMainWnd());/*m_pModState->m_thread->m_pCurrentWinThread->GetMainWnd()*/

	if (m_pMFCFrameWnd){
		
		m_WinAppMsgMap.initialization(m_pMFCWinApp);
		if ( m_WinAppMsgMap.CreateMessageMap() )
			dwRes = m_pMFCFrameWnd->AttachCustomMenu(this);
	}

	return dwRes;
}


////////////////////////////////////////////////////////////////////////////////
//	CMFCWinApp

BEGIN_MESSAGE_MAP(CMFCWinApp, CWinApp)
	ON_COMMAND(ID_32771, &CMFCWinApp::OnSubMenu_1)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
//  реализация для менюшки
void CMFCWinApp::OnSubMenu_1()
{
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->GetMainWnd());
	CView * pView = static_cast<CView*>(pFrame->GetActiveView());
	//CMainFrame* pFrame=static_cast<CMainFrame*>(AfxGetMainWnd());
	//CView* pView=static_cast<CView*>(pFrame->GetActiveView());
	
	HWND hMainFrame = ((CView *)pFrame->GetActiveView())->GetActiveWindow()->m_hWnd;
	CString str;
	int iWndLenht = ::GetWindowTextLength( hMainFrame );
	::GetWindowText( hMainFrame, str.GetBufferSetLength(iWndLenht), MAX_PATH);

	AfxMessageBox(str);
}


/////////////////////////////////////////////////////////////////////////////
// CMFCFrameWnd

BEGIN_MESSAGE_MAP(CMFCFrameWnd, CWnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

// Call AttachCustomMenu once
//   добавляет мое меню
DWORD CMFCFrameWnd::AttachCustomMenu(CSMAPIex* pApp)
{
	DWORD dwRes = -1;

	CMenu* pMenuBar = GetMenu();
	ENSURE(pMenuBar != NULL);
	//TCHAR szString[256];     // don't change the string
	
	m_CustomMenu = pApp->m_MainMenu;

	if ( m_CustomMenu && IsMenu(m_CustomMenu->m_hMenu) ){
		dwRes &= m_CustomMenu->InsertMenu(-1, MF_BYPOSITION, ID_32771, _T("Подменю 1"));
		dwRes &= m_CustomMenu->InsertMenu(-1, MF_BYPOSITION, ID_32772, _T("Подменю 2"));
		if ( dwRes != 0 ){
#           pragma warning( disable : 4311)
			dwRes = pMenuBar->InsertMenu(1, MF_BYPOSITION | MF_POPUP, (UINT)m_CustomMenu->m_hMenu, _T("Для Игоря"));
#           pragma warning( default : 4311)
			if ( dwRes ){

				//pMenuBar->GetMenuString(1, szString, sizeof(szString), MF_BYPOSITION);
				//VERIFY(GetMenu()->ModifyMenu(1, MF_BYPOSITION | MF_POPUP, (UINT_PTR)m_CustomMenu->m_hMenu, szString));

				dwRes = 0;
				DrawMenuBar();
			}
		}
	}
	return dwRes;
}


/////////////////////////////////////////////////////////////////////////////
// CCustomMenu

CCustomMenu::CCustomMenu()
{
	VERIFY(CreateMenu());
}

CCustomMenu::~CCustomMenu()
{
	Detach();
	ASSERT(m_hMenu == NULL);    // defaul CMenu::~CMenu will destroy
}


/////////////////////////////////////////////////////////////////////////////
// tread func

UINT __cdecl WaitCreateFrameThreadProc(LPVOID lpParameter){
	ASSERT_POINTER(lpParameter,CSMAPIex*);
	
	CSMAPIex *pApp = (CSMAPIex*)lpParameter;
	
	if ( !pApp->WaitCreateFrame() ){
        ::Sleep( 1000 );
		// Запустите поток для создания меню.
		if ( !pApp->CreateThread() )
			::AfxMessageBox(_T("Error run thread"), MB_OK | MB_ICONERROR);

		return 0L;
	}

	CWinThread  *pThisThread  = AfxGetThread();


#ifdef _DEBUG
	pApp->GetAndCreateMenu(_T("test"));
#else
	pApp->GetAndCreateMenu();
#endif

	pThisThread->ExitInstance();

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// CMessageMap

template <class ClassWnd>
CMessageMap<ClassWnd>::CMessageMap()
{
	m_pWnd = NULL;
	m_pSmMessageMap = NULL;
	m_hHeap = NULL;
}

template <class ClassWnd>
void CMessageMap<ClassWnd>::initialization(LPClassWnd pWnd)
{
	VERIFY(pWnd);
	m_pWnd = pWnd;
	m_pSmMessageMap = new AFX_MSGMAP;
	m_hHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, MAX_PATH, 0);
}

template <class ClassWnd>
CMessageMap<ClassWnd>::~CMessageMap(){
	if (m_hHeap){
		::HeapFree(m_hHeap, 0, (LPVOID)m_pEntryArrayNew);
		m_hHeap = NULL;
	}
	if (m_pSmMessageMap){
		_delete(m_pSmMessageMap);
	}
}

template <class ClassWnd>
int CMessageMap<ClassWnd>::GetMsgCount(AFX_MSGMAP* pMessageMap)
{
	int iMsgCount = 0;
	
	AFX_MSGMAP_ENTRY* lpMsgEntryCur = (AFX_MSGMAP_ENTRY*)pMessageMap->lpEntries;
	while (lpMsgEntryCur->nSig != AfxSig_end){
		iMsgCount++;
		lpMsgEntryCur++;
	}
	return iMsgCount;
}

template <class ClassWnd>
void CMessageMap<ClassWnd>::CopyMsgEntry(AFX_MSGMAP_ENTRY* pDest, const AFX_MSGMAP_ENTRY* pSrc, DWORD dwSizeToCopy)
{
	memcpy((void*)pDest,(void*)(AFX_MSGMAP_ENTRY*)pSrc, dwSizeToCopy);
}

template <class ClassWnd>
BOOL CMessageMap<ClassWnd>::MapMessageEntry(AFX_MSGMAP* pMessageMapOld, AFX_MSGMAP* pMessageMapSrc)
{
	BOOL blRes = false;
	DWORD dwProtect;
	int iSizeEntry = sizeof(AFX_MSGMAP_ENTRY);

	// получим количестово обработчиков из карт сообщений
    int iMsgCountOld = GetMsgCount(pMessageMapOld);          // из карты в exe
    int iMsgCountSrc = GetMsgCount(pMessageMapSrc);         // из карты в dll
    int iMsgCountNew = iMsgCountOld + iMsgCountSrc + 1+1;   // размер обоих карт + завершающий

	m_pEntryArrayNew = AllocMemory (iMsgCountNew * iSizeEntry);
	
	// скопируем сначало оригинальную карту, затем из шаблонного класса, затем завершающую
	CopyMsgEntry( m_pEntryArrayNew, pMessageMapOld->lpEntries, iMsgCountOld*iSizeEntry);
	CopyMsgEntry((m_pEntryArrayNew + iMsgCountOld), pMessageMapSrc->lpEntries, iMsgCountSrc*iSizeEntry);
	CopyMsgEntry((m_pEntryArrayNew + iMsgCountOld + iMsgCountSrc), (pMessageMapOld->lpEntries+iMsgCountOld), iSizeEntry);
	
	// заполним первый элемент (&TheBaseClass::GetThisMessageMap) структуры AFX_MSGMAP в указателе m_pSmMessageMap
	VirtualProtect((LPVOID)(char*)m_pSmMessageMap, sizeof(AFX_MSGMAP*), PAGE_READWRITE, &dwProtect);
	*(AFX_MSGMAP*)m_pSmMessageMap = *(AFX_MSGMAP*)pMessageMapOld;
	VirtualProtect((LPVOID)m_pSmMessageMap, sizeof(AFX_MSGMAP*), dwProtect, &dwProtect);
	
	// заполним второй элемент (const AFX_MSGMAP_ENTRY* lpEntries) структуры AFX_MSGMAP в указателе m_pSmMessageMap
	(LPCENTRIES)m_pSmMessageMap->lpEntries = (LPCENTRIES)m_pEntryArrayNew;

	// изменим значение по указателю структуры AFX_MSGMAP из самого приложения (из exe),
	// в результате lpEntries ссылается как и m_pEntryArrayNew, а значит на новую карту сообщений
	VirtualProtect((LPVOID)(char*)pMessageMapOld, sizeof(AFX_MSGMAP*), PAGE_READWRITE, &dwProtect);
	*(AFX_MSGMAP*)pMessageMapOld = *(AFX_MSGMAP*)m_pSmMessageMap;
	VirtualProtect((LPVOID)pMessageMapOld, sizeof(AFX_MSGMAP*), dwProtect, &dwProtect);

#ifdef _DEBUG
	AFX_MSGMAP_ENTRY* lpMsgEntryNew = (AFX_MSGMAP_ENTRY*)pMessageMapOld->lpEntries;
	while (lpMsgEntryNew->nSig != AfxSig_end){
		lpMsgEntryNew++;
	}
#endif

	blRes = (char*)pMessageMapOld->lpEntries == (char*)m_pEntryArrayNew;

	return blRes;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


///////////////////////////////// End of File /////////////////////////////////