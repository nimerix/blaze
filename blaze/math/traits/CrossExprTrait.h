//=================================================================================================
/*!
//  \file blaze/math/traits/CrossExprTrait.h
//  \brief Header file for the CrossExprTrait class template
//
//  Copyright (C) 2012-2017 Klaus Iglberger - All Rights Reserved
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

#ifndef _BLAZE_MATH_TRAITS_CROSSEXPRTRAIT_H_
#define _BLAZE_MATH_TRAITS_CROSSEXPRTRAIT_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <utility>
#include <blaze/math/typetraits/IsColumnVector.h>
#include <blaze/math/typetraits/IsRowVector.h>
#include <blaze/util/InvalidType.h>
#include <blaze/util/mpl/And.h>
#include <blaze/util/mpl/If.h>
#include <blaze/util/mpl/Or.h>
#include <blaze/util/typetraits/RemoveReference.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Evaluation of the return type of a cross product expression.
// \ingroup math_traits
//
// Via this type trait it is possible to evaluate the return type of a cross product expression.
// Given the two types \a T1 and \a T2, which must be dense or sparse column vectors, the nested
// type \a Type corresponds to the resulting return type. In case the types of \a T1 or \a T2
// don't fit or if the two types cannot be used in a cross product, the resulting data type
// \a Type is set to \a INVALID_TYPE.
*/
template< typename T1    // Type of the left-hand side cross product operand
        , typename T2 >  // Type of the right-hand side cross product operand
struct CrossExprTrait
{
 private:
   //**struct Failure******************************************************************************
   /*! \cond BLAZE_INTERNAL */
   struct Failure { using Type = INVALID_TYPE; };
   /*! \endcond */
   //**********************************************************************************************

   //**struct Result*******************************************************************************
   /*! \cond BLAZE_INTERNAL */
   struct Result { using Type = decltype( std::declval<T1>() % std::declval<T2>() ); };
   /*! \endcond */
   //**********************************************************************************************

 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   using Type = typename If_< Or< And< IsColumnVector< RemoveReference_<T1> >
                                     , IsColumnVector< RemoveReference_<T2> > >
                                , And< IsRowVector< RemoveReference_<T1> >
                                     , IsRowVector< RemoveReference_<T2> > > >
                            , Result
                            , Failure >::Type;
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Auxiliary alias declaration for the CrossExprTrait class template.
// \ingroup math_traits
//
// The CrossExprTrait_ alias declaration provides a convenient shortcut to access the nested
// \a Type of the CrossExprTrait class template. For instance, given the types \a T1 and \a T2
// the following two type definitions are identical:

   \code
   using Type1 = typename CrossExprTrait<T1,T2>::Type;
   using Type2 = CrossExprTrait_<T1,T2>;
   \endcode
*/
template< typename T1    // Type of the left-hand side cross product operand
        , typename T2 >  // Type of the right-hand side cross product operand
using CrossExprTrait_ = typename CrossExprTrait<T1,T2>::Type;
//*************************************************************************************************

} // namespace blaze

#endif
