The MicroC compiler

Coded in OCaml, this takes a highly stripped-down subset of C (ints,
bools, and void types, arithmetic, if-else, for, and while statements,
and user-defined functions) and compiles it into LLVM IR.

It needs the OCaml llvm library, which is most easily installed through opam.

Install LLVM and its development libraries, the m4 macro preprocessor,
and opam, then use opam to install llvm.

The version of the OCaml llvm library must match the version of the LLVM
system installed on your system.

In addition to print, which calls the C library function printf(),
microc gratuitiously includes a primitive function "printbig," which
prints large ASCII-encoded characters.

The stock C compiler compiles printbig.o.  testall.sh runs the microc
executable on each testcase (.mc file) to produce a .ll file, invokes
"llc" (the LLVM compiler) to produce a .s (assembly) file, then
invokes "cc" (the stock C compiler) to assemble the .s file, link in
printbig.o, and generate an executable.  See testall.sh for details.
------------------------------
Tips to test built-in fucntion

Compile external function as object file with gcc (do not use g++, it would casue link error!!!)

$ gcc -o mydraw.o -c mydraw.c -lGL -lGLU -lglut

Compile FPL code into assembly 

$ ./micro.native ./fplTest/test-drawLine.mc | llc > drawLine.S

Link two files

$ gcc -o mydraw mydraw.o drawLine.S -lGL -lGLU -lglut
