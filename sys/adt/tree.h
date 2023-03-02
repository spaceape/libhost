#ifndef sys_adt_tree_h
#define sys_adt_tree_h
/** 
    Copyright (c) 2022, wicked systems
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
#include <sys.h>
#include "node.h"
#include <type_traits>

template<typename Xt>
class mount_point: public sys::adt::node
{
  public:
  using   device_type = typename std::remove_cv<Xt>::type;

  public:
  template<typename... Args>
  inline  mount_point(sys::adt::directory* owner, const char* name, Args&&... args) noexcept:
          node(owner, name, std::make_shared<Xt>(std::forward<Args>(args)...)) {
  }

  inline  mount_point(const mount_point&) noexcept = delete;
  inline  mount_point(mount_point&&) noexcept = delete;

  inline  ~mount_point() {
  }

  inline  device_type* operator->() const noexcept {
          return static_cast<device_type*>(m_device);
  }

  inline  mount_point& operator=(const mount_point&) noexcept = delete;
  inline  mount_point& operator=(mount_point&&) noexcept = delete;
};
#endif
