#pragma once

#include <QObject>
#include <QtPlugin>

#include "includes/SPlugins/SPluginInterface.hpp"

class TestPlugin : public QObject, public SPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(SPluginInterface)
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "testplugin.json")

public:
    explicit TestPlugin();
    SPluginProp pluginProp();

    void init(InitState state, const QString &settingsPath);
    void unload();
    bool testPlugin();

    void showSettings(QWidget *parent);

    bool mousePress(const Sn::ObjectName &objName, QObject *obj, QMouseEvent *event);

};
