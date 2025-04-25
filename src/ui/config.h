#pragma once

#include <QObject>

namespace Rd {
    namespace Ui {
        class Config : public QObject {
            Q_OBJECT
            Q_PROPERTY(QString languages READ languages NOTIFY updated)
            Q_PROPERTY(bool fpo READ fpo NOTIFY updated)
            Q_PROPERTY(QString username READ username NOTIFY updated)
            Q_PROPERTY(QString password READ password NOTIFY updated)
        public:
            explicit Config(QObject* parent = nullptr);

            QString languages();
            bool fpo();
            QString username();
            QString password();

            Q_INVOKABLE void saveConfig(const QString& languages, bool fpo);
            Q_INVOKABLE void saveAuth(const QString& username, const QString& password);

            Q_SIGNAL void updated();
        };
    }
}
