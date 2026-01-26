#include "BlockAccess.h"
#include <cstdlib>
#include <cstring>
RecId BlockAccess::linearSearch(int relId, char attrName[ATTR_SIZE], union Attribute attrVal, int op) {
  
    RecId prevRecId;
    RelCacheTable::getSearchIndex(relId, &prevRecId);
    
    int block, slot;

    if (prevRecId.block == -1 && prevRecId.slot == -1)
    {
        RelCatEntry relCatBuf;
        RelCacheTable::getRelCatEntry(relId, &relCatBuf);
        block = relCatBuf.firstBlk;
        slot = 0;
         
    }
    else
    {
       block = prevRecId.block;
       slot  = prevRecId.slot + 1;        
    }
     AttrCatEntry attrCatBuf;
    AttrCacheTable::getAttrCatEntry(relId, attrName, &attrCatBuf);
    int offset = attrCatBuf.offset;

    while (block != -1)
    {
         RecBuffer buf(block);
         HeadInfo header;
         buf.getHeader(&header);

         unsigned char slotMap[header.numSlots];

         buf.getSlotMap(slotMap);

       if(slot >= header.numSlots)          
        {
            block = header.rblock;
            slot = 0;
            continue;  // continue to the beginning of this while loop
        }

        if(slotMap[slot] == SLOT_UNOCCUPIED)        
        {
          slot++;
          continue;
        }
          Attribute record[header.numAttrs];
        buf.getRecord(record, slot);
        Attribute recAttr = record[offset];

        int cmpVal = compareAttrs(recAttr, attrVal, attrCatBuf.attrType);
        
        if (
            (op == NE && cmpVal != 0) ||    // if op is "not equal to" 
            (op == LT && cmpVal < 0) ||     // if op is "less than"
            (op == LE && cmpVal <= 0) || // if op is "less than or equalto"
            (op == EQ && cmpVal == 0) ||    // if op is "equal to"
            (op == GT && cmpVal > 0) ||     // if op is "greater than"
            (op == GE && cmpVal >= 0)     // if op is "greater than or equalto"
        )
        {
            RecId currRecId = {block, slot};
            RelCacheTable::setSearchIndex(relId, &currRecId);

            return RecId{block, slot};
        }

        slot++;
    }

    // no record in the relation with Id relid satisfies the given condition
    
 return RecId{-1, -1};
}