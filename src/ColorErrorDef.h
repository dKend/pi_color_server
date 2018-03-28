/*
*	Defines for all errors caught/checked for before any function can be 
*	executed.
*/
#define CERR_NOERROR 												0
#define CERR_GenSinCurveColorList_BadWavelength_Zero 				1
#define CERR_GenSinCurveColorList_BadDelay_Zero 					2
#define CERR_GenSinCurveColorList_BadDelay_NotLessThanWavelength	3
#define CERR_GenSinCurveColorList_BadColorList_NULL 				4
#define CERR_GenSinCurveColorList_BadColorList_AlreadyInit			5
#define CERR_SinColorCycle_BadEndpoints_StartEqualsEnd 				6
#define CERR_SinColorCycle_BadWavelength_Zero 						7
#define CERR_PrintColorList_BadColorList 							8
#define CERR_FreeColorList_BadSelf_NullPointer						9
#define CERR_FreeColorList_BadSelf_NullPointerPointer				10
#define CERR_AddColorList_BadSelf_NULLPointer						11
#define CERR_AddColorList_BadColor_NULLPointer						12
#define CERR_InitColorList_BadSelf_NULLPointer						13
#define CERR_InitColorList_BadSelf_NULLPointerPointer				14
#define CERR_FreeColor_BadPointer									15
#define CERR_GetCERRMessage_BadPointer								16
#define CERR_GetCERRMessage_BadPointer_NonNULL						17
#define CERR_GetCERRName_BadE										18
#define CERR_GetCERRName_BadPointer									19
#define CERR_GetCERRName_BadPointer_NonNULL							20
#define CERR_DuplicateColor_BadSrcPointer_NULL						21
#define CERR_DuplicateColor_BadDestPtr_NULL							22
#define CERR_DuplicateColor_BadDestPtr_NonNULL						23
#define CERR_GenFadeColorList_WavelengthZero						24
#define CERR_GenFadeColorList_DelayZero								25
#define CERR_GenFadeColorList_DelayGtWavelength						26	
#define CERR_GenFadeColorList_SelfEqNULL							27			
#define CERR_GenFadeColorList_SelfInit								28
#define CERR_ColorFade_TimeGtWavelength								29
#define CERR_ColorFade_StartEqEnd									30
#define CERR_ColorFade_WavelengthEqZero								31
#define CERR_SaveColorList_AccessDenied								32
#define CERR_SaveColorList_OutOfSpace								33
