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
#include "node.h"
#include "device.h"
#include "directory.h"
#include <limits>

namespace sys {
namespace adt {

      node::node(directory* owner, const char* name) noexcept:
      node(owner, name, nullptr)
{
}

      node::node(directory* owner, const char* name, device* device_ptr) noexcept:
      p_owner(nullptr),
      m_name(name),
      p_node_prev(nullptr),
      p_node_next(nullptr),
      m_hooks(1),
      m_device(nullptr)
{
      if(owner != nullptr) {
          owner->link_node(this);
      }
      if(device_ptr) {
          m_device = device_ptr->hook();
      }
}

      node::~node()
{
      if(m_device) {
          m_device = m_device->drop();
      }
      if(p_owner != nullptr) {
          p_owner->drop_node(this);
      }
}

bool  node::has_root() const noexcept
{
      return p_owner != nullptr;
}

bool  node::has_root(directory* root) const noexcept
{
      return p_owner == root;
}

bool  node::has_name(const char* name, int name_length) const noexcept
{
      return m_name == name;
}

auto  node::get_name() const noexcept -> const char*
{
      return m_name;
}

bool  node::has_type(unsigned int type) const noexcept
{
      if(m_device != nullptr) {
          return m_device->has_type(type);
      }
      return type == device::type_undef;
}

auto  node::get_type() const noexcept -> unsigned int
{
      if(m_device != nullptr) {
          return m_device->get_type();
      }
      return device::type_undef;
}

device* node::get_device() const noexcept
{
      return m_device;
}

node* node::hook() noexcept
{
      if(m_hooks < std::numeric_limits<short int>::max()) {
          ++m_hooks;
          return this;
      }
      return nullptr;
}

node* node::drop() noexcept
{
      if(m_hooks > 0) {
          --m_hooks;
          if(m_hooks == 0) {
              delete this;
          }
      }
      return nullptr;
}

void  node::sync(float dt) noexcept
{
      if(m_device != nullptr) {
          m_device->sync(dt);
      }
}

/*namespace adt*/ }
/*namespace sys*/ }
