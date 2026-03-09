#!/bin/bash

set -e

# ===== Set German Keyboard Layout =====
echo "Configuring German keyboard layout..."
export DEBIAN_FRONTEND=noninteractive
echo 'keyboard-configuration keyboard-configuration/layout select German' | debconf-set-selections
echo 'keyboard-configuration keyboard-configuration/variant select German' | debconf-set-selections
dpkg-reconfigure --frontend noninteractive keyboard-configuration > /dev/null 2>&1 || true
echo "✓ German keyboard configured"
echo ""

# Cleanup function called on EXIT
cleanup() {
    echo ""
    echo "╔════════════════════════════════════════════╗"
    echo "║   Shutting down...                         ║"
    echo "╚════════════════════════════════════════════╝"
    echo ""

    echo "Stopping XFCE..."
    pkill -f startxfce4 || true
    sleep 1

    echo "Stopping websockify..."
    pkill -f websockify || true
    sleep 1

    echo "Stopping x11vnc..."
    pkill x11vnc || true
    sleep 1

    echo "Stopping Xvfb..."
    pkill Xvfb || true
    sleep 1

    echo ""
    echo "✓ All services stopped"
    echo ""
}

# Register cleanup on EXIT (triggered by Ctrl+C or exit)
trap cleanup EXIT

echo "╔════════════════════════════════════════════╗"
echo "║   SDL3 Codespaces Setup                    ║"
echo "╚════════════════════════════════════════════╝"
echo ""

# ===== STEP 1: Dependencies =====
echo "[1/4] Installing dependencies..."
echo "(This may take up to 5 minutes...)"
echo ""

apt-get update -qq
apt-get install -y -qq \
  xvfb x11vnc novnc websockify \
  xfce4 xfce4-terminal \
  cmake build-essential git \
  libx11-dev libxext-dev libxrandr-dev libxinerama-dev \
  libxcursor-dev libxi-dev libxfixes-dev libxss-dev \
  libxtst-dev libwayland-dev libxkbcommon-dev \
  libdbus-1-dev libudev-dev libgl1-mesa-dev libasound2-dev

echo "✓ Dependencies installed"
echo ""

# ===== Step 2 : Submodules =====
echo "[2/4] Initializing SDL3 submodule..."

cd /workspaces/Test

# Check if .git directory exists
if [ ! -d ".git" ]; then
    echo "✗ Not a git repository"
    exit 1
fi

git submodule update --init --recursive

if [ -f "external/SDL/CMakeLists.txt" ]; then
    echo "✓ SDL3 submodule initialized"
else
    echo "✗ SDL3 CMakeLists.txt still not found"
    exit 1
fi

if [ -f "external/SDL_ttf/CMakeLists.txt" ]; then
    echo "✓ SDL3_ttf submodule initialized"
else
    echo "✗ SDL3_ttf CMakeLists.txt still not found"
    exit 1
fi
echo ""

# ===== Step 3: Auto Build App =====
echo "[3/4] Building SDL3 application..."

rm -rf build
mkdir -p build
cd build

echo "  → Running CMake..."
cmake ..

echo "  → Building..."
make

if [ ! -f "app" ]; then
    echo "✗ Build failed - executable not found"
else
    echo "✓ Build successful"
fi
echo ""

# ===== Step 4: Start Desktop =====
echo "[4/4] Setting up desktop..."

mkdir -p /root/Desktop

echo "Starting desktop services..."
echo ""

export DISPLAY=:1

# Xvfb
echo "  → Starting Xvfb..."
nohup Xvfb :1 -screen 0 1200x900x24 > /tmp/xvfb.log 2>&1 &
sleep 2

# x11vnc
echo "  → Starting x11vnc..."
nohup x11vnc -display :1 -nopw -forever -shared > /tmp/vnc.log 2>&1 &
sleep 1

# websockify
echo "  → Starting websockify..."
nohup websockify --web=/usr/share/novnc/ 6080 localhost:5900 > /tmp/novnc.log 2>&1 &
sleep 1

# XFCE
echo "  → Starting XFCE..."
nohup startxfce4 > /tmp/xfce4.log 2>&1 &
sleep 3

echo ""
echo "╔════════════════════════════════════════════╗"
echo "║   ✓ Setup Complete!                       ║"
echo "╚════════════════════════════════════════════╝"
echo ""
echo "🌐 Access your desktop:"
echo "   http://localhost:6080/vnc.html"
echo ""
echo "Press Ctrl+C or type 'exit' to stop and shut down services..."
echo ""

# Keep the script alive so desktop remains open
tail -f /dev/null