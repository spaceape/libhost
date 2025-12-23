#ifndef core_util_h
#define core_util_h
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
#include <utility>
#include <math.h>

/* get_round_value()
*/
constexpr int get_round_value(int value, int granularity) noexcept {
      int l_mod_value = value % granularity;
      if(l_mod_value) {
          value -= l_mod_value;
          value += granularity;
      }
      return value;
}

/* get_round_value()
*/
constexpr std::size_t get_round_value(std::size_t value, std::size_t granularity) noexcept {
      std::size_t l_mod_value = value % granularity;
      if(l_mod_value) {
          value -= l_mod_value;
          value += granularity;
      }
      return value;
}

/* get_round_value()
*/
constexpr float get_round_value(float value, float granularity) noexcept {
      float rem = fmodf(value, granularity);
      if(rem != 0.0f) {
          return value - rem;
      }
      return value;
}

/* get_div_lb()
 * get lower bound value result from the division of value with divider
*/
template<typename Xt>
constexpr Xt get_div_lb(Xt value, Xt divider) noexcept {
      return value / divider;
}

/* get_div_lb()
 * get lower bound value result from the division of value with divider
*/
constexpr int get_div_lb(float value, float divider) noexcept {
      float l_div_value = value / divider;
      return static_cast<int>(l_div_value);
}

/* get_div_ub()
 * get upper bound value result from the division of value with divider
*/
template<typename Xt>
constexpr Xt get_div_ub(Xt value, Xt divider) noexcept {
      Xt  l_div_value = value / divider;
      Xt  l_mod_value = value % divider;
      if(l_mod_value > 0) {
          return l_div_value + 1;
      } else
      if(l_mod_value < 0) {
          return l_div_value - 1;
      } else
          return l_div_value;
}

/* get_div_ub()
 * get upper bound value result from the division of value with divider
*/
constexpr int get_div_ub(float value, float divider) noexcept {
      float l_div_value = value / divider;
      float l_mod_value = fmodf(value, divider);
      int   l_result    = static_cast<int>(l_div_value);
      if(l_mod_value > 0.0f) {
          return l_result + 1;
      } else
      if(l_mod_value < 0.0f) {
          return l_result - 1;
      } else
          return l_result;
}

/* get_div_round()
*/
constexpr int get_div_round(float value, float divider, bool = true) noexcept {
      float l_div_value = value / divider;
      float l_mod_value = fmodf(value, divider);
      int   l_result    = static_cast<int>(l_div_value);
      if(l_mod_value > +0.5f) {
          return l_result + 1;
      } else
      if(l_mod_value < -0.5f) {
          return l_result - 1;
      } else
          return l_result;
}
#endif
