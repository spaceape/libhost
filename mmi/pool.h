#ifndef mmi_pool_h
#define mmi_pool_h
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
#include "pool_base.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>

namespace mmi {

/* pool
   memory pool
   Xt - data type
   Rt - resource type (default: heap)
*/
template<typename Xt, typename Rt>
class pool: public pool_base<Xt, Rt>
{
  public:
  using  base_type     = pool_base<Xt, Rt>;
  using  node_type     = typename base_type::node_type;

  public:
  inline  pool() noexcept:
          base_type() {
  }

          pool(const pool& copy) noexcept = delete;
          pool(pool&& copy) noexcept = delete;

  inline  node_type*  at(off_t index) const noexcept {
          return  base_type::at(index);
  }

  inline  node_type*  get_head() const noexcept {
          if(base_type::m_tail > base_type::m_head) {
              return base_type::m_head;
          } else
              return nullptr;
  }

  template<typename... Args>
  inline  node_type*  raw_get(Args&&... args) noexcept {
          return  base_type::raw_get(std::forward<Args>(args)...);
  }

  inline  node_type*  raw_unget() noexcept {
          return  base_type::raw_unget();
  }

  inline  node_type*  get_last() const noexcept {
          if(base_type::m_tail > base_type::m_head) {
              return base_type::m_tail - 1;
          } else
              return nullptr;
  }

  inline  node_type*  get_tail() const noexcept {
          if(base_type::m_tail > base_type::m_head) {
              return base_type::m_tail;
          } else
              return nullptr;
  }

  inline  bool reserve(std::size_t count) noexcept {
          return base_type::reserve(count);
  }

  inline  void clear() noexcept {
          base_type::clear();
  }

          pool& operator=(const pool&) noexcept = delete;
          pool& operator=(pool&&) noexcept = delete;
};

/* pool<char>
   memory pool specialization for char
*/
template<typename Rt>
class pool<char, Rt>
{
  public:
  using  node_type     = char;
  using  resource_type = typename std::remove_cv<Rt>::type;

  protected:
  resource_type m_resource;
  char*         m_base;
  char*         m_used;
  char*         m_next;
  char*         m_last;
  size_t        m_size;

  public:
  inline  pool() noexcept:
          m_resource(),
          m_base(nullptr),
          m_used(nullptr),
          m_next(nullptr),
          m_last(nullptr),
          m_size(0ul) {
  }

  inline  pool(std::size_t reserve_size) noexcept:
          pool() {
          reserve(reserve_size);
  }

          pool(const pool& copy) noexcept = delete;
          pool(pool&& copy) noexcept = delete;

  /**/    ~pool() {
          if(m_base != nullptr) {
              m_resource.deallocate(m_base, m_size, alignof(max_align_t));
          }
  }

  inline  auto   save() const noexcept -> char* {
          return m_next;
  }

  inline  void   save(const char*& position) const noexcept {
          position = save();
  }

  inline  char*  acquire(std::size_t length) noexcept {
          if(m_next != nullptr) {
              char* p_raw = m_used;
              char* p_next = m_used + length + 1ul;
              if(p_next <= m_last) {
                  p_raw[length] = 0;
                  m_next = p_next;
                  return p_raw;
              }
          }
          return nullptr;
  }

  inline  char*  acquire(std::size_t length, const char* copy) noexcept {
          char*  p_raw = acquire(length);
          if(p_raw != nullptr) {
              strncpy(p_raw, copy, length + 1ul);
          }
          return p_raw;
  }

  inline  char*  acquire(const char* copy) noexcept {
          if(copy != nullptr) {
              return acquire(strnlen(copy, m_size), copy);
          }
          return nullptr;
  }

  inline  char*  fvacquire(const char* format, va_list va) noexcept {
          char*  p_raw = nullptr;
          int    l_len = 0;
          if(format != nullptr) {
              va_list l_vc;
              va_copy(l_vc, va);
              l_len = std::vsnprintf(nullptr, 0, format, va);
              p_raw = acquire(l_len);
              if(p_raw != nullptr) {
                  std::vsnprintf(p_raw, l_len + 1, format, l_vc);
              }
              va_end(l_vc);
          }
          return p_raw;
  }

  inline  char*  facquire(const char* format, ...) noexcept {
          char*   p_raw;
          va_list l_va;
          va_start(l_va, format);
          p_raw = fvacquire(format, l_va);
          va_end(l_va);
          return  p_raw;
  }

  inline  std::size_t commit() noexcept {
          std::ptrdiff_t l_length = m_next - m_used - 1l;
          m_used = m_next;
          return l_length;
  }

  inline  char*  get(std::size_t length) noexcept {
          char*  p_raw = acquire(length);
          if(p_raw != nullptr) {
              commit();
          }
          return p_raw;
  }

  inline  char*  copy(const char* copy, std::size_t length) noexcept {
          char*  p_raw = acquire(length, copy);
          if(p_raw != nullptr) {
              commit();
          }
          return p_raw;
  }

  inline  char*  copy(const char* copy) noexcept {
          char*  p_raw = acquire(copy);
          if(p_raw != nullptr) {
              commit();
          }
          return p_raw;
  }

  inline  void   restore(const char*& position) noexcept {
          if((position != nullptr) &&
              ((position >= m_base) && (position <= m_last))) {
              m_used = position;
              position = nullptr;
          }
  }

  inline  bool   reserve(std::size_t size) noexcept {
          void*  p_alloc = m_resource.allocate(size, alignof(max_align_t));
          if(p_alloc != nullptr) {
              m_base = reinterpret_cast<char*>(p_alloc);
              if(m_base != nullptr) {
                  m_used = m_base;
                  m_next = m_base;
                  m_last = m_base + size;
                  m_size = size;
                  return true;
              }
          }
          return false;
  }

          pool& operator=(const pool&) noexcept = delete;
          pool& operator=(pool&&) noexcept = delete;
};

/*namespace mmi*/ }
#endif
