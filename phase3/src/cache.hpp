#include "builtin_funcs.hpp"
struct cache_entry{
    int valid;
    int dirty;
    int tag;
    char * block;
};

class Cache{
    public:
    int cache_size;
    int cache_block_size;
    int mapping_choice;
    int replacementpolicy;
    int ways;
    int hit_time;
    int miss_time;
    int index_bits;
    int block_offset_bits;
    int tag_bits;
    int n_hits, n_miss,n_cold,n_capacity,n_conflict;
    vector<int> LRU;
    vector<int> LFU;
    vector<int> FIFO;
    struct cache_entry * mycache;
    void createCache();
    unsigned long long int readCache(unsigned int address, int bytes);
    void setCache(int cache_size,int block_size,int ways,int block_replacement_policy,int mapping_ch,int hit_time,int miss_time);
    void setCache(int cache_size,int block_size,int ways,int block_replacement_policy,int mapping_ch);
    void write_cache_to_main(unsigned int address,char * block);
    void read_cache_from_main(unsigned int address,int index);
    void writeCache(unsigned int address, unsigned long long int value, int bytes);
    long long int findLRUind(unsigned int address, int bytes);
    long long int findLFUind(unsigned int address, int bytes);
    long long int findFIFOind(unsigned int address, int bytes);
    long long int findRandomind(unsigned int address, int bytes);
};