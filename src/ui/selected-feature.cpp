#include "selected-feature.h"

#include <QDebug>

Rd::Ui::SelectedFeature::SelectedFeature(QObject* parent)
: QObject(parent) {
}

QString Rd::Ui::SelectedFeature::title() {
    return m_feature.titleDisplay();
}

QString Rd::Ui::SelectedFeature::year() {
    return QString::number(m_feature.year);
}

QString Rd::Ui::SelectedFeature::imdb() {
    return m_feature.imdbDisplay();
}

void Rd::Ui::SelectedFeature::setSelected(const Feature& feature) {
    m_feature = feature;
    Q_EMIT updated();
}

void Rd::Ui::SelectedFeature::clear() {
    m_feature.clear();
    Q_EMIT updated();
}
