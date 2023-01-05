# Sniper

<img width="600" alt="sniper-game" src="https://user-images.githubusercontent.com/35516367/210126880-09b173bf-5f1d-47e2-8f3c-40c0589f72a9.png">

## Overview

Sniper is a very simple game where you run from an adversary which tries to
shoot you, but you can collect the bullets and fire them back to gain score.

## Build

### Dependencies

Make sure you have GLFW installed:

<details>
<summary>GLFW Install Instructions</summary>

#### Arch Linux

```bash
sudo pacman -S glfw-x11
```

#### Debian/Ubuntu

```bash
sudo apt install libglfw3 libglfw3-dev
```

#### Windows

Download a pre-compiled binary from [the GLFW website](https://www.glfw.org/download.html).

#### macOS

Install from Homebrew:

```zsh
brew install glfw
```

Or download a pre-compiled binary from [the GLFW website](https://www.glfw.org/download.html).

#### From source

See the instructions on [the GLFW website](https://www.glfw.org/docs/latest/compile.html).

</details>

Then check that OpenGL 4.1 or above is installed. You can download OpenGL from [here](https://www.khronos.org/opengl/wiki/Getting_Started#Downloading_OpenGL).

### Run build script

To build, run [`./scripts/build.sh`](scripts/build.sh) (or [`.\scripts\build.bat`](scripts/build.bat) on Windows).
