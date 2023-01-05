#ifndef core_hash_md5_h
#define core_hash_md5_h
/** 
    Copyright (c) 2023, wicked systems
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
#include "md5_digest.h"

/* md5
   md5 hash
*/
class md5
{
  public:
  using                 digest    = md5_digest;
  static constexpr int  hash_size = 16;

  private:
  std::uint8_t  m_value[hash_size];

  public:
  inline  md5() noexcept {
          // initialize with a dummy (but probably unique) value
          md5_digest l_digest(this); l_digest.copy(m_value);
  }

  inline  md5(const std::uint8_t* bytes) noexcept {
          std::memcpy(m_value, bytes, hash_size);
  }

  inline  md5(md5::digest& hash) noexcept {
          reset(hash);
  }
  
  inline  md5(const md5& copy) noexcept {
          std::memcpy(m_value, copy.m_value, hash_size);
  }

  inline  md5(md5&& copy) noexcept {
          std::memcpy(m_value, copy.m_value, hash_size);
  }

  inline  ~md5() {
  }

  inline  auto  get_value() const noexcept -> const std::uint8_t* {
          return m_value;
  }

  inline  int   compare(const md5& rhs) const noexcept {
          return std::memcmp(m_value, rhs.m_value, hash_size);
  }

  inline  void  reset(md5::digest& hash) noexcept {
          hash.copy(m_value);
  }

  inline  operator const std::uint8_t*() const noexcept {
          return get_value();
  }

  inline  bool  operator<(const md5& rhs) const noexcept {
          return compare(rhs) < 0;
  }

  inline  bool  operator<=(const md5& rhs) const noexcept {
          return compare(rhs) <= 0;
  }

  inline  bool  operator>(const md5& rhs) const noexcept {
          return compare(rhs) > 0;
  }

  inline  bool  operator>=(const md5& rhs) const noexcept {
          return compare(rhs) >= 0;
  }
  
  inline  bool  operator==(const md5& rhs) const noexcept {
          return compare(rhs) == 0;
  }

  inline  bool  operator!=(const md5& rhs) const noexcept {
          return compare(rhs) != 0;
  }

  inline  md5&  operator=(const md5& rhs) noexcept {
          if(std::addressof(rhs) != this) {
              std::memcpy(m_value, rhs.m_value, hash_size);
          }
          return *this;
  }

  inline  md5&  operator=(md5&& rhs) noexcept {
          if(std::addressof(rhs) != this) {
              std::memcpy(m_value, rhs.m_value, hash_size);
          }
          return *this;
  }
};
#endif
