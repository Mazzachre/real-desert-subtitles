#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include "../types/subtitle-result.h"

namespace Rd {
    namespace Library {
        class SubtitleFinder : public QObject {
            Q_OBJECT
        public:
            SubtitleFinder(QObject* parent = nullptr);
            ~SubtitleFinder();

            void findByHash(const QString& hash);

            Q_SIGNAL void error(const QString& error) const;
            Q_SIGNAL void noSubtitlesFound() const;
            Q_SIGNAL void subtitlesFound(const QList<SubtitleResult>& results) const;
        private:
            QNetworkAccessManager *m_networkAccess;

            Q_SLOT void handleResponse(QNetworkReply *reply);
        };
    }
}
