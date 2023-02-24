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
#include "process.h"
#ifdef   sys_process_h
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/argv.h>
#include <sys/wait.h>

      constexpr unsigned int s_status_idle = 0;
      constexpr unsigned int s_status_ready = 1u;
      constexpr unsigned int s_status_pause = 2u;
      constexpr unsigned int s_status_done = 4u;
      constexpr unsigned int s_status_suspend_soft = 16u;
      constexpr unsigned int s_status_suspend_hard = 32u;

/* s_kill_time_default
   how much to wait to force-kill a process before issuing a soft suspend
*/
      constexpr float        s_kill_time_default = 10.0f;

namespace sys {

      process::process(monitor* monitor_ptr) noexcept:
      m_id(0),
      m_rc(0),
      m_status(s_status_idle),
      m_pwd(),
      m_kill_timeout(s_kill_time_default),
      m_listen(false),
      m_block(false),
      m_delete(false),
      m_stdin_pipe{undef, undef},
      m_stdout_pipe{undef, undef},
      m_stderr_pipe{undef, undef},
      m_poll_list{undef, undef},
      m_monitor_ptr(monitor_ptr),
      m_kill_time(s_kill_time_default)
{
}

      process::~process()
{
      if(m_status & s_status_ready) {
          suspend(true);
          waitpid(m_id, std::addressof(m_rc), 0);
      }
      ppi_dispose(false);
}

      process::monitor::monitor() noexcept
{
}

      process::monitor::monitor(const monitor&) noexcept
{
}

      process::monitor::monitor(monitor&&) noexcept
{
}

