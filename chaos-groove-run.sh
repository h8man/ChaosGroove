#!/bin/bash
# ====================================================
# Bash script to run Raylib game with ./lib dynamic libs
# Sets working directory and library path automatically
# ====================================================

# Project root (adjust if script is in another folder)
PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"

# Game executable
GAME="$PROJECT_ROOT/bin/game"

# Directory containing dynamic libraries (Raylib, etc.)
LIB_DIR="$PROJECT_ROOT/linux/raylib/lib"

# Set library path for dynamic loader
export LD_LIBRARY_PATH="$LIB_DIR:$LD_LIBRARY_PATH"

# Optional: set working directory to project root
cd "$PROJECT_ROOT" || exit 1

# Run the game
"$GAME"