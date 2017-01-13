#include "includes/SWidgets/Web/SWebHitTestResult.hpp"
#include "includes/SWidgets/Web/SWebPage.hpp"

SWebHitTestResult::SWebHitTestResult(const SWebPage *page, const QPoint &pos) :
    m_isNull(true),
    m_isContentEditable(false),
    m_isContentSelected(false),
    m_mediaPaused(false),
    m_mediaMuted(false),
    m_pos(pos)
{
    QString source = QLatin1String("(function() {"
                          "var e = document.elementFromPoint(%1, %2);"
                          "if (!e)"
                          "    return;"
                          "function isMediaElement(e) {"
                          "    return e.tagName == 'AUDIO' || e.tagName == 'VIDEO';"
                          "}"
                          "function isEditableElement(e) {"
                          "    if (e.isContentEditable)"
                          "        return true;"
                          "    if (e.tagName == 'INPUT' || e.tagName == 'TEXTAREA')"
                          "        return e.getAttribute('readonly') != 'readonly';"
                          "    return false;"
                          "}"
                          "function isSelected(e) {"
                          "    var selection = window.getSelection();"
                          "    if (selection.type != 'Range')"
                          "        return false;"
                          "    return window.getSelection().containsNode(e, true);"
                          "}"
                          "var res = {"
                          "    baseUrl: document.baseURI,"
                          "    alternateText: e.getAttribute('alt'),"
                          "    boundingRect: '',"
                          "    imageUrl: '',"
                          "    contentEditable: isEditableElement(e),"
                          "    contentSelected: isSelected(e),"
                          "    linkTitle: '',"
                          "    linkUrl: '',"
                          "    mediaUrl: '',"
                          "    tagName: e.tagName.toLowerCase()"
                          "};"
                          "var r = e.getBoundingClientRect();"
                          "res.boundingRect = [r.top, r.left, r.width, r.height];"
                          "if (e.tagName == 'IMG')"
                          "    res.imageUrl = e.getAttribute('src');"
                          "if (e.tagName == 'A') {"
                          "    res.linkTitle = e.text;"
                          "    res.linkUrl = e.getAttribute('href');"
                          "}"
                          "while (e) {"
                          "    if (res.linkTitle == '' && e.tagName == 'A')"
                          "        res.linkTitle = e.text;"
                          "    if (res.linkUrl == '' && e.tagName == 'A')"
                          "        res.linkUrl = e.getAttribute('href');"
                          "    if (res.mediaUrl == '' && isMediaElement(e)) {"
                          "        res.mediaUrl = e.currentSrc;"
                          "        res.mediaPaused = e.paused;"
                          "        res.mediaMuted = e.muted;"
                          "    }"
                          "    e = e.parentElement;"
                          "}"
                          "return res;"
                          "})()");

    SWebPage *p = const_cast<SWebPage*>(page);
    m_viewportPos = p->mapToViewport(m_pos);

    const QString &js = source.arg(m_viewportPos.x()).arg(m_viewportPos.y());
    const QUrl &url{ p->url() };
    const QVariantMap &map{ p->executeJavaScript(js, QWebEngineScript::ApplicationWorld).toMap() };

    if (map.isEmpty())
        return;

    m_isNull = false;
    m_baseUrl = map.value(QStringLiteral("baseUrl")).toUrl();
    m_alternateText = map.value(QStringLiteral("alternateText")).toString();
    m_imageUrl = map.value(QStringLiteral("imageUrl")).toUrl();
    m_isContentEditable = map.value(QStringLiteral("contentEditable")).toBool();
    m_isContentSelected = map.value(QStringLiteral("contentSelected")).toBool();
    m_linkTitle = map.value(QStringLiteral("linkTitle")).toString();
    m_linkUrl = map.value(QStringLiteral("linkUrl")).toUrl();
    m_mediaUrl = map.value(QStringLiteral("mediaUrl")).toUrl();
    m_mediaPaused = map.value(QStringLiteral("mediaPaused")).toBool();
    m_mediaMuted = map.value(QStringLiteral("mediaMuted")).toBool();
    m_tagName = map.value(QStringLiteral("tagName")).toString();

    const QVariantList &rect = map.value(QStringLiteral("boundingRect")).toList();
    if (rect.size() == 4)
        m_boundingRect = QRect(rect.at(0).toInt(), rect.at(1).toInt(), rect.at(2).toInt(), rect.at(3).toInt());

    if (!m_imageUrl.isEmpty())
        m_imageUrl = url.resolved(m_imageUrl);
    if (!m_linkUrl.isEmpty())
        m_linkUrl = m_baseUrl.resolved(m_linkUrl);
    if (!m_mediaUrl.isEmpty())
        m_mediaUrl = url.resolved(m_mediaUrl);
}

void SWebHitTestResult::updateWithContextMenuData(const QWebEngineContextMenuData &data)
{
    if (!data.isValid()) {
        return;
    }

    m_linkTitle = data.linkText();
    m_linkUrl = data.linkUrl();
    m_isContentEditable = data.isContentEditable();
    m_isContentSelected = !data.selectedText().isEmpty();

    switch (data.mediaType()) {
    case QWebEngineContextMenuData::MediaTypeImage:
        m_imageUrl = data.mediaUrl();
        break;

    case QWebEngineContextMenuData::MediaTypeVideo:
    case QWebEngineContextMenuData::MediaTypeAudio:
        m_mediaUrl = data.mediaUrl();
        break;

    default:
        break;
    }
}

QUrl SWebHitTestResult::baseUrl() const
{
    return m_baseUrl;
}

QString SWebHitTestResult::alternateText() const
{
    return m_alternateText;
}

QRect SWebHitTestResult::boundingRect() const
{
    return m_boundingRect;
}

QUrl SWebHitTestResult::imageUrl() const
{
    return m_imageUrl;
}

bool SWebHitTestResult::isContentEditable() const
{
    return m_isContentEditable;
}

bool SWebHitTestResult::isContentSelected() const
{
    return m_isContentSelected;
}

bool SWebHitTestResult::isNull() const
{
    return m_isNull;
}

QString SWebHitTestResult::linkTitle() const
{
    return m_linkTitle;
}

QUrl SWebHitTestResult::linkUrl() const
{
    return m_linkUrl;
}

QUrl SWebHitTestResult::mediaUrl() const
{
    return m_mediaUrl;
}

bool SWebHitTestResult::mediaPaused() const
{
    return m_mediaPaused;
}

bool SWebHitTestResult::mediaMuted() const
{
    return m_mediaMuted;
}

QPoint SWebHitTestResult::pos() const
{
    return m_pos;
}

QPointF SWebHitTestResult::viewportPos() const
{
    return m_viewportPos;
}

QString SWebHitTestResult::tagName() const
{
    return m_tagName;
}
