# Custom Libc Development Roadmap

## Functions Implemented

### File I/O Functions
- [x] [`fopen`](libc.md#fopen)
- [x] [`fclose`](libc.md#fclose)
- [x] [`fwrite`](libc.md#fwrite)
- [x] [`fread`](libc.md#fread)
- [X] [`fprintf`](libc.md#fprintf)
- [x] [`create_file`](libc.md#create_file)
- [x] [`rm_file`](libc.md#rm_file)

## Potential Additions and Enhancements

### File I/O Functions
- [ ] `open`
- [ ] `close`
- [ ] `read`
- [ ] `write`
- [ ] `seek`
- [ ] `remove`

### Standard I/O Functions
- [X] `printf`
- [ ] `scanf`

### Memory Management Functions
- [ ] `malloc`
- [ ] `free`

## General Enhancements
- [ ] Improve error handling and return codes for existing functions.
- [ ] Ensure that the behavior of each function closely aligns with established standards (e.g., POSIX).
