#pragma once
#include "npc.h"
#include <fstream>

class TextObserver : public IFightObserver {
private:
    TextObserver() {};

public:
    static std::shared_ptr<IFightObserver> get() {
        static TextObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker,
                  const std::shared_ptr<NPC> defender, bool win) override {
        if (win) {
            std::cout << std::endl << "Murder --------" << std::endl;
            attacker->print();
            defender->print();
        }
    }
};

class FileObserver : public IFightObserver {
private:
    std::ofstream log_file;

    FileObserver() {
        log_file.open("log.txt", std::ios::app);
        if (!log_file.is_open()) {
            std::cerr << "Failed to open log.txt for writing." << std::endl;
        }
    }

public:
    static std::shared_ptr<IFightObserver> get() {
        static FileObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker,
                  const std::shared_ptr<NPC> defender, bool win) override {
        if (win && log_file.is_open()) {
            log_file << "Murder --------" << std::endl;
            log_file << "Attacker: " << attacker->type << " at (" << attacker->x << ", " << attacker->y << ")" << std::endl;
            log_file << "Defender: " << defender->type << " at (" << defender->x << ", " << defender->y << ")" << std::endl;
        }
    }

    ~FileObserver() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }
};
