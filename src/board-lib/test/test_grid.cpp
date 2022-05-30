//
// Created by tomek on 23.05.2022.
//

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/Grid.h"

#include "../../core-lib/include/EventQueue.h"
#include "../../core-lib/include/Event.h"

namespace helper {

    class TestGrid : public Grid {
    };
}

SCENARIO("Checking grid's contents") {
    EventQueue<Event> *eventQueue = EventQueue<Event>::instance();

    GIVEN("An empty grid") {
        helper::TestGrid testGrid{};
        WHEN("Accessing the contents of the grid") {
            TileType foundTile = testGrid.getTileAtPosition(27, 42);
            THEN("A NullTile should be returned") {
                REQUIRE(foundTile == NullTile);
            }
        }
    }GIVEN("A grid with some tiles") {
        helper::TestGrid testGrid{};
        testGrid.setTile(10, 20, Bricks);
        testGrid.setTile(30, 45, Steel);

        eventQueue->clear();

        WHEN("Checking a field containing a tile") {
            TileType foundTile = testGrid.getTileAtPosition(10, 20);

            THEN("A correct tile should be returned") {
                REQUIRE(foundTile == Bricks);
            }
        }WHEN("Trying to access a field with a negative index") {
            THEN("An exception should be thrown") {
                REQUIRE_THROWS_AS(testGrid.getTileAtPosition(-5, 17), OutOfGridException);
            }
        }WHEN("Trying to access a field with an index larger than map's size") {
            THEN("An exception should be thrown") {
                REQUIRE_THROWS_AS(testGrid.getTileAtPosition(20, testGrid.getSizeY() + 5), OutOfGridException);
            }
        }
    }
}

SCENARIO("Setting grid's contents") {
    EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
    GIVEN("A grid with some tiles") {
        helper::TestGrid testGrid{};
        testGrid.setTile(10, 20, Bricks);
        testGrid.setTile(30, 45, Steel);

        eventQueue->clear();

        WHEN("Placing a new tile") {
            testGrid.setTile(20, 20, Trees);

            THEN("A tile should be placed in a correct location") {
                REQUIRE(testGrid.getTileAtPosition(20, 20) == Trees);

                AND_THEN("An event should be added to the queue") {
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::TilePlaced);
                    REQUIRE(event->info.tileInfo.tile_x == 20);
                    REQUIRE(event->info.tileInfo.tile_y == 20);
                    REQUIRE(event->info.tileInfo.grid == &testGrid);
                    REQUIRE(eventQueue->isEmpty());
                }
                eventQueue->clear();

                AND_WHEN("Overriding tile value (with anything other than NullTile)") {
                    testGrid.setTile(10, 20, Water);

                    THEN("Tile type should be updated") {
                        REQUIRE(testGrid.getTileAtPosition(10, 20) == Water);

                        AND_THEN("An event should be added to the queue") {
                            auto event = eventQueue->pop();
                            REQUIRE(event->type == Event::TileChanged);
                            REQUIRE(event->info.tileInfo.tile_x == 10);
                            REQUIRE(event->info.tileInfo.tile_y == 20);
                            REQUIRE(event->info.tileInfo.grid == &testGrid);
                            REQUIRE(eventQueue->isEmpty());
                        }
                    }
                }
            }
        }

        WHEN("Deleting a tile") {
            REQUIRE_FALSE(testGrid.getTileAtPosition(30, 45) == NullTile);
            testGrid.deleteTile(30, 45);

            THEN("NullTile should be placed in a given location") {
                REQUIRE(testGrid.getTileAtPosition(30, 45) == NullTile);

                AND_THEN("An event should be added to the queue") {
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::TileDeleted);
                    REQUIRE(event->info.tileInfo.tile_x == 30);
                    REQUIRE(event->info.tileInfo.tile_y == 45);
                    REQUIRE(event->info.tileInfo.grid == &testGrid);
                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }WHEN("Trying to delete a NullTile") {
            REQUIRE(eventQueue->isEmpty());
            testGrid.deleteTile(17, 17);

            THEN("Tile should remain a NullTile") {
                REQUIRE(testGrid.getTileAtPosition(17, 17) == NullTile);

                AND_THEN("No event should be added to the queue") {
                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }WHEN("Overriding a tile with NullTile") {
            testGrid.setTile(30, 45, NullTile);

            THEN("Everything should happen the same way as if the tile was to be deleted") {
                REQUIRE(testGrid.getTileAtPosition(30, 45) == NullTile);

                AND_THEN("An event should be added to the queue") {
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::TileDeleted);
                    REQUIRE(event->info.tileInfo.tile_x == 30);
                    REQUIRE(event->info.tileInfo.tile_y == 45);
                    REQUIRE(event->info.tileInfo.grid == &testGrid);
                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }
    }
}