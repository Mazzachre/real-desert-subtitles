#include "feature-model.h"
#include <QDebug>

Rd::Ui::FeatureModel::FeatureModel(QObject* parent)
: QAbstractListModel(parent) {
}

int Rd::Ui::FeatureModel::rowCount(const QModelIndex& parent) const {
    return m_results.size();
}

QHash<int, QByteArray> Rd::Ui::FeatureModel::roleNames() const {
    return {
        {IdRole, "id"},
        {ImdbRole, "imdb"},
        {TitleRole, "title"},
        {YearRole, "year"}
    };
}

QVariant Rd::Ui::FeatureModel::data(const QModelIndex& index, int role) const {
	if (index.isValid()) {
        const Feature& feature = m_results[index.row()];
        switch(role) {
            case IdRole:
                return feature.imdbId;
            case ImdbRole:
                return feature.imdbDisplay();
            case TitleRole:
                return feature.titleDisplay();
            case YearRole:
                return QString::number(feature.year);
        }
    }
    return QVariant();
}

void Rd::Ui::FeatureModel::clear() {
    beginResetModel();
    m_results.clear();
    endResetModel();
}

void Rd::Ui::FeatureModel::setFeatures(const QList<Feature>& results) {
    beginResetModel();
    m_results = results;
    std::sort(m_results.begin(), m_results.end(), featureSort);
    endResetModel();
}

Feature Rd::Ui::FeatureModel::getFeature(quint64 id) const {
    for ( int i = 0 ; i < m_results.size() ; ++i ) {
        if (m_results[i].imdbId == id) return m_results[i];
    }

    return Feature();
}
