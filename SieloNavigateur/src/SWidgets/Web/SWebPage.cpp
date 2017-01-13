#include "includes/SWidgets/Web/SWebPage.hpp"
#include "includes/SWidgets/Web/SWebHitTestResult.hpp"
#include "includes/SWidgets/Web/SWebView.hpp"

#include "includes/SDialogs/SCheckBoxDialog.hpp"

#include "includes/SApplication.hpp"
#include "includes/SPlugins/SPluginProxy.hpp"

#include <QPointer>
#include <QTimer>
#include <QWebEngineSettings>

SWebPage::SWebPage(QObject *parent) :
    QWebEnginePage(parent),
    m_runningLoop(nullptr),
    m_loadProgress(-1),
    m_blockAlerts(false),
    m_secureStatus(false),
    m_adjustingSheduled(false)
{

}

SWebPage::~SWebPage()
{
    //TODO: Say to plugin the web page was delete !

    if (m_runningLoop) {
        m_runningLoop->exit(1);
        m_runningLoop = nullptr;
    }
}

SWebView *SWebPage::view() const
{
    return static_cast<SWebView*>(QWebEnginePage::view());
}

QVariant SWebPage::executeJavaScript(const QString &scriptSrc, quint32 worldId, int timeout)
{
    QPointer<QEventLoop> loop{ new QEventLoop };
    QVariant result{};
    QTimer::singleShot(timeout, loop.data(), &QEventLoop::quit);

    runJavaScript(scriptSrc, worldId, [loop, &result](const QVariant &res) {
        if (loop && loop->isRunning()) {
            result = res;
            loop->quit();
        }
    });

    loop->exec();
    delete loop;

    return result;
}

QPointF SWebPage::mapToViewport(const QPointF &pos) const
{
    return QPointF(pos.x() / zoomFactor(), pos.y() / zoomFactor());
}

SWebHitTestResult SWebPage::hitTestContent(const QPoint &pos) const
{
    return SWebHitTestResult(this, pos);
}

void SWebPage::scroll(int x, int y)
{
    runJavaScript(QStringLiteral("window.scrollTo(window.scrollX + %1, window.scrollY + %2)").arg(x).arg(y), QWebEngineScript::ApplicationWorld);
}

void SWebPage::setScrollPosition(const QPointF &pos)
{
    const QPointF v{ mapToViewport(pos.toPoint()) };
    runJavaScript(QStringLiteral("window.scrollTo(%1, %2)").arg(v.x()).arg(v.y()));
}

void SWebPage::javaScriptAlert(const QUrl &securityOrigin, const QString &msg)
{
    Q_UNUSED(securityOrigin)

    if (m_blockAlerts || m_runningLoop)
        return;

    QString title{ tr("Alerte JavaScripte") };

    if (!url().host().isEmpty())
        title.append(QString(" - %1").arg(url().host()));

    SCheckBoxDialog dialog(QDialogButtonBox::Ok, view());
    dialog.setWindowTitle(title);
    dialog.setText(msg);
    dialog.setCheckBoxText(tr("Empêcher la page de créer des boites de dialog supplémentaires."));
    dialog.exec();

    m_blockAlerts = dialog.isChecked();
}

void SWebPage::setJavaScriptEnable(bool enabled)
{
    settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, enabled);
}

bool SWebPage::isRunningLoop()
{
    return m_runningLoop;
}

bool SWebPage::isLoading() const
{
    return m_loadProgress < 100;
}

void SWebPage::progress(int progression)
{
    m_loadProgress = progression;

    bool secStatus = url().scheme() == QLatin1String("https");

    if (secStatus != m_secureStatus) {
        m_secureStatus = secStatus;
        emit privacyChanged(secStatus);
    }
}

void SWebPage::finished()
{
    progress(100);

    if (m_adjustingSheduled) {
        m_adjustingSheduled = false;
        setZoomFactor(zoomFactor() + 1);
        setZoomFactor(zoomFactor() - 1);
    }

    // TODO: Manage files
}

void SWebPage::urlChanged(const QUrl &url)
{
    Q_UNUSED(url)

    if (isLoading())
        m_blockAlerts = false;
}

void SWebPage::windowCloseRequested()
{
    if (!view())
        return;

    //TODO: view()->closeView();
}

void SWebPage::fullScreenRequested(QWebEngineFullScreenRequest fullScreenRequest)
{
    //TODO: view()->requestFullScreen(fullScreenRequest.toggleOn());

    const bool accepted = fullScreenRequest.toggleOn() == view()->isFullScreen();

    if (accepted)
        fullScreenRequest.accept();
    else
        fullScreenRequest.reject();
}

void SWebPage::featurePermissionRequested(const QUrl &origin, const QWebEnginePage::Feature& feature)
{
    if (feature == MouseLock && view()->isFullScreen())
        setFeaturePermission(origin, feature, PermissionGrantedByUser);
//    else
//      //TODO: Manage permission by app
}

bool SWebPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame)
{
    //TODO: Plugins implementation

    return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
}

QWebEnginePage *SWebPage::createWindow(QWebEnginePage::WebWindowType type)
{
    return Q_NULLPTR;

    //TODO: do

}
