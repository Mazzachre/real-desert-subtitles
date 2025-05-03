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
            {"title", feature.titleDisplay()},
            {"year", feature.year}
        };
        jobj = QJsonObject::fromVariantMap(map);
    } else if (feature.type == u"Episode"_qs) {
        QVariantMap map = {
            {"file", file.toLocalFile()},
            {"type", "Episode"},
            {"title", feature.titleDisplay()},
            {"year", feature.year},
            {"season", feature.season},
            {"episode", feature.episode},
            {"seriesImdbId", feature.imdbDisplay()},
            {"tmdbId", feature.tmdbId},
            {"episodeImdb", "tt"+QString("%1").arg(feature.imdbId, 7, 10, QLatin1Char('0'))}
        };
        jobj = QJsonObject::fromVariantMap(map);
    } else {
        QVariantMap map = {
            {"file", file.toLocalFile()},
            {"type", feature.type},
            {"title", feature.titleDisplay()},
        };
        jobj = QJsonObject::fromVariantMap(map);
    }
    QJsonDocument json(jobj);

    qDebug() << "Identified" << json.toJson() << Qt::endl;
    Q_EMIT Identified(QString(json.toJson(QJsonDocument::Compact)));
}
