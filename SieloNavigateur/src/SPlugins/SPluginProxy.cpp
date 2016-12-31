#include "includes/SPlugins/SPluginProxy.hpp"
#include "includes/SActions.hpp"

SPluginProxy::SPluginProxy() :
    SPlugins()
{
    connect(this, &SPluginProxy::pluginUnloaded, this, &SPluginProxy::pluginUnloaded);
}

void SPluginProxy::registerAppEventHandler(const SPluginProxy::EventHandlerType &type, SPluginInterface *obj)
{
    switch (type) {
    case MouseDoubleClickHandler:
        if (!m_mouseDoubleClickHandlers.contains(obj))
            m_mouseDoubleClickHandlers.append(obj);
        break;

    case MousePressHandler:
        if (!m_mousePressHandlers.contains(obj))
            m_mousePressHandlers.append(obj);
        break;

    case MouseReleaseHandler:
        if (!m_mouseReleaseHandlers.contains(obj))
            m_mouseReleaseHandlers.append(obj);
        break;

    case MouseMoveHandler:
        if (!m_mouseMoveHandlers.contains(obj))
            m_mouseMoveHandlers.append(obj);
        break;

    case KeyPressHandler:
        if (!m_keyPressHandlers.contains(obj))
            m_keyPressHandlers.append(obj);
        break;

    case KeyReleaseHandler:
        if (!m_keyReleaseHandlers.contains(obj))
            m_keyReleaseHandlers.append(obj);
        break;

    case WheelEventHandler:
        if (!m_wheelEventHandlers.contains(obj))
            m_wheelEventHandlers.append(obj);
        break;

    default:
        qWarning("Registering unknown event handler type");
        break;
    }
}

void SPluginProxy::pluginUnloaded(SPluginInterface *plugin) {
    m_mouseDoubleClickHandlers.removeOne(plugin);
    m_mousePressHandlers.removeOne(plugin);
    m_mouseReleaseHandlers.removeOne(plugin);
    m_mouseMoveHandlers.removeOne(plugin);

    m_wheelEventHandlers.removeOne(plugin);

    m_keyPressHandlers.removeOne(plugin);
    m_keyReleaseHandlers.removeOne(plugin);
}

bool SPluginProxy::processMouseDoubleClick(const Sn::ObjectName &type, QObject *obj, QMouseEvent *event)
{
    bool accepted{ false };

    foreach (SPluginInterface *iPlugin, m_mouseDoubleClickHandlers) {
        if (iPlugin->mouseDoubleClick(type, obj, event))
            accepted = true;
    }

    return accepted;
}
bool SPluginProxy::processMousePress(const Sn::ObjectName &type, QObject *obj, QMouseEvent *event)
{
    bool accepted{ false };

    foreach (SPluginInterface *iPlugin, m_mousePressHandlers) {
        if (iPlugin->mousePress(type, obj, event))
            accepted = true;
    }

    return accepted;
}
bool SPluginProxy::processMouseRelease(const Sn::ObjectName &type, QObject *obj, QMouseEvent *event)
{
    bool accepted{ false };

    foreach (SPluginInterface *iPlugin, m_mouseReleaseHandlers) {
        if (iPlugin->mouseRelease(type, obj, event))
            accepted = true;
    }

    return accepted;
}
bool SPluginProxy::processMouseMove(const Sn::ObjectName &type, QObject *obj, QMouseEvent *event)
{
    bool accepted{ false };

    foreach (SPluginInterface *iPlugin, m_mouseMoveHandlers) {
        if (iPlugin->mouseDoubleClick(type, obj, event))
            accepted = true;
    }

    return accepted;
}

bool SPluginProxy::processKeyPress(const Sn::ObjectName &type, QObject *obj, QKeyEvent *event)
{
    bool accepted{ false };

    foreach (SPluginInterface *iPlugin, m_keyPressHandlers) {
        if (iPlugin->keyPress(type, obj, event))
            accepted = true;
    }

    return accepted;
}
bool SPluginProxy::processKeyRelease(const Sn::ObjectName &type, QObject *obj, QKeyEvent *event)
{
    bool accepted{ false };

    foreach (SPluginInterface *iPlugin, m_keyReleaseHandlers) {
        if (iPlugin->keyRelease(type, obj, event))
            accepted = true;
    }

    return accepted;
}

bool SPluginProxy::processWheelEvent(const Sn::ObjectName &type, QObject *obj, QWheelEvent *event)
{
    bool accepted{ false };

    foreach (SPluginInterface *iPlugin, m_wheelEventHandlers) {
        if (iPlugin->wheelEvent(type, obj, event))
            accepted = true;
    }

    return accepted;
}
