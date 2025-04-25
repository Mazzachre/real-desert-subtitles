#pragma once

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include "authentication.h"

namespace Rd {
    namespace Library {
        class SubtitleDownloader : public QObject {
            Q_OBJECT
        public:
            explicit SubtitleDownloader(QObject* parent = nullptr);
            ~SubtitleDownloader() noexcept;

            void download(quint64 fileId);

            Q_SIGNAL void error(const QString& error, const QString& body) const;
            Q_SIGNAL void usage(quint32 remaining, const QString& reset) const;
            Q_SIGNAL void found(const QUrl& url, const QString& filename) const;
        private:
            QNetworkAccessManager* m_networkAccess;
            Authentication* m_authentication;
            quint64 m_fileId;

            Q_SLOT void handleAuthenticated(const QString& token);
            Q_SLOT void handleResponse(QNetworkReply *reply);
        };
    }
}
