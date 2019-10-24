using System;
using System.Collections;
using System.Collections.Specialized;

namespace Solimar
{
	namespace Licensing
	{
		namespace Attribs
		{
			public class Lic_PackageAttribs : LicensingAttribsBase
			{
				public static string ClassName = "L_PkgA";
				
				public Lic_PackageAttribs() : 
					base(ClassName)
				{
					;
				}
				
				[FlagsAttribute]
				public enum TLic_ProductID : uint
				{
					pid_SPD = 0,
					pid_XImage = 1,
					pid_XImageNT = 2,
					pid_SolPCL = 3,
					pid_NotAvailable1 = 4,
					pid_IndexPlugin = 5,
					pid_Connect = 6,
					pid_Iconvert = 7,
					pid_SolsearcherEnt = 8,
					pid_SolScript = 9,
					pid_SolIndexer = 10,
					pid_Quantum = 11,
					pid_Rubika = 12,
					pid_SolFusion = 13,
					pid_Spde = 14,
					pid_SolsearcherSp = 15,
					pid_SOLitrack = 16,
					pid_RubikaProcessBuilder = 17,
					pid_SdxDesigner = 18,
					pid_SpdeQueueManager = 19,
					pid_LibraryServices = 20,
					pid_XIMAGEpdf = 21,
					pid_SolimarUtilities = 22,
					pid_RubikaPLP = 23,
					pid_SolimarIndexingToolsPLP = 24,
					pid_ReadyPDFServer = 25,
					pid_IPPServer = 26,
					pid_TestDevIconvert = 107,
					pid_TestDevSolsearcherEp = 108,
					pid_TestDevSolScript = 109,
					pid_TestDevSolIndexer = 110,
					pid_TestDevRubika = 112,
					pid_TestDevSolfusionSp = 113,
					pid_TestDevSpde = 114,
					pid_TestDevSseSp = 115,
					pid_TestDevSOLitrack = 116,
					pid_TestDevRubikaProcessBuilder = 117,
					pid_TestDevSdxDesigner = 118,
					pid_TestDevSpdeQueueManager = 119,
					pid_TestDevLibraryServices = 120,
					pid_TestDevXIMAGEpdf = 121,
					pid_TestDevSolimarUtilities = 122,
					pid_TestDevRubikaPLP = 123,
					pid_TestDevSolimarIndexingToolsPLP = 124,
					pid_TestDevReadyPDFServer = 125,
					pid_TestDevIPPServer = 126,
					pid_UnknownProduct = 0xFFFF,
				};
				
				public class AttribsMemberEnum_TLic_ProductID : AttribsMemberEnum
				{
					public AttribsMemberEnum_TLic_ProductID(string keyName, TLic_ProductID defaultVal) :
						base(keyName, defaultVal, typeof(TLic_ProductID))
					{
						;
					}
					
