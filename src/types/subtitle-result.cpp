#include "subtitle-result.h"
#include <QJsonArray>

Subtitle::Subtitle() {}

Subtitle::Subtitle(bool hearingImpaired, double fps, bool hashMatch, bool fpo, const QString& comments, const QJsonObject& item) {
    this->hearingImpaired = hearingImpaired;
    this->fps = fps;
    this->hashMatch = hashMatch;
    this->fpo = fpo;
    this->comments = comments;
    fileId = item.value(u"file_id"_qs).toInt();
    fileName = item.value(u"file_name"_qs).toString();
}

Subtitle::Subtitle(const Subtitle& other) {
    hearingImpaired = other.hearingImpaired;
    fps = other.fps;
    hashMatch = other.hashMatch;
    fpo = other.fpo;
    comments = other.comments;
    fileId = other.fileId;
    fileName = other.fileName;
}

Subtitle& Subtitle::operator=(const Subtitle& other) {
    hearingImpaired = other.hearingImpaired;
    fps = other.fps;
    hashMatch = other.hashMatch;
    fpo = other.fpo;
    comments = other.comments;
    fileId = other.fileId;
    fileName = other.fileName;
    return *this;
}

void Subtitle::clear() {
    hearingImpaired = false;
    fps = 0.0;
    hashMatch = false;
    fpo = false;
    comments.clear();
    fileId = 0;
    fileName.clear();
}

QDebug operator <<(QDebug dbg, const Subtitle& subtitle) {
    dbg.nospace().noquote() << "Subtitle:(" << subtitle.fileName << " " <<  subtitle.hashMatch << ")";
    return dbg;
}

Feature::Feature() {}

Feature::Feature(const QJsonObject& item) {
    QJsonObject attributes = item.value(u"attributes"_qs).toObject();
    bool hearingImpaired = attributes.value(u"imdb_id"_qs).toBool();
    double fps = attributes.value(u"fps"_qs).toDouble();
    bool hashMatch = attributes.value(u"moviehash_match"_qs).toBool();
    bool fpo =  attributes.value(u"foreign_parts_only"_qs).toBool();
    QString comments = attributes.value(u"comments"_qs).toString();
    QJsonObject feature = attributes.value(u"feature_details"_qs).toObject();

    imdbId = feature.value(u"imdb_id"_qs).toInt();
    tmdbId = feature.value(u"tmdb_id"_qs).toInt();
    year = feature.value(u"year"_qs).toInt();
    season = feature.value(u"season_number"_qs).toInt();
    episode = feature.value(u"episode_number"_qs).toInt();
    parentImdbId = feature.value(u"parent_imdb_id"_qs).toInt();
    parentTmdbId = feature.value(u"parent_tmdb_id"_qs).toInt();
    type = feature.value(u"feature_type"_qs).toString();
    title = feature.value(u"title"_qs).toString();
    parentTitle = feature.value(u"parent_title"_qs).toString();

    QJsonArray fileArray = attributes.value(u"files"_qs).toArray();
    subtitles << Subtitle(hearingImpaired, fps, hashMatch, fpo, comments, fileArray[0].toObject());
}

Feature::Feature(const Feature& other) {
    imdbId = other.imdbId;
    tmdbId = other.tmdbId;
    year = other.year;
    season = other.season ;
    episode = other.episode;
    parentImdbId = other.parentImdbId;
    parentTmdbId = other.parentTmdbId;
    type = other.type;
    title = other.title;
    parentTitle = other.parentTitle;
    subtitles = other.subtitles;
}

Feature& Feature::operator=(const Feature& other) {
    imdbId = other.imdbId;
    tmdbId = other.tmdbId;
    year = other.year;
    season = other.season ;
    episode = other.episode;
    parentImdbId = other.parentImdbId;
    parentTmdbId = other.parentTmdbId;
    type = other.type;
    title = other.title;
    parentTitle = other.parentTitle;
    subtitles = other.subtitles;
    return *this;
}

QString Feature::titleDisplay() const {
    if (type == u"Movie"_qs) {
        return title;
    } else if (type == u"Episode"_qs) {
        QString s = QString("%1").arg(season, 2, 10, QLatin1Char('0'));
        QString e = QString("%1").arg(episode, 2, 10, QLatin1Char('0'));
        return parentTitle + " S" + s + "E" + e + " " + title;
    } else {
        return type + "-" + title;
    }
}

QString Feature::imdbDisplay() const {
    if (type == u"Movie"_qs) {
        return "tt"+QString("%1").arg(imdbId, 7, 10, QLatin1Char('0'));
    } else if (type == u"Episode"_qs) {
        return "tt"+QString("%1").arg(parentImdbId, 7, 10, QLatin1Char('0'));
    } else {
        return "Unknown";
    }
}

void Feature::clear() {
    imdbId = 0;
    tmdbId = 0;
    year = 0;
    season = 0;
    episode = 0;
    parentImdbId = 0;
    parentTmdbId = 0;
    type.clear();
    title.clear();
    parentTitle.clear();
    subtitles.clear();
}

QDebug operator <<(QDebug dbg, const Feature& feature) {
    if (feature.type.isEmpty()) {
        dbg << "No feature";
    } else if (feature.type ==u"Movie"_qs) {
        dbg.nospace().noquote() << "Movie:(\"" << feature.title << "\" <" << feature.subtitles.size() << ">)";
    } else if (feature.type == u"Episode"_qs) {
        dbg.nospace().noquote() << "Episode:(\"" << feature.parentTitle << " S" << feature.season << "E" << feature.episode << " " << feature.title << "\" <" << feature.subtitles.size() << ">)";
    } else {
        dbg.nospace().noquote() << "Unknown:(\"" << feature.type << "-" << feature.title << "\" <" << feature.subtitles.size() << ">)";
    }
    return dbg;
}

Feature merge(const Feature& a, const Feature& b) {
    Feature retval(a);
    retval.subtitles.append(b.subtitles);
    return retval;
}

QList<Feature> parse(const QJsonArray& data) {
    QHash<quint64, Feature> features;

    for (int i = 0; i < data.size(); ++i) {
        Feature feature(data[i].toObject());
        if (features.contains(feature.imdbId)) {
            Feature existing = features[feature.imdbId];
            features.insert(feature.imdbId, merge(feature, existing));
        } else {
            features.insert(feature.imdbId, feature);
        }
    }

    return features.values();
}
