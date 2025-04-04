#include "subtitle-finder.h"
#include "../../key.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include <QDebug>

Rd::Library::SubtitleFinder::SubtitleFinder(QObject* parent)
: QObject(parent)
, m_networkAccess{new QNetworkAccessManager} {

    connect(m_networkAccess, &QNetworkAccessManager::finished, this, &Rd::Library::SubtitleFinder::handleResponse);
}

Rd::Library::SubtitleFinder::~SubtitleFinder() {
    delete m_networkAccess;
}

void Rd::Library::SubtitleFinder::findByHash(const QString& hash) {
    QUrlQuery query;
    query.addQueryItem("ai_translated", "exclude");
    query.addQueryItem("languages", "en");
    query.addQueryItem("moviehash", hash);
    QUrl url("https://api.opensubtitles.com/api/v1/subtitles");
    url.setQuery(query);

    QNetworkRequest req(url);
    req.setRawHeader(QByteArrayLiteral("Api-Key"), QByteArrayLiteral(API_KEY));
    req.setHeader(QNetworkRequest::UserAgentHeader, "ReadDesertSubtitles 1.0");

    m_networkAccess->get(req);
}

void Rd::Library::SubtitleFinder::handleResponse(QNetworkReply *reply) {
    //How do we know if we were successful?

    qDebug() << ((QIODevice *) reply)->readAll() << Qt::endl;
    ((QObject *) reply)->deleteLater();
}
