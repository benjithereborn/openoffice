###############################################################
#  
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#  
###############################################################



WFDIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
GBUILDDIR := $(SOLARENV)/gbuild

include $(WFDIR)generated.mk
include $(GBUILDDIR)/gbuild.mk

# doctok

doctok :
	mkdir doctok

wf_SRC_doctok_Model=$(WFDIR)doctok/resources.xmi
wf_SRC_doctok_Resources_xsl=$(WFDIR)doctok/resources.xsl
wf_SRC_doctok_ResourcesImpl_xsl=$(WFDIR)doctok/resourcesimpl.xsl
wf_SRC_doctok_ResourceIds_xsl=$(WFDIR)doctok/resourceids.xsl
wf_SRC_doctok_SprmIds_xsl=$(WFDIR)doctok/sprmids.xsl
wf_SRC_doctok_ResourceTools_xsl=$(WFDIR)doctok/resourcetools.xsl

wf_GEN_doctok_SprmIds_hxx=doctok/sprmids.hxx
wf_GEN_doctok_ResourceIds_hxx=doctok/resourceids.hxx
wf_GEN_doctok_Resources_hxx=doctok/resources.hxx
wf_GEN_doctok_Resources_cxx=resources.cxx

$(wf_GEN_doctok_SprmIds_hxx) : doctok $(wf_SRC_doctok_SprmIds_xsl) $(wf_SRC_doctok_Model)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_doctok_SprmIds_xsl) $(wf_SRC_doctok_Model)) > $@

$(wf_GEN_doctok_ResourceIds_hxx) : doctok $(wf_SRC_doctok_ResourceIds_xsl) $(wf_SRC_doctok_Model)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_doctok_ResourceIds_xsl) $(wf_SRC_doctok_Model)) > $@

$(wf_GEN_doctok_Resources_hxx) : doctok $(wf_SRC_doctok_Resources_xsl) $(wf_SRC_doctok_Model)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_doctok_Resources_xsl) $(wf_SRC_doctok_Model)) > $@

$(wf_GEN_doctok_Resources_cxx) : doctok $(wf_SRC_doctok_ResourcesImpl_xsl) $(wf_SRC_doctok_Model) $(wf_SRC_doctok_ResourceTools_xsl)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_doctok_ResourcesImpl_xsl) $(wf_SRC_doctok_Model)) > $@

# ooxml

ooxml :
	mkdir ooxml

wf_SRC_ooxml_Model=$(WFDIR)ooxml/model.xml
wf_SRC_ooxml_Preprocess_xsl=$(WFDIR)ooxml/modelpreprocess.xsl
wf_SRC_ooxml_FactoryTools_xsl=$(WFDIR)ooxml/factorytools.xsl
wf_SRC_ooxml_FastTokens_xsl=$(WFDIR)ooxml/fasttokens.xsl
wf_SRC_ooxml_NamespaceIds_xsl=$(WFDIR)ooxml/namespaceids.xsl
wf_SRC_ooxml_FactoryValues_xsl=$(WFDIR)ooxml/factory_values.xsl
wf_SRC_ooxml_FactoryValuesImpl_xsl=$(WFDIR)ooxml/factoryimpl_values.xsl
wf_SRC_ooxml_ResourceIds_xsl=$(WFDIR)ooxml/resourceids.xsl
wf_SRC_ooxml_GperfFastTokenHandler_xsl=$(WFDIR)ooxml/gperffasttokenhandler.xsl
wf_SRC_ooxml_Analyze_model_xsl=$(WFDIR)ooxml/analyzemodel.xsl
wf_SRC_ooxml_Status_stage1_sh=$(WFDIR)ooxml/stage1.sh
wf_SRC_ooxml_Status_stage2_xsl=$(WFDIR)ooxml/analyzestage2.xsl
wf_SRC_ooxml_Status_stage3_xsl=$(WFDIR)ooxml/analyzestage3.xsl
wf_SRC_ooxml_Status_todo_xsl=$(WFDIR)ooxml/todo.xsl
wf_SRC_ooxml_Status_efforts_xsl=$(WFDIR)ooxml/effort.xsl

