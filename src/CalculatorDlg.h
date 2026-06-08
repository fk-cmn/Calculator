// CalculatorDlg.h
#pragma once

class CCalculatorDlg : public CDialogEx
{
public:
    CCalculatorDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CALCULATOR_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedButton(UINT nID);
    afx_msg void OnBnClickedClear();
    afx_msg void OnBnClickedBackspace();
    afx_msg void OnBnClickedEquals();

private:
    CString m_display;
    CEdit* m_pEdit;

    // Expression evaluation
    std::string CStringToStd(const CString& s);
    CString StdToCString(const std::string& s);
    bool IsOperator(char c);
    int Precedence(char op);
    std::vector<std::string> Tokenize(const std::string& expr);
    std::vector<std::string> ToRPN(const std::vector<std::string>& tokens, bool& ok, CString& err);
    double EvalRPN(const std::vector<std::string>& rpn, bool& ok, CString& err);

    void UpdateEdit();
};
