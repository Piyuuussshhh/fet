# fet - File Encryption Tool
This command-line tool encrypts a standard .txt file to a special .etxt file, that contains encrypted, jarbled text. It can only be decrypted with this tool.

To encrypt a text file: `./build/fet -e path/to/file/to/be/encrypted -k <key>`

To decrypt a .etxt file: `./build/fet -d path/to/file/to/be/decrypted -k <key>`

Upon decryption, a new text file with the same name before .etxt will be created, in the same directory as the .etxt, for viewing the contents.
Think of the key parameter as a password to the file.

Make sure to **use the same key** for encrypting and decrypting a file. If you forget it, the contents of the file are lost forever (lmao no well maybe).

Also make sure to run the program from the directory where the `.txt` and `.etxt` files are stored. The program is written in such a way that only the `.txt` and `.etxt` belonging to the same directory as the current working directory of the program will be considered safe to read from and write to.

Basically:
```
$ cd to/the/path/where/the/files/to/be/encrypted/decrypted/are/stored
$ fet this fet that etc
```

For this to actually work properly, you'd have to add `fet` to the `$PATH` environment variable. But for testing purposes, just create text files in the `fet` directory itself.

Works only on POSIX-compliant OSs (Linux distro + macOS). Sorry Windows, Win32 API is crazy time consuming to write.

## Build Process
1. Clone the project:

```
$ git clone https://github.com/Piyuuussshhh/fet.git
```

2. Build the project:

```
$ cd fet
$ make
```