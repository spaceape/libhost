#ifndef mmi_place_h
#define mmi_place_h
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
#include <mmi.h>
#include <traits.h>

namespace mmi {

/* place
   object placeholder, useful to reserve memory for an object
*/
template<typename Xt>
class place
{
  public:

  using  node_type = typename std::remove_cv<Xt>::type;

  static constexpr std::size_t node_size = sizeof(node_type);
  static constexpr std::size_t array_size = 1;
  static constexpr std::size_t data_size = node_size * array_size;

  static constexpr bool is_node_constructible =
      std::is_trivial<node_type>::value == false;

  static constexpr bool is_node_destructible =
      std::is_destructible<node_type>::value && 
      (std::is_trivially_destructible<node_type>::value == false);

  private:
  node_type*  p_object;
  char        m_data[sizeof(Xt)];

  public:
  inline  place() noexcept:
          p_object(nullptr) {
  }

  template<typename... Args>
  inline  place(Args&&... args) noexcept:
          p_object(nullptr) {
          emplace(std::forward<Args>(args)...);
  }

  inline  place(const place& copy) noexcept:
          place() {
          emplace(copy);
  }

  inline  place(place&& copy) noexcept:
          place() {
          emplace(std::move(copy));
  }

  inline  ~place() {
          reset();
  }

  template<typename... Args>
  inline  node_type* emplace(Args&&... args) noexcept {
          if constexpr (is_node_constructible) {
              reset();
              p_object = new(std::addressof(m_data)) node_type(std::forward<Args>(args)...);
          }
          return p_object;
  }

  inline  node_type* emplace(const node_type& copy) noexcept {
          if constexpr (is_node_constructible) {
              if(p_object) {
                  p_object->operator=(copy);
              } else
                  p_object = new(m_data) node_type(copy);
          }
          return p_object;
  }

  inline  node_type* emplace(node_type&& copy) noexcept {
          if constexpr (is_node_constructible) {
              if(p_object) {
                  p_object->operator=(std::move(copy));
              } else
                  p_object = new(m_data) node_type(std::move(copy));
          }
          return p_object;
  }

  inline  node_type* reset() noexcept {
          if constexpr (is_node_destructible) {
              if(p_object) {
                  p_object->~node_type();
              }
          }
          return p_object = nullptr;
  }
  
  inline  node_type*  get() noexcept {
          return p_object;
  }

  inline  bool is_set() const noexcept {
          return p_object != nullptr;
  }

  inline  bool is_empty() const noexcept {
          return p_object == nullptr;
  }
  
  inline  operator node_type&() noexcept {
          return *p_object;
  }

  inline  operator node_type*() noexcept {
          return p_object;
  }

  inline  node_type* operator->() noexcept {
          return p_object;
  }

  inline  place& operator=(const place& rhs) noexcept {
          if(std::addressof(rhs) != this) {
              emplace(rhs);
          }
          return *this;
  }

  inline  place& operator=(place&& rhs) noexcept {
          if(std::addressof(rhs) != this) {
              emplace(std::move(rhs));
          }
          return *this;
  }
};

/*namespace mmi*/ }
#endif
