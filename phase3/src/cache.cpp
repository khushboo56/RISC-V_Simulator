#include "builtin_funcs.hpp"
#include "self_defined_funcs.hpp"
#include "global_variables.hpp"
#ifndef MYCLASSES
#include "cache.hpp"
#endif
void Cache::setCache(int cache_size,int block_size,int ways,int block_replacement_policy,int hit_time,int miss_time){
    this->cache_size=cache_size;
    this->cache_block_size=block_size;
    this->mapping_choice=block_replacement_policy;
    this->ways=ways;
    this->hit_time=hit_time;
    this->miss_time=miss_time;
    this->index_bits=((int)log2(cache_size/cache_block_size)) - ( (int) log2(ways) );;
    this->block_offset_bits=(int)log2(cache_block_size);
    this->tag_bits=32 - this->index_bits - this->block_offset_bits;
    if(block_replacement_policy==0){
        this->ways=1;
    }
    //createCache()
}
void Cache::setCache(int cache_size,int block_size,int ways,int block_replacement_policy){
    this->cache_size=cache_size;
    this->cache_block_size=block_size;
    this->mapping_choice=block_replacement_policy;
    this->ways=ways;
    this->hit_time=1;
    this->miss_time=20;
    this->index_bits=((int)log2(cache_size/cache_block_size)) - ( (int) log2(ways) );;
    this->block_offset_bits=(int)log2(cache_block_size);
    this->tag_bits=32 - this->index_bits - this->block_offset_bits;
    if(block_replacement_policy==0){
        this->ways=1;
    }
    //createCache()
}
void Cache::createCache(){
    mycache=(struct cache_entry *)malloc(pow(2,index_bits)*ways*sizeof(struct cache_entry));
    char * temp=(char *)malloc(pow(2,index_bits)*ways*cache_block_size*sizeof(char));
    for(int i=0;i<pow(2,index_bits)*ways;i++){
        mycache[i].block=temp+(i*cache_block_size);
        mycache[i].dirty=0;
        mycache[i].valid=0;
        mycache[i].tag=0;
    }
    
}
void Cache::write_cache_to_main(unsigned int address,char * block){
    if(address%cache_block_size!=0){
        cout<<"*** some error while running write_cache_to_main ***"<<endl;
    }
    for(int i=0;i<cache_block_size;i++){
        mem[address+i]=block[i];
    }

}
void Cache::read_cache_from_main(unsigned int address,int index){
    address=address-(address%cache_block_size);
    string str_add=dec2bin(address);
    string s_tag=str_add.substr(0,tag_bits);
    unsigned long long int tag=unsgn_binaryToDecimal(s_tag);
    mycache[index].tag=tag;
    mycache[index].valid=1;
    mycache[index].dirty=0;
    for(int i=0;i<cache_block_size;i++){
        mycache[index].block[i]=mem[address+i];
    }

}
unsigned long long int Cache::readCache(unsigned int address, int bytes){
    string str_add=dec2bin(address);
    string s_tag=str_add.substr(0,tag_bits);
    string s_index=str_add.substr(tag_bits,index_bits);
    string s_BO=str_add.substr(tag_bits+index_bits,block_offset_bits);
    // cout<<"str_add"<<str_add<<endl;
    // cout<<"tag_bits"<<tag_bits<<endl;
    // cout<<"index_bits"<<index_bits<<endl;
    // cout<<"block_offset_bits"<<block_offset_bits<<endl;
    // cout<<"s_tag:"<<s_tag<<endl;
    // cout<<"s_index:"<<s_index<<endl;
    // cout<<"s_BO"<<s_BO<<endl;
    unsigned long long int tag=unsgn_binaryToDecimal(s_tag);
    unsigned long long int index=unsgn_binaryToDecimal(s_index);
    unsigned long long int BO=unsgn_binaryToDecimal(s_BO);
    // printf("block_offset:%llu\n",BO);
    if(mapping_choice==0){
        if(mycache[index].valid==1){
            if(mycache[index].tag==tag){
                //hit
                // cout<<"**"<<endl;

                unsigned long long int temp_value = 0;
                for (int i = 0; i < bytes; i++)
                {
                    temp_value = (((unsigned long long int)((unsigned char)(mycache[index].block)[BO+i])) << (i * 8)) | (temp_value);
                }
                return temp_value;
            }
            else{
                //miss
                if(mycache[index].dirty==1){
                    unsigned int temp_address= (((unsigned int)mycache[index].tag)<<(index_bits+block_offset_bits))+(((unsigned int)index)<<(block_offset_bits));
                    write_cache_to_main(temp_address,mycache[index].block);
                }
                // read_cache_to_main()
                read_cache_from_main(address,index);
                unsigned long long int temp_value = 0;
                for (int i = 0; i < bytes; i++)
                {
                    temp_value = (((unsigned long long int)((unsigned char)(mycache[index].block)[BO+i])) << (i * 8)) | (temp_value);
                }
                return temp_value;



            }
        }
        else{
            //miss
            read_cache_from_main(address,index);
            unsigned long long int temp_value = 0;
            for (int i = 0; i < bytes; i++)
            {
                temp_value = (((unsigned long long int)((unsigned char)(mycache[index].block)[BO+i])) << (i * 8)) | (temp_value);
            }
            return temp_value;
        }


    }
    else{
        return 0;
    }
    
}
void Cache::writeCache(unsigned int address, unsigned long long int value, int bytes){
    string str_add=dec2bin(address);
    string s_tag=str_add.substr(0,tag_bits);
    string s_index=str_add.substr(tag_bits,index_bits);
    string s_BO=str_add.substr(tag_bits+index_bits,block_offset_bits);
    unsigned long long int tag=unsgn_binaryToDecimal(s_tag);
    unsigned long long int index=unsgn_binaryToDecimal(s_index);
    unsigned long long int BO=unsgn_binaryToDecimal(s_BO);
    if(mapping_choice==0){
        if(mycache[index].valid==1){
            if(mycache[index].tag==tag){
                //hit
                mycache[index].dirty=1;
                for (int i = 0; i < bytes; i++)
                {
                    mycache[index].block[BO+i]= (char)((value >> (i * 8)) & 0xFF);
                }
            }
            else{
                //miss
                if(mycache[index].dirty==1){
                    unsigned int temp_address= (((unsigned int)mycache[index].tag)<<(index_bits+block_offset_bits))+(((unsigned int)index)<<(block_offset_bits));
                    write_cache_to_main(temp_address,mycache[index].block);
                }
                // read_cache_to_main()
                read_cache_from_main(address,index);
                mycache[index].dirty=1;
                for (int i = 0; i < bytes; i++)
                {
                    mycache[index].block[BO+i]= (char)((value >> (i * 8)) & 0xFF);
                }
            }

        }
        else{
            //miss
            read_cache_from_main(address,index);
            mycache[index].dirty=1;
            for (int i = 0; i < bytes; i++)
            {
                mycache[index].block[BO+i]= (char)((value >> (i * 8)) & 0xFF);
            }
        }

    }

}