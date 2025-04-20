#pragma once

#include <QObject>
#include <QApplication>
#include "../ui/ui.h"
#include "../lib/subtitle-finder.h"

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

            Q_SLOT void whatever(const QUrl& file, const Feature& feature);
        };
    }
}
