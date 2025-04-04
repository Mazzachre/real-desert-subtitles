#include "drop-target.h"

#include <QDebug>
#include <QFileInfo>

Rd::Application::DropTarget::DropTarget(QObject *parent)
: QObject(parent)
, m_db{new QMimeDatabase} {}

Rd::Application::DropTarget::~DropTarget() {
    delete m_db;
}

bool Rd::Application::DropTarget::acceptEntered(const QList<QUrl>& urls) const {
    if (urls.count() != 1 || !urls[0].isLocalFile()) return false;
    QFileInfo fileInfo(urls[0].toLocalFile());
    if (!fileInfo.isFile()) return false;
    QMimeType type = m_db->mimeTypeForFile(fileInfo);
    return type.name().startsWith(u"video/"_qs);
}

void Rd::Application::DropTarget::handle(const QList<QUrl>& urls) const {
    Q_EMIT fileDropped(urls[0]);
}
