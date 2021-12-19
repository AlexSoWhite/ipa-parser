# ipa-parser
This is a tool for iOS apps (ipa) static security testing.
## Installation
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
## Usage
To use this instrument you have to get .ipa app archive via ```frida-ios-dump```.
```
$ ./parser [path-to-ipa]
```
