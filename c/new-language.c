#include <stdio.h>

typedef enum { v_memory, v_constant, v_register } h_vtype;

typedef enum { r_eax, r_ebx } h_rtype;

typedef struct {
  int offset;
} h_memobj;

typedef struct {
  h_vtype vtype;
  union {
    h_memobj mem;
    int constant;
    h_rtype rtype;
  } entries;
} h_argument;

typedef enum { c_assign, c_add, c_printi, c_exit } h_ctype;

typedef struct {
  h_argument lval;
  h_argument rval;
} h_assign;

typedef struct {
  h_argument val;
} h_add;

typedef struct {
  h_argument val;
} h_printi;

typedef struct {
  h_ctype ctype;
  union {
    h_assign assign;
    h_add add;
    h_printi printi;
  } args;
} h_call;

int execute(int *memory, h_call *calls) {
  register int eax;
  register int ebx;
  register int v;

#define val_get(arg, val)                                                      \
  switch (arg.vtype) {                                                         \
  case v_constant:                                                             \
    val = arg.entries.constant;                                                \
    break;                                                                     \
  case v_memory:                                                               \
    val = memory[arg.entries.mem.offset];                                      \
    break;                                                                     \
  case v_register:                                                             \
    switch (arg.entries.rtype) {                                               \
    case r_eax:                                                                \
      val = eax;                                                               \
      break;                                                                   \
    case r_ebx:                                                                \
      val = ebx;                                                               \
      break;                                                                   \
    }                                                                          \
    break;                                                                     \
  }

#define val_set(arg, val)                                                      \
  switch (arg.vtype) {                                                         \
  case v_constant:                                                             \
    goto exit;                                                                 \
  case v_memory:                                                               \
    memory[arg.entries.mem.offset] = val;                                      \
    break;                                                                     \
  case v_register:                                                             \
    switch (arg.entries.rtype) {                                               \
    case r_eax:                                                                \
      eax = val;                                                               \
      break;                                                                   \
    case r_ebx:                                                                \
      ebx = val;                                                               \
      break;                                                                   \
    }                                                                          \
    break;                                                                     \
  }

  while (1) {
    h_call call = *calls;
    switch (call.ctype) {
    case c_add:
      val_get(call.args.add.val, v);
      eax += v;
      break;
    case c_assign:
      val_get(call.args.assign.rval, v);
      val_set(call.args.assign.lval, v);
      break;
    case c_printi:
      val_get(call.args.add.val, v);
      printf("%d\n", v);
      break;
    case c_exit:
      goto exit;
      break;
    }

    calls++;
  }
exit:
  return ebx;
#undef val_set
#undef val_get
}

int main() {
  int memory[] = {0, 0, 0, 0, 0};
#define define_call(ct, ...) \
  {c_##ct, .args.ct = {__VA_ARGS__}}
#define assign_call(...) define_call(assign, __VA_ARGS__)
#define add_call(...) define_call(add, __VA_ARGS__)
#define printi_call(...) define_call(printi, __VA_ARGS__)
#define exit_call {c_exit}
  h_call calls[] = {
      assign_call({v_memory, {0}}, {v_constant, 1}),
      assign_call({v_memory, {1}}, {v_constant, 1}),
      assign_call({v_register, r_ebx}, {v_memory, {0}}),
      assign_call({v_register, r_eax}, {v_memory, {1}}),
      add_call({v_register, r_ebx}),
      assign_call({v_memory, {2}}, {v_register, r_eax}),
      assign_call({v_register, r_ebx}, {v_memory, {1}}),
      add_call({v_register, r_ebx}),
      assign_call({v_memory, {3}}, {v_register, r_eax}),
      assign_call({v_register, r_ebx}, {v_memory, {2}}),
      add_call({v_register, r_ebx}),
      assign_call({v_memory, {4}}, {v_register, r_eax}),
      printi_call({v_memory, {0}}),
      printi_call({v_memory, {1}}),
      printi_call({v_memory, {2}}),
      printi_call({v_memory, {3}}),
      printi_call({v_memory, {4}}),
      assign_call({v_register, r_ebx}, {v_constant, 0}),
      exit_call};
#undef define_call
#undef assign_call
#undef add_call
#undef printi_call
#undef exit_call
  printf("fibonacci 5:\n");
  int exitcode = execute(memory, calls);
  printf("exit code: %d\n", exitcode);

  return exitcode;
}
