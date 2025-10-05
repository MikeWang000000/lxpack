# lxpack

**lxpack** (Linux eXecutable PACKer) is a tool for compressing Linux executable
files using the LZMA algorithm. The packed executable decompresses itself into
memory at startup and then runs normally.


## Features

- Compresses ELF or shell script executables using LZMA to reduce file size
- Self-extracts in memory before execution
- No additional runtime dependencies beyond those required by the original
  executable
- Supports Linux kernel version 2.6.39 and above
- Supports packing executables for any CPU architecture supported by the GCC
  toolchain


## Packaging Requirements

To pack executables with lxpack, you will need:

- GCC toolchain (for native or cross-compilation)
- `xz-utils`
- `xxd`


## Packaging Instructions

To pack a native Linux executable, run:

```sh
make PAYLOAD=/path/to/native_executable
```

To pack an executable for a different CPU architecture (cross-compilation),
run:

```sh
make PAYLOAD=/path/to/aarch64_executable CROSS_COMPILE=aarch64-linux-
```

If you encounter an "Unsupported Architecture" error, it means the inline
assembly for system calls has not yet been adapted for your CPU architecture.
In this case, you can use `STDLIB=1` to statically link with glibc or musl.
Note that the resulting file will be larger:

```sh
make PAYLOAD=/path/to/alpha_executable CROSS_COMPILE=alpha-linux-gnu- STDLIB=1
```

After packing, the resulting file will be located in the `build/` directory.


## Usage

The packed executable can be run directly on any compatible Linux system. No
additional dependencies are required at runtime, except those needed by the
original executable.

**Note:** The minimum supported Linux kernel version is 2.6.39.


## Acknowledgements

Special thanks to the following projects:

- [TinyZZZ](https://github.com/WangXuan95/TinyZZZ) by WangXuan95
- [nolibc](https://github.com/torvalds/linux/tree/master/tools/include/nolibc)
  from the Linux kernel


## License

This project is licensed under the [MIT License](./LICENSE).
