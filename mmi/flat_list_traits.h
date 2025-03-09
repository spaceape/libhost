#ifndef mmi_flat_list_traits_h
#define mmi_flat_list_traits_h
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
namespace flat_list_traits {

  struct has_prev_impl {
    template<typename Xt>
    static constexpr auto test(int) -> decltype(std::declval<Xt>().get_prev());

    template<typename Xt>
    static constexpr auto test(...) -> std::false_type;
  };

  template<typename Xt>
  struct has_prev: std::conditional<
          std::is_same<Xt*, decltype(has_prev_impl::test<Xt>(0))>::value,
          std::true_type,
          std::false_type
      >::type {
  };

  struct has_next_impl {
    template<typename Xt>
    static constexpr auto test(int) -> decltype(std::declval<Xt>().get_next());

    template<typename Xt>
    static constexpr auto test(...) -> std::false_type;
  };

  template<typename Xt>
  struct has_next: std::conditional<
          std::is_same<Xt*, decltype(has_next_impl::test<Xt>(0))>::value,
          std::true_type,
          std::false_type
      >::type {
  };

  template<typename Kt, typename Vt>
  struct key_value_pair {

      using key_type = std::remove_cv<Kt>::type;
      using value_type = std::remove_cv<Vt>::type;

      key_type key;
      value_type value;

      key_value_pair(const key_type& key_arg, const value_type& value_arg) noexcept:
          key(key_arg),
          value(value_arg) {
      }

      key_value_pair(const key_type& key_arg, value_type&& value_arg) noexcept:
          key(key_arg),
          value(std::move(value_arg)) {
      }

      ~key_value_pair() {
      }
  };

/*namespace flat_list_traits*/ }
/*namespace mmi*/ }
#endif
