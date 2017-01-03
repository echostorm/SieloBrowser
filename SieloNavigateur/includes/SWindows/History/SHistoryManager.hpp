#pragma once

#include "includes/SMainWindow.hpp"

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QSpacerItem>
#include <QStandardItemModel>

#include <QPushButton>
#include <QDialogButtonBox>

class SHistory;
class SHistoryItem;
class SHistoryView;
class SHistoryModel;

class SHistoryWindow : public QDialog
{
    Q_OBJECT
public:
    // Constructor and destructor
    SHistoryWindow(SMainWindow* parent = nullptr);
    ~SHistoryWindow();

    void setupUI();

public slots:
    void load(); //< Load an history item
    void deleteOne(); //< delete an history item
    void deleteAll(); //< delete all history

private:
    // Custom delete functions
    void deleteOneFromParent();
    void deleteOneFromItem();

    SMainWindow* m_parent{ nullptr };

    // Layouts of the window
    QVBoxLayout* m_layout{ new QVBoxLayout(this) };
    QHBoxLayout* m_btnLayout{ new QHBoxLayout() };

    // Widgets of the window
    SHistoryView *m_view{ nullptr };
    SHistoryModel *m_model{ nullptr };
    QSpacerItem* m_hSpacer{ new QSpacerItem(40, 20, QSizePolicy::Expanding) };
    QPushButton* m_deleteOneBtn{ new QPushButton(QObject::tr("Effacer"), this) };
    QPushButton* m_deleteAllBtn{ new QPushButton(QObject::tr("Effacer tout l'historique"), this) };
    QDialogButtonBox* m_boxBtn{ new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, this) };
};
