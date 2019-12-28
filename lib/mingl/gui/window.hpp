#ifndef MINGL_WINDOW_INCLUDED
#define MINGL_WINDOW_INCLUDED

#include <map>
#include <list>

#include <mingl/gui/basewindow.hpp>
#include <mingl/gui/button.hpp>
#include <mingl/gui/edittext.hpp>
#include <mingl/gui/label.hpp>
#include <mingl/messagehandler.hpp>

/**
 * Derived main Window to create and show window.
 */
class Window : public BaseWindow<Window> {
    std::map<UINT, MessageHandler> messageHandlers{};
    std::map<long, GUIObject *> objects{};

    /**
     * Allows addition of GUIObjects passed as rvalue. Template class Object should be subclass of GUIObject.
     *
     * @tparam Object
     * @returns a pointer allocated for the passed object stored in the window
     */
    template<class Object>
    Object *addObject(Object &&);

public:
    Window();

    Window(const char *title);

    Window(int x, int y, int width, int height);

    Window(const char *title, int x, int y, int width, int height);

    virtual ~Window();

    LRESULT handleMessage(Message) override;

    /** Adds a message handler to respond to window messages. */
    void addHandler(MessageHandler);

    /** Adds a message handler to respond to specified message with handle function. */
    void addHandler(Message, void (*handler)(Window *, Message));

    void addObject(GUIObject *);

    /* Label additions. */

    void addLabel(Label *);

    Label *addLabel(Label &&);

    Label *addLabel(long id, const char *text, int x, int y, int width, int height);

    /* EditText additions. */

    void addEditText(EditText *);

    EditText *addEditText(EditText &&);

    EditText *addEditText(long id, int x, int y, int width, int height);

    EditText *addEditText(long id, const char *text, int x, int y, int width, int height);

    /* Button additions. */

    void addButton(Button *);

    Button *addButton(Button &&);

    /**
     * Adds a button with specified title at certain position relative to the window.
     * Assign id to the button to respond with later assigned onClick method.
     *
     * @returns pointer to the newly created button.
     */
    Button *addButton(long id, const char *title, int x, int y, int width, int height);

    /**
     * Adds a button with specified title at certain position relative to the window.
     * Assign id to the button to respond with assigned onClick method.
     *
     * @returns pointer to the newly created button.
     */
    Button *addButton(long id, const char *title, int x, int y, int width, int height, std::function<void()> onClick);

    /**
     * Removes object from window. @returns true if found and removed.
     */
    template<class Object, typename = std::enable_if<std::is_base_of<GUIObject, Object>::value>>
    bool remove(Object *object) {
        // todo: map.erase(id)?
        for (std::map<long, GUIObject *>::const_iterator it = objects.begin(); it != objects.end(); it++)
            if (it->second == object) {
                objects.erase(it);
                object->setParent(nullptr);
                return true;
            }
        return false;
    }

    GUIObject *find(long id);

    /** Creates objects when window is created. */
    void createObjects();

    /** Performs click on a button found by id. If not found, nothing happens. */
    void performClick(long id);
};

#endif
