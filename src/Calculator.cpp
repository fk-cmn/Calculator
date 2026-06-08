#include "pch.h"
#include "Calculator.h"
#include "CalculatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCalculatorApp::CCalculatorApp()
{
}

BOOL CCalculatorApp::InitInstance()
{
    CWinApp::InitInstance();

    CCalculatorDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}

// the one and only CCalculatorApp object
CCalculatorApp theApp;
