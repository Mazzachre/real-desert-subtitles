#include "file-search.h"

#include <QDebug>

Rd::Application::FileSearch::FileSearch(QObject* parent)
: QObject(parent)
, m_hash{new Rd::Library::FileHash}
, m_finder{new Rd::Library::SubtitleFinder} {
    connect(m_finder, &Rd::Library::SubtitleFinder::subtitlesFound, this, &Rd::Application::FileSearch::foundSubtitles);
    connect(m_finder, &Rd::Library::SubtitleFinder::noSubtitlesFound, this, &Rd::Application::FileSearch::foundNoSubtitles);

    connect(m_hash, &Rd::Library::FileHash::error, this, &Rd::Application::FileSearch::handleError);
    connect(m_finder, &Rd::Library::SubtitleFinder::error, this, &Rd::Application::FileSearch::handleError);
}

Rd::Application::FileSearch::~FileSearch() {
    delete m_finder;
    delete m_hash;
}

void Rd::Application::FileSearch::find(const QUrl& file) {
    m_working = true;
    Q_EMIT workingUpdated();
    m_file = file;
    QString hash = m_hash->computeHash(file);
    m_finder->findByHash(hash);
}

void Rd::Application::FileSearch::clear() {
    m_working = false;
    Q_EMIT workingUpdated();
}


bool Rd::Application::FileSearch::working() const {
    return m_working;
}

QString Rd::Application::FileSearch::file() const {
    return m_file.fileName();
}

void Rd::Application::FileSearch::foundNoSubtitles() {
    if (m_working) {
        qDebug() << "Found no subtitles for file" << Qt::endl;

        m_working = false;
        Q_EMIT workingUpdated();
    }
}

void Rd::Application::FileSearch::foundSubtitles(const QList<SubtitleResult>& results) {
    if (m_working) {
        Q_EMIT subtitlesFound(results);
        m_working = false;
        Q_EMIT workingUpdated();
    }
}

void Rd::Application::FileSearch::handleError(const QString& errorStr) {
    Q_EMIT error(errorStr);
    m_working = false;
    Q_EMIT workingUpdated();
}
