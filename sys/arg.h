#ifndef sys_arg_h
#define sys_arg_h
/** 
    Copyright (c) 2021, wicked systems
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
#include <sys.h>

namespace sys {

/* arg
   argument retrieved by a command line parser
*/
class arg
{
  const char* m_text;
  char*       m_data_far;
  int         m_size;

  public:
  static  constexpr int  near_size = 44;

  private:
  char        m_data_near[near_size];

  public:
          arg() noexcept;
          arg(const char*) noexcept;
          arg(const char*, int) noexcept;
          arg(const arg&) noexcept;
          arg(arg&&) noexcept;
          ~arg();

          bool   has_key(const char*) const noexcept;
          bool   has_value(const char*) const noexcept;

          bool   has_text() const noexcept;
          bool   has_text(const char*) const noexcept;
          bool   has_text(const char*, int, int) const noexcept;
          auto   get_text() const noexcept -> const char*;
          char*  get_copy() noexcept;

          int    get_char(int) const noexcept;

          int    get_bin_int() const noexcept;
          int    get_dec_int() const noexcept;
          int    get_hex_int() const noexcept;
          int    get_int() const noexcept;

          bool   has_size(int) const noexcept;
          int    get_size() const noexcept;

          int    operator[](int) const noexcept;
          bool   operator==(const char*) const noexcept;
          bool   operator==(int) const noexcept;
          bool   operator!=(const char*) const noexcept;
          bool   operator!=(int) const noexcept;

  inline         operator char*() noexcept {
          return get_copy();
  }

  inline         operator const char*() const noexcept {
          return get_text();
  }

  inline         operator int() const noexcept {
          return get_int();
  }

          arg&   operator=(char*) noexcept;
          arg&   operator=(const arg&) noexcept;
          arg&   operator=(arg&&) noexcept;
};

/*namespace sys*/ }
#endif
