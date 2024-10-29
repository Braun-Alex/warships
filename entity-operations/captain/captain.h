#pragma once

#include "database.h"
#include "entities.h"

#define CAPTAIN_PRIMARY_KEY_PREFIX "captain: "

bool putCaptain(Database& db, const Captain& captain);

std::optional<Captain> getCaptain(Database& db, const std::string& officer_certificate);

bool deleteCaptain(Database& db, const std::string& officer_certificate);