#include "guiobject.hpp"
#include "window.hpp"

GUIObject::GUIObject(const char *className,
                     const char *name,
                     UINT16 x,
                     UINT16 y,
                     UINT16 width,
                     UINT16 height,
                     HINSTANCE hInstance,
                     LPVOID additionalData)
        : className(className),
          name(name),
          x(x),
          y(y),
          width(width),
          height(height),
          hInstance(hInstance),
          additionalData(additionalData) {}

GUIObject::GUIObject(Window *parent,
                     const char *className,
                     long id,
                     const char *name,
                     UINT16 x,
                     UINT16 y,
                     UINT16 width,
                     UINT16 height)
        : className(className),
          name(name),
          x(x),
          y(y),
          width(width),
          height(height),
          id(id),
          parent(parent) {
    addToParent();
}

GUIObject::~GUIObject() { removeFromParent(); }

bool GUIObject::create() {
    if (isCreated()) return true;
    hWnd = CreateWindowEx(
            extendedStyle,
            className,
            name,
            style,
            x,
            y,
            width,
            height,
            parent != nullptr ? parent->getWindowHandle() : nullptr,
            hMenu,
            hInstance,
            additionalData
    );
    return isCreated();
}

void GUIObject::addStyle(UINT s) { style |= s; }
void GUIObject::removeStyle(UINT s) { style ^= s; }

const char *GUIObject::getClassName() const { return className; }
const char *GUIObject::getName() const { return name; }
HINSTANCE GUIObject::getInstance() const { return hInstance; }

UINT16 GUIObject::getX() const { return x; }
UINT16 GUIObject::getY() const { return y; }
UINT16 GUIObject::getWidth() const { return width; }
UINT16 GUIObject::getHeight() const { return height; }
void GUIObject::setWidth(UINT16 w) { width = w; }
void GUIObject::setHeight(UINT16 h) { height = h; }

long GUIObject::getId() const { return id; }
HWND GUIObject::getWindowHandle() const { return hWnd; }
void GUIObject::setWindowHandle(HWND handle) { hWnd = handle; }

bool GUIObject::isCreated() const { return hWnd != nullptr; }

void GUIObject::setParent(Window *window, bool onlySet) {
    if (!onlySet) removeFromParent();
    parent = window;
    if (!onlySet) addToParent();
}

void GUIObject::addToParent() {
    if (parent != nullptr) {
        addStyle(WS_CHILD);
        parent->addObject(this, true);
    }
}

void GUIObject::removeFromParent() { if (parent != nullptr) parent->remove(this); }

void GUIObject::setName(const char *n) { name = n; }

std::string GUIObject::getText(UINT16 length) {
    if (!isCreated()) return name;

    if (++length == 1) {
        char *text = nullptr;
        while (true) {
            text = new char[++length];
            GetWindowText(hWnd, text, length);

            if (text[length - 2] == 0) {
                std::string finalText = text;
                delete[] text;
                return finalText;
            }
            delete[] text;
        }
    }

    char *text = new char[length];
    GetWindowText(hWnd, text, length);
    std::string finalText = text;
    delete[] text;
    return finalText;
}

void GUIObject::setText(const char *text) {
    if (isCreated()) SetWindowText(hWnd, text);
    name = text;
}

/* Comparision operators */
bool GUIObject::operator==(const GUIObject &o) const { return id == o.id; }
bool GUIObject::operator!=(const GUIObject &o) const { return !(*this == o); }
bool GUIObject::operator<(const GUIObject &o) const { return id < o.id; }
bool GUIObject::operator>(const GUIObject &o) const { return o < *this; }
bool GUIObject::operator<=(const GUIObject &o) const { return !(*this > o); }
bool GUIObject::operator>=(const GUIObject &o) const { return !(*this < o); }
