#pragma once

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>

namespace Rd {
    namespace Library {
        class SubtitleDownloader : public QObject {
            Q_OBJECT
        public:
            SubtitleDownloader(QObject* parent = nullptr);
            ~SubtitleDownloader();

            void download(quint64 fileId);

            Q_SIGNAL void error(const QString& error) const;
        private:
            QNetworkAccessManager* m_networkAccess;

            Q_SLOT void handleResponse(QNetworkReply *reply);
        };
    }
}
