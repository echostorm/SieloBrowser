#pragma once

#include <QObject>
#include <QString>
#include <QUrl>
#include <QDataStream>

class SHistoryItem : public QObject
{
    Q_OBJECT
public:
    SHistoryItem(const QString &title = "Page web", const QUrl& url = QUrl("blank"), QObject *parent = nullptr) :
        QObject(parent),
        m_title(title),
        m_url(url)
    {}
    SHistoryItem(const SHistoryItem& other) :
        m_title(other.title()),
        m_url(other.url())
    {}
    ~SHistoryItem() {}

    const QString &title() const { return m_title; }
    const QUrl &url() const { return m_url; }

    void setTitle(QString &title) { m_title = title; }
    void setUrl(QUrl &url) { m_url = url; }

    bool operator ==(const SHistoryItem& other) const {
        return (m_title == other.title() &&
                m_url == other.url());
    }
    SHistoryItem& operator =(const SHistoryItem& other) {
        m_title = other.title();
        m_url = other.url();
    }

    friend QDataStream& operator <<(QDataStream& out, const SHistoryItem& item) {
        out << item.title() << item.url();
        return out;
    }

    friend QDataStream& operator >>(QDataStream& in, SHistoryItem& item) {
        QString title{};
        QUrl url{};
        in >> title;
        in >> url;
        item.setTitle(title);
        item.setUrl(url);

        return in;
    }

private:
    QString m_title{};
    QUrl m_url{};
};



Q_DECLARE_METATYPE(SHistoryItem)
