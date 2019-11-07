#ifndef MINGL_BASE_WINDOW_INCLUDED
#define MINGL_BASE_WINDOW_INCLUDED

#include <string>
#include <iomanip>

#include <windows.h>
#include <gdiplus/gdiplus.h>
#include <mingl/message.hpp>

/*
Define USE_WNDCLASSEX macro to allow BaseWindow class to store WNDCLASSEX instead of WNDCLASS.
WNDCLASSEX (extended window class) has extra variable hIconSm and cbSize which needs to be 'size(WNDCLASSEX)'.
To create WNDCLASSEX use method CreateWindowEx instead of CreateWindow,
which needs optional window styles argument passed.
*/

template<class DerivedWindow>
class BaseWindow {
private:
#ifdef USE_WNDCLASSEX
    WNDCLASSEX
#else
    WNDCLASS
#endif
            wc{};
    HWND hwnd{};
    int x{CW_USEDEFAULT}, y{CW_USEDEFAULT}, width{CW_USEDEFAULT}, height{CW_USEDEFAULT};
    std::string title{};
    char *wndClassName{nullptr};

    Gdiplus::GdiplusStartupInput gdiplusStartupInput{};
    ULONG_PTR gdiplusToken{};

    void setClassName() {
        if (wndClassName == nullptr) {
            wndClassName = new char[8];
            srand(time(nullptr));
            int r = std::rand();
            std::stringstream stream;
            stream << std::setfill('0') << std::setw(8) << std::hex << r;
            const std::string &tmpstr = stream.str();
            lstrcpy(wndClassName, tmpstr.c_str());
        }
    }

    static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        DerivedWindow *pThis = nullptr;

        if (msg == WM_NCCREATE) {
            auto pCreate = (CREATESTRUCT *) lParam;
            pThis = (DerivedWindow *) pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) pThis);

            pThis->hwnd = hwnd;
        } else pThis = (DerivedWindow *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

        if (msg == WM_DESTROY) {
            PostQuitMessage(0);
            return 0;
        }

        if (pThis) return pThis->handleMessage(Message(msg, wParam, lParam));
        else return DefWindowProc(hwnd, msg, wParam, lParam);
    }

protected:
    virtual LRESULT handleMessage(Message) = 0;

public:
    BaseWindow() {
        initialize();
    }

    virtual ~BaseWindow() {
        if (wndClassName != nullptr) delete[] wndClassName;
    }

    virtual void initialize() final {
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
        setClassName();
#ifdef USE_WNDCLASSEX
        wc.cbSize = sizeof(WNDCLASSEX);
#endif
        wc.lpszClassName = wndClassName;
        setWindowProcedure(DerivedWindow::wndProc);
        setDefaultSettings();
    }

    virtual void setDefaultSettings() {
        setIcon(nullptr, IDI_APPLICATION);
#ifdef USE_WNDCLASSEX
        setSmallIcon(nullptr, IDI_APPLICATION);
#endif
        setCursor(nullptr, IDI_APPLICATION);
        setBackground((HBRUSH) (COLOR_WINDOW + 1));
        setMenuName(nullptr);
        setStyle(0);
        setWindowExtraBytes(0);
        setClassExtraBytes(0);
    }

    virtual bool create() final {
        if (wndClassName == nullptr) setClassName();
        wc.lpszClassName = wndClassName;
        wc.hInstance = GetModuleHandle(nullptr);
#ifdef USE_WNDCLASSEX
        wc.cbSize = sizeof(WNDCLASSEX);
        RegisterClassEx(&wc);
        hwnd = CreateWindowEx(
                WS_EX_CLIENTEDGE, // optional window styles
#else
        RegisterClass(&wc);
        hwnd = CreateWindow(
#endif
                wndClassName, // window class
title.c_str(), // window title
WS_OVERLAPPEDWINDOW, // window style
x, y, width, height, // coordinates
nullptr, // parent window
nullptr, // menu
wc.hInstance, // instance handle
this // additional application data
        );
        return hwnd != nullptr;
    }

    virtual WPARAM show(int nCmdShow) final {
        MSG msg{};
        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);
        while (GetMessage(&msg, nullptr, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Gdiplus::GdiplusShutdown(gdiplusToken);
        return msg.wParam;
    }

    virtual
#ifdef USE_WNDCLASSEX
    WNDCLASSEX
#else
    WNDCLASS
#endif
    getWindowClass() const final {
        return wc;
    }

    virtual const char *getWindowClassName() const final {
        return wndClassName;
    }

    virtual HWND getWindowHandle() const final {
        return hwnd;
    }

    virtual std::string getTitle() const final {
        return title;
    }

    virtual void setTitle(std::string str) final {
        title = str;
    }

    virtual void setWindowProcedure(LRESULT CALLBACK (*wndProc)(HWND, UINT, WPARAM, LPARAM)) final {
        wc.lpfnWndProc = wndProc;
    }

    virtual void setIcon(HINSTANCE hinstance, LPCSTR icon_name) final {
        wc.hIcon = LoadIcon(hinstance, icon_name);
    }

#ifdef USE_WNDCLASSEX
    virtual void setSmallIcon(HINSTANCE hinstance, LPCSTR icon_name) final {
        wc.hIconSm = LoadIcon(hinstance, icon_name);
    }
#endif

    virtual void setCursor(HINSTANCE hinstance, LPCSTR cursor_name) final {
        wc.hCursor = LoadCursor(hinstance, cursor_name);
    }

    virtual void setMenuName(LPCSTR menu_name) final {
        wc.lpszMenuName = menu_name;
    }

    virtual void setBackground(HBRUSH color) final {
        wc.hbrBackground = color;
    }

    virtual void setStyle(UINT style) final {
        wc.style = style;
    }

    virtual void setWindowExtraBytes(int n_bytes) final {
        wc.cbWndExtra = n_bytes;
    }

    virtual void setClassExtraBytes(int n_bytes) final {
        wc.cbClsExtra = n_bytes;
    }
};

#endif
