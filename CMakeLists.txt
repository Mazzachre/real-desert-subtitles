cmake_minimum_required(VERSION 3.16)

project(real-desert-subtitles VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Qml Quick)
find_package(QuaZip-Qt6 REQUIRED)
qt_standard_project_setup()

qt_add_executable(real-desert-subtitles
    src/main.cpp
    src/app/application.cpp
    src/lib/file-hash.cpp
    src/lib/file-saver.cpp
    src/lib/subtitle-finder.cpp
    src/lib/subtitle-downloader.cpp
    src/types/subtitle-result.cpp
    src/ui/ui.cpp
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
)

target_link_libraries(real-desert-subtitles
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::Qml
    Qt6::Quick
    QuaZip::QuaZip
)
