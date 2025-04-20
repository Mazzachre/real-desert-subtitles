#pragma once

#include <QAbstractListModel>
#include <QUrl>
#include "../types/subtitle-result.h"
#include "../lib/subtitle-downloader.h"

namespace Rd {
    namespace Ui {
        class SubtitleModel : public QAbstractListModel {
            Q_OBJECT
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

            explicit SubtitleModel(QObject* parent = nullptr);
            ~SubtitleModel() noexcept;

            int rowCount(const QModelIndex& parent = QModelIndex()) const;
            QHash<int, QByteArray> roleNames() const;
            QVariant data(const QModelIndex &index, int role) const;

            Q_SLOT void setSubtitles(const QList<Subtitle>& results);
            Q_SLOT void clear();

            Subtitle getSubtitle(quint64 id) const;
        private:
            QList<Subtitle> m_subtitles;
        };
    }
}
