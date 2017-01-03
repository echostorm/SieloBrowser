#include "includes/SApplication.hpp"
#include "includes/SMainWindow.hpp"
#include "includes/SWindows/SPreferences.hpp"
#include "includes/SWidgets/SWebView.hpp"
#include "includes/SPlugins/SPluginProxy.hpp"
#include "includes/SWindows/History/SHistory.hpp"
#include "includes/SWindows/History/SHistoryItem.hpp"

#include <QWebEngineView>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QFile>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QProcess>
#include <QMessageBox>


SApplication *SApplication::instance()
{
    return static_cast<SApplication*>(QCoreApplication::instance());
}

SApplication::SApplication(int &argc, char **argv) :
    QApplication(argc, argv),
    m_settings(new QSettings(m_dataPath + "/settings.ini", QSettings::IniFormat)),
    m_plugins(new SPluginProxy()),
    m_history(new SHistory(this))
{

    loadSettings();

    QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, mApp->settings()->value("preferences/enablePlugins", true).toBool());
    QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::JavascriptEnabled, mApp->settings()->value("preferences/enableJavascript", true).toBool());

    QMessageBox::information(nullptr, "DEBUG", m_dataPath);

    if (m_isPortable) {
        if(!mApp->settings()->value("builded", false).toBool()) {
            QStringList args{};
            args << "decompress" << ":/data/DData" << mApp->dataPath();
            QProcess::execute(QDir(QCoreApplication::applicationDirPath()).absolutePath() + "/SieloDataSoftware", args);
            mApp->settings()->setValue("builded", true);
        }
    }

    if(needMaJ()) {
#ifndef Q_OS_WIN32
        QMessageBox::warning(nullptr, "Mise à joure", "Sielo Navigateur n'est pas à joure, nous vous \n"
                                                      "recommandont de passer à la version " + m_version);

#else
        if (mApp->settings()->value("Maj/remind", true).toBool()) {
            // Show update dialog
            MaJDialog *majDialog{ new MaJDialog(nullptr) };
            majDialog->show();
            return;
        }
#endif
    }

    // Check if we want to open a file at the start of Sielo
    if (QCoreApplication::arguments().count() > 1) {
        SMainWindow *fen{ createWindow(false, QUrl("File:///" + QFileInfo(arguments()[1]).absoluteFilePath())) };
        fen->show();

        return;
    }

    SMainWindow *fen{ createWindow(false, nullptr) };
    fen->show();

    if(needToShowTxt()) {
        // Show the text
        TextToShow *textToShow{ new TextToShow(fen) };
        textToShow->show();
    }

    m_plugins->loadPlugins();
}

SApplication::~SApplication()
{
    m_plugins->shutdown();

    delete m_plugins;
}

SMainWindow *SApplication::createWindow(bool isPrivateWindow, const QUrl &startUrl)
{
    SWebView *view{ new SWebView() };
    view->load(startUrl);
    SMainWindow *fen{ new SMainWindow(view, isPrivateWindow) };
    connect(fen, &SMainWindow::destroyed, this, &SApplication::windowDestroyed);

    m_windows.prepend(fen);
    return fen;
}

SMainWindow *SApplication::createWindow(bool isPrivateWindow, SWebView *startView)
{
    SMainWindow *fen{ new SMainWindow(startView, isPrivateWindow) };
    connect(fen, &SMainWindow::destroyed, this, &SApplication::windowDestroyed);

    m_windows.prepend(fen);
    return new SMainWindow();
}

void SApplication::loadSettings()
{
    m_currentTheme = m_settings->value("preferences/themes/currentTheme", 0).toInt();
    m_themePath = m_dataPath + "/Themes/" + m_settings->value("preferences/themes/" + QString::number(m_currentTheme) + "/name", "SIcons").toString();
}

void SApplication::windowDestroyed(QObject *window)
{
    Q_ASSERT(static_cast<SMainWindow*>(window));
    Q_ASSERT(m_windows.contains(static_cast<SMainWindow*>(window)));

    m_windows.removeOne(static_cast<SMainWindow*>(window));
}

