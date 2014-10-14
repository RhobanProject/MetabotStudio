# Metabot

The all-in-one robot workshop

## Building

First, you'll have to build the metabot backend:

```
cd metabot
mkdir build
cd build
cmake ..
make
```

Then, go to `gui/`, and use Qt to build.

**Note: you'll need to copy or link the backends/ directory in the same
directory that the binary**
