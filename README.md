# ipa-parser
This is a tool for iOS apps (ipa) static security testing.
## Installation
1. Install decompiler https://github.com/AlexSoWhite/swift-decompiler
2. `cd` to project root directory 
3. `$ mkdir build`
4. `$ cd build`
5. `$ cmake ..`
6. `$ make`

## Usage
To use this instrument you have to get .ipa app archive via ```frida-ios-dump```.
```
$ ./parser [path-to-ipa]
```
