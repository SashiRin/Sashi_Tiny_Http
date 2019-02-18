//
// Created by SashiRin on 2019-02-18.
//

#include "connection_manager.hpp"

namespace sashi_tiny_http {

    ConnectionManager::ConnectionManager() = default;

    void ConnectionManager::Start(sashi_tiny_http::ConnectionPtr c) {
        connections_.insert(c);
        c->Start();
    }

    void ConnectionManager::Stop(sashi_tiny_http::ConnectionPtr c) {
        connections_.erase(c);
        c->Stop();
    }

    void ConnectionManager::StopAll() {
        for (auto &c : connections_) {
            c->Stop();
        }
        connections_.clear();
    }
}