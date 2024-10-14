#ifndef mmi_flat_set_h
#define mmi_flat_set_h
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
#include "flat_set_traits.h"
#include <compare.h>
#include <memory_resource>

namespace mmi {
  
template<typename Xt>
class flat_set: public flat_set_traits<Xt>::base_type
{
  public:
  using  node_type  = typename flat_set_traits<Xt>::node_type;
  using  key_type   = node_type;
  using  value_type = node_type;
  using  base_type  = typename flat_set_traits<Xt>::base_type;

  public:
  using  node_forward_type = typename std::conditional<
         (sizeof(node_type) > sizeof(void*)),
          node_type&,
          node_type
         >::type;

  using  iterator_type = typename flat_set_traits<Xt>::iterator_type;
  using  result_type   = node_type*;

  private:
  iterator_type m_pos;
  unsigned int  m_replace_bit:1; /*whether to replace an already existing element or fail*/
  unsigned int  m_remove_bit:1;  /*whether to erase or invalidate an element upon removal (not implemented)*/

  private:
  /* test_p()
  */
  inline  bool test_p(key_type key) noexcept {
          int  cmp = compare(*m_pos, key);
          if(cmp == 0) {
              return true;
          } else
          if(cmp < 0) {
            ++m_pos;
          }
          return false;
  }

  /* find_p()
   * binary search with m_pos as initial pivot
  */
          bool find_p(key_type key) noexcept {
          int  cmp;
          if(m_pos != base_type::end()) {
              cmp = compare(*m_pos, key);
              if(cmp < 0) {
                  m_pos = std::lower_bound(m_pos + 1, base_type::end(), key);
                  if(m_pos != base_type::end()) {
                      return test_p(key);
                  }
              } else
              if(cmp > 0) {
                  m_pos = std::lower_bound(base_type::begin(), m_pos - 1, key);
                  if(m_pos != base_type::end()) {
                      return test_p(key);
                  }
              } else
                  return true;
          }
          return false;
  }

  /* place_p()
   * blind insert before m_pos
  */
  template<typename... Args>
  inline  bool  place_p(Args&&... args) noexcept {
          m_pos = base_type::emplace(m_pos, std::forward<Args>(args)...);
          return  true;
  }

  /* place_p()
   * blind insert before m_pos
  */
  inline  bool  place_p(const node_type& node) noexcept {
          m_pos = base_type::insert(m_pos, node);
          return  true;
  }

  /* replace_p()
   * replace node at m_pos
  */
  inline  void  replace_p(const node_type& node) noexcept {
          *m_pos = node;
  }

  /* replace_p()
   * replace node at m_pos
  */
  inline  void  replace_p(node_type&& node) noexcept {
          *m_pos = std::move(node);
  }

  /* reset_p()
   * try to set the pointer into a valid state if it's at the end()
  */
  inline  void  reset_p() noexcept {
          if(m_pos == base_type::end()) {
              m_pos = base_type::begin() + (base_type::size() / 2);
          }
  }

  public:
  inline  flat_set(bool replace = false, bool remove = true) noexcept:
          base_type(),
          m_pos(base_type::end()),
          m_replace_bit(replace),
          m_remove_bit(remove) {
  }

  inline  flat_set(size_t reserve, bool replace = false, bool remove = true) noexcept:
          base_type(),
          m_pos(),
          m_replace_bit(replace),
          m_remove_bit(remove) {
          base_type::reserve(reserve);
          m_pos = base_type::end();
  }

  inline  flat_set(const flat_set& copy) noexcept:
          base_type(copy),
          m_pos(copy.m_pos),
          m_replace_bit(copy.m_replace_bit), 
          m_remove_bit(copy.m_remove_bit) {
  }

  inline  flat_set(flat_set&& copy) noexcept:
          base_type(std::move(copy)),
          m_pos(std::move(copy.m_pos)),
          m_replace_bit(copy.m_replace_bit),
          m_remove_bit(copy.m_remove_bit) {
  }

          ~flat_set() {
  }

  /* find()
  */
  inline  iterator_type find(key_type key) noexcept {
          reset_p();
          if(find_p(key)) {
              return m_pos;
          } else
              return base_type::end();
  }

  inline  bool contains(key_type key) noexcept {
          return find(key) != base_type::end();
  }
  
  /* insert()
  */
          iterator_type insert(key_type key) noexcept {
          reset_p();
          if(find_p(key)) {
          } else
          if(place_p(key)) {
              return m_pos;
          }
          return base_type::end();
  }

  /* insert()
  */
  template<typename... Args>
          iterator_type insert(key_type key, Args&&... args) noexcept {
          reset_p();
          if(find_p(key)) {
              if(m_replace_bit) {
                  *m_pos = value_type(std::forward<Args>(args)...);
              }
          } else
          if(place_p(key, std::forward<Args>(args)...)) {
              return m_pos;
          }
          return base_type::end();
  }

  /* insert()
  */
          iterator_type insert(key_type key, const value_type& value) noexcept {
          reset_p();
          if(find_p(key)) {
              if(m_replace_bit) {
                  *m_pos = value;
                  return m_pos;
              }
          } else
          if(place_p(key, value)) {
              return m_pos;
          }
          return base_type::end();
  }

  /* insert()
  */
          iterator_type insert(key_type key, value_type&& value) noexcept {
          reset_p();
          if(find_p(key)) {
              if(m_replace_bit) {
                  *m_pos = std::move(value);
                  return m_pos;
              }
          } else
          if(place_p(key, std::move(value))) {
              return m_pos;
          }
          return base_type::end();
  }

  /* remove()
   * erase node of given key, if found
  */
  inline  void remove(key_type key) noexcept {
          reset_p();
          if(find_p(key)) {
              if(m_remove_bit) {
                  m_pos = base_type::erase(m_pos);
              }
          }
  }

  /* remove()
  */
  inline  void remove(iterator_type pos) noexcept {
          m_pos = base_type::erase(pos);
  }

  inline  iterator_type begin() noexcept {
          return base_type::begin();
  }

  inline  iterator_type position() noexcept {
          return m_pos;
  }
  
  inline  iterator_type position(off_t offset) noexcept {
          iterator_type l_result = m_pos;
          if(l_result != base_type::end()) {
              std::advance(l_result, offset);
          }
          return l_result;
  }
  
  inline  iterator_type none() noexcept {
          return base_type::end();
  }
  
  inline  iterator_type end() noexcept {
          return base_type::end();
  }
  
  /* reserve()
  */
  inline  void reserve(size_t count) noexcept {
          base_type::reserve(count);
  }

  /* clear()
  */
  inline  void clear(size_t /*count*/) noexcept {
          base_type::clear();
  }

  inline  std::size_t size() const noexcept {
          return base_type::size();
  }

  inline  flat_set& operator=(const flat_set& rhs) noexcept {
          base_type::operator=(rhs);
          return *this;
  }

  inline  flat_set& operator=(flat_set&& rhs) noexcept {
          base_type::operator=(std::move(rhs));
          return *this;
  }
};
/*namespace mmi*/ }
#endif
