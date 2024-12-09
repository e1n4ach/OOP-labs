#include "factory.h"
#include "vykhuhol.h"

std::shared_ptr<NPC> factory(std::istream &is) {
    std::shared_ptr<NPC> result;
    int type{0};
    if (is >> type) {
        switch (type) {
            case BearType:
                result = std::make_shared<Bear>(is);
                break;
            case VupType:
                result = std::make_shared<Vup>(is);
                break;
            case VykhuholType:
                result = std::make_shared<Vykhuhol>(is);
                break;
            default:
                std::cerr << "unexpected NPC type:" << type << std::endl;
        }
    } else {
        std::cerr << "Failed to read NPC type." << std::endl;
    }

    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }

    return result;
}

std::shared_ptr<NPC> factory(NpcType type, int x, int y) {
    std::shared_ptr<NPC> result;
    switch (type) {
        case BearType:
            result = std::make_shared<Bear>(x, y);
            break;
        case VupType:
            result = std::make_shared<Vup>(x, y);
            break;
        case VykhuholType:
            result = std::make_shared<Vykhuhol>(x, y);
            break;
        default:
            break;
    }
    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }

    return result;
}

void save(const set_t &array, const std::string &filename) {
    std::ofstream fs(filename);
    if (!fs.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing." << std::endl;
        return;
    }
    fs << array.size() << std::endl;
    for (auto &n : array)
        n->save(fs);
    fs.flush();
    fs.close();
}

set_t load(const std::string &filename) {
    set_t result;
    std::ifstream is(filename);
    if (is.good() && is.is_open()) {
        size_t count;
        is >> count;
        for (size_t i = 0; i < count; ++i)
            result.insert(factory(is));
        is.close();
    } else {
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
    }
    return result;
}

std::ostream &operator<<(std::ostream &os, const set_t &array) {
    for (auto &n : array)
        n->print();
    return os;
}
