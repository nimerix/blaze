//=================================================================================================
/*!
//  \file blaze/math/lapack/heev.h
//  \brief Header file for the LAPACK Hermitian matrix eigenvalue functions (heev)
//
//  Copyright (C) 2013 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
*/
//=================================================================================================

#ifndef _BLAZE_MATH_LAPACK_HEEV_H_
#define _BLAZE_MATH_LAPACK_HEEV_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <memory>
#include <boost/cast.hpp>
#include <blaze/math/Aliases.h>
#include <blaze/math/constraints/Adaptor.h>
#include <blaze/math/constraints/BLASCompatible.h>
#include <blaze/math/constraints/Computation.h>
#include <blaze/math/constraints/MutableDataAccess.h>
#include <blaze/math/Exception.h>
#include <blaze/math/expressions/DenseMatrix.h>
#include <blaze/math/expressions/DenseVector.h>
#include <blaze/math/lapack/clapack/heev.h>
#include <blaze/math/typetraits/IsResizable.h>
#include <blaze/math/typetraits/IsRowMajorMatrix.h>
#include <blaze/util/Assert.h>
#include <blaze/util/constraints/Complex.h>


namespace blaze {

//=================================================================================================
//
//  LAPACK HERMITIAN MATRIX EIGENVALUE FUNCTIONS (HEEV)
//
//=================================================================================================

//*************************************************************************************************
/*!\name LAPACK Hermitian matrix eigenvalue functions (heev) */
//@{
template< typename MT, bool SO, typename VT, bool TF >
inline void heev( DenseMatrix<MT,SO>& A, DenseVector<VT,TF>& w, char jobz, char uplo );
//@}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief LAPACK kernel for computing the eigenvalues of the given dense Hermitian matrix.
// \ingroup lapack_eigenvalue
//
// \param A The given Hermitian matrix.
// \param w The resulting vector of eigenvalues.
// \param jobz \c 'V' to compute the eigenvectors of \a A, \c 'N' to only compute the eigenvalues.
// \param uplo \c 'L' to use the lower part of the matrix, \c 'U' to use the upper part.
// \return void
// \exception std::invalid_argument Invalid non-square matrix provided.
// \exception std::invalid_argument Vector cannot be resized.
// \exception std::invalid_argument Invalid jobz argument provided.
// \exception std::invalid_argument Invalid uplo argument provided.
// \exception std::runtime_error Eigenvalue computation failed.
//
// This function computes the eigenvalues of a Hermitian \a n-by-\a n matrix based on the LAPACK
// heev() functions. Optionally, it computes the left and right eigenvectors.
//
// The real eigenvalues are returned in ascending order in the given vector \a w. \a w is resized
// to the correct size (if possible and necessary). In case \a A is a row-major matrix, the left
// eigenvectors are returned in the rows of \a A, in case \a A is a column-major matrix, the right
// eigenvectors are returned in the columns of \a A.
//
// The function fails if ...
//
//  - ... the given matrix \a A is not a square matrix;
//  - ... the given vector \a w is a fixed size vector and the size doesn't match;
//  - ... the given \a jobz argument is neither \c 'V' nor \c 'N';
//  - ... the given \a uplo argument is neither \c 'L' nor \c 'U';
//  - ... the eigenvalue computation fails.
//
// In all failure cases a \a std::invalid_argument exception is thrown.
//
// Examples:

   \code
   using blaze::DynamicMatrix;
   using blaze::DynamicVector;
   using blaze::rowMajor;
   using blaze::columnVector;

   DynamicMatrix<complex<double>,rowMajor> A( 5UL, 5UL );  // The Hermitian matrix A
   // ... Initialization

   DynamicVector<double,columnVector> w( 5UL );  // The vector for the real eigenvalues

   heev( A, w, 'V', 'L' );
   \endcode

// For more information on the heev() functions (i.e. cheev() and zheev()) see the LAPACK online
// documentation browser:
//
//        http://www.netlib.org/lapack/explore-html/
//
// \note This function can only be used if the fitting LAPACK library is available and linked to
// the executable. Otherwise a call to this function will result in a linker error.
*/
template< typename MT  // Type of the matrix A
        , bool SO      // Storage order of the matrix A
        , typename VT  // Type of the vector w
        , bool TF >    // Transpose flag of the vector w
inline void heev( DenseMatrix<MT,SO>& A, DenseVector<VT,TF>& w, char jobz, char uplo )
{
   using boost::numeric_cast;

   BLAZE_CONSTRAINT_MUST_NOT_BE_ADAPTOR_TYPE( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_COMPUTATION_TYPE( MT );
   BLAZE_CONSTRAINT_MUST_HAVE_MUTABLE_DATA_ACCESS( MT );
   BLAZE_CONSTRAINT_MUST_BE_BLAS_COMPATIBLE_TYPE( ElementType_<MT> );
   BLAZE_CONSTRAINT_MUST_BE_COMPLEX_TYPE( ElementType_<MT> );

   BLAZE_CONSTRAINT_MUST_NOT_BE_COMPUTATION_TYPE( VT );
   BLAZE_CONSTRAINT_MUST_HAVE_MUTABLE_DATA_ACCESS( VT );
   BLAZE_CONSTRAINT_MUST_BE_BLAS_COMPATIBLE_TYPE( ElementType_<VT> );
   BLAZE_CONSTRAINT_MUST_BE_BUILTIN_TYPE( ElementType_<VT> );

   using CT = ElementType_<MT>;
   using BT = UnderlyingElement_<CT>;

   if( !isSquare( ~A ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid non-square matrix provided" );
   }

   if( jobz != 'V' && jobz != 'N' ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid jobz argument provided" );
   }

   if( uplo != 'L' && uplo != 'U' ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid uplo argument provided" );
   }

   resize( ~w, (~A).rows(), false );

   int n   ( numeric_cast<int>( (~A).rows()    ) );
   int lda ( numeric_cast<int>( (~A).spacing() ) );
   int info( 0 );

   if( n == 0 ) {
      return;
   }

   int lwork( 10*n + 2 );
   const std::unique_ptr<CT[]> work ( new CT[lwork] );
   const std::unique_ptr<BT[]> rwork( new BT[3*n-2] );

   if( IsRowMajorMatrix<MT>::value ) {
      ( uplo == 'L' )?( uplo = 'U' ):( uplo = 'L' );
   }

   heev( jobz, uplo, n, (~A).data(), lda, (~w).data(), work.get(), lwork, rwork.get(), &info );

   BLAZE_INTERNAL_ASSERT( info >= 0, "Invalid argument for eigenvalue computation" );

   if( info > 0 ) {
      BLAZE_THROW_LAPACK_ERROR( "Eigenvalue computation failed" );
   }
}
//*************************************************************************************************

} // namespace blaze

#endif