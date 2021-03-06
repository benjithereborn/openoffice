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
#include "precompiled_chart2.hxx"

#include "ConfigurationAccess.hxx"
#include "macros.hxx"

// header for class SvtSysLocale
#include <unotools/syslocale.hxx>
// header for class ConfigItem
#include <unotools/configitem.hxx>
// header for rtl::Static
#include <rtl/instance.hxx>


//.............................................................................
namespace chart
{
//.............................................................................
using namespace ::com::sun::star;

namespace
{
bool lcl_IsMetric()
{
    SvtSysLocale aSysLocale;
    const LocaleDataWrapper* pLocWrapper = aSysLocale.GetLocaleDataPtr();
    MeasurementSystem eSys = pLocWrapper->getMeasurementSystemEnum();

    return ( eSys == MEASURE_METRIC );
}
}//end anonymous namespace

// ----------------------------------------

class CalcConfigItem : public ::utl::ConfigItem
{
public:
    CalcConfigItem();
    virtual ~CalcConfigItem();

    FieldUnit getFieldUnit();
    virtual void                    Commit();
    virtual void                    Notify( const uno::Sequence<rtl::OUString>& aPropertyNames);
};

CalcConfigItem::CalcConfigItem()
    : ConfigItem( ::rtl::OUString( C2U( "Office.Calc/Layout" )))
{
}

CalcConfigItem::~CalcConfigItem()
{
}

void CalcConfigItem::Commit() {}
void CalcConfigItem::Notify( const uno::Sequence<rtl::OUString>& ) {}

FieldUnit CalcConfigItem::getFieldUnit()
{
    FieldUnit eResult( FUNIT_CM );

    uno::Sequence< ::rtl::OUString > aNames( 1 );
    if( lcl_IsMetric() )
        aNames[ 0 ] = ::rtl::OUString( C2U( "Other/MeasureUnit/Metric" ));
    else
        aNames[ 0 ] = ::rtl::OUString( C2U( "Other/MeasureUnit/NonMetric" ));

    uno::Sequence< uno::Any > aResult( GetProperties( aNames ));
    sal_Int32 nValue = 0;
    if( aResult[ 0 ] >>= nValue )
        eResult = static_cast< FieldUnit >( nValue );

    return eResult;
}

namespace
{
    //a CalcConfigItem Singleton
    struct theCalcConfigItem : public rtl::Static< CalcConfigItem, theCalcConfigItem > {};
}

namespace ConfigurationAccess
{
    FieldUnit getFieldUnit()
    {
        FieldUnit aUnit( theCalcConfigItem::get().getFieldUnit() );
        return aUnit;
    }
} //namespace ConfigurationAccess

//.............................................................................
} //namespace chart
//.............................................................................
