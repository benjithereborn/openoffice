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
#include "precompiled_svtools.hxx"

#include "SvFilterOptionsDialog.hxx"
#include <svtools/FilterConfigItem.hxx>
#include <svtools/filter.hxx>
#include "FilterConfigCache.hxx"
#include <osl/file.hxx>
#include <osl/module.hxx>
#include <svl/solar.hrc>
#include <svtools/fltcall.hxx>
#include "exportdialog.hxx"
#include <uno/mapping.hxx>
#include <com/sun/star/frame/XModel.hpp>
#include <com/sun/star/document/XViewDataSupplier.hpp>
#include <com/sun/star/container/XIndexAccess.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/uno/Sequence.h>
#include <com/sun/star/uno/Any.h>
#include <unotools/syslocale.hxx>
#include "vcl/svapp.hxx"

using namespace ::rtl;
using namespace ::com::sun::star;

// -------------------------
// - SvFilterOptionsDialog -
// -------------------------

uno::Reference< uno::XInterface >
    SAL_CALL SvFilterOptionsDialog_CreateInstance(
        const uno::Reference< lang::XMultiServiceFactory > & _rxFactory )
{
	return static_cast< ::cppu::OWeakObject* > ( new SvFilterOptionsDialog( _rxFactory ) );
}

OUString SvFilterOptionsDialog_getImplementationName()
	throw( uno::RuntimeException )
{
	return OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.comp.svtools.SvFilterOptionsDialog" ) );
}
#define SERVICE_NAME "com.sun.star.ui.dialog.FilterOptionsDialog"
sal_Bool SAL_CALL SvFilterOptionsDialog_supportsService( const OUString& ServiceName )
	throw( uno::RuntimeException )
{
    return ServiceName.equalsAsciiL( RTL_CONSTASCII_STRINGPARAM( SERVICE_NAME ) );
}

uno::Sequence< OUString > SAL_CALL SvFilterOptionsDialog_getSupportedServiceNames()
	throw( uno::RuntimeException )
{
	uno::Sequence< OUString > aRet(1);
    OUString* pArray = aRet.getArray();
    pArray[0] = OUString( RTL_CONSTASCII_USTRINGPARAM( SERVICE_NAME ) );
    return aRet;
}
#undef SERVICE_NAME

// -----------------------------------------------------------------------------

SvFilterOptionsDialog::SvFilterOptionsDialog( const uno::Reference< lang::XMultiServiceFactory > xMgr ) :
    mxMgr				( xMgr ),
	meFieldUnit			( FUNIT_CM ),
	mbExportSelection	( sal_False )
{
}

// -----------------------------------------------------------------------------

SvFilterOptionsDialog::~SvFilterOptionsDialog()
{
}

// -----------------------------------------------------------------------------

void SAL_CALL SvFilterOptionsDialog::acquire() throw()
{
	OWeakObject::acquire();
}

// -----------------------------------------------------------------------------

void SAL_CALL SvFilterOptionsDialog::release() throw()
{
	OWeakObject::release();
}

// XInitialization
void SAL_CALL SvFilterOptionsDialog::initialize( const uno::Sequence< uno::Any > & )
	throw ( uno::Exception, uno::RuntimeException )
{
}

// XServiceInfo
OUString SAL_CALL SvFilterOptionsDialog::getImplementationName()
	throw( uno::RuntimeException )
{
	return SvFilterOptionsDialog_getImplementationName();
}
sal_Bool SAL_CALL SvFilterOptionsDialog::supportsService( const OUString& rServiceName )
	throw( uno::RuntimeException )
{
    return SvFilterOptionsDialog_supportsService( rServiceName );
}
uno::Sequence< OUString > SAL_CALL SvFilterOptionsDialog::getSupportedServiceNames()
	throw ( uno::RuntimeException )
{
    return SvFilterOptionsDialog_getSupportedServiceNames();
}


// XPropertyAccess
uno::Sequence< beans::PropertyValue > SvFilterOptionsDialog::getPropertyValues()
        throw ( uno::RuntimeException )
{
	sal_Int32 i, nCount;
	for ( i = 0, nCount = maMediaDescriptor.getLength(); i < nCount; i++ )
	{
		if ( maMediaDescriptor[ i ].Name.equalsAscii( "FilterData" ) ) 
			break;
	}
	if ( i == nCount )
		maMediaDescriptor.realloc( ++nCount );

	// the "FilterData" Property is an Any that will contain our PropertySequence of Values
	maMediaDescriptor[ i ].Name = String( RTL_CONSTASCII_USTRINGPARAM( "FilterData" ) );
	maMediaDescriptor[ i ].Value <<= maFilterDataSequence;
    return maMediaDescriptor;
}

