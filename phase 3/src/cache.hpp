#include "builtin_funcs.hpp"
struct cache_entry{
    int tag;
    char * block;
};

class Cache{
    public:
    int cache_size;
    int cache_block_size;
    int mapping_choice;
    int ways;
    int hit_time;
    int miss_time;
    int index_bits;
    int block_offset;
    struct cache_entry * mycache;
    void createCache();

};