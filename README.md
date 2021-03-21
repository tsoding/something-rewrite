# Rewrite of Something from scratch

## Objectives of the Rewrite

### Migrating completely to OpenGL.

First iteration of [Something](https://github.com/tsoding/something) was using pure SDL2 which was extremely limiting and didn't allow certain features that are available even in the oldest OpenGL versions.

I finally learnt enough OpenGL so I can implement a simple 2D engine in it.

### Changing the Architecture to simplify piling features on top of the game.

Making the game state easily available to almost any part of the code almost at any time.

The only limitation is spliting the access into two distinct phases:
- Update Phase -- the state is mutable by any part of the code that has a pointer to the `Game` object.
- Render Phase -- the state is read-only.

The `Game` object becames the central communication hub between different parts of the system, similarly to a database in [CRUD](https://en.wikipedia.org/wiki/Create,_read,_update_and_delete) applications.

## Quick Start in Linux OS

```console
$ ./build.sh
$ ./something.debug
```

## Quick Start in Windows OS

```console
> build.bat
> something.debug.exe
```

