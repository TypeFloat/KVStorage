// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: raft.proto

#include "raft.pb.h"
#include "raft.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace RaftRpcProtoc {

static const char* RaftRpc_method_names[] = {
  "/RaftRpcProtoc.RaftRpc/AppendEntries",
  "/RaftRpcProtoc.RaftRpc/InstallSnapshot",
  "/RaftRpcProtoc.RaftRpc/RequestVote",
};

std::unique_ptr< RaftRpc::Stub> RaftRpc::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< RaftRpc::Stub> stub(new RaftRpc::Stub(channel, options));
  return stub;
}

RaftRpc::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_AppendEntries_(RaftRpc_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_InstallSnapshot_(RaftRpc_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_RequestVote_(RaftRpc_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status RaftRpc::Stub::AppendEntries(::grpc::ClientContext* context, const ::RaftRpcProtoc::AppendEntriesRequest& request, ::RaftRpcProtoc::AppendEntriesResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::RaftRpcProtoc::AppendEntriesRequest, ::RaftRpcProtoc::AppendEntriesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_AppendEntries_, context, request, response);
}

void RaftRpc::Stub::async::AppendEntries(::grpc::ClientContext* context, const ::RaftRpcProtoc::AppendEntriesRequest* request, ::RaftRpcProtoc::AppendEntriesResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::RaftRpcProtoc::AppendEntriesRequest, ::RaftRpcProtoc::AppendEntriesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_AppendEntries_, context, request, response, std::move(f));
}

void RaftRpc::Stub::async::AppendEntries(::grpc::ClientContext* context, const ::RaftRpcProtoc::AppendEntriesRequest* request, ::RaftRpcProtoc::AppendEntriesResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_AppendEntries_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::RaftRpcProtoc::AppendEntriesResponse>* RaftRpc::Stub::PrepareAsyncAppendEntriesRaw(::grpc::ClientContext* context, const ::RaftRpcProtoc::AppendEntriesRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::RaftRpcProtoc::AppendEntriesResponse, ::RaftRpcProtoc::AppendEntriesRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_AppendEntries_, context, request);
}

::grpc::ClientAsyncResponseReader< ::RaftRpcProtoc::AppendEntriesResponse>* RaftRpc::Stub::AsyncAppendEntriesRaw(::grpc::ClientContext* context, const ::RaftRpcProtoc::AppendEntriesRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncAppendEntriesRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status RaftRpc::Stub::InstallSnapshot(::grpc::ClientContext* context, const ::RaftRpcProtoc::InstallSnapshotRequest& request, ::RaftRpcProtoc::InstallSnapshotResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::RaftRpcProtoc::InstallSnapshotRequest, ::RaftRpcProtoc::InstallSnapshotResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_InstallSnapshot_, context, request, response);
}

void RaftRpc::Stub::async::InstallSnapshot(::grpc::ClientContext* context, const ::RaftRpcProtoc::InstallSnapshotRequest* request, ::RaftRpcProtoc::InstallSnapshotResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::RaftRpcProtoc::InstallSnapshotRequest, ::RaftRpcProtoc::InstallSnapshotResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_InstallSnapshot_, context, request, response, std::move(f));
}

void RaftRpc::Stub::async::InstallSnapshot(::grpc::ClientContext* context, const ::RaftRpcProtoc::InstallSnapshotRequest* request, ::RaftRpcProtoc::InstallSnapshotResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_InstallSnapshot_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::RaftRpcProtoc::InstallSnapshotResponse>* RaftRpc::Stub::PrepareAsyncInstallSnapshotRaw(::grpc::ClientContext* context, const ::RaftRpcProtoc::InstallSnapshotRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::RaftRpcProtoc::InstallSnapshotResponse, ::RaftRpcProtoc::InstallSnapshotRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_InstallSnapshot_, context, request);
}

