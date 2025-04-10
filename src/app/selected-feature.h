#pragma once

#include <QObject>
#include <QUrl>
#include "../types/subtitle-result.h"

namespace Rd {
    namespace Application {
        class SelectedFeature : public QObject {
            Q_OBJECT
            Q_PROPERTY(bool result READ result NOTIFY updated)
            Q_PROPERTY(QString filename READ filename NOTIFY updated)
            Q_PROPERTY(QString title READ title NOTIFY updated)
            Q_PROPERTY(QString year READ year NOTIFY updated)
            Q_PROPERTY(QString imdb READ imdb NOTIFY updated)
        public:
            SelectedFeature(QObject* parent = nullptr);

            bool result();
            QString filename();
            QString title();
            QString year();
            QString imdb();

            Q_SIGNAL void updated();

            Q_SLOT void setSelected(const QUrl& file, const Feature& feature);
            Q_SLOT void clear();
        private:
            QUrl m_file;
            Feature m_feature;
        };
    }
}