wf_GEN_ooxml_ResourceIds_hxx=ooxml/resourceids.hxx

wf_GEN_ooxml_token_xml=token.xml
wf_GEN_ooxml_token_tmp=token.tmp

wf_GEN_ooxml_Factory_hxx=OOXMLFactory_generated.hxx
wf_GEN_ooxml_Factory_cxx=OOXMLFactory_generated.cxx
wf_GEN_ooxml_FastTokens_hxx=ooxml/OOXMLFastTokens.hxx
wf_GEN_ooxml_NamespaceIds_hxx=ooxml/OOXMLnamespaceids.hxx
wf_GEN_ooxml_FactoryValues_hxx=OOXMLFactory_values.hxx
wf_GEN_ooxml_FactoryValues_cxx=OOXMLFactory_values.cxx
wf_GEN_ooxml_GperfFastToken_hxx=gperffasttoken.hxx
wf_GEN_ooxml_Model_processed=model_preprocessed.xml
wf_GEN_ooxml_Model_analyzed=ooxml/model_analyzed.xml
wf_GEN_ooxml_Status_stage1=ooxml/status_stage1.xml
wf_GEN_ooxml_Status_stage2=ooxml/status_stage2.xml
wf_GEN_ooxml_Status_stage3=ooxml/status_stage3.xml
wf_GEN_ooxml_Status_todo=ooxml/todo.xml
wf_GEN_ooxml_Status_efforts=ooxml/efforts.csv

$(wf_GEN_ooxml_token_tmp) : $(OUTDIR)/inc$(UPDMINOREXT)/oox/token/tokens.txt
	cat $(OUTDIR)/inc$(UPDMINOREXT)/oox/token/tokens.txt \
	| sed "s#\(.*\)#<fasttoken>\1</fasttoken>#" > $@

$(wf_GEN_ooxml_token_xml) : $(WFDIR)ooxml/tokenxmlheader $(wf_GEN_ooxml_token_tmp) $(WFDIR)ooxml/tokenxmlfooter
	cat $(WFDIR)ooxml/tokenxmlheader $(wf_GEN_ooxml_token_tmp) $(WFDIR)ooxml/tokenxmlfooter > $@

$(wf_GEN_ooxml_FastTokens_hxx) : $(wf_SRC_ooxml_FastTokens_xsl) $(wf_GEN_ooxml_token_xml)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_ooxml_FastTokens_xsl) $(wf_GEN_ooxml_token_xml)) > $@

$(wf_GEN_ooxml_Factory_hxx) : $(WFDIR)ooxml/factoryinc.xsl $(wf_GEN_ooxml_Model_processed)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $< $(wf_GEN_ooxml_Model_processed)) > $@

$(wf_GEN_ooxml_Factory_cxx) : $(WFDIR)ooxml/factoryimpl.xsl $(wf_GEN_ooxml_Model_processed)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $< $(wf_GEN_ooxml_Model_processed)) > $@

OOXMLFactory%.cxx : $(WFDIR)ooxml/factoryimpl_ns.xsl $(wf_GEN_ooxml_Model_processed)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) --stringparam file $@ $< $(wf_GEN_ooxml_Model_processed)) > $@

OOXMLFactory%.hxx : $(WFDIR)ooxml/factory_ns.xsl $(wf_GEN_ooxml_Model_processed)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) --stringparam file $@ $< $(wf_GEN_ooxml_Model_processed)) > $@

$(wf_GEN_ooxml_FactoryValues_hxx) : $(wf_SRC_ooxml_FactoryValues_xsl) $(wf_GEN_ooxml_Model_processed)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_ooxml_FactoryValues_xsl) $(wf_GEN_ooxml_Model_processed)) > $@

$(wf_GEN_ooxml_FactoryValues_cxx) : $(wf_SRC_ooxml_FactoryValuesImpl_xsl) $(wf_GEN_ooxml_Model_processed)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_ooxml_FactoryValuesImpl_xsl) $(wf_GEN_ooxml_Model_processed)) > $@