      process::monitor::~monitor()
{
}

void  process::monitor::psm_launch(process*) noexcept
{
}

void  process::monitor::psm_feed_stdout(process*, int, int) noexcept
{
}

void  process::monitor::psm_feed_stderr(process*, int, int) noexcept
{
}

void  process::monitor::psm_finish(process*, int) noexcept
{
}

void  process::monitor::psm_error(process*) noexcept
{
}

process::monitor& process::monitor::operator=(const monitor&) noexcept
{
      return *this;
}

process::monitor& process::monitor::operator=(monitor&&) noexcept
{
      return *this;
}

bool  process::ppi_set_blocking(int descriptor, bool blocking) noexcept
{
      if(int 
          l_get_flags = fcntl(descriptor, F_GETFL, 0);
          l_get_flags >= 0) {
          int l_set_flags = l_get_flags;
          if(blocking == true) {
              l_set_flags &= ~O_NONBLOCK;
          } else
          if(blocking == false) {
              l_set_flags |=  O_NONBLOCK;
          }
          if(l_set_flags != l_get_flags) {
              if(int l_fnctl = fcntl(descriptor, F_SETFL, l_set_flags); l_fnctl >= 0) {
                  return true;
              }
          } else
              return true;
      }
      return false;
}

void  process::ppi_dispose(bool clear) noexcept
{
      if(m_stderr_pipe[1] != undef) {
          close(m_stderr_pipe[1]);
      }
      if(m_stderr_pipe[0] != undef) {
          ppi_set_blocking(m_stderr_pipe[0], true);
          close(m_stderr_pipe[0]);
      }
      if(m_stdout_pipe[1] != undef) {
          close(m_stdout_pipe[1]);
      }
      if(m_stdout_pipe[0] != undef) {
          ppi_set_blocking(m_stdout_pipe[0], true);
          close(m_stdout_pipe[0]);
      }
      if(m_stdin_pipe[1] != undef) {
          close(m_stdin_pipe[1]);
      }
      if(m_stdin_pipe[0] != undef) {
          close(m_stdin_pipe[0]);
      }
      if(clear) {
          m_stdin_pipe[0] = undef;
          m_stdin_pipe[1] = undef;
          m_stdout_pipe[0] = undef;
          m_stdout_pipe[1] = undef;
          m_stderr_pipe[0] = undef;
          m_stderr_pipe[1] = undef;
      }
}

bool  process::has_pid(pid_t pid) const noexcept
{
      return m_id == pid;
}

pid_t process::get_pid() const noexcept
{
      return m_id;
}

void  process::set_cwd(const std::filesystem::path& pwd) noexcept
{
      m_pwd = pwd;
}

void  process::set_kill_time(float time) noexcept
{
      if(time < 0.0f) {
          time = -1.0f;
      }
      if(time >= 0.0f) {
          m_kill_timeout = time;
          if(m_status & s_status_ready) {
              if(((m_status & s_status_suspend_soft) == 0) &&
                  ((m_status & s_status_suspend_hard) == 0)) {
                  m_kill_time = time;
              }
          }
      }
}

void  process::set_descriptor_blocking(bool value) noexcept
{
      m_block = value;
      if(m_status & s_status_ready) {
          ppi_set_blocking(m_stdout_pipe[0], m_block);
          ppi_set_blocking(m_stderr_pipe[0], m_block);
      }
}

void  process::set_descriptor_polling(bool value) noexcept
{
      m_listen = value;
}

bool  process::get_delete() const noexcept
{
      return m_delete;
}

void  process::set_delete(bool value) noexcept
{
      m_delete = value;
}

bool  process::resume() noexcept
{
      int l_rc;
      if(m_status & s_status_pause) {
          l_rc = kill(m_id, SIGCONT);
          if(l_rc == 0) {
              m_status ^= s_status_pause;
              return true;
          }
      }
      return false;
}

bool  process::resume(const char* bin, const sys::argv& argv) noexcept
{
      int l_fork_rc;

      if(m_status & s_status_ready) {
          return false;
      }

      // set up bidirectional communication with the child process
      if(pipe(m_stdin_pipe) != 0) {
          return false;
      }
      if(pipe(m_stdout_pipe) != 0) {
          return false;
      }
      if(pipe(m_stderr_pipe) != 0) {
          return false;
      }
   
      // fork
      l_fork_rc = fork();
      if(l_fork_rc == 0) {
          if(dup2(m_stdin_pipe[0], STDIN_FILENO) < 0) {
              exit(EXIT_FAILURE);
          }
          if(dup2(m_stdout_pipe[1], STDOUT_FILENO) < 0) {
              exit(EXIT_FAILURE);
          }
          if(dup2(m_stderr_pipe[1], STDERR_FILENO) < 0) {
              exit(EXIT_FAILURE);
          }
          close(m_stdin_pipe[1]);
          close(m_stdout_pipe[0]);
          close(m_stderr_pipe[0]);
          if(m_pwd.empty() == false) {
              std::error_code  l_rc;
              std::filesystem::current_path(m_pwd, l_rc);
              if(l_rc.value() != 0) {
                  fprintf(stderr, "Unable to change directory to \"%s\".\n", m_pwd.c_str());
                  exit(EXIT_FAILURE);
              }
          }
          if(true) {
              int    l_argc = argv.get_count() + 1;
              char** p_args = reinterpret_cast<char**>(alloca(l_argc * sizeof(void*)));
              auto   l_argv = argv.get_exec_ptr(p_args, l_argc);
              if(l_argv) {
                  execv(bin, l_argv);
              }
          }
          exit(EXIT_FAILURE);
      } else
      if(l_fork_rc > 0) {
          m_id = l_fork_rc;
          m_rc = EXIT_SUCCESS;
          m_status = s_status_ready;
          m_kill_time = m_kill_timeout;
          if(m_monitor_ptr) {
              m_monitor_ptr->psm_launch(this);
          }
          ppi_set_blocking(m_stdout_pipe[0], m_block);
          ppi_set_blocking(m_stderr_pipe[0], m_block);
          return true;
      } else
          m_monitor_ptr->psm_error(this);
      return false;
}

bool  process::pause() noexcept
{
      int l_rc;
      if(m_status & s_status_ready) {
          if(((m_status & s_status_suspend_soft) == 0) &&
              ((m_status & s_status_suspend_hard) == 0)) {
              if((m_status & s_status_pause) == 0) {
                  l_rc = kill(m_id, SIGSTOP);
                  if(l_rc == 0) {
                      m_status |= s_status_pause;
                  }
              }
              return m_status & s_status_pause;
          }
      }
      return false;
}

bool  process::suspend(bool force) noexcept
{
      int l_rc;
      if(m_status & s_status_ready) {
          if(m_status & s_status_suspend_hard) {
              return true;
          }
          if(m_status & s_status_pause) {
              l_rc = kill(m_id, SIGCONT);
              if(l_rc == 0) {
                  m_status ^= s_status_pause;
              } else
              if(l_rc < 0) {
                  return false;
              }
          }
          if(force) {
              if((m_status & s_status_suspend_hard) == 0) {
                  l_rc = kill(m_id, SIGKILL);
                  if(l_rc == 0) {
                      m_status |= s_status_suspend_hard;
                  }
              }
          } else
          if((m_status & s_status_suspend_soft) == 0) {
              l_rc = kill(m_id, SIGTERM);
              if(l_rc == 0) {
                  m_status |= s_status_suspend_soft;
              }
          }
      }
      return false;
}

int   process::get_stdin_descriptor() const noexcept
{
      return m_stdin_pipe[1];
}

int   process::get_stdout_descriptor() const noexcept
{
      return m_stdout_pipe[0];
}

int   process::get_stderr_descriptor() const noexcept
{
      return m_stderr_pipe[0];
}

bool  process::is_launched() const noexcept
{
      return m_status & s_status_ready;
}

bool  process::is_finished() const noexcept
{
      return m_status & s_status_done;
}

bool  process::operator==(int id) const noexcept
{
      return m_id == id;
}

bool  process::operator!=(int id) const noexcept
{
      return m_id != id;
}

bool  process::operator==(process& rhs) const noexcept
{
      if(m_id) {
          return m_id == rhs.m_id;
      } else
          return false;
}

bool  process::operator!=(process& rhs) const noexcept
{
      return operator==(rhs) == false;
}

