#pragma once

#include <QList>
#include <QUrl>
#include <QObject>
#include <QMimeDatabase>

namespace Rd {
    namespace Application {
        class DropTarget : public QObject {
            Q_OBJECT
        public:
            explicit DropTarget(QObject *parent = nullptr);
            ~DropTarget();

            Q_SIGNAL void fileDropped(const QUrl& path) const;

            Q_INVOKABLE bool acceptEntered(const QList<QUrl>& urls) const;
            Q_INVOKABLE void handle(const QList<QUrl>& urls) const;
        private:
            QMimeDatabase *m_db;
        };
    }
}
