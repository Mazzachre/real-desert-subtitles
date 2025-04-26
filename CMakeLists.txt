cmake_minimum_required(VERSION 3.16)

project(real-desert-subtitles VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Qml Quick DBus)
qt_standard_project_setup()

qt_add_executable(real-desert-subtitles
    src/main.cpp
    src/app/application.cpp
    src/dbus/subtitles-adapter.cpp
    src/lib/authentication.cpp
    src/lib/file-hash.cpp
    src/lib/file-saver.cpp
    src/lib/subtitle-finder.cpp
    src/lib/subtitle-downloader.cpp
    src/types/subtitle-result.cpp
    src/ui/ui.cpp
    src/ui/config.cpp
    src/ui/drop-target.cpp
    src/ui/feature-model.cpp
    src/ui/subtitle-model.cpp
    src/ui/selected-feature.cpp
)

qt_add_qml_module(real-desert-subtitles
    URI res
    VERSION 1.0
    QML_FILES
        qml/main.qml
    RESOURCES
        images/desert-logo.svg
        images/foreign.svg
        images/hash.svg
        images/hearing-impaired.svg
        images/comment.svg
        images/config.svg
        images/user.svg
)

target_link_libraries(real-desert-subtitles
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::Qml
    Qt6::Quick
    Qt6::DBus
)

include(InstallRequiredSystemLibraries)

# Install rules
install(TARGETS real-desert-subtitles
    DESTINATION bin
)

include(GNUInstallDirs)

install(FILES resources/real-desert-subtitles.desktop
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
)

install(FILES images/desert-logo.svg
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps
    RENAME real-desert-subtitles.svg
)

# Package metadata
set(CPACK_PACKAGE_NAME "real-desert-subtitles")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "Tanya Randstoft <tanya@randstoft.dk>")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Tanya Randstoft")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_DEPENDS
    "qml6-module-qtqml, qml6-module-qtqml-workerscript, qml6-module-qtquick, qml6-module-qtquick-controls,
     libqt6core6t64 (>= 6.4), libqt6gui6t64 (>= 6.4), libqt6widgets6t64 (>= 6.4),
     libqt6network6t64 (>= 6.4), libqt6qml6 (>= 6.4), libqt6quick6 (>= 6.4), libqt6dbus6 (>= 6.4), qml6-module-qtquick-templates (>= 6.4), qml6-module-qtquick-window (>= 6.4), libqt6svg6 (>= 6.4)"
)
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_SYSTEM_NAME "amd64")
set(CPACK_GENERATOR "DEB")

include(CPack)
