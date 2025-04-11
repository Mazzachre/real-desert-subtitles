#include "subtitle-finder.h"
#include "../../key.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

Rd::Library::SubtitleFinder::SubtitleFinder(QObject* parent)
: QObject(parent)
, m_networkAccess{new QNetworkAccessManager} {
    connect(m_networkAccess, &QNetworkAccessManager::finished, this, &Rd::Library::SubtitleFinder::handleResponse);
}

Rd::Library::SubtitleFinder::~SubtitleFinder() {
    delete m_networkAccess;
}

void Rd::Library::SubtitleFinder::findByFile(const QString& hash, const QUrl& file) {
    QUrlQuery query;
    query.addQueryItem(u"ai_translated"_qs, u"exclude"_qs);
    query.addQueryItem(u"languages"_qs, u"en"_qs);//config value
//    query.addQueryItem(u"foreign_parts_only"_qs, u"exclude"_qs);//config value?
    query.addQueryItem(u"moviehash"_qs, hash);
    query.addQueryItem(u"query"_qs, file.fileName());
    QUrl url(u"https://api.opensubtitles.com/api/v1/subtitles"_qs);
    url.setQuery(query);

    QNetworkRequest req(url);
    req.setRawHeader(QByteArrayLiteral("Api-Key"), QByteArrayLiteral(API_KEY));
    req.setHeader(QNetworkRequest::UserAgentHeader, "ReadDesertSubtitles 1.0");

    m_networkAccess->get(req);
}

void Rd::Library::SubtitleFinder::handleResponse(QNetworkReply *reply) {
    QByteArray result = ((QIODevice *) reply)->readAll();
    qDebug() << "Result" << result << Qt::endl;

    if (reply->error() != QNetworkReply::NoError) {
        Q_EMIT error("Network error - " + ((QIODevice*)reply)->errorString() + QString(result).remove(QRegularExpression("<[^>]*>")).replace(QRegularExpression("[\\s\\n\\r]+"), " "));
        return;
    }

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(result, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        Q_EMIT error(jsonError.errorString());
        return;
    }

    if (doc.object().value(u"total_count"_qs).toInt() == 0) {
        Q_EMIT noSubtitlesFound();
        return;
    }

    QList<Feature> results = parse(doc.object().value(u"data"_qs).toArray());
    Q_EMIT subtitlesFound(results);
    ((QObject *) reply)->deleteLater();
}