bool SApplication::needMaJ()
{
    // Networks objects to download the last version
    QNetworkAccessManager manager{};
    m_reply = manager.get(QNetworkRequest(QUrl("http://feldrise.com/Sielo/version.txt")));

    // Downloading the last version
    QEventLoop loop{};
    connect(m_reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString newVersion{ m_reply->readAll() };

    return (newVersion == m_version);
}

bool SApplication::needToShowTxt()
{
    // Get if we need to show a text
    QNetworkAccessManager manager{};
    m_reply = manager.get(QNetworkRequest(QUrl("http://feldrise.com/Sielo/showTxt.txt")));
    QEventLoop loop2{};
    connect(m_reply, &QNetworkReply::finished, &loop2, &QEventLoop::quit);
    loop2.exec();

    QString showTxt{ m_reply->readAll() };

    return (showTxt == "true\n");
}

TextToShow::TextToShow(QWidget *parent) :
    QDialog(parent)
{
    // Set attributes of the window
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);

    // Get text to show
    QNetworkAccessManager manager{};
    m_reply = manager.get(QNetworkRequest(QUrl("http://feldrise.com/Sielo/textToShow.html")));
    QEventLoop loop{{}};
    connect(m_reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    m_textToShow->setText(m_reply->readAll());

    // Fill the layout
    m_boxLayout->addWidget(m_textToShow);
    m_layout->addWidget(m_box);
    m_layout->addWidget(m_boxBtn);

    // Connections
    connect(m_boxBtn, &QDialogButtonBox::accepted, this, &TextToShow::accept);
    connect(m_boxBtn, &QDialogButtonBox::rejected, this, &TextToShow::close);
}

TextToShow::~TextToShow()
{
    // Empty
}

MaJDialog::MaJDialog(QWidget * parent) :
    QDialog(parent)
{
    // Set the window attributes
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);

    // Fill the main layout
    m_layout->addWidget(m_box);
    m_layout->addLayout(m_btnLayout);

    // Set widgets attributes
    m_box->setTitle(tr("Une nouvelle mise à jour est diponible !"));
    m_boxBtn->addButton(m_installButton, QDialogButtonBox::AcceptRole);
    m_boxBtn->addButton(QDialogButtonBox::Close);;
    m_remindMaj->setChecked(true);
    m_icon->setPixmap(QPixmap(mApp->dataPath() + "/Images/icon2.PNG"));

    // Get the text to show
    m_reply = m_netManager.get(QNetworkRequest(QUrl("http://feldrise.com/Sielo/updateTxt.html")));
    QEventLoop loop{};
    connect(m_reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    m_text->setText(m_reply->readAll());

    // Fill others layouts
    m_boxLayout->addWidget(m_text, 0, 0, 1, 2);
    m_boxLayout->addWidget(m_icon, 1, 0);
    m_boxLayout->addWidget(m_progress, 1, 1);
    m_btnLayout->addWidget(m_remindMaj);
    m_btnLayout->addWidget(m_boxBtn);

    connect(m_boxBtn, &QDialogButtonBox::accepted, this, &MaJDialog::startUpdate);
    connect(m_boxBtn, &QDialogButtonBox::rejected, this, &MaJDialog::close);
}

MaJDialog::~MaJDialog()
{
}

void MaJDialog::startUpdate()
{
#if SieloPortable
    if (!m_updateSuccess) {
        QDesktopServices::openUrl(QUrl(QDir(QCoreApplication::applicationDirPath()).absolutePath() + "/SieloPortableUpdater.exe"));
        m_updateSuccess = true;
        m_installingUpdate = false;
        close();
    }
#else
    m_boxBtn->setEnabled(false);
    m_installingUpdate = true;

    m_reply = m_netManager.get(QNetworkRequest(QUrl("http://feldrise.com/Sielo/SN_update_setup.exe")));
    connect(m_reply, &QNetworkReply::downloadProgress, this, &MaJDialog::downloadProgress);
    connect(m_reply, &QNetworkReply::finished, this, &MaJDialog::save);
#endif
}

void MaJDialog::downloadProgress(quint64 bytesReceived, quint16 bytesTotal)
{
    m_progress->setRange(0, bytesTotal);
    m_progress->setValue(bytesReceived);
}

void MaJDialog::save()
{
    m_updateSuccess = true;
    m_installingUpdate = false;

    m_reply->deleteLater();
    QFile updater{ QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/SNUpdater.exe" };

    if (!updater.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Erreur"), tr("Erreur lors de l'installation de la mise à jours !"));
        return;
    }

    updater.write(m_reply->readAll());
    updater.close();
    mApp->settings()->setValue("Maj/remind", true);

    QDesktopServices::openUrl(QUrl(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/SNUpdater.exe"));
    if (parentWidget() != nullptr) {
        qApp->quit();
        return;
    }
    close();
}

void MaJDialog::closeEvent(QCloseEvent * event)
{
    // We bloke the close event during the update
    if (m_installingUpdate) {
        event->ignore();
        return;
    }
    if (!m_updateSuccess) {
        if (!m_remindMaj->isChecked()) {
            mApp->settings()->setValue("Maj/remind", false);
        }

        if (parent() == nullptr) {
            SMainWindow* fen{ new SMainWindow() };

            QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
            QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, mApp->settings()->value("preferences/enablePlugins", true).toBool());
            QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::JavascriptEnabled, mApp->settings()->value("preferences/enableJavascript", true).toBool());

            fen->show();
        }
    }
}
