#ifndef SWEBPAGE_HPP
#define SWEBPAGE_HPP

#include <QWebEnginePage>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineScript>
#include <QPoint>
#include <QEventLoop>

class SWebView;
class SWebHitTestResult;

class SWebPage : public QWebEnginePage
{
    Q_OBJECT

public:
    SWebPage(QObject *parent = nullptr);
    ~SWebPage();

    SWebView *view() const;

    QVariant executeJavaScript(const QString &scriptSrc, quint32 worldId = QWebEngineScript::MainWorld, int timeout = 500);

    QPointF mapToViewport(const QPointF &pos) const;
    SWebHitTestResult hitTestContent(const QPoint &pos) const;

    void scroll(int x, int y);
    void setScrollPosition(const QPointF &pos);

    void javaScriptAlert(const QUrl &securityOrigin, const QString &msg) Q_DECL_OVERRIDE;

    void setJavaScriptEnable(bool enabled);

    bool isRunningLoop();

    bool isLoading() const;

signals:
    void privacyChanged(bool status);

protected slots:
    void progress(int progression);
    void finished();

private slots:
    void urlChanged(const QUrl &url);
    void windowCloseRequested();
    void fullScreenRequested(QWebEngineFullScreenRequest fullScreenRequest);
    void featurePermissionRequested(const QUrl &origine, const QWebEnginePage::Feature& feature);

private:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) Q_DECL_OVERRIDE;
    QWebEnginePage *createWindow(WebWindowType type) Q_DECL_OVERRIDE;

    void handleUnknowProtocol(const QUrl &url);
    void desktopServiceOpen(const QUrl &url);

    QEventLoop *m_runningLoop{ nullptr };

    int m_loadProgress{ -1 };
    bool m_blockAlerts{ false };
    bool m_secureStatus{ false };
    bool m_adjustingSheduled{ false };

};

#endif // SWEBPAGE_HPP
