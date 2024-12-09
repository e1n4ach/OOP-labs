#pragma once

#include "npc.h"
#include "common.h"  
#include <mutex>
#include <iostream>
#include <fstream>

class TextObserver : public IFightObserver {
private:
    TextObserver(){};

public:
    static std::shared_ptr<IFightObserver> get() {
        static TextObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, int attack_roll,
                  const std::shared_ptr<NPC> defender, int defense_roll,
                  bool win) override {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "\nFight Details --------\n";
        std::cout << "Attacker:\n";
        attacker->print();
        std::cout << "Defender:\n";
        defender->print();
        std::cout << "Attack roll: " << attack_roll
                  << ", Defense roll: " << defense_roll << '\n';
        std::cout << (win ? "Defender died!" : "Defender survived.") << "\n\n";
    }
};

class FileObserver : public IFightObserver {
private:
    std::ofstream log_file;
    std::mutex file_mutex;

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

    void on_fight(const std::shared_ptr<NPC> attacker, int attack_roll,
                  const std::shared_ptr<NPC> defender, int defense_roll,
                  bool win) override {
        if (win && log_file.is_open()) {
            std::lock_guard<std::mutex> lock(file_mutex);
            log_file << "Fight Details --------\n";
            log_file << "Attacker: " << attacker->get_type() << " at (" 
                     << attacker->position().first << ", " << attacker->position().second << ")\n";
            log_file << "Defender: " << defender->get_type() << " at (" 
                     << defender->position().first << ", " << defender->position().second << ")\n";
            log_file << "Attack roll: " << attack_roll << ", Defense roll: " << defense_roll << '\n';
            log_file << (win ? "Defender died!\n\n" : "Defender survived.\n\n");
        }
    }

    ~FileObserver() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }
};
