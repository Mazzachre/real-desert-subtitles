#pragma once

#include <QObject>
#include <QNetworkAccessManager>

namespace Rd {
    namespace Library {
        class SubtitleFinder : public QObject {
            Q_OBJECT
        public:
            SubtitleFinder(QObject *parent = nullptr);
            ~SubtitleFinder();

            Q_SIGNAL void hashError(const QString& error) const;
        private:
            QNetworkAccessManager *m_networkAccess;
        };
    }
}
