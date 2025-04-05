#pragma once

#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "drop-target.h"
#include "file-search.h"
#include "feature-model.h"

namespace Rd {
    namespace Application {
        class Application : public QObject {
            Q_OBJECT
        public:
            explicit Application(QObject *parent = nullptr);
            ~Application();

            void start(const QRect& dimensions);
        private:
            QQmlApplicationEngine *m_engine;
            QQuickWindow *m_window;
            DropTarget *m_dropTarget;
            FileSearch *m_fileSearch;
            FeatureModel *m_features;

            Q_SLOT void handleError(const QString& error);
        };
    }
}
