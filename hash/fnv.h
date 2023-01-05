#ifndef core_hash_fnv_h
#define core_hash_fnv_h
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

/* basic Fowler–Noll–Vo hash implementation in 32 and 64 bit variants
*/
template<typename Ht>
struct hash_traits
{
  static_assert("Unsupported hash type.");
};

template<>
struct hash_traits<std::uint32_t>
{
  static constexpr std::uint32_t  key  = 16777619u;
  static constexpr std::uint32_t  bias = 2166136261u;
  static constexpr int            size = 4;

  using value_type = std::uint32_t;
  
  union data_type {
    std::uint32_t value;
    std::uint8_t  bytes[size];
  };
};

template<>
struct hash_traits<std::uint64_t>
{
  static constexpr std::uint64_t  key  = 1099511628211u;
  static constexpr std::uint64_t  bias = 14695981039346656037u;
  static constexpr int            size = 8;

  using value_type = std::uint64_t;

  union data_type {
    std::uint64_t value;
    std::uint8_t  bytes[size];
  };
};

template<typename Ht = std::uint32_t>
class fnv
{
  public:
  using hash_type                 = hash_traits<Ht>::value_type;
  static constexpr int hash_size  = hash_traits<Ht>::size;
  
  private:
  hash_type     m_value;
  
  public:
  inline  fnv() noexcept:
          m_value(hash_traits<Ht>::bias) {
  }
  
  template<typename... Args>
  inline  fnv(Args&&... args) noexcept:
          fnv() {
          hash(std::forward<Args>(args)...);
  }

  inline  fnv(const fnv& copy) noexcept:
          m_value(copy.m_value) {
  }

  inline  fnv(fnv&& copy) noexcept:
          m_value(copy.m_value) {
  }

  inline  ~fnv() {
  }

  inline  void hash() noexcept {
  }

  inline  void hash(const char* bytes, std::size_t length) noexcept {
          if(bytes) {
              while(length) {
                  m_value *= hash_traits<Ht>::key;
                  m_value ^=*bytes++;
                  length--;
              }
          }
  }

  inline  void hash(const char* string) noexcept {
          if(string) {
              while(string[0]) {
                  m_value *= hash_traits<Ht>::key;
                  m_value ^= string[0];
                  string++;
              }
          }
  }
  
  inline  void hash(void* object) noexcept {
          hash(reinterpret_cast<const char*>(reinterpret_cast<std::size_t>(object)), sizeof(std::size_t));
  }

  template<typename Xt>
  inline  void hash(const Xt* object) noexcept {
          hash(reinterpret_cast<void*>(object));
  }

  template<typename Xt>
  inline  void hash(const Xt& object) noexcept {
          hash(reinterpret_cast<const char*>(std::addressof(object)), sizeof(object));
  }

  template<typename Xt, std::size_t N>
  inline  void hash(const Xt object[N]) noexcept {
          hash(reinterpret_cast<const char*>(std::addressof(object[0])), sizeof(object) * N);
  }

  inline  hash_type get_value() const noexcept {
          return m_value;
  }

  inline  void reset() noexcept {
          m_value = hash_traits<Ht>::bias;
  }

  inline  operator hash_type() const noexcept {
          return m_value;
  }

  inline  fnv& operator=(const fnv& rhs) noexcept {
          if(std::addressof(rhs) != this) {
              m_value = rhs.m_value;
          }
          return *this;
  }

  inline  fnv& operator=(fnv&& rhs) noexcept {
          if(std::addressof(rhs) != this) {
              m_value = rhs.m_value;
          }
          return *this;
  }
};
#endif
