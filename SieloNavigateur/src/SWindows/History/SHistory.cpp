#include "includes/SWindows/History/SHistory.hpp"
#include "includes/SWindows/History/SHistoryItem.hpp"

SHistory::SHistory(QObject *parent) :
    QObject(parent)
{
    QDate currentDate{ QDate::currentDate() };
    mApp->settings()->beginGroup("History/" + QString::number(currentDate.year()) + '/' + QString::number(currentDate.month()) + '/' + QString::number(currentDate.day()));

    int size{ mApp->settings()->beginReadArray("history") };

    for (int i{ 0 }; i < size; ++i) {
        mApp->settings()->setArrayIndex(i);
        m_items.append(mApp->settings()->value("item").value<SHistoryItem>());
    }

    mApp->settings()->endArray();
    mApp->settings()->endGroup();
}

SHistory::~SHistory()
{
    // Empty
}

void SHistory::load(SHistoryItem &item)
{
    // Empty
}

void SHistory::save()
{
    QDate currentDate{ QDate::currentDate() };
    mApp->settings()->beginGroup("History/" + QString::number(currentDate.year()) + '/' + QString::number(currentDate.month()) + '/' + QString::number(currentDate.day()));
    mApp->settings()->remove("");

    mApp->settings()->beginWriteArray("history");

    for (int i{ 0 }; i < m_items.size(); ++i) {
        mApp->settings()->setArrayIndex(i);
        mApp->settings()->setValue("item", QVariant::fromValue(m_items[i]));
    }
    mApp->settings()->endArray();
    mApp->settings()->endGroup();

}

void SHistory::addItem(SHistoryItem &item)
{
    if (m_items.contains(item))
        deleteOne(item);

    m_items.append(item);
}

void SHistory::deleteOne(SHistoryItem &item)
{
    m_items.removeOne(item);
}

void SHistory::deleteAll()
{
    m_items.clear();
}
