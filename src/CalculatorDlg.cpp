#include "pch.h"
#include "Calculator.h"
#include "CalculatorDlg.h"
#include "res\resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCalculatorDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_COMMAND_RANGE(IDC_BTN_0, IDC_BTN_RPAREN, &CCalculatorDlg::OnBnClickedButton)
    ON_BN_CLICKED(IDC_BTN_CLEAR, &CCalculatorDlg::OnBnClickedClear)
    ON_BN_CLICKED(IDC_BTN_BS, &CCalculatorDlg::OnBnClickedBackspace)
    ON_BN_CLICKED(IDC_BTN_EQ, &CCalculatorDlg::OnBnClickedEquals)
END_MESSAGE_MAP()

CCalculatorDlg::CCalculatorDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CALCULATOR_DIALOG, pParent), m_display(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BOOL CCalculatorDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DISPLAY);
    m_display = _T("");
    UpdateEdit();

    return TRUE;
}

void CCalculatorDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CCalculatorDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CCalculatorDlg::OnBnClickedButton(UINT nID)
{
    CString text;
    switch (nID) {
    case IDC_BTN_0: text = _T("0"); break;
    case IDC_BTN_1: text = _T("1"); break;
    case IDC_BTN_2: text = _T("2"); break;
    case IDC_BTN_3: text = _T("3"); break;
    case IDC_BTN_4: text = _T("4"); break;
    case IDC_BTN_5: text = _T("5"); break;
    case IDC_BTN_6: text = _T("6"); break;
    case IDC_BTN_7: text = _T("7"); break;
    case IDC_BTN_8: text = _T("8"); break;
    case IDC_BTN_9: text = _T("9"); break;
    case IDC_BTN_DOT: text = _T("."); break;
    case IDC_BTN_ADD: text = _T("+"); break;
    case IDC_BTN_SUB: text = _T("-"); break;
    case IDC_BTN_MUL: text = _T("*"); break;
    case IDC_BTN_DIV: text = _T("/"); break;
    case IDC_BTN_LPAREN: text = _T("("); break;
    case IDC_BTN_RPAREN: text = _T(")"); break;
    case IDC_BTN_SIN: text = _T("sin("); break;
    case IDC_BTN_COS: text = _T("cos("); break;
    case IDC_BTN_TAN: text = _T("tan("); break;
    default: text = _T(""); break;
    }
    m_display += text;
    UpdateEdit();
}

void CCalculatorDlg::OnBnClickedClear()
{
    m_display = _T("");
    UpdateEdit();
}

void CCalculatorDlg::OnBnClickedBackspace()
{
    int len = m_display.GetLength();
    if (len > 0) {
        m_display = m_display.Left(len - 1);
        UpdateEdit();
    }
}

void CCalculatorDlg::OnBnClickedEquals()
{
    UpdateEdit();
    std::string expr = CStringToStd(m_display);
    bool ok = true;
    CString err;
    auto tokens = Tokenize(expr);
    auto rpn = ToRPN(tokens, ok, err);
    if (!ok) {
        AfxMessageBox(err);
        return;
    }
    double result = EvalRPN(rpn, ok, err);
    if (!ok) {
        AfxMessageBox(err);
        return;
    }
    CString out;
    out.Format(_T("%.12g"), result);
    m_display = out;
    UpdateEdit();
}

// Utility implementations
std::string CCalculatorDlg::CStringToStd(const CString& s)
{
    CT2CA pszConvertedAnsiString(s);
    std::string strStd(pszConvertedAnsiString);
    return strStd;
}

CString CCalculatorDlg::StdToCString(const std::string& s)
{
    CString c;
    c.SetString(s.c_str());
    return c;
}

bool CCalculatorDlg::IsOperator(char c)
{
    return c=='+'||c=='-'||c=='*'||c=='/';
}

int CCalculatorDlg::Precedence(char op)
{
    if (op=='+'||op=='-') return 1;
    if (op=='*'||op=='/') return 2;
    return 0;
}

std::vector<std::string> CCalculatorDlg::Tokenize(const std::string& expr)
{
    std::vector<std::string> out;
    size_t i=0;
    while (i<expr.size()) {
        if (isspace((unsigned char)expr[i])) { i++; continue; }
        if (isdigit((unsigned char)expr[i])||expr[i]=='.') {
            size_t j=i+1;
            while (j<expr.size() && (isdigit((unsigned char)expr[j])||expr[j]=='.')) j++;
            out.push_back(expr.substr(i,j-i));
            i=j;
            continue;
        }
        if (isalpha((unsigned char)expr[i])) {
            size_t j=i+1;
            while (j<expr.size() && isalpha((unsigned char)expr[j])) j++;
            out.push_back(expr.substr(i,j-i));
            i=j;
            continue;
        }
        // single char tokens
        out.push_back(std::string(1, expr[i]));
        i++;
    }
    return out;
}

