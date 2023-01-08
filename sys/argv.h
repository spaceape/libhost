#ifndef sys_argv_h
#define sys_argv_h
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
#include <sys.h>
#include "arg.h"

namespace sys {

/* argv
   generic command line container and parser
*/
class argv
{
  /* arg_near_reserve
     how many arg instances to keep within the 'near' array
  */
  static constexpr int   arg_near_reserve = 6;

  /* arg_count_max
    maximum number of arguments to record
  */ 
  static constexpr int   arg_count_max = 1024;

  private:
  arg       m_arg_none;
  arg       m_arg_near[arg_near_reserve];   // statically allocated list for args with index [0..arg_near_reserve]
  arg*      m_arg_far;                      // dynamically allocated list for args with index [arg_far_reserve...]
  short int m_arg_lb;
  short int m_arg_ub;
  short int m_arg_count;
  short int m_far_reserve;                  // how many args are available onto the 'far' argument list

  private:
          bool         arg_reserve(int, int) noexcept;
          int          arg_extract_next(char*&, char*&, bool&) noexcept;

  public:
          argv() noexcept;
          argv(std::initializer_list<const char*>) noexcept;
          argv(char*, int = arg_count_max) noexcept;
          argv(char**, int) noexcept;
          argv(const argv&) noexcept;
          argv(argv&&) noexcept;
          ~argv();

          void        assign(const argv&) noexcept;
          void        assign(argv&&) noexcept;
          int         load(char*, int = arg_count_max) noexcept;
          int         load(char**, int = arg_count_max) noexcept;
          
          const arg&  shift() noexcept;
          const arg&  get_arg(int) const noexcept;
          const arg&  pop() noexcept;

          int         get_index_of(const char*) const noexcept;
          int         get_index_of(arg&) const noexcept;
   
          bool        has_arg(const char*) const noexcept;
          const arg&  get_arg(const char*) const noexcept;
          const arg&  get_arg_relative(const char*, int) const noexcept;

          auto        get_exec_ptr(char**, int) const noexcept -> char* const*;

          bool        has_count(int) const noexcept;
          int         get_count() const noexcept;

          void        clear() noexcept;
          void        release() noexcept;
          void        dispose(bool = true) noexcept;

          const arg&  operator[](int) const noexcept;
          argv&       operator=(const argv&) noexcept;
          argv&       operator=(argv&&) noexcept;
};

/*namespace sys*/ }
#endif
