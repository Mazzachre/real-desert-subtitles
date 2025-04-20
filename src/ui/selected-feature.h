#pragma once

#include <QObject>
#include <QUrl>
#include "../types/subtitle-result.h"

namespace Rd {
    namespace Ui {
        class SelectedFeature : public QObject {
            Q_OBJECT
            Q_PROPERTY(QString title READ title NOTIFY updated)
            Q_PROPERTY(QString year READ year NOTIFY updated)
            Q_PROPERTY(QString imdb READ imdb NOTIFY updated)
        public:
            SelectedFeature(QObject* parent = nullptr);

            QString title();
            QString year();
            QString imdb();
            Q_SIGNAL void updated();

            Q_SLOT void setSelected(const Feature& feature);
            Q_SLOT void clear();
        private:
            Feature m_feature = Feature();
        };
    }
}
