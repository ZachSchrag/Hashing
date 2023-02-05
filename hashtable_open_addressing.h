/*
 *  Open addressing hashtable implemeneted using quadratic probing. Max load factor set to .5 by default
 *  Written by Zach Schrag
*/

#pragma once
#include <functional>
#include <vector>
#include <stdexcept>
#include <iostream> // for print_table only

using std::vector, std::cout, std::endl;

template <class Key, class Hash=std::hash<Key>>
class HashTable {

    private:
        struct Cell {
            // constants for cell status
            #define EMPTY_CELL 0
            #define ACTIVE_CELL 1
            #define DELETED_CELL -1

            int status;
            Key value;
            Cell() : status(EMPTY_CELL), value{} {}
            explicit Cell(const Key& value) : status(ACTIVE_CELL), value{value} {}
        };

        vector<Cell> table;
        size_t _size; // active cell count
        size_t deleted_cell_count;
        float _max_load_factor;


        // Primality test in C-family on Wikipedia: https://en.wikipedia.org/wiki/Primality_test#C,_C++,_C#_&_D - this is not my algorithm
        bool is_prime(size_t n) {
            if (n == 2 || n == 3) return true;
            if (n <= 1 || n % 2 == 0 || n % 3 == 0) return false;
            for (size_t i = 5; i * i <= n; i += 6)
                if (n % i == 0 || n % (i + 2) == 0) return false;
        
            return true;
        }

        // helper method for rehashing 
        size_t find_next_prime(size_t size) {
            size_t ret = size * 2 + 1;
            while (!is_prime(ret)) ret += 2; 
            return ret;
        }

        void rehash(size_t size) {
            // save old elements 
            vector<Cell> old_table = table;
            this->table = vector<Cell>{size}; // all cells are initalized to empty here 

            // reinsert all old values
            _size = 0;
            deleted_cell_count = 0;
            for (Cell cell : old_table) {
                if (cell.status == ACTIVE_CELL)
                    insert(cell.value); // updates _size, _current_load_factor, and status of new cells
            }
        }


    public:
        // constructors
        HashTable() : table{11}, _size{0}, deleted_cell_count{0}, _max_load_factor{0.5} {}
        explicit HashTable(size_t size) : table{size}, _size{0}, deleted_cell_count{0}, _max_load_factor{0.5} {}

        // capacity
        bool is_empty() const { return _size == 0; }
        size_t size() const { return _size; }
        size_t table_size() const { return table.size(); }

        // modifiers
        void make_empty() {
            table = vector<Cell>{table.size()};
            _size = 0;
            deleted_cell_count = 0;
        }

        bool insert(const Key& value) { // returns true on successful insert, false on failed insert
            if (contains(value)) return false;

            // rehash check: second condition is for a series of insertions followed by deltions which never cause a rehash but lazy deletion leaves no open cells.
            if (static_cast<float>(_size + 1) / table.size() > _max_load_factor ||
                static_cast<float>(1 + _size + deleted_cell_count) / table.size() > _max_load_factor)
                rehash(find_next_prime(table.size()));

            // perform insert
            table.at(position(value)) = Cell(value);
            // update members
            _size++;
            return true;
        }
        
        size_t remove(const Key& value) { // returns 1 on successful removal, 0 on failed removal.
            if (!contains(value)) return 0;
            // value not contained, delete using lazy deletion
            table.at(position(value)).status = DELETED_CELL;
            _size--;
            deleted_cell_count++;

            return 1;
        }

        // lookup
        bool contains(const Key& value) const {
            return table.at(position(value)).status == ACTIVE_CELL;
        }

        // position
        size_t position(const Key& value) const {
            // collision resolution done using quadratic probing
            int i = 0;
            while (true) {
                size_t index = (Hash{}(value) + i * i) % table.size(); // obtain our attempt at a location
                if (table.at(index).status == EMPTY_CELL || table.at(index).value == value)
                    return index; // found an available cell or index value should be
                i++; // else incremenet i and try again
            }
        }

        // visualization
        void print_table(std::ostream& os = std::cout) const {
            if (is_empty()) {
                os << "<empty>" << endl;
                return;
            }

            for (Cell cell : table) {
                if (cell.status == ACTIVE_CELL) {
                    os << position(cell.value) << ": ";
                    os << cell.value << endl;
                }
            }
        }

        // FOR TESTING ONLY
        Cell at(size_t index) { return table.at(index); }
        size_t hash(const Key& value) { return Hash{}(value); }
};
