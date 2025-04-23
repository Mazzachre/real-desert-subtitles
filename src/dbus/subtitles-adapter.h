#pragma once

#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QUrl>
#include "../types/subtitle-result.h"

/*
 * Adaptor class for interface com.realdesert.Subtitles
 */
class SubtitlesAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.realdesert.Subtitles")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.realdesert.Subtitles\">\n"
"    <method name=\"FindFor\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"url\"/>\n"
"    </method>\n"
"    <signal name=\"Identified\">\n"
"      <arg type=\"s\" name=\"result\"/>\n"
"    </signal>\n"
"  </interface>\n"
        "")
public:
    SubtitlesAdaptor(QObject *parent);
    virtual ~SubtitlesAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void FindFor(const QString &url);
    void fileIdentified(const QUrl& file, const Feature& feature);
Q_SIGNALS: // SIGNALS
    void Identified(const QString &result);
};
