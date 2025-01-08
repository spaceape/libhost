/**
    Copyright (c) 2018, wicked systems
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
#include "log.h"
#include <time.h>
#include <stdio.h>
#include "dbg.h"

// static const char*  s_tag_none = "    ";
static const char*  s_tag_info = "(i) ";

static FILE*  g_log_out = stderr;
static FILE*  g_log_aux = nullptr;

void  vprintlog(const char* message, const char* file, int line, va_list va_out) noexcept
{
      if(g_log_out != nullptr) {
          auto    l_tag = s_tag_info;
          va_list va_aux;
          if(g_log_aux != nullptr) {
              va_copy(va_aux, va_out);
              fprintf(g_log_aux, "%.8lx %s", time(nullptr), l_tag);
              vfprintf(g_log_aux, message, va_aux);
              fprintf(g_log_aux, "\n");
              fflush(g_log_aux);
          }
          fprintf(g_log_out, l_tag);
          vfprintf(g_log_out, message, va_out);
          fprintf(g_log_out, "\n");
#ifndef NDEBUG
          if(file != nullptr) {
              if(line > 0) {
                  if(g_log_aux) {
                      fprintf(g_log_aux, "    %s:%d\n", file, line);
                  }
                  fprintf(g_log_out, "    %s:%d\n", file, line);
              }
          }
#endif
      }
}

void  printlog(const char* message, ...) noexcept
{
      va_list va_out;
      if(g_log_aux != nullptr) {
          va_list va_aux;
          va_copy(va_aux, va_out);
          va_start(va_aux, message);
          fprintf(g_log_aux, "%.8lx ", time(nullptr));
          vfprintf(g_log_aux, message, va_aux);
          va_end(va_aux);
          fprintf(g_log_aux, "\n");
          fflush(g_log_aux);
      }
      va_start(va_out, message);
      vfprintf(g_log_out, message, va_out);
      va_end(va_out);
      fprintf(g_log_out, "\n");
}

void  printlog(const char* message, const char* file, int line, ...) noexcept
{
      va_list va_out;
      va_start(va_out, line);
      vprintlog(message, file, line, va_out);
      va_end(va_out);
}

void  printdbg(const char* message, const char* file, int line, ...) noexcept
{
#if DEBUG
      va_list va_list;
      va_start(va_list, line);
      if(DEBUG >= dbg_level_verbose) {
          vprintlog(message, file, line, va_list);
      }
      va_end(va_list);
#endif
}

void  printdbgi(const char* message, const char* file, int line, ...) noexcept
{
#if DEBUG
      va_list va_list;
      va_start(va_list, line);
      if(DEBUG >= dbg_level_info) {
          vprintlog(message, file, line, va_list);
      }
      va_end(va_list);
#endif
}

void  printdbgw(const char* message, const char* file, int line, ...) noexcept
{
#if DEBUG
      va_list va_list;
      va_start(va_list, line);
      if(DEBUG >= dbg_level_warning) {
          vprintlog(message, file, line, va_list);
      }
      va_end(va_list);
#endif
}

bool  log_aux_open(const char* filename) noexcept
{
      log_aux_close();
      g_log_aux = fopen(filename, "w+");
      if(g_log_aux != nullptr) {
          return true;
      }
      return false;
}

void  log_aux_close() noexcept
{
      if(g_log_aux != nullptr) {
          fclose(g_log_aux);
          g_log_aux = nullptr;
      }
}

__attribute__((destructor))
void  log_dispose() noexcept {
      log_aux_close();
}
