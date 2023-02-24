#ifndef core_dpu_soft_h
#define core_dpu_soft_h
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

#define dptype int32_t
#define dpts 4

namespace dpu {


constexpr dptype zero = 0;
constexpr dptype one = 1;

inline  void  nop() noexcept
{
}

inline  void  reg_clr(dptype* r) noexcept
{
        r[0] = 0;
        r[1] = 0;
        r[2] = 0;
        r[3] = 0;
}

inline  void  reg_mov(dptype* r, dptype imm) noexcept
{
        r[0] = imm;
        r[1] = imm;
        r[2] = imm;
        r[3] = imm;
}

inline  void  reg_mov(dptype* r, const dptype* s) noexcept
{
        r[0] = s[0];
        r[1] = s[1];
        r[2] = s[2];
        r[3] = s[3];
}

inline  void  reg_mov(dptype* r, dptype imm0, dptype imm1, dptype imm2, dptype imm3) noexcept
{
        r[0] = imm0;
        r[1] = imm1;
        r[2] = imm2;
        r[3] = imm3;
}

inline  void  reg_shl(dptype* r) noexcept
{
        r[0] = r[1];
        r[1] = r[2];
        r[2] = r[3];
}

inline  void  reg_shr(dptype* r) noexcept
{
        r[3] = r[2];
        r[2] = r[1];
        r[1] = r[0];
}

inline  void  reg_neg(dptype* r) noexcept
{
        r[0] =-r[0];
        r[1] =-r[1];
        r[2] =-r[2];
        r[3] =-r[3];
}

inline  void  reg_inv(dptype* r) noexcept
{
        r[0] = 0;
        r[1] = 0;
        r[2] = 0;
        r[3] = 0;
}

inline  void  reg_add(dptype* r, const dptype imm) noexcept
{
        r[0] += imm;
        r[1] += imm;
        r[2] += imm;
        r[3] += imm;
}

inline  void  reg_add(dptype* r, const dptype* s) noexcept
{
        r[0] += s[0];
        r[1] += s[1];
        r[2] += s[2];
        r[3] += s[3];
}

inline  void  reg_sub(dptype* r, const dptype imm) noexcept
{
        r[0] -= imm;
        r[1] -= imm;
        r[2] -= imm;
        r[3] -= imm;
}

inline  void  reg_sub(dptype* r, const dptype* s) noexcept
{
        r[0] -= s[0];
        r[1] -= s[1];
        r[2] -= s[2];
        r[3] -= s[3];
}

inline  void  reg_mul(dptype* r, const dptype imm) noexcept
{
        r[0] *= imm;
        r[1] *= imm;
        r[2] *= imm;
        r[3] *= imm;
}

inline  void  reg_mul(dptype* r, const dptype* s) noexcept
{
        r[0] *= s[0];
        r[1] *= s[1];
        r[2] *= s[2];
        r[3] *= s[3];
}

inline  void  reg_div(dptype* r, const dptype imm) noexcept
{
        r[0] /= imm;
        r[1] /= imm;
        r[2] /= imm;
        r[3] /= imm;
}

inline  void  reg_div(dptype* r, const dptype* s) noexcept
{
        r[0] /= s[0];
        r[1] /= s[1];
        r[2] /= s[2];
        r[3] /= s[3];
}

inline  void  vec_mov(dptype* dp, const dptype imm, int size) noexcept
{
        dptype* l_dst_base = dp;
        dptype* l_dst_last = dp + size;
        while(l_dst_base < l_dst_last) {
            *l_dst_base = imm;
            ++l_dst_base;
        }
}

inline  void vec_mov(dptype* dp, dptype* sp, int size) noexcept
{
        dptype* l_dst_base = dp;
        dptype* l_dst_last = dp + size;
        dptype* l_src_base = sp;
        while(l_dst_base < l_dst_last) {
            *l_dst_base = *l_src_base;
            ++l_src_base;
            ++l_dst_base;
        }
}

inline  void vec_clr(dptype* dp, int size) noexcept
{
        return vec_mov(dp, 0.0f, size);
}

inline  bool equ(dptype lhs, dptype rhs) noexcept
{
        dptype error = 0;
        if(lhs == rhs) {
            return true;
        }
        if(std::abs(rhs) > std::abs(lhs)) {
            error = std::abs((lhs - rhs) / rhs);
        } else
            error = std::abs((lhs - rhs) / lhs);
        return error <=_epsilon;
}

inline  bool equ(const dptype* r, dptype imm) noexcept
{
        for(auto x = 0; x != dpts; x++) {
            if(equ(r[x], imm) == false) {
                return false;
            }
        }
        return true;
}

inline  bool equ(const dptype* r, const dptype* s) noexcept
{
        for(auto x = 0; x != dpts; x++) {
            if(equ(r[x], s[x]) == false) {
                return false;
            }
        }
        return true;
}

inline  bool   equ_0(const dptype* r) noexcept
{
        return equ(r, zero);
}

inline  bool   equ_1(const dptype* r) noexcept
{
        return equ(r, one);
}
/*namespace dpu*/ }
#endif
