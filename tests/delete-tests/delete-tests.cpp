#include <gtest/gtest.h>
#include <filesystem>

#include "captain.h"
#include "warship.h"
#include "warship-compartment.h"

TEST(CRUDTest, DeleteTest) {
    const std::string DB_PATH = "test_db";
    Database db(DB_PATH);

    WarshipCompartment compartment{"Armory", "Red Thunder", "Armory"};
    EXPECT_TRUE(putWarshipCompartment(db, compartment));

    Warship warship{"Red Thunder", "Safe Harbor", DESTROYER, {"Armory"}};
    EXPECT_TRUE(putWarship(db, warship));

    Captain captain{"12dc12da7ffa7a5f2475f69ea9a1d4d3ca90cdf315880eaddf031c0e97bf0e93", "Richard Roe", "Red Thunder"};
    EXPECT_TRUE(putCaptain(db, captain));

    std::pair<std::string, std::string> compartmentKey = std::make_pair(compartment.warship_name, compartment.name);
    EXPECT_TRUE(deleteWarshipCompartment(db, compartmentKey));

    auto deletedCompartment = getWarshipCompartment(db, compartmentKey);
    EXPECT_TRUE(deletedCompartment == std::nullopt);

    EXPECT_TRUE(deleteCaptain(db, captain.officer_certificate));

    auto deletedCaptain = getCaptain(db, captain.officer_certificate);
    EXPECT_TRUE(deletedCaptain == std::nullopt);

    auto deletedWarship = getWarship(db, warship.name);
    EXPECT_TRUE(deletedWarship == std::nullopt);

    EXPECT_FALSE(deleteCaptain(db, "non_existent_certificate"));

    EXPECT_FALSE(deleteWarship(db, "NonExistentShip"));

    Warship anotherWarship{"Blue Ocean", "Harbor", CRUISER, {"Engine Room"}};
    WarshipCompartment anotherCompartment{"Engine Room", "Blue Ocean", "Engineering"};
    EXPECT_TRUE(putWarshipCompartment(db, anotherCompartment));
    EXPECT_TRUE(putWarship(db, anotherWarship));

    EXPECT_TRUE(deleteWarship(db, anotherWarship.name));
    auto deletedAnotherWarship = getWarship(db, anotherWarship.name);
    EXPECT_TRUE(deletedAnotherWarship == std::nullopt);

    std::pair<std::string, std::string> anotherCompartmentKey = std::make_pair(anotherWarship.name, "Engine Room");

    auto deletedAnotherCompartment = getWarshipCompartment(db, anotherCompartmentKey);
    EXPECT_TRUE(deletedAnotherCompartment == std::nullopt);

    EXPECT_FALSE(deleteWarshipCompartment(db, std::make_pair("Blue Ocean", "NonExistentCompartment")));

    EXPECT_TRUE(std::filesystem::remove_all(DB_PATH) > 0);
}