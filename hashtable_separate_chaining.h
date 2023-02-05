/*
 *  Implementation of a separate chaining hashtable which has an underlying vector of lists of keys. Max load factor is set to 1.0 by default
 *  Written by Zach Schrag
*/


#pragma once
#include <functional>
#include <vector>
#include <list>
#include <stdexcept>
#include <iostream> // for print_table only

using std::vector, std::list, std::cout, std::endl;

template <class Key, class Hash=std::hash<Key>>
class HashTable {

    private:
        vector<list<Key>> table;
        size_t _size;
        float _current_load_factor;
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

    public:
        // constructors
        HashTable() : table{11}, _size{0}, _current_load_factor{0.0}, _max_load_factor{1.0} {}
        explicit HashTable(size_t size) : table{size}, _size{0}, _current_load_factor{0.0}, _max_load_factor{1.0} {}

        // capacity
        bool is_empty() const { return _size == 0; }
        size_t size() const { return _size; }

        // modifiers
        void make_empty() { 
            table = vector<list<Key>>{table.size()};
            _size = 0;
            _current_load_factor = 0.0;
        }
        
        bool insert(const Key& value) { // returns true on successful insert, false on failed insert
            if (contains(value)) return false;

            // rehash check
            float load_factor_check = static_cast<float>(_size + 1) / table.size();
            if (load_factor_check > _max_load_factor)
                rehash(find_next_prime(table.size()));

            // perform insert
            size_t index = Hash{}(value) % table.size();
            table.at(index).push_front(value);
            _size++;
            _current_load_factor = static_cast<float>(_size) / table.size();

            return true;
        }

        size_t remove(const Key& value) { // returns 1 on successful removal, 0 on failed removal
            if (!contains(value)) return 0;

            // perform removal
            size_t index = Hash{}(value) % table.size();
            table.at(index).remove(value);
            _size--;
            _current_load_factor = static_cast<float>(_size) / table.size();

            return 1;
        }

        // lookup
        bool contains(const Key& value) const {
            size_t index = Hash{}(value) % table.size();
            for (Key key : table.at(index)) {
                if (key == value) return true;
            }

            return false;
        }

        // bucket interface
        size_t bucket_count() const { return table.size(); }
        size_t bucket_size(size_t index) const {
            if (index >= table.size()) throw std::out_of_range("specified bucket is out of bounds");

            return table.at(index).size();
        }
        size_t bucket(const Key& value) const { return Hash{}(value) % table.size(); }

        // hash policy
        float load_factor() const { return _current_load_factor; }
        float max_load_factor() const { return _max_load_factor; }

        void max_load_factor(float max) {
            if (max <= 0) throw std::invalid_argument("invalid max load factor value");
            _max_load_factor = max;
            if (_current_load_factor > _max_load_factor) {
                rehash(find_next_prime(table.size()));
            }
        }

        void rehash(size_t num_buckets) {
            if (num_buckets == table.size()) return; // nothing to do
            if (_size > num_buckets * _max_load_factor) 
                num_buckets = _size / _max_load_factor; // minimum number of buckets needed if passed something that will cause rehash

            // save old values and reinsert them
            vector<list<Key>> old_table = table;
            this->table = vector<list<Key>>{num_buckets};
            _size = 0;
            _current_load_factor = 0;

            for (list bucket : old_table) {
                for (Key key : bucket) {
                    insert(key); // _size and _current_load_factor will be properly updated here
                }
            }
        }

        // visualization
        void print_table(std::ostream& os = std::cout) const {
            if (is_empty()) {
                os << "<empty>" << endl;
                return;
            }

            for (list bucket : table) {
                if (bucket.size() != 0) {
                    os << this->bucket(bucket.front()) << ": [";
                    for (Key key : bucket) {
                        if (key == bucket.back()) 
                            os << key;
                        else 
                            os << key << ", ";
                    }
                    os << "]" << endl;
                }
            }
        }

        // FOR TESTING BUCKET INTERFACE METHODS
        size_t hash(const Key& value) { return Hash{}(value); } 
};
