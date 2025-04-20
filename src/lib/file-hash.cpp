#include "file-hash.h"

#include <QDebug>
#include <QUrl>
#include <QFile>
#include <QDataStream>

Rd::Library::FileHash::FileHash(QObject* parent)
: QObject(parent) {
}

quint64 hashChunk(QDataStream& stream) {
    quint64 hash = 0, block = 0;
    for (quint16 i = 0 ; i < 8192 ; ++i) {
        stream >> block;
        if (stream.status() != QDataStream::Ok) return 0;
        hash += block;
    }
    return hash;
}

QString Rd::Library::FileHash::computeHash(const QUrl& file) const {
    QFile l_file(file.toLocalFile());
    qint64 l_size = l_file.size();
    quint64 hash = l_size;

    if(!l_file.open(QIODeviceBase::ReadOnly | QIODeviceBase::ExistingOnly)) {
        Q_EMIT error("Unable to hash file", l_file.errorString());
        return "";
    }

    QDataStream stream(&l_file);
    stream.setByteOrder(QDataStream::LittleEndian);

    hash += hashChunk(stream);
    if (stream.status() != QDataStream::Ok) {
        Q_EMIT error("Unable to hash file", l_file.errorString());
        return "";
    }

    l_file.seek(l_size-65536);

    hash += hashChunk(stream);
    if (stream.status() != QDataStream::Ok) {
        Q_EMIT error("Unable to hash file", l_file.errorString());
        return "";
    }

    return QString::number(hash, 16).rightJustified(16, '0');
}
