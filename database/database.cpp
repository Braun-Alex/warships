#include "database.h"

Database::Database(const std::string& path) {
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, path, &db);
    if (!status.ok()) {
        throw std::runtime_error("Error while opening database " + path);
    }
}

bool Database::AtomicWrite(leveldb::WriteBatch& batch) {
    const leveldb::Status status = db->Write(leveldb::WriteOptions(), &batch);
    return status.ok();
}

bool Database::Get(const std::string& key, nlohmann::json& value) {
    std::string db_value;
    if (const leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &db_value); status.ok()) {
        value = nlohmann::json::parse(db_value);
        return true;
    }
    return false;
}

Database::~Database() {
    delete db;
}