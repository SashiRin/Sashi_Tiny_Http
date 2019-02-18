//
// Created by SashiRin on 2019-02-18.
//

#ifndef SASHI_TINY_HTTP_CONNECTION_MANAGER_HPP
#define SASHI_TINY_HTTP_CONNECTION_MANAGER_HPP

#include <set>
#include "connection.hpp"

namespace sashi_tiny_http {

    class ConnectionManager {
    public:
        ConnectionManager(const ConnectionManager &) = delete;

        ConnectionManager& operator=(const ConnectionManager &) = delete;

        ConnectionManager();

        void Start(ConnectionPtr c);

        void Stop(ConnectionPtr c);

        void StopAll();

    private:

        std::set<ConnectionPtr> connections_;
    };
}

#endif //SASHI_TINY_HTTP_CONNECTION_MANAGER_HPP
