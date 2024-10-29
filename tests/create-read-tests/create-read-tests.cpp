#include <gtest/gtest.h>
#include <filesystem>

#include "captain.h"
#include "warship.h"
#include "warship-compartment.h"

TEST(CRUDTest, CreateReadTests) {
    const std::string DB_PATH = "test_db";

    Database db(DB_PATH);

    WarshipCompartment engineCompartment{"Engine Room", "Dark Sun", "Engine"},
        bridgeCompartment{"BridgeArea", "Dark Sun", "Bridge"};
    EXPECT_TRUE(putWarshipCompartment(db, engineCompartment));
    EXPECT_TRUE(putWarshipCompartment(db, bridgeCompartment));

    std::pair<std::string, std::string> engineCompartmentKey = std::make_pair(engineCompartment.warship_name, engineCompartment.name),
        bridgeCompartmentKey = std::make_pair(bridgeCompartment.warship_name, bridgeCompartment.name);

    auto retrievedEngineCompartment = getWarshipCompartment(db, engineCompartmentKey);
    EXPECT_TRUE(retrievedEngineCompartment != std::nullopt);

    EXPECT_EQ(retrievedEngineCompartment->name, engineCompartment.name);
    EXPECT_EQ(retrievedEngineCompartment->warship_name, engineCompartment.warship_name);

    auto retrievedBridgeCompartment = getWarshipCompartment(db, bridgeCompartmentKey);
    EXPECT_TRUE(retrievedBridgeCompartment != std::nullopt);

    EXPECT_EQ(retrievedBridgeCompartment->name, bridgeCompartment.name);
    EXPECT_EQ(retrievedBridgeCompartment->warship_name, bridgeCompartment.warship_name);



    Warship lightSunWarship{"Light Sun", "First Secret Port", BATTLESHIP, {}};
    EXPECT_FALSE(putWarship(db, lightSunWarship));

    Warship redSunWarship{"Green Sun", "Second Secret Port", BATTLESHIP, {"Bridge"}};
    EXPECT_FALSE(putWarship(db, redSunWarship));

    Warship darkSunWarship{"Dark Sun", "Thirst Secret Port", BATTLESHIP, {"Engine Room", "BridgeArea"}};
    EXPECT_TRUE(putWarship(db, darkSunWarship));

    auto retrievedWarship = getWarship(db, darkSunWarship.name);
    EXPECT_TRUE(retrievedWarship != std::nullopt);

    EXPECT_EQ(retrievedWarship->name, darkSunWarship.name);
    EXPECT_EQ(retrievedWarship->home_port, darkSunWarship.home_port);
    EXPECT_EQ(retrievedWarship->type, darkSunWarship.type);



    Captain lightSunCaptain{"12dc12da7ffa7a5f2475f69ea9a1d4d3ca90cdf315880eaddf031c0e97bf0e79", "John Doe", "Light Sun"};
    EXPECT_FALSE(putCaptain(db, lightSunCaptain));

    Captain darkSunCaptain{"12dc12da7ffa7a5f2475f69ea9a1d4d3ca90cdf315880eaddf031c0e97bf0e79", "John Doe", "Dark Sun"};
    EXPECT_TRUE(putCaptain(db, darkSunCaptain));

    auto retrievedCaptain = getCaptain(db, darkSunCaptain.officer_certificate);
    EXPECT_TRUE(retrievedCaptain != std::nullopt);

    EXPECT_EQ(retrievedCaptain->full_name, darkSunCaptain.full_name);
    EXPECT_EQ(retrievedCaptain->warship_name, darkSunCaptain.warship_name);

    EXPECT_TRUE(std::filesystem::remove_all(DB_PATH) > 0);
}