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
#include "descriptor.h"
#include <unistd.h>

namespace sys {

      descriptor::descriptor() noexcept:
      m_descriptor(undef)
{
}

      descriptor::descriptor(int descriptor) noexcept:
      m_descriptor(descriptor)
{
}

      descriptor::descriptor(const descriptor& copy) noexcept:
      descriptor()
{
      if(copy.m_descriptor != undef) {
          m_descriptor = ::dup(copy.m_descriptor);
      }
}

      descriptor::descriptor(descriptor&& copy) noexcept:
      m_descriptor(copy.m_descriptor)
{
      copy.release();
}

      descriptor::~descriptor()
{
      dispose(false);
}

void  descriptor::reset() noexcept
{
      dispose(true);
}

void  descriptor::clone(int descriptor) noexcept
{
      if(descriptor != m_descriptor) {
          dispose(false);
          m_descriptor = ::dup(descriptor);
      }
}

void  descriptor::reset(int descriptor) noexcept
{
      if(descriptor != m_descriptor) {
          dispose(false);
          m_descriptor = descriptor;
      }
}

void  descriptor::release() noexcept
{
      m_descriptor = undef;
}

void  descriptor::dispose(bool clear) noexcept
{
      if(m_descriptor != undef) {
          ::close(m_descriptor);
          if(clear) {
              release();
          }
      }
}

int   descriptor::get_value() const noexcept
{
      return m_descriptor;
}

bool  descriptor::get_defined(bool expected) const noexcept
{
      if(expected) {
          return m_descriptor != undef;
      } else
          return m_descriptor == undef;
}

      descriptor::operator int() const noexcept
{
      return m_descriptor;
}

descriptor&   descriptor::operator=(int descriptor) noexcept
{
      reset(descriptor);
      return *this;
}

descriptor&   descriptor::operator=(const descriptor& rhs) noexcept
{
      if(std::addressof(rhs) != this) {
          clone(rhs.m_descriptor);
      }
      return *this;
}

descriptor&   descriptor::operator=(descriptor&& rhs) noexcept
{
      if(std::addressof(rhs) != this) {
          reset(rhs.m_descriptor);
          rhs.release();
      }
      return *this;
}

/*namespace sys*/ }