      process::operator bool() const noexcept
{
      return m_id;
}

void  process::sync(float dt) noexcept
{
      int       l_rc;
      siginfo_t l_si;
      if(m_status & s_status_ready) {
          l_rc = waitid(P_PID, m_id, std::addressof(l_si), WUNTRACED | WEXITED | WNOHANG);
          if(l_rc >= 0) {
              if(m_listen) {
                  if(m_monitor_ptr) {
                      if(m_stdout_pipe[0] != undef) {
                          m_monitor_ptr->psm_feed_stdout(this, m_stdout_pipe[0], m_stdin_pipe[1]);
                      }
                      if(m_stderr_pipe[0] != undef) {
                          m_monitor_ptr->psm_feed_stderr(this, m_stderr_pipe[0], m_stdin_pipe[1]);
                      }
                  }
              }
              if(l_si.si_code == CLD_EXITED) {
                  m_rc = l_si.si_status;
                  if(m_monitor_ptr) {
                      m_monitor_ptr->psm_finish(this, m_rc);
                  }
                  m_status ^= s_status_ready;
                  m_status = s_status_done;
                  ppi_dispose(true);
              } else
              if(((m_status & s_status_suspend_soft) != 0) &&
                  ((m_status & s_status_suspend_hard) == 0)) {
                  // send a SIGKILL to the process if it took longer than `m_kill_time` to terminate
                  if(m_kill_time >= 0.0f) {
                      m_kill_time -= dt;
                      if(m_kill_time <= 0.0f) {
                          suspend(true);
                      }
                  }
              }
          } else
          if(l_rc < 0) {
              if(m_monitor_ptr) {
                  m_status = s_status_done;
                  m_monitor_ptr->psm_error(this);
              }
          }
      }
}

/*namespace sys*/ }
#endif
