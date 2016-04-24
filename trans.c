// Eric Watson 'ewats13'		CS341L       11/14/2014

/*
*trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
//8x8 block of data. divide 32x32 into 16 blocks. transpose block by block
        int b_size;
        int b_row;
        int b_col;
        int i=0;
        int j=0;
        int tmp;
        int w;
        int D_indicator =0;

if (M==32 && N==32)
{
        b_size = 8;

for (b_col=0; b_col < N; b_col += (b_size))
{
        for (b_row = 0; b_row < M; b_row += b_size)
        {
                for (i = b_row; i < b_row + b_size; i++)
                {
                        for (j = b_col; j < b_col + b_size; j++)
                        {
//when its not a diagonal, execute transpose normally
				 if(i !=j) 
                                                                    
                                {
                                        B[j][i] = A[i][j];
                                }

                                else
                                {
                                 tmp=A[i][j];
                                 w=i;
                                 D_indicator = 1;
                                }
                        }
                                if(D_indicator == 1)
                                {
                                B[w][w]=tmp;
                                D_indicator = 0;
                                }
                }
        }
}

}


//incrememt by block size, then have inner loop going through each block
//handle diagonals!! 
//try a block size of 8x4 rather than 4x4
//increment inner most loop by (blocksize<<1)


else if (M == 64 && N==64)
{
        b_size = 4;

for (b_col = 0; b_col < M; b_col += b_size)
{
        for (b_row = 0; b_row < N; b_row += b_size)
                {
                for (i = b_row; i <b_row + b_size; i++)
                {
                        for (j = b_col; j < b_col + b_size; j++)
                        {

                        if(i !=j)

                        B[j][i] = A[i][j];

                        else
                        {                      
                        tmp = A[i][j];
                        w = i;
                        D_indicator = 1;
                        }

                        }

                        if(D_indicator == 1)

                        B[w][w]=tmp;

                        D_indicator = 0;
                        }

                }
}
}

//try block size 20. symmetric. use similar accesses as 32x32
// skip col (M=61) and last row (N=67) else execute normal transpose
else if (M==61 && N==67)
{
        b_size = 20;

for (b_row = 0; b_row < N; b_row += b_size)
{
        for (b_col = 0; b_col < M;b_col += b_size)
         {
                for (i = b_row; i < b_row + b_size; i++)
                 {

                for (j = b_col; j < b_col + b_size; j++)
                 {
                if (i > 66 || j > 60)
                  continue;
                        else
                            B[j][i] = A[i][j];
                }
                }
        }
}
}


}
/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
                                                             
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

                                                         

