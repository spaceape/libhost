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
#include "page_base.h"
#include <cstdarg>
#include <cstring>

namespace mmi {

/* pool
   memory pool
   Xt        - data type;
   PageSize  - how many elements the page can contain
   ArraySize - how many entries each element contains
   EnableMap - enable bitmap for fast allocation
*/
template<typename Xt, std::size_t PageSize, std::size_t ArraySize = 1, bool EnableMap = false>
class pool: public page_base<Xt, PageSize, ArraySize, EnableMap>
{
  public:
  using  base_type = page_base<Xt, PageSize, ArraySize, EnableMap>;
  using  node_type = typename base_type::node_type;

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

/*namespace mmi*/ }
#endif
