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
#include "device.h"
#include "path.h"

constexpr char    path_split_chr = '/';
constexpr char    path_end_chr = 0;

namespace sys {
namespace adt {

      device::device(unsigned int type) noexcept:
      m_type(type),
      m_hooks(1)
{
}

      device::~device()
{
}

auto  device::get_next(char* path, path_t* cwd_ptr) noexcept -> device*
{
      char*  l_base     = path;
      char*  l_next     = path;
      bool   l_has_base = false;
      bool   l_has_next = false;
      // extract current path element and seek to the next
      while(l_has_base == false) {
          if((*l_next == path_split_chr) ||
              (*l_next == path_end_chr)) {
              l_has_base = true;
              l_has_next = *l_next == path_split_chr;
              *l_next = 0;
          }
          l_next++;
      }
      // process the current (base) path element
      if(l_has_base) {
          device* l_device_ptr;
          int     l_base_length = l_next - l_base;
          if(l_base_length >= 1) {
              if(l_base[0] == '.') {
                  if(l_base_length == 2) {
                      if(l_base[1] == '.') {
                          if(cwd_ptr->parent_ptr != nullptr) {
                              return cwd_ptr->parent_ptr->device_ptr->get_next(l_next, cwd_ptr->parent_ptr);
                          }
                      }
                  } else
                  if(l_base_length == 1) {
                      return get_next(l_next, cwd_ptr);
                  }
              }
              l_device_ptr = get_entry_by_name(l_base);
              if(l_has_next) {
                  path_t l_device_path;
                  if(l_device_ptr != nullptr) {
                      l_device_path.parent_ptr = cwd_ptr;
                      l_device_path.device_ptr = l_device_ptr;
                      return l_device_ptr->get_next(l_next, std::addressof(l_device_path));
                  }
              }
              return l_device_ptr;
          }
      }
      return nullptr;
}

auto  device::get_type() const noexcept -> unsigned int
{
      return m_type;
}

bool  device::has_type(unsigned int type) const noexcept
{
      return (m_type & type) == type;
}

auto  device::get_next(char* path) noexcept -> device*
{
      path_t  l_cwd;
      l_cwd.parent_ptr = nullptr;
      l_cwd.device_ptr = this;
      return  get_next(path, std::addressof(l_cwd));
}

auto  device::get_entry_by_name(const char*, int) noexcept -> device*
{
      return nullptr;
}

auto  device::get_entry_by_index(int) noexcept -> device*
{
      return nullptr;
}

int   device::get_entry_count() noexcept
{
      return 0;
}

auto  device::hook() noexcept -> device*
{
      if(m_hooks < std::numeric_limits<short int>::max()) {
          ++m_hooks;
          return this;
      }
      return nullptr;
}

auto  device::drop() noexcept -> device*
{
      if(m_hooks > 0) {
          --m_hooks;
          if(m_hooks == 0) {
              delete this;
          }
      }
      return nullptr;
}

void  device::yield(unsigned int) const noexcept
{
}

void  device::sync(float) noexcept
{
}

/*namespace adt*/ }
/*namespace sys*/ }
