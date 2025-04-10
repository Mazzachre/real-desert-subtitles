#pragma once

#include <QAbstractListModel>
#include "../types/subtitle-result.h"
#include <QHash>

//I can do table view?
namespace Rd {
    namespace Application {
        class FeatureModel : public QAbstractListModel {
            Q_OBJECT
            Q_PROPERTY(bool results READ results NOTIFY resultsUpdated)
        public:
            enum FeatureRoles {
                IdRole = Qt::UserRole + 1,
                TitleRole,
                YearRole
            };

            FeatureModel(QObject* parent = nullptr);
            int rowCount(const QModelIndex& parent = QModelIndex()) const;
            QHash<int, QByteArray> roleNames() const;
            QVariant data(const QModelIndex &index, int role) const;

            bool results() const;
            Q_SIGNAL void resultsUpdated() const;

            Q_SLOT void clear();
            Q_SLOT void setResults(const QUrl& file, const QList<Feature>& results);

            Q_SLOT void selectFeature(quint32 id);
            Q_SIGNAL void featureSelected(const QUrl& file, const Feature& feature);
        private:
            QUrl m_file;
            QList<Feature> m_results;
        };
    }
}
