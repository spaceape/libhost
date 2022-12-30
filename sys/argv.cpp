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
#include "argv.h"
#include <convert.h>
#include <cstring>
#include <sys.h>
#include <log.h>

namespace sys {

      argv::argv() noexcept:
      m_arg_none(),
      m_arg_far(nullptr),
      m_arg_count(0),
      m_far_reserve(0)
{
}

      argv::argv(std::initializer_list<const char*> args) noexcept:
      argv()
{
      bool l_reserve_success = arg_reserve(args.size(), 0);
      if(l_reserve_success) {
          int l_arg_index = 0;
          for(const char* l_arg_ptr : args) {
              if(l_arg_index < arg_near_reserve) {
                  m_arg_near[l_arg_index] = l_arg_ptr;
              } else
                  m_arg_far[l_arg_index - arg_near_reserve] = l_arg_ptr;
              l_arg_index++;
          }
          m_arg_count = l_arg_index;
      }
}

      argv::argv(char* line, int count_max) noexcept:
      argv()
{
      load(line, count_max);
}

      argv::argv(char** arg_ptr, int arg_count) noexcept:
      argv()
{
      load(arg_ptr, arg_count);
}

      argv::argv(const argv& copy) noexcept:
      argv()
{
      assign(copy);
}

      argv::argv(argv& copy) noexcept:
      argv()
{
      assign(std::move(copy));
}

