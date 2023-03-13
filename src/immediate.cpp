#include "builtin_funcs.h"
long long int bin2dec(string str);
long long int unsgn_binaryToDecimal(string n);
long long int sign_extn(string bin){
    string temp;
    long long int imm=0;
    long long int itr=32-bin.length();
    if(bin[0]=='1'){
        temp="";
        for(long long int i=0;i<itr;i++){
            temp.push_back('1');
        }
        bin=temp+bin;
        imm=bin2dec(bin);
    }
    else{
        temp="";
        for(long long int i=0;i<itr;i++){
            temp.push_back('0');
        }
        bin=temp+bin;
        imm=bin2dec(bin);
    } 
    return imm;

}
long long int unsign_extn(string bin){
    string temp;
    long long int imm=0;
    long long int itr=32-bin.length();
    temp="";
    for(long long int i=0;i<itr;i++){
        temp.push_back('0');
    }
    bin=temp+bin;
    imm=bin2dec(bin);
    return imm;
}
long long int immediate(string instruction){
    string opcode=instruction.substr(25,7);
    long long int imm=0;
    string imm_str;
    string temp;
    if(opcode=="0010011"||opcode=="0000011"||opcode=="1100111"){
        //arithmetic-immediate, load and jalr
        string funct3=instruction.substr(17,3);
        if((opcode=="0010011")&&(funct3=="001"||funct3=="101")){
            imm_str=instruction.substr(7,5);
            imm=sign_extn(imm_str);
        }
        else{
            imm_str=instruction.substr(0,12);
            imm=sign_extn(imm_str);
        }   
    }
    else if(opcode=="0100011"){
        imm_str=instruction.substr(0,7)+instruction.substr(20,5);
        imm=sign_extn(imm_str);
    }
    else if(opcode=="1100011"){
        imm_str=instruction.substr(0,1)+instruction.substr(24,1)+instruction.substr(1,6)+instruction.substr(20,4)+"0";
        imm=sign_extn(imm_str);
    }
    else if(opcode=="1101111"){
        imm_str=instruction.substr(0,1)+instruction.substr(12,8)+instruction.substr(11,1)+instruction.substr(1,10)+"0";
        cout<<imm_str<<" jal str"<<endl;
        imm=sign_extn(imm_str);
    }
    else if(opcode=="0110111"||opcode=="0010111"){
        //lui and auipc
        imm_str=instruction.substr(0,20);
        for(long long int i=0;i<12;i++){
            imm_str.push_back('0');
        }
        cout<<imm_str<<endl;
        imm=sign_extn(imm_str);
    }
    return imm;
}

long long int unsgn_binaryToDecimal(string n)
{
    string num = n;
    long long int dec_value = 0;
 
    // Initializing base value to 1, i.e 2^0
    long long int base = 1;
 
    long long int len = num.length();
    for (long long int i = len - 1; i >= 0; i--) {
        if (num[i] == '1')
            dec_value += base;
        base = base * 2;
    }
 
    return dec_value;
}
long long int bin2dec(string str){
    long long int n=str.length();
    if(str[0]=='1'){
        return -(pow(2,n)-unsgn_binaryToDecimal(str));
    }
    else{
        return unsgn_binaryToDecimal(str);
    }
}