$(wf_GEN_ooxml_ResourceIds_hxx) : ooxml $(wf_SRC_ooxml_ResourceIds_xsl) $(wf_GEN_ooxml_Model_processed)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_ooxml_ResourceIds_xsl) $(wf_GEN_ooxml_Model_processed)) > $@

$(wf_GEN_ooxml_NamespaceIds_hxx) : $(wf_SRC_ooxml_NamespaceIds_xsl) $(wf_GEN_ooxml_Model_processed)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_ooxml_NamespaceIds_xsl) $(wf_GEN_ooxml_Model_processed)) > $@

$(wf_GEN_ooxml_GperfFastToken_hxx) : $(wf_SRC_ooxml_GperfFastTokenHandler_xsl) $(wf_GEN_ooxml_Model_processed)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_ooxml_GperfFastTokenHandler_xsl) $(wf_GEN_ooxml_Model_processed)) \
	| tr -d '\r' | gperf -I -t -E -S1 -c -G -LC++ > $@

$(wf_GEN_ooxml_Model_analyzed): $(wf_SRC_ooxml_Analyze_model_xsl) $(wf_SRC_ooxml_Model)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $^) > $@

$(wf_GEN_ooxml_Status_stage1) : $(wf_GEN_ooxml_Model_analyzed) $(wf_SRC_ooxml_stage1_sh)
	$(wf_SRC_ooxml_Status_stage1_sh) $(wf_GEN_ooxml_Model_analyzed) $(WFDIR)dmapper > $@

$(wf_GEN_ooxml_Status_stage2) : $(wf_SRC_ooxml_Status_stage2_xsl) $(wf_GEN_ooxml_Status_stage1)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $^) > $@

$(wf_GEN_ooxml_Status_stage3) : $(wf_SRC_ooxml_Status_stage3_xsl) $(wf_GEN_ooxml_Status_stage2)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $^) > $@

$(wf_GEN_ooxml_Status_todo) : $(wf_SRC_ooxml_Status_todo_xsl) $(wf_GEN_ooxml_Status_stage3)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $^) > $@

$(wf_GEN_ooxml_Status_efforts) : $(wf_SRC_ooxml_Status_efforts_xsl) $(wf_GEN_ooxml_Status_todo)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $^) > $@

# rtftok

wf_SRC_rtftok_scanner_lex=$(WFDIR)rtftok/RTFScanner.lex
wf_SRC_rtftok_scanner_skl=$(WFDIR)rtftok/RTFScanner.skl

wf_GEN_rtftok_scanner_cxx=RTFScanner.cxx

# does not build, currently unused
#$(wf_GEN_rtftok_scanner_cxx) : $(wf_SRC_rtftok_scanner_lex) $(wf_SRC_rtftok_scanner_skl)
#	flex -+ -S$(wf_SRC_rtftok_scanner_skl) -o$@ $(wf_SRC_rtftok_scanner_lex)

# resourcemodel

wf_GEN_model_QNameToStr_cxx=qnametostr.cxx
wf_GEN_ooxml_QNameToStr_tmp=OOXMLqnameToStr.tmp
wf_GEN_model_SprmCodeToStr_cxx=sprmcodetostr.cxx
wf_GEN_model_SprmCodeToStr_tmp=sprmcodetostr.tmp
wf_GEN_doctok_QnameToStr_tmp=DOCTOKqnameToStr.tmp
wf_SRC_doctok_SprmCodeToStr_xsl=$(WFDIR)doctok/sprmcodetostr.xsl
wf_SRC_doctok_QNameToStr_xsl=$(WFDIR)doctok/qnametostr.xsl
wf_SRC_ooxml_QNameToStr_xsl=$(WFDIR)ooxml/qnametostr.xsl
wf_SRC_model_NamespacePreprocess=$(WFDIR)resourcemodel/namespace_preprocess.pl
wf_GEN_ooxml_Namespacesmap_xsl=namespacesmap.xsl
wf_DEP_ooxml_namespaces_txt=$(OUTDIR)/inc$(UPDMINOREXT)/oox/token/namespaces.txt
wf_GEN_ooxml_preprocess_xsl=modelpreprocess.xsl

