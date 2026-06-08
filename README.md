# Calculator

This repository contains a Visual Studio 2022 MFC dialog-based Calculator project (branch `calculator-mfc`).

Project: Calculator (MFC Dialog-based)

Features:
- Expression input with operator precedence (supports + - * / and parentheses)
- Decimal numbers
- Trigonometric functions: sin, cos, tan (input interpreted as degrees)
- Clear and backspace
- Basic error handling (divide by zero, syntax errors)

How to use:
1. Clone the repository and checkout the branch:
   git clone --branch calculator-mfc https://github.com/fk-cmn/Calculator.git
2. Open Calculator.sln in Visual Studio 2022.
3. Build and run (Note: If Visual Studio reports missing project settings, create a new MFC Dialog-based project named `Calculator` and replace the generated source files with the ones in this repo.)

Notes:
- If you prefer, create a new Dialog-based MFC project in VS2022 and copy the source files from /src into the project. This repository includes all source files and resources.

Files added on branch `calculator-mfc`:
- /src : Visual C++ source files (.cpp/.h)
- /res : resource script and header
- /docs : experiment report and screenshot placeholders
- LICENSE, README
