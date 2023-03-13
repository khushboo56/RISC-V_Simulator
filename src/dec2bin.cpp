#include "builtin_funcs.h"
string dec2bin(unsigned int n)
{
    string val="";
    while (n != 0)
    {
        if(n % 2==1){
            val.push_back('1');
        }
        else{
            val.push_back('0');
        }
        n /= 2;
    }
    int len =val.length();
    for(int i=0;i<(32-len);i++){
        val.push_back('0');
    }
    reverse(val.begin(),val.end());
    return val;
}
