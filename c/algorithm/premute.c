#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int factorial(int n) {
  int r = n;
  while (--n) {
    r *= n;
  }
  return r;
}

void permute(int i, int* nums, int n) {
  int j, t, nf = factorial(n - 1);
  while (n > 2 && i) {
    j = i / nf;
    i = i % nf;
    if (j) {
      t = nums[j];
      nums[j] = nums[0];
      nums[0] = t;
    }
    nf /= --n;
    nums++;
  }
  if (i == 1) {
    t = nums[1];
    nums[1] = nums[0];
    nums[0] = t;
  }
}

void permute_sorted(int i, int* nums, int n) {
  int j, t, nf = factorial(n - 1);
  while (n > 2 && i) {
    j = i / nf;
    i = i % nf;
    while (j--) {
      t = nums[j + 1];
      nums[j + 1] = nums[j];
      nums[j] = t;
    }
    nf /= --n;
    nums++;
  }
  if (i == 1) {
    t = nums[1];
    nums[1] = nums[0];
    nums[0] = t;
  }
}

int main() {
  int n = 4, i, j;
  int nums[] = {1,2,3,4};
  int* copy = (int*)malloc(n * sizeof(int));

  printf("premutate unsorted\n");
  for (i = 0; i < factorial(n); i++) {
    memcpy(copy, nums, n * sizeof(int));
    permute(i, copy, n);

    printf("%d -> ", i);
    for (j = 0; j < n - 1; j++) {
      printf("%d ", copy[j]);
    }
    printf("%d\n", copy[j]);
  }

  printf("premutate sorted\n");
  for (i = 0; i < factorial(n); i++) {
    memcpy(copy, nums, n * sizeof(int));
    permute_sorted(i, copy, n);

    printf("%d -> ", i);
    for (j = 0; j < n - 1; j++) {
      printf("%d ", copy[j]);
    }
    printf("%d\n", copy[j]);
  }
}
