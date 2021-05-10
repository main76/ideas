#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
  const int nmethod = 2;
  void (*prt[nmethod])(int);
  const char* pnames[] = {"print_n", "print_sqrn"};

  void *handle;
  char *error;

  handle = dlopen("./libprint.so", RTLD_LAZY);
  if (!handle) {
    fprintf(stderr, "%s\n", dlerror());
    exit(1);
  }

  dlerror();
  for (int i = 0; i < nmethod; i++) {
    prt[i] = dlsym(handle, pnames[i]);
    if ((error = dlerror()) != NULL) {
      fprintf(stderr, "%s\n", dlerror());
      exit(1);
    }
  }
  
  int t = 5;
  while (t--) {
    prt[t%nmethod](t);
  }

  dlclose(handle);
  return 0;
}
