#include "includes/SWidgets/Web/SWebView.hpp"

#include "includes/SWidgets/Web/SWebPage.hpp"
#include "includes/SWidgets/Web/SWebHitTestResult.hpp"


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
