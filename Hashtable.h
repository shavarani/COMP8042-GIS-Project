//
// Created by Hassan S. Shavarani
// Starter code for lab 05 of COMP 8042
//

#ifndef FINAL_PROJECT_HASHTABLE_H
#define FINAL_PROJECT_HASHTABLE_H

#include <string>
#include <vector>
#include <sstream>

using namespace std;

template <typename K>
class HashFunction {
    public:
        virtual unsigned long operator()(const K& key) const = 0;
        virtual ~HashFunction() = default;
};

class SimpleStringHash : public HashFunction<string> {
    public:
        // Computes a simple hash value for a string
        unsigned long operator()(const string& s) const override {
            unsigned int hash = 0;
            for (char i : s) {
                hash += i;
            }
            return hash;
        }
};

class ElfHash : public HashFunction<string> {
    public:
        unsigned long operator()(const string& str) const override {
            const char *s = str.c_str();
            unsigned long h = 0, high;
            while (*s) {
                h = ( h << 4 ) + *s++;
                if (high = h & 0xF0000000)
                    h ^= high >> 24;
                h &= ~high;
            }
            return h;
        }
};

/// An abstract collision resolution function
class ResolutionFunction {
    public:
        virtual unsigned int operator()(int i) const = 0;
        virtual ~ResolutionFunction() = default;
};

/// The instructor did the hard part for you and implemented the two resolution functions. Phew!
class LinearProbing : public ResolutionFunction {
    public:
        unsigned int operator()(int i) const override {
            return i;
        }
};

class QuadraticProbing : public ResolutionFunction {
    public:
        unsigned int operator()(int i) const override {
            return i * i;
        }
};

/// Returns the bit index of the most significant bit.
/// If the input is zero, it returns zero
/// This function will help you locate the crrect good prime in the array below
/// It will also help you compute the next power of two
/*int mostSignificantBit(int x) {
    if (x == 0)
        return 0;
    int mostSignificantBit = 31;
    while ((x & (1 << mostSignificantBit)) == 0)
        mostSignificantBit--;
    return mostSignificantBit;
}*/


template <typename K, class V>
class Hashtable{
    private:
        enum BucketStatus {EMPTY, OCCUPIED, DELETED};
        int numCollisions;
        std::vector<K> m_buckets;
        std::vector<V> m_values;
        std::vector<BucketStatus> m_statuses;
        HashFunction<K>* hash;
        ResolutionFunction* f;
        int longestProbe = 0;
    public:
        Hashtable(int n, HashFunction<K>* _h, ResolutionFunction* _f) : numCollisions(0), hash(_h), f(_f) {
            int c = n * 2;
            m_buckets.resize(c);
            m_statuses.resize(c);
            m_values.resize(c);
        }

        bool insert(const K& key, const V& value) {
            int tableSize = m_buckets.size();
            unsigned int h = hash->operator()(key) % tableSize;
            unsigned int firstH = h;
            int lp = 0;
            while (m_statuses[h] == OCCUPIED){
                ++numCollisions;
                h = (h + hash->operator()(key)) % tableSize;
                if (h == firstH)
                    return false;
                lp ++;
            }
            if (lp > longestProbe)
                longestProbe = lp;
            m_buckets[h] = key;
            m_statuses[h] = OCCUPIED;
            m_values[h] = value;
            return true;
        }

        V* search(const K& key) {
            int tableSize = m_buckets.size();
            unsigned int h = hash->operator()(key) % tableSize;
            unsigned int firstH = h;
            while (m_statuses[h] != EMPTY){
                if (m_statuses[h] == OCCUPIED && m_buckets[h] == key)
                    return &m_values[h];
                h = (h + hash->operator()(key)) % tableSize;
                if (h == firstH)
                    return nullptr;
            }
            return nullptr;
        }

        bool erase(const K& key) {
            int tableSize = m_buckets.size();
            unsigned int h = hash->operator()(key) % tableSize;
            unsigned int firstH = h;
            while (m_statuses[h] != EMPTY){
                if (m_buckets[h] == key){
                    m_statuses[h] = DELETED;
                    return true;
                }
                h = (h + hash->operator()(key)) % tableSize;
                if (h == firstH)
                    return false;
            }
            return false;
        }

        int getLongestProbe() const{
            return longestProbe;
        }

        int getNumCollisions() const {
            return numCollisions;
        }

        std::vector<K> getKeys() {
            return m_buckets;
        };

        std::string str() {
            std::ostringstream os;
            for (int i = 0; i < m_statuses.size(); ++i)
                if(m_statuses[i] == OCCUPIED)
                    os << "\t" << i << ": " << m_values[i].str() << std::endl;
            return os.str();
        }
};

/*
int stressTest(int hashtableSize) {
    // Load words from file
    vector<string> words = loadWords("1kwords.txt");
    // Generate random phrases
    vector<string> phrases;
    generateRandomPhrases(words, hashtableSize, phrases);

    // Create the hashtable
    HashFunction<string>* hash = new SimpleStringHash();
    //ResolutionFunction* f = new LinearProbing();
    ResolutionFunction* f = new QuadraticProbing();
    Hashtable<string> hashtable = Hashtable<string>(hashtableSize, hash, f);


    // Insert all phrases into the hashtable
    for (string phrase : phrases)
        hashtable.insert(phrase);
    // Search for all phrases
    for (string phrase : phrases)
        hashtable.search(phrase);

    delete hash;
    delete f;
    // return number of collisions
    return hashtable.getNumCollisions();
}
*/
#endif //FINAL_PROJECT_HASHTABLE_H
