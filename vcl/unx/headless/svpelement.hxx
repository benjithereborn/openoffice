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



#ifndef _SVP_SVPELEMENT_HXX
#define _SVP_SVPELEMENT_HXX

#include <basebmp/bitmapdevice.hxx>

#define SVP_DEFAULT_BITMAP_FORMAT basebmp::Format::TWENTYFOUR_BIT_TC_MASK

class SvpElement
{
    protected:
    SvpElement();
    virtual ~SvpElement();
    public:    
    virtual const basebmp::BitmapDeviceSharedPtr& getDevice() const = 0;

    static sal_uInt32 getBitCountFromScanlineFormat( sal_Int32 nFormat );
};

#endif
