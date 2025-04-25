#include "authentication.h"
#include "../../key.h"
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>

Rd::Library::Authentication::Authentication(QObject* parent)
: QObject(parent)
, m_networkAccess{new QNetworkAccessManager} {
    connect(m_networkAccess, &QNetworkAccessManager::finished, this, &Authentication::handleResponse);
}

void Rd::Library::Authentication::authenticate() {
    QSettings settings;
    QString username = settings.value(u"username"_qs).toString();
    QString password = settings.value(u"password"_qs).toString();

    if (username.isEmpty() || password.isEmpty()) {
        Q_EMIT authenticated(QString());
        return;
    }

    QJsonObject obj
    {
        {"username", QJsonValue(username)},
        {"password", QJsonValue(password)}
    };

    QJsonDocument doc;
    doc.setObject(obj);

    QNetworkRequest req(QUrl(u"https://api.opensubtitles.com/api/v1/download"_qs));

    req.setRawHeader(QByteArrayLiteral("Api-Key"), QByteArrayLiteral(API_KEY));
    req.setHeader(QNetworkRequest::UserAgentHeader, "ReadDesertSubtitles 1.0");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("application/json"));

    m_networkAccess->post(req, doc.toJson());
}


void Rd::Library::Authentication::handleResponse(QNetworkReply* reply) {
    QByteArray result = ((QIODevice *) reply)->readAll();
    qDebug() << "Result" << result << Qt::endl;

    if (reply->error() != QNetworkReply::NoError) {
        Q_EMIT error("Network error - " + ((QIODevice*)reply)->errorString(), QString(result));
        return;
    }

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(result, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        Q_EMIT error("Error parsing search result", jsonError.errorString());
        return;
    }

    QJsonObject obj = doc.object();
    QString token = obj[u"token"_qs].toString();

    Q_EMIT authenticated(token);
}
