#ifndef sys_adt_node_h
#define sys_adt_node_h
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
#include <char.h>
#include <memory>

namespace sys {
namespace adt {

/* node
   Base class for an entry into the Abstract Device Tree
*/
class node
{
  directory*  p_owner;
  char_ptr<>  m_name;
  node*       p_node_prev;
  node*       p_node_next;
  short int   m_hooks;

  protected:
  device*     m_device;

  friend class directory;
  public:
          node(directory*, const char*) noexcept;
          node(directory*, const char*, device*) noexcept;
          node(const node&) noexcept = delete;
          node(node&&) noexcept = delete;
          ~node();

          bool    has_root() const noexcept;
          bool    has_root(directory* root) const noexcept;
          bool    has_name(const char* name, int name_length) const noexcept;
          auto    get_name() const noexcept -> const char*;
          bool    has_type(unsigned int) const noexcept;
          auto    get_type() const noexcept -> unsigned int;
          auto    get_device() const noexcept -> device*;

          node*   hook() noexcept;
          node*   drop() noexcept;

          void    sync(float) noexcept;

          device* operator->() const noexcept;
          node&   operator=(const node&) noexcept = delete;
          node&   operator=(node&&) noexcept = delete;
};
/*namespace adt*/ }
/*namespace sys*/ }
#endif
