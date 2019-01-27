/****************************************************************************** 
  Header: VirtualizerSDK.h
  Description: SDK header definition for the C/C++ language

  Author/s: Oreans Technologies  
  (c) 2008 Oreans Technologies
*****************************************************************************/ 


#ifndef __VIRTUALIZERSDK__

#define __VIRTUALIZERSDK__

 #ifndef __cplusplus

 extern void _stdcall VirtualizerStart(void);

 extern void _stdcall VirtualizerEnd(void);

 extern void _stdcall Virtualizer1Start(void);

 extern void _stdcall Virtualizer1End(void);

 extern void _stdcall Virtualizer2Start(void);

 extern  void _stdcall Virtualizer2End(void);
 
 extern  void _stdcall Virtualizer3Start(void);

 extern  void _stdcall Virtualizer3End(void);

 extern  void _stdcall Virtualizer4Start(void);

 extern  void _stdcall Virtualizer4End(void);

 extern  void _stdcall Virtualizer5Start(void);

 extern  void _stdcall Virtualizer5End(void);

 extern  void _stdcall VirtualizerMutate1Start(void);

 extern  void _stdcall VirtualizerMutate2Start(void);

 extern  void _stdcall VirtualizerMutate3Start(void);

 #elif __BORLANDC__

 extern "C" _stdcall __declspec(dllimport) void VirtualizerStart(void);

 extern "C" _stdcall __declspec(dllimport) void VirtualizerEnd(void);

 extern "C" _stdcall __declspec(dllimport) void Virtualizer1Start(void);

 extern "C" _stdcall __declspec(dllimport) void Virtualizer1End(void);

 extern "C" _stdcall __declspec(dllimport) void Virtualizer2Start(void);

 extern "C" _stdcall __declspec(dllimport) void Virtualizer2End(void);
 
 extern "C" _stdcall __declspec(dllimport) void Virtualizer3Start(void);

 extern "C" _stdcall __declspec(dllimport) void Virtualizer3End(void);

 extern "C" _stdcall __declspec(dllimport) void Virtualizer4Start(void);

 extern "C" _stdcall __declspec(dllimport) void Virtualizer4End(void);

 extern "C" _stdcall __declspec(dllimport) void Virtualizer5Start(void);

 extern "C" _stdcall __declspec(dllimport) void Virtualizer5End(void);

 extern "C" _stdcall __declspec(dllimport) void VirtualizerMutate1Start(void);

 extern "C" _stdcall __declspec(dllimport) void VirtualizerMutate2Start(void);

 extern "C" _stdcall __declspec(dllimport) void VirtualizerMutate3Start(void);
 
 #else

 extern "C" void _stdcall VirtualizerStart(void);

 extern "C" void _stdcall VirtualizerEnd(void);

 extern "C" void _stdcall Virtualizer1Start(void);

 extern "C" void _stdcall Virtualizer1End(void);

 extern "C" void _stdcall Virtualizer2Start(void);

 extern "C" void _stdcall Virtualizer2End(void);
 
 extern "C" void _stdcall Virtualizer3Start(void);

 extern "C" void _stdcall Virtualizer3End(void);

 extern "C" void _stdcall Virtualizer4Start(void);

 extern "C" void _stdcall Virtualizer4End(void);

 extern "C" void _stdcall Virtualizer5Start(void);

 extern "C" void _stdcall Virtualizer5End(void);

 extern "C" void _stdcall VirtualizerMutate1Start(void);

 extern "C" void _stdcall VirtualizerMutate2Start(void);

 extern "C" void _stdcall VirtualizerMutate3Start(void);

 #endif

/* Definition for Borland C/C++  */

#ifdef __BORLANDC__
 
 #define VIRTUALIZER_START __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);
 #define VIRTUALIZER_END  __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0D, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);

 #define VIRTUALIZER1_START __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x00, 0x01, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);
 #define VIRTUALIZER1_END  __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0D, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);

 #define VIRTUALIZER2_START __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x00, 0x02, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);
 #define VIRTUALIZER2_END  __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0D, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);

 #define VIRTUALIZER3_START __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x00, 0x03, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);
 #define VIRTUALIZER3_END  __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0D, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);

 #define VIRTUALIZER4_START __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x00, 0x04, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);
 #define VIRTUALIZER4_END  __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0D, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);

 #define VIRTUALIZER5_START __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x00, 0x05, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);
 #define VIRTUALIZER5_END  __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0D, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);

 #define VIRTUALIZER_MUTATE1_START __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x01, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);
 #define VIRTUALIZER_MUTATE2_START __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x02, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);
 #define VIRTUALIZER_MUTATE3_START __emit__ (0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x03, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20);
 #define __VIRTUALIZER_MACROS__

 #endif


