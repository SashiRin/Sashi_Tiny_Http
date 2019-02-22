#include <memory>

//
// Created by SashiRin on 2019-02-18.
//

#ifndef SASHI_TINY_HTTP_CONNECTION_HPP
#define SASHI_TINY_HTTP_CONNECTION_HPP

#include <array>
#include <memory>
#include <boost/asio.hpp>
#include "response.hpp"
#include "request.hpp"

namespace sashi_tiny_http {

    class ConnectionManager;

    class Connection : public std::enable_shared_from_this<Connection> {
    public:
        Connection(const Connection &) = delete;

        Connection &operator=(const Connection &) = delete;

        /// Construct a connection with the given socket.
        explicit Connection(boost::asio::ip::tcp::socket socket, ConnectionManager &manager, RequestHandler &handler);

        /// Start the first asynchronous operation for the connection.
        void Start();

        /// Stop all asynchronous operations associated with the connection.
        void Stop();

        void SetTimeout(long long seconds);

        void CancelTimeout();

        std::shared_ptr<boost::asio::ip::tcp::endpoint> remote_endpoint_;

        boost::asio::ip::tcp::socket socket_;

        std::unique_ptr<boost::asio::steady_timer> timer;

    private:

        void DoRead();

        void DoWrite();

        ConnectionManager &connection_manager_;

        RequestHandler &request_handler_;

        RequestParser request_parser_;

        Request request_;

        Response response_;

        std::array<char, 8192> buffer_;
    };

    typedef std::shared_ptr<Connection> ConnectionPtr;
}

#endif //SASHI_TINY_HTTP_CONNECTION_HPP
