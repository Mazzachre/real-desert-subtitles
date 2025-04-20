#pragma once

#include <QAbstractListModel>
#include "../types/subtitle-result.h"
#include <QHash>

namespace Rd {
    namespace Ui {
        class FeatureModel : public QAbstractListModel {
            Q_OBJECT
        public:
            enum FeatureRoles {
                IdRole = Qt::UserRole + 1,
                ImdbRole,
                TitleRole,
                YearRole
            };

            explicit FeatureModel(QObject* parent = nullptr);
            int rowCount(const QModelIndex& parent = QModelIndex()) const;
            QHash<int, QByteArray> roleNames() const;
            QVariant data(const QModelIndex &index, int role) const;

            Q_SLOT void setFeatures(const QList<Feature>& results);
            Q_SLOT void clear();

            Feature getFeature(quint64 id) const;
        private:
            QList<Feature> m_results;
        };
    }
}
