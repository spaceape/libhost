#ifndef core_hash_md5_digest_h
#define core_hash_md5_digest_h
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
#include <openssl/md5.h>

class md5_digest
{
  MD5_CTX m_ctx;

  public:
  inline  md5_digest() noexcept {
          reset();
  }
  
  template<typename... Args>
  inline  md5_digest(Args&&... args) noexcept:
          md5_digest() {
          hash(std::forward<Args>(args)...);
  }

          md5_digest(const md5_digest& copy) noexcept = delete;
          md5_digest(md5_digest&& copy) noexcept = delete;

  inline  ~md5_digest() {
  }

  inline  void  hash() noexcept {
  }

  inline  void  hash(const char* bytes, std::size_t length) noexcept {
          MD5_Update(std::addressof(m_ctx), bytes, length);
  }

  inline  void  hash(const char* string) noexcept {
          MD5_Update(std::addressof(m_ctx), string, std::strlen(string));
  }
  
  inline  void  hash(void* object) noexcept {
          hash(reinterpret_cast<const char*>(reinterpret_cast<std::size_t>(object)), sizeof(std::size_t));
  }

  template<typename Xt>
  inline  void  hash(const Xt* object) noexcept {
          hash(reinterpret_cast<void*>(object));
  }

  template<typename Xt>
  inline  void  hash(const Xt& object) noexcept {
          hash(reinterpret_cast<const char*>(std::addressof(object)), sizeof(object));
  }

  template<typename Xt, std::size_t N>
  inline  void  hash(const Xt object[N]) noexcept {
          hash(reinterpret_cast<const char*>(std::addressof(object[0])), sizeof(object) * N);
  }

  inline  void  reset() noexcept {
          MD5_Init(std::addressof(m_ctx));
  }

  inline  void  copy(std::uint8_t* hash) noexcept {
          MD5_Final(hash, std::addressof(m_ctx));
  }

          md5_digest& operator=(const md5_digest& rhs) noexcept = delete;
          md5_digest& operator=(md5_digest&& rhs) noexcept = delete;
};
#endif
