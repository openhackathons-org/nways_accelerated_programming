#include<stdio.h>
#include<stdlib.h>

int main() {
  int d;
  cudaGetDevice(&d);

  int pma = 0;
  cudaDeviceGetAttribute(&pma, cudaDevAttrPageableMemoryAccess, d);
  printf("Full Unified Memory Support: %s\n", pma == 1? "YES" : "NO");
  
  int cma = 0;
  cudaDeviceGetAttribute(&cma, cudaDevAttrConcurrentManagedAccess, d);
  printf("CUDA Managed Memory with full support: %s\n", cma == 1? "YES" : "NO");

  return 0;
}