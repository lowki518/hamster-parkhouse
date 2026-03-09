FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV DISPLAY=:1

# Alle Dependencies in einem Schritt installieren
RUN apt-get update && apt-get install -y \
    # Build Tools
    git cmake build-essential pkg-config \
    # SDL3 & X11 Dependencies
    libx11-dev libxext-dev libxrandr-dev libxinerama-dev \
    libxcursor-dev libxi-dev libxfixes-dev libxss-dev \
    libxtst-dev libwayland-dev libxkbcommon-dev \
    libdbus-1-dev libudev-dev libgl1-mesa-dev libasound2-dev \
    # Desktop & VNC
    xvfb x11vnc novnc websockify xfce4 xfce4-terminal \
    # Additional tools
    git curl wget nano htop \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /workspaces/Test
CMD ["/bin/bash"]