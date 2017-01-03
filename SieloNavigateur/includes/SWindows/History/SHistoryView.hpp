#pragma once

#include "includes/SWindows/History/SHistoryManager.hpp"

#include <QTreeView>

class SHistoryView : public QTreeView
{
    Q_OBJECT
public:
    SHistoryView(SHistoryWindow *parent = nullptr);
    ~SHistoryView();

private:
    SHistoryWindow *m_parent{ nullptr };

};
