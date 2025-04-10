#pragma once

#include <QAbstractListModel>
#include <QUrl>
#include "../types/subtitle-result.h"

namespace Rd {
    namespace Application {
        class SubtitleModel : public QAbstractListModel {
            Q_OBJECT
            Q_PROPERTY(bool results READ results NOTIFY resultsUpdated)
            Q_PROPERTY(bool onlyMatch READ onlyMatch WRITE setOnlyMatch NOTIFY onlyMatchUpdated)
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
            int rowCount(const QModelIndex& parent = QModelIndex()) const;
            QHash<int, QByteArray> roleNames() const;
            QVariant data(const QModelIndex &index, int role) const;

            bool results() const;
            Q_SIGNAL void resultsUpdated() const;

            bool onlyMatch() const;
            void setOnlyMatch(bool match);
            Q_SIGNAL void onlyMatchUpdated() const;

            Q_SLOT void setSelected(const QUrl& file, const Feature& feature);
            Q_SLOT void clear();
        private:
            bool m_onlyMatch;
            QUrl m_file;
            QList<Subtitle> m_subtitles;
        };
    }
}
