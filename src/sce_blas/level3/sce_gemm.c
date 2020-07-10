#include "scelib.h"
#include "scemisc.h"

void sce_gemm(
   const enum CBLAS_TRANSPOSE TRANSA,
   const enum CBLAS_TRANSPOSE TRANSB,
   const int                  M,
   const int                  N,
   const int                  K,
   const double               ALPHA,
   const double               * A,
   const int                  LDA,
   const double               * B,
   const int                  LDB,
   const double               BETA,
   double                     * C,
   const int                  LDC
)
{
	
   double            t0;
   int               i, iail, iblj, icij, j, jal, jbj, jcj, l;
	
   if( ( M == 0 ) || ( N == 0 ) ||
       ( ( ( ALPHA == 0.0 ) || ( K == 0 ) ) &&
         ( BETA == 1.0 ) ) ) return;

   if( ALPHA == 0.0 )
   { Mdgescal( M, N, BETA, C, LDC ); return; }

   if( TRANSB == CblasNoTrans )
   {
      if( TRANSA == CblasNoTrans )
      {
         sce_gemmNN( M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC );
      }
      else
      {
         sce_gemmTN( M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC );
      }
   }
   else
   {
      if( TRANSA == CblasNoTrans )
      {
         sce_gemmNT( M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC );
      }
      else
      {
         sce_gemmTT( M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC );
      }
   }
}
