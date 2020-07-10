#include "scelib.h"

void Mdelscal(double BETA, double X){
    if(BETA == 0.0){
        X = 0.0;
    }else if(BETA != 1.0){
        X *= BETA;
    }
}

void ATL_drefgemvN(const int M,
                   const int N,
                   const double ALPHA,
                   const double               * A,
                   const int                  LDA,
                   const double               * X,
                   const int                  INCX,
                   const double               BETA,
                   double                     * Y,
                   const int                  INCY
                  ) {

    register double            t0;
    int                        i, iaij, iy, j, jaj, jx;

    sce_scal( M, BETA, Y, INCY );
    for( j = 0, jaj  = 0, jx = 0; j < N; j++, jaj += LDA, jx += INCX ) {
        t0 = ALPHA * X[jx];
        for( i = 0, iaij = jaj, iy = 0; i < M; i++, iaij += 1, iy += INCY ) {
            Y[iy] += A[iaij] * t0;
        }
    }
}

void ATL_drefgemvT
(
    const int                  M,
    const int                  N,
    const double               ALPHA,
    const double               * A,
    const int                  LDA,
    const double               * X,
    const int                  INCX,
    const double               BETA,
    double                     * Y,
    const int                  INCY
) {
    register double            t0;
    int                        i, iaij, ix, j, jaj, jy;

    for( j = 0, jaj  = 0, jy  = 0; j < M; j++, jaj += LDA, jy += INCY ) {
        t0 = 0.0;
        for( i = 0, iaij = jaj, ix = 0; i < N; i++, iaij += 1, ix += INCX ) {
            t0 += A[iaij] * X[ix];
        }
        Mdelscal( BETA, Y[jy] );
        Y[jy] += ALPHA * t0;
    }
}

void cblas_dgemv(const enum CBLAS_ORDER order,
                 const enum CBLAS_TRANSPOSE TransA, const int M, const int N,
                 const double alpha, const double  *A, const int lda,
                 const double  *X, const int incX, const double beta,
                 double  *Y, const int incY)
{
    if( ( M == 0 ) || ( N == 0 ) ||
    ( ( alpha == 0.0 ) && ( beta == 1.0  ) ) ) return;

    if( alpha == 0.0 ) {
        sce_scal( M, beta, Y, incY );
        return;
    }

    if (order == CblasColMajor) {
        if ( ( TransA == CblasNoTrans ) || ( TransA == CblasConjTrans ) ) {
            ATL_drefgemvN( M, N, alpha, A, lda, X, incX, beta, Y, incY );
        } else if(TransA == CblasTrans) {
            ATL_drefgemvT( M, N, alpha, A, lda, X, incX, beta, Y, incY );
        } else {
            return;
        }
    }
    else if (order == CblasRowMajor) {
        if ( ( TransA == CblasNoTrans ) || ( TransA == CblasConjTrans ) ) {
            ATL_drefgemvN(N, M, alpha, A, lda, X, incX, beta, Y, incY);
        } else if(TransA == CblasTrans) {
            ATL_drefgemvT( N, M, alpha, A, lda, X, incX, beta, Y, incY );
        } else {
            return;
        }
    }
   //else cblas_xerbla(1, "cblas_dgemv", "Illegal Order setting, %d\n", order);
   return;
}
