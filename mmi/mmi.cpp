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
#include "mmi.h"
#include "metrics.h"
#include "error.h"
#include <limits>

static inline bool is_aligned(std::size_t value, std::size_t align) noexcept
{
      return (value % align) == 0u;
}

static inline bool is_aligned(void* p, std::size_t align) noexcept
{
      return is_aligned(reinterpret_cast<std::size_t>(p), align);
}

static inline std::size_t get_aligned_value(std::size_t value, std::size_t align) noexcept
{
      size_t remainder = value % align;
      if(remainder) {
          value -= remainder;
          value += align;
      }
      return value;
}

namespace mmi {
/*namespace mmi*/ }

/* resource
*/
static heap       s_heap;
static resource*  s_default_resource = resource::set_default(nullptr);

      resource::resource() noexcept
{
}

      resource::resource(const resource& copy) noexcept
      
{
}

      resource::resource(resource&& copy) noexcept
{
}

      resource::~resource()
{
}

void* resource::reallocate(void*, std::size_t, std::size_t, std::size_t, mmi::fixed) noexcept
{
      return nullptr;
}

void* resource::reallocate(void*, std::size_t, std::size_t, std::size_t, mmi::expand_throw)
{
      #ifdef __EXCEPTIONS
      throw  std::length_error("memory boundaries exceeded");
      #else
      return nullptr;
      #endif
}

void* resource::reallocate(void*, std::size_t, std::size_t, std::size_t, ...) noexcept
{
      return nullptr;
}

resource* resource::get_default() noexcept
{
      return s_default_resource;
}

resource* resource::set_default(resource* resource) noexcept
{
      if(resource) {
          s_default_resource = resource;
      } else
          s_default_resource = std::addressof(s_heap);
      return s_default_resource;
}

resource* resource::get_resource_of(const void*) noexcept
{
      return s_default_resource;
}

resource* resource::set_resource_of(const void*, resource* resource) noexcept
{
      if(resource == s_default_resource) {
          return resource;
      }
      return nullptr;
}

resource& resource::operator=(const resource&) noexcept
{
      return *this;
}

resource& resource::operator=(resource&&) noexcept
{
      return *this;
}

/* heap memory manager
*/
      heap::heap() noexcept:
      resource()
{
}

      heap::heap(const heap& copy) noexcept:
      resource(copy)
{
}

      heap::heap(heap&& copy) noexcept:
      resource(std::move(copy))
{
}

      heap::~heap()
{
}

void* heap::do_allocate(std::size_t size, std::size_t align) noexcept
{
      return aligned_alloc(align, size);
}

void  heap::do_deallocate(void* p, std::size_t, std::size_t) noexcept
{
      free(p);
}

bool  heap::do_is_equal(const std::pmr::memory_resource&) const noexcept
{
      return true;
}

void* heap::reallocate(void* p, std::size_t, std::size_t new_size, std::size_t align, mmi::fixed) noexcept
{
      if(p) {
          return nullptr;
      } else
          return aligned_alloc(align, new_size);
}

void* heap::reallocate(void* p, std::size_t size, std::size_t, std::size_t align, mmi::expand_throw)
{
      if(p) {
      #ifdef __EXCEPTIONS
          throw  std::length_error("memory boundaries exceeded");
      #else
          return nullptr;
      #endif
      } else
          return aligned_alloc(align, size);
}

void* heap::reallocate(void* p, std::size_t, std::size_t new_size, std::size_t align, ...) noexcept
{
      if(p) {
          if(is_aligned(p, align)) {
              return realloc(p, new_size);
          } else
              return nullptr;
      } else
          return aligned_alloc(align, new_size);
}

std::size_t heap::get_fixed_size() const noexcept
{
      return 0;
}

bool  heap::has_variable_size() const noexcept
{
      return true;
}

std::size_t heap::get_alloc_size(std::size_t size) const noexcept
{
      return get_round_value(size, alloc_bytes);
}

heap& heap::operator=(const heap& rhs) noexcept
{
      resource::operator=(rhs);
      return *this;
}

heap& heap::operator=(heap&& rhs) noexcept
{
      resource::operator=(std::move(rhs));
      return *this;
}
