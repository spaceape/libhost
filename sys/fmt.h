#ifndef sys_fmt_h
#define sys_fmt_h
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
#include <os.h>
#include <sys.h>
#include <cstring>

namespace fmt {

class d
{
  char  m_text[32];
  
  public:
  template<typename Xt>
  explicit  d(Xt value, bool show_sign = false) noexcept {
          int   l_digit;
          int   l_offset;
          int   l_length = 0;
          int   l_sign   = 1;
          int   l_size   = sizeof(m_text);
          if(value < 0) {
              l_sign    =-1;
              value     =-value; 
              show_sign = true;
          }
          if constexpr (os::is_lsb) {
              l_offset = l_size;
              while(value) {
                  if(l_offset > 0) {
                      l_offset--;
                  } else
                      break;
                  l_digit = value % 10;
                  m_text[l_offset] = '0' + l_digit;
                  value = value / 10;
                  l_length++;
              }

              // copy the converted buffer at the beginning of the buffer
              if(l_offset) {
                  int l_dst = 0;
                  int l_src = l_offset;
                  if(show_sign) {
                      if(value != 0) {
                          if(l_sign < 0) {
                              m_text[0] = '-';
                          } else
                          if(l_sign > 0) {
                              m_text[0] = '+';
                          }
                      } else
                          m_text[0] = '0';
                      l_dst++;
                  }
                  while(l_src < l_size) {
                      m_text[l_dst] = m_text[l_src];
                      l_dst++;
                      l_src++;
                  }
              }
              m_text[l_length] = 0;
          }
          static_assert(os::is_msb == false, "fmt::d conversion not implemented for big endian");
  }

  inline  d(const d& copy) noexcept {
          std::memcpy(m_text, copy.m_text, sizeof(m_text));
  }
  
  inline  d(d&& copy) noexcept:
          d(copy) {
          copy.m_text[0] = 0;
  }
  
  inline  ~d() {
  }
  
  inline  operator const char*() const noexcept {
          return m_text;
  }
  
  inline  d& operator=(const d& rhs) noexcept {
          std::memcpy(m_text, rhs.m_text, sizeof(m_text));
          return *this;
  }

  inline  d& operator=(d&& rhs) noexcept {
          std::memcpy(m_text, rhs.m_text, sizeof(m_text));
          rhs.m_text[0] = 0;
          return *this;
  }
};

/* format x
*/
class x
{
  char  m_text[20];
  
  public:
  template<typename Xt>
  explicit  x(Xt value, std::size_t width = sizeof(Xt)) noexcept {
          int   l_digit;
          int   l_offset;
          int   l_length = 0;
          int   l_count  = 2 * width;
          auto& l_value  = value;
          if constexpr (os::is_lsb) {
              l_offset = sizeof(m_text) - 1;
              if(l_value >= 0) {
                  while(l_count || l_value) {
                      l_digit = l_value & 15;
                      if(l_digit < 10) {
                          m_text[l_offset] = '0' + l_digit;
                      } else
                          m_text[l_offset] = 'a' + l_digit - 10;
                      l_value = l_value >> 4;
                      l_count--;
                      l_length++;
                      if(l_offset > 0) {
                          l_offset--;
                      } else
                          break;
                  }
              } else
              if(l_value < 0) {
                  while(l_count || ((l_value & 128) == 0)) {
                      l_digit = l_value & 15;
                      if(l_digit < 10) {
                          m_text[l_offset] = '0' + l_digit;
                      } else
                          m_text[l_offset] = 'a' + l_digit - 10;
                      l_value = l_value >> 4;
                      l_count--;
                      l_length++;
                      if(l_offset > 0) {
                          l_offset--;
                      } else
                          break;
                  }
              }
              // copy the converted buffer at the beginning of the buffer
              if(l_offset) {
                  int l_index = 0;
                  while(l_index < l_length) {
                      m_text[l_index] = m_text[l_index + l_offset + 1];
                      l_index++ ;
                  }
                  m_text[l_index] = 0;
              }
          }
          static_assert(os::is_msb == false, "fmt::x conversion not implemented for big endian");
  }

  inline  x(const x& copy) noexcept {
          std::memcpy(m_text, copy.m_text, sizeof(m_text));
  }
  
  inline  x(x&& copy) noexcept:
          x(copy) {
          copy.m_text[0] = 0;
  }
  
  inline  ~x() {
  }
  
  inline  operator const char*() const noexcept {
          return m_text;
  }
  
  inline  x& operator=(const x& rhs) noexcept {
          std::memcpy(m_text, rhs.m_text, sizeof(m_text));
          return *this;
  }

  inline  x& operator=(x&& rhs) noexcept {
          std::memcpy(m_text, rhs.m_text, sizeof(m_text));
          rhs.m_text[0] = 0;
          return *this;
  }
};

/* format X
*/
class X
{
  char  m_text[20];
  
