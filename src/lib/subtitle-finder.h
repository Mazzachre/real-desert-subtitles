#pragma once

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include "file-hash.h"
#include "../types/subtitle-result.h"

// We need authentication too
namespace Rd {
    namespace Library {
        class SubtitleFinder : public QObject {
            Q_OBJECT
        public:
            explicit SubtitleFinder(QObject* parent = nullptr);
            ~SubtitleFinder() noexcept;

            void findByFile(const QUrl& file);
            void findMovie(const QString& title, const QString& year);
            void findShow(const QString& title, const QString& season, const QString& episode);

            Q_SIGNAL void error(const QString& error, const QString& body) const;
            Q_SIGNAL void noSubtitlesFound() const;
            Q_SIGNAL void subtitlesFound(const QList<Feature>& results) const;
        private:
            QNetworkAccessManager* m_networkAccess;
            FileHash* m_fileHash;

            Q_SLOT void handleResponse(QNetworkReply* reply);
        };
    }
}
