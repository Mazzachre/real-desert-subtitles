#pragma once

#include <QObject>

namespace Rd {
    namespace Library {
        class FileHash : public QObject {
            Q_OBJECT
        public:
            explicit FileHash(QObject *parent = nullptr);
            QString computeHash(const QUrl& file) const;
            Q_SIGNAL void error(const QString& error, const QString& body) const;
        };
    }
}
