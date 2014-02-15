# gamebase

## Introduction
This project is mainly the wscript file which downloads, extracts, and compiles some common C/C++ libraries for building games. It will also build your game and link it with those libraries.

### But Why??
gamebase serves a few goals for GameBuilders. The most pressing was the need for a common build system a C++ game-build-a-thon. Unlike, say, Lua+LÖVE or Python+pygame, we could not just double click user submitted C++ games and expect them to run. By having a common build script that everybody must use, we can ensure most submissions would compile. We chose that accepting binaries as submissions was unacceptable because of DLL hell, OS lock-in, and other dependency issues.

This project serves another purpose. That purpose is to allow people to quickly get up and running with game development without having to fight the compiler or the libraries. This often happens with new C++ programmers that had just taken a class on it. They know how to program, but they never really had to deal with the compiler or downloading and building external libraries. It's a valuable lesson to learn, but not when just starting out and definitely not as the first 12 hours of a 24 hour game development competition. 

### What's in the Box
* [waf](http://docs.waf.googlecode.com/git/book_16/single.html)
* [Box2D](http://www.box2d.org/manual.html)
* [SFML](http://www.sfml-dev.org/documentation/2.1/)
* [SFGUI](http://sfgui.sfml-dev.de/p/docs/guide/hello_world)
* [GLEW](http://glew.sourceforge.net/basic.html)
* [pugixml](http://pugixml.googlecode.com/svn/tags/latest/docs/quickstart.html)
* [JsonCpp](http://jsoncpp.sourceforge.net/)
* [FreeType 2](http://www.freetype.org/freetype2/docs/tutorial/step1.html)

In addition to these libraries, there is an example game using Box2D and SFML. It is located in [example/main.cpp](example/main.cpp).

## Setup

### Windows
If you already have gcc 4.8+ and Python 2.7 (not 3) installed and accessible from your PATH you are ready to go and can skip setup.
Otherwise, gamebase requires you download and extract a MinGW package into the folder so that setenv.cmd works properly. I recommend [this one](http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.8.2/threads-posix/dwarf/i686-4.8.2-release-posix-dwarf-rt_v3-rev2.7z/download). You'll need to extract it with [7-Zip](http://www.7-zip.org/). After that is done, double click setenv.cmd to open up a command prompt with the PATH set properly to use the compiler. You'll need to use setenv.cmd for every command prompt you wish to open for game development.

### Linux
pkg-config, XRandR, X11, OpenGL, OpenAL, libjpeg, and libsndfile must be installed. This is in addition to Python 2.7 (not 3) and gcc 4.8+ in order to compile. Use your preferred package manager to get these.

### OS X
XCode and the Command Line tools must be installed. Your XCode must be recent enough to support C++11.

## Compile
Simply run this in a console with the game framework folder as the current working directory:

```bash
python waf configure
python waf get_deps
python waf build
```

The first command checks your system for the required libraries and compilers.
The second command downloads and extracts various libraries for static linking (e.g. SFML, freetype, etc...).
The last command needs to be redone in order to compile any changes to the project.
In case you get some Python exceptions, make sure you are using Python 2. Some distros call the binary python2, which you should replace the above commands with.
