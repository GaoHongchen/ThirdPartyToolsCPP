#include "stdafx.h"
#include "resource.h"
#include "SplashWnd.h"


// CSplashWnd

//IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

BOOL CSplashWnd::c_bShowSplashWnd = FALSE;
CSplashWnd* CSplashWnd::c_pSplashWnd;

CSplashWnd::CSplashWnd()
{

}

CSplashWnd::~CSplashWnd()
{
	// Clear the static window pointer.
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
}


BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CSplashWnd::EnableSplashScreen(BOOL bEnable )
{
	c_bShowSplashWnd = bEnable;
}

void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd )
{
	if (!c_bShowSplashWnd || c_pSplashWnd != NULL)
		return;
	// Allocate a new splash screen, and create the window.
	c_pSplashWnd = new CSplashWnd;
	if (!c_pSplashWnd->Create(pParentWnd))
		delete c_pSplashWnd;
	else
		c_pSplashWnd->UpdateWindow();
}

BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (c_pSplashWnd == NULL)
		return FALSE;

	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
		pMsg->message == WM_SYSKEYDOWN ||
		pMsg->message == WM_LBUTTONDOWN ||
		pMsg->message == WM_RBUTTONDOWN ||
		pMsg->message == WM_MBUTTONDOWN ||
		pMsg->message == WM_NCLBUTTONDOWN ||
		pMsg->message == WM_NCRBUTTONDOWN ||
		pMsg->message == WM_NCMBUTTONDOWN)
	{
		c_pSplashWnd->HideSplashScreen();
		return TRUE; // message handled here
	}

	return FALSE; // message not handled
}

BOOL CSplashWnd::Create(CWnd* pParentWnd )
{
	if (!m_bitmap.LoadBitmap(IDB_SPLASH)) //<span style="font-family: Arial, Helvetica, sans-serif;">IDB_SPLASH ΪҪ�����Ļ���</span>

		return FALSE;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	return CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
}


void CSplashWnd::HideSplashScreen()
{
	// Destroy the window, and update the mainframe.
	DestroyWindow();
	AfxGetMainWnd()->UpdateWindow();
}

void CSplashWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Center the window.
	CenterWindow();

	// Set a timer to destroy the splash screen.
	SetTimer(1, 1000, NULL);

	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);

	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc))
		return;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	// Paint the image.
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);
}

void CSplashWnd::OnTimer(UINT nIDEvent)
{
	// Destroy the splash screen window.
	HideSplashScreen();
} 