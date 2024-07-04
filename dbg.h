#ifndef host_dbg_h
#define host_dbg_h
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
#include <log.h>
#include <sys/types.h>

#if NDEBUG
static constexpr bool is_debug = false;
#else
static constexpr bool is_debug = true;
#endif

inline void dbg_dump_hex(void* bytes, int size, bool use_absolute_address = false) noexcept
{
      if constexpr (is_debug) {
          int   l_width = 8;
          int   l_left = size;
          auto  p_data_ptr = reinterpret_cast<std::uint8_t*>(bytes);
          auto  p_data_end = p_data_ptr + size;
          off_t l_addr = 0;
          char  l_hex_cache[48];
          char  l_ascii_cache[16];
          if(use_absolute_address == true) {
              l_addr = reinterpret_cast<off_t>(bytes);
          }
          while(p_data_ptr < p_data_end) {
              int l_hex_offset = 0;
              int l_ascii_offset = 0;
              for(int i_cx = 0; i_cx < l_width; i_cx++) {
                  if(i_cx < l_left) {
                      l_hex_offset += std::sprintf(l_hex_cache + l_hex_offset, "%.2x ", p_data_ptr[i_cx]);
                  } else
                      l_hex_offset += std::sprintf(l_hex_cache + l_hex_offset, "   ");
              }
              for(int i_cx = 0; i_cx < l_width; i_cx++) {
                  if(i_cx < l_left) {
                      if((p_data_ptr[i_cx] >= 32) &&
                          (p_data_ptr[i_cx]) <= 127) {
                          l_ascii_cache[i_cx] = p_data_ptr[i_cx];
                      } else
                          l_ascii_cache[i_cx] = '.';
                  } else
                      l_ascii_cache[i_cx] = ' ';
                  l_ascii_offset++;
              }
              l_hex_cache[l_hex_offset] = 0;
              l_ascii_cache[l_ascii_offset] = 0;
              printf("    %.8lx: %s %s\n", l_addr, l_hex_cache, l_ascii_cache);
              l_left     -= l_width;
              l_addr     += l_width;
              p_data_ptr += l_width;
          }
      }
}
#endif
