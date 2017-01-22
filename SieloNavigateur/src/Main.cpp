#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QTextCodec>
#include <QtWidgets>
#include <QtWebEngineWidgets>
#include <QStyle>
#include <QDataStream>
#include <QSettings>

#include "includes/SWidgets/OLDSWebView.hpp"
#include "includes/SWidgets/STabWidget.hpp"
#include "includes/SMainWindow.hpp"
#include "includes/SApplication.hpp"
#include "includes/SWindows/History/SHistoryItem.hpp"


int main(int argc, char **argv)
{
    qRegisterMetaTypeStreamOperators<SHistoryItem>("SHistoryItem");
    SApplication app(argc, argv);

	QString locale = QLocale::system().name().section('_', 0, 0);
	QTranslator translator;
	translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&translator);

	return app.exec();
}
