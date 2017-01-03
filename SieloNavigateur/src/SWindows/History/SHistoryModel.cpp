#include "includes/SWindows/History/SHistoryModel.hpp"
#include "includes/SWindows/History/SHistoryItem.hpp"
#include "includes/SWindows/History/SHistory.hpp"
#include "includes/SApplication.hpp"

SHistoryModel::SHistoryModel(QObject *parent) :
    QStandardItemModel(parent)
{
    appendRow(currentSession());

    QDate date{ QDate::currentDate().addDays(-1)};
    QDate limitDate{ date.addDays(-7) };

    for (date = QDate::currentDate().addDays(-1); date > limitDate; date = date.addDays(-1)) {
        QStandardItem *dateItem{ historyFromDate(date) };
        if (dateItem)
            appendRow(dateItem);
    }

    QStringList headersNames{};
    headersNames << tr("Nom des pages") << tr("Adresses");
    setHorizontalHeaderLabels(headersNames);
}

SHistoryModel::~SHistoryModel()
{
    // Empty
}

QStandardItem *SHistoryModel::currentSession()
{
    QList<QStandardItem*> titles{};
    QList<QStandardItem*> urls{};

    for (int i{ mApp->history()->items().size() - 1}; i >= 0; --i) {
        titles.append(new QStandardItem(mApp->history()->items()[i].title()));
        urls.append(new QStandardItem(mApp->history()->items()[i].url().toString()));
    }

    QStandardItem *ret{ new QStandardItem(tr("Aujourd'hui")) };
    ret->appendColumn(titles);
    ret->appendColumn(urls);

    return ret;
}

QStandardItem *SHistoryModel::historyFromDate(QDate &date)
{
    mApp->settings()->beginGroup("History/" + QString::number(date.year()) + '/' + QString::number(date.month()) + '/' + QString::number(date.day()));

    QList<QStandardItem*> titles{};
    QList<QStandardItem*> urls{};
    int size{ mApp->settings()->beginReadArray("history") };

    if (size <= 0)
        return nullptr;

    for (int i{ size - 1}; i >= 0; --i) {
        mApp->settings()->setArrayIndex(i);

        titles.append(new QStandardItem(mApp->settings()->value("item").value<SHistoryItem>().title()));
        urls.append(new QStandardItem(mApp->settings()->value("item").value<SHistoryItem>().url().toString()));
    }
    mApp->settings()->endArray();
    mApp->settings()->endGroup();

    QStandardItem *ret{ new QStandardItem(QString::number(date.month()) + "/" + QString::number(date.day())) };

    if(date == QDate(QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().day() - 1))
        ret->setText(tr("Hier"));

    ret->appendColumn(titles);
    ret->appendColumn(urls);

    return ret;
}
