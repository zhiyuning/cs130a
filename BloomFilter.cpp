#include "BloomFilter.h"

BloomFilter::BloomFilter(int k, int m, std::string strfn, std::string intfn){
  this->k=k;
  this->m=m;
  int q=(m/64)+1;
  bits=new uint64_t[q];
  for(int i = 0; i < q; i++){
    bits[i] = uint64_t(0);
  }
  if(strfn=="jenkins"){
    this->strfn=new JenkinsHash;
  }else{
    this->strfn=new PearsonHash;
  }

  this->intfns=new IntegerHash *[k];
  if(intfn=="division"){
    //this->intfns=new IntegerHash[k];
    for(int i=0;i<this->k;++i){
      this->intfns[i]=new DivisionHash(i, m);
    }
  }else if(intfn=="reciprocal"){
    //this->intfns=new IntegerHash[k];
    for(int i=0;i<this->k;++i){
      this->intfns[i]=new ReciprocalHash(i, m);
    }
  }else if(intfn=="squareroot"){
    //this->intfns=new IntegerHash[k];
    for(int i=0;i<this->k;++i){
      this->intfns[i]=new SquareRootHash(i, m);
    }
  }
  //this->bits = new uint64_t[m];
  //*bits=0;
  //*intfns++;
  //(*bits) = 0;
  //(*bits) |= (0<<m);
}



BloomFilter::~BloomFilter(){
  delete strfn;
  for(int i=0;i<this->k;++i){
    delete intfns[i];
  }
  delete[] intfns;
  delete[] bits;
}

void BloomFilter::insert(const std::string& value){
    uint64_t stringHashValue=strfn->hash(value);
    for(int i=0;i<k;i++){
        uint64_t intHash=intfns[i]->hash(stringHashValue);
        uint64_t index=(intHash/64);
        int position=intHash % 64;
        uint64_t k=(uint64_t(1)<<position);
        bits[index] |= k;
    }
}

bool BloomFilter::lookup(const std::string& value) const{
    uint64_t stringHashValue=strfn->hash(value);
    for(int i=0;i<k;i++){
        uint64_t intHash=intfns[i]->hash(stringHashValue);
        int index=(intHash / 64);
        uint64_t temp=bits[index]>>(intHash % 64);
        if((temp&uint64_t(1))==0){
            return false;
        }
    }
    return true;
}
