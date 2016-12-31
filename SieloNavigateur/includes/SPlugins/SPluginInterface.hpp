#pragma once

#include "includes/SApplication.hpp"

#include <QtPlugin>
#include <QPixmap>
#include <QObject>

struct SPluginProp {
    QString name{};
    QString info{};
    QString desc{};
    QString author{};
    QString version{};
    QPixmap icon{};
    bool hasSettings{ false };

    SPluginProp() {}

    bool operator ==(const SPluginProp& other) const {
        return (name == other.name &&
                info == other.info &&
                desc == other.desc &&
                author == other.author &&
                version == other.version);
    }
};

class SPluginInterface {
public:
    enum InitState{ StartupInitState, LateInitState };

    virtual SPluginProp pluginProp() = 0;
    virtual void init(InitState state, const QString& settingsPath) = 0;
    virtual void unload() = 0;
    virtual bool testPlugin() = 0;

    virtual ~SPluginInterface() {}
    virtual void showSettings(QWidget *parent = nullptr) { Q_UNUSED(parent) }

    virtual bool mouseDoubleClick(const Sn::ObjectName &objName, QObject *obj, QMouseEvent *event) {
        Q_UNUSED(objName)
        Q_UNUSED(obj)
        Q_UNUSED(event)
        return false;
    }
    virtual bool mousePress(const Sn::ObjectName &objName, QObject *obj, QMouseEvent *event) {
        Q_UNUSED(objName)
        Q_UNUSED(obj)
        Q_UNUSED(event)
        return false;
    }
    virtual bool mouseRelease(const Sn::ObjectName &objName, QObject *obj, QMouseEvent *event) {
        Q_UNUSED(objName)
        Q_UNUSED(obj)
        Q_UNUSED(event)
        return false;
    }
    virtual bool mouseMouve(const Sn::ObjectName &objName, QObject *obj, QMouseEvent *event) {
        Q_UNUSED(objName)
        Q_UNUSED(obj)
        Q_UNUSED(event)
        return false;
    }

    virtual bool wheelEvent(const Sn::ObjectName &objName, QObject *obj, QWheelEvent *event) {
        Q_UNUSED(objName)
        Q_UNUSED(obj)
        Q_UNUSED(event)
        return false;
    }

    virtual bool keyPress(const Sn::ObjectName &objName, QObject *obj, QKeyEvent *event) {
        Q_UNUSED(objName)
        Q_UNUSED(obj)
        Q_UNUSED(event)
        return false;
    }
    virtual bool keyRelease(const Sn::ObjectName &objName, QObject *obj, QKeyEvent *event) {
        Q_UNUSED(objName)
        Q_UNUSED(obj)
        Q_UNUSED(event)
        return false;
    }

};

QT_BEGIN_NAMESPACE

#define PluginInterface_iid "com.feldrise.SieloNavigateur.PluginInterface"
Q_DECLARE_INTERFACE(SPluginInterface, PluginInterface_iid)

QT_END_NAMESPACE
