### Image Palette
Use k colors to recreate the input image. Example for [k_means.h](https://github.com/TimSousa1/k_means)

Uses some [stb libs](https://github.com/nothings/stb) to read from and write to more complex image formats.

### Usage:
```
$ ./km [-k <number of colors>] [-i] <input file> [-o <output file>]
```
Compile with
```
make
```

You can also use raylib to step through each iteration manually and visualize the image every step.
```
make VISUALIZE=1
```
