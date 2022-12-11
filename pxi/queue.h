#ifndef pxi_queue_h
#define pxi_queue_h
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
#include "consumer.h"
#include <vector>

namespace pxi {

template<typename Xt, typename Ct>
class queue: public consumer<Xt, Ct>
{
  public:
  using producer_type = producer<Xt, Ct>;
  using consumer_type = consumer<Xt, Ct>;
  using callable_type = typename consumer_type::callable_type;
  using iterator      = typename consumer_type::iterator;

  public:
  inline  queue() noexcept:
          consumer_type(0) {
  }

  template<typename... Args>
  inline  queue(int reserve, Args&&... args) noexcept:
          consumer_type(reserve, std::forward<Args>(args)...) {
  }

          queue(const queue&) noexcept = delete;
          queue(queue&&) noexcept = delete;

  virtual ~queue() {
  }

  template<typename... Args>
  inline  void   set_params(Args&&...) noexcept {
  }

          queue& operator=(const queue&) noexcept = delete;
          queue& operator=(queue&&) noexcept = delete;
};
/*namespace pxi*/ }
#endif
