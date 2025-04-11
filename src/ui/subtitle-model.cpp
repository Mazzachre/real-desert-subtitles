#include "subtitle-model.h"

Rd::Ui::SubtitleModel::SubtitleModel(QObject* parent)
: QAbstractListModel(parent)
, m_downloader{new Rd::Library::SubtitleDownloader} {
}

Rd::Ui::SubtitleModel::~SubtitleModel() {
    delete m_downloader;
}

QHash<int, QByteArray> Rd::Ui::SubtitleModel::roleNames() const {
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

int Rd::Ui::SubtitleModel::rowCount(const QModelIndex& parent) const {
    return m_subtitles.size();
}

QVariant Rd::Ui::SubtitleModel::data(const QModelIndex& index, int role) const {
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

bool Rd::Ui::SubtitleModel::results() const {
    return !m_subtitles.empty();
}

void Rd::Ui::SubtitleModel::download(quint64 fileId) {
    m_downloader->download(fileId);
}

void Rd::Ui::SubtitleModel::setSelected(const QUrl& file, const Feature& feature) {
    m_file = file;
    beginResetModel();
    m_subtitles = feature.subtitles;
    std::sort(m_subtitles.begin(), m_subtitles.end(), subtitleSort);
    endResetModel();
    Q_EMIT resultsUpdated();
}

void Rd::Ui::SubtitleModel::clear() {
    m_file.clear();
    beginResetModel();
    m_subtitles.clear();
    endResetModel();
    Q_EMIT resultsUpdated();
}
