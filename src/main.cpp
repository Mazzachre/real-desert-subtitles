#include <QApplication>
#include "app/application.h"
#include <QString>
#include <QCommandLineParser>
#include "types/subtitle-result.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName(u"Real Desert Subtitles"_qs);
    app.setApplicationVersion(u"0.1"_qs);
    app.setOrganizationDomain(u"realdesert.com"_qs);
    app.setOrganizationName(u"Real Desert Productions"_qs);

//    QCommandLineParser parser;
    //2 modes as I see it?
    //DBus mode (start hidden, open window on dbus execusion then hide when clicked OK)
    //UI mode (start open, don't listen on dbus, default)

    Rd::Application::Application application;

    qRegisterMetaType<SubtitleFile>();
    qRegisterMetaType<SubtitleResult>();

    application.start(app.primaryScreen()->geometry());

    return app.exec();
}
