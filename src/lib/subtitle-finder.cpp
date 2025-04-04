#include "subtitle-finder.h"

Rd::Library::SubtitleFinder::SubtitleFinder(QObject* parent)
: QObject(parent)
, m_networkAccess{new QNetworkAccessManager} {
}

Rd::Library::SubtitleFinder::~SubtitleFinder() {
    delete m_networkAccess;
}
