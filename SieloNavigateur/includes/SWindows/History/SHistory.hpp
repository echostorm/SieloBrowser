#pragma once

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QUrl>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QPushButton>
#include <QDialogButtonBox>

#include "includes/SApplication.hpp"
#include "includes/SWindows/History/SHistoryItem.hpp"

class SHistory : public QObject
{
    Q_OBJECT
public:
    SHistory(QObject *parent);
    ~SHistory();

    void load(SHistoryItem &item);
    void save();

    void addItem(SHistoryItem &item);
    void deleteOne(SHistoryItem &item);
    void deleteAll();

    QList<SHistoryItem> items() const { return m_items; }
private:
    QList<SHistoryItem> m_items{};
};
