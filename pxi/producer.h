#ifndef pxi_producer_h
#define pxi_producer_h
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
#include <vector>
#include <thread>
#include <condition_variable>
#include <limits>

namespace pxi {

template<typename Xt, typename Ct>
class producer
{
  public:
  using consumer_type = std::remove_cv<Ct>;
  using callable_type = Xt;

  protected:
  using list_type     = std::vector<callable_type>;
  using iterator_type = typename list_type::iterator;

  protected:
  int         m_capacity_min;
  int         m_capacity_max;

  protected:
  std::mutex              m_list_guard;
  std::condition_variable m_list_fence;
  std::atomic<float>      m_load;

  protected:
  list_type   m_task_list;

  public:
  inline  producer(int reserve) noexcept:
          m_capacity_min(global::cache_small_max),
          m_capacity_max(std::numeric_limits<int>::max()) {
          if(reserve < m_capacity_min) {
              m_task_list.reserve(m_capacity_min);
          } else
              m_task_list.reserve(reserve);
  }

          producer(const producer&) noexcept = delete;
          producer(producer&&) noexcept = delete;

  inline  ~producer() {
  }

  inline  float  get_load() const noexcept {
          return m_load;
  }

          producer& operator=(const producer&) noexcept = delete;
          producer& operator=(producer&&) noexcept = delete;
};
/*namespace pxi*/ }
#endif
