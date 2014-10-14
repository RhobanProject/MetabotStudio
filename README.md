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

**Note 2: you'll need an openscad version with marker() support**

**Note 3: the program will generate STL cache on first run, so be patient
when opening the components wizard for the first time!**
