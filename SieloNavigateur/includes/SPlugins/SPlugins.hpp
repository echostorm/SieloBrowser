#pragma once

#include "includes/SPlugins/SPluginInterface.hpp"

#include <QObject>
#include <QList>

class QPluginLoader;

class SPlugins : public QObject {
    Q_OBJECT
public:
    struct SPlugin {
        QString fileName{};
        QString fullPath{};
        SPluginProp pluginProp{};
        QPluginLoader *pluginLoader{ nullptr };
        SPluginInterface *instance{ nullptr };

        SPlugin() {}

        bool isLoaded() const { return instance; }
        bool operator ==(const SPlugin &other) const {
            return (fileName == other.fileName &&
                    fullPath == other.fullPath &&
                    pluginProp == other.pluginProp &&
                    instance == other.instance);
        }
    };

    explicit SPlugins(QObject *parent = nullptr);

    QList<SPlugin> getAvailablePlugins();

    bool loadPlugin(SPlugin *plugin);
    void unloadPlugin(SPlugin *plugin);

    void shutdown();

public slots:
    void loadSettings();

    void loadPlugins();

protected:
    QList<SPluginInterface*> m_loadedPlugins{};

signals:
    void pluginUnloaded(SPluginInterface *plugin);

private:
    bool alreadyPropInAvailable(const SPluginProp &prop);
    SPluginInterface *initPlugin(SPluginInterface::InitState state, SPluginInterface *pluginInterface, QPluginLoader *loader);

    void refreshLoadedPlugins();
    void loadAvailablePlugins();

    QList<SPlugin> m_availablePlugins{};
    QStringList m_allowedPlugins{};

    bool m_pluginsEnabled{ true };
    bool m_pluginsLoaded{ false };
};
