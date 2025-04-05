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
        switch(role) {
            case IdRole:
                return m_results[index.row()].imdbId;
            case TitleRole:
                return m_results[index.row()].title;
            case YearRole:
                return m_results[index.row()].year;
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

void Rd::Application::FeatureModel::setResults(const QList<SubtitleResult>& results) {
    //TODO If we don't find anything we should error

    beginResetModel();
    m_results = results;
    endResetModel();
    Q_EMIT resultsUpdated();
}