/* Definition for Intel cpp compiler */
 
#ifndef __VIRTUALIZER_MACROS__
 
 #ifdef __ICL
 
 #define VIRTUALIZER_START \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56\
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0C \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \
 
 #define VIRTUALIZER_END \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0D \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \

  #define VIRTUALIZER_MUTATE1_START \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56\
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0C \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x01 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \

  #define VIRTUALIZER_MUTATE2_START \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56\
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0C \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x02 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \

  #define VIRTUALIZER_MUTATE3_START \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56\
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0C \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x03 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \

 #define VIRTUALIZER1_START \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56\
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0C \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x01 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \
 
 #define VIRTUALIZER1_END \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0D \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \

 #define VIRTUALIZER2_START \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56\
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0C \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x02 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \
 
 #define VIRTUALIZER2_END \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0D \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \


 #define VIRTUALIZER3_START \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56\
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0C \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x03 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \
 
 #define VIRTUALIZER3_END \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0D \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \

 #define VIRTUALIZER4_START \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56\
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0C \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x04 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \
 
 #define VIRTUALIZER4_END \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0D \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \

 #define VIRTUALIZER5_START \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56\
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0C \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x05 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \
 
 #define VIRTUALIZER1_END \
  __asm __emit 0xEB \
  __asm __emit 0x10 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \
  __asm __emit 0x0D \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x00 \
  __asm __emit 0x43 \
  __asm __emit 0x56 \
  __asm __emit 0x20 \
  __asm __emit 0x20 \


 #define __VIRTUALIZER_MACROS__
 
 #endif

#endif
 
 
/* Definition for LCC by Jacob Navia */
 
#ifndef __VIRTUALIZER_MACROS__
 
 #ifdef __LCC__
 
 #define VIRTUALIZER_START __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");
 #define VIRTUALIZER_END  __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0D, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");

 #define VIRTUALIZER1_START __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x00, 0x01, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");
 #define VIRTUALIZER1_END  __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0D, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");

 #define VIRTUALIZER2_START __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x00, 0x02, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");
 #define VIRTUALIZER2_END  __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0D, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");

 #define VIRTUALIZER3_START __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x00, 0x03, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");
 #define VIRTUALIZER3_END  __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0D, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");

 #define VIRTUALIZER4_START __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x00, 0x04, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");
 #define VIRTUALIZER4_END  __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0D, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");

 #define VIRTUALIZER5_START __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x00, 0x05, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");
 #define VIRTUALIZER5_END  __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0D, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");

 #define VIRTUALIZER_MUTATE1_START __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x01, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");
 #define VIRTUALIZER_MUTATE2_START __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x02, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");
 #define VIRTUALIZER_MUTATE3_START __asm__ (" .byte\t0xEB, 0x10, 0x43, 0x56, 0x20, 0x20, 0x0C, 0x00, 0x00, 0x00, \
                                    0x03, 0x00, 0x00, 0x00, 0x43, 0x56, 0x20, 0x20");
  
 #define __VIRTUALIZER_MACROS__
 
 #endif
 
#endif
 

/* Definition for Visual C++ and others */

#ifndef __VIRTUALIZER_MACROS__
 
 #define VIRTUALIZER_START \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0C \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
 
 #define VIRTUALIZER_END \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0D \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \

  #define VIRTUALIZER_MUTATE1_START \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0C \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x01 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \

  #define VIRTUALIZER_MUTATE2_START \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0C \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x02 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \

  #define VIRTUALIZER_MUTATE3_START \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0C \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x03 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \


 #define VIRTUALIZER1_START \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0C \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x01 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
 
 #define VIRTUALIZER1_END \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0D \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \


 #define VIRTUALIZER2_START \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0C \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x02 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
 
 #define VIRTUALIZER2_END \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0D \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \

 #define VIRTUALIZER3_START \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0C \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x03 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
 
 #define VIRTUALIZER3_END \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0D \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \

 #define VIRTUALIZER4_START \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0C \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x04 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
 
 #define VIRTUALIZER4_END \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0D \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \


 #define VIRTUALIZER5_START \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0C \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x05 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
 
 #define VIRTUALIZER5_END \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x0D \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43 \
  __asm _emit 0x56 \
  __asm _emit 0x20 \
  __asm _emit 0x20 \

 #endif

#endif

