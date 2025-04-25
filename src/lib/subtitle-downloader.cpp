#include "subtitle-downloader.h"
#include "../../key.h"
#include <QJsonDocument>
#include <QJsonObject>

Rd::Library::SubtitleDownloader::SubtitleDownloader(QObject* parent)
: QObject(parent)
, m_networkAccess{new QNetworkAccessManager}
, m_authentication{new Authentication} {
    connect(m_networkAccess, &QNetworkAccessManager::finished, this, &SubtitleDownloader::handleResponse);
    connect(m_authentication, &Authentication::authenticated, this, &SubtitleDownloader::handleAuthenticated);
}

Rd::Library::SubtitleDownloader::~SubtitleDownloader() noexcept {
    delete m_authentication;
    delete m_networkAccess;
}

void Rd::Library::SubtitleDownloader::download(quint64 fileId) {
    m_fileId = fileId;
    m_authentication->authenticate();
}

void Rd::Library::SubtitleDownloader::handleAuthenticated(const QString& token) {
    QJsonObject obj
    {
        {"file_id", QJsonValue(static_cast<qint64>(m_fileId))}
    };

    QJsonDocument doc;
    doc.setObject(obj);

    QNetworkRequest req(QUrl(u"https://api.opensubtitles.com/api/v1/download"_qs));

    req.setRawHeader(QByteArrayLiteral("Api-Key"), QByteArrayLiteral(API_KEY));
    if (!token.isEmpty()) {
        QString bearer = u"Bearer "_qs+token;
        req.setRawHeader(QByteArrayLiteral("Authorization"), bearer.toUtf8());
    }
    req.setHeader(QNetworkRequest::UserAgentHeader, "ReadDesertSubtitles 1.0");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("application/json"));

    m_networkAccess->post(req, doc.toJson());
}

void Rd::Library::SubtitleDownloader::handleResponse(QNetworkReply* reply) {
    QByteArray result = ((QIODevice *) reply)->readAll();
    qDebug() << "Result" << result << Qt::endl;

    if (reply->error() != QNetworkReply::NoError) {
        Q_EMIT error("Network error - " + ((QIODevice*)reply)->errorString(), QString(result));
        return;
    }

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(result, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        Q_EMIT error("Error parsing download result", jsonError.errorString());
        return;
    }

    QJsonObject obj = doc.object();
    QUrl link = QUrl(obj[u"link"_qs].toString());
    QString filename = obj[u"file_name"_qs].toString();
    quint32 remaining = obj[u"remaining"_qs].toInt();
    QString resetTime = obj[u"reset_time"_qs].toString();

    Q_EMIT usage(remaining, resetTime);
    Q_EMIT found(link, filename);

    ((QObject *) reply)->deleteLater();
}
