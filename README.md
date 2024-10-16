
# macOS build
```
brew install sdl2 --build-from-source
./buildLibs.sh
./buildMac.sh
```

# Linux build
```
sudo apt-get update
sudo apt-get install build-essential libsdl2-dev libxdamage-dev libxcomposite-dev libglew-dev
./buildLibs.sh
./buildLinux.sh
```

If you run into issues installing packages try:
```
sudo apt-get remove --purge libegl-mesa0
sudo apt-get clean
sudo apt-get autoremove
```