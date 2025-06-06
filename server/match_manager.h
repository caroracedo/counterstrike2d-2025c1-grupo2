#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "acceptor.h"
#include "match.h"

class MatchManager: public Thread {
private:
    Config config;
    Acceptor acceptor;

public:
    explicit MatchManager(const char* yaml_path): config(yaml_path), acceptor(config) {}

    void run() override { acceptor.start(); }

    void stop() override {
        Thread::stop();
        acceptor.stop();
        acceptor.join();
    }
};

#endif  // MATCH_MANAGER_H
