#include "broker.hpp"

schedule::schedule(int port){
    this->sock_id_counter = 0;
    sockpp::socket_initializer sockInit;
    acc = new sockpp::tcp_acceptor(port);
};

void schedule::sock_connect(void){
    while(true){
        sockpp::inet_address peer;
        sockpp::tcp_socket sock = acc->accept(&peer);
        if(sock){
            sock_client_m[sock_id_counter] = std::move(sock);
        }
        sock_id_counter++;
    }
}

void schedule::start(void){
    ptr_sock_connect = new std::thread(&schedule::sock_connect, this);
    ptr_sock_process = new std::thread(&schedule::sock_process, this);
    ptr_publish_process = new std::thread(&schedule::publish_process, this);
}

void schedule::publish_process(void){
    while(true){
        while(!publish_message_q.empty()){
            message pub_message = publish_message_q.front();
            int pub_topic_id = pub_message.client_id;
            std::string body = pub_message.body;
            // std::cout << publish_list_m.find(pub_topic_id) << std::endl;
            std::map<int, std::vector<int>>::iterator it = publish_list_m.find(pub_topic_id);
            if (it != publish_list_m.end()){
                for(long unsigned int idx_v = 0; idx_v < it->second.size(); idx_v++){
                    int client_id = it->second[idx_v];
                    // check connected
                    if(sock_client_m[client_id]){
                        std::string pub_data("{ \"header\": 1, \"topic_id\": ");
                        pub_data.append(std::to_string(pub_topic_id));
                        pub_data.append(", \"body\":");
                        pub_data.append("\"");
                        pub_data.append(body);
                        pub_data.append("\"");
                        pub_data.append("}");
                        sock_client_m[client_id].write(pub_data);
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    }
                }
            }
            publish_message_q.pop();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void schedule::sock_process(void){
    while(true){
        std::map<int, sockpp::tcp_socket>::iterator sock_client_m_begin = sock_client_m.begin();
        std::map<int, sockpp::tcp_socket>::iterator sock_client_m_end = sock_client_m.end();
        while(sock_client_m_begin != sock_client_m_end){
            ssize_t n;
            // clear buffer
            char buf[256] = {0};
            if ((n = sock_client_m_begin->second.read(buf, sizeof(buf))) > 0){
                buf[n+1] = '\0';
                nlohmann::basic_json<> recv_json = nlohmann::json::parse(std::string(buf));
                switch((int)recv_json["header"]){
                    // register subscribe
                    case 1:
                        this->subscribe_register((int)sock_client_m_begin->first, recv_json["topic_id"]);
                    break;
                    case 2:
                        this->publish_body_put(recv_json["topic_id"], recv_json["body"]);
                    break;
                }
            }
            sock_client_m_begin++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void schedule::publish_body_put(int topic_id, std::string body){
    publish_message_q.push(message(topic_id, body));
}

void schedule::subscribe_register(int client_id, int topic_id){
    publish_list_m[topic_id].push_back(client_id);
}