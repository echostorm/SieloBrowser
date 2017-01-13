#ifndef SWEBHITTESTRESULT_HPP
#define SWEBHITTESTRESULT_HPP

#include <QUrl>
#include <QRect>
#include <QString>
#include <QVariantMap>
#include <QWebEngineContextMenuData>

class SWebPage;
class SWebHitTestResult
{
public:
    SWebHitTestResult(const SWebPage *page, const QPoint &pos);

    void updateWithContextMenuData(const QWebEngineContextMenuData &data);

    QUrl baseUrl() const;
    QString alternateText() const;
    QRect boundingRect() const;
    QUrl imageUrl() const;
    bool isContentEditable() const;
    bool isContentSelected() const;
    bool isNull() const;
    QString linkTitle() const;
    QUrl linkUrl() const;
    QUrl mediaUrl() const;
    bool mediaPaused() const;
    bool mediaMuted() const;
    QPoint pos() const;
    QPointF viewportPos() const;
    QString tagName() const;

private:
    bool m_isNull{ true };
    QUrl m_baseUrl{ QUrl() };
    QString m_alternateText{ QString() };
    QRect m_boundingRect{ QRect() };
    QUrl m_imageUrl{ QUrl() };
    bool m_isContentEditable{ false };
    bool m_isContentSelected{ false };
    QString m_linkTitle{ QString() };
    QUrl m_linkUrl{ QUrl() };
    QUrl m_mediaUrl{ QUrl() };
    bool m_mediaPaused{ false };
    bool m_mediaMuted{ false };
    QPoint m_pos{ QPoint() };
    QPointF m_viewportPos{ QPointF() };
    QString m_tagName{ QString() };
};

#endif // SWEBHITTESTRESULT_HPP
