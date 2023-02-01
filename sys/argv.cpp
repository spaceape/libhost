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
      m_arg_lb(0),
      m_arg_ub(0),
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

      argv::argv(argv&& copy) noexcept:
      argv()
{
      assign(std::move(copy));
      copy.release();
}

      argv::~argv()
{
      dispose(false);
}

bool  argv::arg_reserve(int count, int keep) noexcept
{
      int l_far_count = count - arg_near_reserve;   // how many 'far' instances to reserve memory for
      int l_far_move  = keep;                       // how many of the existing args to preserve into the new array
      if(l_far_count > m_far_reserve) {
          int   l_far_reserve = get_round_value(l_far_count, global::cache_small_max);
          arg*  l_far_ptr     = reinterpret_cast<arg*>(malloc(l_far_reserve * sizeof(arg)));
          if(l_far_ptr != nullptr) {
              int i_far_arg = 0;
              // initialise newly allocated arg vector: move existing arg instances to the new vector
              // and destruct the old ones
              // TODO: don't recreate arg instances on positions lower than `m_arg_lb`
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

int   argv::arg_extract_next(char*& arg_str, char*& arg_iter, bool& has_next) noexcept
{
      int  l_stop_char  = SPC;

      // handle "" and '' arguments
      if(arg_str[0] == '"') {
          l_stop_char = '"';
          arg_iter[0] = 0;
          ++arg_iter;
          ++arg_str;
      } else
      if(arg_str[0] == '\'') {
          l_stop_char = '\'';
          arg_iter[0] = 0;
          ++arg_iter;
          ++arg_str;
      }

      // parse the argument presumably starting at `arg_iter`
      // has_next = false;
      bool l_stop_found = l_stop_char == SPC;
      bool l_next_found = false;
      while(*arg_iter != NUL) {
          if(*arg_iter == l_stop_char) {
              l_next_found = true;
              l_stop_found = true;
              *arg_iter = NUL;
          } else
              arg_iter++;
      }
      has_next = l_next_found;
      if(l_stop_found) {
          return arg_iter - arg_str;
      }
      return -1;
}

void  argv::assign(const argv& copy) noexcept
{
      if(this != std::addressof(copy)) {
          clear();
          if(copy.get_count() > 0) {
              bool l_reserve_success = arg_reserve(copy.m_arg_count, 0);
              if(l_reserve_success) {
                  for(int 
                        i_arg = 0;
                        (i_arg < copy.get_count()) && (i_arg < arg_near_reserve);
                        i_arg++) {
                        m_arg_near[i_arg] = copy.get_arg(i_arg);
                  }
                  for(int
                        i_arg = arg_near_reserve;
                        i_arg < copy.get_count();
                        i_arg++) {
                        m_arg_far[i_arg - arg_near_reserve] = copy.get_arg(i_arg);
                          
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
          if(copy.get_count() > 0) {
              for(int 
                    i_arg = 0;
                    (i_arg < copy.m_arg_count) && (i_arg < arg_near_reserve);
                    i_arg++) {
                    m_arg_near[i_arg] = copy.m_arg_near[i_arg];
              }
              m_arg_far = copy.m_arg_far;
              m_arg_lb = copy.m_arg_lb;
              m_arg_ub = copy.m_arg_ub;
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
          bool l_has_next = false;
          if((*l_str_iter >= SPC) &&
              (*l_str_iter <= ASCII_MAX)) {
              char* l_arg_base = l_str_iter;
              int   l_arg_length = arg_extract_next(l_arg_base, l_str_iter, l_has_next);
              if(l_arg_length >= 0) {
                  if(l_arg_length > 0) {
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
          if(l_has_next) {
              l_str_iter++;
          }
      }
      m_arg_count = l_arg_index;
      m_arg_lb = 0;
      m_arg_ub = 0;
      return m_arg_count;
}

int   argv::load(char** argv, int argc) noexcept
{
      m_arg_lb = 0;
      m_arg_ub = 0;
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

const arg& argv::push(const char* p) noexcept
{
      if(p) {
          if(p[0]) {
              int  l_arg_index = m_arg_count - m_arg_ub;
              int  l_far_index = l_arg_index - arg_near_reserve;
              arg* l_arg_ptr = nullptr;
              if(l_arg_index < m_arg_count) {
                  if(l_arg_index < arg_near_reserve) {
                      l_arg_ptr = std::addressof(m_arg_near[l_arg_index]);
                  } else
                      l_arg_ptr = std::addressof(m_arg_far[l_arg_index - arg_near_reserve]);
                  m_arg_ub--;
              } else
              if(l_arg_index < arg_near_reserve) {
                  l_arg_ptr = std::addressof(m_arg_near[l_arg_index]);
                  m_arg_count++;
              } else
              if(l_far_index >= 0) {
                  if(l_far_index >= m_far_reserve) {
                      if(bool
                          l_reserve_success = arg_reserve(m_arg_count + global::cache_small_max, m_arg_count);
                          l_reserve_success == false) {
                          return m_arg_none;
                      }
                  }
                  l_arg_ptr = std::addressof(m_arg_far[l_far_index]);
                  m_arg_count++;
              }
              l_arg_ptr->set(p);
              return *l_arg_ptr;
          }
      }
      return m_arg_none;
}

const arg& argv::shift() noexcept
{
      if(m_arg_lb < m_arg_count - m_arg_ub) {
          if(m_arg_lb < arg_near_reserve) {
              return m_arg_near[m_arg_lb++];
          } else
              return m_arg_far[m_arg_lb++ - arg_near_reserve];
      } else
          return m_arg_none;
}

void  argv::shift(int count) noexcept
{
      while(count > 0) {
          shift();
          count--;
      }
}

const arg& argv::get_arg(int index) const noexcept
{
      if(index >= 0) {
          int l_tr_index = index + m_arg_lb;
          int l_tr_count = m_arg_count - m_arg_lb - m_arg_ub;
          if(index < l_tr_count) {
              if(l_tr_index < arg_near_reserve) {
                  return m_arg_near[l_tr_index];
              }
              return m_arg_far[l_tr_index - arg_near_reserve];
          }
      }
      return m_arg_none;
}

void  argv::pop(int count) noexcept
{
      while(count > 0) {
          pop();
          count--;
      }
}

const arg& argv::pop() noexcept
{
      int l_arg_index = m_arg_count - m_arg_ub - 1;
      if(l_arg_index >= m_arg_lb) {
          --m_arg_ub;
          if(l_arg_index < arg_near_reserve) {
              return m_arg_near[l_arg_index];
          } else
              return m_arg_far[l_arg_index - arg_near_reserve];
      } else
          return m_arg_none;
}

int   argv::get_index_of(const char* text) const noexcept
{
      if((text != nullptr) && (text[0] != 0)) {
          int l_arg_index = 0;
          int l_arg_count = get_count();
          while(l_arg_index < l_arg_count) {
              if(const arg& l_arg = get_arg(l_arg_index); l_arg.has_text(text)) {
                  return l_arg_index;
              }
              ++l_arg_index;
          }
      }
      return -1;
}

int   argv::get_index_of(arg& inst) const noexcept
{
      int l_arg_index = 0;
      int l_arg_count = get_count();
      while(l_arg_index < l_arg_count) {
          if(const arg& l_arg = get_arg(l_arg_index); std::addressof(l_arg) == std::addressof(inst)) {
              return l_arg_index;
          }
          ++l_arg_index;
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
      int l_arg_index = 0;
      int l_arg_count = get_count();
      while(l_arg_index < arg_count) {
          if(l_arg_index < l_arg_count) {
              // NOTE: the `const_cast` is absolutely **not** how to handle this, but suspect `execv` has the wrong signature
              // and doesn't actually modify the contents of the strings
              arg_ptr[l_arg_index] = const_cast<char*>(get_arg(l_arg_index).get_text());
          } else
              arg_ptr[l_arg_index] = nullptr;
          ++l_arg_index;
      }
      return arg_ptr;
}

bool  argv::has_count(int count) const noexcept
{
      return get_count() == count;
}

int   argv::get_count() const noexcept
{
      return m_arg_count - m_arg_lb - m_arg_ub;
}

bool  argv::reserve(int count) noexcept
{
      return arg_reserve(count, m_arg_count - m_arg_ub);
}

void  argv::clear() noexcept
{
      m_arg_count = 0;
      m_arg_lb = 0;
      m_arg_ub = 0;
}

void  argv::release() noexcept
{
      m_arg_far = nullptr;
      m_far_reserve = 0;
      m_arg_count = 0;
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

argv& argv::operator=(char* str) noexcept
{
      load(str);
      return *this;
}

argv& argv::operator=(const argv& rhs) noexcept
{
      assign(rhs);
      return *this;
}

argv& argv::operator=(argv&& rhs) noexcept
{
      assign(std::move(rhs));
      rhs.release();
      return *this;
}

/*namespace sys*/ }
