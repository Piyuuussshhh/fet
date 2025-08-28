# fet - File Encryption Tool
This command-line tool encrypts a standard .txt file to a special .etxt file, that contains encrypted, jarbled text. It can only be decrypted with this tool.

To encrypt a text file: `./build/fet -e path/to/file -k <key>`

To decrypt a .etxt file: `./build/fet -d path/to/file -k <key>`

Upon decryption, a new text file with the same name before .etxt will be created, in the same directory as the .etxt, for viewing the contents.
Think of the key parameter as a password to the file.

Make sure to **use the same key** for encrypting and decrypting a file. If you forget it, the contents of the file are lost forever (lmao no well maybe).