//
// Created by SashiRin on 2019-02-08.
//

#ifndef SASHI_TINY_HTTP_SERVER_HPP
#define SASHI_TINY_HTTP_SERVER_HPP

#include "request.hpp"
#include "connection_manager.hpp"
#include "connection.hpp"
#include "config.hpp"

namespace sashi_tiny_http {
    class Server {
    public:
        Server(const Server &) = delete;

        Server& operator=(const Server &) = delete;

        explicit Server(const std::string &address, const std::string &port, const std::string &doc_root);

        void Run();

    private:

        void DoAccept();

        void DoAwaitStop();

        boost::asio::io_context io_context_;

        boost::asio::signal_set signals_;

        boost::asio::ip::tcp::acceptor acceptor_;

        ConnectionManager connection_manager_;

        RequestHandler request_handler_;
    };
}

#endif //SASHI_TINY_HTTP_SERVER_HPP
