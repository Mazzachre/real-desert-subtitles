#include "application.h"
// #include "../types/subtitle-result.h"
// #include "../types/ui-mode.h"

Rd::Application::Application::Application(const QRect& dimensions, QObject* parent)
: QObject(parent)
, m_ui{new Rd::Ui::Ui} {
    m_dimensions = dimensions;

    connect(m_ui, &Rd::Ui::Ui::fileIdentified, this, &Application::whatever);
}

Rd::Application::Application::~Application() noexcept {
    delete m_ui;
}

void  Rd::Application::Application::start() {
    m_ui->show(m_dimensions);
}

void Rd::Application::Application::whatever(const QUrl& file, const Feature& feature) {
    qDebug() << "Identified" << file << feature << Qt::endl;
}
