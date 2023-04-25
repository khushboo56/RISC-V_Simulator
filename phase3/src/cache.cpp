#include "builtin_funcs.hpp"
#include "self_defined_funcs.hpp"
#include "global_variables.hpp"
#ifndef MYCLASSES
#include "cache.hpp"
#endif
void Cache::setCache(int cache_size,int block_size,int ways,int block_replacement_policy,int mapping_ch,int hit_time,int miss_time){
    this->cache_size=cache_size;
    this->cache_block_size=block_size;
    this->mapping_choice=mapping_ch;
    this->replacementpolicy=block_replacement_policy;
    this->ways=ways;
    this->hit_time=hit_time;
    this->miss_time=miss_time;
    this->index_bits=((int)log2(cache_size/cache_block_size)) - ( (int) log2(ways) );;
    this->block_offset_bits=(int)log2(cache_block_size);
    this->tag_bits=32 - this->index_bits - this->block_offset_bits;
    if(this->mapping_choice==0){
        this->ways=1;
    }
    if(this->mapping_choice==2){
        this->ways=(cache_size/cache_block_size);
    }
    //createCache()
}
void Cache::setCache(int cache_size,int block_size,int ways,int block_replacement_policy,int mapping_ch){
    this->cache_size=cache_size;
    this->cache_block_size=block_size;
    this->mapping_choice=mapping_ch;
    this->replacementpolicy=block_replacement_policy;
    this->ways=ways;
    this->hit_time=1;
    this->miss_time=20;
    this->index_bits=((int)log2(cache_size/cache_block_size)) - ( (int) log2(ways) );;
    this->block_offset_bits=(int)log2(cache_block_size);
    this->tag_bits=32 - this->index_bits - this->block_offset_bits;
    if(this->mapping_choice==0){
        this->ways=1;
    }
    if(this->mapping_choice==2){
        this->ways=(cache_size/cache_block_size);
    }
}
void Cache::createCache(){
    mycache=(struct cache_entry *)malloc(pow(2,index_bits)*ways*sizeof(struct cache_entry));
    char * temp=(char *)malloc(pow(2,index_bits)*ways*cache_block_size*sizeof(char));
    if(replacementpolicy==0){
        this->LRU.clear();
        this->LRU.resize(pow(2,index_bits)*ways,0);
    }
    if(replacementpolicy==1){
        this->LFU.clear();
        this->LFU.resize(pow(2,index_bits)*ways,0);
    }
    if(replacementpolicy==2){
        this->FIFO.clear();
        this->FIFO.resize(pow(2,index_bits)*ways,0);
    }
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
long long int Cache::findLRUind(unsigned int address, int bytes){
    string str_add=dec2bin(address);
    string s_tag=str_add.substr(0,tag_bits);
    string s_index=str_add.substr(tag_bits,index_bits);
    string s_BO=str_add.substr(tag_bits+index_bits,block_offset_bits);
    unsigned long long int tag=unsgn_binaryToDecimal(s_tag);
    unsigned long long int index=unsgn_binaryToDecimal(s_index);
    unsigned long long int BO=unsgn_binaryToDecimal(s_BO);
    
    index=index*ways;
    for(int k=index;k<index+ways;k++){
        if( (mycache[k].valid==1) && (mycache[k].tag)==tag){
            //hit
            return k;
        }
    }
    //miss
    for(int k=index;k<index+ways;k++){
        if(mycache[k].valid==0){
            return k;  
        }
    }
    for(int k=index;k<index+ways;k++){
        if(LRU[k]==0){
            return k;  
        }
    }
    return -1;
}

long long int Cache:: findLFUind(unsigned int address, int bytes)
{
    string str_add=dec2bin(address);
    string s_tag=str_add.substr(0,tag_bits);
    string s_index=str_add.substr(tag_bits,index_bits);
    string s_BO=str_add.substr(tag_bits+index_bits,block_offset_bits);
    unsigned long long int tag=unsgn_binaryToDecimal(s_tag);
    unsigned long long int index=unsgn_binaryToDecimal(s_index);
    unsigned long long int BO=unsgn_binaryToDecimal(s_BO);
    index=index*ways;
    for(int k=index;k<index+ways;k++)
    {
        if( (mycache[k].valid==1) && (mycache[k].tag)==tag){
            //hit
            return k;
        }
    }
    //miss
    for(int k=index;k<index+ways;k++){
        if(mycache[k].valid==0){
            return k;  
        }
    }
    int mini=LFU[index];
    int ind=-1;
    for(int k=index;k<index+ways;k++){
        //if we want to replace
        mini=min(mini,LFU[k]);
        if(mini==LFU[k])
        {
            ind=k;
        }
    }
    return ind;
}
long long int Cache::findFIFOind(unsigned int address, int bytes){
    string str_add=dec2bin(address);
    string s_tag=str_add.substr(0,tag_bits);
    string s_index=str_add.substr(tag_bits,index_bits);
    string s_BO=str_add.substr(tag_bits+index_bits,block_offset_bits);
    unsigned long long int tag=unsgn_binaryToDecimal(s_tag);
    unsigned long long int index=unsgn_binaryToDecimal(s_index);
    unsigned long long int BO=unsgn_binaryToDecimal(s_BO);
    
    index=index*ways;
    for(int k=index;k<index+ways;k++){
        if( (mycache[k].valid==1) && (mycache[k].tag)==tag){
            //hit
            return k;
        }
    }
    //miss
    for(int k=index;k<index+ways;k++){
        if(mycache[k].valid==0){
            return k;  
        }
    }
    for(int k=index;k<index+ways;k++){
        if(FIFO[k]==0){
            return k;  
        }
    }
    cout<<"##fatal error##"<<endl;
    return -1;

}
long long int Cache::findRandomind(unsigned int address, int bytes){
    string str_add=dec2bin(address);
    string s_tag=str_add.substr(0,tag_bits);
    string s_index=str_add.substr(tag_bits,index_bits);
    string s_BO=str_add.substr(tag_bits+index_bits,block_offset_bits);
    unsigned long long int tag=unsgn_binaryToDecimal(s_tag);
    unsigned long long int index=unsgn_binaryToDecimal(s_index);
    unsigned long long int BO=unsgn_binaryToDecimal(s_BO);
    
    index=index*ways;
    for(int k=index;k<index+ways;k++){
        if( (mycache[k].valid==1) && (mycache[k].tag)==tag){
            //hit
            return k;
        }
    }
    //miss
    for(int k=index;k<index+ways;k++){
        if(mycache[k].valid==0){
            return k;  
        }
    }
    return (index+(rand()%ways));

}
unsigned long long int Cache::readCache(unsigned int address, int bytes){
    string str_add=dec2bin(address);
    string s_tag=str_add.substr(0,tag_bits);
    string s_index=str_add.substr(tag_bits,index_bits);
    string s_BO=str_add.substr(tag_bits+index_bits,block_offset_bits);
    unsigned long long int tag=unsgn_binaryToDecimal(s_tag);
    unsigned long long int index=unsgn_binaryToDecimal(s_index);
    unsigned long long int BO=unsgn_binaryToDecimal(s_BO);
    // printf("block_offset:%llu\n",BO);
    if(mapping_choice==0){ //direct map
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
    else if(mapping_choice==1||mapping_choice==2){ // set associative
        int if_hit=0;
        long long int myind;
        int flag=1;
        if(replacementpolicy==0)
        {
            //LRU
            myind=findLRUind(address,bytes);
            if(myind<0){
                cout<<"some Error in LRU ~##"<<endl;
                flag=0;
            }
        }
        else if(replacementpolicy==1)
        {
            //LFU
            myind=findLFUind(address,bytes);
            if(myind<0){
                cout<<"some Error in LFU ~##"<<endl;
                flag=0;
            }
        }
        else if(replacementpolicy==2)
        {
            //FIFO
            myind=findFIFOind(address,bytes);
            if(myind<0){
                cout<<"some Error in LFU ~##"<<endl;
                flag=0;
            }
        }
        else if(replacementpolicy==3){
            myind=findRandomind(address,bytes);
            if(myind<0){
                cout<<"some Error in LFU ~##"<<endl;
                flag=0;
            }
        }
        else {
            printf("****No replacemennt policy taken****");
        }
        unsigned long long int temp_value = 0;
        if(flag==0)
        {
            printf("error occured");
        }
        else if(mycache[myind].valid==1){
            if(mycache[myind].tag==tag){
                //hit
                if_hit=1;
                for (int i = 0; i < bytes; i++)
                {
                    temp_value = (((unsigned long long int)((unsigned char)(mycache[myind].block)[BO+i])) << (i * 8)) | (temp_value);
                }
                
            }
            else{
                //miss
                if(mycache[myind].dirty==1){
                    unsigned int temp_address= (((unsigned int)mycache[myind].tag)<<(index_bits+block_offset_bits))+(((unsigned int)index)<<(block_offset_bits));
                    write_cache_to_main(temp_address,mycache[myind].block);
                }
                // read_cache_to_main()
                read_cache_from_main(address,myind);
                for (int i = 0; i < bytes; i++)
                {
                    temp_value = (((unsigned long long int)((unsigned char)(mycache[myind].block)[BO+i])) << (i * 8)) | (temp_value);
                }

            }
        }
        else{
            read_cache_from_main(address,myind);
            for (int i = 0; i < bytes; i++)
            {
                temp_value = (((unsigned long long int)((unsigned char)(mycache[myind].block)[BO+i])) << (i * 8)) | (temp_value);
            }
        }
        if(replacementpolicy==0)
        {
            //update LRU
            for(int k=index*ways;k<(index+1)*ways;k++){
                if(LRU[k]>LRU[myind]){
                    LRU[k]--;
                }
            }
            LRU[myind]=ways-1;
        }
        else if(replacementpolicy==1)
        {
            //update LFU
            LFU[myind]++;            
        }
        else if(replacementpolicy==2 )
        {
            //FIFO
            if(!if_hit){
                for(int k=index*ways;k<(index+1)*ways;k++){
                    if(FIFO[k]>FIFO[myind]){
                        FIFO[k]--;
                    }
                }
                FIFO[myind]=ways-1;
            }
        }
        else if(replacementpolicy==3){
            //nothing to update
            ;
        }
        else {
            printf("****No replacemennt policy taken****");
        }
        return temp_value;
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
    if(mapping_choice==0){ //direct map
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
    else if(mapping_choice==1||mapping_choice==2){ //set associative
        int if_hit=0;
        long long int myind;
        int flag=1;
        if(replacementpolicy==0)
        {
            //LRU
            myind=findLRUind(address,bytes);
            if(myind<0){
                cout<<"some Error in LRU ~##"<<endl;
                flag=0;
            }
        }
        else if(replacementpolicy==1)
        {
            //LFU
            myind=findLFUind(address,bytes);
            if(myind<0){
                cout<<"some Error in LFU ~##"<<endl;
                flag=0;
            }
        }
        else if(replacementpolicy==2)
        {
            //FIFO
            myind=findFIFOind(address,bytes);
            if(myind<0){
                cout<<"some Error in LFU ~##"<<endl;
                flag=0;
            }
        }
        else if(replacementpolicy==3){
            myind=findRandomind(address,bytes);
            if(myind<0){
                cout<<"some Error in LFU ~##"<<endl;
                flag=0;
            }
        }
        else {
            printf("****No replacemennt policy taken****");
        }
        unsigned long long int temp_value = 0;
        if(flag==0)
        {
            printf("error occured");
        }
        else if(mycache[myind].valid==1){
            if(mycache[myind].tag==tag){
                //hit
                if_hit=1;
                mycache[myind].dirty=1;
                for (int i = 0; i < bytes; i++)
                {
                    mycache[myind].block[BO+i]= (char)((value >> (i * 8)) & 0xFF);
                }
                
            }
            else{
                //miss
                if(mycache[myind].dirty==1){
                    unsigned int temp_address= (((unsigned int)mycache[myind].tag)<<(index_bits+block_offset_bits))+(((unsigned int)index)<<(block_offset_bits));
                    write_cache_to_main(temp_address,mycache[myind].block);
                }
                // read_cache_to_main()
                read_cache_from_main(address,myind);
                mycache[myind].dirty=1;
                for (int i = 0; i < bytes; i++)
                {
                    mycache[myind].block[BO+i]= (char)((value >> (i * 8)) & 0xFF);
                }

            }
        }
        else{
            read_cache_from_main(address,myind);
            mycache[myind].dirty=1;
            for (int i = 0; i < bytes; i++)
            {
                mycache[myind].block[BO+i]= (char)((value >> (i * 8)) & 0xFF);
            }
        }
        if(replacementpolicy==0)
        {
            //update LRU
            for(int k=index*ways;k<(index+1)*ways;k++){
                if(LRU[k]>LRU[myind]){
                    LRU[k]--;
                }
            }
            LRU[myind]=ways-1;
        }
        else if(replacementpolicy==1)
        {
            //update LFU
            LFU[myind]++;            
        }
        else if(replacementpolicy==2 )
        {   
            if(!if_hit){
                //FIFO
                for(int k=index*ways;k<(index+1)*ways;k++){
                    if(FIFO[k]>FIFO[myind]){
                        FIFO[k]--;
                    }
                }
                FIFO[myind]=ways-1;
            }
        }
        else if(replacementpolicy==3){
            //nothing to update
            ;
        }
        else{
            printf("****No replacemennt policy taken****");
        }
    }

}