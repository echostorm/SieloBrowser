#pragma once

#include <QStandardItemModel>
#include <QDate>

class SHistoryModel : public QStandardItemModel
{
    Q_OBJECT
public:
    SHistoryModel(QObject *parent = nullptr);
    ~SHistoryModel();

    QStandardItem *currentSession();
    QStandardItem *historyFromDate(QDate &date);
};
