#ifdef  LINUX
#ifndef sys_process_h
#define sys_process_h
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
#include <filesystem>
#include "arg.h"
#include "argv.h"

namespace sys {

class process
{
  pid_t         m_id;
  int           m_rc;
  unsigned int  m_status;

  std::filesystem::path   m_pwd;
  float                   m_kill_timeout;
  bool                    m_listen;
  bool                    m_block;
  bool                    m_delete;

  protected:
  int           m_stdin_pipe[2];
  int           m_stdout_pipe[2];
  int           m_stderr_pipe[2];
  int           m_poll_list[2];

  public:
  class   monitor
  {
    protected:
    virtual void  psm_launch(process*) noexcept;
    virtual void  psm_feed_stdout(process*, int, int) noexcept;
    virtual void  psm_feed_stderr(process*, int, int) noexcept;
    virtual void  psm_finish(process*, int) noexcept;
    virtual void  psm_error(process*) noexcept;
    friend  class process;
    public:
            monitor() noexcept;
            monitor(const monitor&) noexcept;
            monitor(monitor&&) noexcept;
    virtual ~monitor();
            monitor& operator=(const monitor&) noexcept;
            monitor& operator=(monitor&&) noexcept;
  };

  private:
  monitor*  m_monitor_ptr;
  float     m_kill_time;

  protected:
          bool  ppi_set_blocking(int, bool) noexcept;
          void  ppi_dispose(bool = true) noexcept;

  public:
          process(monitor* = nullptr) noexcept;
          process(const process&) noexcept = delete;
          process(process&&) noexcept = delete;
          ~process();

          bool  has_pid(pid_t) const noexcept;
          pid_t get_pid() const noexcept;

          void  set_cwd(const std::filesystem::path&) noexcept;
          void  set_kill_time(float) noexcept;
          void  set_descriptor_blocking(bool) noexcept;
          void  set_descriptor_polling(bool) noexcept;
          bool  get_delete() const noexcept;
          void  set_delete(bool) noexcept;

          bool  resume() noexcept;
          bool  resume(const char*, const sys::argv&) noexcept;
          bool  pause() noexcept;
          bool  suspend(bool = false) noexcept;

          int   get_stdin_descriptor() const noexcept;
          int   get_stdout_descriptor() const noexcept;
          int   get_stderr_descriptor() const noexcept;

          bool  is_launched() const noexcept;
          bool  is_finished() const noexcept;

          bool  operator==(int) const noexcept;
          bool  operator!=(int) const noexcept;
          bool  operator==(process&) const noexcept;
          bool  operator!=(process&) const noexcept;

          operator bool() const noexcept;

          void   sync(float) noexcept;

          process& operator=(const process&) noexcept = delete;
          process& operator=(process&&) noexcept = delete;
};

/*namespace sys*/ }
#endif
#endif