					protected static SortedList m_MapAliasToEnum;
					protected static SortedList m_MapEnumToAlias;
					protected static SortedList m_MapOrderingIndexToAlias;
					protected static SortedList m_MapAliasToIndex;
					protected static SortedList m_MapEnumToIndex;
					static AttribsMemberEnum_TLic_ProductID()
					{
						m_MapAliasToEnum = new SortedList();
						m_MapEnumToAlias = new SortedList();
						m_MapOrderingIndexToAlias = new SortedList();	// map of the ordering indexes from the xml file to aliases. The ordering indexes are not guranteed to be continuous or 0 based. 
						m_MapAliasToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
						m_MapEnumToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
						m_MapAliasToEnum.Add("SPD_PRODUCT",TLic_ProductID.pid_SPD);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_SPD,"SPD_PRODUCT");
						m_MapOrderingIndexToAlias.Add(1,"SPD_PRODUCT");
						m_MapAliasToEnum.Add("XIMAGE_PRODUCT",TLic_ProductID.pid_XImage);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_XImage,"XIMAGE_PRODUCT");
						m_MapOrderingIndexToAlias.Add(2,"XIMAGE_PRODUCT");
						m_MapAliasToEnum.Add("XIMAGENT_PRODUCT",TLic_ProductID.pid_XImageNT);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_XImageNT,"XIMAGENT_PRODUCT");
						m_MapOrderingIndexToAlias.Add(3,"XIMAGENT_PRODUCT");
						m_MapAliasToEnum.Add("SOLPCLNT_PRODUCT",TLic_ProductID.pid_SolPCL);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_SolPCL,"SOLPCLNT_PRODUCT");
						m_MapOrderingIndexToAlias.Add(4,"SOLPCLNT_PRODUCT");
						m_MapAliasToEnum.Add("NOT_AVAILABLE",TLic_ProductID.pid_NotAvailable1);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_NotAvailable1,"NOT_AVAILABLE");
						m_MapOrderingIndexToAlias.Add(5,"NOT_AVAILABLE");
						m_MapAliasToEnum.Add("INDEX_PLUGIN",TLic_ProductID.pid_IndexPlugin);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_IndexPlugin,"INDEX_PLUGIN");
						m_MapOrderingIndexToAlias.Add(6,"INDEX_PLUGIN");
						m_MapAliasToEnum.Add("CONNECT_PRODUCT",TLic_ProductID.pid_Connect);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_Connect,"CONNECT_PRODUCT");
						m_MapOrderingIndexToAlias.Add(7,"CONNECT_PRODUCT");
						m_MapAliasToEnum.Add("ICONVERT_PRODUCT",TLic_ProductID.pid_Iconvert);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_Iconvert,"ICONVERT_PRODUCT");
						m_MapOrderingIndexToAlias.Add(8,"ICONVERT_PRODUCT");
						m_MapAliasToEnum.Add("SOLSEARCHER_ENTERPRISE_PRODUCT",TLic_ProductID.pid_SolsearcherEnt);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_SolsearcherEnt,"SOLSEARCHER_ENTERPRISE_PRODUCT");
						m_MapOrderingIndexToAlias.Add(9,"SOLSEARCHER_ENTERPRISE_PRODUCT");
						m_MapAliasToEnum.Add("SOLSCRIPT_PRODUCT",TLic_ProductID.pid_SolScript);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_SolScript,"SOLSCRIPT_PRODUCT");
						m_MapOrderingIndexToAlias.Add(10,"SOLSCRIPT_PRODUCT");
						m_MapAliasToEnum.Add("SOLINDEXER_PRODUCT",TLic_ProductID.pid_SolIndexer);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_SolIndexer,"SOLINDEXER_PRODUCT");
						m_MapOrderingIndexToAlias.Add(11,"SOLINDEXER_PRODUCT");
						m_MapAliasToEnum.Add("QUANTUM_PRODUCT",TLic_ProductID.pid_Quantum);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_Quantum,"QUANTUM_PRODUCT");
						m_MapOrderingIndexToAlias.Add(12,"QUANTUM_PRODUCT");
						m_MapAliasToEnum.Add("RUBIKA_PRODUCT",TLic_ProductID.pid_Rubika);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_Rubika,"RUBIKA_PRODUCT");
						m_MapOrderingIndexToAlias.Add(13,"RUBIKA_PRODUCT");
						m_MapAliasToEnum.Add("SOLFUSION_PRODUCT",TLic_ProductID.pid_SolFusion);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_SolFusion,"SOLFUSION_PRODUCT");
						m_MapOrderingIndexToAlias.Add(14,"SOLFUSION_PRODUCT");
						m_MapAliasToEnum.Add("SPDE_PRODUCT",TLic_ProductID.pid_Spde);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_Spde,"SPDE_PRODUCT");
						m_MapOrderingIndexToAlias.Add(15,"SPDE_PRODUCT");
						m_MapAliasToEnum.Add("SSE_SP_PRODUCT",TLic_ProductID.pid_SolsearcherSp);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_SolsearcherSp,"SSE_SP_PRODUCT");
						m_MapOrderingIndexToAlias.Add(16,"SSE_SP_PRODUCT");
						m_MapAliasToEnum.Add("SOLITRACK_PRODUCT",TLic_ProductID.pid_SOLitrack);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_SOLitrack,"SOLITRACK_PRODUCT");
						m_MapOrderingIndexToAlias.Add(17,"SOLITRACK_PRODUCT");
						m_MapAliasToEnum.Add("RUBIKA_PROCESS_BUILDER_PRODUCT",TLic_ProductID.pid_RubikaProcessBuilder);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_RubikaProcessBuilder,"RUBIKA_PROCESS_BUILDER_PRODUCT");
						m_MapOrderingIndexToAlias.Add(18,"RUBIKA_PROCESS_BUILDER_PRODUCT");
						m_MapAliasToEnum.Add("SDX_DESIGNER_PRODUCT",TLic_ProductID.pid_SdxDesigner);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_SdxDesigner,"SDX_DESIGNER_PRODUCT");
						m_MapOrderingIndexToAlias.Add(19,"SDX_DESIGNER_PRODUCT");
						m_MapAliasToEnum.Add("SPDE_QUEUEMANAGER_PRODUCT",TLic_ProductID.pid_SpdeQueueManager);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_SpdeQueueManager,"SPDE_QUEUEMANAGER_PRODUCT");
						m_MapOrderingIndexToAlias.Add(20,"SPDE_QUEUEMANAGER_PRODUCT");
						m_MapAliasToEnum.Add("LIBRARY_SERVICES_PRODUCT",TLic_ProductID.pid_LibraryServices);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_LibraryServices,"LIBRARY_SERVICES_PRODUCT");
						m_MapOrderingIndexToAlias.Add(21,"LIBRARY_SERVICES_PRODUCT");
						m_MapAliasToEnum.Add("XIMAGEPDF_PRODUCT",TLic_ProductID.pid_XIMAGEpdf);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_XIMAGEpdf,"XIMAGEPDF_PRODUCT");
						m_MapOrderingIndexToAlias.Add(22,"XIMAGEPDF_PRODUCT");
						m_MapAliasToEnum.Add("SOLIMARUTILITIES_PRODUCT",TLic_ProductID.pid_SolimarUtilities);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_SolimarUtilities,"SOLIMARUTILITIES_PRODUCT");
						m_MapOrderingIndexToAlias.Add(23,"SOLIMARUTILITIES_PRODUCT");
						m_MapAliasToEnum.Add("RUBIKA_PLP_PRODUCT",TLic_ProductID.pid_RubikaPLP);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_RubikaPLP,"RUBIKA_PLP_PRODUCT");
						m_MapOrderingIndexToAlias.Add(24,"RUBIKA_PLP_PRODUCT");
						m_MapAliasToEnum.Add("SOLIMAR_INDEXING_TOOLS_PLP_PRODUCT",TLic_ProductID.pid_SolimarIndexingToolsPLP);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_SolimarIndexingToolsPLP,"SOLIMAR_INDEXING_TOOLS_PLP_PRODUCT");
						m_MapOrderingIndexToAlias.Add(25,"SOLIMAR_INDEXING_TOOLS_PLP_PRODUCT");
						m_MapAliasToEnum.Add("READYPDF_PRODUCT",TLic_ProductID.pid_ReadyPDFServer);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_ReadyPDFServer,"READYPDF_PRODUCT");
						m_MapOrderingIndexToAlias.Add(26,"READYPDF_PRODUCT");
						m_MapAliasToEnum.Add("IPPSERVER_PRODUCT",TLic_ProductID.pid_IPPServer);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_IPPServer,"IPPSERVER_PRODUCT");
						m_MapOrderingIndexToAlias.Add(27,"IPPSERVER_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_ICONVERT_PRODUCT",TLic_ProductID.pid_TestDevIconvert);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevIconvert,"TEST_DEV_ICONVERT_PRODUCT");
						m_MapOrderingIndexToAlias.Add(28,"TEST_DEV_ICONVERT_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_SOLSEARCHER_ENTERPRISE_PRODUCT",TLic_ProductID.pid_TestDevSolsearcherEp);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevSolsearcherEp,"TEST_DEV_SOLSEARCHER_ENTERPRISE_PRODUCT");
						m_MapOrderingIndexToAlias.Add(29,"TEST_DEV_SOLSEARCHER_ENTERPRISE_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_SOLSCRIPT_PRODUCT",TLic_ProductID.pid_TestDevSolScript);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevSolScript,"TEST_DEV_SOLSCRIPT_PRODUCT");
						m_MapOrderingIndexToAlias.Add(30,"TEST_DEV_SOLSCRIPT_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_SOLINDEXER_PRODUCT",TLic_ProductID.pid_TestDevSolIndexer);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevSolIndexer,"TEST_DEV_SOLINDEXER_PRODUCT");
						m_MapOrderingIndexToAlias.Add(31,"TEST_DEV_SOLINDEXER_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_RUBIKA_PRODUCT",TLic_ProductID.pid_TestDevRubika);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevRubika,"TEST_DEV_RUBIKA_PRODUCT");
						m_MapOrderingIndexToAlias.Add(32,"TEST_DEV_RUBIKA_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_SOLFUSION_SP_PRODUCT",TLic_ProductID.pid_TestDevSolfusionSp);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevSolfusionSp,"TEST_DEV_SOLFUSION_SP_PRODUCT");
						m_MapOrderingIndexToAlias.Add(33,"TEST_DEV_SOLFUSION_SP_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_SPDE_PRODUCT",TLic_ProductID.pid_TestDevSpde);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevSpde,"TEST_DEV_SPDE_PRODUCT");
						m_MapOrderingIndexToAlias.Add(34,"TEST_DEV_SPDE_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_SSE_SP_PRODUCT",TLic_ProductID.pid_TestDevSseSp);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevSseSp,"TEST_DEV_SSE_SP_PRODUCT");
						m_MapOrderingIndexToAlias.Add(35,"TEST_DEV_SSE_SP_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_SOLITRACK_PRODUCT",TLic_ProductID.pid_TestDevSOLitrack);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevSOLitrack,"TEST_DEV_SOLITRACK_PRODUCT");
						m_MapOrderingIndexToAlias.Add(36,"TEST_DEV_SOLITRACK_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_RUBIKA_PROCESS_BUILDER_PRODUCT",TLic_ProductID.pid_TestDevRubikaProcessBuilder);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevRubikaProcessBuilder,"TEST_DEV_RUBIKA_PROCESS_BUILDER_PRODUCT");
						m_MapOrderingIndexToAlias.Add(37,"TEST_DEV_RUBIKA_PROCESS_BUILDER_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_SDX_DESIGNER_PRODUCT",TLic_ProductID.pid_TestDevSdxDesigner);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevSdxDesigner,"TEST_DEV_SDX_DESIGNER_PRODUCT");
						m_MapOrderingIndexToAlias.Add(38,"TEST_DEV_SDX_DESIGNER_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_SPDE_QUEUEMANAGER_PRODUCT",TLic_ProductID.pid_TestDevSpdeQueueManager);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevSpdeQueueManager,"TEST_DEV_SPDE_QUEUEMANAGER_PRODUCT");
						m_MapOrderingIndexToAlias.Add(39,"TEST_DEV_SPDE_QUEUEMANAGER_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_LIBRARY_SERVICES_PRODUCT",TLic_ProductID.pid_TestDevLibraryServices);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevLibraryServices,"TEST_DEV_LIBRARY_SERVICES_PRODUCT");
						m_MapOrderingIndexToAlias.Add(40,"TEST_DEV_LIBRARY_SERVICES_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_XIMAGEPDF_PRODUCT",TLic_ProductID.pid_TestDevXIMAGEpdf);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevXIMAGEpdf,"TEST_DEV_XIMAGEPDF_PRODUCT");
						m_MapOrderingIndexToAlias.Add(41,"TEST_DEV_XIMAGEPDF_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_SOLIMARUTILITIES_PRODUCT",TLic_ProductID.pid_TestDevSolimarUtilities);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevSolimarUtilities,"TEST_DEV_SOLIMARUTILITIES_PRODUCT");
						m_MapOrderingIndexToAlias.Add(42,"TEST_DEV_SOLIMARUTILITIES_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_RUBIKA_PLP_PRODUCT",TLic_ProductID.pid_TestDevRubikaPLP);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevRubikaPLP,"TEST_DEV_RUBIKA_PLP_PRODUCT");
						m_MapOrderingIndexToAlias.Add(43,"TEST_DEV_RUBIKA_PLP_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_SOLIMAR_INDEXING_TOOLS_PLP_PRODUCT",TLic_ProductID.pid_TestDevSolimarIndexingToolsPLP);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevSolimarIndexingToolsPLP,"TEST_DEV_SOLIMAR_INDEXING_TOOLS_PLP_PRODUCT");
						m_MapOrderingIndexToAlias.Add(44,"TEST_DEV_SOLIMAR_INDEXING_TOOLS_PLP_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_READYPDF_PRODUCT",TLic_ProductID.pid_TestDevReadyPDFServer);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevReadyPDFServer,"TEST_DEV_READYPDF_PRODUCT");
						m_MapOrderingIndexToAlias.Add(45,"TEST_DEV_READYPDF_PRODUCT");
						m_MapAliasToEnum.Add("TEST_DEV_IPPSERVER_PRODUCT",TLic_ProductID.pid_TestDevIPPServer);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_TestDevIPPServer,"TEST_DEV_IPPSERVER_PRODUCT");
						m_MapOrderingIndexToAlias.Add(46,"TEST_DEV_IPPSERVER_PRODUCT");
						m_MapAliasToEnum.Add("UNKNOWN_PRODUCT",TLic_ProductID.pid_UnknownProduct);
						m_MapEnumToAlias.Add(TLic_ProductID.pid_UnknownProduct,"UNKNOWN_PRODUCT");
						m_MapOrderingIndexToAlias.Add(47,"UNKNOWN_PRODUCT");
						m_MapAliasToIndex.Add("SPD_PRODUCT",0);
						m_MapAliasToIndex.Add("XIMAGE_PRODUCT",1);
						m_MapAliasToIndex.Add("XIMAGENT_PRODUCT",2);
						m_MapAliasToIndex.Add("SOLPCLNT_PRODUCT",3);
						m_MapAliasToIndex.Add("NOT_AVAILABLE",4);
						m_MapAliasToIndex.Add("INDEX_PLUGIN",5);
						m_MapAliasToIndex.Add("CONNECT_PRODUCT",6);
						m_MapAliasToIndex.Add("ICONVERT_PRODUCT",7);
						m_MapAliasToIndex.Add("SOLSEARCHER_ENTERPRISE_PRODUCT",8);
						m_MapAliasToIndex.Add("SOLSCRIPT_PRODUCT",9);
						m_MapAliasToIndex.Add("SOLINDEXER_PRODUCT",10);
						m_MapAliasToIndex.Add("QUANTUM_PRODUCT",11);
						m_MapAliasToIndex.Add("RUBIKA_PRODUCT",12);
						m_MapAliasToIndex.Add("SOLFUSION_PRODUCT",13);
						m_MapAliasToIndex.Add("SPDE_PRODUCT",14);
						m_MapAliasToIndex.Add("SSE_SP_PRODUCT",15);
						m_MapAliasToIndex.Add("SOLITRACK_PRODUCT",16);
						m_MapAliasToIndex.Add("RUBIKA_PROCESS_BUILDER_PRODUCT",17);
						m_MapAliasToIndex.Add("SDX_DESIGNER_PRODUCT",18);
						m_MapAliasToIndex.Add("SPDE_QUEUEMANAGER_PRODUCT",19);
						m_MapAliasToIndex.Add("LIBRARY_SERVICES_PRODUCT",20);
						m_MapAliasToIndex.Add("XIMAGEPDF_PRODUCT",21);
						m_MapAliasToIndex.Add("SOLIMARUTILITIES_PRODUCT",22);
						m_MapAliasToIndex.Add("RUBIKA_PLP_PRODUCT",23);
						m_MapAliasToIndex.Add("SOLIMAR_INDEXING_TOOLS_PLP_PRODUCT",24);
						m_MapAliasToIndex.Add("READYPDF_PRODUCT",25);
						m_MapAliasToIndex.Add("IPPSERVER_PRODUCT",26);
						m_MapAliasToIndex.Add("TEST_DEV_ICONVERT_PRODUCT",27);
						m_MapAliasToIndex.Add("TEST_DEV_SOLSEARCHER_ENTERPRISE_PRODUCT",28);
						m_MapAliasToIndex.Add("TEST_DEV_SOLSCRIPT_PRODUCT",29);
						m_MapAliasToIndex.Add("TEST_DEV_SOLINDEXER_PRODUCT",30);
						m_MapAliasToIndex.Add("TEST_DEV_RUBIKA_PRODUCT",31);
						m_MapAliasToIndex.Add("TEST_DEV_SOLFUSION_SP_PRODUCT",32);
						m_MapAliasToIndex.Add("TEST_DEV_SPDE_PRODUCT",33);
						m_MapAliasToIndex.Add("TEST_DEV_SSE_SP_PRODUCT",34);
						m_MapAliasToIndex.Add("TEST_DEV_SOLITRACK_PRODUCT",35);
						m_MapAliasToIndex.Add("TEST_DEV_RUBIKA_PROCESS_BUILDER_PRODUCT",36);
						m_MapAliasToIndex.Add("TEST_DEV_SDX_DESIGNER_PRODUCT",37);
						m_MapAliasToIndex.Add("TEST_DEV_SPDE_QUEUEMANAGER_PRODUCT",38);
						m_MapAliasToIndex.Add("TEST_DEV_LIBRARY_SERVICES_PRODUCT",39);
						m_MapAliasToIndex.Add("TEST_DEV_XIMAGEPDF_PRODUCT",40);
						m_MapAliasToIndex.Add("TEST_DEV_SOLIMARUTILITIES_PRODUCT",41);
						m_MapAliasToIndex.Add("TEST_DEV_RUBIKA_PLP_PRODUCT",42);
						m_MapAliasToIndex.Add("TEST_DEV_SOLIMAR_INDEXING_TOOLS_PLP_PRODUCT",43);
						m_MapAliasToIndex.Add("TEST_DEV_READYPDF_PRODUCT",44);
						m_MapAliasToIndex.Add("TEST_DEV_IPPSERVER_PRODUCT",45);
						m_MapAliasToIndex.Add("UNKNOWN_PRODUCT",46);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_SPD,0);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_XImage,1);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_XImageNT,2);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_SolPCL,3);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_NotAvailable1,4);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_IndexPlugin,5);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_Connect,6);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_Iconvert,7);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_SolsearcherEnt,8);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_SolScript,9);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_SolIndexer,10);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_Quantum,11);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_Rubika,12);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_SolFusion,13);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_Spde,14);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_SolsearcherSp,15);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_SOLitrack,16);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_RubikaProcessBuilder,17);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_SdxDesigner,18);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_SpdeQueueManager,19);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_LibraryServices,20);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_XIMAGEpdf,21);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_SolimarUtilities,22);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_RubikaPLP,23);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_SolimarIndexingToolsPLP,24);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_ReadyPDFServer,25);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_IPPServer,26);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevIconvert,27);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevSolsearcherEp,28);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevSolScript,29);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevSolIndexer,30);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevRubika,31);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevSolfusionSp,32);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevSpde,33);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevSseSp,34);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevSOLitrack,35);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevRubikaProcessBuilder,36);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevSdxDesigner,37);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevSpdeQueueManager,38);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevLibraryServices,39);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevXIMAGEpdf,40);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevSolimarUtilities,41);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevRubikaPLP,42);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevSolimarIndexingToolsPLP,43);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevReadyPDFServer,44);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_TestDevIPPServer,45);
						m_MapEnumToIndex.Add(TLic_ProductID.pid_UnknownProduct,46);
					}
					
				
					public static string GetAlias(System.Enum enum_value)
					{
						return (string)m_MapEnumToAlias[enum_value];
					}
					public static StringCollection GetAliases()
					{
						StringCollection alias_list = new StringCollection();
						foreach (string alias in m_MapOrderingIndexToAlias.Values)
						{
							alias_list.Add(alias);
						}
						return alias_list;
					}
					public static System.Enum GetEnumValueFromAlias(string alias)
					{
						return (System.Enum)m_MapAliasToEnum[alias];
					}
					public static int GetIndexFromAlias(string alias)
					{
						return (int)m_MapAliasToIndex[alias];
					}
					public static int GetIndexFromEnum(System.Enum enum_value)
					{
						return (int)m_MapEnumToIndex[enum_value];
					}
					
					public string GetAlias()
					{
						return GetAlias(EVal);
					}
					public void SetEnumValueFromAlias(string alias)
					{
						EVal = GetEnumValueFromAlias(alias);
					}
				
					public static implicit operator TLic_ProductID(AttribsMemberEnum_TLic_ProductID t)
					{
						return (TLic_ProductID)t.m_tVal;
					}
					
					public TLic_ProductID TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
				
					/*
				Software Spec, Module Attributes
				*/
				public class Lic_ModuleSoftwareSpecAttribs : LicensingAttribsBase
				{
					public static string ClassName = "L_MSwSpA";
					
					public Lic_ModuleSoftwareSpecAttribs() : 
						base(ClassName)
					{
						;
					}
					
					
					public AttribsMemberDWORD moduleID = new AttribsMemberDWORD("mI", 0);
					public AttribsMemberString moduleName = new AttribsMemberString("mN", "");
					public AttribsMemberDWORD modUnlimitedValue = new AttribsMemberDWORD("mUV", 0);
					public AttribsMemberDWORD modulePoolModuleID = new AttribsMemberDWORD("mPMI", 0);
					public AttribsMemberDWORD moduleDefaultLicense = new AttribsMemberDWORD("mDL", 0);
					public AttribsMemberDWORD moduleTrialLicense = new AttribsMemberDWORD("mTV", 0);
					public AttribsMemberBOOL bModuleIsCounter = new AttribsMemberBOOL("bMC", false);
					public AttribsMemberBOOL bModuleIsPool = new AttribsMemberBOOL("bMP", false);
					public AttribsMemberDWORD moduleVersionIntroduced_Major = new AttribsMemberDWORD("iVMj", 0);
					public AttribsMemberDWORD moduleVersionIntroduced_Minor = new AttribsMemberDWORD("iVMn", 0);
					public AttribsMemberDWORD moduleVersionIntroduced_SubMajor = new AttribsMemberDWORD("iVSMj", 0);
					public AttribsMemberDWORD moduleVersionIntroduced_SubMinor = new AttribsMemberDWORD("iVSMn", 0);
					public AttribsMemberDWORD moduleVersionDeprecated_Major = new AttribsMemberDWORD("dVMj", 0);
					public AttribsMemberDWORD moduleVersionDeprecated_Minor = new AttribsMemberDWORD("dVMn", 0);
					public AttribsMemberDWORD moduleVersionDeprecated_SubMajor = new AttribsMemberDWORD("dVSMj", 0);
					public AttribsMemberDWORD moduleVersionDeprecated_SubMinor = new AttribsMemberDWORD("dVSMn", 0);
				
				};
				
				public class AttribsMemberAttribsClass_Lic_ModuleSoftwareSpecAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_ModuleSoftwareSpecAttribs(string keyName, Lic_ModuleSoftwareSpecAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_ModuleSoftwareSpecAttribs(AttribsMemberAttribsClass_Lic_ModuleSoftwareSpecAttribs t)
					{
						return (Lic_ModuleSoftwareSpecAttribs)t.m_tVal;
					}
				
					public Lic_ModuleSoftwareSpecAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					/*
				Software Spec, Product Attributes
				*/
				public class Lic_ProductSoftwareSpecAttribs : LicensingAttribsBase
				{
					public static string ClassName = "L_PSwSpA";
					
					public Lic_ProductSoftwareSpecAttribs() : 
						base(ClassName)
					{
						;
					}
					
					[FlagsAttribute]
					public enum TProductLicenseType : uint
					{
						pltServer = 0,
						pltClient = 1,
					};
					
					public class AttribsMemberEnum_TProductLicenseType : AttribsMemberEnum
					{
						public AttribsMemberEnum_TProductLicenseType(string keyName, TProductLicenseType defaultVal) :
							base(keyName, defaultVal, typeof(TProductLicenseType))
						{
							;
						}
						
						protected static SortedList m_MapAliasToEnum;
						protected static SortedList m_MapEnumToAlias;
						protected static SortedList m_MapOrderingIndexToAlias;
						protected static SortedList m_MapAliasToIndex;
						protected static SortedList m_MapEnumToIndex;
						static AttribsMemberEnum_TProductLicenseType()
						{
							m_MapAliasToEnum = new SortedList();
							m_MapEnumToAlias = new SortedList();
							m_MapOrderingIndexToAlias = new SortedList();	// map of the ordering indexes from the xml file to aliases. The ordering indexes are not guranteed to be continuous or 0 based. 
							m_MapAliasToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
							m_MapEnumToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
							m_MapAliasToEnum.Add("Server Licensing",TProductLicenseType.pltServer);
							m_MapEnumToAlias.Add(TProductLicenseType.pltServer,"Server Licensing");
							m_MapOrderingIndexToAlias.Add(1,"Server Licensing");
							m_MapAliasToEnum.Add("Client Licensing",TProductLicenseType.pltClient);
							m_MapEnumToAlias.Add(TProductLicenseType.pltClient,"Client Licensing");
							m_MapOrderingIndexToAlias.Add(2,"Client Licensing");
							m_MapAliasToIndex.Add("Server Licensing",0);
							m_MapAliasToIndex.Add("Client Licensing",1);
							m_MapEnumToIndex.Add(TProductLicenseType.pltServer,0);
							m_MapEnumToIndex.Add(TProductLicenseType.pltClient,1);
						}
						
					
						public static string GetAlias(System.Enum enum_value)
						{
							return (string)m_MapEnumToAlias[enum_value];
						}
						public static StringCollection GetAliases()
						{
							StringCollection alias_list = new StringCollection();
							foreach (string alias in m_MapOrderingIndexToAlias.Values)
							{
								alias_list.Add(alias);
							}
							return alias_list;
						}
						public static System.Enum GetEnumValueFromAlias(string alias)
						{
							return (System.Enum)m_MapAliasToEnum[alias];
						}
						public static int GetIndexFromAlias(string alias)
						{
							return (int)m_MapAliasToIndex[alias];
						}
						public static int GetIndexFromEnum(System.Enum enum_value)
						{
							return (int)m_MapEnumToIndex[enum_value];
						}
						
						public string GetAlias()
						{
							return GetAlias(EVal);
						}
						public void SetEnumValueFromAlias(string alias)
						{
							EVal = GetEnumValueFromAlias(alias);
						}
					
						public static implicit operator TProductLicenseType(AttribsMemberEnum_TProductLicenseType t)
						{
							return (TProductLicenseType)t.m_tVal;
						}
						
						public TProductLicenseType TVal
						{
							get {return this;}
							set {m_tVal = (Object)value;}
						}
					}
					
						
					public class Lic_ModuleSoftwareSpecAttribsMap : AttribsMemberGenericMap {public Lic_ModuleSoftwareSpecAttribsMap(string keyName, SortedList defaultVal) : base(keyName, typeof(UInt32), typeof(Lic_ModuleSoftwareSpecAttribs), defaultVal){;} }
						
					public AttribsMemberEnum_TProductLicenseType productLicType = new AttribsMemberEnum_TProductLicenseType("pLT", TProductLicenseType.pltServer);
					public AttribsMemberDWORD productID = new AttribsMemberDWORD("pI", 0);
					public AttribsMemberDWORD sameModSpecProductID = new AttribsMemberDWORD("sMSpPI", 0);
					public AttribsMemberDWORD prevSharedProductID = new AttribsMemberDWORD("pSPI", 0);
					public AttribsMemberString productName = new AttribsMemberString("pN", "");
					public AttribsMemberString productLabel = new AttribsMemberString("pL", "");
					public AttribsMemberDWORD softwareSpec_Major = new AttribsMemberDWORD("vMj", 0);
					public AttribsMemberDWORD softwareSpec_Minor = new AttribsMemberDWORD("vMn", 0);
					public AttribsMemberDWORD softwareSpec_SubMajor = new AttribsMemberDWORD("vSMj", 0);
					public AttribsMemberDWORD softwareSpec_SubMinor = new AttribsMemberDWORD("vSMn", 0);
					public Lic_ModuleSoftwareSpecAttribsMap moduleSpecMap = new Lic_ModuleSoftwareSpecAttribsMap("mSpMp", new SortedList());
				
				};
				
				public class AttribsMemberAttribsClass_Lic_ProductSoftwareSpecAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_ProductSoftwareSpecAttribs(string keyName, Lic_ProductSoftwareSpecAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_ProductSoftwareSpecAttribs(AttribsMemberAttribsClass_Lic_ProductSoftwareSpecAttribs t)
					{
						return (Lic_ProductSoftwareSpecAttribs)t.m_tVal;
					}
				
					public Lic_ProductSoftwareSpecAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					/*
				Software Spec Attributes
				*/
				public class Lic_SoftwareSpecAttribs : LicensingAttribsBase
				{
					public static string ClassName = "L_SwSpA";
					
					public Lic_SoftwareSpecAttribs() : 
						base(ClassName)
					{
						;
					}
					
					
					public class Lic_ProductSoftwareSpecAttribsMap : AttribsMemberGenericMap {public Lic_ProductSoftwareSpecAttribsMap(string keyName, SortedList defaultVal) : base(keyName, typeof(UInt32), typeof(Lic_ProductSoftwareSpecAttribs), defaultVal){;} }
						
					public Lic_ProductSoftwareSpecAttribsMap productSpecMap = new Lic_ProductSoftwareSpecAttribsMap("pMp", new SortedList());
					public AttribsMemberDWORD softwareSpec_Major = new AttribsMemberDWORD("vMj", 0);
					public AttribsMemberDWORD softwareSpec_Minor = new AttribsMemberDWORD("vMn", 0);
					public AttribsMemberDWORD softwareSpec_SubMajor = new AttribsMemberDWORD("vSMj", 0);
					public AttribsMemberDWORD softwareSpec_SubMinor = new AttribsMemberDWORD("vSMn", 0);
				
				};
				
				public class AttribsMemberAttribsClass_Lic_SoftwareSpecAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_SoftwareSpecAttribs(string keyName, Lic_SoftwareSpecAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_SoftwareSpecAttribs(AttribsMemberAttribsClass_Lic_SoftwareSpecAttribs t)
					{
						return (Lic_SoftwareSpecAttribs)t.m_tVal;
					}
				
					public Lic_SoftwareSpecAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					/*
				License Information - Module Attributes
				*/
				public class Lic_ModuleInfoAttribs : LicensingAttribsBase
				{
					public static string ClassName = "L_MIA";
					
					public Lic_ModuleInfoAttribs() : 
						base(ClassName)
					{
						;
					}
					
					
					public AttribsMemberDWORD moduleID = new AttribsMemberDWORD("mI", 0);
					public AttribsMemberDWORD moduleValue = new AttribsMemberDWORD("mV", 0);
					public AttribsMemberDWORD moduleAppInstance = new AttribsMemberDWORD("mAI", 0);
				
				};
				
				public class AttribsMemberAttribsClass_Lic_ModuleInfoAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_ModuleInfoAttribs(string keyName, Lic_ModuleInfoAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_ModuleInfoAttribs(AttribsMemberAttribsClass_Lic_ModuleInfoAttribs t)
					{
						return (Lic_ModuleInfoAttribs)t.m_tVal;
					}
				
					public Lic_ModuleInfoAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					/*
				License Information - Product Attributes
				*/
				public class Lic_ProductInfoAttribs : LicensingAttribsBase
				{
					public static string ClassName = "L_PIA";
					
					public Lic_ProductInfoAttribs() : 
						base(ClassName)
					{
						;
					}
					
					[FlagsAttribute]
					public enum TProductState : uint
					{
						psLicensed = 0,
						psTrial = 1,
						psAddOn = 2,
					};
					
					public class AttribsMemberEnum_TProductState : AttribsMemberEnum
					{
						public AttribsMemberEnum_TProductState(string keyName, TProductState defaultVal) :
							base(keyName, defaultVal, typeof(TProductState))
						{
							;
						}
						
						protected static SortedList m_MapAliasToEnum;
						protected static SortedList m_MapEnumToAlias;
						protected static SortedList m_MapOrderingIndexToAlias;
						protected static SortedList m_MapAliasToIndex;
						protected static SortedList m_MapEnumToIndex;
						static AttribsMemberEnum_TProductState()
						{
							m_MapAliasToEnum = new SortedList();
							m_MapEnumToAlias = new SortedList();
							m_MapOrderingIndexToAlias = new SortedList();	// map of the ordering indexes from the xml file to aliases. The ordering indexes are not guranteed to be continuous or 0 based. 
							m_MapAliasToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
							m_MapEnumToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
							m_MapAliasToEnum.Add("Licensed",TProductState.psLicensed);
							m_MapEnumToAlias.Add(TProductState.psLicensed,"Licensed");
							m_MapOrderingIndexToAlias.Add(1,"Licensed");
							m_MapAliasToEnum.Add("Trial",TProductState.psTrial);
							m_MapEnumToAlias.Add(TProductState.psTrial,"Trial");
							m_MapOrderingIndexToAlias.Add(2,"Trial");
							m_MapAliasToEnum.Add("AddOn",TProductState.psAddOn);
							m_MapEnumToAlias.Add(TProductState.psAddOn,"AddOn");
							m_MapOrderingIndexToAlias.Add(3,"AddOn");
							m_MapAliasToIndex.Add("Licensed",0);
							m_MapAliasToIndex.Add("Trial",1);
							m_MapAliasToIndex.Add("AddOn",2);
							m_MapEnumToIndex.Add(TProductState.psLicensed,0);
							m_MapEnumToIndex.Add(TProductState.psTrial,1);
							m_MapEnumToIndex.Add(TProductState.psAddOn,2);
						}
						
					
						public static string GetAlias(System.Enum enum_value)
						{
							return (string)m_MapEnumToAlias[enum_value];
						}
						public static StringCollection GetAliases()
						{
							StringCollection alias_list = new StringCollection();
							foreach (string alias in m_MapOrderingIndexToAlias.Values)
							{
								alias_list.Add(alias);
							}
							return alias_list;
						}
						public static System.Enum GetEnumValueFromAlias(string alias)
						{
							return (System.Enum)m_MapAliasToEnum[alias];
						}
						public static int GetIndexFromAlias(string alias)
						{
							return (int)m_MapAliasToIndex[alias];
						}
						public static int GetIndexFromEnum(System.Enum enum_value)
						{
							return (int)m_MapEnumToIndex[enum_value];
						}
						
						public string GetAlias()
						{
							return GetAlias(EVal);
						}
						public void SetEnumValueFromAlias(string alias)
						{
							EVal = GetEnumValueFromAlias(alias);
						}
					
						public static implicit operator TProductState(AttribsMemberEnum_TProductState t)
						{
							return (TProductState)t.m_tVal;
						}
						
						public TProductState TVal
						{
							get {return this;}
							set {m_tVal = (Object)value;}
						}
					}
					
						
					public class Lic_ModuleInfoAttribsList : AttribsMemberGenericList {public Lic_ModuleInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_ModuleInfoAttribs), defaultVal){;} }
						
					public AttribsMemberDWORD productID = new AttribsMemberDWORD("pI", 0);
					public AttribsMemberDWORD productAppInstance = new AttribsMemberDWORD("pAI", 0);
					public AttribsMemberDWORD product_Major = new AttribsMemberDWORD("pMj", 0);
					public AttribsMemberDWORD product_Minor = new AttribsMemberDWORD("pMn", 0);
					public AttribsMemberDWORD product_SubMajor = new AttribsMemberDWORD("pSMj", 0);
					public AttribsMemberDWORD product_SubMinor = new AttribsMemberDWORD("pSMn", 0);
					public AttribsMemberDateTime activationCurrentExpirationDate = new AttribsMemberDateTime("aCDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
					public AttribsMemberDateTime expirationDate = new AttribsMemberDateTime("eDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
					public AttribsMemberDWORD activationTotal = new AttribsMemberDWORD("aT", 0);
					public AttribsMemberDWORD activationCurrent = new AttribsMemberDWORD("aC", 0);
					public AttribsMemberDWORD activationAmountInDays = new AttribsMemberDWORD("aA", 0);
					public AttribsMemberDWORD activationOverrideCurrent = new AttribsMemberDWORD("aOC", 0);
					public AttribsMemberDWORD activationOverrideCurrentHoursToExpire = new AttribsMemberDWORD("aOE", 0);
					public AttribsMemberBOOL bActivationOverrideCurrent = new AttribsMemberBOOL("bAC", false);
					public AttribsMemberBOOL bActivationOverrideCurrentHoursToExpire = new AttribsMemberBOOL("bAO", false);
					public AttribsMemberBOOL bUseActivations = new AttribsMemberBOOL("bUA", false);
					public AttribsMemberBOOL bUseExpirationDate = new AttribsMemberBOOL("bUE", true);
					public AttribsMemberString contractNumber = new AttribsMemberString("cN", "");
					public AttribsMemberEnum_TProductState productState = new AttribsMemberEnum_TProductState("pSt", TProductState.psTrial);
					public Lic_ModuleInfoAttribsList moduleList = new Lic_ModuleInfoAttribsList("mLt", new ArrayList());
				
				};
				
				public class AttribsMemberAttribsClass_Lic_ProductInfoAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_ProductInfoAttribs(string keyName, Lic_ProductInfoAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_ProductInfoAttribs(AttribsMemberAttribsClass_Lic_ProductInfoAttribs t)
					{
						return (Lic_ProductInfoAttribs)t.m_tVal;
					}
				
					public Lic_ProductInfoAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					public class Lic_LicenseInfoAttribs : LicensingAttribsBase
				{
					public static string ClassName = "L_LIA";
					
					public Lic_LicenseInfoAttribs() : 
						base(ClassName)
					{
						;
					}
					
					public class Lic_ActivitySlotInfoAttribs : LicensingAttribsBase
					{
						public static string ClassName = "L_aSA";
						
						public Lic_ActivitySlotInfoAttribs() : 
							base(ClassName)
						{
							;
						}
						
						
						public AttribsMemberWORD activitySlotID = new AttribsMemberWORD("aI", 0);
						public AttribsMemberString contractNumber = new AttribsMemberString("cN", "");
					
					};
					
					public class AttribsMemberAttribsClass_Lic_ActivitySlotInfoAttribs : AttribsMemberAttribsClass
					{
						public AttribsMemberAttribsClass_Lic_ActivitySlotInfoAttribs(string keyName, Lic_ActivitySlotInfoAttribs defaultVal) : 
							base(keyName, defaultVal)
						{
							;
						}
						
						public static implicit operator Lic_ActivitySlotInfoAttribs(AttribsMemberAttribsClass_Lic_ActivitySlotInfoAttribs t)
						{
							return (Lic_ActivitySlotInfoAttribs)t.m_tVal;
						}
					
						public Lic_ActivitySlotInfoAttribs TVal
						{
							get {return this;}
							set {m_tVal = (Object)value;}
						}
					}
						
					public class Lic_ActivitySlotInfoAttribsList : AttribsMemberGenericList {public Lic_ActivitySlotInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_ActivitySlotInfoAttribs), defaultVal){;} }
						public class Lic_ActivitySlotChangeInfoAttribs : LicensingAttribsBase
					{
						public static string ClassName = "L_aSCA";
						
						public Lic_ActivitySlotChangeInfoAttribs() : 
							base(ClassName)
						{
							;
						}
						
						[FlagsAttribute]
						public enum TActivitySlotChangeActionType : uint
						{
							ascaIgnore = 0,
							ascaAdd = 1,
							ascaDelete = 2,
							ascaMove = 3,
							ascaSetActivations = 4,
							ascaSetHoursToExpire = 5,
						};
						
						public class AttribsMemberEnum_TActivitySlotChangeActionType : AttribsMemberEnum
						{
							public AttribsMemberEnum_TActivitySlotChangeActionType(string keyName, TActivitySlotChangeActionType defaultVal) :
								base(keyName, defaultVal, typeof(TActivitySlotChangeActionType))
							{
								;
							}
							
							protected static SortedList m_MapAliasToEnum;
							protected static SortedList m_MapEnumToAlias;
							protected static SortedList m_MapOrderingIndexToAlias;
							protected static SortedList m_MapAliasToIndex;
							protected static SortedList m_MapEnumToIndex;
							static AttribsMemberEnum_TActivitySlotChangeActionType()
							{
								m_MapAliasToEnum = new SortedList();
								m_MapEnumToAlias = new SortedList();
								m_MapOrderingIndexToAlias = new SortedList();	// map of the ordering indexes from the xml file to aliases. The ordering indexes are not guranteed to be continuous or 0 based. 
								m_MapAliasToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
								m_MapEnumToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
								m_MapAliasToEnum.Add("Ignore",TActivitySlotChangeActionType.ascaIgnore);
								m_MapEnumToAlias.Add(TActivitySlotChangeActionType.ascaIgnore,"Ignore");
								m_MapOrderingIndexToAlias.Add(1,"Ignore");
								m_MapAliasToEnum.Add("Add",TActivitySlotChangeActionType.ascaAdd);
								m_MapEnumToAlias.Add(TActivitySlotChangeActionType.ascaAdd,"Add");
								m_MapOrderingIndexToAlias.Add(2,"Add");
								m_MapAliasToEnum.Add("Delete",TActivitySlotChangeActionType.ascaDelete);
								m_MapEnumToAlias.Add(TActivitySlotChangeActionType.ascaDelete,"Delete");
								m_MapOrderingIndexToAlias.Add(3,"Delete");
								m_MapAliasToEnum.Add("Move",TActivitySlotChangeActionType.ascaMove);
								m_MapEnumToAlias.Add(TActivitySlotChangeActionType.ascaMove,"Move");
								m_MapOrderingIndexToAlias.Add(4,"Move");
								m_MapAliasToEnum.Add("Set Activations",TActivitySlotChangeActionType.ascaSetActivations);
								m_MapEnumToAlias.Add(TActivitySlotChangeActionType.ascaSetActivations,"Set Activations");
								m_MapOrderingIndexToAlias.Add(5,"Set Activations");
								m_MapAliasToEnum.Add("Set Hours To Expire",TActivitySlotChangeActionType.ascaSetHoursToExpire);
								m_MapEnumToAlias.Add(TActivitySlotChangeActionType.ascaSetHoursToExpire,"Set Hours To Expire");
								m_MapOrderingIndexToAlias.Add(6,"Set Hours To Expire");
								m_MapAliasToIndex.Add("Ignore",0);
								m_MapAliasToIndex.Add("Add",1);
								m_MapAliasToIndex.Add("Delete",2);
								m_MapAliasToIndex.Add("Move",3);
								m_MapAliasToIndex.Add("Set Activations",4);
								m_MapAliasToIndex.Add("Set Hours To Expire",5);
								m_MapEnumToIndex.Add(TActivitySlotChangeActionType.ascaIgnore,0);
								m_MapEnumToIndex.Add(TActivitySlotChangeActionType.ascaAdd,1);
								m_MapEnumToIndex.Add(TActivitySlotChangeActionType.ascaDelete,2);
								m_MapEnumToIndex.Add(TActivitySlotChangeActionType.ascaMove,3);
								m_MapEnumToIndex.Add(TActivitySlotChangeActionType.ascaSetActivations,4);
								m_MapEnumToIndex.Add(TActivitySlotChangeActionType.ascaSetHoursToExpire,5);
							}
							
						
							public static string GetAlias(System.Enum enum_value)
							{
								return (string)m_MapEnumToAlias[enum_value];
							}
							public static StringCollection GetAliases()
							{
								StringCollection alias_list = new StringCollection();
								foreach (string alias in m_MapOrderingIndexToAlias.Values)
								{
									alias_list.Add(alias);
								}
								return alias_list;
							}
							public static System.Enum GetEnumValueFromAlias(string alias)
							{
								return (System.Enum)m_MapAliasToEnum[alias];
							}
							public static int GetIndexFromAlias(string alias)
							{
								return (int)m_MapAliasToIndex[alias];
							}
							public static int GetIndexFromEnum(System.Enum enum_value)
							{
								return (int)m_MapEnumToIndex[enum_value];
							}
							
							public string GetAlias()
							{
								return GetAlias(EVal);
							}
							public void SetEnumValueFromAlias(string alias)
							{
								EVal = GetEnumValueFromAlias(alias);
							}
						
							public static implicit operator TActivitySlotChangeActionType(AttribsMemberEnum_TActivitySlotChangeActionType t)
							{
								return (TActivitySlotChangeActionType)t.m_tVal;
							}
							
							public TActivitySlotChangeActionType TVal
							{
								get {return this;}
								set {m_tVal = (Object)value;}
							}
						}
						
							
						public AttribsMemberString contractNumber = new AttribsMemberString("cN", "");
						public AttribsMemberWORD param1 = new AttribsMemberWORD("p1", 0);
						public AttribsMemberWORD param2 = new AttribsMemberWORD("p2", 0);
						public AttribsMemberEnum_TActivitySlotChangeActionType actionType = new AttribsMemberEnum_TActivitySlotChangeActionType("aT", TActivitySlotChangeActionType.ascaIgnore);
					
					};
					
					public class AttribsMemberAttribsClass_Lic_ActivitySlotChangeInfoAttribs : AttribsMemberAttribsClass
					{
						public AttribsMemberAttribsClass_Lic_ActivitySlotChangeInfoAttribs(string keyName, Lic_ActivitySlotChangeInfoAttribs defaultVal) : 
							base(keyName, defaultVal)
						{
							;
						}
						
						public static implicit operator Lic_ActivitySlotChangeInfoAttribs(AttribsMemberAttribsClass_Lic_ActivitySlotChangeInfoAttribs t)
						{
							return (Lic_ActivitySlotChangeInfoAttribs)t.m_tVal;
						}
					
						public Lic_ActivitySlotChangeInfoAttribs TVal
						{
							get {return this;}
							set {m_tVal = (Object)value;}
						}
					}
						public class Lic_ActivitySlotHistoryInfoAttribs : LicensingAttribsBase
					{
						public static string ClassName = "L_aSHA";
						
						public Lic_ActivitySlotHistoryInfoAttribs() : 
							base(ClassName)
						{
							;
						}
						
						
						public class Lic_ActivitySlotChangeInfoAttribsList : AttribsMemberGenericList {public Lic_ActivitySlotChangeInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_ActivitySlotChangeInfoAttribs), defaultVal){;} }
							
						public AttribsMemberWORD historyNumber = new AttribsMemberWORD("hN", 0);
						public Lic_ActivitySlotChangeInfoAttribsList activitySlotChangeInfoList = new Lic_ActivitySlotChangeInfoAttribsList("aSCLt", new ArrayList());
					
					};
					
					public class AttribsMemberAttribsClass_Lic_ActivitySlotHistoryInfoAttribs : AttribsMemberAttribsClass
					{
						public AttribsMemberAttribsClass_Lic_ActivitySlotHistoryInfoAttribs(string keyName, Lic_ActivitySlotHistoryInfoAttribs defaultVal) : 
							base(keyName, defaultVal)
						{
							;
						}
						
						public static implicit operator Lic_ActivitySlotHistoryInfoAttribs(AttribsMemberAttribsClass_Lic_ActivitySlotHistoryInfoAttribs t)
						{
							return (Lic_ActivitySlotHistoryInfoAttribs)t.m_tVal;
						}
					
						public Lic_ActivitySlotHistoryInfoAttribs TVal
						{
							get {return this;}
							set {m_tVal = (Object)value;}
						}
					}
						
					public class Lic_ActivitySlotHistoryInfoAttribsList : AttribsMemberGenericList {public Lic_ActivitySlotHistoryInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_ActivitySlotHistoryInfoAttribs), defaultVal){;} }
						
					public class Lic_ProductInfoAttribsList : AttribsMemberGenericList {public Lic_ProductInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_ProductInfoAttribs), defaultVal){;} }
						public class Lic_ValidationTokenAttribs : LicensingAttribsBase
					{
						public static string ClassName = "L_vTA";
						
						public Lic_ValidationTokenAttribs() : 
							base(ClassName)
						{
							;
						}
						
						[FlagsAttribute]
						public enum TTokenType : uint
						{
							ttNone = 0,
							ttHardwareKeyID = 1,
							ttLicenseCode = 2,
							ttBiosSerialNumber = 3,
							ttMacAddress = 4,
							ttComputerName = 5,
							ttTypeCopyFromCustomerOnly = 6,
							ttTypeArchiveOnly = 7,
							ttTypePackageOnly = 8,
							ttDomainName = 9,
							ttOperatingSystem = 10,
							ttPartOfDomain = 11,
							ttSystemManufacturer = 12,
							ttSystemModel = 13,
							ttSystemType = 14,
							ttSystemUuid = 15,
						};
						
						public class AttribsMemberEnum_TTokenType : AttribsMemberEnum
						{
							public AttribsMemberEnum_TTokenType(string keyName, TTokenType defaultVal) :
								base(keyName, defaultVal, typeof(TTokenType))
							{
								;
							}
							
							protected static SortedList m_MapAliasToEnum;
							protected static SortedList m_MapEnumToAlias;
							protected static SortedList m_MapOrderingIndexToAlias;
							protected static SortedList m_MapAliasToIndex;
							protected static SortedList m_MapEnumToIndex;
							static AttribsMemberEnum_TTokenType()
							{
								m_MapAliasToEnum = new SortedList();
								m_MapEnumToAlias = new SortedList();
								m_MapOrderingIndexToAlias = new SortedList();	// map of the ordering indexes from the xml file to aliases. The ordering indexes are not guranteed to be continuous or 0 based. 
								m_MapAliasToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
								m_MapEnumToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
								m_MapAliasToEnum.Add("None",TTokenType.ttNone);
								m_MapEnumToAlias.Add(TTokenType.ttNone,"None");
								m_MapOrderingIndexToAlias.Add(1,"None");
								m_MapAliasToEnum.Add("HardwareKeyID",TTokenType.ttHardwareKeyID);
								m_MapEnumToAlias.Add(TTokenType.ttHardwareKeyID,"HardwareKeyID");
								m_MapOrderingIndexToAlias.Add(2,"HardwareKeyID");
								m_MapAliasToEnum.Add("LicenseCode",TTokenType.ttLicenseCode);
								m_MapEnumToAlias.Add(TTokenType.ttLicenseCode,"LicenseCode");
								m_MapOrderingIndexToAlias.Add(3,"LicenseCode");
								m_MapAliasToEnum.Add("BiosSerialNumber",TTokenType.ttBiosSerialNumber);
								m_MapEnumToAlias.Add(TTokenType.ttBiosSerialNumber,"BiosSerialNumber");
								m_MapOrderingIndexToAlias.Add(4,"BiosSerialNumber");
								m_MapAliasToEnum.Add("MacAddress",TTokenType.ttMacAddress);
								m_MapEnumToAlias.Add(TTokenType.ttMacAddress,"MacAddress");
								m_MapOrderingIndexToAlias.Add(5,"MacAddress");
								m_MapAliasToEnum.Add("ComputerName",TTokenType.ttComputerName);
								m_MapEnumToAlias.Add(TTokenType.ttComputerName,"ComputerName");
								m_MapOrderingIndexToAlias.Add(6,"ComputerName");
								m_MapAliasToEnum.Add("TypeCopyCustOnly",TTokenType.ttTypeCopyFromCustomerOnly);
								m_MapEnumToAlias.Add(TTokenType.ttTypeCopyFromCustomerOnly,"TypeCopyCustOnly");
								m_MapOrderingIndexToAlias.Add(7,"TypeCopyCustOnly");
								m_MapAliasToEnum.Add("TypeArchiveOnly",TTokenType.ttTypeArchiveOnly);
								m_MapEnumToAlias.Add(TTokenType.ttTypeArchiveOnly,"TypeArchiveOnly");
								m_MapOrderingIndexToAlias.Add(8,"TypeArchiveOnly");
								m_MapAliasToEnum.Add("TypePackageOnly",TTokenType.ttTypePackageOnly);
								m_MapEnumToAlias.Add(TTokenType.ttTypePackageOnly,"TypePackageOnly");
								m_MapOrderingIndexToAlias.Add(9,"TypePackageOnly");
								m_MapAliasToEnum.Add("DomainName",TTokenType.ttDomainName);
								m_MapEnumToAlias.Add(TTokenType.ttDomainName,"DomainName");
								m_MapOrderingIndexToAlias.Add(10,"DomainName");
								m_MapAliasToEnum.Add("OperatingSystem",TTokenType.ttOperatingSystem);
								m_MapEnumToAlias.Add(TTokenType.ttOperatingSystem,"OperatingSystem");
								m_MapOrderingIndexToAlias.Add(11,"OperatingSystem");
								m_MapAliasToEnum.Add("True",TTokenType.ttPartOfDomain);
								m_MapEnumToAlias.Add(TTokenType.ttPartOfDomain,"True");
								m_MapOrderingIndexToAlias.Add(12,"True");
								m_MapAliasToEnum.Add("SystemManufacturer",TTokenType.ttSystemManufacturer);
								m_MapEnumToAlias.Add(TTokenType.ttSystemManufacturer,"SystemManufacturer");
								m_MapOrderingIndexToAlias.Add(13,"SystemManufacturer");
								m_MapAliasToEnum.Add("SystemModel",TTokenType.ttSystemModel);
								m_MapEnumToAlias.Add(TTokenType.ttSystemModel,"SystemModel");
								m_MapOrderingIndexToAlias.Add(14,"SystemModel");
								m_MapAliasToEnum.Add("SystemType",TTokenType.ttSystemType);
								m_MapEnumToAlias.Add(TTokenType.ttSystemType,"SystemType");
								m_MapOrderingIndexToAlias.Add(15,"SystemType");
								m_MapAliasToEnum.Add("SystemUUID",TTokenType.ttSystemUuid);
								m_MapEnumToAlias.Add(TTokenType.ttSystemUuid,"SystemUUID");
								m_MapOrderingIndexToAlias.Add(16,"SystemUUID");
								m_MapAliasToIndex.Add("None",0);
								m_MapAliasToIndex.Add("HardwareKeyID",1);
								m_MapAliasToIndex.Add("LicenseCode",2);
								m_MapAliasToIndex.Add("BiosSerialNumber",3);
								m_MapAliasToIndex.Add("MacAddress",4);
								m_MapAliasToIndex.Add("ComputerName",5);
								m_MapAliasToIndex.Add("TypeCopyCustOnly",6);
								m_MapAliasToIndex.Add("TypeArchiveOnly",7);
								m_MapAliasToIndex.Add("TypePackageOnly",8);
								m_MapAliasToIndex.Add("DomainName",9);
								m_MapAliasToIndex.Add("OperatingSystem",10);
								m_MapAliasToIndex.Add("True",11);
								m_MapAliasToIndex.Add("SystemManufacturer",12);
								m_MapAliasToIndex.Add("SystemModel",13);
								m_MapAliasToIndex.Add("SystemType",14);
								m_MapAliasToIndex.Add("SystemUUID",15);
								m_MapEnumToIndex.Add(TTokenType.ttNone,0);
								m_MapEnumToIndex.Add(TTokenType.ttHardwareKeyID,1);
								m_MapEnumToIndex.Add(TTokenType.ttLicenseCode,2);
								m_MapEnumToIndex.Add(TTokenType.ttBiosSerialNumber,3);
								m_MapEnumToIndex.Add(TTokenType.ttMacAddress,4);
								m_MapEnumToIndex.Add(TTokenType.ttComputerName,5);
								m_MapEnumToIndex.Add(TTokenType.ttTypeCopyFromCustomerOnly,6);
								m_MapEnumToIndex.Add(TTokenType.ttTypeArchiveOnly,7);
								m_MapEnumToIndex.Add(TTokenType.ttTypePackageOnly,8);
								m_MapEnumToIndex.Add(TTokenType.ttDomainName,9);
								m_MapEnumToIndex.Add(TTokenType.ttOperatingSystem,10);
								m_MapEnumToIndex.Add(TTokenType.ttPartOfDomain,11);
								m_MapEnumToIndex.Add(TTokenType.ttSystemManufacturer,12);
								m_MapEnumToIndex.Add(TTokenType.ttSystemModel,13);
								m_MapEnumToIndex.Add(TTokenType.ttSystemType,14);
								m_MapEnumToIndex.Add(TTokenType.ttSystemUuid,15);
							}
							
						
							public static string GetAlias(System.Enum enum_value)
							{
								return (string)m_MapEnumToAlias[enum_value];
							}
							public static StringCollection GetAliases()
							{
								StringCollection alias_list = new StringCollection();
								foreach (string alias in m_MapOrderingIndexToAlias.Values)
								{
									alias_list.Add(alias);
								}
								return alias_list;
							}
							public static System.Enum GetEnumValueFromAlias(string alias)
							{
								return (System.Enum)m_MapAliasToEnum[alias];
							}
							public static int GetIndexFromAlias(string alias)
							{
								return (int)m_MapAliasToIndex[alias];
							}
							public static int GetIndexFromEnum(System.Enum enum_value)
							{
								return (int)m_MapEnumToIndex[enum_value];
							}
							
							public string GetAlias()
							{
								return GetAlias(EVal);
							}
							public void SetEnumValueFromAlias(string alias)
							{
								EVal = GetEnumValueFromAlias(alias);
							}
						
							public static implicit operator TTokenType(AttribsMemberEnum_TTokenType t)
							{
								return (TTokenType)t.m_tVal;
							}
							
							public TTokenType TVal
							{
								get {return this;}
								set {m_tVal = (Object)value;}
							}
						}
						
							
						public AttribsMemberString tokenValue = new AttribsMemberString("tV", "");
						public AttribsMemberEnum_TTokenType tokenType = new AttribsMemberEnum_TTokenType("tT", TTokenType.ttNone);
					
					};
					
					public class AttribsMemberAttribsClass_Lic_ValidationTokenAttribs : AttribsMemberAttribsClass
					{
						public AttribsMemberAttribsClass_Lic_ValidationTokenAttribs(string keyName, Lic_ValidationTokenAttribs defaultVal) : 
							base(keyName, defaultVal)
						{
							;
						}
						
						public static implicit operator Lic_ValidationTokenAttribs(AttribsMemberAttribsClass_Lic_ValidationTokenAttribs t)
						{
							return (Lic_ValidationTokenAttribs)t.m_tVal;
						}
					
						public Lic_ValidationTokenAttribs TVal
						{
							get {return this;}
							set {m_tVal = (Object)value;}
						}
					}
						public class Lic_VerificationCodeAttribs : LicensingAttribsBase
					{
						public static string ClassName = "L_vCA";
						
						public Lic_VerificationCodeAttribs() : 
							base(ClassName)
						{
							;
						}
						
						
						public AttribsMemberDateTime verificationDate = new AttribsMemberDateTime("vDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
						public AttribsMemberString verificationValue = new AttribsMemberString("vVl", "");
					
					};
					
					public class AttribsMemberAttribsClass_Lic_VerificationCodeAttribs : AttribsMemberAttribsClass
					{
						public AttribsMemberAttribsClass_Lic_VerificationCodeAttribs(string keyName, Lic_VerificationCodeAttribs defaultVal) : 
							base(keyName, defaultVal)
						{
							;
						}
						
						public static implicit operator Lic_VerificationCodeAttribs(AttribsMemberAttribsClass_Lic_VerificationCodeAttribs t)
						{
							return (Lic_VerificationCodeAttribs)t.m_tVal;
						}
					
						public Lic_VerificationCodeAttribs TVal
						{
							get {return this;}
							set {m_tVal = (Object)value;}
						}
					}
						public class Lic_VerificationAttribs : LicensingAttribsBase
					{
						public static string ClassName = "L_VrfA";
						
						public Lic_VerificationAttribs() : 
							base(ClassName)
						{
							;
						}
						
						
						public class Lic_ValidationTokenAttribsList : AttribsMemberGenericList {public Lic_ValidationTokenAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_ValidationTokenAttribs), defaultVal){;} }
							
						public class Lic_VerificationCodeAttribsList : AttribsMemberGenericList {public Lic_VerificationCodeAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_VerificationCodeAttribs), defaultVal){;} }
							
						public Lic_ValidationTokenAttribsList validationTokenList = new Lic_ValidationTokenAttribsList("tLt", new ArrayList());
						public Lic_VerificationCodeAttribsList verificationCodeHistoryList = new Lic_VerificationCodeAttribsList("hLt", new ArrayList());
					
					};
					
					public class AttribsMemberAttribsClass_Lic_VerificationAttribs : AttribsMemberAttribsClass
					{
						public AttribsMemberAttribsClass_Lic_VerificationAttribs(string keyName, Lic_VerificationAttribs defaultVal) : 
							base(keyName, defaultVal)
						{
							;
						}
						
						public static implicit operator Lic_VerificationAttribs(AttribsMemberAttribsClass_Lic_VerificationAttribs t)
						{
							return (Lic_VerificationAttribs)t.m_tVal;
						}
					
						public Lic_VerificationAttribs TVal
						{
							get {return this;}
							set {m_tVal = (Object)value;}
						}
					}
						
					public AttribsMemberDWORD customerID = new AttribsMemberDWORD("cI", 0);
					public AttribsMemberDWORD destinationID = new AttribsMemberDWORD("dI", 0);
					public AttribsMemberDWORD softwareGroupLicenseID = new AttribsMemberDWORD("swI", 0);
					public AttribsMemberDateTime modifiedDate = new AttribsMemberDateTime("mDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
					public AttribsMemberBOOL bLicClockViolation = new AttribsMemberBOOL("lv", false);
					public AttribsMemberString diagDataErrMsg = new AttribsMemberString("ddM", "");
					public AttribsMemberDWORD diagDataErr = new AttribsMemberDWORD("ddE", 0xffffff);
					public Lic_ActivitySlotInfoAttribsList activitySlotList = new Lic_ActivitySlotInfoAttribsList("aSLt", new ArrayList());
					public Lic_ActivitySlotHistoryInfoAttribsList activitySlotHistoryList = new Lic_ActivitySlotHistoryInfoAttribsList("aSHLt", new ArrayList());
					public Lic_ProductInfoAttribsList productList = new Lic_ProductInfoAttribsList("pLt", new ArrayList());
					public AttribsMemberAttribsClass_Lic_VerificationAttribs licVerificationAttribs = new AttribsMemberAttribsClass_Lic_VerificationAttribs("lVA", new Lic_VerificationAttribs());
				
				};
				
				public class AttribsMemberAttribsClass_Lic_LicenseInfoAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_LicenseInfoAttribs(string keyName, Lic_LicenseInfoAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_LicenseInfoAttribs(AttribsMemberAttribsClass_Lic_LicenseInfoAttribs t)
					{
						return (Lic_LicenseInfoAttribs)t.m_tVal;
					}
				
					public Lic_LicenseInfoAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					
				public AttribsMemberAttribsClass_Lic_LicenseInfoAttribs licLicenseInfoAttribs = new AttribsMemberAttribsClass_Lic_LicenseInfoAttribs("lIA", new Lic_LicenseInfoAttribs());
				public AttribsMemberAttribsClass_Lic_SoftwareSpecAttribs licSoftwareSpecAttribs = new AttribsMemberAttribsClass_Lic_SoftwareSpecAttribs("lSA", new Lic_SoftwareSpecAttribs());
				public AttribsMemberAttribsClass_Lic_SoftwareSpecAttribs licReplaceSoftwareSpecAttribs = new AttribsMemberAttribsClass_Lic_SoftwareSpecAttribs("lRSA", new Lic_SoftwareSpecAttribs());
				public AttribsMemberString streamed_EmulationInfoAttribs = new AttribsMemberString("sEA", "");
			
			};
			
		}
	}
}
