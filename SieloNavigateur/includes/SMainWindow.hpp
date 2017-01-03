#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QVector>
#include <QSettings>
#include <QLineEdit>
#include <QSharedPointer>
#include <QWebEngineDownloadItem>
#include <QSplitter>


struct SActions;
class STabWidget;
class SWebView;
class SToolBar;
class SMenu;
class SUrlArea;
class SSearchArea;
class SDownloadItem;
class SHistoryWindow;
class SHistoryItem;

class SMainWindow : public QMainWindow
{
public:
	// Constructor and destructor
    SMainWindow(SWebView *view = nullptr, bool isPrivateBrowsing = false);
	~SMainWindow();

	// Loading functions
	void loadMenus();
	bool loadToolBar(const QString& filePath);

	// Function to change tab title and update url area
	void changeTabTitle(const QString& newTitle);
	void changeTabUrl(const QUrl& newUrl);

    // Save states
    void saveTabs();
    void saveWinState();

    void setTabs(STabWidget *tabs) { m_tabs = tabs; }
	void createTab(QWidget *widget, const QString& title);

	// Getters
	QSharedPointer<SActions> getActions() const { return m_actions; }
	SUrlArea* getUrlArea() const { return m_urlArea; }
	SSearchArea* getSearchArea() const { return m_searchArea; }
    QSplitter* getSplitter() { return m_splitter; }
	STabWidget* getTabs() { return m_tabs; }
	SWebView* currentPage();
	QVector<SMenu*>& getMenus() { return m_menus; }

	static QVector<SDownloadItem*> dlItems;

	bool privateBrowsing{ false };

	QColor getWindowFrameColor();
public slots:
	// Change title of tab and url in url area from a web view
	void changeTitle(const QString& newTitle);
	void changeUrl(const QUrl& newUrl);

	// Classical action
	void fullScreen();
	void addDownload(QWebEngineDownloadItem *download);
	void removeDownload();

	// YouTube actions
	void separateVideo();
	void attachVideo();

	// Action of navigation
	void back();
	void next();
	void home();
	void refresh();
	void stop();

protected:
	virtual void closeEvent(QCloseEvent *event); //< Called when Sielo is stoped

private:
	// Private function
	void restoreTabs(); //< Restore tab from the previous session

	// Private attributs
	
	// Widgets of the window
    SUrlArea* m_urlArea{ nullptr };
	SSearchArea* m_searchArea{ nullptr };
    QSplitter* m_splitter{ new QSplitter(this) };
    STabWidget* m_tabs{ nullptr };
	QVector<SToolBar*> m_toolsBars{};
	QVector<SMenu*> m_menus{};

	// Other
    QSharedPointer<SActions> m_actions{};
};

