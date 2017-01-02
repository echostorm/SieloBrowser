#include "includes/SApplication.hpp"
#include "includes/SPlugins/SPlugins.hpp"
#include "includes/SMainWindow.hpp"

#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>

SPlugins::SPlugins(QObject *parent) :
    QObject(parent)
{
    loadSettings();
}

QList<SPlugins::SPlugin> SPlugins::getAvailablePlugins() {
    loadAvailablePlugins();

    return m_availablePlugins;
}

bool SPlugins::loadPlugin(SPlugin *plugin) {
    if (plugin->isLoaded())
        return true;

    plugin->pluginLoader->setFileName(plugin->fullPath);
    SPluginInterface *iPlugin{ qobject_cast<SPluginInterface*>(plugin->pluginLoader->instance()) };
    if(!iPlugin)
        return false;

    m_availablePlugins.removeOne(*plugin);
    plugin->instance = initPlugin(SPluginInterface::LateInitState, iPlugin, plugin->pluginLoader);
    m_availablePlugins.prepend(*plugin);

    refreshLoadedPlugins();

    return plugin->isLoaded();
}

void SPlugins::unloadPlugin(SPlugin *plugin) {
    if (!plugin->isLoaded())
        return;

    plugin->instance->unload();
    plugin->pluginLoader->unload();
    emit pluginUnloaded(plugin->instance);

    m_availablePlugins.removeOne(*plugin);
    plugin->instance = nullptr;
    m_availablePlugins.append(*plugin);

    refreshLoadedPlugins();
}

void SPlugins::loadSettings() {
    SMainWindow::SSettings->beginGroup("Plugin-Settings");
    m_allowedPlugins = SMainWindow::SSettings->value("AllowedPlugins", QStringList()).toStringList();
    SMainWindow::SSettings->endGroup();
}

void SPlugins::shutdown()
{
    foreach (SPluginInterface *iPlugin, m_loadedPlugins)
        iPlugin->unload();
}

void SPlugins::loadPlugins()
{
    if (!m_pluginsEnabled)
        return;

    QDir settingsDir(SMainWindow::dataPath + "/plugins/");

    if (!settingsDir.exists())
        settingsDir.mkdir(settingsDir.absolutePath());

    foreach (const QString &fullPath, m_allowedPlugins) {
        QPluginLoader *loader{ new QPluginLoader(fullPath) };
        SPluginInterface *iPlugin{ qobject_cast<SPluginInterface*>(loader->instance()) };

        QMessageBox::information(nullptr, "DEBUG", "Load plugin from " + fullPath);
        if (!iPlugin) {
            qWarning() << "Loading " << fullPath << " plugin failed: " << loader->errorString();
            continue;
        }

        SPlugin plugin{};
        plugin.fileName = QFileInfo(fullPath).fileName();
        plugin.fullPath = fullPath;
        plugin.pluginLoader = loader;
        plugin.instance = initPlugin(SPluginInterface::StartupInitState, iPlugin, loader);

        if (plugin.isLoaded()) {
            plugin.pluginProp = iPlugin->pluginProp();

            m_loadedPlugins.append(plugin.instance);
            m_availablePlugins.append(plugin);
        }
    }

    refreshLoadedPlugins();

    qDebug() << "Plugins are loaded";
}

void SPlugins::loadAvailablePlugins() {
    if (m_pluginsLoaded)
        return;

    m_pluginsLoaded = true;

    QDir pluginsDir{ QDir(SMainWindow::dataPath + "/plugins/") };
    foreach (const QString& fileName, pluginsDir.entryList(QDir::Files)) {
        const QString absolutePath{ pluginsDir.absoluteFilePath(fileName) };

        QPluginLoader *loader{ new QPluginLoader(absolutePath) };
        SPluginInterface *iPlugin{ qobject_cast<SPluginInterface*>(loader->instance()) };

        if (!iPlugin) {
            qWarning() << "Available plugin loading error: " << loader->errorString();
            continue;
        }

        SPlugin plugin;
        plugin.fileName = fileName;
        plugin.fullPath = absolutePath;
        plugin.pluginProp = iPlugin->pluginProp();
        plugin.pluginLoader = loader;
        plugin.instance = nullptr;

        loader->unload();

        if (!alreadyPropInAvailable(plugin.pluginProp))
            m_availablePlugins.append(plugin);
    }
}

SPluginInterface *SPlugins::initPlugin(SPluginInterface::InitState state, SPluginInterface *pluginInterface, QPluginLoader *loader)
{
    if (!pluginInterface)
        return nullptr;

    pluginInterface->init(state, SMainWindow::dataPath + "/plugins/");

    if (!pluginInterface->testPlugin()) {
        pluginInterface->unload();
        loader->unload();

        emit pluginUnloaded(pluginInterface);

        return nullptr;
    }

    return pluginInterface;
}

void SPlugins::refreshLoadedPlugins()
{
    m_loadedPlugins.clear();

    foreach (const SPlugin &plugin, m_availablePlugins) {
        if (plugin.isLoaded())
            m_loadedPlugins.append(plugin.instance);
    }
}

bool SPlugins::alreadyPropInAvailable(const SPluginProp &prop)
{
    foreach (const SPlugin &plugin, m_availablePlugins) {
        if (plugin.pluginProp == prop)
            return true;
    }

    return false;
}
