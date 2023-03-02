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
#include <sys.h>
#include <sys/adt.h>
#include "directory.h"

constexpr char    path_root_chr = '/';
constexpr int     path_reserve_bytes = 64;

namespace sys {
namespace adt {
 
static directory  s_adt_root;
static char*      s_adt_path_cache;
static int        s_adt_path_size;


directory* get_root() noexcept
{
      return std::addressof(s_adt_root);
}

device* get_device(const char* path) noexcept
{
      sys::adt::device* l_device = nullptr;
      if(path != nullptr) {
          char* l_path_ptr;
          int   l_path_size = std::strlen(path) + 1;
          int   l_path_reserve = get_round_value(l_path_size, path_reserve_bytes);
          // reserve memory for the temporary path object
          if(l_path_reserve > s_adt_path_size) {
              s_adt_path_cache = reinterpret_cast<char*>(realloc(s_adt_path_cache, l_path_reserve));
              if(s_adt_path_cache == nullptr) {
                  return nullptr;
              }
              s_adt_path_size = l_path_reserve;
          }
          l_path_ptr = reinterpret_cast<char*>(std::memcpy(s_adt_path_cache, path, l_path_size));
          // remove root directory prefix slashes
          while(*l_path_ptr == path_root_chr) {
              ++l_path_ptr;
          }
          // browse path
          l_device = s_adt_root.get_next(l_path_ptr);
      }
      return l_device;
}

/*namespace adt*/ }
/*namespace sys*/ }
