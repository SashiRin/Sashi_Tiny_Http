//
// Created by SashiRin on 2019-02-18.
//

#include "connection.hpp"
#include "connection_manager.hpp"
#include "config.hpp"

namespace sashi_tiny_http {
    Connection::Connection(boost::asio::ip::tcp::socket socket, sashi_tiny_http::ConnectionManager &manager,
                           sashi_tiny_http::RequestHandler &handler)
                           : socket_(std::move(socket)),
                           connection_manager_(manager),
                           request_handler_(handler) {}

    void Connection::Start() {
        DoRead();
    }

    void Connection::Stop() {
        socket_.close();
    }

    void Connection::DoRead() {
        this->SetTimeout(config::timeout_request);
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(buffer_),
            [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
            this->CancelTimeout();
            if (!ec) {
                RequestParser::ResultType result;
                std::tie(result, std::ignore) = request_parser_.Parse(request_, buffer_.data(), buffer_.data() + bytes_transferred);

                if (result == RequestParser::ResultType::kGood) {
                    request_handler_.HandleRequest(request_, response_);
                    request_parser_.reset();
                    request_.reset();
                    DoWrite();
                } else if (result == RequestParser::ResultType::kBad) {
                    response_ = Response::StockReply(StatusCode::client_error_bad_request);
                    request_parser_.reset();
                    request_.reset();
                    DoWrite();
                } else {
                    DoRead();
                }
            } else if (ec != boost::asio::error::operation_aborted) {
                connection_manager_.Stop(shared_from_this());
            }
        });
    }

    void Connection::DoWrite() {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, response_.ConvertToBuffers(),
            [this, self](boost::system::error_code ec, std::size_t) {
                DoRead();
//            if (!ec) {
//                // Initiate graceful connection closure.
//                boost::system::error_code ignore_ec;
//
//                // Not keep-alive
//                socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignore_ec);
//            }
//
//            if (ec != boost::asio::error::operation_aborted) {
//                connection_manager_.Stop(shared_from_this());
//            }
        });
    }

    void Connection::SetTimeout(long long seconds) {
        if (seconds == 0) {
            timer = nullptr;
            return;
        }

        timer = std::make_unique<boost::asio::steady_timer>(socket_.get_io_service());
        timer->expires_from_now(std::chrono::seconds(seconds));
        auto self = shared_from_this();
        timer->async_wait([self](const boost::system::error_code &ec) {
            if (!ec) {
                self->Stop();
            }
        });
    }

    void Connection::CancelTimeout() {
        if (timer) {
            boost::system::error_code ec;
            timer->cancel(ec);
        }
    }
}