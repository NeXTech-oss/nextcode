//
// sse2_initializer.cpp
//
//      Copyright (c) ITGSS Corporation. All rights reserved.
//
// CRT initializers and terminators have been extracted from the main CRT sources
// to enable the CRT DLL to be built with LTCG enabled.  The source files in which
// the CRT initializers and terminators are defined cannot be compiled as /GL
// because the compiler will optimize them away during link-time code generation.
// We inhibit this optimization by defining the initializers and terminators in
// separate source files that are not compiled with /GL.
//
#include <corecrt_internal.h>

extern "C" int __cdecl __acrt_initialize_sse2();

#ifdef _M_HYBRID_X86_ARM64
#pragma intrinsic(_HybridGenerateThunks)

extern "C" int __cdecl __acrt_initialize_sse2()
{
    _HybridGenerateThunks(__acrt_initialize_sse2, 1);
    return 0;
}
#endif

extern "C" _CRTALLOC(".CRT$XIC") _PIFV const __acrt_tran_sse2_initializer = __acrt_initialize_sse2;
