#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Rd {
    namespace Library {
        class Authentication : public QObject {
            Q_OBJECT
        public:
            Authentication(QObject* parent = nullptr);

            void authenticate();
            Q_SIGNAL void error(const QString& error, const QString& body) const;
            Q_SIGNAL void authenticated(const QString& token);
        private:
            QNetworkAccessManager* m_networkAccess;

            Q_SLOT void handleResponse(QNetworkReply *reply);
        };
    }
}
