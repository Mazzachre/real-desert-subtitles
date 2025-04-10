#include "subtitle-model.h"

Rd::Application::SubtitleModel::SubtitleModel(QObject* parent)
: QAbstractListModel(parent) {
}

QHash<int, QByteArray> Rd::Application::SubtitleModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[SDHRole] = "sdh";
    roles[MatchRole] = "match";
    roles[FPORole] = "fpo";
    roles[HasCommentsRole] = "hasComments";
    roles[CommentsRole] = "comments";
    return roles;
}

int Rd::Application::SubtitleModel::rowCount(const QModelIndex& parent) const {
    return m_subtitles.size();
}

QVariant Rd::Application::SubtitleModel::data(const QModelIndex& index, int role) const {
	if (index.isValid()) {
        Subtitle subtitle = m_subtitles[index.row()];
        switch(role) {
            case IdRole:
                return subtitle.fileId;
            case NameRole:
                return subtitle.fileName;
            case SDHRole:
                return subtitle.hearingImpaired;
            case MatchRole:
                return subtitle.hashMatch;
            case FPORole:
                return subtitle.fpo;
            case HasCommentsRole:
                return !subtitle.comments.isEmpty();
            case CommentsRole:
                return subtitle.comments;
        }
    }
    return QVariant();
}

bool Rd::Application::SubtitleModel::results() const {
    return !m_subtitles.empty();
}

bool Rd::Application::SubtitleModel::onlyMatch() const {
    return m_onlyMatch;
}

void Rd::Application::SubtitleModel::setOnlyMatch(bool match) {
    m_onlyMatch = match;
    Q_EMIT onlyMatchUpdated();
}


void Rd::Application::SubtitleModel::setSelected(const QUrl& file, const Feature& feature) {
    m_file = file;
    beginResetModel();
    m_subtitles = feature.subtitles;
    std::sort(m_subtitles.begin(), m_subtitles.end(), subtitleSort);
    endResetModel();
    Q_EMIT resultsUpdated();

    m_onlyMatch = false;
    Q_EMIT onlyMatchUpdated();
}

void Rd::Application::SubtitleModel::clear() {
    m_file.clear();
    beginResetModel();
    m_subtitles.clear();
    endResetModel();
    Q_EMIT resultsUpdated();

    m_onlyMatch = false;
    Q_EMIT onlyMatchUpdated();
}
