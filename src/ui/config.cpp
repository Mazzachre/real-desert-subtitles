#include "config.h"

#include <QSettings>

Rd::Ui::Config::Config(QObject* parent)
: QObject(parent) {
    Q_EMIT updated();
}

bool Rd::Ui::Config::fpo() {
    QSettings settings;
    return settings.value(u"fpo"_qs, false).toBool();
}

QString Rd::Ui::Config::languages() {
    QSettings settings;
    return settings.value(u"languages"_qs, "en").toString();
}

QString Rd::Ui::Config::username() {
    QSettings settings;
    return settings.value(u"username"_qs, "").toString();
}

QString Rd::Ui::Config::password() {
    QSettings settings;
    return settings.value(u"password"_qs, "").toString();
}

void Rd::Ui::Config::saveConfig(const QString& languages, bool fpo) {
    QSettings settings;
    settings.setValue(u"languages"_qs, QVariant(languages));
    settings.setValue(u"fpo"_qs,  fpo);
}

void Rd::Ui::Config::saveAuth(const QString& username, const QString& password) {
    QSettings settings;
    settings.setValue(u"username"_qs, QVariant(username));
    settings.setValue(u"password"_qs, QVariant(password));
}
