# Real Desert Subtitles

Subtitle downloader for Real Desert Media center.

## Build

Application uses CMake to build and requires QT6 Core Widgets Network Qml Quick DBus dev libraries to build.

### API-KEY

Application uses opensubtitles.com and requires an API key to work properly.

The API key be acquired from https://www.opensubtitles.com/en/consumers

create a file in the root folder called "key.h" with the following line

#define API_KEY "api-key-goes-here"

## Run

The application can be started as is and starts in stand-alone mode. A file can be dragged in to be analyzed. If it is matched a list of downloadable subtitles is presented and will be downloaded to the directory of the film or episode.

### Login

opensubtitles.com expects users to login to get the full number of downloads pr. day available.

### DBus

The application will send an event on com.realdesert.Media /Subtitles/Identified with the path to the file along with info of the file.

If the application is started in DBus mode with argument -d or --dbus it will start hidden and only show itself once the dbus method com.realdesert.Media /Subtitles/FindFor is called with a file path as argument. This functions just like having dragged the file into the window in UI mode.

## Future development

The UI in dbus mode should react slightly differently. It should no longer clear search results and it should no longer exit the application on close but instead hide the window.
