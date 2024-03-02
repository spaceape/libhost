#ifndef pxi_pool_h
#define pxi_pool_h
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
#include <pxi.h>
#include "queue.h"
#include <array>
#include <limits>

namespace pxi {

template<typename Xt, typename Ct, int Size, int Policy>
class pool
{
  static_assert(Size > 1, "pool should reserve at least 2 queues");
  static_assert(Size < std::numeric_limits<int>::max(), "pool can reserve at most INT_MAX queues");

  public:
  using                 queue_type = queue<Xt, Ct>;
  static constexpr int  queue_size = Size; 

  private:
  int         m_queue_index;
  int         m_queue_count;
  queue_type  m_queue_list[Size];

  protected:
  public:
  template<typename... Args>
  inline  pool(int count, Args&&... args) noexcept:
          m_queue_index(0),
          m_queue_count(Size) {
          if(count >= 2) {
              if(count <= Size) {
                  m_queue_count = count;
              }
          }
          if constexpr (sizeof...(args)) {
              int i;
              for(i = 0; i < m_queue_count; i++) {
                  m_queue_list[i].set_params(std::forward<Args>(args)...);
              }
          }
  }

          pool(const pool&) noexcept = delete;
          pool(pool&&) noexcept = delete;

  inline  ~pool() {
  }

  inline  queue_type& get(int index) noexcept {
          return m_queue_list[index];
  }

  template<typename... Args>
  inline  bool schedule_explicit(int index, Args&&... args) noexcept {
          if((index >= 0) &&
              (index < Size)) {
              return m_queue_list[index].schedule(std::forward<Args>(args)...);
          }
          return false;
  }

  template<typename... Args>
  inline  bool schedule(Args&&... args) noexcept {
          // fast scheduling
          // try to find a less busy queue than the current one, but not necessarily the least busiest one
          if constexpr (Policy == pxi_policy_fast) {
              int   l_fuel     = m_queue_count;
              float l_load_min = std::numeric_limits<float>::infinity();
              float l_load_new;
              do {
                  l_load_new = m_queue_list[m_queue_index].get_load();
                  if(l_load_new == 0.0f) {
                      break;
                  }
                  if(l_load_new < l_load_min) {
                      break;
                  }
                  l_load_min = l_load_new;
                  ++m_queue_index;
                  if(m_queue_index == m_queue_count) {
                      m_queue_index = 0;
                  }
                  --l_fuel;
              }
              while(l_fuel);
          }

          // min scheduling
          // try to find the least busiest queue
          if constexpr (Policy == pxi_policy_min) {
              int   l_index_new;
              float l_load_new = 0;
              float l_load_min = std::numeric_limits<float>::infinity();
              for(l_index_new = 0; l_index_new < m_queue_count; l_index_new++) {
                  l_load_new = m_queue_list[l_index_new].get_load();
                  if(l_load_new < l_load_min) {
                      m_queue_index = l_index_new;
                      l_load_min = l_load_new;
                      if(l_load_min <= 0.0f) {
                          break;
                      }
                  }
              }
          }

          return m_queue_list[m_queue_index].schedule(std::forward<Args>(args)...);
  }

  inline  queue_type& operator[](int index) noexcept {
          return get(index);
  }

          pool& operator=(const pool&) noexcept = delete;
          pool& operator=(pool&&) noexcept = delete;
};
/*namespace pxi*/ }
#endif
