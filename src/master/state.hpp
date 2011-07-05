/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MASTER_STATE_HPP__
#define __MASTER_STATE_HPP__

#include <iostream>
#include <string>
#include <vector>

#include "common/foreach.hpp"

#include "config/config.hpp"


// TODO(...): Make all the variable naming in here consistant with the
// rest of the code base. This will require cleaning up some Python code.


namespace mesos { namespace internal { namespace master { namespace state {

struct SlaveResources
{  
  std::string slave_id;
  double cpus;
  double mem;
  
  SlaveResources(std::string _slaveId, double _cpus, double _mem)
    : slave_id(_slaveId), cpus(_cpus), mem(_mem) {}
};


struct Offer
{  
  std::string id;
  std::string framework_id;
  std::vector<SlaveResources *> resources;
  
  Offer(std::string _id, std::string _frameworkId)
    : id(_id), framework_id(_frameworkId) {}
    
  ~Offer()
  {
    foreach (SlaveResources *sr, resources)
      delete sr;
  }
};


struct Slave
{
  Slave(std::string id_, const std::string& host_,
        const std::string& web_ui_url_,
	double cpus_, double mem_, double connect_)
    : id(id_), host(host_), web_ui_url(web_ui_url_),
      cpus(cpus_), mem(mem_), connect_time(connect_) {}

  Slave() {}

  std::string id;
  std::string host;
  std::string web_ui_url;
  double cpus;
  double mem;
  double connect_time;
};


struct Task
{
  Task(std::string id_, const std::string& name_, std::string framework_id_,
       std::string slaveId_, std::string state_, double _cpus, double _mem)
    : id(id_), name(name_), framework_id(framework_id_), slave_id(slaveId_),
      state(state_), cpus(_cpus), mem(_mem) {}

  Task() {}

  std::string id;
  std::string name;
  std::string framework_id;
  std::string slave_id;
  std::string state;
  double cpus;
  double mem;
};


struct Framework
{
  Framework(std::string id_, const std::string& user_,
            const std::string& name_, const std::string& executor_,
            double cpus_, double mem_, double connect_)
    : id(id_), user(user_), name(name_), executor(executor_),
      cpus(cpus_), mem(mem_), connect_time(connect_) {}

  Framework() {}

  ~Framework()
  {
    foreach (Task *task, tasks)
      delete task;
    foreach (Offer *offer, offers)
      delete offer;
  }

  std::string id;
  std::string user;
  std::string name;
  std::string executor;
  double cpus;
  double mem;
  double connect_time;

  std::vector<Task *> tasks;
  std::vector<Offer *> offers;
};


struct MasterState
{
  MasterState(const std::string& build_date_, const std::string& build_user_,
	      const std::string& pid_)
    : build_date(build_date_), build_user(build_user_), pid(pid_) {}

  MasterState() {}

  ~MasterState()
  {
    foreach (Slave *slave, slaves)
      delete slave;
    foreach (Framework *framework, frameworks)
      delete framework;
  }

  std::string build_date;
  std::string build_user;
  std::string pid;

  std::vector<Slave *> slaves;
  std::vector<Framework *> frameworks;
};

}}}} // namespace mesos { namespace internal { namespace master { namespace state {

#endif // __MASTER_STATE_HPP__
