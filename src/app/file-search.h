#pragma once

#include <QString>
#include <QObject>
#include <QUrl>
#include "../lib/file-hash.h"
#include "../lib/subtitle-finder.h"

namespace Rd {
    namespace Application {
        class FileSearch : public QObject {
            Q_OBJECT
            Q_PROPERTY(bool working READ working NOTIFY workingUpdated)
            Q_PROPERTY(QString file READ file NOTIFY workingUpdated)
        public:
            FileSearch(QObject *parent = nullptr);
            ~FileSearch();

            bool working() const;
            QString file() const;
            Q_SIGNAL void workingUpdated() const;

            Q_SLOT void find(const QUrl& file);
            Q_SIGNAL void subtitlesFound(const QUrl& file, const QList<Feature>& results) const;

            Q_SLOT void clear();
            Q_SIGNAL void error(const QString& error) const;
        private:
            Q_SLOT void foundNoSubtitles();
            Q_SLOT void foundSubtitles(const QList<Feature>& results);
            Q_SLOT void handleError(const QString& error);

            Rd::Library::FileHash *m_hash;
            Rd::Library::SubtitleFinder *m_finder;
            QUrl m_file;
            bool m_working;
        };
    }
}
