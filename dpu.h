#ifndef core_dpu_h
#define core_dpu_h
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
#include "os.h"
#include DPU_H
#include <initializer_list>

namespace dpu {

constexpr int pts = dpts;

static constexpr size_t get_atom_count(size_t size) noexcept {
      size_t result = size / pts;
      if(size % pts) {
          result += 1;
      }
      return result;
}

template<size_t Size>
struct vec
{
  static  constexpr size_t atoms = get_atom_count(Size);

  dptype  m_value[atoms][pts];

  public:
  inline  vec() noexcept {
  }

  inline  vec(dptype imm) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_mov(m_value[u], imm);
          }
  }

  inline  vec(std::initializer_list<dptype> values) noexcept {
          auto i = values.begin();
          for(auto u = 0u; u < atoms; u++) {
              for(auto v = 0u; v < pts; v++) {
                  if(i != values.end()) {
                      m_value[u][v] =*i;
                  } else
                      m_value[u][v] = 0.0f;
                  i++;
              }
          }
  }

  inline  vec(const vec* copy) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_mov(m_value[u], copy->m_value[u]);
          }
  }

  inline  vec(const vec& copy) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_mov(m_value[u], copy.m_value[u]);
          }
  }

  inline  vec(vec&& copy) noexcept:
          vec(copy) {
  }

  inline  bool equals(dptype rhs) const noexcept {
          for(auto u = 0u; u < atoms; u++) {
              if(equ(m_value[u], rhs) == false) {
                  return false;
              }
          }
          return true;
  }

  inline  bool equals(vec& rhs) const noexcept {
          for(auto u = 0u; u < atoms; u++) {
              if(equ(m_value[u], rhs.m_value[u]) == false) {
                  return false;
              }
          }
          return true;
  }

  inline  size_t get_atoms() const noexcept {
          return atoms;
  }
  
  inline  size_t get_points() const noexcept {
          return pts;
  }

  inline  vec operator+(dptype rhs) const noexcept {
          vec result(this);
          result += rhs;
          return result;
  }

  inline  vec operator+(vec& rhs) const noexcept {
          vec result(this);
          result += rhs;
          return result;
  }

  inline  vec& operator+=(dptype rhs) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_add(m_value[u], rhs);
          }
          return *this;
  }

  inline  vec& operator+=(vec& rhs) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_add(m_value[u], rhs.m_value[u]);
          }
          return *this;
  }

  inline  vec operator-(dptype rhs) const noexcept {
          vec result(this);
          result -= rhs;
          return result;
  }

  inline  vec operator-(vec& rhs) const noexcept {
          vec result(this);
          result -= rhs;
          return result;
  }

  inline  vec& operator-=(dptype rhs) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_sub(m_value[u], rhs);
          }
          return *this;
  }

  inline  vec& operator-=(vec& rhs) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_sub(m_value[u], rhs.m_value[u]);
          }
          return *this;
  }

  inline  vec operator*(dptype rhs) const noexcept {
          vec result(this);
          result *= rhs;
          return result;
  }

  inline  vec operator*(vec& rhs) const noexcept {
          vec result(this);
          result *= rhs;
          return result;
  }

  inline  vec& operator*=(dptype rhs) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_mul(m_value[u], rhs);
          }
          return *this;
  }

  inline  vec& operator*=(vec& rhs) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_mul(m_value[u], rhs.m_value[u]);
          }
          return *this;
  }

  inline  vec operator/(dptype rhs) const noexcept {
          vec result(this);
          result /= rhs;
          return result;
  }

  inline  vec operator/(vec& rhs) const noexcept {
          vec result(this);
          result /= rhs;
          return result;
  }

  inline  vec& operator/=(dptype rhs) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_div(m_value[u], rhs);
          }
          return *this;
  }

  inline  vec& operator/=(vec& rhs) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_div(m_value[u], rhs.m_value[u]);
          }
          return *this;
  }

  inline  bool operator==(dptype rhs) const noexcept {
          return equals(rhs);
  }

  inline  bool operator==(vec& rhs) const noexcept {
          return equals(rhs);
  }

  inline  bool operator!=(dptype rhs) const noexcept {
          return equals(rhs) == false;
  }

  inline  bool operator!=(vec& rhs) const noexcept {
          return equals(rhs) == false;
  }

  inline  dptype& operator[](int index) noexcept {
          return std::addressof(m_value[0][0]) + index;
  }

  inline  operator dptype*() const noexcept {
          return m_value;
  }
  
  inline  vec& operator=(dptype value) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_mov(m_value[u], value);
          }
  }

  inline  vec& operator=(const vec& rhs) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_mov(m_value[u], rhs.m_value[u]);
          }
          return *this;
  }

  inline  vec& operator=(vec&& rhs) noexcept {
          for(auto u = 0u; u < atoms; u++) {
              reg_mov(m_value[u], rhs.m_value[u]);
          }
          return *this;
  }
};

