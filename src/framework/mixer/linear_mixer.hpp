// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011,2012 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#pragma once

#include <string>
#include <pficommon/concurrent/thread.h>
#include <pficommon/concurrent/rwmutex.h>
#include <pficommon/concurrent/condition.h>
#include <pficommon/lang/shared_ptr.h>
#include "../../common/lock_service.hpp"
#include "../../common/shared_ptr.hpp"
#include "mixer.hpp"

namespace jubatus {
namespace framework {
namespace mixer {

class linear_mixer : public mixer {
public:
  linear_mixer(common::cshared_ptr<common::lock_service>& zk,
               const std::string& type, const std::string& name,
               int timeout_sec,
               unsigned int count_threshold, unsigned int tick_threshold);

  void register_api(pfi::network::mprpc::rpc_server& server);
  void register_mixable(const pfi::lang::shared_ptr<mixable0>& m);

  void start();
  void stop();

  void updated();

  void get_status(server_base::status_t& status) const;

private:
  void mixer_loop();
  void mix();

  void clear();

  std::vector<std::string> get_diff(int);
  int put_diff(const std::vector<std::string>& unpacked);

  common::cshared_ptr<common::lock_service> zk_;
  std::string type_;
  std::string name_;
  int timeout_sec_;
  unsigned int count_threshold_;
  unsigned int tick_threshold_;

  unsigned int counter_;
  unsigned int ticktime_;
  unsigned int mix_count_;

  volatile bool is_running_;

  pfi::concurrent::thread t_;
  pfi::concurrent::rw_mutex m_;
  pfi::concurrent::condition c_;

  std::vector<pfi::lang::shared_ptr<mixable0> > mixables_;
};

}
}
}