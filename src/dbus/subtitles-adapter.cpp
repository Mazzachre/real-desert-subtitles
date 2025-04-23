#include "subtitles-adapter.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

SubtitlesAdaptor::SubtitlesAdaptor(QObject *parent)
: QDBusAbstractAdaptor(parent) {}

SubtitlesAdaptor::~SubtitlesAdaptor() {}

void SubtitlesAdaptor::FindFor(const QString& url) {
    QMetaObject::invokeMethod(parent(), "findFile", Q_ARG(QUrl, QUrl(url)));
}

void SubtitlesAdaptor::fileIdentified(const QUrl& file, const Feature& feature) {


    //TODO Here we want to convert from file and feature to JSON string
    qDebug() << "Identified" << file << feature << Qt::endl;
    Q_EMIT Identified("stuff");
}
