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

    private:

        void DoRead();

        void DoWrite();

        boost::asio::ip::tcp::socket socket_;

        ConnectionManager &connection_manager_;

        RequestHandler &request_handler_;

        RequestParser request_parser_;

        HttpRequest request_;

        Response response_;

        std::array<char, 8192> buffer_;
    };

    typedef std::shared_ptr<Connection> ConnectionPtr;
}

#endif //SASHI_TINY_HTTP_CONNECTION_HPP
