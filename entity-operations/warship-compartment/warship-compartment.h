#pragma once

#include "database.h"
#include "entities.h"

#define COMPARTMENT_PRIMARY_KEY_PREFIX "warship-compartment: "

bool putWarshipCompartment(Database& db, const WarshipCompartment& warshipCompartment);

std::optional<WarshipCompartment> getWarshipCompartment(Database& db, std::pair<std::string, std::string>& compartment);

bool deleteWarshipCompartment(Database& db, const std::pair<std::string, std::string>& compartment);