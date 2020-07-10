

//level 1
		   
#define    Mdabs( a_ ) ( ( (a_) < 0.0 ) ? -(a_) : (a_) )

//level 2

#define    Mdelscal( al_, a_ )                                         \
           {                                                           \
              if(      (al_) == 0.0 ) { (a_)  = 0.0; }     \
              else if( (al_) != 1.0  ) { (a_) *= (al_);     }     \
           }
#define    Mdvscal( n_, al_, x_, incx_ )                               \
           {                                                           \
            int i_, ix_;                                               \
            if(      (al_) == 0.0 )                              \
            {                                                          \
             for( i_ = 0, ix_ = 0; i_ < (n_); i_++, ix_ += (incx_) )   \
             { (x_)[ix_] = 0.0; }                                \
            }                                                          \
            else if( (al_) != 1.0 )                               \
            {                                                          \
             for( i_ = 0, ix_ = 0; i_ < (n_); i_++, ix_ += (incx_) )   \
             { (x_)[ix_] *= (al_); }                                   \
            }                                                          \
           }
//level3
#define    Mdgescal( m_, n_, al_, a_, lda_ )                           \
           {                                                           \
            int i_, iaij_, j_, jaj_;                                   \
            if(      (al_) == 0.0 )                              \
            {                                                          \
             for( j_ = 0, jaj_ = 0; j_ < (n_); j_++, jaj_ += (lda_) )  \
             {                                                         \
              for( i_ = 0, iaij_ = jaj_; i_ < (m_); i_++, iaij_ += 1 ) \
              { (a_)[iaij_] = 0.0; }                             \
             }                                                         \
            }                                                          \
            else if( (al_) != 1.0 )                               \
            {                                                          \
             for( j_ = 0, jaj_ = 0; j_ < (n_); j_++, jaj_ += (lda_) )  \
             {                                                         \
              for( i_ = 0, iaij_ = jaj_; i_ < (m_); i_++, iaij_ += 1 ) \
              { (a_)[iaij_] *= (al_); }                                \
             }                                                         \
            }                                                          \
           }
