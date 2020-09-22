//
// Created by Hassan S. Shavarani
//

#include "Hashtable.h"
/*
template <typename K, class V>
Hashtable<K,V>::Hashtable(int n, HashFunction<K>* _h, ResolutionFunction* _f) : numCollisions(0), hash(_h), f(_f) {
    int c = n * 2;
    m_buckets.resize(c);
    m_statuses.resize(c);
    m_values.resize(c);
}

template <typename K, class V>
bool Hashtable<K,V>::insert(const K& key, const V& value) {
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

template <typename K, class V>
V* Hashtable<K,V>::search(const K& key) {
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

template <typename K, class V>
bool Hashtable<K,V>::erase(const K& key) {
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

template <typename K, class V>
int Hashtable<K,V>::getLongestProbe() const{
    return longestProbe;
}

template <typename K, class V>
int Hashtable<K,V>::getNumCollisions() const {
    return numCollisions;
}

template <typename K, class V>
std::vector<K> Hashtable<K,V>::getKeys() {
    return m_buckets;
};
template <typename K, class V>
std::string Hashtable<K,V>::str() {
    std::ostringstream os;
    for (int i = 0; i < m_statuses.size(); ++i)
        if(m_statuses[i] == OCCUPIED)
            os << "\t" << i << ": " << m_values[i].str() << std::endl;
    return os.str();
}

*/
