#include "subtitle-finder.h"
#include "../../key.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QSettings>

Rd::Library::SubtitleFinder::SubtitleFinder(QObject* parent)
: QObject(parent)
, m_networkAccess{new QNetworkAccessManager}
, m_fileHash{new FileHash} {
    connect(m_networkAccess, &QNetworkAccessManager::finished, this, &Rd::Library::SubtitleFinder::handleResponse);
}

Rd::Library::SubtitleFinder::~SubtitleFinder() noexcept {
    delete m_fileHash;
    delete m_networkAccess;
}

void Rd::Library::SubtitleFinder::findByFile(const QUrl& file) {
    QString hash = m_fileHash->computeHash(file);
    QSettings settings;
    QString fpo = settings.value(u"fpo"_qs, false).toBool() ? u"include"_qs : u"exclude"_qs;
    QString lang = settings.value(u"languages"_qs, "en").toString();

    QUrlQuery query;
    query.addQueryItem(u"ai_translated"_qs, u"exclude"_qs);
    query.addQueryItem(u"foreign_parts_only"_qs, fpo);
    query.addQueryItem(u"languages"_qs, lang);
    query.addQueryItem(u"moviehash"_qs, hash);
    query.addQueryItem(u"query"_qs, file.fileName());

    QUrl url(u"https://api.opensubtitles.com/api/v1/subtitles"_qs);
    url.setQuery(query);

    QNetworkRequest req(url);
    req.setRawHeader(QByteArrayLiteral("Api-Key"), QByteArrayLiteral(API_KEY));
    req.setHeader(QNetworkRequest::UserAgentHeader, "ReadDesertSubtitles 1.0");

    m_networkAccess->get(req);
}

void Rd::Library::SubtitleFinder::findMovie(const QString& title, const QString& year) {
    QSettings settings;
    QString fpo = settings.value(u"fpo"_qs, false).toBool() ? u"include"_qs : u"exclude"_qs;
    QString lang = settings.value(u"languages"_qs, "en").toString();

    QUrlQuery query;
    query.addQueryItem(u"ai_translated"_qs, u"exclude"_qs);
    query.addQueryItem(u"foreign_parts_only"_qs, fpo);
    query.addQueryItem(u"languages"_qs, lang);
    query.addQueryItem(u"query"_qs, title);
    query.addQueryItem(u"type"_qs, u"movie"_qs);
    query.addQueryItem(u"year"_qs, year);

    QUrl url(u"https://api.opensubtitles.com/api/v1/subtitles"_qs);
    url.setQuery(query);

    QNetworkRequest req(url);
    req.setRawHeader(QByteArrayLiteral("Api-Key"), QByteArrayLiteral(API_KEY));
    req.setHeader(QNetworkRequest::UserAgentHeader, "ReadDesertSubtitles 1.0");

    m_networkAccess->get(req);
}

void Rd::Library::SubtitleFinder::findShow(const QString& title, const QString& season, const QString& episode) {
    QSettings settings;
    QString fpo = settings.value(u"fpo"_qs, false).toBool() ? u"include"_qs : u"exclude"_qs;
    QString lang = settings.value(u"languages"_qs, "en").toString();

    QUrlQuery query;
    query.addQueryItem(u"ai_translated"_qs, u"exclude"_qs);
    query.addQueryItem(u"episode"_qs, episode);
    query.addQueryItem(u"foreign_parts_only"_qs, fpo);
    query.addQueryItem(u"languages"_qs, lang);
    query.addQueryItem(u"query"_qs, title);
    query.addQueryItem(u"season"_qs, season);
    query.addQueryItem(u"type"_qs, u"episode"_qs);

    QUrl url(u"https://api.opensubtitles.com/api/v1/subtitles"_qs);
    url.setQuery(query);

    QNetworkRequest req(url);
    req.setRawHeader(QByteArrayLiteral("Api-Key"), QByteArrayLiteral(API_KEY));
    req.setHeader(QNetworkRequest::UserAgentHeader, "ReadDesertSubtitles 1.0");

    m_networkAccess->get(req);
}

void Rd::Library::SubtitleFinder::handleResponse(QNetworkReply *reply) {
    QByteArray result = reply->readAll();
    qDebug() << "Result" << result << Qt::endl;

    if (reply->error() != QNetworkReply::NoError) {
        Q_EMIT error("Network error - " + reply->errorString(),  QString(result));
        return;
    }

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(result, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        Q_EMIT error("Error parsing search result", jsonError.errorString());
        return;
    }

    if (doc.object().value(u"total_count"_qs).toInt() == 0) {
        Q_EMIT noSubtitlesFound();
        return;
    }

    QList<Feature> results = parse(doc.object().value(u"data"_qs).toArray());
    Q_EMIT subtitlesFound(results);
    reply->deleteLater();
}
