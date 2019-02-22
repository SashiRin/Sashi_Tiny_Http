//
// Created by SashiRin on 2019-02-08.
//

#include "server.hpp"
#include <boost/thread.hpp>

namespace sashi_tiny_http {

    Server::Server(const std::string &address, const std::string &port, const std::string &doc_root)
    : io_context_(1),
    signals_(io_context_),
    acceptor_(io_context_),
    connection_manager_(),
    request_handler_(io_context_, doc_root),
    expiremap_timer_thread_(boost::bind(&boost::asio::io_context::run, &io_context_)) {

        signals_.add(SIGINT);
        signals_.add(SIGTERM);
        #if defined(SIGQUIT)
        signals_.add(SIGQUIT);
        #endif

        DoAwaitStop();

        boost::asio::ip::tcp::resolver resolver(io_context_);
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        DoAccept();
    }

    void Server::Run() {
        io_context_.run();
    }

    void Server::DoAccept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
                if (!acceptor_.is_open()) {
                    return;
                }

                if (!ec) {
                    connection_manager_.Start(std::make_shared<Connection>(std::move(socket), connection_manager_, request_handler_));
                }

                DoAccept();
            });
    }

    void Server::DoAwaitStop() {
        signals_.async_wait(
            [this](boost::system::error_code, int) {
                acceptor_.close();
                connection_manager_.StopAll();
                request_handler_.StopFileCache();
                pthread_kill(expiremap_timer_thread_.native_handle(), 9);
            });
    }
}