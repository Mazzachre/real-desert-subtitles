#include "selected-feature.h"

#include <QDebug>

Rd::Application::SelectedFeature::SelectedFeature(QObject* parent)
: QObject(parent) {
}

bool Rd::Application::SelectedFeature::result() {
    return !m_file.isEmpty();
}

QString Rd::Application::SelectedFeature::filename() {
    return m_file.fileName();
}

QString Rd::Application::SelectedFeature::title() {
    return m_feature.display();
}

QString Rd::Application::SelectedFeature::year() {
    return QString::number(m_feature.year);
}

QString Rd::Application::SelectedFeature::imdb() {
    return m_feature.imdbDisplay();
}

void Rd::Application::SelectedFeature::setSelected(const QUrl& file, const Feature& feature) {
    m_file = file;
    m_feature = feature;

    qDebug() << "Selected feature:" << file << feature << Qt::endl;

    Q_EMIT updated();
}

void Rd::Application::SelectedFeature::clear() {
    m_file.clear();
    m_feature.clear();
    Q_EMIT updated();
}
