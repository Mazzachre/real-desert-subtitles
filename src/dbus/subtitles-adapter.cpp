#include "subtitles-adapter.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QVariantMap>
#include <QtCore/QJsonDocument>

SubtitlesAdaptor::SubtitlesAdaptor(QObject *parent)
: QDBusAbstractAdaptor(parent) {}

SubtitlesAdaptor::~SubtitlesAdaptor() {}

void SubtitlesAdaptor::FindFor(const QString& url) {
    QMetaObject::invokeMethod(parent(), "findFile", Q_ARG(QUrl, QUrl(url)));
}

void SubtitlesAdaptor::fileIdentified(const QUrl& file, const Feature& feature) {
    QJsonObject jobj;
    if (feature.type == u"Movie"_qs) {
        QVariantMap map = {
            {"file", file.toLocalFile()},
            {"type", "Movie"},
            {"imdbId", feature.imdbDisplay()},
            {"tmdbId", feature.tmdbId},
            {"title", feature.title},
            {"year", feature.year}
        };
        jobj = QJsonObject::fromVariantMap(map);
    }
    QJsonDocument json(jobj);

    qDebug() << "Identified" << file << feature << Qt::endl;
    Q_EMIT Identified(QString(json.toJson(QJsonDocument::Compact)));
}
