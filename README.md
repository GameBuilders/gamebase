# Setup

## All Platforms
Download one of the waf binaries from https://code.google.com/p/waf/downloads/list and rename it to waf inside the folder.

## Windows
The framework requires you download and extract a MinGW package into the folder so that setenv.cmd works properly. I recommend http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.8.2/threads-posix/dwarf/i686-4.8.2-release-posix-dwarf-rt_v3-rev2.7z/download  Then you double click setenv.cmd. The exception to this is if you already have a gcc 4.8+ and Python 2.7 (not 3) installed and accessible from your PATH. In that case, you can skip the previous.

## Linux
pkg-config, XRandR, X11, OpenGL, OpenAL, libjpeg, and libsndfile must be installed. This is in addition to Python 2.7 (not 3) and gcc 4.8+ in order to compile. Use your preferred package manager to get these.

## Apple OS X
For OS X, good luck. I'd love your feedback, but I have no OS X machine to test this. I expect it to be similar to the Linux setup.

# Compile
Simply run this in a console with the game framework folder as the current working directory:
Code:
python waf configure
python waf get_deps
python waf build

The first command checks your system for the required libraries and compilers.
The second command downloads and extracts various libraries for static linking (e.g. SFML, freetype, etc...).
The last command needs to be redone in order to compile any changes to the project.
In case you get some Python exceptions, make sure you are using Python 2. Some distros call the binary python2, which you should replace the above commands with.
