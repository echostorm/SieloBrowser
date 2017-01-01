#include "TestPlugin.hpp"

#include "includes/SPlugins/SPluginProxy.hpp"
#include "includes/SApplication.hpp"

#include <QMessageBox>

TestPlugin::TestPlugin() :
    QObject()
{
    QMessageBox::information(nullptr, "DEBUG PLUGIN", "The ctor of the test plugin was called");
}

SPluginProp TestPlugin::pluginProp()
{
    SPluginProp prop{};
    prop.name = "TestPlugin";
    prop.info = "Juste a test plugin";
    prop.desc = "The first plugin of Sielo";
    prop.version = "0.0.1";
    prop.author = "Feldrise <admin@feldrise.com>";
    prop.icon = QPixmap();
    prop.hasSettings = false;

    return prop;
}

void TestPlugin::init(InitState state, const QString &settingsPath)
{
    Q_UNUSED(state);

    if (!static_cast<SApplication*>(QCoreApplication::instance())->plugins())
        QMessageBox::information(nullptr, "DEBUG", "That why it crash...");
    else
        QMessageBox::information(nullptr, "DEBUG", "The plugins existe");

    static_cast<SApplication*>(QCoreApplication::instance())->plugins()->registerAppEventHandler(SPluginProxy::MousePressHandler, this);
}

void TestPlugin::unload() {}


bool TestPlugin::testPlugin()
{
    return true;
}

void TestPlugin::showSettings(QWidget *parent) {}

bool TestPlugin::mousePress(const Sn::ObjectName &objName, QObject *obj, QMouseEvent *event)
{
    if (objName == Sn::ON_WebView) {
        QMessageBox::information(nullptr, "DEBUG PLUGIN", "The plugin get a mouse press event on web view");
        return true;
    }

    return false;
}
