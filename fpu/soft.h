#ifndef core_fpu_soft_h
#define core_fpu_soft_h
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
#include <cstdlib>
#include <cstring>
#include <cmath>

#define fptype float
#define fpts 4

namespace fpu {

constexpr fptype zero = 0.0;
constexpr fptype one = 1.0;

inline  void  nop() noexcept
{
}

inline  void  reg_clr(fptype* r) noexcept
{
        r[0] = 0;
        r[1] = 0;
        r[2] = 0;
        r[3] = 0;
}

inline  void  reg_mov(fptype* r, fptype imm) noexcept
{
        r[0] = imm;
        r[1] = imm;
        r[2] = imm;
        r[3] = imm;
}

inline  void  reg_mov(fptype* r, const fptype* s) noexcept
{
        r[0] = s[0];
        r[1] = s[1];
        r[2] = s[2];
        r[3] = s[3];
}

inline  void  reg_mov(fptype* r, fptype imm0, fptype imm1, fptype imm2, fptype imm3) noexcept
{
        r[0] = imm0;
        r[1] = imm1;
        r[2] = imm2;
        r[3] = imm3;
}

inline  void  reg_shl(fptype* r) noexcept
{
        r[0] = r[1];
        r[1] = r[2];
        r[2] = r[3];
}

inline  void  reg_shr(fptype* r) noexcept
{
        r[3] = r[2];
        r[2] = r[1];
        r[1] = r[0];
}

inline  void  reg_neg(fptype* r) noexcept
{
        r[0] =-r[0];
        r[1] =-r[1];
        r[2] =-r[2];
        r[3] =-r[3];
}

inline  void  reg_inv(fptype* r) noexcept
{
        r[0] = 1.0 / r[0];
        r[1] = 1.0 / r[1];
        r[2] = 1.0 / r[2];
        r[3] = 1.0 / r[3];
}

inline  void  reg_add(fptype* r, const fptype imm) noexcept
{
        r[0] += imm;
        r[1] += imm;
        r[2] += imm;
        r[3] += imm;
}

inline  void  reg_add(fptype* r, const fptype* s) noexcept
{
        r[0] += s[0];
        r[1] += s[1];
        r[2] += s[2];
        r[3] += s[3];
}

inline  void  reg_sub(fptype* r, const fptype imm) noexcept
{
        r[0] -= imm;
        r[1] -= imm;
        r[2] -= imm;
        r[3] -= imm;
}

inline  void  reg_sub(fptype* r, const fptype* s) noexcept
{
        r[0] -= s[0];
        r[1] -= s[1];
        r[2] -= s[2];
        r[3] -= s[3];
}

inline  void  reg_mul(fptype* r, const fptype imm) noexcept
{
        r[0] *= imm;
        r[1] *= imm;
        r[2] *= imm;
        r[3] *= imm;
}

inline  void  reg_mul(fptype* r, const fptype* s) noexcept
{
        r[0] *= s[0];
        r[1] *= s[1];
        r[2] *= s[2];
        r[3] *= s[3];
}

inline  void  reg_div(fptype* r, const fptype imm) noexcept
{
        r[0] /= imm;
        r[1] /= imm;
        r[2] /= imm;
        r[3] /= imm;
}

inline  void  reg_div(fptype* r, const fptype* s) noexcept
{
        r[0] /= s[0];
        r[1] /= s[1];
        r[2] /= s[2];
        r[3] /= s[3];
}

inline  void  vec_mov(fptype* dp, const fptype imm, int size) noexcept
{
        fptype* l_dst_base = dp;
        fptype* l_dst_last = dp + size;
        while(l_dst_base < l_dst_last) {
            *l_dst_base = imm;
            ++l_dst_base;
        }
}

inline  void vec_mov(fptype* dp, fptype* sp, int size) noexcept
{
        fptype* l_dst_base = dp;
        fptype* l_dst_last = dp + size;
        fptype* l_src_base = sp;
        while(l_dst_base < l_dst_last) {
            *l_dst_base = *l_src_base;
            ++l_src_base;
            ++l_dst_base;
        }
}

inline  void vec_clr(fptype* dp, int size) noexcept {
        return vec_mov(dp, 0.0f, size);
}

inline  bool equ(fptype lhs, fptype rhs) noexcept {
        fptype error = 0.0;
        if(lhs == rhs) {
            return true;
        }
        if(std::fabs(rhs) > std::fabs(lhs)) {
            error = std::fabs((lhs - rhs) / rhs);
        } else
            error = std::fabs((lhs - rhs) / lhs);
        return error <=_epsilon;
}

inline  bool equ(const fptype* r, fptype imm) noexcept {
        for(auto x = 0; x != fpts; x++) {
            if(equ(r[x], imm) == false) {
                return false;
            }
        }
        return true;
}

inline  bool equ(const fptype* r, const fptype* s) noexcept {
        for(auto x = 0; x != fpts; x++) {
            if(equ(r[x], s[x]) == false) {
                return false;
            }
        }
        return true;
}

inline  bool   equ_0(const fptype* r) noexcept {
        return equ(r, zero);
}

inline  bool   equ_1(const fptype* r) noexcept {
        return equ(r, one);
}
/*namespace fpu*/ }
#endif
