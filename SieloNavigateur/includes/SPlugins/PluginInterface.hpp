#pragma once

#include "includes/SStarter.hpp"

#include <QtPlugin>
#include <QPixmap>

struct PluginProp {
    QString name{};
    QString info{};
    QString desc{};
    QString author{};
    QString version{};
    QPixmap icon{};
    bool hasSettings{ false };

    PluginProp() {}

    bool operator ==(const PluginProp& other) const {
        return (name == other.name &&
                info == other.info &&
                desc == other.desc &&
                author == other.author &&
                version == other.version);
    }
};

class PluginInterface {
public:
    enum InitState{ StartupInitState, LateInitState };

    virtual PluginProp pluginProp() = 0;
    virtual void init(InitState state, const QString& settingsPath) = 0;
    virtual void unload() = 0;
    virtual void testPlugin() = 0;

    virtual ~PluginInterface() {}
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

    //TODO: With other event

};

Q_DECLARE_INTERFACE(PluginInterface, "Sielo.Navigateur.PluginInterface")
