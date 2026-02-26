# Qt Client (C++ / Qt Widgets)

This application is a simple CRUD client for the `car` table via the REST API.

## Requirements
- Qt 6 (Widgets + Network)

## Build

Open `qt-client/` in Qt Creator and build the CMake project.

## Configuration

The API base URL is defined in `src/mainwindow.h`:

- `http://localhost:3000/api`

Change it if the REST API runs on another host/port.

