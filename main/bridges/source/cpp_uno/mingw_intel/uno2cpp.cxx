/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_bridges.hxx"

#include <com/sun/star/uno/genfunc.hxx>
#include "com/sun/star/uno/RuntimeException.hpp"
#include <uno/data.h>
#include <sal/alloca.h>

#include "bridges/cpp_uno/shared/bridge.hxx"
#include "bridges/cpp_uno/shared/types.hxx"
#include "bridges/cpp_uno/shared/unointerfaceproxy.hxx"
#include "bridges/cpp_uno/shared/vtables.hxx"

#include "share.hxx"
#include "smallstruct.hxx"

using namespace ::rtl;
using namespace ::com::sun::star::uno;

namespace
{

//==================================================================================================
// The call instruction within the asm section of callVirtualMethod may throw
// exceptions.  So that the compiler handles this correctly, it is important
// that (a) callVirtualMethod might call dummy_can_throw_anything (although this
// never happens at runtime), which in turn can throw exceptions, and (b)
// callVirtualMethod is not inlined at its call site (so that any exceptions are
// caught which are thrown from the instruction calling callVirtualMethod):
void callVirtualMethod(
    void * pAdjustedThisPtr,
    sal_Int32 nVtableIndex,
    void * pRegisterReturn,
    typelib_TypeDescription const * returnType,
    sal_Int32 * pStackLongs,
    sal_Int32 nStackLongs ) __attribute__((noinline));

void callVirtualMethod(
    void * pAdjustedThisPtr,
    sal_Int32 nVtableIndex,
    void * pRegisterReturn,
    typelib_TypeDescription const * returnType,
    sal_Int32 * pStackLongs,
    sal_Int32 nStackLongs )
{
	// parameter list is mixed list of * and values
	// reference parameters are pointers
    
	OSL_ENSURE( pStackLongs && pAdjustedThisPtr, "### null ptr!" );
	OSL_ENSURE( (sizeof(void *) == 4) && (sizeof(sal_Int32) == 4), "### unexpected size of int!" );
	OSL_ENSURE( nStackLongs && pStackLongs, "### no stack in callVirtualMethod !" );
    
    // never called
    if (! pAdjustedThisPtr) CPPU_CURRENT_NAMESPACE::dummy_can_throw_anything("xxx"); // address something

	volatile long edx = 0, eax = 0; // for register returns
    void * stackptr;
	asm volatile (
        "mov   %%esp, %6\n\t"
		// copy values
		"mov   %0, %%eax\n\t"
		"mov   %%eax, %%edx\n\t"
		"dec   %%edx\n\t"
		"shl   $2, %%edx\n\t"
		"add   %1, %%edx\n"
		"Lcopy:\n\t"
		"pushl 0(%%edx)\n\t"
		"sub   $4, %%edx\n\t"
		"dec   %%eax\n\t"
		"jne   Lcopy\n\t"
		// do the actual call
		"mov   %2, %%edx\n\t"
		"mov   0(%%edx), %%edx\n\t"
		"mov   %3, %%eax\n\t"
		"shl   $2, %%eax\n\t"
		"add   %%eax, %%edx\n\t"
		"mov   0(%%edx), %%edx\n\t"
		"call  *%%edx\n\t"
		// save return registers
 		"mov   %%eax, %4\n\t"
 		"mov   %%edx, %5\n\t"
		// cleanup stack
        "mov   %6, %%esp\n\t"
		:
        : "m"(nStackLongs), "m"(pStackLongs), "m"(pAdjustedThisPtr),
          "m"(nVtableIndex), "m"(eax), "m"(edx), "m"(stackptr)
        : "eax", "edx" );
	switch( returnType->eTypeClass )
	{
		case typelib_TypeClass_VOID:
			break;
		case typelib_TypeClass_HYPER:
		case typelib_TypeClass_UNSIGNED_HYPER:
			((long*)pRegisterReturn)[1] = edx;
		case typelib_TypeClass_LONG:
		case typelib_TypeClass_UNSIGNED_LONG:
		case typelib_TypeClass_CHAR:
		case typelib_TypeClass_ENUM:
			((long*)pRegisterReturn)[0] = eax;
			break;
		case typelib_TypeClass_SHORT:
		case typelib_TypeClass_UNSIGNED_SHORT:
			*(unsigned short*)pRegisterReturn = eax;
			break;
		case typelib_TypeClass_BOOLEAN:
		case typelib_TypeClass_BYTE:
			*(unsigned char*)pRegisterReturn = eax;
			break;
		case typelib_TypeClass_FLOAT:
			asm ( "fstps %0" : : "m"(*(char *)pRegisterReturn) );
			break;
		case typelib_TypeClass_DOUBLE:
			asm ( "fstpl %0\n\t" : : "m"(*(char *)pRegisterReturn) );
			break;
		case typelib_TypeClass_STRUCT:
			if (bridges::cpp_uno::shared::isSmallStruct(returnType)) {
                		if (returnType->nSize <= 1) {
					*(unsigned char*)pRegisterReturn = eax;
	        		}
	        		else if (returnType->nSize <= 2) {
					*(unsigned short*)pRegisterReturn = eax;
	        		}
                		else if (returnType->nSize <= 8) {
					((long*)pRegisterReturn)[0] = eax;
        				if (returnType->nSize > 4) {
						((long*)pRegisterReturn)[1] = edx;
					}
               			}
	    		}
			break;
        default:
	    break;
	}
}

//================================================================================================== 
static void cpp_call(
	bridges::cpp_uno::shared::UnoInterfaceProxy * pThis,
    bridges::cpp_uno::shared::VtableSlot aVtableSlot,
	typelib_TypeDescriptionReference * pReturnTypeRef,
	sal_Int32 nParams, typelib_MethodParameter * pParams,
	void * pUnoReturn, void * pUnoArgs[], uno_Any ** ppUnoExc )
{
  	// max space for: [complex ret ptr], values|ptr ...
  	char * pCppStack		=
#ifdef BROKEN_ALLOCA
  		(char *)malloc( sizeof(sal_Int32) + ((nParams+2) * sizeof(sal_Int64)) );
#else
  		(char *)alloca( sizeof(sal_Int32) + ((nParams+2) * sizeof(sal_Int64)) );
#endif
  	char * pCppStackStart	= pCppStack;
	
	// return
	typelib_TypeDescription * pReturnTypeDescr = 0;
	TYPELIB_DANGER_GET( &pReturnTypeDescr, pReturnTypeRef );
	OSL_ENSURE( pReturnTypeDescr, "### expected return type description!" );
	
	void * pCppReturn = 0; // if != 0 && != pUnoReturn, needs reconversion
	
	if (pReturnTypeDescr)
	{
		if (bridges::cpp_uno::shared::isSimpleType( pReturnTypeDescr ))
		{
			pCppReturn = pUnoReturn; // direct way for simple types
		}
		else
		{
			// complex return via ptr
			pCppReturn
                = (bridges::cpp_uno::shared::relatesToInterfaceType(
                       pReturnTypeDescr )
#ifdef BROKEN_ALLOCA
                   ? malloc( pReturnTypeDescr->nSize )
#else
                   ? alloca( pReturnTypeDescr->nSize )
#endif
                   : pUnoReturn); // direct way
			if (!bridges::cpp_uno::shared::isSmallStruct(pReturnTypeDescr)) {
				*(void **)pCppStack = pCppReturn;
				pCppStack += sizeof(void *);
			}
		}
	}
	// push this
    void * pAdjustedThisPtr = reinterpret_cast< void ** >(pThis->getCppI())
        + aVtableSlot.offset;
	*(void**)pCppStack = pAdjustedThisPtr;
	pCppStack += sizeof( void* );

	// stack space
	OSL_ENSURE( sizeof(void *) == sizeof(sal_Int32), "### unexpected size!" );
	// args
#ifdef BROKEN_ALLOCA
	void ** pCppArgs  = (void **)malloc( 3 * sizeof(void *) * nParams );
#else
	void ** pCppArgs  = (void **)alloca( 3 * sizeof(void *) * nParams );
#endif
	// indizes of values this have to be converted (interface conversion cpp<=>uno)
	sal_Int32 * pTempIndizes = (sal_Int32 *)(pCppArgs + nParams);
	// type descriptions for reconversions
	typelib_TypeDescription ** ppTempParamTypeDescr = (typelib_TypeDescription **)(pCppArgs + (2 * nParams));
	
	sal_Int32 nTempIndizes   = 0;
	
	for ( sal_Int32 nPos = 0; nPos < nParams; ++nPos )
	{
		const typelib_MethodParameter & rParam = pParams[nPos];
		typelib_TypeDescription * pParamTypeDescr = 0;
		TYPELIB_DANGER_GET( &pParamTypeDescr, rParam.pTypeRef );
		
		if (!rParam.bOut
            && bridges::cpp_uno::shared::isSimpleType( pParamTypeDescr ))
		{
			uno_copyAndConvertData( pCppArgs[nPos] = pCppStack, pUnoArgs[nPos], pParamTypeDescr,
									pThis->getBridge()->getUno2Cpp() );
			
			switch (pParamTypeDescr->eTypeClass)
			{
			case typelib_TypeClass_HYPER:
			case typelib_TypeClass_UNSIGNED_HYPER:
			case typelib_TypeClass_DOUBLE:
				pCppStack += sizeof(sal_Int32); // extra long
                break;
            default:
                break;
			}
			// no longer needed
			TYPELIB_DANGER_RELEASE( pParamTypeDescr );
		}
		else // ptr to complex value | ref
		{
			if (! rParam.bIn) // is pure out
			{
				// cpp out is constructed mem, uno out is not!
				uno_constructData(
#ifdef BROKEN_ALLOCA
					*(void **)pCppStack = pCppArgs[nPos] = malloc( pParamTypeDescr->nSize ),
#else
					*(void **)pCppStack = pCppArgs[nPos] = alloca( pParamTypeDescr->nSize ),
#endif
					pParamTypeDescr );
				pTempIndizes[nTempIndizes] = nPos; // default constructed for cpp call
				// will be released at reconversion
				ppTempParamTypeDescr[nTempIndizes++] = pParamTypeDescr;
			}
			// is in/inout
			else if (bridges::cpp_uno::shared::relatesToInterfaceType(
                         pParamTypeDescr ))
			{
				uno_copyAndConvertData(
#ifdef BROKEN_ALLOCA
					*(void **)pCppStack = pCppArgs[nPos] = malloc( pParamTypeDescr->nSize ),
#else
					*(void **)pCppStack = pCppArgs[nPos] = alloca( pParamTypeDescr->nSize ),
#endif
					pUnoArgs[nPos], pParamTypeDescr,
                    pThis->getBridge()->getUno2Cpp() );
				
				pTempIndizes[nTempIndizes] = nPos; // has to be reconverted
				// will be released at reconversion
				ppTempParamTypeDescr[nTempIndizes++] = pParamTypeDescr;
			}
			else // direct way
			{
				*(void **)pCppStack = pCppArgs[nPos] = pUnoArgs[nPos];
				// no longer needed
				TYPELIB_DANGER_RELEASE( pParamTypeDescr );
			}
		}
		pCppStack += sizeof(sal_Int32); // standard parameter length
	}

	try
	{
		OSL_ENSURE( !( (pCppStack - pCppStackStart ) & 3), "UNALIGNED STACK !!! (Please DO panic)" );
		callVirtualMethod(
			pAdjustedThisPtr, aVtableSlot.index,
			pCppReturn, pReturnTypeDescr,
			(sal_Int32 *)pCppStackStart, (pCppStack - pCppStackStart) / sizeof(sal_Int32) );
		// NO exception occurred...
		*ppUnoExc = 0;
		
		// reconvert temporary params
		for ( ; nTempIndizes--; )
		{
			sal_Int32 nIndex = pTempIndizes[nTempIndizes];
			typelib_TypeDescription * pParamTypeDescr = ppTempParamTypeDescr[nTempIndizes];
			
			if (pParams[nIndex].bIn)
			{
				if (pParams[nIndex].bOut) // inout
				{
					uno_destructData( pUnoArgs[nIndex], pParamTypeDescr, 0 ); // destroy uno value
					uno_copyAndConvertData( pUnoArgs[nIndex], pCppArgs[nIndex], pParamTypeDescr,
											pThis->getBridge()->getCpp2Uno() );
				}
			}
			else // pure out
			{
				uno_copyAndConvertData( pUnoArgs[nIndex], pCppArgs[nIndex], pParamTypeDescr,
										pThis->getBridge()->getCpp2Uno() );
			}
			// destroy temp cpp param => cpp: every param was constructed
			uno_destructData( pCppArgs[nIndex], pParamTypeDescr, cpp_release );
#ifdef BROKEN_ALLOCA
			free( pCppArgs[nIndex] );
#endif
			
			TYPELIB_DANGER_RELEASE( pParamTypeDescr );
		}
		// return value
		if (pCppReturn && pUnoReturn != pCppReturn)
		{
			uno_copyAndConvertData( pUnoReturn, pCppReturn, pReturnTypeDescr,
									pThis->getBridge()->getCpp2Uno() );
			uno_destructData( pCppReturn, pReturnTypeDescr, cpp_release );
		}
	}
 	catch (...)
 	{
  		// fill uno exception
		fillUnoException( CPPU_CURRENT_NAMESPACE::__cxa_get_globals()->caughtExceptions, *ppUnoExc, pThis->getBridge()->getCpp2Uno() );
        
		// temporary params
		for ( ; nTempIndizes--; )
		{
			sal_Int32 nIndex = pTempIndizes[nTempIndizes];
			// destroy temp cpp param => cpp: every param was constructed
			uno_destructData( pCppArgs[nIndex], ppTempParamTypeDescr[nTempIndizes], cpp_release );
			TYPELIB_DANGER_RELEASE( ppTempParamTypeDescr[nTempIndizes] );
#ifdef BROKEN_ALLOCA
			free( pCppArgs[nIndex] );
#endif
		}
		// return type
		if (pReturnTypeDescr)
			TYPELIB_DANGER_RELEASE( pReturnTypeDescr );
	}
	if (pCppReturn && pUnoReturn != pCppReturn)
	{
#ifdef BROKEN_ALLOCA
		free( pCppReturn );
#endif
	}
#ifdef BROKEN_ALLOCA
	free( pCppStackStart );
#endif
}

}

