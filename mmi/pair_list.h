#ifndef mmi_pair_list_h
#define mmi_pair_list_h
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

template<typename Kt, typename Vt>
class pair_list: public std::vector<mmi::flat_list_traits::key_value_pair<Kt, Vt>>
{
  using  base_type = typename std::vector<mmi::flat_list_traits::key_value_pair<Kt, Vt>>;

  public:
  using  key_type = typename mmi::flat_list_traits::key_value_pair<Kt, Vt>::key_type;
  using  value_type = typename mmi::flat_list_traits::key_value_pair<Kt, Vt>::value_type;
  using  node_type = typename mmi::flat_list_traits::key_value_pair<Kt, Vt>;

  using  iterator_type = typename base_type::iterator;
  using  const_iterator_type = typename base_type::const_iterator;

  static constexpr size_t elements_min = global::cache_small_max;
  static constexpr size_t elements_max = 0;

  public:
  inline  pair_list() noexcept:
          base_type() {
  }

  inline  pair_list(size_t reserve) noexcept:
          base_type() {
          base_type::reserve(reserve);
  }

  inline  pair_list(const pair_list& copy) noexcept:
          base_type(copy) {
  }

  inline  pair_list(pair_list&& copy) noexcept:
          base_type(std::move(copy)) {
  }

          ~pair_list() {
  }

  inline  void insert(const key_type& key, const value_type& value) noexcept {
          base_type::emplace_back(key, value);
  }

  inline  void insert(const key_type& key, value_type&& value) noexcept {
          base_type::emplace_back(key, std::move(value));
  }

  template<typename It>
  inline  auto insert(It pos, const key_type& key, const value_type& value) noexcept {
          return base_type::insert(pos, node_type(key, value));
  }

  template<typename It>
  inline  auto insert(It pos, const key_type& key, value_type&& value) noexcept {
          return base_type::insert(pos, node_type(key, std::move(value)));
  }

  inline  iterator_type find(const key_type& key) noexcept {
          for(auto it = base_type::begin(); it != base_type::end(); it++) {
              if(it->key == key) {
                  return it;
              }
          }
          return base_type::end();
  }

  inline  int  get_index_of(const key_type& key) noexcept {
          auto it = find(key);
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

  inline  bool contains(const key_type& key) const noexcept {
          for(auto it = base_type::cbegin(); it != base_type::cend(); it++) {
              if(it->key == key) {
                  return true;
              }
          }
          return false;
  }

  inline  bool remove(const key_type& key) noexcept {
          for(auto it = base_type::begin(); it != base_type::end(); it++) {
              if(it->key == key) {
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

  inline  pair_list& operator=(const pair_list& rhs) noexcept {
          base_type::operator=(rhs);
          return *this;
  }

  inline  pair_list& operator=(pair_list&& rhs) noexcept {
          base_type::operator=(std::move(rhs));
          return *this;
  }
};
/*namespace mmi*/ }
#endif
