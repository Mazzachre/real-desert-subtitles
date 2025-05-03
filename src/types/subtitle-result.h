#pragma once

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QDebug>

struct Subtitle {
    quint64 fileId; //files[0].file_id
    bool hearingImpaired; //hearing_impaired
    double fps; //fps
    QString fileName; //files[0].file_name
    bool hashMatch; //hash_match
    bool fpo; //foreign_parts_only
    QString comments; //comments

    Subtitle();
    Subtitle(const Subtitle& other);
    Subtitle(bool hearingImpaired, double fps, bool hashMatch, bool fpo, const QString& comments, const QJsonObject& item);
    Subtitle& operator=(const Subtitle& other);

    void clear();
};

struct {
    bool operator()(const Subtitle& a, const Subtitle& b) {
        return a.fileName < b.fileName;
    }
} subtitleSort;

QDebug operator <<(QDebug dbg, const Subtitle& subtitle);

struct Feature {
    QString type; //feature.feature_type
    quint64 imdbId; //feature.imdb_id
    quint64 tmdbId; //feature.tmdb_id
    QString title; //feature.title
    quint16 year; //feature.year
    quint16 season; //feature.season_number
    quint16 episode; //feature.episode_number
    quint64 parentImdbId; //feature.parent_imdb_id
    quint64 parentTmdbId; //feature.parent_tmdb_id
    QString parentTitle; //feature.parent_title

    QList<Subtitle> subtitles;

    Feature();
    explicit Feature(const QJsonObject& item);
    Feature(const Feature& other);
    Feature& operator=(const Feature& other);

    QString titleDisplay() const;
    QString imdbDisplay() const;

    void clear();
};

struct {
    bool operator()(const Feature& a, const Feature& b) const {
        if (a.type == b.type) {
            if (a.parentImdbId != 0) {
                if (a.parentTitle == b.parentTitle) {
                    if (a.season == b.season) {
                        return a.episode < b.episode;
                    }
                    return a.season < b.season;
                }
                return a.parentTitle < b.parentTitle;
            }
            return a.title < b.title;
        }
        return a.type < b.type;
    }
} featureSort;

QDebug operator <<(QDebug dbg, const Feature& feature);

Feature merge(const Feature& a, const Feature& b);

QList<Feature> parse(const QJsonArray& data);
