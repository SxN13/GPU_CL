__kernel void add(__global const int *A, __global const int *B, __global int *C) {
    int i = get_global_id(0);
	C[i] = A[i] + B[i];
}
__kernel void div(__global const int *A, __global const int *B, __global int *C) {
    int i = get_global_id(0);
	C[i] = A[i] / B[i];
}
__kernel void mul(__global const int *A, __global const int *B, __global int *C) {
    int i = get_global_id(0);
	C[i] = A[i] * B[i];
}
__kernel void sub(__global const int *A, __global const int *B, __global int *C) {
    int i = get_global_id(0);
	C[i] = A[i] - B[i];
}