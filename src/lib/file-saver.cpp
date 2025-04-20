#include "file-saver.h"
#include <QNetworkReply>
#include "../../key.h"

Rd::Library::FileSaver::FileSaver(QObject* parent)
: QObject(parent)
, m_networkAccess{new QNetworkAccessManager} {

}

Rd::Library::FileSaver::~FileSaver() noexcept {
    delete m_networkAccess;
}

void Rd::Library::FileSaver::download(const QUrl& url, const QUrl& path, const QString& filename) {
    QNetworkRequest req(url);

    req.setRawHeader(QByteArrayLiteral("Api-Key"), QByteArrayLiteral(API_KEY));
    req.setHeader(QNetworkRequest::UserAgentHeader, "ReadDesertSubtitles 1.0");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("application/json"));

    m_networkAccess->get(req);

}

void Rd::Library::FileSaver::handleResponse(QNetworkReply* reply) {
    QByteArray result = ((QIODevice *) reply)->readAll();
    qDebug() << "Result" << result << Qt::endl;

    if (reply->error() != QNetworkReply::NoError) {
        Q_EMIT error("Network error - " + ((QIODevice*)reply)->errorString(), QString(result));
        return;
    }
}
