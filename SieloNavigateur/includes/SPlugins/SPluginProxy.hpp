#pragma once

#include <QList>

#include "includes/SPlugins/SPlugins.hpp"

class SPluginProxy : public SPlugins  {
    Q_OBJECT
public:

    enum EventHandlerType {
        MouseDoubleClickHandler,
        MousePressHandler,
        MouseReleaseHandler,
        MouseMoveHandler,
        KeyPressHandler,
        KeyReleaseHandler,
        WheelEventHandler
    };

    explicit SPluginProxy();

    void registerAppEventHandler(const EventHandlerType &type, SPluginInterface *obj);

    bool processMouseDoubleClick(const Sn::ObjectName &type, QObject *obj, QMouseEvent *event);
    bool processMousePress(const Sn::ObjectName &type, QObject *obj, QMouseEvent *event);
    bool processMouseRelease(const Sn::ObjectName &type, QObject *obj, QMouseEvent *event);
    bool processMouseMove(const Sn::ObjectName &type, QObject *obj, QMouseEvent *event);

    bool processWheelEvent(const Sn::ObjectName &type, QObject *obj, QWheelEvent *event);

    bool processKeyPress(const Sn::ObjectName &type, QObject *obj, QKeyEvent *event);
    bool processKeyRelease(const Sn::ObjectName &type, QObject *obj, QKeyEvent *event);

private slots:
    void pluginUnloaded(SPluginInterface *plugin);

private:
    QList<SPluginInterface*> m_mouseDoubleClickHandlers;
    QList<SPluginInterface*> m_mousePressHandlers;
    QList<SPluginInterface*> m_mouseReleaseHandlers;
    QList<SPluginInterface*> m_mouseMoveHandlers;

    QList<SPluginInterface*> m_wheelEventHandlers;

    QList<SPluginInterface*> m_keyPressHandlers;
    QList<SPluginInterface*> m_keyReleaseHandlers;
};

#include "includes/SApplication.hpp"
#define SN_REGISTER_EVENT_HANDLER(Type) mApp->plugins()->registerAppEventHandler(Type, this);
