#include "calculator.grpc.pb.h"
#include <memory>
#include <iostream>
#include <gtest/gtest.h>
#include "rpc/rpc.hpp"

using namespace grpc;
using namespace calculator;

class CalculatorClient : public RpcClient {
public:
    CalculatorClient(const std::string &host, const std::string &port)
            : RpcClient(host, port), stub(Calculator::NewStub(this->channel)) {
    }

    int64_t Add(int32_t addend_1, int32_t addend_2) {
        // Prepare request
        AddRequest request;
        request.set_addend_1(addend_1);
        request.set_addend_2(addend_2);

        // Send request
        AddResponse response;
        ClientContext context;
        Status status;
        status = stub->Add(&context, request, &response);

        // Handle response
        if (status.ok()) {
            return response.sum();
        } else {
            throw std::runtime_error(std::to_string(status.error_code()) + ": " + status.error_message());
        }
    }

    int64_t Sub(int32_t minuend, int32_t subtrahend) {
        // Prepare request
        SubRequest request;
        request.set_minuend(minuend);
        request.set_subtrahend(subtrahend);

        // Send request
        SubResponse response;
        ClientContext context;
        Status status;
        status = stub->Sub(&context, request, &response);

        // Handle repsonse
        if (status.ok()) {
            return response.difference();
        } else {
            throw std::runtime_error(std::to_string(status.error_code()) + ": " + status.error_message());
        }
    }

    int64_t Mul(int32_t multiplicand, int32_t multiplier) {
        // Prepare request
        MulRequest request;
        request.set_multiplicand(multiplicand);
        request.set_multiplier(multiplier);

        // Send request
        MulResponse response;
        ClientContext context;
        Status status;
        status = stub->Mul(&context, request, &response);

        // Handle response
        if (status.ok()) {
            return response.product();
        } else {
            throw std::runtime_error(std::to_string(status.error_code()) + ": " + status.error_message());
        }
    }

    int64_t Div(int32_t dividend, int32_t divisor) {
        // Prepare request
        DivRequest request;
        request.set_dividend(dividend);
        request.set_divisor(divisor);

        // Send request
        DivResponse response;
        ClientContext context;
        Status status;
        status = stub->Div(&context, request, &response);

        // Handle response
        if (status.ok()) {
            return response.quotient();
        } else {
            throw std::runtime_error(std::to_string(status.error_code()) + ": " + status.error_message());
        }
    }

    int64_t Mod(int32_t dividend, int32_t divisor) {
        // Prepare request
        ModRequest request;
        request.set_dividend(dividend);
        request.set_divisor(divisor);

        // Send request
        ModResponse response;
        ClientContext context;
        Status status;
        status = stub->Mod(&context, request, &response);

        // Handle response
        if (status.ok()) {
            return response.result();
        } else {
            throw std::runtime_error(std::to_string(status.error_code()) + ": " + status.error_message());
        }
    }

    std::unique_ptr<Calculator::Stub> stub;
};

class CalculatorServiceImpl final : public Calculator::Service {
    Status Add(ServerContext *context, const AddRequest *request, AddResponse *response) {
        response->set_sum(request->addend_1() + request->addend_2());
        return Status::OK;
    }

    Status Sub(ServerContext *context, const SubRequest *request, SubResponse *response) {
        response->set_difference(request->minuend() - request->subtrahend());
        return Status::OK;
    }

    Status Mul(ServerContext *context, const MulRequest *request, MulResponse *response) {
        response->set_product(request->multiplicand() * request->multiplier());
        return Status::OK;
    }

    Status Div(ServerContext *context, const DivRequest *request, DivResponse *response) {
        response->set_quotient(request->dividend() / request->divisor());
        return Status::OK;
    }

    Status Mod(ServerContext *context, const ModRequest *request, ModResponse *response) {
        response->set_result(request->dividend() % request->divisor());
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address{"localhost:2510"};
    CalculatorServiceImpl service;

    // Build server
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server{builder.BuildAndStart()};

    // Run server
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

TEST(RPC, Calculator) {
    int pid = fork();
    if (pid > 0) {
        sleep(1);
        CalculatorClient client("localhost", "2510");
        int32_t var_1 = 25, var_2 = 10;
        EXPECT_EQ(client.Add(var_1, var_2), 35);
        EXPECT_EQ(client.Sub(var_1, var_2), 15);
        EXPECT_EQ(client.Mul(var_1, var_2), 250);
        EXPECT_EQ(client.Div(var_1, var_2), 2);
        EXPECT_EQ(client.Mod(var_1, var_2), 5);
    } else if (pid == 0) {
        std::shared_ptr<CalculatorServiceImpl> service = std::make_shared<CalculatorServiceImpl>();
        RpcServer server("localhost", "2510", service);
        server.run();
    }
}