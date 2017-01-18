#pragma once

#include <QApplication>
#include <QPointer>
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSpacerItem>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QCheckBox>
#include <QProgressBar>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QCloseEvent>
#include <QStandardPaths>

#define mApp SApplication::instance()

namespace Sn {
enum ObjectName {
    ON_WebView,
    ON_TabBar,
    ON_BrowserWindow
};
enum NewTabType {
    NTT_SelectedTab =1,
    NTT_NotSelectedTab = 2,
    NTT_CleanTab = 4,
    NTT_TabAtEnd = 8,
    NTT_NewEmptyTab = 16,
/* ------------------------- */
    NTT_SelectedNewEmptyTab = NTT_SelectedTab | NTT_TabAtEnd | NTT_NewEmptyTab,
    NTT_SelectedTabAtEnd = NTT_SelectedTab | NTT_TabAtEnd,
    NTT_NotSelectedTabAtEnd = NTT_NotSelectedTab | NTT_TabAtEnd,
    NTT_CleanSelectedTabAtEnd = NTT_SelectedTab | NTT_TabAtEnd | NTT_CleanTab,
    NTT_CleanSelectedTab = NTT_CleanTab | NTT_SelectedTab,
    NTT_CleanNotSelectedTab = NTT_CleanTab, NTT_NotSelectedTab
};
}

class SWebView;
class SMainWindow;
class SPluginProxy;
class SHistory;
class SHistoryItem;

// This is a basic dialog to show text from the feldrise website at the start of Sielo
class TextToShow : public QDialog
{
public:
    // Constructor and destructor
    TextToShow(QWidget *parent = nullptr);
    ~TextToShow();

private:
    // Layouts and box of the window
    QVBoxLayout *m_layout{ new QVBoxLayout(this) };
    QGroupBox *m_box{ new QGroupBox(this) };
    QVBoxLayout *m_boxLayout{ new QVBoxLayout(m_box) };

    // Widgets of the window
    QLabel *m_textToShow{ new QLabel("Texte", this)};
    QDialogButtonBox *m_boxBtn{ new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this) };

    QNetworkReply *m_reply{}; //< Reply to get the text to show
};

// This is the dialog to announce and do an update
class MaJDialog : public QDialog
{
public:
    // Constructor and destructor
    MaJDialog(QWidget *parent = nullptr);
    ~MaJDialog();

public slots:
    void startUpdate(); //< Start the download of the updater
    void downloadProgress(quint64 bytesReceived, quint16 bytesTotal); //< Increase the QProgressBar
    void save(); //< Save and execute the updater

protected:
    virtual void closeEvent(QCloseEvent *event); //< Prevent from closing updater during the update

private:
    // Boolean to check the progress of the update
    bool m_installingUpdate{ false };
    bool m_updateSuccess{ false };

    // Layouts and boxes of the window
    QVBoxLayout *m_layout{ new QVBoxLayout(this) };
    QGroupBox *m_box{ new QGroupBox(this) };
    QGridLayout *m_boxLayout{ new QGridLayout(m_box) };
    QHBoxLayout *m_btnLayout{ new QHBoxLayout() };

    // Widgets of the window
    QLabel *m_text{ new QLabel(QObject::tr("Nouveautés"), m_box) };
    QLabel *m_icon{ new QLabel(this) };
    QProgressBar *m_progress{ new QProgressBar(m_box) };
    QCheckBox *m_remindMaj{ new QCheckBox(QObject::tr("Rappeler les mises à jour"), this) };
    QPushButton *m_installButton{ new QPushButton(QObject::tr("Installer"), this) };
    QDialogButtonBox *m_boxBtn{ new QDialogButtonBox(Qt::Horizontal, this) };

    // Networks objects to download the updater
    QNetworkReply *m_reply{};
    QNetworkAccessManager m_netManager{};
};


class SApplication : public QApplication {
public:
    SApplication(int &argc, char** argv);
    ~SApplication();

    int windowCount() const;
    QList<SMainWindow*> windows() const;

    bool isPortable() const { return m_isPortable; }

    SMainWindow *getWindow() const;
    SMainWindow *createWindow(bool isPrivateWindow = false, const QUrl &startUrl = QUrl());
    SMainWindow *createWindow(bool isPrivateWindow = false, SWebView *startView = nullptr);

    void loadSettings();

    const QString &version() const { return m_version; }
    const QString &dataPath() const { return m_dataPath; }
    const QString &themePath() const { return m_themePath; }

    QSettings *settings() { return m_settings; }
    SPluginProxy *plugins() { return m_plugins; }
    SHistory *history() { return m_history; }

    bool needMaJ();
    bool needToShowTxt();
    bool isUrlValid(const QUrl &url);

    static SApplication *instance();

private slots:
    void windowDestroyed(QObject *window);

private:
    QNetworkReply *m_reply{}; //< Reply to get the version of Sielo

    const QString m_version{ "0.4.1"};
    const QString m_dataPath{ QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) };
    bool m_isPortable{ false };

    QSettings *m_settings{ nullptr };
    SPluginProxy *m_plugins{ nullptr };
    SHistory *m_history{ nullptr };

    int m_currentTheme{};
    QString m_themePath{};

    QList<SMainWindow*> m_windows;
    QPointer<SMainWindow> m_lastActiveWindow;
};
