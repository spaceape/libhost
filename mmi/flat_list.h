#ifndef mmi_flat_list_h
#define mmi_flat_list_h
/**
    Copyright (c) 2018, wicked systems
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
#include <vector>
#include "flat_list_traits.h"

namespace mmi {

template<typename Xt>
class flat_list: public std::vector<Xt>
{
  using  base_type = typename std::vector<Xt>;

  public:
  using  node_type = typename std::remove_cv<Xt>::type;

  using  iterator_type = typename base_type::iterator;
  using  const_iterator_type = typename base_type::const_iterator;

  static constexpr size_t elements_min = global::cache_small_max;
  static constexpr size_t elements_max = 0;

  public:
  inline  flat_list() noexcept:
          base_type() {
  }

  inline  flat_list(size_t reserve) noexcept:
          base_type() {
          base_type::reserve(reserve);
  }

  inline  flat_list(const flat_list& copy) noexcept:
          base_type(copy) {
  }

  inline  flat_list(flat_list&& copy) noexcept:
          base_type(std::move(copy)) {
  }

          ~flat_list() {
  }

  inline  void insert(const node_type& node) noexcept {
          base_type::emplace_back(node);
  }

  inline  void insert(node_type&& node) noexcept {
          base_type::emplace_back(std::move(node));
  }

  template<typename It>
  inline  auto insert(It pos, const node_type& node) noexcept {
          return base_type::insert(pos, node);
  }

  template<typename It>
  inline  auto insert(It pos, node_type&& node) noexcept {
          return base_type::insert(pos, std::move(node));
  }

  inline  auto find(const node_type& node) noexcept -> iterator_type {
          for(auto it = base_type::begin(); it != base_type::end(); it++) {
              if(it.operator*() == node) {
                  return it;
              }
          }
          return base_type::end();
  }

  inline  auto find(node_type* ptr) noexcept -> iterator_type {
          for(auto it = base_type::begin(); it != base_type::end(); it++) {
              if(std::addressof(*it) == ptr) {
                  return it;
              }
          }
          return base_type::end();
  }

  inline  int  get_index_of(const node_type& node) noexcept {
          auto it = find(node);
          if(it != base_type::end()) {
              return std::distance(base_type::begin(), it);
          }
          return -1;
  }

  inline  int  get_index_of(node_type* ptr) noexcept {
          auto it = find(ptr);
          if(it != base_type::end()) {
              return std::distance(base_type::begin(), it);
          }
          return -1;
  }

  inline  int  get_index_of(iterator_type it) noexcept {
          if(it != base_type::end()) {
              return std::distance(base_type::begin(), it);
          }
          return -1;
  }

  inline  auto get_iterator_at(int index) noexcept -> iterator_type {
          if((index >= 0) &&
              (index < static_cast<int>(base_type::size()))) {
              return base_type::begin() + index;
          } else
          if((index < 0) &&
              (index >= 0 - static_cast<int>(base_type::size()))) {
              return base_type::end() + index;
          }
          return base_type::end();
  }

  inline  bool contains(const node_type& node) const noexcept {
          for(auto it = base_type::cbegin(); it != base_type::cend(); it++) {
              if(it.operator*() == node) {
                  return true;
              }
          }
          return false;
  }

  inline  bool remove(const node_type& node) noexcept {
          for(auto it = base_type::begin(); it != base_type::end(); it++) {
              if(it.operator*() == node) {
                  base_type::erase(it);
                  return true;
              }
          }
          return false;
  }

  inline  auto remove(iterator_type pos) noexcept -> iterator_type {
          return base_type::erase(pos);
  }

  inline  void remove(int index) noexcept {
          if((index >= 0) &&
              (index <= static_cast<int>(base_type::size()))) {
              remove(base_type::begin() + index);
          }
  }

  inline  flat_list& operator=(const flat_list& rhs) noexcept {
          base_type::operator=(rhs);
          return *this;
  }

  inline  flat_list& operator=(flat_list&& rhs) noexcept {
          base_type::operator=(std::move(rhs));
          return *this;
  }
};

/*namespace mmi*/ }
#endif
