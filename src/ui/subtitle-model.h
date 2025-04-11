#pragma once

#include <QAbstractListModel>
#include <QUrl>
#include "../types/subtitle-result.h"
#include "../lib/subtitle-downloader.h"

namespace Rd {
    namespace Ui {
        class SubtitleModel : public QAbstractListModel {
            Q_OBJECT
            Q_PROPERTY(bool results READ results NOTIFY resultsUpdated)
        public:
            enum FeatureRoles {
                IdRole = Qt::UserRole + 1,
                NameRole,
                SDHRole,
                MatchRole,
                FPORole,
                HasCommentsRole,
                CommentsRole
            };

            SubtitleModel(QObject* parent = nullptr);
            ~SubtitleModel();
            int rowCount(const QModelIndex& parent = QModelIndex()) const;
            QHash<int, QByteArray> roleNames() const;
            QVariant data(const QModelIndex &index, int role) const;

            bool results() const;
            Q_SIGNAL void resultsUpdated() const;

            Q_SLOT void download(quint64 fileId);
            Q_SLOT void setSelected(const QUrl& file, const Feature& feature);
            Q_SLOT void clear();
        private:
            QUrl m_file;
            QList<Subtitle> m_subtitles;
            Rd::Library::SubtitleDownloader* m_downloader;
        };
    }
}
