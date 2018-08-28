/*
 *  Copyright (c) 2010 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_AUDIO_DEVICE_LATEBINDINGSYMBOLTABLE_LINUX_H
#define WEBRTC_AUDIO_DEVICE_LATEBINDINGSYMBOLTABLE_LINUX_H

#include <assert.h>
#include <stddef.h>  // for NULL
#include <string.h>

#define typeof(x) __typeof__(x)

// This file provides macros for creating "symbol table" classes to simplify the
// dynamic loading of symbols from DLLs. Currently the implementation only
// supports Linux and pure C symbols.
// See talk/sound/pulseaudiosymboltable.(h|cc) for an example.

namespace webrtc_adm_linux {

// This macro must be invoked in a header to declare a symbol table class.
#define LATE_BINDING_SYMBOL_TABLE_DECLARE_BEGIN(ClassName) \
enum {

// This macro must be invoked in the header declaration once for each symbol
// (recommended to use an X-Macro to avoid duplication).
// This macro defines an enum with names built from the symbols, which
// essentially creates a hash table in the compiler from symbol names to their
// indices in the symbol table class.
#define LATE_BINDING_SYMBOL_TABLE_DECLARE_ENTRY(ClassName, sym) \
  ClassName##_SYMBOL_TABLE_INDEX_##sym,

// This macro completes the header declaration.
#define LATE_BINDING_SYMBOL_TABLE_DECLARE_END(ClassName) \
  ClassName##_SYMBOL_TABLE_SIZE \
}; \
\
extern const char ClassName##_kDllName[]; \
extern const char *const \
    ClassName##_kSymbolNames[ClassName##_SYMBOL_TABLE_SIZE]; \
\
typedef ::webrtc_adm_linux::LateBindingSymbolTable<ClassName##_SYMBOL_TABLE_SIZE, \
                                            ClassName##_kDllName, \
                                            ClassName##_kSymbolNames> \
    ClassName;

// This macro must be invoked in a .cc file to define a previously-declared
// symbol table class.
#define LATE_BINDING_SYMBOL_TABLE_DEFINE_BEGIN(ClassName, dllName) \
const char ClassName##_kDllName[] = dllName; \
const char *const ClassName##_kSymbolNames[ClassName##_SYMBOL_TABLE_SIZE] = {

// This macro must be invoked in the .cc definition once for each symbol
// (recommended to use an X-Macro to avoid duplication).
// This would have to use the mangled name if we were to ever support C++
// symbols.
#define LATE_BINDING_SYMBOL_TABLE_DEFINE_ENTRY(ClassName, sym) \
  #sym,

#define LATE_BINDING_SYMBOL_TABLE_DEFINE_END(ClassName) \
};

// Index of a given symbol in the given symbol table class.
#define LATESYM_INDEXOF(ClassName, sym) \
  (ClassName##_SYMBOL_TABLE_INDEX_##sym)

// Returns a reference to the given late-binded symbol, with the correct type.
#define LATESYM_GET(ClassName, inst, sym) \
  (*reinterpret_cast<typeof(&sym)>( \
      (inst)->GetSymbol(LATESYM_INDEXOF(ClassName, sym))))

}  // namespace webrtc_adm_linux

#endif  // WEBRTC_ADM_LATEBINDINGSYMBOLTABLE_LINUX_H
