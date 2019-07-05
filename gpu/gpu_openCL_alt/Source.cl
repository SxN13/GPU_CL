__kernel void vector_add(__global const int *A, __global const int *B, __global int *C) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
	for(int j = 0; j < 1000; j++)
    {
		// Do the operation
		C[i] = A[i] + B[i];
	}
}