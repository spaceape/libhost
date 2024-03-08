#ifndef core_sys_h
#define core_sys_h
/** 
    Copyright (c) 2020, wicked systems
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
#include <mmi.h>
#include <mmi/resource.h>

constexpr char EOL ='\n';
constexpr char NUL = 0;
constexpr char EOS = NUL;

#ifndef EOF
constexpr char EOF = -1;
#endif

constexpr char RET ='\r';
constexpr char TAB ='\t';
constexpr char SPC =' ';
constexpr char ASCII_MAX = 127;

class rio;
class sio;
class fio;
class bio;
class pio;

namespace sys {

class arg;
class argv;

class ios;

template<typename Bt = bio, typename Xt = fio>
class asio;

using fio = ::fio;
using bio = ::bio;
using pio = ::pio;

template<typename Xt, std::size_t, std::size_t = 0>
class var;

class descriptor;

/*namespace sys*/ }

// take ownership of file descriptor (for sys::fio)
constexpr long int  M_ACQUIRE = 0x00010000;

// method to qualify unix socket (for sys::net)
constexpr long int  M_BIND = 0x00010000;
constexpr long int  M_CONNECT = 0x00020000;

#endif
