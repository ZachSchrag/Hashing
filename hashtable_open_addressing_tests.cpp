#include "hashtable_open_addressing.h"
#include <sstream>
#include <iostream>

#define black   "\033[30m"
#define red     "\033[31m"
#define green   "\033[32m"
#define yellow  "\033[33m"
#define blue    "\033[34m"
#define magenta "\033[35m"
#define cyan    "\033[36m"
#define white   "\033[37m"
#define reset   "\033[m"

#define to_be ==
#define not_to_be !=
#define is to_be
#define is_not not_to_be

#define expect(X) try {\
  if (!(X)) {\
    std::cout << red "  [fail]" reset " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << "expected " << #X << "." << reset << std::endl;\
  }\
} catch(...) {\
  std::cout << red "  [fail]" reset " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << #X << " threw an unexpected exception." << reset << std::endl;\
}

#define assert(X) try {\
  if (!(X)) {\
    std::cout << red "  [fail]" reset " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << "failed assertion that " << #X << "." << reset << std::endl;\
    std::abort();\
  }\
} catch(...) {\
  std::cout << red "  [fail]" reset " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << #X << " assertion threw an unexpected exception." << reset << std::endl;\
}

#define expect_throw(X,E) {\
  bool threw_expected_exception = false;\
  try { X; }\
  catch(const E& err) {\
    threw_expected_exception = true;\
  } catch(...) {\
    std::cout << blue << "  [help] " << #X << " threw an incorrect exception." << reset << std::endl;\
  }\
  if (!threw_expected_exception) {\
    std::cout << red <<"  [fail]" << reset << " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << "expected " << #X << " to throw " << #E <<"." << reset <<std::endl;\
  }\
}

#define expect_no_throw(X) {\
  try { X; }\
  catch(...) {\
    std::cout << red << "  [fail]" << red << " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << "expected " << #X << " not to throw an excpetion." << reset << std::endl;\
  }\
}

int main() {
    // is_empty() / make_empty
    {
        HashTable<int> intTable;
        expect(intTable.is_empty() to_be true);
        intTable.insert(1);
        expect(intTable.is_empty() to_be false);
        intTable.make_empty();
        expect(intTable.is_empty() to_be true);
        expect(intTable.table_size() to_be 11);
    }

    // default constructor
    {
        HashTable<int> intTable;
        expect(intTable.size() to_be 0);
        expect(intTable.table_size() to_be 11);
        for (size_t i = 0; i < intTable.table_size(); i++) {
            expect(intTable.at(i).status == EMPTY_CELL);
        }
        expect(intTable.is_empty() to_be true);
        expect(intTable.contains(0) to_be false);
    }

    // explicit constructor
    {
        HashTable<double> doubleTable(23);
        expect(doubleTable.size() to_be 0);
        expect(doubleTable.table_size() to_be 23);
        for (size_t i = 0; i < doubleTable.table_size(); i++) {
            expect(doubleTable.at(i).status == EMPTY_CELL);
        }
        expect(doubleTable.is_empty() to_be true);
        expect(doubleTable.contains(0.0) to_be false);
    }

    // position - no collisions
    {
        HashTable<int> intTable; // hash(int) just return int so
        expect(intTable.position(0) to_be 0);
        expect(intTable.position(1) to_be 1);
        expect(intTable.position(2) to_be 2);
        expect(intTable.position(3) to_be 3);
        expect(intTable.position(4) to_be 4);
        expect(intTable.position(5) to_be 5);
        expect(intTable.position(6) to_be 6);
        expect(intTable.position(7) to_be 7);
        expect(intTable.position(8) to_be 8);
        expect(intTable.position(9) to_be 9);
        expect(intTable.position(10) to_be 10);

        HashTable<double> doubleTable; // use testing hash function to see where it should go
        expect(doubleTable.position(1.0) to_be doubleTable.hash(1.0) % 11);
        expect(doubleTable.position(34.5) to_be doubleTable.hash(34.5) % 11);
        expect(doubleTable.position(1231233.2) to_be doubleTable.hash(1231233.2) % 11);
        expect(doubleTable.position(973912.4123) to_be doubleTable.hash(973912.4123) % 11);
        expect(doubleTable.position(-273.3) to_be doubleTable.hash(-273.3) % 11);
        expect(doubleTable.position(761.0) to_be doubleTable.hash(761.0) % 11);

        HashTable<char> charTable(23); // use testing hash function to see where it should go
        expect(charTable.position('a') to_be charTable.hash('a') % 23);
        expect(charTable.position('9') to_be charTable.hash('9') % 23);
        expect(charTable.position('n') to_be charTable.hash('n') % 23);
        expect(charTable.position('r') to_be charTable.hash('r') % 23);
        expect(charTable.position('_') to_be charTable.hash('_') % 23);
        expect(charTable.position('/') to_be charTable.hash('/') % 23);
        expect(charTable.position('\\') to_be charTable.hash('\\') % 23);
        expect(charTable.position('`') to_be charTable.hash('`') % 23);

        HashTable<std::string> stringTable(107);
        expect(stringTable.position("hello") to_be stringTable.hash("hello") % 107);
        expect(stringTable.position("world") to_be stringTable.hash("world") % 107);
        expect(stringTable.position("space space space") to_be stringTable.hash("space space space") % 107);
        expect(stringTable.position(" ") to_be stringTable.hash(" ") % 107);
        expect(stringTable.position("@@#(*m!(*#DWLOAMD!?\\~") to_be stringTable.hash("@@#(*m!(*#DWLOAMD!?\\~") % 107);
    }

    // insert - no collisions 
    {
        HashTable<int> intTable;
        expect(intTable.insert(1) to_be true); 
        expect(intTable.size() to_be 1);
        expect(intTable.contains(1) to_be true);
        expect(intTable.at(1).status to_be ACTIVE_CELL);
        expect(intTable.at(1).value to_be 1);

        expect(intTable.insert(2) to_be true);
        expect(intTable.size() to_be 2);
        expect(intTable.contains(2) to_be true);
        expect(intTable.at(2).status to_be ACTIVE_CELL);
        expect(intTable.at(2).value to_be 2);
        
        expect(intTable.insert(3) to_be true);
        expect(intTable.size() to_be 3);
        expect(intTable.contains(3) to_be true);
        expect(intTable.at(3).status to_be ACTIVE_CELL);
        expect(intTable.at(3).value to_be 3);
        // attempt insert duplicate
        expect(intTable.insert(3) to_be false);
        expect(intTable.size() to_be 3);
        expect(intTable.contains(3) to_be true);
        expect(intTable.at(3).status to_be ACTIVE_CELL);
        expect(intTable.at(3).value to_be 3);
        // check state before we insert 4
        expect(intTable.contains(4) to_be false);
        expect(intTable.at(4).status to_be EMPTY_CELL);
        // after insert 4
        expect(intTable.insert(4) to_be true);
        expect(intTable.size() to_be 4);
        expect(intTable.contains(4) to_be true);
        expect(intTable.at(4).status to_be ACTIVE_CELL);
        expect(intTable.at(4).value to_be 4);


        HashTable<char> charTable;
        expect(charTable.insert('a') to_be true);
        expect(charTable.size() to_be 1);
        expect(charTable.contains('a') to_be true);
        expect(charTable.at(charTable.hash('a') % charTable.table_size()).status to_be ACTIVE_CELL);
        expect(charTable.at(charTable.hash('a') % charTable.table_size()).value to_be 'a');

        expect(charTable.insert('b') to_be true);
        expect(charTable.size() to_be 2);
        expect(charTable.contains('b') to_be true);
        expect(charTable.at(charTable.hash('b') % charTable.table_size()).status to_be ACTIVE_CELL);
        expect(charTable.at(charTable.hash('b') % charTable.table_size()).value to_be 'b');

        expect(charTable.insert('c') to_be true);
        expect(charTable.size() to_be 3);
        expect(charTable.contains('c') to_be true);
        expect(charTable.at(charTable.hash('c') % charTable.table_size()).status to_be ACTIVE_CELL);
        expect(charTable.at(charTable.hash('c') % charTable.table_size()).value to_be 'c');
        // attempt insert duplicate
        expect(charTable.insert('c') to_be false);
        expect(charTable.size() to_be 3);
        expect(charTable.contains('c') to_be true);
        expect(charTable.at(charTable.hash('c') % charTable.table_size()).status to_be ACTIVE_CELL);
        expect(charTable.at(charTable.hash('c') % charTable.table_size()).value to_be 'c');
        // check state before inserting a new value
        expect(charTable.contains('d') to_be false);
        expect(charTable.at(charTable.hash('d') % charTable.table_size()).status to_be EMPTY_CELL);

        expect(charTable.insert('d') to_be true);
        expect(charTable.size() to_be 4);
        expect(charTable.contains('d') to_be true);
        expect(charTable.at(charTable.hash('d') % charTable.table_size()).status to_be ACTIVE_CELL);
        expect(charTable.at(charTable.hash('d') % charTable.table_size()).value to_be 'd');
    }

    // contains
    {
        HashTable<int> intTable;
        expect(intTable.insert(2) to_be true);
        expect(intTable.contains(2) to_be true);
        expect(intTable.contains(3) to_be false);
        expect(intTable.insert(3) to_be true);
        expect(intTable.contains(3) to_be true);
        expect(intTable.remove(3) to_be 1);
        expect(intTable.contains(3) to_be false);
        expect(intTable.insert(13) to_be true);
        expect(intTable.contains(13) to_be true);

        HashTable<double> doubleTable;
        expect(doubleTable.insert(2.0) to_be true);
        expect(doubleTable.contains(2.0) to_be true);
        expect(doubleTable.contains(3.0) to_be false);
        expect(doubleTable.insert(3.0) to_be true);
        expect(doubleTable.contains(3.0) to_be true);
        expect(doubleTable.remove(3.0) to_be 1);
        expect(doubleTable.contains(3.0) to_be false);

        HashTable<double> charTable;
        expect(charTable.insert('a') to_be true);
        expect(charTable.contains('a') to_be true);
        expect(charTable.contains('b') to_be false);
        expect(charTable.insert('b') to_be true);
        expect(charTable.contains('b') to_be true);
        expect(charTable.remove('b') to_be 1);
        expect(charTable.contains('b') to_be false);
    }

    // remove and inserting into removed values - no collisons
    {
        HashTable<int> intTable;
        intTable.insert(0);
        intTable.insert(6);
        intTable.insert(10);

        // remove a value
        expect(intTable.contains(6) to_be true);
        expect(intTable.at(6).status to_be ACTIVE_CELL);
        expect(intTable.at(6).value to_be 6);
        expect(intTable.size() to_be 3);
        expect(intTable.table_size() to_be 11);
        expect(intTable.remove(6) to_be 1);
        expect(intTable.contains(6) to_be false);
        expect(intTable.at(6).status to_be DELETED_CELL);
        expect(intTable.at(6).value to_be 6); // should still be there but as a ghost value
        expect(intTable.size() to_be 2);
        expect(intTable.table_size() to_be 11);
        
        // attempt to remove value that is not in table
        expect(intTable.remove(6) to_be 0);
        expect(intTable.contains(6) to_be false);
        expect(intTable.at(6).status to_be DELETED_CELL);
        expect(intTable.at(6).value to_be 6);
        expect(intTable.size() to_be 2);
        expect(intTable.table_size() to_be 11);
        // remove 0
        expect(intTable.contains(0) to_be true);
        expect(intTable.at(0).status to_be ACTIVE_CELL)
        expect(intTable.at(0).value to_be 0)
        expect(intTable.remove(0) to_be 1);
        expect(intTable.contains(0) to_be false);
        expect(intTable.at(0).status to_be DELETED_CELL);
        expect(intTable.at(0).value to_be 0); // lazy deletion
        expect(intTable.size() to_be 1);
        expect(intTable.table_size() to_be 11);
        // reinsert 0
        expect(intTable.insert(0) to_be true);
        expect(intTable.contains(0) to_be true);
        expect(intTable.at(0).status to_be ACTIVE_CELL);
        expect(intTable.at(0).value to_be 0);
        expect(intTable.size() to_be 2);
        expect(intTable.table_size() to_be 11);
    }

    // insert which cause a rehash
    {
        HashTable<int> intTable;
        intTable.insert(0);
        intTable.insert(1);
        intTable.insert(2);
        intTable.insert(3);
        intTable.insert(4);
        expect(intTable.size() to_be 5);
        expect(intTable.table_size() to_be 11);
        // expect next insert to cause rehash as load_factor > 0.5
        intTable.insert(5);
        expect(intTable.size() to_be 6);
        expect(intTable.table_size() to_be 23);
        expect(intTable.at(0).status to_be ACTIVE_CELL);
        expect(intTable.at(0).value to_be 0);
        expect(intTable.at(1).status to_be ACTIVE_CELL);
        expect(intTable.at(1).value to_be 1);
        expect(intTable.at(2).status to_be ACTIVE_CELL);
        expect(intTable.at(2).value to_be 2);
        expect(intTable.at(3).status to_be ACTIVE_CELL);
        expect(intTable.at(3).value to_be 3);
        expect(intTable.at(4).status to_be ACTIVE_CELL);
        expect(intTable.at(4).value to_be 4);
        expect(intTable.at(5).status to_be ACTIVE_CELL);
        expect(intTable.at(5).value to_be 5);
    }

    // insert that causes collision
    {
        HashTable<int> intTable;
        intTable.insert(0);
        intTable.insert(1);
        intTable.insert(2);
        // expect next insert to cause collision
        expect(intTable.insert(11) to_be true); // index 0 is taken, index 1 is taken, expect to be placed in index 4.
        expect(intTable.size() to_be 4);
        expect(intTable.table_size() to_be 11);
        expect(intTable.at(0).status to_be ACTIVE_CELL);
        expect(intTable.at(0).value not_to_be 11);
        expect(intTable.at(1).status to_be ACTIVE_CELL);
        expect(intTable.at(1).value not_to_be 11);
        expect(intTable.at(4).status to_be ACTIVE_CELL);
        expect(intTable.at(4).value to_be 11);

        // remove a value to ensure probing still happens past deleted cells
        expect(intTable.remove(1) to_be 1);
        expect(intTable.at(1).status to_be DELETED_CELL);
        expect(intTable.size() to_be 3);
        // insert value which would hash to index 1. expect it to not be placed in 1. expect to be placed in index 5 (index 2 is taken)
        expect(intTable.insert(12) to_be true);
        expect(intTable.at(1).status to_be DELETED_CELL);
        expect(intTable.at(1).value not_to_be 12);
        expect(intTable.at(2).status to_be ACTIVE_CELL);
        expect(intTable.at(2).value not_to_be 12);
        expect(intTable.at(5).status to_be ACTIVE_CELL);
        expect(intTable.at(5).value to_be 12);
        expect(intTable.size() to_be 4);

        // expect next insert to cause rehash
        expect(intTable.insert(15) to_be true);
        expect(intTable.table_size() to_be 23);
        expect(intTable.at(0).status to_be ACTIVE_CELL);
        expect(intTable.at(0).value to_be 0);
        expect(intTable.at(1).status to_be EMPTY_CELL);
        expect(intTable.at(1).value to_be int());
        expect(intTable.at(2).status to_be ACTIVE_CELL);
        expect(intTable.at(2).value to_be 2);
        expect(intTable.at(11).status to_be ACTIVE_CELL);
        expect(intTable.at(11).value to_be 11);
        expect(intTable.at(12).status to_be ACTIVE_CELL);
        expect(intTable.at(12).value to_be 12);
        expect(intTable.at(15).status to_be ACTIVE_CELL);
        expect(intTable.at(15).value to_be 15);
        expect(intTable.size() to_be 5);

        // expect next insert to cause a rehash. expect to see deleted cell turned to empty and new locations of the existing elements
        expect(intTable.insert(22));
        expect(intTable.table_size() to_be 23);
        expect(intTable.size() to_be 6);
        expect(intTable.at(0).status to_be ACTIVE_CELL);
        expect(intTable.at(0).value to_be 0);
        expect(intTable.at(1).status to_be EMPTY_CELL);
        expect(intTable.at(1).value to_be int());
        expect(intTable.at(2).status to_be ACTIVE_CELL);
        expect(intTable.at(2).value to_be 2);
        expect(intTable.at(11).status to_be ACTIVE_CELL);
        expect(intTable.at(11).value to_be 11);
        expect(intTable.at(12).status to_be ACTIVE_CELL);
        expect(intTable.at(12).value to_be 12);
        expect(intTable.at(15).status to_be ACTIVE_CELL);
        expect(intTable.at(15).value to_be 15);
        expect(intTable.at(22).status to_be ACTIVE_CELL);
        expect(intTable.at(22).value to_be 22);
    }

    // print table
    {
      HashTable<int> intTable;

      intTable.insert(2);
      intTable.insert(3);
      std::stringstream ss;
      intTable.print_table(ss);
      std::string expected = "2: 2\n3: 3\n";
      expect(ss.str() to_be expected);
    }


    /*
      problem found: when a series of insertions was followed by removals causing no rehash until there were no open spaces
      in the table causing the position function to get stuck in a loop. updated rehash check to account for size + deleted_cell_count
      to be taken into consideration when determining when to rehash
    
    insert and remove until rehash test. just going to use a gui to insert and remove values to see if I can break it
    {
      HashTable<int> intTable;
      int action;
      cout << ">> ";
      while (std::cin >> action) {
        if (action > 0) {
          cout << "inserting: " << action << endl;
          intTable.insert(action);
          cout << "insertion complete" << endl;
          cout << "New number of active cells: " << intTable.size() << endl;;
          cout << "New number of total cells: " << intTable.table_size() << endl;
          cout << "New load factor: " << static_cast<float>(intTable.size()) / intTable.table_size() << endl; 

        } else {
          cout << "removing: " << action << endl;
          intTable.remove(-action);
          cout << "removal complete" << endl;
          cout << "New number of active cells: " << intTable.size() << endl;;
          cout << "New number of total cells: " << intTable.table_size() << endl;
          cout << "New load factor: " << static_cast<float>(intTable.size()) / intTable.table_size() << endl; 
        }
        cout << "Table looks like: " << endl;
        intTable.print_table();
        cout << endl << ">> ";
      }
    }
    */

    // all the above kind of test but commenting out to hide output
    // {
    //     std::cout << "make an empty hash table with 11 buckets for strings" << std::endl;
    //     HashTable<std::string> table(11);

    //     std::cout << "initial size is " << table.size() << std::endl;
    //     std::cout << "initial table size is " << table.table_size() << std::endl;

    //     std::cout << "insert several strings" << std::endl;   
    //     table.insert("And them who hold High Places");
    //     table.insert("Must be the ones to start");
    //     table.insert("To mold a new Reality");
    //     table.insert("Closer to the Heart");
    //     table.insert("The Blacksmith and the Artist");
    //     table.insert("Reflect it in their Art");
    //     table.insert("Forge their Creativity");
    //     table.insert("Closer to the Heart");
    //     table.insert("Philosophers and Plowmen");
    //     table.insert("Each must know their Part");
    //     table.insert("To sow a new Mentality");
    //     table.insert("Closer to the Heart");
    //     table.insert("You can be the Captain");
    //     table.insert("I will draw the Chart");
    //     table.insert("Sailing into Destiny");
    //     table.insert("Closer to the Heart");

    //     std::cout << "size is " << table.size() << std::endl;
    //     std::cout << "table size is " << table.table_size() << std::endl;

    //     {
    //     std::cout << "print the table" << std::endl;
    //     std::stringstream ss;
    //     table.print_table(ss);
    //     std::cout << ss.str() << std::endl;
    //     }

    //     std::cout << "remove \"Philosophers and Plowmen\"" << std::endl;
    //     table.remove("Philosophers and Plowmen");
    //     std::cout << "remove \"Each must know their Part\"" << std::endl;
    //     table.remove("Each must know their Part");

    //     std::cout << "size is " << table.size() << std::endl;
    //     std::cout << "table size is " << table.table_size() << std::endl;

    //     {
    //     std::cout << "print the table" << std::endl;
    //     std::stringstream ss;
    //     table.print_table(ss);
    //     std::cout << ss.str() << std::endl;
    //     }

    //     std::cout << "find \"The Blacksmith and the Artist\"" << std::endl;
    //     size_t index = table.position("The Blacksmith and the Artist");
    //     std::cout << " ==> cell " << index << std::endl;

    //     std::cout << "make the table empty" << std::endl;
    //     table.make_empty();

    //     std::cout << "size is " << table.size() << std::endl;
    //     std::cout << "table size is " << table.table_size() << std::endl;

    //     {
    //     std::cout << "print the table" << std::endl;
    //     std::stringstream ss;
    //     table.print_table(ss);
    //     std::cout << ss.str() << std::endl;
    //     }
    // }
    
    return 0;
}