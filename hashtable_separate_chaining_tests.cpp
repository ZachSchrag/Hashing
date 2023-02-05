#include "hashtable_separate_chaining.h"
#include <sstream>

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
        expect(intTable.bucket_count() to_be 11);
    }

    // default constructor
    {
        HashTable<int> intTable;
        expect(intTable.size() to_be 0);
        expect(intTable.bucket_count() to_be 11);
        for (size_t i = 0; i < intTable.bucket_count(); i++) { expect(intTable.bucket_size(i) to_be 0); }
        expect(intTable.is_empty() to_be true);
        expect(intTable.contains(0) to_be false);
    }

    // explicit constructor
    {
        HashTable<double> doubleTable(23);
        expect(doubleTable.size() to_be 0);
        expect(doubleTable.bucket_count() to_be 23);
        for (size_t i = 0; i < doubleTable.bucket_count(); i++) { expect(doubleTable.bucket_size(i) to_be 0); }
        expect(doubleTable.is_empty() to_be true);
        expect(doubleTable.contains(0.0) to_be false);
    }

    // bucket_count
    {
      HashTable<int> intTable(23);
      expect(intTable.bucket_count() to_be 23);

      HashTable<char> charTable;
      expect(charTable.bucket_count() to_be 11);

      HashTable<std::string> stringTable(211);
      expect(stringTable.bucket_count() to_be 211);

      HashTable<double> doubleTable(373);
      expect(doubleTable.bucket_count() to_be 373);
    }

    // max_load_factor
    {
      HashTable<int> intTable;
      expect_no_throw(intTable.max_load_factor(2.0));
      expect_throw(intTable.max_load_factor(-1.0), std::invalid_argument);
      expect_throw(intTable.max_load_factor(0), std::invalid_argument);
    }

    // max_load_factor pt 2
    {
      HashTable<int> intTable;

      intTable.insert(3);
      intTable.insert(4);
      intTable.insert(5);
      intTable.insert(6);
      intTable.insert(7);
      intTable.insert(8);
      // intTable has load factor > .5 (6/11 > 1/2)
      // set a load factor which will force a rehash
      intTable.max_load_factor(.5);
      expect(intTable.max_load_factor() to_be 0.5);
      expect(intTable.load_factor() <= 0.5);
      expect(intTable.bucket_count() to_be 23);
      expect(intTable.size() to_be 6);
    }

    // bucket_size
    {
      HashTable<int> intTable;
      intTable.insert(1);
      expect(intTable.bucket_size(0) to_be 0);
      expect(intTable.bucket_size(1) to_be 1);
      expect(intTable.bucket_size(2) to_be 0);
      expect(intTable.bucket_size(3) to_be 0);
      expect(intTable.bucket_size(4) to_be 0);
      expect(intTable.bucket_size(5) to_be 0);
      expect(intTable.bucket_size(6) to_be 0);
      expect(intTable.bucket_size(7) to_be 0);
      expect(intTable.bucket_size(8) to_be 0);
      expect(intTable.bucket_size(9) to_be 0);
      expect(intTable.bucket_size(10) to_be 0);
      intTable.insert(2);
      expect(intTable.bucket_size(0) to_be 0);
      expect(intTable.bucket_size(1) to_be 1);
      expect(intTable.bucket_size(2) to_be 1);
      expect(intTable.bucket_size(3) to_be 0);
      expect(intTable.bucket_size(4) to_be 0);
      expect(intTable.bucket_size(5) to_be 0);
      expect(intTable.bucket_size(6) to_be 0);
      expect(intTable.bucket_size(7) to_be 0);
      expect(intTable.bucket_size(8) to_be 0);
      expect(intTable.bucket_size(9) to_be 0);
      expect(intTable.bucket_size(10) to_be 0);
      intTable.insert(0);
      expect(intTable.bucket_size(0) to_be 1);
      expect(intTable.bucket_size(1) to_be 1);
      expect(intTable.bucket_size(2) to_be 1);
      expect(intTable.bucket_size(3) to_be 0);
      expect(intTable.bucket_size(4) to_be 0);
      expect(intTable.bucket_size(5) to_be 0);
      expect(intTable.bucket_size(6) to_be 0);
      expect(intTable.bucket_size(7) to_be 0);
      expect(intTable.bucket_size(8) to_be 0);
      expect(intTable.bucket_size(9) to_be 0);
      expect(intTable.bucket_size(10) to_be 0);
      intTable.insert(9);
      expect(intTable.bucket_size(0) to_be 1);
      expect(intTable.bucket_size(1) to_be 1);
      expect(intTable.bucket_size(2) to_be 1);
      expect(intTable.bucket_size(3) to_be 0);
      expect(intTable.bucket_size(4) to_be 0);
      expect(intTable.bucket_size(5) to_be 0);
      expect(intTable.bucket_size(6) to_be 0);
      expect(intTable.bucket_size(7) to_be 0);
      expect(intTable.bucket_size(8) to_be 0);
      expect(intTable.bucket_size(9) to_be 1);
      expect(intTable.bucket_size(10) to_be 0);
      intTable.insert(11);
      expect(intTable.bucket_size(0) to_be 2);
      expect(intTable.bucket_size(1) to_be 1);
      expect(intTable.bucket_size(2) to_be 1);
      expect(intTable.bucket_size(3) to_be 0);
      expect(intTable.bucket_size(4) to_be 0);
      expect(intTable.bucket_size(5) to_be 0);
      expect(intTable.bucket_size(6) to_be 0);
      expect(intTable.bucket_size(7) to_be 0);
      expect(intTable.bucket_size(8) to_be 0);
      expect(intTable.bucket_size(9) to_be 1);
      expect(intTable.bucket_size(10) to_be 0);
      expect_throw(intTable.bucket_size(11), std::out_of_range);


      HashTable<double> doubleTable(20);
      size_t double_buckets[20]{}; // using this array to keep track of the count of the correct buckets
      for (double i = 0.0; i < 20.0; i++) {
        double_buckets[doubleTable.hash(i) % 20]++;
        doubleTable.insert(i);
      }
      expect(doubleTable.bucket_size(0) to_be double_buckets[0]);
      expect(doubleTable.bucket_size(1) to_be double_buckets[1]);
      expect(doubleTable.bucket_size(2) to_be double_buckets[2]);
      expect(doubleTable.bucket_size(3) to_be double_buckets[3]);
      expect(doubleTable.bucket_size(4) to_be double_buckets[4]);
      expect(doubleTable.bucket_size(5) to_be double_buckets[5]);
      expect(doubleTable.bucket_size(6) to_be double_buckets[6]);
      expect(doubleTable.bucket_size(7) to_be double_buckets[7]);
      expect(doubleTable.bucket_size(8) to_be double_buckets[8]);
      expect(doubleTable.bucket_size(9) to_be double_buckets[9]);
      expect(doubleTable.bucket_size(10) to_be double_buckets[10]);
      expect(doubleTable.bucket_size(11) to_be double_buckets[11]);
      expect(doubleTable.bucket_size(12) to_be double_buckets[12]);
      expect(doubleTable.bucket_size(13) to_be double_buckets[13]);
      expect(doubleTable.bucket_size(14) to_be double_buckets[14]);
      expect(doubleTable.bucket_size(15) to_be double_buckets[15]);
      expect(doubleTable.bucket_size(16) to_be double_buckets[16]);
      expect(doubleTable.bucket_size(17) to_be double_buckets[17]);
      expect(doubleTable.bucket_size(18) to_be double_buckets[18]);
      expect(doubleTable.bucket_size(19) to_be double_buckets[19]);
      expect_throw(doubleTable.bucket_size(20), std::out_of_range);

      HashTable<char> charTable(26);
      size_t char_buckets[26]{}; // using this array to keep track of the count of the correct buckets
      for (char c = 'a'; c <= 'z'; c++) {
        char_buckets[charTable.hash(c) % 26]++;
        charTable.insert(c);
      }
      expect(charTable.bucket_size(0) to_be char_buckets[0]);
      expect(charTable.bucket_size(1) to_be char_buckets[1]);
      expect(charTable.bucket_size(2) to_be char_buckets[2]);
      expect(charTable.bucket_size(3) to_be char_buckets[3]);
      expect(charTable.bucket_size(4) to_be char_buckets[4]);
      expect(charTable.bucket_size(5) to_be char_buckets[5]);
      expect(charTable.bucket_size(6) to_be char_buckets[6]);
      expect(charTable.bucket_size(7) to_be char_buckets[7]);
      expect(charTable.bucket_size(8) to_be char_buckets[8]);
      expect(charTable.bucket_size(9) to_be char_buckets[9]);
      expect(charTable.bucket_size(10) to_be char_buckets[10]);
      expect(charTable.bucket_size(11) to_be char_buckets[11]);
      expect(charTable.bucket_size(12) to_be char_buckets[12]);
      expect(charTable.bucket_size(13) to_be char_buckets[13]);
      expect(charTable.bucket_size(14) to_be char_buckets[14]);
      expect(charTable.bucket_size(15) to_be char_buckets[15]);
      expect(charTable.bucket_size(16) to_be char_buckets[16]);
      expect(charTable.bucket_size(17) to_be char_buckets[17]);
      expect(charTable.bucket_size(18) to_be char_buckets[18]);
      expect(charTable.bucket_size(19) to_be char_buckets[19]);
      expect(charTable.bucket_size(20) to_be char_buckets[20]);
      expect(charTable.bucket_size(21) to_be char_buckets[21]);
      expect(charTable.bucket_size(22) to_be char_buckets[22]);
      expect(charTable.bucket_size(23) to_be char_buckets[23]);
      expect(charTable.bucket_size(24) to_be char_buckets[24]);
      expect(charTable.bucket_size(25) to_be char_buckets[25]);
      expect_throw(charTable.bucket_size(26), std::out_of_range);
    }


    // bucket
    {
      HashTable<int> intTable;
      expect(intTable.bucket(0) to_be 0);
      expect(intTable.bucket(1) to_be 1);
      expect(intTable.bucket(2) to_be 2);
      expect(intTable.bucket(3) to_be 3);
      expect(intTable.bucket(4) to_be 4);
      expect(intTable.bucket(5) to_be 5);
      expect(intTable.bucket(6) to_be 6);
      expect(intTable.bucket(7) to_be 7);
      expect(intTable.bucket(8) to_be 8);
      expect(intTable.bucket(9) to_be 9);
      expect(intTable.bucket(10) to_be 10);
      expect(intTable.bucket(11) to_be 0);
      expect(intTable.bucket(12) to_be 1);
      expect(intTable.bucket(13) to_be 2);

      HashTable<double> doubleTable;
      expect(doubleTable.bucket(0.0) to_be doubleTable.hash(0.0) % 11);
      expect(doubleTable.bucket(1.0) to_be doubleTable.hash(1.0) % 11);
      expect(doubleTable.bucket(2.0) to_be doubleTable.hash(2.0) % 11);
      expect(doubleTable.bucket(3.0) to_be doubleTable.hash(3.0) % 11);
      expect(doubleTable.bucket(4.0) to_be doubleTable.hash(4.0) % 11);
      expect(doubleTable.bucket(5.0) to_be doubleTable.hash(5.0) % 11);
      expect(doubleTable.bucket(6.0) to_be doubleTable.hash(6.0) % 11);
      expect(doubleTable.bucket(7.0) to_be doubleTable.hash(7.0) % 11);
      expect(doubleTable.bucket(8.0) to_be doubleTable.hash(8.0) % 11);
      expect(doubleTable.bucket(9.0) to_be doubleTable.hash(9.0) % 11);
      expect(doubleTable.bucket(10.0) to_be doubleTable.hash(10.0) % 11);
      expect(doubleTable.bucket(341.2) to_be doubleTable.hash(341.2) % 11);
      expect(doubleTable.bucket(123.41) to_be doubleTable.hash(123.41) % 11);
      expect(doubleTable.bucket(0.001023) to_be doubleTable.hash(0.001023) % 11);
      expect(doubleTable.bucket(12312.31) to_be doubleTable.hash(12312.31) % 11);
      expect(doubleTable.bucket(30.20) to_be doubleTable.hash(30.20) % 11);
      expect(doubleTable.bucket(5) to_be doubleTable.hash(5) % 11);

      HashTable<char> charTable;
      expect(charTable.bucket('a') to_be charTable.hash('a') % 11);
      expect(charTable.bucket('b') to_be charTable.hash('b') % 11);
      expect(charTable.bucket('c') to_be charTable.hash('c') % 11);
      expect(charTable.bucket('d') to_be charTable.hash('d') % 11);
      expect(charTable.bucket('e') to_be charTable.hash('e') % 11);
      expect(charTable.bucket('2') to_be charTable.hash('2') % 11);
      expect(charTable.bucket('!') to_be charTable.hash('!') % 11);
      expect(charTable.bucket('?') to_be charTable.hash('?') % 11);
      expect(charTable.bucket('_') to_be charTable.hash('_') % 11);
      expect(charTable.bucket('[') to_be charTable.hash('[') % 11);
      expect(charTable.bucket('/') to_be charTable.hash('/') % 11);
      expect(charTable.bucket(' ') to_be charTable.hash(' ') % 11);

    }


    // insert - no rehash / load_factor
    {
        HashTable<int> intTable;
        for (int i = 0; i < 10; i++) {
            intTable.insert(i);
            expect(intTable.contains(i) to_be true);
            expect(intTable.load_factor() to_be static_cast<float>(intTable.size()) / intTable.bucket_count());
            expect(intTable.contains(i + 1) to_be false);
        }

        HashTable<double> doubleTable(23);
        for (double i = 0.0; i < 22.0; i++) {
            doubleTable.insert(i);
            expect(doubleTable.load_factor() to_be static_cast<float>(doubleTable.size()) / doubleTable.bucket_count());
            expect(doubleTable.contains(i) to_be true);
            expect(doubleTable.contains(i + 1) to_be false);
        }

        HashTable<char> charTable(26);
        for (char c = 'a'; c <= 'z'; c++) {
            charTable.insert(c);
            expect(charTable.load_factor() to_be static_cast<float>(charTable.size()) / charTable.bucket_count());
            expect(charTable.contains(c) to_be true);
            expect(charTable.contains(c + 1) to_be false);
        }
    }

    // rehash to 0
    {
      HashTable<int> intTable;

      intTable.insert(2);
      intTable.insert(6);
      intTable.insert(4);
      intTable.insert(1);
      intTable.insert(0);

      intTable.rehash(0);
      expect(intTable.bucket_count() to_be intTable.size() / intTable.max_load_factor());
      expect(intTable.size() to_be 5);
      expect(intTable.is_empty() to_be false);

    }

    // rehash other
    {
      HashTable<int> intTable;
      intTable.insert(0);
      intTable.insert(1);
      intTable.insert(9);
      // confirm starting conditions
      expect(intTable.size() to_be 3);
      expect(intTable.bucket_count() to_be 11);
      expect(intTable.load_factor() to_be static_cast<float>(intTable.size()) / intTable.bucket_count());
      expect(intTable.bucket_size(0) to_be 1);
      expect(intTable.bucket_size(1) to_be 1);
      expect(intTable.bucket_size(9) to_be 1);
      // rehash to size 7
      intTable.rehash(7);
      expect(intTable.size() to_be 3);
      expect(intTable.bucket_count() to_be 7);
      expect(intTable.load_factor() to_be static_cast<float>(intTable.size()) / intTable.bucket_count());
      expect(intTable.bucket_size(0) to_be 1);
      expect(intTable.bucket_size(1) to_be 1);
      expect(intTable.bucket_size(2) to_be 1);
      expect_throw(intTable.bucket_size(9), std::out_of_range);


      HashTable<double> doubleTable;
      doubleTable.insert(4.7);
      doubleTable.insert(1.9);
      doubleTable.insert(-6.2);
      // confirm starting conditions
      expect(doubleTable.size() to_be 3);
      expect(doubleTable.bucket_count() to_be 11);
      expect(doubleTable.load_factor() to_be static_cast<float>(doubleTable.size()) / doubleTable.bucket_count());
      expect(doubleTable.bucket_size(doubleTable.hash(4.7) % doubleTable.bucket_count()) to_be 1);
      expect(doubleTable.bucket_size(doubleTable.hash(1.9) % doubleTable.bucket_count()) to_be 1);
      expect(doubleTable.bucket_size(doubleTable.hash(-6.2) % doubleTable.bucket_count()) to_be 1);
      // rehash to size 7
      doubleTable.rehash(7);
      expect(doubleTable.size() to_be 3);
      expect(doubleTable.bucket_count() to_be 7);
      expect(doubleTable.load_factor() to_be static_cast<float>(doubleTable.size()) / doubleTable.bucket_count());
      expect(doubleTable.bucket_size(doubleTable.hash(4.7) % doubleTable.bucket_count()) to_be 2); // 4.7 and -6.2 now hash to the same place
      expect(doubleTable.bucket_size(doubleTable.hash(1.9) % doubleTable.bucket_count()) to_be 1);
      expect(doubleTable.bucket_size(doubleTable.hash(-6.2) % doubleTable.bucket_count()) to_be 2);


      HashTable<char> charTable;
      charTable.insert('a');
      charTable.insert('b');
      charTable.insert('n');
      // confirm starting conditions
      expect(charTable.size() to_be 3);
      expect(charTable.bucket_count() to_be 11);
      expect(charTable.load_factor() to_be static_cast<float>(charTable.size()) / charTable.bucket_count());
      expect(charTable.bucket_size(charTable.hash('a') % charTable.bucket_count()) to_be 1);
      expect(charTable.bucket_size(charTable.hash('b') % charTable.bucket_count()) to_be 1);
      expect(charTable.bucket_size(charTable.hash('n') % charTable.bucket_count()) to_be 1);
      // rehash to size 7
      charTable.rehash(7);
      expect(charTable.size() to_be 3);
      expect(charTable.bucket_count() to_be 7);
      expect(charTable.load_factor() to_be static_cast<float>(charTable.size()) / charTable.bucket_count());
      expect(charTable.bucket_size(charTable.hash('a') % charTable.bucket_count()) to_be 1);
      expect(charTable.bucket_size(charTable.hash('b') % charTable.bucket_count()) to_be 1);
      expect(charTable.bucket_size(charTable.hash('n') % charTable.bucket_count()) to_be 1);
    }

    // insert - rehash
    {
      HashTable<int> intTable(3);
      intTable.insert(0);
      intTable.insert(1);
      intTable.insert(2);
      // confirm starting conditions
      expect(intTable.size() to_be 3);
      expect(intTable.bucket_count() to_be 3);
      expect(intTable.load_factor() to_be static_cast<float>(intTable.size()) / intTable.bucket_count());
      expect(intTable.bucket_size(0) to_be 1);
      expect(intTable.bucket_size(1) to_be 1);
      expect(intTable.bucket_size(2) to_be 1);
      // expect next insert to cause rehash
      intTable.insert(3);
      expect(intTable.size() to_be 4);
      expect(intTable.bucket_count() to_be 7);
      expect(intTable.load_factor() to_be static_cast<float>(intTable.size()) / intTable.bucket_count());
      expect(intTable.bucket_size(0) to_be 1);
      expect(intTable.bucket_size(1) to_be 1);
      expect(intTable.bucket_size(2) to_be 1);
      expect(intTable.bucket_size(3) to_be 1);

    }


    // remove / load_factor
    {
      HashTable<int> intTable;
      for (int i = 0; i < 10; i++) {
          intTable.insert(i);
      }
      expect(intTable.size() to_be 10);
      expect(intTable.load_factor() to_be static_cast<float>(10)/11); 
      expect(intTable.contains(7) to_be true);
      intTable.remove(7);
      expect(intTable.size() to_be 9);
      expect(intTable.load_factor() to_be static_cast<float>(9)/11); 
      expect(intTable.contains(7) to_be false);
      expect(intTable.contains(3) to_be true);
      intTable.remove(3);
      expect(intTable.size() to_be 8);
      expect(intTable.load_factor() to_be static_cast<float>(8)/11); 
      expect(intTable.contains(3) to_be false);
      expect(intTable.contains(1) to_be true);
      expect(intTable.remove(1) == 1);
      expect(intTable.size() to_be 7);
      expect(intTable.load_factor() to_be static_cast<float>(7)/11); 
      expect(intTable.remove(50) == 0);
      expect(intTable.size() to_be 7);
      expect(intTable.load_factor() to_be static_cast<float>(7)/11); 

      HashTable<double> doubleTable(23);
      for (double i = 0.0; i < 22.0; i++) {
          doubleTable.insert(i);
      }
      expect(doubleTable.size() to_be 22);
      expect(doubleTable.load_factor() to_be static_cast<float>(22)/23); 
      expect(doubleTable.contains(7.0) to_be true);
      doubleTable.remove(7.0);
      expect(doubleTable.size() to_be 21);
      expect(doubleTable.load_factor() to_be static_cast<float>(21)/23); 
      expect(doubleTable.contains(7.0) to_be false);
      expect(doubleTable.contains(3.0) to_be true);
      doubleTable.remove(3.0);
      expect(doubleTable.size() to_be 20);
      expect(doubleTable.load_factor() to_be static_cast<float>(20)/23); 
      expect(doubleTable.contains(3.0) to_be false);
      expect(doubleTable.contains(1.0) to_be true);
      expect(doubleTable.remove(1.0) == 1);
      expect(doubleTable.size() to_be 19);
      expect(doubleTable.load_factor() to_be static_cast<float>(19)/23); 
      expect(doubleTable.remove(130.0) == 0);
      expect(doubleTable.size() to_be 19);
      expect(doubleTable.load_factor() to_be static_cast<float>(19)/23); 


      HashTable<char> charTable(26);
      for (char c = 'a'; c <= 'z'; c++) {
        charTable.insert(c);
      }
      expect(charTable.size() to_be 26);
      expect(charTable.load_factor() to_be static_cast<float>(26)/26); 
      expect(charTable.contains('a') to_be true);
      charTable.remove('a');
      expect(charTable.size() to_be 25);
      expect(charTable.load_factor() to_be static_cast<float>(25)/26); 
      expect(charTable.contains('a') to_be false);
      expect(charTable.contains('h') to_be true);
      charTable.remove('h');
      expect(charTable.size() to_be 24);
      expect(charTable.load_factor() to_be static_cast<float>(24)/26); 
      expect(charTable.contains('h') to_be false);
      expect(charTable.contains('q') to_be true);
      expect(charTable.remove('q') == 1);
      expect(charTable.size() to_be 23);
      expect(charTable.load_factor() to_be static_cast<float>(23)/26); 
      expect(charTable.remove('9') == 0);
      expect(charTable.size() to_be 23);
      expect(charTable.load_factor() to_be static_cast<float>(23)/26); 
    }

    // print table
    {
      HashTable<int> intTable;
      std::string empty = "<empty>\n";
      std::stringstream emptyss;
      intTable.print_table(emptyss);
      expect(emptyss.str() to_be empty);

      intTable.insert(2);
      intTable.insert(3);
      std::stringstream ss;
      intTable.print_table(ss);
      std::string expected = "2: [2]\n3: [3]\n";
      expect(ss.str() to_be expected);
    }

    // all the above kind of test but commented out to hide output
    // {
    //   std::cout << "make an empty hash table with 11 buckets for strings" << std::endl;
    //   HashTable<std::string> table(11);

    //   std::cout << "initial size is " << table.size() << std::endl;
    //   std::cout << "initial bucket count is " << table.bucket_count() << std::endl;
    //   std::cout << "initial load factor is " << table.load_factor() << std::endl;
    //   std::cout << "initial max load factor is " << table.max_load_factor() << std::endl;

    //   std::cout << "insert several strings" << std::endl;   
    //   table.insert("And them who hold High Places");
    //   table.insert("Must be the ones to start");
    //   table.insert("To mold a new Reality");
    //   table.insert("Closer to the Heart");
    //   table.insert("The Blacksmith and the Artist");
    //   table.insert("Reflect it in their Art");
    //   table.insert("Forge their Creativity");
    //   table.insert("Closer to the Heart");
    //   table.insert("Philosophers and Plowmen");
    //   table.insert("Each must know their Part");
    //   table.insert("To sow a new Mentality");
    //   table.insert("Closer to the Heart");
    //   table.insert("You can be the Captain");
    //   table.insert("I will draw the Chart");
    //   table.insert("Sailing into Destiny");
    //   table.insert("Closer to the Heart");

    //   std::cout << "size is " << table.size() << std::endl;
    //   std::cout << "bucket count is " << table.bucket_count() << std::endl;
    //   std::cout << "load factor is " << table.load_factor() << std::endl;
    //   std::cout << "max load factor is " << table.max_load_factor() << std::endl;

    //   {
    //       std::cout << "print the table" << std::endl;
    //       std::stringstream ss;
    //       table.print_table(ss);
    //       std::cout << ss.str() << std::endl;
    //   }

    //   std::cout << "remove \"Philosophers and Plowmen\"" << std::endl;
    //   table.remove("Philosophers and Plowmen");
    //   std::cout << "remove \"Each must know their Part\"" << std::endl;
    //   table.remove("Each must know their Part");

    //   std::cout << "size is " << table.size() << std::endl;
    //   std::cout << "bucket count is " << table.bucket_count() << std::endl;
    //   std::cout << "load factor is " << table.load_factor() << std::endl;
    //   std::cout << "max load factor is " << table.max_load_factor() << std::endl;

    //   {
    //       std::cout << "print the table" << std::endl;
    //       std::stringstream ss;
    //       table.print_table(ss);
    //       std::cout << ss.str() << std::endl;
    //   }

    //   std::cout << "set max load factor to 2" << std::endl;
    //   table.max_load_factor(2);
    //   std::cout << "rehash the table to size 7" << std::endl;
    //   table.rehash(7);

    //   std::cout << "size is " << table.size() << std::endl;
    //   std::cout << "bucket count is " << table.bucket_count() << std::endl;
    //   std::cout << "load factor is " << table.load_factor() << std::endl;
    //   std::cout << "max load factor is " << table.max_load_factor() << std::endl;

    //   {
    //       std::cout << "print the table" << std::endl;
    //       std::stringstream ss;
    //       table.print_table(ss);
    //       std::cout << ss.str() << std::endl;
    //   }

    //   std::cout << "find \"The Blacksmith and the Artist\"" << std::endl;
    //   size_t index = table.bucket("The Blacksmith and the Artist");
    //   std::cout << " ==> bucket " << index << std::endl;
    //   std::cout << "     which has " << table.bucket_size(index) << " elements" << std::endl;

    //   std::cout << "make the table empty" << std::endl;
    //   table.make_empty();

    //   std::cout << "size is " << table.size() << std::endl;
    //   std::cout << "bucket count is " << table.bucket_count() << std::endl;
    //   std::cout << "load factor is " << table.load_factor() << std::endl;
    //   std::cout << "max load factor is " << table.max_load_factor() << std::endl;

    //   {
    //       std::cout << "print the table" << std::endl;
    //       std::stringstream ss;
    //       table.print_table(ss);
    //       std::cout << ss.str() << std::endl;
    //   }

    // }


    return 0;
}
