#include "includes/SWidgets/Web/SWebView.hpp"

#include "includes/SWidgets/Web/SWebPage.hpp"
#include "includes/SWidgets/Web/SWebHitTestResult.hpp"

#include <QHostInfo>
#include <QWebEngineHistory>

QList<int> SWebView::zoomLevels()
{
    return QList<int>() << 10 << 20 << 30 << 40 << 50 << 60 << 80 << 90 << 100 <<
                           110 << 120 << 130 << 140 << 150 << 160 << 170 << 180 << 190 << 200;
}

SWebView::SWebView(QWidget *parent) :
    QWebEngineView(parent),
    m_currentZoomLevel(zoomLevel().indexOf(100)),
    m_progress(100),
    m_page(nullptr),
    m_firstLoad(false)
{
    connect(this, &QWebEngineView::loadStarted, this, &SWebView::sLoadStarted);
    connect(this, &QWebEngineView::loadProgress, this, &SWebView::sLoadProgress);
    connect(this, &QWebEngineView::loadFinished, this, &SWebView::sLoadFinished);
    connect(this, &QWebEngineView::urlChanged, this, &SWebView::sUrlChanged);

    setAcceptDrops(true);
    installEventFilter(this);
    if(parentWidget())
        parentWidget()->installEventFilter(this);
}

SWebView::~SWebView()
{
    // Empty
}

bool SWebView::event(QEvent *event)
{
    if (event->type() = QEvent::ChildAdded) {
        QChildEvent *child_ev = static_cast<QChildEvent*>(ev);

        QOpenGLWidget *w = qobject_cast<QOpenGLWidget*>(child_ev->child());
        if (w) {
            m_child = w;
            w->installEventFilter(this);
        }
    }

    if (event->type() == QEvent::ParentChange && parentWidget())
        parentWidget()->installEventFilter(this);

    return QWebEngineView::event(ev);
}

bool SWebView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_child) {
        switch (event->type()) {
        case QEvent::Wheel:
            newWheelEvent(static_cast<QWheelEvent*>(event));
        case QEvent::MouseButtonPress:
            newMousePessEvent(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonRelease:
            newMouseReleaseEvent(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseMove:
            newMouseMoveEvent(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::FocusIn:
            emit focusChanged(true);
            break;
        case QEvent::FocusOut:
            emit focusChanged(false);
            break;
        default:
            break;
        }
    }

    if (watched == parentWidget()) {
        switch (event->type()) {
        case QEvent::KeyPress:
            newKeyPressEvent(static_cast<QKeyEvent*>(event));
            break;
        case QEvent::KeyRelease:
            newKeyReleaseEvent(static_cast<QKeyEvent*>(event));
        default:
            break;
        }
    }

    return QWebEngineView::eventFilter(watched, event);
}

QString SWebView::title() const
{
    QString title{ title() };

    if (title.isEmpty())
        title = url().toString(QUrl::RemoveFragment);

    if (title.isEmpty() || title == QLatin1String("about:blank"))
        return tr("Page vide");

    return title;
}

bool SWebView::isTitleEmpty() const
{
    return QWebEngineView::title().isEmpty();
}

SWebPage* SWebView::page() const
{
    return m_page;
}

void SWebView::setPage(SWebPage *page)
{
    if (m_page == page)
        return;

    m_page = page;
    m_page->setParent(this);
    QWebEngineView::setPage(m_page);

    zoomReset();
    initActions();

    // TODO: Plugins signal, privacy, ...
}

void SWebView::load(const QUrl &url)
{
    QWebEngineView::load(url);

    if (!m_firstLoad) {
        m_firstLoad = true;
    }
}

void SWebView::load(const SLoadRequest &request)
{
    const QUrl requestUrl = request.url();

    if (requestUrl.isEmpty())
        return;

    if (requestUrl.scheme() == QLatin1String("javascript")) {
        const QString scriptSrc = requestUrl.toString().mid(11);
        // We will probaly add code for Herve
        if (scriptSrc.contains(QLatin1Char('%')))
            page()->runJavaScript(QUrl::fromPercentEncoding(scriptSrc.toUtf8()));
        else
            page()->runJavaScript(scriptSrc);

        return;
    }

    if (mApp->isUrlValid(requestUrl)) {
        loadRequest(request);
        return;
    }

    if (!requestUrl.isEmpty() &&
        requestUrl.scheme().isEmpty() &&
        !requestUrl.path().contains(QLatin1Char(' ')) &&
        !requestUrl.path().contains(QLatin1Char('.'))) {

        QUrl url{ QStringLiteral("http://") + requestUrl.path() };
        if (url.isValid()) {
            QHostInfo info{ QHostInfo::fromName(url.path()) };
            if (info.error() == QHostInfo::NoError) {
                SLoadRequest req = request;
                req.setUrl(url);
                loadRequest(req);
                return;
            }
        }
    }

    // TODO: Manage search !
}

bool SWebView::isLoading() const
{
    return m_progress < 100;
}

bool SWebView::loadingProgress() const
{
    return m_progress;
}

bool SWebView::zoomLevel() const
{
    return m_currentZoomLevel;
}

bool SWebView::setZoomLevel(int level)
{
    m_currentZoomLevel = level;
    applyZoom();
}

QPointF SWebView::mapToViewport(const QPointF &pos) const
{
    return page()->mapToViewport(pos);
}

QWidget *SWebView::inputWidget() const
{
    return m_child ? m_child : const_cast<SWebView*>(this);
}

void SWebView::zoomIn()
{
    if (m_currentZoomLevel < zoomLevels().count() - 1) {
        ++m_currentZoomLevel;
        applyZoom();
    }
}

void SWebView::zoomOut()
{
    if (m_currentZoomLevel > 0) {
        --m_currentZoomLevel;
        applyZoom();
    }
}

void SWebView::zoomReset()
{
    int defaultZoomLevel{ mApp->settings()->value("Preferences/ZoomLevel", zoomLevels().indexOf(100)).toInt() };
    if (m_currentZoomLevel != defaultZoomLevel) {
        m_currentZoomLevel = defaultZoomLevel;
        applyZoom();
    }
}

void SWebView::back()
{
    QWebEngineHistory* history{ page()->history() };

    if (history->canGoBack()) {
        history->back();
        emit urlChanged(url());
    }
}

void SWebView::forward()
{
    QWebEngineHistory* history{ page()->history() };

    if (history->canGoForward()) {
        history->forward();
        emit urlChanged(url());
    }
}
