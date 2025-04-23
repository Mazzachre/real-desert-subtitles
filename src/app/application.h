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
            explicit Application(const QRect& dimensions, QObject *parent = nullptr);
            ~Application() noexcept;

            void start();
        private:
            QRect m_dimensions;
            Rd::Ui::Ui* m_ui;
            SubtitlesAdaptor* m_dbus;

            Q_INVOKABLE void findFile(const QUrl& file);
        };
    }
}
