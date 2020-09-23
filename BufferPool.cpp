//
// LRUCache taken from Taken from https://www.geeksforgeeks.org/lru-cache-implementation/
//

#include "BufferPool.h"


std::string BufferPool::str(){
    std::ostringstream os;
    os<< "MRU" << endl;
    for (int & it : dq)
        os << "  "<< it << ": " << pool.find(it)->second.get_raw_record() << endl;
    os<< "LRU" << endl;
    return os.str();
}

void BufferPool::create_db_file(const char* db_file_adr) {
    db_file.create_file_to_write(db_file_adr);
}

FileManager* BufferPool::get_db_file(){
    return &db_file;
}

vector<GISRecord> BufferPool::retrieve_records(const std::set<int>& record_offsets){
    vector<GISRecord> result;
    if(record_offsets.empty())
        return result;
    std::set<int> fetch_offsets;
    for(auto &offset: record_offsets){
        if(exists(offset)){
            refer(offset);
            result.emplace_back(pool.find(offset)->second);
        }
        else
            fetch_offsets.emplace(offset);
    }
    if(!fetch_offsets.empty())
        for (auto& elem: db_file.retrieve_records(fetch_offsets)) {
            GISRecord e(elem.first);
            e.set_file_offset(elem.second);
            pool.insert(std::pair<int,GISRecord>(elem.second,e));
            refer(elem.second);
            result.insert(result.end(), e);
        }
    return result;
}

// Refers key x with in the LRU cache
void BufferPool::refer(int x) {
    if (ma.find(x) == ma.end()) { // not present in cache
        if (dq.size() == csize) { // cache is full
            // delete least recently used element
            int last = dq.back();
            // Pops the last element
            dq.pop_back();
            // Erase the last
            ma.erase(last);
            pool.erase(pool.find (last));
        }
    } else // present in cache
        dq.erase(ma[x]);
    // update reference
    dq.push_front(x);
    ma[x] = dq.begin();
}

bool BufferPool::exists(int offset){
    return pool.find(offset) != pool.end();
}