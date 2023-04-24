#include "builtin_funcs.hpp"
#include "global_variables.hpp"
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
    int ways;
    int hit_time;
    int miss_time;
    int index_bits;
    int block_offset_bits;
    int tag_bits;
    struct cache_entry * mycache;
    void createCache();
    unsigned long long int readCache(unsigned int address, int bytes);
    Cache(int cache_size,int block_size,int ways,int block_replacement_policy,int hit_time,int miss_time);
    Cache(int cache_size,int block_size,int ways,int block_replacement_policy);
    void write_cache_to_main(unsigned int address,char * block,int block_size);
};