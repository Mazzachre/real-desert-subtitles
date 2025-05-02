#include "subtitle-model.h"
#include <algorithm>

Rd::Ui::SubtitleModel::SubtitleModel(QObject* parent)
: QAbstractListModel(parent) {
}

Rd::Ui::SubtitleModel::~SubtitleModel() noexcept {
}

QHash<int, QByteArray> Rd::Ui::SubtitleModel::roleNames() const {
    return {
        {IdRole, "id"},
        {DownloadedRole, "downloaded"},
        {NameRole, "name"},
        {SDHRole, "sdh"},
        {MatchRole, "match"},
        {FPORole, "fpo"},
        {HasCommentsRole, "hasComments"},
        {CommentsRole, "comments"}
    };
}

int Rd::Ui::SubtitleModel::rowCount(const QModelIndex& parent) const {
    return m_subtitles.size();
}

QVariant Rd::Ui::SubtitleModel::data(const QModelIndex& index, int role) const {
	if (index.isValid()) {
        const Subtitle& subtitle = m_subtitles[index.row()];
        switch(role) {
            case IdRole:
                return subtitle.fileId;
            case DownloadedRole:
                return m_downloaded.contains(subtitle.fileId);
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

void Rd::Ui::SubtitleModel::setSubtitles(const QList<Subtitle>& results) {
    beginResetModel();
    m_subtitles = results;
    std::sort(m_subtitles.begin(), m_subtitles.end(), subtitleSort);
    endResetModel();
}

void Rd::Ui::SubtitleModel::clear() {
    beginResetModel();
    m_subtitles.clear();
    m_downloaded.clear();
    endResetModel();
}

void Rd::Ui::SubtitleModel::markDownloaded(quint64 id) {
    m_downloaded << id;
    for (int i = 0; i < m_subtitles.size(); ++i) {
        if (m_subtitles[i].fileId == id) {
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx);
            break;
        }
    }
}

Subtitle Rd::Ui::SubtitleModel::getSubtitle(quint64 id) const {
    for ( int i = 0 ; i < m_subtitles.size() ; ++i ) {
        if (m_subtitles[i].fileId == id) return m_subtitles[i];
    }

    return Subtitle();
}
