#ifndef SLOADREQUEST_HPP
#define SLOADREQUEST_HPP

#include <QUrl>
#include <QByteArray>

class SLoadRequest
{
public:
    enum Operation {
        GetOp = 0,
        PostOp = 1
    };

    SLoadRequest() :
        m_operation(GetOp)
    {}
    SLoadRequest(const SLoadRequest &other) :
        m_url(other.m_url),
        m_operation(other.m_operation),
        m_data(other.m_data)
    {}
    SLoadRequest(const QUrl &url, Operation operation = GetOp, const QByteArray &data = QByteArray()) :
        m_url(url),
        m_operation(operation),
        m_data(data)
    {}

    SLoadRequest &operator =(const SLoadRequest &other)
    {
        m_url = other.m_url;
        m_operation = other.m_operation;
        m_data = other.m_data;
        return *this;
    }

    bool isEmpty() const { return m_url.isEmpty(); }

    QUrl url() const { return m_url; }
    void setUrl(const QUrl &url) { m_url = url; }

    QString urlString() const { return QUrl::fromPercentEncoding(m_url.toEncoded()); }

    Operation operation() const { return m_operation; }
    void setOperation(Operation operation) { m_operation = operation; }

    QByteArray data() const { return m_data; }
    void setData(const QByteArray &data) { m_data = data; }

private:
    QUrl m_url;
    Operation m_operation;
    QByteArray m_data;
};

#endif // SLOADREQUEST_HPP
