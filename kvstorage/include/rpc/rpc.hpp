#pragma once

#include <grpcpp/channel.h>
#include <grpcpp/grpcpp.h>

#include <memory>
#include <string>

class RpcClient {
public:
    RpcClient(const std::string &host, const std::string &port);

protected:
    std::shared_ptr<grpc::Channel> channel;
};

/**
 * @brief
 * 服务端实例，具体的函数执行由service实现，服务端只负服务的注册与维护；
 *
 */
class RpcServer {
public:
    RpcServer(const std::string &host, const std::string &port, std::shared_ptr<grpc::Service> service);
    void run();

protected:
    std::unique_ptr<grpc::Server> server;
    std::shared_ptr<grpc::Service> service;
};