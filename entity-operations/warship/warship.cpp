#include "warship.h"
#include "warship-compartment.h"

bool putWarship(Database& db, const Warship& warship) {
    if (warship.compartments.empty()) {
        return false;
    }

    for (const auto& compartment: warship.compartments) {
        if (auto warshipCompartment = std::make_pair(warship.name, compartment);
            getWarshipCompartment(db, warshipCompartment) == std::nullopt) {
            return false;
        }
    }

    const std::string jsonWarship = nlohmann::json(warship).dump();
    leveldb::WriteBatch batch;

    batch.Put(WARSHIP_PRIMARY_KEY_PREFIX + warship.name, leveldb::Slice(jsonWarship));
    return db.AtomicWrite(batch);
}

std::optional<Warship> getWarship(Database& db, const std::string& name) {
    nlohmann::json jsonWarship;

    if (!db.Get(WARSHIP_PRIMARY_KEY_PREFIX + name, jsonWarship)) {
        return std::nullopt;
    }

    Warship warship;
    jsonWarship.get_to(warship);
    return warship;
}

bool deleteWarship(Database& db, const std::string& name) {
    const std::optional<Warship> warship = getWarship(db, name);

    if (warship == std::nullopt) {
        return false;
    }

    leveldb::WriteBatch batch;

    for (const auto& compartmentName: warship.value().compartments) {
        batch.Delete(leveldb::Slice(COMPARTMENT_PRIMARY_KEY_PREFIX + name + compartmentName));
    }

    batch.Delete(leveldb::Slice(WARSHIP_PRIMARY_KEY_PREFIX + name));
    return db.AtomicWrite(batch);
}