std::vector<std::string> CCalculatorDlg::ToRPN(const std::vector<std::string>& tokens, bool& ok, CString& err)
{
    ok = true;
    err = _T("");
    std::vector<std::string> output;
    std::stack<std::string> ops;
    for (size_t i=0;i<tokens.size();++i) {
        const std::string& t = tokens[i];
        if (t.empty()) continue;
        if (isdigit((unsigned char)t[0]) || (t.size()>1 && t[0]=='.')) {
            output.push_back(t);
        } else if (isalpha((unsigned char)t[0])) {
            // function
            ops.push(t);
        } else if (t=="," ) {
            while (!ops.empty() && ops.top()!="(") {
                output.push_back(ops.top()); ops.pop();
            }
            if (ops.empty()) { ok=false; err=_T("语法错误：函数参数或逗号位置不正确"); return {} }
        } else if (t=="+") {
            while (!ops.empty() && IsOperator(ops.top()[0]) && Precedence(ops.top()[0])>=Precedence('+')) { output.push_back(ops.top()); ops.pop(); }
            ops.push(t);
        } else if (t=="-") {
            while (!ops.empty() && IsOperator(ops.top()[0]) && Precedence(ops.top()[0])>=Precedence('-')) { output.push_back(ops.top()); ops.pop(); }
            ops.push(t);
        } else if (t=="*") {
            while (!ops.empty() && IsOperator(ops.top()[0]) && Precedence(ops.top()[0])>=Precedence('*')) { output.push_back(ops.top()); ops.pop(); }
            ops.push(t);
        } else if (t=="/") {
            while (!ops.empty() && IsOperator(ops.top()[0]) && Precedence(ops.top()[0])>=Precedence('/')) { output.push_back(ops.top()); ops.pop(); }
            ops.push(t);
        } else if (t=="(") {
            ops.push(t);
        } else if (t==")") {
            while (!ops.empty() && ops.top()!="(") { output.push_back(ops.top()); ops.pop(); }
            if (ops.empty()) { ok=false; err=_T("语法错误：括号不匹配"); return {} }
            ops.pop(); // pop '('
            if (!ops.empty() && isalpha((unsigned char)ops.top()[0])) { output.push_back(ops.top()); ops.pop(); }
        } else {
            ok=false; err=_T("未知的令牌"); return {};
        }
    }
    while (!ops.empty()) {
        if (ops.top()=="(" || ops.top()==")") { ok=false; err=_T("语法错误：括号不匹配"); return {} }
        output.push_back(ops.top()); ops.pop();
    }
    return output;
}

static double ToDouble(const std::string& s) {
    try { return std::stod(s); } catch(...) { return 0.0; }
}

double CCalculatorDlg::EvalRPN(const std::vector<std::string>& rpn, bool& ok, CString& err)
{
    ok = true; err = _T("");
    std::stack<double> st;
    for (const auto& tok : rpn) {
        if (tok.empty()) continue;
        if (isdigit((unsigned char)tok[0]) || (tok.size()>1 && (isdigit((unsigned char)tok[1])||tok[0]=='.'))) {
            st.push(ToDouble(tok));
        } else if (isalpha((unsigned char)tok[0])) {
            if (st.empty()) { ok=false; err=_T("语法错误：函数缺少参数"); return 0; }
            double v = st.top(); st.pop();
            double rad = v * M_PI / 180.0; // input degrees
            if (tok=="sin") st.push(sin(rad));
            else if (tok=="cos") st.push(cos(rad));
            else if (tok=="tan") {
                double c = cos(rad);
                if (fabs(c)<1e-12) { ok=false; err=_T("数学错误：tan 定义域异常（cos=0）"); return 0; }
                st.push(tan(rad));
            } else { ok=false; err=_T("未知函数"); return 0; }
        } else if (tok=="+" || tok=="-" || tok=="*" || tok=="/") {
            if (st.size()<2) { ok=false; err=_T("语法错误：操作数不足"); return 0; }
            double b = st.top(); st.pop();
            double a = st.top(); st.pop();
            if (tok=="+") st.push(a+b);
            else if (tok=="-") st.push(a-b);
            else if (tok=="*") st.push(a*b);
            else if (tok=="/") {
                if (fabs(b)<1e-12) { ok=false; err=_T("数学错误：除以零"); return 0; }
                st.push(a/b);
            }
        } else {
            ok=false; err=_T("未知令牌在 RPN 中"); return 0;
        }
    }
    if (st.size()!=1) { ok=false; err=_T("语法错误：表达式无法计算为单一结果"); return 0; }
    return st.top();
}

void CCalculatorDlg::UpdateEdit()
{
    if (m_pEdit) {
        m_pEdit->SetWindowTextW(m_display);
    }
}
