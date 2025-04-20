#include "drop-target.h"

#include <QDebug>
#include <QFileInfo>

Rd::Ui::DropTarget::DropTarget(QObject *parent)
: QObject(parent)
, m_db{new QMimeDatabase} {}

Rd::Ui::DropTarget::~DropTarget() noexcept {
    delete m_db;
}

bool Rd::Ui::DropTarget::acceptEntered(const QList<QUrl>& urls) const {
    if (urls.count() != 1 || !urls[0].isLocalFile()) return false;
    QFileInfo fileInfo(urls[0].toLocalFile());
    if (!fileInfo.isFile()) return false;
    QMimeType type = m_db->mimeTypeForFile(fileInfo);
    return type.name().startsWith(u"video/"_qs);
}

void Rd::Ui::DropTarget::handle(const QList<QUrl>& urls) const {
    Q_EMIT fileDropped(urls[0]);
}
