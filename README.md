# Cross-Platform Port Monitor

A lightweight, cross-platform desktop application built with C++ and the Qt Framework to monitor network ports and system traffic in real-time. This tool features a graphical user interface (GUI) and runs seamlessly on both Windows and Linux environments.

## Features
- **Real-Time Monitoring:** Track active network connections, open ports, and protocol statuses (TCP/UDP).
- **Cross-Platform Support:** Fully compatible with Windows and Linux (tested on Pop!_OS / Linux Mint).
- **Intuitive GUI:** Built with Qt for a clean, responsive, and user-friendly interface.
- **Asynchronous Architecture:** Heavy network polling tasks are handled efficiently using Qt's Signal and Slot mechanism to ensure the UI never freezes.

## Tech Stack
- **Language:** C++ (C++17 or higher recommended)
- **Framework:** Qt (Widgets / Core / Network)
- **Build System:** CMake / qmake
- **Version Control:** Git

## Prerequisites
Before building the project, ensure you have the following installed:
- A C++ compiler (GCC/Clang for Linux, MSVC/MinGW for Windows)
- Qt Creator and Qt SDK (v5.15 or v6.x)
- CMake (optional, depending on your build system)

## How to Build and Run

### Using Qt Creator (Recommended)
1. Open Qt Creator.
2. Select **Open File or Project** and choose the `CMakeLists.txt` file.
3. Configure the project with your kit (Desktop Qt).
4. Click the **Run** button (or press `Ctrl + R`).

### Using Command Line (Linux)
```bash
# Clone the repository
git clone [https://github.com/mascqtdev/QtPortMonitoring.git](https://github.com/mascqtdev/QtPortMonitoring.git)
cd port-monitor

# Create a build directory
mkdir build && cd build

# Run CMake and Build
cmake ..
make

# Run the application
./PortMonitor
