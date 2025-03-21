// Copyright 2023 DeepMind Technologies Limited
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// An implementation of the `Agent` gRPC service.

#ifndef GRPC_AGENT_SERVICE_H
#define GRPC_AGENT_SERVICE_H

#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>
#include <mujoco/mujoco.h>
#include "grpc/agent.grpc.pb.h"
#include "grpc/agent.pb.h"
#include "mjpc/agent.h"
#include "mjpc/task.h"
#include "mjpc/threadpool.h"
#include "mjpc/utilities.h"

namespace agent_grpc {

class AgentService final : public agent::Agent::Service {
 public:
  explicit AgentService(std::vector<std::shared_ptr<mjpc::Task>> tasks)
      : thread_pool_(mjpc::NumAvailableHardwareThreads()),
        tasks_(std::move(tasks)) {}
  ~AgentService();
  grpc::Status Init(grpc::ServerContext* context,
                    const agent::InitRequest* request,
                    agent::InitResponse* response) override;

  grpc::Status GetState(grpc::ServerContext* context,
                        const agent::GetStateRequest* request,
                        agent::GetStateResponse* response) override;

  grpc::Status SetState(grpc::ServerContext* context,
                        const agent::SetStateRequest* request,
                        agent::SetStateResponse* response) override;

  grpc::Status GetAction(grpc::ServerContext* context,
                         const agent::GetActionRequest* request,
                         agent::GetActionResponse* response) override;

  grpc::Status PlannerStep(grpc::ServerContext* context,
                           const agent::PlannerStepRequest* request,
                           agent::PlannerStepResponse* response) override;

  grpc::Status Step(grpc::ServerContext* context,
                    const agent::StepRequest* request,
                    agent::StepResponse* response) override;

  grpc::Status Reset(grpc::ServerContext* context,
                     const agent::ResetRequest* request,
                     agent::ResetResponse* response) override;

  grpc::Status SetTaskParameter(
      grpc::ServerContext* context,
      const agent::SetTaskParameterRequest* request,
      agent::SetTaskParameterResponse* response) override;

  grpc::Status SetCostWeights(
      grpc::ServerContext* context,
      const agent::SetCostWeightsRequest* request,
      agent::SetCostWeightsResponse* response) override;

 private:
  mjpc::ThreadPool thread_pool_;
  mjpc::Agent agent_;
  std::vector<std::shared_ptr<mjpc::Task>> tasks_;
  mjData* data_ = nullptr;
};

}  // namespace agent_grpc

#endif  // GRPC_AGENT_SERVICE_H
