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

void Rd::Library::SubtitleFinder::findByHash(const QString& hash) {
    QUrlQuery query;
    query.addQueryItem("ai_translated", "exclude");
    query.addQueryItem("languages", "en");//config value
    query.addQueryItem("moviehash", hash);
    QUrl url("https://api.opensubtitles.com/api/v1/subtitles");
    url.setQuery(query);

    QNetworkRequest req(url);
    req.setRawHeader(QByteArrayLiteral("Api-Key"), QByteArrayLiteral(API_KEY));
    req.setHeader(QNetworkRequest::UserAgentHeader, "ReadDesertSubtitles 1.0");

    m_networkAccess->get(req);
}

QList<SubtitleResult> parseData(const QJsonArray& items) {
    QList<SubtitleResult> results;
    for (int i = 0 ; i < items.size() ; ++i) {
        results << SubtitleResult(items[i].toObject());
    }
    return results;
}

void Rd::Library::SubtitleFinder::handleResponse(QNetworkReply *reply) {
    //How do we know if we were successful?
    QByteArray result = ((QIODevice *) reply)->readAll();

    qDebug() << "Result" << result << Qt::endl;

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

    QList<SubtitleResult> results = parseData(doc.object().value(u"data"_qs).toArray());
    Q_EMIT subtitlesFound(results);
    ((QObject *) reply)->deleteLater();
}
