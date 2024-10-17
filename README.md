
# TurtleLab
TurtleLab is an audio-visual programming toy that can be scripted multiple languages. 

Supported Languages:
- Scheme
- JavaScript
- Lua

# CLI Usage
```
Usage: turtle <file> [options]
Options:
  -l, --lang <lang>     Specify the lang (scheme, js) - Overrides auto detection
  -d, --delay <delay>   Specify the delay between ticks
  -s, --skipIntro       Skip the intro
  -t, --hideTurtle      Hide the cursor
  -o, --noLoop          Do not loop the program
  -b, --noDebug         Do not show debug messages
  -v, --volume <volume> Set the volume
  -m, --mute            Mute the sound
  -h, --help            Display this help message
```

# Scripting
## JavaScript ([`examples/api.js`](examples/api.js))
```js
// This uses the DuktapeJS engine to run JavaScript code
// It only supports ES5, so no let, const, or arrow functions etc
function main() { 
    bg(22,33,44);
    color(255,255,255);
    teleport(128,64);
    down(64);
    right(64);
    up(64);
    left(64);
    downRight(64);
    downLeft(64);
    upLeft(64);
    upRight(64);
    back(7);
    downLeft(32);
    print("Hello, world!");
}
```

## Scheme ([`examples/api.scm`](examples/api.scm))
```c
(bg 22 33 44)
(color 255 255 255)
(teleport 128 64)
(down 64)
(right 64)
(up 64)
(left 64)
(down-right 64)
(down-left 64)
(up-left 64)
(up-right 64)
(back 7)
(down-left 32)
(print "Hello, world!")
```

## Lua ([`examples/api.lua`](examples/api.lua))
```lua
bg(22, 33, 44)
color(255, 255, 255)
teleport(128, 64)
down(64)
right(64)
up(64)
left(64)
downRight(64)
downLeft(64)
upLeft(64)
upRight(64)
back(7)
downLeft(32)
print("Hello, world!")
```

# Build
Install the deps listed below and then run the Makefile or build with the shell scripts.
## macOS build
Deps;
```sh
brew install sdl2 --build-from-source
```
Build scripts:
```sh
./buildLibs.sh
./buildMac.sh
```

Makefile:
```sh
make turtle_mac
```

## Linux build
Deps:
```sh
sudo apt-get update
sudo apt-get install build-essential libsdl2-dev libxdamage-dev libxcomposite-dev libglew-dev
```

If you run into issues installing packages try:
```sh
sudo apt-get remove --purge libegl-mesa0
sudo apt-get clean
sudo apt-get autoremove
```

Build Scripts:
```sh
./buildLibs.sh
./buildLinux.sh
```

Makefile:
```
make
```