      argv::~argv()
{
      dispose(false);
}

bool  argv::arg_reserve(int count, int keep) noexcept
{
      int l_far_count = count - arg_near_reserve;   // how many 'far' instances to reserve memory for
      int l_far_move  = keep;                       // how many instances to preserve into the new array
      if(l_far_count > m_far_reserve) {
          int   l_far_reserve = get_round_value(l_far_count, global::cache_small_max);
          arg*  l_far_ptr     = reinterpret_cast<arg*>(malloc(l_far_reserve * sizeof(arg)));
          if(l_far_ptr != nullptr) {
              int i_far_arg = 0;
              // initialise newly allocated arg vector: move existing arg instances to the new vector
              // and destruct the old ones
              if(m_arg_far != nullptr) {
                  while(i_far_arg < l_far_move - arg_near_reserve) {
                      new(l_far_ptr + i_far_arg) arg(std::move(m_arg_far[i_far_arg]));
                      m_arg_far[i_far_arg].~arg();
                      i_far_arg++;
                  }
              }
              // initialise newly allocated arg vector: construct the new instances
              while(i_far_arg < l_far_reserve - arg_near_reserve) {
                  new(l_far_ptr + i_far_arg) arg();
                  i_far_arg++;
              }
              // free the memory of the old array
              free(m_arg_far);
              // store the new array
              m_arg_far     = l_far_ptr;
              m_far_reserve = l_far_reserve;
              return true;
          }
          return false;
      }
      return true;
}

int   argv::arg_extract_next(char*& arg_str, char*& arg_iter) noexcept
{
      int  l_end_min = 0;
      int  l_end_max = SPC;

      // handle "" and '' arguments
      if(arg_str[0] == '"') {
          l_end_min = '"';
          l_end_max = '"';
          arg_iter[0] = 0;
          ++arg_iter;
          ++arg_str;
      } else
      if(arg_str[0] == '\'') {
          l_end_min = '\'';
          l_end_max = '\'';
          arg_iter[0] = 0;
          ++arg_iter;
          ++arg_str;
      }
      // parse the argument presumably starting at `arg_iter`: iterate until the first character within the range
      // `l_end_min`..`l_end_max` is encountered.
      while(*arg_iter <= ASCII_MAX) {
          if((*arg_iter >= l_end_min) &&
              (*arg_iter <= l_end_max)) {
              *arg_iter = 0;
              return arg_iter - arg_str;
          }
          arg_iter++;
      }
      return -1;
}

void  argv::assign(const argv& copy) noexcept
{
      if(this != std::addressof(copy)) {
          clear();
          if(copy.m_arg_count > 0) {
              bool l_reserve_success = arg_reserve(copy.m_arg_count, 0);
              if(l_reserve_success) {
                  for(int 
                        i_arg = 0;
                        (i_arg < copy.m_arg_count) && (i_arg < arg_near_reserve);
                        i_arg++) {
                        m_arg_near[i_arg] = copy.m_arg_near[i_arg];
                  }
                  for(int
                        i_arg = arg_near_reserve;
                        i_arg < copy.m_arg_count;
                        i_arg++) {
                        m_arg_far[i_arg - arg_near_reserve] = copy.m_arg_far[i_arg - arg_near_reserve];
                          
                  }
                  m_arg_count = copy.m_arg_count;
              }
          }
      }
}

void  argv::assign(argv&& copy) noexcept
{
      if(this != std::addressof(copy)) {
          dispose();
          clear();
          if(copy.m_arg_count > 0) {
              for(int 
                    i_arg = 0;
                    (i_arg < copy.m_arg_count) && (i_arg < arg_near_reserve);
                    i_arg++) {
                    m_arg_near[i_arg] = copy.m_arg_near[i_arg];
              }
              m_arg_far = copy.m_arg_far;
              m_arg_count = copy.m_arg_count;
              copy.release();
          }
      }
      copy.clear();
}

/* load()
   build argument list from string
*/
int   argv::load(char* str, int) noexcept
{
      char* l_str_iter  = str;
      int   l_arg_index = 0;
      while(*l_str_iter) {
          if((*l_str_iter >= SPC) &&
              (*l_str_iter <= ASCII_MAX)) {
              char* l_arg_base = l_str_iter;
              int   l_arg_length = arg_extract_next(l_arg_base, l_str_iter);
              if(l_arg_length >= 0) {
                  if(l_arg_index < arg_near_reserve) {
                      m_arg_near[l_arg_index] = arg(l_arg_base, l_arg_length);
                  } else
                  if(l_arg_index < arg_count_max) {
                      int  l_far_index   = l_arg_index - arg_near_reserve;
                      bool l_far_success = arg_reserve(l_arg_index + 1, l_arg_index);
                      if(l_far_success) {
                          m_arg_far[l_far_index] = arg(l_arg_base, l_arg_length);
                      }
                      else {
                          printdbg(
                              "Failed to resize argument list.\n",
                              __FILE__,
                              __LINE__
                          );
                          l_arg_index = 0;
                          break;
                      }
                  }
                  else {
                      printdbg(
                          "Too many argument list entries.\n",
                          __FILE__,
                          __LINE__
                      );
                      l_arg_index = 0;
                      break;
                  }
                  l_arg_index++;
              }
              else {
                  printdbg(
                      "Error while parsing argument %d.\n"
                      " \"  %s \" ",
                      __FILE__,
                      __LINE__,
                      l_arg_index,
                      str
                  );
                  l_arg_index = 0;
                  break;
              }
          }
          l_str_iter++;
      }
      m_arg_count = l_arg_index;
      return m_arg_count;
}

int   argv::load(char** argv, int argc) noexcept
{
      m_arg_count = 0;
      if((argc > 0) &&
          (argc < arg_count_max)) {
          bool l_reserve_success = arg_reserve(argc, 0);
          if(l_reserve_success) {
              for(int 
                    i_arg = 0;
                    (i_arg < argc) && (i_arg < arg_near_reserve);
                    i_arg++) {
                    m_arg_near[i_arg] = arg(argv[i_arg]);
              }
              for(int
                    i_arg = arg_near_reserve;
                    i_arg < argc;
                    i_arg++) {
                    m_arg_far[i_arg - arg_near_reserve] = arg(argv[i_arg]);
                      
              }
              m_arg_count = argc;
          }
      }
      return m_arg_count;
 }

const arg& argv::get_arg(int index) const noexcept
{
      if(index >= 0) {
          if(index < arg_near_reserve) {
              return m_arg_near[index];
          }
          return m_arg_far[index - arg_near_reserve];
      }
      return m_arg_none;
}

int   argv::get_index_of(const char* text) const noexcept
{
      if((text != nullptr) &&
          (text[0] != 0)) {
          for(int i_arg = 0; i_arg < m_arg_count; i_arg++) {
              const arg& r_arg = get_arg(i_arg);
              if(r_arg.has_text(text)) {
                  return i_arg;
              }
              i_arg++;
          }
      }
      return -1;
}

int   argv::get_index_of(arg& inst) const noexcept
{
      int i_arg = 0;
      while(i_arg < m_arg_count) {
          const arg& l_arg = get_arg(i_arg);
          if(std::addressof(inst) == std::addressof(l_arg)) {
              return i_arg;
          }
          i_arg++;
      }
      return -1;
}

bool  argv::has_arg(const char* name) const noexcept
{
      return get_index_of(name) >= 0;
}

const arg& argv::get_arg(const char* name) const noexcept
{
      int  i_arg = get_index_of(name);
      if(i_arg >= 0) {
          return get_arg(i_arg);
      }
      return m_arg_none;
}

const arg& argv::get_arg_relative(const char* base, int offset) const noexcept
{
      int  i_arg = get_index_of(base);
      if(i_arg >= 0) {
          int i_offset = i_arg + offset;
          if(i_offset < m_arg_count) {
              return get_arg(i_offset);
          }
      }
      return m_arg_none;
}

/* get_exec_ptr()
   create an arg list as for the exec/v/e() family of functions
*/
auto  argv::get_exec_ptr(char** arg_ptr, int arg_count) const noexcept -> char* const*
{
      if(arg_count > m_arg_count) {
          int i_arg = 0;
          while(i_arg < m_arg_count) {
              // NOTE: the `const_cast` is absolutely **not** how to handle this, but suspect `execv` has the wrong signature
              // and doesn't actually modify the contents of the strings
              arg_ptr[i_arg] = const_cast<char*>(get_arg(i_arg).get_text());
              i_arg++;
          }
          arg_ptr[i_arg] = nullptr;
          return arg_ptr;
      }
      return nullptr;
}

bool  argv::has_count(int count) const noexcept
{
      return get_count() == count;
}

int   argv::get_count() const noexcept
{
      return m_arg_count;
}

void  argv::clear() noexcept
{
      m_arg_count = 0;
}

void  argv::release() noexcept
{
      m_arg_far = nullptr;
      m_far_reserve = 0;
      if(m_arg_count > arg_near_reserve) {
          m_arg_count = arg_near_reserve;
      }
}

void  argv::dispose(bool reset) noexcept
{
      if(m_arg_far != nullptr) {
          for(int
                i_arg = arg_near_reserve;
                i_arg < m_arg_count;
                i_arg++) {
                m_arg_far[i_arg - arg_near_reserve].~arg();
          }
          free(m_arg_far);
          if(reset) {
              release();
          }
      }
}

const arg&  argv::operator[](int index) const noexcept
{
      return get_arg(index);
}

argv&  argv::operator=(const argv& rhs) noexcept
{
      assign(rhs);
      return *this;
}

argv&  argv::operator=(argv&& rhs) noexcept
{
      assign(std::move(rhs));
      return *this;
}

/*namespace sys*/ }
