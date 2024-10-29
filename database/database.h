#pragma once

#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <nlohmann/json.hpp>

class Database {
public:
    explicit Database(const std::string& path);

    bool AtomicWrite(leveldb::WriteBatch& batch);

    bool Get(const std::string& key, nlohmann::json& value);

    ~Database();

private:
    leveldb::DB* db;
};