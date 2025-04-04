#include <QApplication>
#include "app/application.h"
#include <QString>
#include <QCommandLineParser>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral(u"Real Desert Subtitles"));
    app.setApplicationVersion(QStringLiteral(u"0.1"));
    app.setOrganizationDomain(QStringLiteral(u"realdesert.com"));
    app.setOrganizationName(QStringLiteral(u"Real Desert Productions"));

//    QCommandLineParser parser;
    //2 modes as I see it?
    //DBus mode (start hidden, open window on dbus execusion then hide when clicked OK)
    //UI mode (start open, don't listen on dbus, default)

    Rd::Application::Application application;
    application.start(app.primaryScreen()->geometry());

    return app.exec();
}
