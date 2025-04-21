#pragma once

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Rd {
    namespace Library {
        class FileSaver : public QObject {
            Q_OBJECT
        public:
            explicit FileSaver(QObject* parent = nullptr);
            ~FileSaver() noexcept;

            void download(const QUrl& url, const QString& path, const QString& filename);

            Q_SIGNAL void error(const QString& error, const QString& body) const;
            Q_SIGNAL void done();
        private:
            QNetworkAccessManager* m_networkAccess;
            QString m_target;

            Q_SLOT void handleResponse(QNetworkReply *reply);
        };
    }
}

