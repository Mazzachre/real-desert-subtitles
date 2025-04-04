#pragma once

#include <QString>
#include <QObject>
#include <QUrl>
#include "../lib/file-hash.h"

namespace Rd {
    namespace Application {
        class FileSearch : public QObject {
            Q_OBJECT
            Q_PROPERTY(bool working READ working NOTIFY workingUpdated)
            Q_PROPERTY(QString file READ file NOTIFY workingUpdated)
        public:
            FileSearch(QObject *parent = nullptr);
            ~FileSearch();

            bool working() const;
            QString file() const;
            Q_SIGNAL void workingUpdated();

            Q_SLOT void find(const QUrl& file);
        private:
            Rd::Library::FileHash *m_hash;
            QUrl m_file;
            bool m_working;
        };
    }
}
