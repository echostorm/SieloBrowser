#ifndef SWEBVIEW_HPP
#define SWEBVIEW_HPP

#include <QApplication>
#include <QOpenGLWidget>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWidget>
#include <QEvent>
#include <QKeyEvent>

#include "includes/SWidgets/Web/SLoadRequest.hpp"
#include "includes/SApplication.hpp"

class SWebPage;
class SWebHitTestResult;

class SWebView : public QWebEngineView
{
    Q_OBJECT

public:
    SWebView(QWidget *parent = nullptr);
    ~SWebView();

    bool event(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

//  QIcon icon(bool allowNull = false) const;

    QString title() const;
    bool isTitleEmpty() const;

    SWebPage *page() const;
    void setPage(SWebPage* page);

    void load(const QUrl &url);
    void load(const SLoadRequest &request);
    bool isLoading();

    int loadingProgress() const;

    int zoomLevel() const;
    void setZoomLevel(int level);

    QPointF mapToViewport(const QPointF &pos) const;

    //TODO: History, Notification

    QWidget *inputWidget() const;
    virtual QWidget *overlayWidget() = 0;

signals:
    void focusChanged(bool);
    void viewportResized(QSize);
    void privacyChanged(bool);
    void zoomLevelChanged(int);
    //TODO: Notification signal

public slots:
    void zoomIn();
    void zoomOut();
    void zoomReset();

    void editUndo() {  triggerPageAction(QWebEnginePage::Undo); }
    void editRedo() {  triggerPageAction(QWebEnginePage::Redo); }
    void editCopy() {  triggerPageAction(QWebEnginePage::Copy); }
    void editCut() {  triggerPageAction(QWebEnginePage::Cut); }
    void editPast() {  triggerPageAction(QWebEnginePage::Paste); }
    void editSelectAll() {  triggerPageAction(QWebEnginePage::SelectAll); }
    void editDelete()
    {
        QKeyEvent event(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
        QApplication::sendEvent(this, &event);
    }

    void reloadBypassCache();

    void back();
    void forward();

    //TODO: Show sources
    void openUrlInNewTab(const QUrl &url, Sn::NewTabType position) = 0;

    virtual void closeView() = 0;
    virtual void loadInNewTab(const SLoadRequest &request, Sn::NewTabType position) = 0;

    virtual bool isFullScreen() = 0;
    virtual void requestFullScreen(boon enable) = 0;

protected slots:
    void sLoadStarted();
    void sLoadProgress(int progress);
    void sLoadFinished(bool ok);
    void sUrlChanged(const QUrl& url);

    void openUrlInNewWindow();
    void copyLinkToClipboard();
    void savePageAs();
    void copyImageToClipboard();
    void dlLinkToDisk();
    void dlImageToDisk();
    void dlMediaToDisk();
    void openActionUrl();
    void showSiteInformation();
    void searchSelectedText();
    void searchSelectedTextInBgTab();
    void bookmarkLink();
    void openUrlInSelectedTab();
    void openUrlInBgTab();

protected:
    void resizeEvent(QResizeEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

    virtual void newWheelEvent(QWheelEvent *event);
    virtual void newMousePessEvent(QMouseEvent *event);
    virtual void newMouseReleaseEvent(QMouseEvent *event);
    virtual void newMouseMoveEvent(QMouseEvent *event);
    virtual void newKeyPressEvent(QKeyEvent *event);
    virtual void newKeyReleaseEvent(QKeyEvent *event);
    virtual void newContextMenuEvent(QContextMenuEvent *event);

    void loadRequest(const SLoadRequest &request);
    void applyZoom();

    void createContextMenu(QMenu *menu, SWebHitTestResult &hitTest);
    void createPageContextMenu(QMenu *menu);
    void createLinkContextMenu(QMenu *menu, const SWebHitTestResult &hitTest);
    void createImageContextMenu(QMenu *menu, const SWebHitTestResult &hitTest);
    void createSelectedTextContextMenu(QMenu *menu, const SWebHitTestResult &hitTest);
    void createMediaContextMenu(QMenu *menu, const SWebHitTestResult &hitTest);

private slots:
    //TODO: Manage forms (for password for exemple)

    void toggleMediaPause();
    void toggleMediaMute();

private:
    void initActions();

    int m_currentZoomLevel{};
    int m_progress{ 100 };
    bool m_firstLoad{ false };

    QUrl m_clickedUrl{};
    QPointF m_clickedPos{};

    SWebPage *m_page{ nullptr };
    QPointer<QOpenGLWidget> m_child{};
};

#endif // SWEBVIEW_HPP
