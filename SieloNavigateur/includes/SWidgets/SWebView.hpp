#pragma once 

#include <QWidget>
#include <QtWebEngineWidgets>
#include <QContextMenuEvent>

#include <QtEvents>

class STabWidget;

class SWebView : public QWebEngineView
{
public:
	// Constructor and destructor
	SWebView(QWidget* parent = nullptr, STabWidget *parentTab = nullptr, QUrl url = QUrl("http://google.fr"));
	~SWebView();

	// For set parent and override m_parent
	void changeParent(QWidget *parent, STabWidget *parentTab);
protected:
	SWebView * createWindow(QWebEnginePage::WebWindowType type); //< Called when a new tab/window is requested

    virtual void wheelEvent(QWheelEvent *event) {}
    virtual void mousePressEvent(QMouseEvent *event) {}
    virtual void mouseReleaseEvent(QMouseEvent *event) {}
    virtual void mouseMoveEvent(QMouseEvent *event) {}
    virtual void keyPressEvent(QKeyEvent *event) {}
    virtual void keyReleaseEvent(QKeyEvent *event) {}
    virtual void contextMenuEvent(QContextMenuEvent *event);

public slots:
    void setFullScreen(QWebEngineFullScreenRequest request); //< Called whene full screen is requested (ex: YouTube videos)

private:
	QWidget* m_parent{ nullptr };
    STabWidget *m_parentTab{ nullptr };
    bool m_fullScreen{ false };
};
