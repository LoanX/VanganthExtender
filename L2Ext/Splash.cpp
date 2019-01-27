// SPLASH.cpp: implementation of the SPLASH class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SPLASH::SPLASH()
{

}

SPLASH::~SPLASH()
{
 DestroyWindow(hSplashWnd);
}

void SPLASH::Init(HWND hWnd,HINSTANCE hInst,int resid)
{
 hParentWindow=hWnd;
 hSplashWnd=CreateWindowEx(WS_EX_TRANSPARENT,_T("STATIC"),_T(""),
     WS_POPUP|SS_BITMAP,300,300,420,336,hWnd,NULL,hInst,NULL);
 SendMessage(hSplashWnd,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(resid)));
 this->SHOWING = FALSE;
}

void SPLASH::Show()
{
  //get size of hSplashWnd (width and height)
  int x,y;
  int pwidth,pheight;
  int tx,ty;
  HDWP windefer;
  RECT rect,prect;
  GetClientRect(hSplashWnd,&rect);
  x=rect.right;y=rect.bottom;
  //get parent screen coordinates
  prect.top = 1;
  prect.left = 1;
  prect.bottom = GetSystemMetrics(SM_CYFULLSCREEN);
  prect.right = GetSystemMetrics(SM_CXFULLSCREEN);

  //center splash window on parent window
  pwidth=prect.right-prect.left;
  pheight=prect.bottom - prect.top;



  tx=(pwidth/2) - (x/2);
  ty=(pheight/2) - (y/2);

  tx+=prect.left;
  ty+=prect.top;


  windefer=BeginDeferWindowPos(1);
  DeferWindowPos(windefer,hSplashWnd,HWND_NOTOPMOST,tx,ty,50,50,SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOZORDER);
  EndDeferWindowPos(windefer);

  ShowWindow(hSplashWnd,SW_SHOWNORMAL);
  UpdateWindow(hSplashWnd);
  this->SHOWING = TRUE;
}

void SPLASH::Hide()
{
  ShowWindow(hSplashWnd,SW_HIDE);
  this->SHOWING = FALSE;
}