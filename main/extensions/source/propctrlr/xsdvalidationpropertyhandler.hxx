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



#ifndef EXTENSIONS_SOURCE_PROPCTRLR_XSDVALIDATIONPROPERTYHANDLER_HXX
#define EXTENSIONS_SOURCE_PROPCTRLR_XSDVALIDATIONPROPERTYHANDLER_HXX

#include "propertyhandler.hxx"

/** === begin UNO includes === **/
/** === end UNO includes === **/

#include <memory>

//........................................................................
namespace pcr
{
//........................................................................

    class XSDValidationHelper;
	//====================================================================
	//= XSDValidationPropertyHandler
	//====================================================================
    class XSDValidationPropertyHandler;
    typedef HandlerComponentBase< XSDValidationPropertyHandler > XSDValidationPropertyHandler_Base;
    class XSDValidationPropertyHandler : public XSDValidationPropertyHandler_Base
	{
    private:
        ::std::auto_ptr< XSDValidationHelper >  m_pHelper;

    public:
        XSDValidationPropertyHandler(
            const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext >& _rxContext
        );

        static ::rtl::OUString SAL_CALL getImplementationName_static(  ) throw (::com::sun::star::uno::RuntimeException);
        static ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames_static(  ) throw (::com::sun::star::uno::RuntimeException);

    protected:
        ~XSDValidationPropertyHandler();

    protected:
        // XPropertyHandler overriables
        virtual ::com::sun::star::uno::Any  SAL_CALL getPropertyValue( const ::rtl::OUString& _rPropertyName ) throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::uno::RuntimeException);
        virtual void                        SAL_CALL setPropertyValue( const ::rtl::OUString& _rPropertyName, const ::com::sun::star::uno::Any& _rValue ) throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::uno::RuntimeException);
        virtual ::com::sun::star::uno::Sequence< ::rtl::OUString >
                                            SAL_CALL getSupersededProperties( ) throw (::com::sun::star::uno::RuntimeException);
        virtual ::com::sun::star::uno::Sequence< ::rtl::OUString >
                                            SAL_CALL getActuatingProperties( ) throw (::com::sun::star::uno::RuntimeException);
        virtual ::com::sun::star::inspection::LineDescriptor
                                            SAL_CALL describePropertyLine( const ::rtl::OUString& _rPropertyName, const ::com::sun::star::uno::Reference< ::com::sun::star::inspection::XPropertyControlFactory >& _rxControlFactory ) throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::lang::NullPointerException, ::com::sun::star::uno::RuntimeException);
        virtual ::com::sun::star::inspection::InteractiveSelectionResult
                                            SAL_CALL onInteractivePropertySelection( const ::rtl::OUString& _rPropertyName, sal_Bool _bPrimary, ::com::sun::star::uno::Any& _rData, const ::com::sun::star::uno::Reference< ::com::sun::star::inspection::XObjectInspectorUI >& _rxInspectorUI ) throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::lang::NullPointerException, ::com::sun::star::uno::RuntimeException);
        virtual void                        SAL_CALL actuatingPropertyChanged( const ::rtl::OUString& _rActuatingPropertyName, const ::com::sun::star::uno::Any& _rNewValue, const ::com::sun::star::uno::Any& _rOldValue, const ::com::sun::star::uno::Reference< ::com::sun::star::inspection::XObjectInspectorUI >& _rxInspectorUI, sal_Bool ) throw (::com::sun::star::lang::NullPointerException, ::com::sun::star::uno::RuntimeException);
        virtual void                        SAL_CALL addPropertyChangeListener( const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertyChangeListener >& _rxListener ) throw (::com::sun::star::uno::RuntimeException);
        virtual void                        SAL_CALL removePropertyChangeListener( const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertyChangeListener >& _rxListener ) throw (::com::sun::star::uno::RuntimeException);

        // PropertyHandler overridables
        virtual ::com::sun::star::uno::Sequence< ::com::sun::star::beans::Property >
                                            SAL_CALL doDescribeSupportedProperties() const;
        virtual void onNewComponent();

    private:
        bool    implPrepareRemoveCurrentDataType() SAL_THROW(());
        bool    implDoRemoveCurrentDataType() SAL_THROW(());

        bool    implPrepareCloneDataCurrentType( ::rtl::OUString& _rNewName ) SAL_THROW(());
        bool    implDoCloneCurrentDataType( const ::rtl::OUString& _rNewName ) SAL_THROW(());

        /** retrieves the names of the data types which our introspectee can be validated against
        */
        void    implGetAvailableDataTypeNames( ::std::vector< ::rtl::OUString >& /* [out] */ _rNames ) const SAL_THROW(());
	};

//........................................................................
} // namespace pcr
//........................................................................

#endif // EXTENSIONS_SOURCE_PROPCTRLR_XSDVALIDATIONPROPERTYHANDLER_HXX

