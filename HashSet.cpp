#include "HashSet.h"



HashSet::HashSet(){
  nitems=0;
  nslots=10;
  this->strfn=new JenkinsHash;
  this->intfn=new SquareRootHash(1, nslots);
  this->slots=new std::string *[nslots];
  for(int i=0;i<this->nslots;i++){
    this->slots[i]=nullptr;
  }
  this->strfn2=nullptr;

}

HashSet::~HashSet(){
  for(int i = 0;i<nslots;i++){
    delete slots[i];
  }
  delete[] slots;
  delete strfn;
  //strfn=nullptr;
  delete intfn;
  //intfn=nullptr;
}

void HashSet::insert(const std::string& value){
    if(lookup(value)){
        return;
    }
    nitems+=1;
    if(nitems==nslots){
      rehash();
    }
    uint64_t stringToIntHash =strfn->hash(value);
    uint64_t hashValue =intfn->hash(stringToIntHash);
    while(slots[hashValue]!=nullptr){
        hashValue=(hashValue+1)%nslots;
    }
    slots[hashValue] = new std::string(value);

}

bool HashSet::lookup(const std::string& value) const{
    uint64_t stringToIntHash=strfn->hash(value);
    uint64_t hashValue=intfn->hash(stringToIntHash);
    while(slots[hashValue]!=nullptr){
        if(*slots[hashValue]==value){
            return true;
        }
        hashValue=(hashValue+1)%nslots;
    }
    return false;
}

void HashSet::rehash(){
    delete intfn;
    int origsize=nslots;
    nslots=origsize*2;
    intfn=new DivisionHash(3,nslots);
    std::string** temp=slots;
    slots=new std::string* [nslots];

    for(int i=0;i<nslots;i++){
      slots[i] = nullptr;
    }
    for(int q=0;q<origsize;q++){
        if(temp[q]!=nullptr){
            insert(*temp[q]);
        }
    }

    for(int i=0;i<origsize;i++){
        if(temp[i]!=nullptr){
            delete temp[i];
        }
    }

    delete[] temp;
}
