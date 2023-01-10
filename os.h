#ifndef core_os_h
#define core_os_h
/** 
    Copyright (c) 2019, wicked systems
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
    conditions are met:
    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
      disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
      disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of wicked systems nor the names of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/
#include "global.h"
#include <limits>

using     _float = double;                                             /*OS' native floating point type*/
constexpr _float  _epsilon = std::numeric_limits<_float>::epsilon();   /*epsilon for the native floating point type*/

namespace os {

constexpr char  cpu_name[] = __name_of(CPU);
constexpr char  dpu_name[] = __name_of(DPU);
constexpr char  fpu_name[] = __name_of(FPU);
constexpr char  gpu_name[] = __name_of(GPU);

constexpr bool  is_lsb = true;
constexpr bool  is_msb =(is_lsb == false);

constexpr bool  is_linux = LINUX;
constexpr bool  is_unix  = is_linux || UNIX;

constexpr int   memory_page_size = 4096;
constexpr int   filesystem_block_size = 1024;

/*namespace os*/ }
#endif
