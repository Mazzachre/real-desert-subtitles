#include "feature-model.h"
#include <QDebug>

Rd::Application::FeatureModel::FeatureModel(QObject* parent)
: QAbstractListModel(parent) {
}

int Rd::Application::FeatureModel::rowCount(const QModelIndex& parent) const {
    return m_results.size();
}

QHash<int, QByteArray> Rd::Application::FeatureModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[TitleRole] = "title";
    roles[YearRole] = "year";
    return roles;
}

QVariant Rd::Application::FeatureModel::data(const QModelIndex& index, int role) const {
	if (index.isValid()) {
        Feature feature = m_results[index.row()];
        switch(role) {
            case IdRole:
                return feature.imdbId;
            case TitleRole:
                return feature.display();
            case YearRole:
                return QString::number(feature.year);
        }
    }
    return QVariant();
}

void Rd::Application::FeatureModel::clear() {
    beginResetModel();
    m_results.clear();
    endResetModel();
    Q_EMIT resultsUpdated();
}

bool Rd::Application::FeatureModel::results() const {
    return !m_results.empty();
}

void Rd::Application::FeatureModel::setResults(const QUrl& file, const QList<Feature>& results) {
    if (results.size() == 1) {
        Q_EMIT featureSelected(file, results[0]);
    } else {
        m_file = file;
        beginResetModel();
        m_results = results;
        std::sort(m_results.begin(), m_results.end(), featureSort);
        endResetModel();
    }

    Q_EMIT resultsUpdated();
}

void Rd::Application::FeatureModel::selectFeature(quint32 id) {
    for( int i = 0 ; i < m_results.size() ; ++i ) {
        Feature feature = m_results[i];
        if (feature.imdbId == id) {
            Q_EMIT featureSelected(m_file, feature);
        }
    }
    clear();
}