$(wf_GEN_ooxml_QNameToStr_tmp): $(wf_SRC_ooxml_QNameToStr_xsl) $(wf_GEN_ooxml_Model_processed)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_ooxml_QNameToStr_xsl) $(wf_GEN_ooxml_Model_processed)) > $@

$(wf_GEN_doctok_QnameToStr_tmp): $(wf_SRC_doctok_QNameToStr_xsl) $(wf_SRC_doctok_Model)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_doctok_QNameToStr_xsl) $(wf_SRC_doctok_Model)) > $@

$(wf_GEN_model_QNameToStr_cxx): $(wf_GEN_ooxml_QNameToStr_tmp) $(wf_GEN_doctok_QnameToStr_tmp) $(WFDIR)resourcemodel/qnametostrheader $(WFDIR)resourcemodel/qnametostrfooter $(wf_SRC_ooxml_FactoryTools_xsl) $(wf_SRC_doctok_ResourceTools_xsl)
	cat $(WFDIR)resourcemodel/qnametostrheader $(wf_GEN_ooxml_QNameToStr_tmp) $(wf_GEN_doctok_QnameToStr_tmp) $(WFDIR)resourcemodel/qnametostrfooter > $@

$(wf_GEN_model_SprmCodeToStr_tmp) : $(wf_SRC_doctok_SprmCodeToStr_xsl) $(wf_SRC_doctok_Model)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_SRC_doctok_SprmCodeToStr_xsl) $(wf_SRC_doctok_Model)) > $@

$(wf_GEN_model_SprmCodeToStr_cxx): $(WFDIR)resourcemodel/sprmcodetostrheader $(wf_GEN_model_SprmCodeToStr_tmp) $(WFDIR)resourcemodel/sprmcodetostrfooter
	cat $^ > $@

$(wf_GEN_ooxml_Namespacesmap_xsl) : $(wf_SRC_ooxml_Model) $(wf_DEP_ooxml_namespaces_txt) $(wf_SRC_model_NamespacePreprocess)
	$(PERL) $(wf_SRC_model_NamespacePreprocess) $(wf_DEP_ooxml_namespaces_txt) > $@

$(wf_GEN_ooxml_preprocess_xsl) : $(wf_SRC_ooxml_Preprocess_xsl)
	cp -f $(wf_SRC_ooxml_Preprocess_xsl) $@

$(wf_GEN_ooxml_Model_processed) : $(wf_GEN_ooxml_Namespacesmap_xsl) $(wf_GEN_ooxml_preprocess_xsl) $(wf_SRC_ooxml_Model)
	$(call gb_Helper_abbreviate_dirs_native, $(gb_XSLTPROC) $(wf_GEN_ooxml_Namespacesmap_xsl) $(wf_SRC_ooxml_Model)) > $@

# all

.DEFAULT_GOAL:=all
.PHONY: all
all : \
	$(wf_GEN_doctok_SprmIds_hxx) \
	$(wf_GEN_doctok_ResourceIds_hxx) \
	$(wf_GEN_doctok_Resources_hxx) \
	$(wf_GEN_doctok_Resources_cxx) \
	$(wf_GEN_ooxml_ResourceIds_hxx) \
	$(wf_GEN_ooxml_Factory_hxx) \
	$(wf_GEN_ooxml_Factory_cxx) \
	$(wf_GEN_ooxml_FactoryValues_hxx) \
	$(wf_GEN_ooxml_FactoryValues_cxx) \
	$(wf_GEN_ooxml_FastTokens_hxx) \
	$(wf_GEN_ooxml_GperfFastToken_hxx) \
	$(wf_GEN_ooxml_NamespaceIds_hxx) \
	$(wf_GEN_ooxml_Status_efforts) \
	$(wf_GEN_model_QNameToStr_cxx) \
	$(wf_GEN_model_SprmCodeToStr_cxx) \
	$(patsubst %,OOXMLFactory_%.hxx,$(WRITERFILTER_OOXMLNAMESPACES)) \
	$(patsubst %,OOXMLFactory_%.cxx,$(WRITERFILTER_OOXMLNAMESPACES)) \
