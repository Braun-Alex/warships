#include <gtest/gtest.h>
#include <filesystem>

#include "captain.h"
#include "warship.h"
#include "warship-compartment.h"

TEST(CRUDTest, UpdateTest) {
    const std::string DB_PATH = "test_db";
    Database db(DB_PATH);

    WarshipCompartment compartment{"Control Room", "Blue Sea", "Control"};
    EXPECT_TRUE(putWarshipCompartment(db, compartment));

    std::pair<std::string, std::string> compartmentKey = std::make_pair(compartment.warship_name, compartment.name);
    auto retrievedCompartment = getWarshipCompartment(db, compartmentKey);
    EXPECT_TRUE(retrievedCompartment != std::nullopt);

    retrievedCompartment->type = "New Control";
    EXPECT_TRUE(putWarshipCompartment(db, *retrievedCompartment));

    auto updatedCompartment = getWarshipCompartment(db, compartmentKey);
    EXPECT_TRUE(updatedCompartment != std::nullopt);
    EXPECT_EQ(updatedCompartment->type, "New Control");

    Warship warship{"Blue Sea", "New Port", CRUISER, {"Control Room"}};
    EXPECT_TRUE(putWarship(db, warship));

    auto retrievedWarship = getWarship(db, warship.name);
    EXPECT_TRUE(retrievedWarship != std::nullopt);
    EXPECT_EQ(retrievedWarship->home_port, "New Port");
    EXPECT_EQ(retrievedWarship->type, CRUISER);

    Captain captain{"12dc12da7ffa7a5f2475f69ea9a1d4d3ca90cdf315880eaddf031c0e97bf0e31", "Jane Doe", "Blue Sea"};
    EXPECT_TRUE(putCaptain(db, captain));

    auto retrievedCaptain = getCaptain(db, captain.officer_certificate);
    EXPECT_TRUE(retrievedCaptain != std::nullopt);
    EXPECT_EQ(retrievedCaptain->full_name, "Jane Doe");

    retrievedCaptain->full_name = "Jane Smith";
    EXPECT_TRUE(putCaptain(db, *retrievedCaptain));

    auto updatedCaptain = getCaptain(db, captain.officer_certificate);
    EXPECT_TRUE(updatedCaptain != std::nullopt);
    EXPECT_EQ(updatedCaptain->full_name, "Jane Smith");

    Captain newCaptain = captain;
    newCaptain.warship_name = "NonExistentShip";
    EXPECT_FALSE(putCaptain(db, newCaptain));

    Warship warshipNoCompartments{"Ghost Ship", "Hidden Port", DESTROYER, {}};
    EXPECT_FALSE(putWarship(db, warshipNoCompartments));

    Warship warshipWithInvalidCompartment{"Red Sun", "Mystic Port", BATTLESHIP, {"Phantom Compartment"}};
    EXPECT_FALSE(putWarship(db, warshipWithInvalidCompartment));

    warship.home_port = "Updated Port";
    warship.type = AIRCRAFT_CARRIER;
    EXPECT_TRUE(putWarship(db, warship));

    auto finalRetrievedWarship = getWarship(db, warship.name);
    EXPECT_TRUE(finalRetrievedWarship != std::nullopt);
    EXPECT_EQ(finalRetrievedWarship->home_port, "Updated Port");
    EXPECT_EQ(finalRetrievedWarship->type, AIRCRAFT_CARRIER);

    EXPECT_TRUE(std::filesystem::remove_all(DB_PATH) > 0);
}