# Regular Expression for Binary Numbers Divisible by n

This repository contains a C program designed to generate a regular expression (regex) string that can evaluate binary strings and determine whether the given string represents a number divisible by a provided integer 'n'.

## Usage

The program is executed via the command line, taking one positive integer argument:

```
Usage: [program_name] positive_integer
```

The program will generate and print out a regular expression string corresponding to the binary numbers divisible by the input integer.

## Program Structure

The core function of the program, **'regex_divisible_by'**, takes an integer as input and returns a pointer to the generated regex string. This function applies a strategy involving matrix manipulation and buffer management to formulate the correct regular expression. Several helper functions are used to create and populate matrices, concatenate strings and generate the final result.

The **'main'** function parses command line arguments, calls the **'regex_divisible_by'** function and prints the resulting regular expression.

## Build

To build the project, could be used a C compiler such as **'gcc'**. Example build command:

```
gcc -o regex_bin_div program_name.c
```

Where **'program_name.c'** should be replaced with the actual name of the C file.

## Tests

Inputs 1 <= n <= 18 will be tested. Each **'n'** will be tested against random invalid tests and random valid tests (which may or may not pass the regex test itself, accordingly).

## Notes

- Strings that are not binary numbers should be rejected.
- The solution must be under 5000 characters. This means you can't hard-code the answers.
- Only these characters may be included in your returned string: **'01?*+^$()|'** (POSIX Extended Regular Expressions subset)

## Debugging

The program uses the standard C library's memory management functions (like **'malloc'** and **'free'**). These functions are error-prone and can cause segmentation faults if not used properly. In such cases, debugging with tools like **'gdb'** can help identify the source of the problem.

Example debugging session:

```
[teuerstein@fedora-1 ~]$ sudo gdb ./main
GNU gdb (GDB) Fedora Linux 13.1-4.fc38
[...]
(gdb) run 17
Starting program: /home/teuerstein/main 17
[Thread debugging using libthread_db enabled]
Program received signal SIGSEGV, Segmentation fault.
__memcpy_avx_unaligned_erms () at ../sysdeps/x86_64/multiarch/memmove-vec-unaligned-erms.S:406
(gdb) backtrace
#0  __memcpy_avx_unaligned_erms () at ../sysdeps/x86_64/multiarch/memmove-vec-unaligned-erms.S:406
[...]
#5  0x0000000000401abd in regex_divisible_by (n=17) at main.c:110
#6  0x0000000000401f45 in main (argc=2, argv=0x7fffffffe4b8) at main.c:190
...
```

Here, the segmentation fault was traced back to the **'regex_divisible_by'** function at **'main.c:110'**. From this information, the function can be inspected and corrected.
