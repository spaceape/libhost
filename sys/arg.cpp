/** 
    Copyright (c) 2021, wicked systems
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
#include "arg.h"
#include <convert.h>
#include <cstring>
#include <sys.h>
#include <log.h>

namespace sys {

      arg::arg() noexcept:
      m_text(nullptr),
      m_data_far(nullptr),
      m_size(0)
{
}

      arg::arg(const char* text) noexcept:
      m_text(text),
      m_data_far(nullptr),
      m_size(std::strlen(text) + 1)
{
}

      arg::arg(const char* text, int size) noexcept:
      m_text(text),
      m_data_far(nullptr),
      m_size(size)
{
}

      arg::arg(const arg& copy) noexcept:
      arg()
{
      if(copy.m_text) {
          if(copy.m_size > 1) {
              if((copy.m_text == copy.m_data_far) &&
                  (copy.m_size >= near_size)) {
                  m_data_far = reinterpret_cast<char*>(malloc(copy.m_size));
                  if(m_data_far) {
                      m_text = m_data_far;
                      m_size = copy.m_size;
                      std::strncpy(m_data_far, copy.m_text, copy.m_size);
                  }
              } else
              if((copy.m_text == copy.m_data_far) ||
                  (copy.m_text == std::addressof(copy.m_data_near[0]))) {
                  m_text = std::addressof(m_data_near[0]);
                  m_size = copy.m_size;
                  std::strncpy(m_data_near, copy.m_text, copy.m_size);
              } else
              if(true) {
                  m_text = copy.m_text;
                  m_size = copy.m_size;
              }
          }
      }
}

      arg::arg(arg&& copy) noexcept:
      arg()
{
      if(copy.m_text) {
          if(copy.m_size > 1) {
              if((copy.m_text == copy.m_data_far) &&
                  (copy.m_size >= near_size)) {
                  m_data_far = copy.m_data_far;
                  if(m_data_far) {
                      m_text = m_data_far;
                      m_size = copy.m_size;
                  }
                  copy.m_data_far = nullptr;
              } else
              if((copy.m_text == copy.m_data_far) ||
                  (copy.m_text == std::addressof(copy.m_data_near[0]))) {
                  m_text = std::addressof(m_data_near[0]);
                  m_size = copy.m_size;
                  std::strncpy(m_data_near, copy.m_text, copy.m_size);
              } else
              if(true) {
                  m_text = copy.m_text;
                  m_size = copy.m_size;
              }
              copy.m_text = nullptr;
              copy.m_size = 0;
          }
      }
}

      arg::~arg()
{
      if(m_data_far) {
          free(m_data_far);
      }
}

int   arg::get_char(int index) const noexcept
{
      if(index < m_size) {
          return m_text[index];
      } else
          return 0;
}

bool  arg::has_key(const char* value) const noexcept
{
      return false;
}

bool  arg::has_value(const char* value) const noexcept
{
      return false;
}

bool  arg::has_text() const noexcept
{
      return m_text;
}

bool  arg::has_text(const char* text) const noexcept
{
      if(text) {
          return std::strncmp(m_text, text, m_size) == 0;
      }
      return false;
}

bool  arg::has_text(const char* text, int offset, int length) const noexcept
{
      if(text) {
          if(offset < m_size) {
              int l_length_max = m_size - offset;
              if(length > l_length_max) {
                  length = l_length_max;
              }
              if(length > 0) {
                  return std::strncmp(m_text, text,  length) == 0;
              }
          }
      }
      return false;
}

auto  arg::get_text() const noexcept -> const char*
{
      return m_text;
}

char* arg::get_copy() noexcept
{
      if((m_text != m_data_far) &&
          (m_text != std::addressof(m_data_near[0]))) {
          // make a copy of the current text in a writable memory location
          if(m_size < near_size) {
              std::strncpy(m_data_near, m_text, m_size);
              m_text = std::addressof(m_data_near[0]);
          } else
          if(m_size > 0) {
              m_data_far = reinterpret_cast<char*>(malloc(m_size));
              m_text = m_data_far;
          }
      }
      return const_cast<char*>(m_text);
}

int   arg::get_bin_int() const noexcept
{
      int l_result = 0;
      convert<char*>::get_bin_value(l_result, m_text, m_size);
      return l_result;
}

int   arg::get_dec_int() const noexcept
{
      int l_result = 0;
      convert<char*>::get_dec_value(l_result, m_text, m_size);
      return l_result;
}

int   arg::get_hex_int() const noexcept
{
      int l_result = 0;
      convert<char*>::get_hex_value(l_result, m_text, m_size);
      return l_result;
}

int   arg::get_int() const noexcept
{
      int l_result = 0;
      convert<char*>::get_int_value(l_result, m_text, m_size, true);
      return l_result;
}

bool  arg::has_size(int size) const noexcept
{
      return m_size == size;
}

int   arg::get_size() const noexcept
{
      return m_size;
}

int   arg::operator[](int index) const noexcept
{
      return get_char(index);
}

bool  arg::operator==(const char* text) const noexcept
{
      return has_text(text);
}

bool  arg::operator!=(const char* text) const noexcept
{
      return has_text(text) == false;
}

arg&  arg::operator=(char* rhs) noexcept
{
      m_text = rhs;
      return *this;
}

arg&  arg::operator=(const arg& rhs) noexcept
{
      m_text = rhs.m_text;
      m_size = rhs.m_size;
      return *this;
}

arg&  arg::operator=(arg&& rhs) noexcept
{
      m_text = rhs.m_text;
      m_size = rhs.m_size;
      return *this;
}

/*namespace sys*/ }
