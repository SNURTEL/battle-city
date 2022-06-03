//
// Created by tomek on 23.05.2022.
//

#ifndef PROI_PROJEKT_GRID_H
#define PROI_PROJEKT_GRID_H

#include <exception>

class Event;

template<class E>
class EventQueue;

class GridBuilder;


/**
 * Enum representing different tile types
 * Each tile's attributes (collidability, destructibility, etc.) can be accessed through a TileManager object
 */
enum TileType : unsigned int {
    NullTile = 0,
    Bricks,
    Steel,
    Water,
    Trees
};

/**
 * Exception thrown when trying to access a field that's out of map (one or two coords are negative or above 51)
 */
class OutOfGridException : public std::exception {
    [[nodiscard]] const char *what() const noexcept override;
};

/**
 * \brief Represents a grid of tiles
 *
 * Each tile is represented by a TileType enum value, and it's attributes can be accessed using the TileManager object
 * This is done in order to avoid creating up to 52^2 objects with nearly identical attributes
 * 'Empty' fields on the map should be filled with NullTiles
 */  // is there even a need to use NullTiles?
class Grid {
public:
    Grid();

    /**
     * Returns a tile located at given coords
     * @param x X coord
     * @param y Y coord
     * @return A TileType value
     */
    TileType getTileAtPosition(unsigned int x, unsigned int y);

    /**
     * Places a tile of a given type at given coords. Calls deleteTile when trying to place a NullTile
     *
     * Queues Event::TilePlaced, Event::TileChanged, or Event:TileDeleted
     * @param x X coord
     * @param y Y coord
     * @return A TileType value
     */
    void setTile(unsigned int x, unsigned int y, TileType newTile);

    /**
     * Replaces a tile at given coords with a NullTile. If it was already a NullTile, does nothing
     *
     * Possibly queues Event::TileDeleted
     * @param x X coord
     * @param y Y coord
     */
    void deleteTile(unsigned int x, unsigned int y);

    /**
     * Returns board's X size
     * @return Board's X axis size
     */
    [[nodiscard]] unsigned int getSizeX() const;

    /**
     * Returns board's Y size
     * @return Board's Y axis size
     */
    [[nodiscard]] unsigned int getSizeY() const;

    friend class GridBuilder;
protected:
    unsigned int size_x = 52;
    unsigned int size_y = 52;
    TileType grid[52][52] = {TileType::NullTile};

    EventQueue<Event> *eventQueue_;
};


#endif //PROI_PROJEKT_GRID_H
