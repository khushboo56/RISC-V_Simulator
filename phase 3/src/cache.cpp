#include "cache.hpp"
#include "builtin_funcs.hpp"
void Cache::createCache(){
    mycache=(struct cache_entry *)malloc(pow(2,index_bits)*sizeof(struct cache_entry));
    char * temp=(char *)malloc(pow(2,index_bits)*cache_block_size*sizeof(char));
    for(int i=0;i<pow(2,index_bits);i++){
        mycache[i].block=temp+(i*cache_block_size);
    }
}