#pragma once

#include <string>
#include <unordered_set>
#include <nlohmann/json.hpp>

enum WarshipType {
    AIRCRAFT_CARRIER, BATTLESHIP, DESTROYER, CRUISER
};

struct Captain {
    std::string officer_certificate;
    std::string full_name;
    std::string warship_name;
};

struct Warship {
    std::string name;
    std::string home_port;
    WarshipType type;
    std::unordered_set<std::string> compartments;
};

struct WarshipCompartment {
    std::string name;
    std::string warship_name;
    std::string type;
};

NLOHMANN_JSON_SERIALIZE_ENUM(WarshipType, {
    {AIRCRAFT_CARRIER, "aircraft_carrier"},
    {BATTLESHIP, "battleship"},
    {DESTROYER, "destroyer"},
    {CRUISER, "cruiser"}
})

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Captain, officer_certificate, full_name, warship_name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Warship, name, home_port, type, compartments)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WarshipCompartment, name, warship_name, type)
