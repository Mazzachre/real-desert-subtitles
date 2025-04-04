#include "file-search.h"

#include <QDebug>

Rd::Application::FileSearch::FileSearch(QObject* parent)
: QObject(parent)
, m_hash{new Rd::Library::FileHash}
, m_finder{new Rd::Library::SubtitleFinder} {
}

Rd::Application::FileSearch::~FileSearch() {
    delete m_finder;
    delete m_hash;
}

void Rd::Application::FileSearch::find(const QUrl& file) {
    m_file = file;
    m_working = true;
    Q_EMIT workingUpdated();
    QString hash = m_hash->computeHash(file);

    m_finder->findByHash(hash);

    qDebug() << hash << Qt::endl;
}

bool Rd::Application::FileSearch::working() const {
    return m_working;
}

QString Rd::Application::FileSearch::file() const {
    return m_file.fileName();
}
