#pragma once

#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "feature-model.h"
#include "subtitle-model.h"
#include "drop-target.h"
#include "selected-feature.h"
#include "../lib/subtitle-finder.h"
#include "../lib/subtitle-downloader.h"

namespace Rd {
    namespace Ui {
        class Ui : public QObject {
            Q_OBJECT
            Q_PROPERTY(Mode mode READ mode NOTIFY modeUpdated)
            Q_PROPERTY(QString file READ file NOTIFY fileUpdated)
        public:
            //we need different states... I need to draw a diagram!
            enum Mode {
                Search,
                Working,
                Retry,
                Selecting,
                Selected
            };
            Q_ENUM(Mode)

            explicit Ui(QObject* parent = nullptr);
            ~Ui() noexcept;

            Mode mode();
            Q_SIGNAL void modeUpdated();

            QString file();
            Q_SIGNAL void fileUpdated();

            Q_INVOKABLE void searchMovie(const QString& title, const QString& year);
            Q_INVOKABLE void searchShow(const QString& title, const QString& season, const QString& episode);

            Q_SLOT void selectFeature(quint64 id);
            Q_SLOT void selectSubtitle(quint64 id);
            Q_SLOT void clear();

            //Main app control
            Q_SIGNAL void fileIdentified(const QUrl& file, const Feature& feature);
            Q_SLOT void show(const QRect& dimensions);
            Q_SLOT void hide();
        private:
            Mode m_mode = Mode::Search;
            QUrl m_file;

            //System
            QQmlApplicationEngine* m_engine;
            QQuickWindow* m_window;

            //UI modules
            FeatureModel* m_features;
            SubtitleModel* m_subtitles;
            DropTarget* m_target;
            SelectedFeature* m_selected;

            //Library functions
            Rd::Library::SubtitleFinder* m_search;
            Rd::Library::SubtitleDownloader* m_download;

            Q_SLOT void fileSelected(const QUrl& file);
            Q_SLOT void subtitlesFound(const QList<Feature>& results);
            Q_SLOT void noSubtitlesFound();

            Q_SLOT void handleError(const QString& head, const QString& body);
        };
    }
}
