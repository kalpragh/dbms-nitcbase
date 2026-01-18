#include <cstdio>
#include<cstring>
#include "Disk_Class/Disk.h"
#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Cache/RelCacheTable.h"
#include "Cache/AttrCacheTable.h"

int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;

  RelCatEntry relEntry;
  AttrCatEntry attrEntry;
  for(int i=0;i<=1;i++){
    if(RelCacheTable::getRelCatEntry(i,&relEntry)!=SUCCESS){
      printf("failed to get rel entry %d\n",i);
      continue;
    }
    printf("Relation: %s\n", relEntry.relName);
    for(int j=0;j<relEntry.numAttrs;j++){
      if(AttrCacheTable::getAttrCatEntry(i,j, &attrEntry)!=SUCCESS){
        printf("failed to get attribute %d\n",j);
        continue;
      }
      printf("  %s: %s\n", attrEntry.attrName, (attrEntry.attrType == NUMBER ? "NUMBER" : "STRING"));
    }
  }
  return 0;
}