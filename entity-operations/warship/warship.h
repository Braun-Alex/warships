#pragma once

#include "database.h"
#include "entities.h"

#define WARSHIP_PRIMARY_KEY_PREFIX "warship: "

bool putWarship(Database& db, const Warship& warship);

std::optional<Warship> getWarship(Database& db, const std::string& name);

bool deleteWarship(Database& db, const std::string& name);