::grpc::ClientAsyncResponseReader< ::RaftRpcProtoc::InstallSnapshotResponse>* RaftRpc::Stub::AsyncInstallSnapshotRaw(::grpc::ClientContext* context, const ::RaftRpcProtoc::InstallSnapshotRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncInstallSnapshotRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status RaftRpc::Stub::RequestVote(::grpc::ClientContext* context, const ::RaftRpcProtoc::RequestVoteRequest& request, ::RaftRpcProtoc::RequestVoteResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::RaftRpcProtoc::RequestVoteRequest, ::RaftRpcProtoc::RequestVoteResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_RequestVote_, context, request, response);
}

void RaftRpc::Stub::async::RequestVote(::grpc::ClientContext* context, const ::RaftRpcProtoc::RequestVoteRequest* request, ::RaftRpcProtoc::RequestVoteResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::RaftRpcProtoc::RequestVoteRequest, ::RaftRpcProtoc::RequestVoteResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RequestVote_, context, request, response, std::move(f));
}

void RaftRpc::Stub::async::RequestVote(::grpc::ClientContext* context, const ::RaftRpcProtoc::RequestVoteRequest* request, ::RaftRpcProtoc::RequestVoteResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RequestVote_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::RaftRpcProtoc::RequestVoteResponse>* RaftRpc::Stub::PrepareAsyncRequestVoteRaw(::grpc::ClientContext* context, const ::RaftRpcProtoc::RequestVoteRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::RaftRpcProtoc::RequestVoteResponse, ::RaftRpcProtoc::RequestVoteRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_RequestVote_, context, request);
}

::grpc::ClientAsyncResponseReader< ::RaftRpcProtoc::RequestVoteResponse>* RaftRpc::Stub::AsyncRequestVoteRaw(::grpc::ClientContext* context, const ::RaftRpcProtoc::RequestVoteRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncRequestVoteRaw(context, request, cq);
  result->StartCall();
  return result;
}

RaftRpc::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftRpc_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaftRpc::Service, ::RaftRpcProtoc::AppendEntriesRequest, ::RaftRpcProtoc::AppendEntriesResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RaftRpc::Service* service,
             ::grpc::ServerContext* ctx,
             const ::RaftRpcProtoc::AppendEntriesRequest* req,
             ::RaftRpcProtoc::AppendEntriesResponse* resp) {
               return service->AppendEntries(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftRpc_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaftRpc::Service, ::RaftRpcProtoc::InstallSnapshotRequest, ::RaftRpcProtoc::InstallSnapshotResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RaftRpc::Service* service,
             ::grpc::ServerContext* ctx,
             const ::RaftRpcProtoc::InstallSnapshotRequest* req,
             ::RaftRpcProtoc::InstallSnapshotResponse* resp) {
               return service->InstallSnapshot(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftRpc_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaftRpc::Service, ::RaftRpcProtoc::RequestVoteRequest, ::RaftRpcProtoc::RequestVoteResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RaftRpc::Service* service,
             ::grpc::ServerContext* ctx,
             const ::RaftRpcProtoc::RequestVoteRequest* req,
             ::RaftRpcProtoc::RequestVoteResponse* resp) {
               return service->RequestVote(ctx, req, resp);
             }, this)));
}

RaftRpc::Service::~Service() {
}

::grpc::Status RaftRpc::Service::AppendEntries(::grpc::ServerContext* context, const ::RaftRpcProtoc::AppendEntriesRequest* request, ::RaftRpcProtoc::AppendEntriesResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaftRpc::Service::InstallSnapshot(::grpc::ServerContext* context, const ::RaftRpcProtoc::InstallSnapshotRequest* request, ::RaftRpcProtoc::InstallSnapshotResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaftRpc::Service::RequestVote(::grpc::ServerContext* context, const ::RaftRpcProtoc::RequestVoteRequest* request, ::RaftRpcProtoc::RequestVoteResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace RaftRpcProtoc
