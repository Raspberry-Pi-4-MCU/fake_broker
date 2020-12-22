#pragma once
#include <queue>
#include <cstdint>
#include <thread>
#include "sockpp/tcp_acceptor.h"
#include "sockpp/version.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <map>
#include <chrono>
#define PROCESS_QUEUE_SIZE 1000

class message{
    public:
        int client_id;
        std::string body;
        message(int client_id, std::string body){
            this->client_id = client_id;
            this->body = body;
        }
};

class schedule{
    private:
        int sock_id_counter;
        // subscribe table (topic id, client id)
        std::map<int, std::vector<int>> publish_list_m;
        // publish body
        std::queue<message> publish_message_q;
        // std::vector<int> subscribe_list;
        std::map<int, sockpp::tcp_socket> sock_client_m;
        // socket
        sockpp::tcp_acceptor *acc;
        std::thread *ptr_sock_connect;
        std::thread *ptr_sock_process;
        std::thread *ptr_publish_process;
    public:
        schedule(int port = 1843);
        void start(void);
        void subscribe_register(int, int);
        void publish_body_put(int, std::string); // topic id, topic body
        void sock_process(void);
        void sock_connect(void);
        void publish_process(void);
        void test(void);
};