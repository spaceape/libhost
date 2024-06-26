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
#include <cstring>

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
          std::memset(m_value, 0, hash_size);
  }

  inline  md5(const std::uint8_t* bytes) noexcept {
          std::memcpy(m_value, bytes, hash_size);
  }

  inline  md5(const char* hash, int size) noexcept {
          set(hash, size);
  }

  inline  md5(const std::string& hash) noexcept {
          set(hash);
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

  inline  auto  get_c_string(char* data, int size) const noexcept -> const char* {
          if(data) {
              if(size >= hash_size * 2) {
                  char* i_data = data;
                  int   i_hash = 0;
                  int   l_byte;
                  int   l_nibble;
                  // this is probably generated backwards :)
                  while(i_hash < hash_size) {
                      l_byte = m_value[i_hash];
                      while(l_byte > 0) {
                          l_nibble = l_byte & 15;
                          if(l_nibble < 10) {
                              *i_data = '0' + l_nibble;
                          } else
                              *i_data = 'a' + l_nibble - 10;
                          ++i_data;
                          l_byte /= 16;
                      }
                      ++i_hash;
                  }
                  if(size > hash_size * 2) {
                      *i_data = 0;
                  }
                  return data;
              }
          }
          return nullptr;
  }
  
  inline  bool  set(const char* hash, int size) noexcept {
          if(hash) {
              if(size >= hash_size * 2) {
                  auto  i_data = hash;
                  int   i_hash = 0;
                  int   i_nibble;
                  int   l_byte;
                  while(i_hash < hash_size) {
                      i_nibble = 0;
                      l_byte = 0;
                      while(i_nibble < 2) {
                          l_byte *= 16;
                          if((*i_data >= '0') &&
                              (*i_data <= '9')) {
                              l_byte |= *i_data - '0';
                          } else
                          if((*i_data >= 'A') &&
                              (*i_data <= 'F')) {
                              l_byte |= 10 + *i_data - 'A';
                          } else
                          if((*i_data >= 'a') &&
                              (*i_data <= 'f')) {
                              l_byte |= 10 + *i_data - 'a';
                          } else {
                              reset();
                              return false;
                          }
                          ++i_data;
                          ++i_nibble;
                      }
                      m_value[i_hash++] = l_byte;
                  }
                  return true;
              }
          }
          return false;
  }

  inline  auto  get_std_string() const noexcept -> std::string {
          std::size_t l_size = hash_size * 2;
          std::string l_result(l_size, '0');
          get_c_string(l_result.data(), l_result.size());
          return l_result;
  }

  inline  bool  set(const std::string& hash) noexcept {
          return set(hash.c_str(), hash.size());
  }

  inline  int   get_length() const noexcept {
          return hash_size;
  }

  inline   bool get_zero() const noexcept {
          for(int i_hash = 0; i_hash != hash_size; i_hash++) {
              if(m_value[i_hash] != 0) {
                  return false;
              }
          }
          return true;
  }

  inline  int   compare(const md5& rhs) const noexcept {
          return std::memcmp(m_value, rhs.m_value, hash_size);
  }

  inline  void  reset() noexcept {
          std::memset(m_value, 0, hash_size);
  }

  inline  void  reset(md5::digest& hash) noexcept {
          hash.copy(m_value);
  }

  inline  operator bool() const noexcept {
          return get_zero() == false;
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

  inline  bool  operator==(std::nullptr_t) const noexcept {
          return get_zero() == true;
  }

  inline  bool  operator!=(std::nullptr_t) const noexcept {
          return get_zero() == false;
  }

  inline  md5&  operator=(md5::digest& rhs) noexcept {
          reset(rhs);
          return *this;
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
