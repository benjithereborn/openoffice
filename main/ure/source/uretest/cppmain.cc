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



#include "sal/config.h"

#include <cstddef>
#include <functional>
#include <memory>
#include <new>
#include <hash_map>

#include "com/sun/star/lang/XMain.hpp"
#include "com/sun/star/lang/XMultiComponentFactory.hpp"
#include "com/sun/star/uno/Exception.hpp"
#include "com/sun/star/uno/Reference.hxx"
#include "com/sun/star/uno/RuntimeException.hpp"
#include "com/sun/star/uno/Sequence.hxx"
#include "com/sun/star/uno/XComponentContext.hpp"
#include "com/sun/star/uno/XInterface.hpp"
#include "cppuhelper/factory.hxx"
#include "cppuhelper/implbase1.hxx"
#include "cppuhelper/implementationentry.hxx"
#include "cppuhelper/interfacecontainer.hxx"
#include "cppuhelper/unourl.hxx"
#include "cppuhelper/weak.hxx"
#include "osl/mutex.hxx"
#include "osl/thread.h"
#include "rtl/malformeduriexception.hxx"
#include "rtl/string.h"
#include "rtl/ustrbuf.hxx"
#include "rtl/ustring.h"
#include "rtl/ustring.hxx"
#include "sal/types.h"
#include "salhelper/simplereferenceobject.hxx"
#include "uno/current_context.hxx"
#include "uno/environment.h"
#include "uno/lbnames.h"

#include "test/types/CppTest.hpp"
#include "test/types/JavaTest.hpp"
#include "test/types/TestException.hpp"
#include "test/types/XTest.hpp"

namespace css = ::com::sun::star;

namespace {

class Service: public ::cppu::WeakImplHelper1< ::css::lang::XMain > {
public:
    explicit Service(
        ::css::uno::Reference< ::css::uno::XComponentContext > const & context):
        context_(context) {}

    virtual ::sal_Int32 SAL_CALL run(
        ::css::uno::Sequence< ::rtl::OUString > const &)
        throw (::css::uno::RuntimeException);

private:
    Service(Service &); // not defined
    void operator =(Service &); // not defined

    virtual ~Service() {}

    void test(
        ::css::uno::Reference< test::types::XTest > const & test,
        ::rtl::OUString const & name);

    ::css::uno::Reference< ::css::uno::XComponentContext > context_;
};

::sal_Int32 Service::run(::css::uno::Sequence< ::rtl::OUString > const &)
    throw (::css::uno::RuntimeException)
{
    osl_getThreadIdentifier(0); // check for sal
    (new salhelper::SimpleReferenceObject)->release(); // check for salhelper
    css::uno::getCurrentContext(); // check for cppu
    try { // check for cppuhelper
        std::auto_ptr< cppu::UnoUrl > dummy(new cppu::UnoUrl(rtl::OUString()));
    } catch (rtl::MalformedUriException &) {}
    { // check for stlport
        osl::Mutex m;
        std::auto_ptr< cppu::OMultiTypeInterfaceContainerHelperVar<
            int, std::hash< int >, std::equal_to< int > > > dummy(
                new cppu::OMultiTypeInterfaceContainerHelperVar<
                int, std::hash< int >, std::equal_to< int > >(m));
    }
    static char const * const services[] = {
        "com.sun.star.beans.Introspection",
        "com.sun.star.bridge.Bridge",
        "com.sun.star.bridge.BridgeFactory",
        "com.sun.star.bridge.IiopBridge",
        "com.sun.star.bridge.UnoUrlResolver",
        "com.sun.star.bridge.UrpBridge",
        "com.sun.star.connection.Acceptor",
        "com.sun.star.connection.Connector",
        "com.sun.star.io.DataInputStream",
        "com.sun.star.io.DataOutputStream",
        "com.sun.star.io.MarkableInputStream",
        "com.sun.star.io.MarkableOutputStream",
        "com.sun.star.io.ObjectInputStream",
        "com.sun.star.io.ObjectOutputStream",
        "com.sun.star.io.Pipe",
        "com.sun.star.io.Pump",
        "com.sun.star.io.TextInputStream",
        "com.sun.star.io.TextOutputStream",
        "com.sun.star.java.JavaVirtualMachine",
        "com.sun.star.lang.MultiServiceFactory",
        "com.sun.star.lang.RegistryServiceManager",
        "com.sun.star.lang.ServiceManager",
        "com.sun.star.loader.Java",
        "com.sun.star.loader.Java2",
        "com.sun.star.loader.SharedLibrary",
        "com.sun.star.reflection.CoreReflection",
        "com.sun.star.reflection.ProxyFactory",
        "com.sun.star.reflection.TypeDescriptionManager",
        "com.sun.star.reflection.TypeDescriptionProvider",
        "com.sun.star.registry.ImplementationRegistration",
        "com.sun.star.registry.NestedRegistry",
        "com.sun.star.registry.SimpleRegistry",
        "com.sun.star.script.Converter",
        "com.sun.star.script.Invocation",
        "com.sun.star.script.InvocationAdapterFactory",
        "com.sun.star.security.AccessController",
        "com.sun.star.security.Policy",
        "com.sun.star.uno.NamingService",
        "com.sun.star.uri.ExternalUriReferenceTranslator",
        "com.sun.star.uri.UriReferenceFactory",
        "com.sun.star.uri.UriSchemeParser_vndDOTsunDOTstarDOTscript",
        "com.sun.star.uri.VndSunStarPkgUrlReferenceFactory"
        // "com.sun.star.beans.PropertyBag",
        // "com.sun.star.beans.PropertySet",
        // "com.sun.star.bridge.OleApplicationRegistration",
        // "com.sun.star.bridge.OleBridgeSupplier",
        // "com.sun.star.bridge.OleBridgeSupplier2",
        // "com.sun.star.bridge.OleBridgeSupplierVar1",
        // "com.sun.star.bridge.OleObjectFactory",
        // "com.sun.star.bridge.oleautomation.ApplicationRegistration",
        // "com.sun.star.bridge.oleautomation.BridgeSupplier",
        // "com.sun.star.bridge.oleautomation.Factory",
        // "com.sun.star.loader.Dynamic",
        // "com.sun.star.registry.DefaultRegistry",
        // "com.sun.star.script.AllListenerAdapter",
        // "com.sun.star.script.Engine",
        // "com.sun.star.script.JavaScript",
        // "com.sun.star.test.TestFactory",
        // "com.sun.star.util.BootstrapMacroExpander",
        // "com.sun.star.util.MacroExpander",
        // "com.sun.star.util.logging.Logger",
        // "com.sun.star.util.logging.LoggerRemote"
    };
    ::css::uno::Reference< ::css::lang::XMultiComponentFactory > manager(
        context_->getServiceManager());
    if (!manager.is()) {
        throw ::css::uno::RuntimeException(
            ::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("no service manager")),
            static_cast< ::cppu::OWeakObject * >(this));
    }
    for (::std::size_t i = 0; i < sizeof services / sizeof services[0]; ++i) {
        ::css::uno::Reference< ::css::uno::XInterface > instance;
        try {
            instance = manager->createInstanceWithContext(
                ::rtl::OUString::createFromAscii(services[i]), context_);
        } catch (::css::uno::RuntimeException &) {
            throw;
        } catch (::css::uno::Exception &) {
            throw ::css::uno::RuntimeException(
                ::rtl::OUString(
                    RTL_CONSTASCII_USTRINGPARAM("error creating instance")),
                static_cast< ::cppu::OWeakObject * >(this));
        }
        if (!instance.is()) {
            throw ::css::uno::RuntimeException(
                ::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("no instance")),
                static_cast< ::cppu::OWeakObject * >(this));
        }
    }
    static char const * const singletons[] = {
        "com.sun.star.util.theMacroExpander" };
    for (::std::size_t i = 0; i < sizeof singletons / sizeof singletons[0]; ++i)
    {
        ::rtl::OUStringBuffer b;
        b.appendAscii(RTL_CONSTASCII_STRINGPARAM("/singletons/"));
        b.appendAscii(singletons[i]);
        ::css::uno::Reference< ::css::uno::XInterface > instance(
            context_->getValueByName(b.makeStringAndClear()),
            ::css::uno::UNO_QUERY_THROW);
    }
    test(
        ::test::types::CppTest::create(context_),
        ::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("test.types.CppTest")));
    test(
        ::test::types::JavaTest::create(context_),
        ::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("test.types.JavaTest")));
    return 0;
}

