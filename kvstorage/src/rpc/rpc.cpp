#include "rpc/rpc.hpp"
#include <grpcpp/impl/service_type.h>

#include <memory>
#include <string>

RpcClient::RpcClient(const std::string& host, const std::string& port) {
    std::string address = host + ":" + port;
    this->channel =
        grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
}

void RpcServer::run() { this->server->Wait(); }

RpcServer::RpcServer(const std::string &host, const std::string &port, std::shared_ptr<grpc::Service> service) {
    std::string address = host + ":" + port;
    this->service = service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(this->service.get());
    this->server = builder.BuildAndStart();
}