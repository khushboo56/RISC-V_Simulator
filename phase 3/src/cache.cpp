#include "cache.hpp"
#include "builtin_funcs.hpp"
#include "self_defined_funcs.hpp"
Cache::Cache(int cache_size,int block_size,int ways,int block_replacement_policy,int hit_time,int miss_time){
    this->cache_size=cache_size;
    this->cache_block_size=block_size;
    this->mapping_choice=block_replacement_policy;
    this->ways=ways;
    this->hit_time=hit_time;
    this->miss_time=miss_time;
    this->index_bits=((int)log2(cache_size/cache_block_size)) - ( (int) log2(ways) );;
    int block_offset_bits=(int)log2(cache_block_size);
    int tag_bits=32 - this->index - this->block_offset_bits;
}
Cache::Cache(int cache_size,int block_size,int ways,int block_replacement_policy){
    this->cache_size=cache_size;
    this->cache_block_size=block_size;
    this->mapping_choice=block_replacement_policy;
    this->ways=ways;
    this->hit_time=1;
    this->miss_time=20;
    this->index_bits=((int)log2(cache_size/cache_block_size)) - ( (int) log2(ways) );;
    int block_offset_bits=(int)log2(cache_block_size);
    int tag_bits=32 - this->index - this->block_offset_bits;
}
void Cache::createCache(){
    mycache=(struct cache_entry *)malloc(pow(2,index_bits)*sizeof(struct cache_entry));
    char * temp=(char *)malloc(pow(2,index_bits)*cache_block_size*sizeof(char));
    for(int i=0;i<pow(2,index_bits);i++){
        mycache[i].block=temp+(i*cache_block_size);
    }
}

unsigned long long int Cache::readCache(unsigned int address, int bytes){
    str_add=dec2bin(address);
    
}