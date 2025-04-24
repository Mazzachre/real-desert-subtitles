#include "application.h"
#include <QDBusConnection>

Rd::Application::Application::Application(const QRect& dimensions, bool dbusMode, QObject* parent)
: QObject(parent)
, m_ui{new Rd::Ui::Ui} {
    m_dbusMode = dbusMode;
    m_dimensions = dimensions;
    m_dbus = new SubtitlesAdaptor(this);

    QDBusConnection::sessionBus().registerObject("/Subtitles", this);
    QDBusConnection::sessionBus().registerService("com.realdesert.Subtitles");

    connect(m_ui, &Rd::Ui::Ui::fileIdentified, m_dbus, &SubtitlesAdaptor::fileIdentified);
    connect(m_ui, &Rd::Ui::Ui::finished, this, &Application::finished);
}

Rd::Application::Application::~Application() noexcept {
    delete m_ui;
}

void  Rd::Application::Application::start() {
    if (!m_dbusMode) {
        m_ui->show(m_dimensions);
    }
}

void Rd::Application::Application::findFile(const QUrl& file) {
    m_ui->show(m_dimensions);
    m_ui->fileSelected(file);
}

void Rd::Application::Application::finished() {
    m_ui->clear();
    if (m_dbusMode) {
        m_ui->hide();
    }
}
