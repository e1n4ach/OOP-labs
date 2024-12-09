#pragma once
#include "npc.h"
#include "common.h"  
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <optional>

struct FightEvent {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
};

class FightManager {
public:
    FightManager() = default;

    void add_event(const FightEvent& event) {
        std::lock_guard<std::mutex> lock(mtx);
        events.push(event);
        cv.notify_one();
    }

    std::optional<FightEvent> get_event() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return !events.empty() || stop; });
        if (events.empty())
            return std::nullopt;
        FightEvent event = events.front();
        events.pop();
        return event;
    }

    void stop_processing() {
        std::lock_guard<std::mutex> lock(mtx);
        stop = true;
        cv.notify_all();
    }

private:
    std::queue<FightEvent> events;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;
};
