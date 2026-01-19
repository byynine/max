a language should be as flexible, free and simple as possible. So much so even a kid should be able to understand it, and with its power then be able to conquer the universe.

# ideas

- optional args

```Maxfile
print [ arg 1? ] { echo hello }
```

# sketch

```Maxfile
command ( date "dist/main" < "src/main.c" & exist "src/module/core.h" ) [ arg > -1-3 ] { echo }
```

**date** returns the file creation date of a file.
"" are used so that spaces are correctly interpreted and also make a good syntax for other commands.
can be written like: `"file" date` and it'll return the same thing.
if it'll just be `"something"` then the previous keyword will be used, as demoed.

```Maxfile
command ( date "dist/main" < "src/main.c"; ) [ arg > -1-3; ] { echo }
```

```Maxfile
command [ arg 1-2; ] { echo }
```

```bash
$ max print this that this wont
this
that
```

---

```Maxfile
command ( date "dist/main" < "src/main.c"; ) [ arg > 1..-3; ] { echo }
```
---

```Maxfile
print [ arg 1 ] { echo # # cool # }
```

```bash
$ max print test
```

---

```Maxfile
max { gcc -o dist/main src/main.c }
build { gcc -o dist/main src/main.c }
```

```bash
$ max
gcc -o dist/main src/main.c
```

---

```Maxfile
var variable = "cool"

main ( date "file" < "file" -> c1 ) [ arg 1 ] { echo # @variable}
c1 { echo im c1 }
c2 { echo im c2 }
```

```bash
$ max main
expected argument
$ max main sick
sick cool
```
---

```Maxfile
def variable mycoolvalue
ref coolbuild
cmd echo variable
```

```Maxfile
def variable mycoolvalue

def bref ref coolbuild
def ref build

def cmd1 cmd echo variable

lnk build cmd1 
```
