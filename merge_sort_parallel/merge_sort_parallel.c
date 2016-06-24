#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

/* Parallel Merge sort algorithm implemented straight out of CLRS without much
 * thought to speeding it up. The parallel version with openmp is
 * monstrously slower than the version compilied without the -fopenmp flag.
 * Use of openmp here is extremely likely not worth it. Likely no use in
 * optomizing this further other than practice.
 */

#define LIST_SIZE 10000

void P_Merge_Sort(int *A, int p, int r, int *B, int s);
void P_Merge(int *T, int p1, int r1, int p2, int r2, int *A, int p3);
int bin_search(int x, int *T, int start, int end);

main() {
    int i;
    int *A, *B;

    A = malloc(sizeof(int)*LIST_SIZE);
    B = malloc(sizeof(int)*LIST_SIZE);

    for (i=0; i<LIST_SIZE; i++) {
        A[i] = (int) rand();
    }
    #pragma omp parallel
    #pragma omp single
    P_Merge_Sort(A, 0, LIST_SIZE-1, B, 0);
    for (i=1; i<LIST_SIZE; i++) {
        if (B[i] < B[i-1]) {
            printf("Sorting was incorrect!\n");
            break;
        }
    }
    if (i == LIST_SIZE) printf("Success!\n");
    free(A);
    free(B);
}

void P_Merge_Sort(int *A, int p, int r, int *B, int s) {
    int mid;
    int n;
    int *T;
    n = r-p+1;
    if (n == 1) {
        B[s] = A[p];
        return;
    }
    
    T = (int *) malloc(sizeof(int)*n);
    mid = (r+p)/2;
    int left_n = mid - p+1;

    #pragma omp task untied
    P_Merge_Sort(A, p, mid, T, 0);
    #pragma omp task untied
    P_Merge_Sort(A, mid+1, r, T, left_n);
    #pragma omp taskwait
    P_Merge(T, 0, left_n-1, left_n, n-1, B, s);

    free(T);
}

void P_Merge(int *T, int p1, int r1, int p2, int r2, int *A, int p3) {
    int q1, q2, q3;
    int n1 = r1 - p1 + 1;
    int n2 = r2 - p2 + 1;
    if (n2 > n1) {
        int temp;
        temp = p1; p1 = p2; p2 = temp;
        temp = r1; r1 = r2; r2 = temp;
        temp = n1; n1 = n2; n2 = temp;
    }
    if (n1 <= 0) return;

    q1 = (r1 + p1)/2;
    q2 = bin_search(T[q1], T, p2, r2);
    q3 = p3 + (q1 - p1) + (q2 - p2);

    A[q3] = T[q1];
    
    #pragma omp task untied
    P_Merge(T, p1, q1-1, p2, q2-1, A, p3);
    #pragma omp task untied
    P_Merge(T, q1+1, r1, q2, r2, A, q3+1);
    #pragma omp taskwait
}

int bin_search(int x, int *T, int start, int end) {
    int mid;
    int low = start;
    int hi = end+1 > start ? end+1: start; //max(start,end+1)

    while(low < hi) {
        mid = (hi+low)/2;
        if (x <= T[mid]) hi = mid;
        else low = mid+1;
    }
    return low;
}

