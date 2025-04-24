#pragma once

#include <QObject>
#include <QApplication>
#include "../ui/ui.h"
#include "../types/subtitle-result.h"
#include "../dbus/subtitles-adapter.h"

namespace Rd {
    namespace Application {
        class Application : public QObject {
            Q_OBJECT
        public:
            explicit Application(const QRect& dimensions, bool dbusMode, QObject *parent = nullptr);
            ~Application() noexcept;

            void start();
        private:
            QRect m_dimensions;
            Rd::Ui::Ui* m_ui;
            SubtitlesAdaptor* m_dbus;
            bool m_dbusMode;

            Q_INVOKABLE void findFile(const QUrl& file);
            Q_SLOT void finished();
        };
    }
}
