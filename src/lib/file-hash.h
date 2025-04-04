#pragma once

#include <QObject>

namespace Rd {
    namespace Library {
        class FileHash : public QObject {
            Q_OBJECT
        public:
            FileHash(QObject *parent = nullptr);

            QString computeHash(const QUrl& file) const;

            Q_SIGNAL void hashError(const QString& error) const;
        };
    }
}
