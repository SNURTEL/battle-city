//
// Created by Rafal on 31.05.2022.
//

#ifndef PROI_PROJEKT_MENU_H
#define PROI_PROJEKT_MENU_H

/**
 * Represents Menu 
 * Contains data related to it
 */
class Menu {
    private:
        unsigned int pos_ = 1;
        unsigned int max_pos_ = 1;
    public:
    
        /**
         * Inits class Menu
         * @param max_pos
         */
        explicit Menu(unsigned int max_pos);

        /**
         * Returns menu's current position
         * @return Menu's current position
         */
        unsigned int get_pos();

        /**
         * Sets position in the menu
         * @param position
         */
        void set_pos(unsigned int position);

        /**
         * Adds one to current position
         */
        void add_pos();

        /**
         * Subtracts one to current position
         */
        void sub_pos();

        /**
         * Creates event that menu position has changed
         */
        void notify();
};

#endif //PROI_PROJEKT_MENU_H