namespace bridges { namespace cpp_uno { namespace shared {

void unoInterfaceProxyDispatch(
	uno_Interface * pUnoI, const typelib_TypeDescription * pMemberDescr,
    void * pReturn, void * pArgs[], uno_Any ** ppException )
{
	// is my surrogate
	bridges::cpp_uno::shared::UnoInterfaceProxy * pThis
        = static_cast< bridges::cpp_uno::shared::UnoInterfaceProxy * >(pUnoI);
	
	switch (pMemberDescr->eTypeClass)
	{
	case typelib_TypeClass_INTERFACE_ATTRIBUTE:
	{
        VtableSlot aVtableSlot(
            getVtableSlot(
                reinterpret_cast<
                    typelib_InterfaceAttributeTypeDescription const * >(
                        pMemberDescr)));
		if (pReturn)
		{
			// dependent dispatch
			cpp_call(
				pThis, aVtableSlot,
				((typelib_InterfaceAttributeTypeDescription *)pMemberDescr)->pAttributeTypeRef,
				0, 0, // no params
				pReturn, pArgs, ppException );
		}
		else
		{
			// is SET
			typelib_MethodParameter aParam;
			aParam.pTypeRef =
				((typelib_InterfaceAttributeTypeDescription *)pMemberDescr)->pAttributeTypeRef;
			aParam.bIn		= sal_True;
			aParam.bOut		= sal_False;

			typelib_TypeDescriptionReference * pReturnTypeRef = 0;
			OUString aVoidName( RTL_CONSTASCII_USTRINGPARAM("void") );
			typelib_typedescriptionreference_new(
				&pReturnTypeRef, typelib_TypeClass_VOID, aVoidName.pData );
			
			// dependent dispatch
            aVtableSlot.index += 1; // get, then set method
			cpp_call(
				pThis, aVtableSlot,
				pReturnTypeRef,
				1, &aParam,
				pReturn, pArgs, ppException );
			
			typelib_typedescriptionreference_release( pReturnTypeRef );
		}
		
		break;
	}
	case typelib_TypeClass_INTERFACE_METHOD:
	{
        VtableSlot aVtableSlot(
            getVtableSlot(
                reinterpret_cast<
                    typelib_InterfaceMethodTypeDescription const * >(
                        pMemberDescr)));
		switch (aVtableSlot.index)
		{
			// standard calls
		case 1: // acquire uno interface
			(*pUnoI->acquire)( pUnoI );
			*ppException = 0;
			break;
		case 2: // release uno interface
			(*pUnoI->release)( pUnoI );
			*ppException = 0;
			break;
		case 0: // queryInterface() opt
		{
			typelib_TypeDescription * pTD = 0;
			TYPELIB_DANGER_GET( &pTD, reinterpret_cast< Type * >( pArgs[0] )->getTypeLibType() );
			if (pTD)
			{
                uno_Interface * pInterface = 0;
                (*pThis->pBridge->getUnoEnv()->getRegisteredInterface)(
                    pThis->pBridge->getUnoEnv(),
                    (void **)&pInterface, pThis->oid.pData, (typelib_InterfaceTypeDescription *)pTD );
			
                if (pInterface)
                {
                    ::uno_any_construct(
                        reinterpret_cast< uno_Any * >( pReturn ),
                        &pInterface, pTD, 0 );
                    (*pInterface->release)( pInterface );
                    TYPELIB_DANGER_RELEASE( pTD );
                    *ppException = 0;
                    break;
                }
                TYPELIB_DANGER_RELEASE( pTD );
            }
		} // else perform queryInterface()
		default:
			// dependent dispatch
			cpp_call(
				pThis, aVtableSlot,
				((typelib_InterfaceMethodTypeDescription *)pMemberDescr)->pReturnTypeRef,
				((typelib_InterfaceMethodTypeDescription *)pMemberDescr)->nParams,
				((typelib_InterfaceMethodTypeDescription *)pMemberDescr)->pParams,
				pReturn, pArgs, ppException );
		}
		break;
	}
	default:
	{
		::com::sun::star::uno::RuntimeException aExc(
			OUString( RTL_CONSTASCII_USTRINGPARAM("illegal member type description!") ),
			::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >() );
		
		Type const & rExcType = ::getCppuType( &aExc );
		// binary identical null reference
		::uno_type_any_construct( *ppException, &aExc, rExcType.getTypeLibType(), 0 );
	}
	}
}

} } }
