**max** is designed to be an intuitive and easy to use interpreter and command executor.
It can be used for building programs or simple automation of tasks.

*This is a development version, do not expect complete, bugless features and ease of use.*
# Overview
**max** uses regular expressions for parsing strings with a simple algorithm and a modular design that's easy to understand and work with, while being extensible.

Using **max** is as simple as creating a **Maxfile** under the current directory or specified with *-m, --maxfile* option for a different path, it is the file from where behavior is defined.

**Maxfile**
```Maxfile
test=echo @1
```
*Maxfile is space-sensitive*

Running:
```bash
$ max test hello
```
Outputs:
```bash
hello
```
The @n syntax allows you to specify if a command takes an argument. This number is relative and can be zero, a negative, or positive integer.

# Installation
Installation on MacOS or Windows platforms have not been tested yet, but should be compatible.

## Linux

For installation on Linux run the Makefile.
```bash
$ make
```

Now run the following command as root:
```bash
$ cp dist/max /usr/bin
```
**max** can now be used:
```bash
$ max --help
```
