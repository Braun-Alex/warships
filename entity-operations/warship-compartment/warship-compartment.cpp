#include "warship-compartment.h"
#include "warship.h"

bool putWarshipCompartment(Database& db, const WarshipCompartment& warshipCompartment) {
    const std::string jsonWarshipCompartment = nlohmann::json(warshipCompartment).dump();
    leveldb::WriteBatch batch;

    batch.Put(COMPARTMENT_PRIMARY_KEY_PREFIX + warshipCompartment.warship_name + warshipCompartment.name, jsonWarshipCompartment);
    return db.AtomicWrite(batch);;
}

std::optional<WarshipCompartment> getWarshipCompartment(Database& db, std::pair<std::string, std::string>& compartment) {
    nlohmann::json jsonWarshipCompartment;

    if (const auto& [warshipName, compartmentName] = compartment;
        !db.Get(COMPARTMENT_PRIMARY_KEY_PREFIX + warshipName + compartmentName, jsonWarshipCompartment)) {
        return std::nullopt;
    }

    WarshipCompartment warshipCompartment;
    jsonWarshipCompartment.get_to(warshipCompartment);
    return warshipCompartment;
}

bool deleteWarshipCompartment(Database& db, const std::pair<std::string, std::string>& compartment) {
    const auto& [warshipName, compartmentName] = compartment;
    std::optional<Warship> warship = getWarship(db, warshipName);

    if (warship == std::nullopt) {
        return false;
    }

    warship.value().compartments.erase(compartmentName);

    leveldb::WriteBatch batch;
    batch.Delete(leveldb::Slice(COMPARTMENT_PRIMARY_KEY_PREFIX + warshipName + compartmentName));
    return db.AtomicWrite(batch);
}