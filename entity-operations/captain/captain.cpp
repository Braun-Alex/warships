#include "captain.h"
#include "warship.h"

bool putCaptain(Database& db, const Captain& captain) {
    if (getWarship(db, captain.warship_name) == std::nullopt) {
        return false;
    }

    const std::string jsonCaptain = nlohmann::json(captain).dump();
    leveldb::WriteBatch batch;

    batch.Put(CAPTAIN_PRIMARY_KEY_PREFIX + captain.officer_certificate, leveldb::Slice(jsonCaptain));
    return db.AtomicWrite(batch);
}

std::optional<Captain> getCaptain(Database& db, const std::string& officer_certificate) {
    nlohmann::json jsonCaptain;

    if (!db.Get(CAPTAIN_PRIMARY_KEY_PREFIX + officer_certificate, jsonCaptain)) {
        return std::nullopt;
    }

    Captain captain;
    jsonCaptain.get_to(captain);
    return captain;
}

bool deleteCaptain(Database& db, const std::string& officer_certificate) {
    std::optional<Captain> captain = getCaptain(db, officer_certificate);

    if (captain == std::nullopt) {
        return false;
    }

    leveldb::WriteBatch batch;
    batch.Delete(leveldb::Slice(WARSHIP_PRIMARY_KEY_PREFIX + captain.value().warship_name));
    batch.Delete(leveldb::Slice(CAPTAIN_PRIMARY_KEY_PREFIX + officer_certificate));

    return db.AtomicWrite(batch);
}