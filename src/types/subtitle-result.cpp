#include "subtitle-result.h"
#include <QJsonArray>

SubtitleFile::SubtitleFile(const QJsonObject& value) {
    fileId = value.value(u"file_id"_qs).toInt();
    cd = value.value(u"cd_number"_qs).toInt();
    fileName = value.value(u"file_name"_qs).toString();
}

SubtitleResult::SubtitleResult(const QJsonObject& value) {
    QJsonObject attributes = value.value(u"attributes"_qs).toObject();
    QJsonObject feature = attributes.value(u"feature_details"_qs).toObject();

    type = feature.value(u"feature_type"_qs).toString();
    imdbId = feature.value(u"imdb_id"_qs).toInt();
    tmdbId = feature.value(u"tmdb_id"_qs).toInt();
    title = feature.value(u"title"_qs).toString();
    year = feature.value(u"year"_qs).toInt();

    QJsonArray fileArray = attributes.value(u"files"_qs).toArray();
    for (int i = 0 ; i < fileArray.size() ; ++i) {
        files << SubtitleFile(fileArray[i].toObject());
    }
}

QDebug operator <<(QDebug dbg, const SubtitleFile& subtitleFile) {
    dbg.nospace() << subtitleFile.fileId << "(" << subtitleFile.cd << ") " << subtitleFile.fileName << " ";
    return dbg;
}

QDebug operator<<(QDebug dbg, const SubtitleResult& subtitleResult) {
    dbg.nospace() << subtitleResult.imdbId << " " << subtitleResult.title << " (" << subtitleResult.year << ") files[" << subtitleResult.files << "]";
    return dbg;
}
