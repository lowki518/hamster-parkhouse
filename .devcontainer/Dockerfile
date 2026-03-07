# Base C++ container
FROM mcr.microsoft.com/devcontainers/cpp:ubuntu-22.04

# Install SDL + X11 + virtual desktop + VNC + window manager + pkg-config
RUN apt-get update && apt-get install -y \
    build-essential cmake ninja-build pkg-config \
    libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev libxkbcommon-dev libgl1-mesa-dev \
    xvfb x11vnc fluxbox novnc websockify \
 && rm -rf /var/lib/apt/lists/*

# Expose noVNC port
EXPOSE 6080

# Launch virtual display, window manager, VNC, and noVNC automatically
CMD Xvfb :1 -screen 0 1280x720x24 & \
    export DISPLAY=:1 && \
    fluxbox & \
    x11vnc -display :1 -nopw -forever -shared & \
    websockify --web=/usr/share/novnc/ 6080 localhost:5900 & \
    bash