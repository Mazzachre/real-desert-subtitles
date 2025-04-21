#include "file-saver.h"
#include <QNetworkReply>
#include <QDir>
#include "../../key.h"

Rd::Library::FileSaver::FileSaver(QObject* parent)
: QObject(parent)
, m_networkAccess{new QNetworkAccessManager} {
    connect(m_networkAccess, &QNetworkAccessManager::finished, this, &FileSaver::handleResponse);
}

Rd::Library::FileSaver::~FileSaver() noexcept {
    delete m_networkAccess;
}

void Rd::Library::FileSaver::download(const QUrl& url, const QString& path, const QString& filename) {
    m_target = QDir::cleanPath(path + QDir::separator() + filename);

    QNetworkRequest req(url);

    req.setRawHeader(QByteArrayLiteral("Api-Key"), QByteArrayLiteral(API_KEY));
    req.setHeader(QNetworkRequest::UserAgentHeader, "ReadDesertSubtitles 1.0");

    m_networkAccess->get(req);
}

void Rd::Library::FileSaver::handleResponse(QNetworkReply* reply) {
    QByteArray result = ((QIODevice *) reply)->readAll();
    qDebug() << "Result" << m_target << result << Qt::endl;

    if (reply->error() != QNetworkReply::NoError) {
        Q_EMIT error("Network error - " + ((QIODevice*)reply)->errorString(), QString(result));
        return;
    }

    QFile file(m_target);
    file.open(QIODevice::WriteOnly);
    file.write(result);
    file.close();

    Q_EMIT done();
    ((QObject *) reply)->deleteLater();
}