template<>
struct vec<pts>
{
  dptype  m_value[pts];

  public:
  inline  vec() noexcept {
  }

  inline  vec(dptype imm) noexcept {
          reg_mov(m_value, imm);
  }

  inline  vec(std::initializer_list<dptype> values) noexcept {
          auto i = values.begin();
          for(auto v = 0u; v < pts; v++) {
              if(i != values.end()) {
                  m_value[v] =*i;
              } else
                  m_value[v] = 0.0f;
              i++;
          }
  }

  inline  vec(const vec* copy) noexcept {
          reg_mov(m_value, copy->m_value);
  }

  inline  vec(const vec& copy) noexcept {
          reg_mov(m_value, copy.m_value);
  }

  inline  vec(vec&& copy) noexcept:
          vec(copy) {
  }

  inline  bool equals(dptype rhs) const noexcept {
          return equ(m_value, rhs);
  }

  inline  bool equals(vec& rhs) const noexcept {
          return equ(m_value, rhs.m_value);
  }

  inline  size_t get_atoms() const noexcept {
          return 1;
  }
  
  inline  size_t get_points() const noexcept {
          return pts;
  }

  inline  vec operator+(dptype rhs) const noexcept {
          vec result(this);
          result += rhs;
          return result;
  }

  inline  vec operator+(vec& rhs) const noexcept {
          vec result(this);
          result += rhs;
          return result;
  }

  inline  vec& operator+=(dptype rhs) noexcept {
          reg_add(m_value, rhs);
          return *this;
  }

  inline  vec& operator+=(vec& rhs) noexcept {
          reg_add(m_value, rhs.m_value);
          return *this;
  }

  inline  vec operator-(dptype rhs) const noexcept {
          vec result(this);
          result -= rhs;
          return result;
  }

  inline  vec operator-(vec& rhs) const noexcept {
          vec result(this);
          result -= rhs;
          return result;
  }

  inline  vec& operator-=(dptype rhs) noexcept {
          reg_sub(m_value, rhs);
          return *this;
  }

  inline  vec& operator-=(vec& rhs) noexcept {
          reg_sub(m_value, rhs.m_value);
          return *this;
  }

  inline  vec operator*(dptype rhs) const noexcept {
          vec result(this);
          result *= rhs;
          return result;
  }

  inline  vec operator*(vec& rhs) const noexcept {
          vec result(this);
          result *= rhs;
          return result;
  }

  inline  vec& operator*=(dptype rhs) noexcept {
          reg_mul(m_value, rhs);
          return *this;
  }

  inline  vec& operator*=(vec& rhs) noexcept {
          reg_mul(m_value, rhs.m_value);
          return *this;
  }

  inline  vec operator/(dptype rhs) const noexcept {
          vec result(this);
          result /= rhs;
          return result;
  }

  inline  vec operator/(vec& rhs) const noexcept {
          vec result(this);
          result /= rhs;
          return result;
  }

  inline  vec& operator/=(dptype rhs) noexcept {
          reg_div(m_value, rhs);
          return *this;
  }

  inline  vec& operator/=(vec& rhs) noexcept {
          reg_div(m_value, rhs.m_value);
          return *this;
  }

  inline  vec operator<<(unsigned int count) const noexcept {
          vec result(this);
          for(unsigned int x = 0; x != count; x++) {
              reg_shl(result.m_value);
          }
          return result;
  }

  inline  vec& operator<<=(unsigned int count) noexcept {
          for(unsigned int x = 0; x != count; x++) {
              reg_shl(m_value);
          }
          return *this;
  }

  inline  vec operator>>(unsigned int count) const noexcept {
          vec result(this);
          for(unsigned int x = 0; x != count; x++) {
              reg_shr(result.m_value);
          }
          return result;
  }

  inline  vec& operator>>=(unsigned int count) noexcept {
          for(unsigned int x = 0; x != count; x++) {
              reg_shr(m_value);
          }
          return *this;
  }

  inline  bool operator==(dptype rhs) const noexcept {
          return equals(rhs);
  }

  inline  bool operator==(vec& rhs) const noexcept {
          return equals(rhs);
  }

  inline  bool operator!=(dptype rhs) const noexcept {
          return equals(rhs) == false;
  }

  inline  bool operator!=(vec& rhs) const noexcept {
          return equals(rhs) == false;
  }

  inline  dptype& operator[](int index) noexcept {
          return m_value[index];
  }

  inline  operator dptype*() noexcept {
          return std::addressof(m_value[0]);
  }
  
  inline  vec& operator=(dptype value) noexcept {
          reg_mov(m_value, value);
          return *this;
  }

  inline  vec& operator=(const vec& rhs) noexcept {
          reg_mov(m_value, rhs.m_value);
          return *this;
  }

  inline  vec& operator=(vec&& rhs) noexcept {
          reg_mov(m_value, rhs.m_value);
          return *this;
  }
};
/*namespace dpu*/}
#endif
