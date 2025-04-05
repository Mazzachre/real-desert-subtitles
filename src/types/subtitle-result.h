#pragma once

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QDebug>

struct SubtitleFile {
    quint32 fileId;
    quint8 cd;
    QString fileName;

    SubtitleFile(const QJsonObject& value);
};

QDebug operator <<(QDebug dbg, const SubtitleFile& subtitleFile);

struct SubtitleResult {
    QString type;
    quint32 imdbId;
    quint32 tmdbId;
    QString title;
    quint16 year;

    //fps?
    //fileName?

    //Why is this a list?
    QList<SubtitleFile> files;

    SubtitleResult(const QJsonObject& value);
};

QDebug operator<<(QDebug dbg, const SubtitleResult& subtitleResult);
