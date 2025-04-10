#pragma once

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include "../types/subtitle-result.h"

// We need authentication too
namespace Rd {
    namespace Library {
        class SubtitleFinder : public QObject {
            Q_OBJECT
        public:
            SubtitleFinder(QObject* parent = nullptr);
            ~SubtitleFinder();

            void findByFile(const QString& hash, const QUrl& file);

            Q_SIGNAL void error(const QString& error) const;
            Q_SIGNAL void noSubtitlesFound() const;
            Q_SIGNAL void subtitlesFound(const QList<Feature>& results) const;
        private:
            QNetworkAccessManager *m_networkAccess;

            Q_SLOT void handleResponse(QNetworkReply *reply);
        };
    }
}