void SvFilterOptionsDialog::setPropertyValues( const uno::Sequence< beans::PropertyValue > & aProps )
        throw ( beans::UnknownPropertyException, beans::PropertyVetoException, 
                lang::IllegalArgumentException, lang::WrappedTargetException,
                uno::RuntimeException )
{
    maMediaDescriptor = aProps;

	sal_Int32 i, nCount;
	for ( i = 0, nCount = maMediaDescriptor.getLength(); i < nCount; i++ )
	{
		if ( maMediaDescriptor[ i ].Name.equalsAscii( "FilterData" ) ) 
		{
			maMediaDescriptor[ i ].Value >>= maFilterDataSequence;
		}
		else if ( maMediaDescriptor[ i ].Name.equalsAscii( "SelectionOnly" ) )
		{
			maMediaDescriptor[ i ].Value >>= mbExportSelection;
		}
	}
}

// XExecutableDialog
void SvFilterOptionsDialog::setTitle( const OUString& aTitle )
    throw ( uno::RuntimeException )
{
    maDialogTitle = aTitle;
}

sal_Int16 SvFilterOptionsDialog::execute()
	throw ( uno::RuntimeException )
{
    sal_Int16 nRet = ui::dialogs::ExecutableDialogResults::CANCEL;

    String aFilterNameStr( RTL_CONSTASCII_USTRINGPARAM( "FilterName" ) );
    String aInternalFilterName;
    sal_Int32 j, nCount = maMediaDescriptor.getLength();
    for ( j = 0; j < nCount; j++ )
    {
        if ( maMediaDescriptor[ j ].Name.equals( aFilterNameStr ) )
        {
            OUString aStr;
            maMediaDescriptor[ j ].Value >>= aStr;
			aInternalFilterName = aStr;
			aInternalFilterName.SearchAndReplace( String( RTL_CONSTASCII_USTRINGPARAM( "draw_" ) ), String(), 0 );
			aInternalFilterName.SearchAndReplace( String( RTL_CONSTASCII_USTRINGPARAM( "impress_" ) ), String(), 0 );
            break;
       }
    }   
    if ( aInternalFilterName.Len() )
    {
	    GraphicFilter aGraphicFilter( sal_True );

        sal_uInt16 nFormat, nFilterCount = aGraphicFilter.pConfig->GetExportFormatCount();
        for ( nFormat = 0; nFormat < nFilterCount; nFormat++ )
        {        
            if ( aGraphicFilter.pConfig->GetExportInternalFilterName( nFormat ) == aInternalFilterName )
                break;
        }
        if ( nFormat < nFilterCount )
        {
			FltCallDialogParameter aFltCallDlgPara( Application::GetDefDialogParent(), NULL, meFieldUnit );
			aFltCallDlgPara.aFilterData = maFilterDataSequence;

			ByteString	aResMgrName( "svt", 3 );
	        ResMgr*		pResMgr;

	        pResMgr = ResMgr::CreateResMgr( aResMgrName.GetBuffer(), Application::GetSettings().GetUILocale() );
			aFltCallDlgPara.pResMgr = pResMgr;

			aFltCallDlgPara.aFilterExt = aGraphicFilter.pConfig->GetExportFormatShortName( nFormat );
			sal_Bool bIsPixelFormat( aGraphicFilter.pConfig->IsExportPixelFormat( nFormat ) );
			if ( ExportDialog( aFltCallDlgPara, mxMgr, mxSourceDocument, mbExportSelection, bIsPixelFormat ).Execute() == RET_OK )
		        nRet = ui::dialogs::ExecutableDialogResults::OK;

			delete pResMgr;

			// taking the out parameter from the dialog
			maFilterDataSequence = aFltCallDlgPara.aFilterData;
        }
    }
	return nRet;
}

// XEmporter
void SvFilterOptionsDialog::setSourceDocument( const uno::Reference< lang::XComponent >& xDoc )
		throw ( lang::IllegalArgumentException, uno::RuntimeException )
{
	mxSourceDocument = xDoc;

	// try to set the corresponding metric unit
	String aConfigPath;
    uno::Reference< lang::XServiceInfo > xServiceInfo
            ( xDoc, uno::UNO_QUERY );
	if ( xServiceInfo.is() )
	{
		if ( xServiceInfo->supportsService( OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.presentation.PresentationDocument" ) ) ) )
			aConfigPath = String( RTL_CONSTASCII_USTRINGPARAM( "Office.Impress/Layout/Other/MeasureUnit" ) );
		else if ( xServiceInfo->supportsService( OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.drawing.DrawingDocument" ) ) ) )
			aConfigPath = String( RTL_CONSTASCII_USTRINGPARAM( "Office.Draw/Layout/Other/MeasureUnit" ) );
		if ( aConfigPath.Len() )
		{
			FilterConfigItem aConfigItem( aConfigPath );
			String aPropertyName;
			SvtSysLocale aSysLocale;
			if ( aSysLocale.GetLocaleDataPtr()->getMeasurementSystemEnum() == MEASURE_METRIC )
				aPropertyName = String( RTL_CONSTASCII_USTRINGPARAM( "Metric" ) );
			else
				aPropertyName = String( RTL_CONSTASCII_USTRINGPARAM( "NonMetric" ) );
			meFieldUnit = (FieldUnit)aConfigItem.ReadInt32( aPropertyName, FUNIT_CM );
		}
	}
}

