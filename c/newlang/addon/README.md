# how does my new language load c/c++ addons

- create shared library (*.so)
- load with `dlopen`
- save some type info in .json or so

```bash
> gcc -c -Wall -Werror -fpic print.c
> gcc -shared -o libprint.so print.o
> gcc main.c -o main -ldl
> ./main
n: 4
sqr(n): 9
n: 2
sqr(n): 1
n: 0
```