  public:
  template<typename Xt>
  explicit X(Xt value, std::size_t width = sizeof(Xt)) noexcept {
          int   l_digit;
          int   l_offset;
          int   l_length = 0;
          int   l_count  = 2 * width;
          auto& l_value  = value;
          if constexpr (os::is_lsb) {
              l_offset = sizeof(m_text) - 1;
              if(l_value >= 0) {
                  while(l_count || l_value) {
                      l_digit = l_value & 15;
                      if(l_digit < 10) {
                          m_text[l_offset] = '0' + l_digit;
                      } else
                          m_text[l_offset] = 'A' + l_digit - 10;
                      l_value = l_value >> 4;
                      l_count--;
                      l_length++;
                      if(l_offset > 0) {
                          l_offset--;
                      } else
                          break;
                  }
              } else
              if(l_value < 0) {
                  while(l_count || ((l_value & 128) == 0)) {
                      l_digit = l_value & 15;
                      if(l_digit < 10) {
                          m_text[l_offset] = '0' + l_digit;
                      } else
                          m_text[l_offset] = 'A' + l_digit - 10;
                      l_value = l_value >> 4;
                      l_count--;
                      l_length++;
                      if(l_offset > 0) {
                          l_offset--;
                      } else
                          break;
                  }
              }
              // copy the converted buffer at the beginning of the buffer
              if(l_offset) {
                  int l_index = 0;
                  while(l_index < l_length) {
                      m_text[l_index] = m_text[l_offset + l_index + 1];
                      l_index++ ;
                  }
                  m_text[l_index] = 0;
              }
          }
          static_assert(os::is_msb == false, "fmt::x conversion not implemented for big endian");
  }
  
  inline  X(const X& copy) noexcept {
          std::memcpy(m_text, copy.m_text, sizeof(m_text));
  }
  
  inline  X(X&& copy) noexcept:
          X(copy) {
          copy.m_text[0] = 0;
  }
  
  inline  ~X() {
  }
  
  inline  operator const char*() const noexcept {
          return m_text;
  }
  
  inline  X& operator=(const X& rhs) noexcept {
          std::memcpy(m_text, rhs.m_text, sizeof(m_text));
          return *this;
  }

  inline  X& operator=(X&& rhs) noexcept {
          std::memcpy(m_text, rhs.m_text, sizeof(m_text));
          rhs.m_text[0] = 0;
          return *this;
  }
};

class f
{
  char  m_text[32];
  
  public:
  template<typename Xt>
  explicit f(Xt value) noexcept {
          std::snprintf(m_text, sizeof(m_text), "%f", (float)value);
  }

  inline  f(const f& copy) noexcept {
          std::memcpy(m_text, copy.m_text, sizeof(m_text));
  }
  
  inline  f(f&& copy) noexcept:
          f(copy) {
          copy.m_text[0] = 0;
  }
  
  inline  ~f() {
  }
  
  inline  operator const char*() const noexcept {
          return m_text;
  }
  
  inline  f& operator=(const f& rhs) noexcept {
          std::memcpy(m_text, rhs.m_text, sizeof(m_text));
          return *this;
  }

  inline  f& operator=(f&& rhs) noexcept {
          std::memcpy(m_text, rhs.m_text, sizeof(m_text));
          rhs.m_text[0] = 0;
          return *this;
  }
};

/* format p
   convert a pointer to a hex string
*/
template<char option = 'x'>
class p
{
  char  m_text[4 + sizeof(void*) * 2];
  
  public:
  inline  p(void* value) noexcept {
          if(value == nullptr) {
              if constexpr (option == 'p') {
                  std::strncpy(m_text, "null\n", sizeof(m_text));
              }
          }
          if constexpr (os::is_lsb) {
              if constexpr (option != 'p') {
                  int  l_pos = sizeof(void*);
                  int  l_digit;
                  int  l_index = 0;
                  auto l_value = reinterpret_cast<size_t>(value);

                  m_text[l_pos--] = 0;
                  while(l_index < static_cast<int>(sizeof(void*))) {
                      l_digit = l_value & 15;
                      if(l_digit < 10) {
                          m_text[l_pos] = '0' + l_digit;
                      } else
                          m_text[l_pos] = 'a' + l_digit - 10;
                      l_value = l_value >> 4;
                      l_index++;
                      l_pos--;
                  }
              }
          }
          static_assert(os::is_msb == false, "fmt<p> conversion not implemented for big endian");
  }
  
  inline  p(const p& copy) noexcept {
          std::memcpy(m_text, copy.m_text, sizeof(m_text));
  }
  
  inline  p(p&& copy) noexcept:
          p(copy) {
          copy.m_text[0] = 0;
  }
  
  inline  ~p() {
  }
  
  inline  operator char*() const noexcept {
          return m_text;
  }
  
  inline  p& operator=(const p& rhs) noexcept {
          std::memcpy(m_text, rhs.m_text, sizeof(m_text));
          return *this;
  }

  inline  p& operator=(p&& rhs) noexcept {
          std::memcpy(m_text, rhs.m_text, sizeof(m_text));
          rhs.m_text[0] = 0;
          return *this;
  }
};

/*namespace fmt*/ }
#endif