void Service::test(
    ::css::uno::Reference< test::types::XTest > const & test,
    ::rtl::OUString const & name)
{
    bool ok = false;
    try {
        test->throwException();
    } catch (::test::types::TestException &) {
        ok = true;
    }
    if (!ok) {
        throw ::css::uno::RuntimeException(
            (name
             + ::rtl::OUString(
                 RTL_CONSTASCII_USTRINGPARAM(".throwException failed"))),
            static_cast< ::cppu::OWeakObject * >(this));
    }
}

namespace CppMain {

::css::uno::Reference< ::css::uno::XInterface > create(
    ::css::uno::Reference< ::css::uno::XComponentContext > const & context)
    SAL_THROW((::css::uno::Exception))
{
    try {
        return static_cast< ::cppu::OWeakObject * >(new Service(context));
    } catch (::std::bad_alloc &) {
        throw ::css::uno::RuntimeException(
            ::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("std::bad_alloc")),
            ::css::uno::Reference< ::css::uno::XInterface >());
    }
}

::rtl::OUString getImplementationName() {
    return ::rtl::OUString(
        RTL_CONSTASCII_USTRINGPARAM("test.cpp.cppmain.Component"));
}

::css::uno::Sequence< ::rtl::OUString > getSupportedServiceNames() {
    return ::css::uno::Sequence< ::rtl::OUString >();
}

}

::cppu::ImplementationEntry entries[] = {
    { CppMain::create, CppMain::getImplementationName,
      CppMain::getSupportedServiceNames, ::cppu::createSingleComponentFactory,
      0, 0 },
    { 0, 0, 0, 0, 0, 0 } };

}

extern "C" ::sal_Bool SAL_CALL component_writeInfo(
    void * serviceManager, void * registryKey)
{
    return ::cppu::component_writeInfoHelper(
        serviceManager, registryKey, entries);
}

extern "C" void * SAL_CALL component_getFactory(
    char const * implName, void * serviceManager, void * registryKey)
{
    return ::cppu::component_getFactoryHelper(
        implName, serviceManager, registryKey, entries);
}

extern "C" void SAL_CALL component_getImplementationEnvironment(
    char const ** envTypeName, ::uno_Environment **)
{
    *envTypeName = CPPU_CURRENT_LANGUAGE_BINDING_NAME;
}
