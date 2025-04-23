#include <QApplication>
#include "app/application.h"
#include <QString>
#include <QCommandLineParser>
#include "types/subtitle-result.h"
#include "ui/ui.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName(u"Real Desert Subtitles"_qs);
    app.setApplicationVersion(u"0.1"_qs);
    app.setOrganizationDomain(u"realdesert.com"_qs);
    app.setOrganizationName(u"Real Desert Productions"_qs);

    QCommandLineParser parser;
    parser.addOptions({
        {{"d", "dbus"}, "Start in DBus mode"}
    });
    parser.parse(app.arguments());

    qRegisterMetaType<Subtitle>("Subtitle");
    qRegisterMetaType<Feature>("Feature");
    qmlRegisterUncreatableType<Rd::Ui::Ui>("com.realdesert", 1, 0, "Mode", "Not creatable as it is an enum type");

    Rd::Application::Application application(app.primaryScreen()->geometry());
    if (!parser.isSet("d")) {
        application.start();
    }

    return app.exec();
}
