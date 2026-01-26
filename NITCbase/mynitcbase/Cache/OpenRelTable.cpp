#include "OpenRelTable.h"
#include <cstdlib>   // malloc, free
#include <cstring>

// Constructor: load RELCAT + ATTRCAT entries into caches
OpenRelTable::OpenRelTable() {

  // 1) initialize relCache and attrCache with nullptr
  for (int i = 0; i < MAX_OPEN; ++i) {
    RelCacheTable::relCache[i] = nullptr;
    AttrCacheTable::attrCache[i] = nullptr;
  }

  RecBuffer relCatBlock(RELCAT_BLOCK);

  Attribute relCatRecord[RELCAT_NO_ATTRS];
  relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

  RelCacheEntry relCacheEntry0;
  RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry0.relCatEntry);
  relCacheEntry0.recId.block = RELCAT_BLOCK;
  relCacheEntry0.recId.slot  = RELCAT_SLOTNUM_FOR_RELCAT;

  RelCacheTable::relCache[RELCAT_RELID] =
      (RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[RELCAT_RELID]) = relCacheEntry0;

  // ---------------------------
  // ATTRCAT relation (relId = 1)
  // ---------------------------
  Attribute attrRelRecord[RELCAT_NO_ATTRS];
  relCatBlock.getRecord(attrRelRecord, RELCAT_SLOTNUM_FOR_ATTRCAT);

  RelCacheEntry relCacheEntry1;
  RelCacheTable::recordToRelCatEntry(attrRelRecord, &relCacheEntry1.relCatEntry);
  relCacheEntry1.recId.block = RELCAT_BLOCK;
  relCacheEntry1.recId.slot  = RELCAT_SLOTNUM_FOR_ATTRCAT;

  RelCacheTable::relCache[ATTRCAT_RELID] =
      (RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[ATTRCAT_RELID]) = relCacheEntry1;

  /************ Setting up Attribute cache entries ************/
  RecBuffer attrCatBlock(ATTRCAT_BLOCK);

  Attribute attrCatRecord[ATTRCAT_NO_ATTRS];

  // ---------------------------------------------------------
  // Attributes of RELCAT (usually slots 0 to 5 in ATTRCAT)
  // ---------------------------------------------------------
  AttrCacheEntry* headRel = nullptr;
  AttrCacheEntry* tailRel = nullptr;

  for (int slot = 0; slot <= 5; slot++) {
    attrCatBlock.getRecord(attrCatRecord, slot);

    AttrCacheEntry* node = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
    AttrCacheTable::recordToAttrCatEntry(attrCatRecord, &node->attrCatEntry);

    node->recId.block = ATTRCAT_BLOCK;
    node->recId.slot  = slot;
    node->next = nullptr;

    if (headRel == nullptr) {
      headRel = tailRel = node;
    } else {
      tailRel->next = node;
      tailRel = node;
    }
  }

  AttrCacheTable::attrCache[RELCAT_RELID] = headRel;

  // ---------------------------------------------------------
  // Attributes of ATTRCAT (usually slots 6 to 11 in ATTRCAT)
  // ---------------------------------------------------------
  AttrCacheEntry* headAttr = nullptr;
  AttrCacheEntry* tailAttr = nullptr;

  for (int slot = 6; slot <= 11; slot++) {
    attrCatBlock.getRecord(attrCatRecord, slot);

    AttrCacheEntry* node = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
    AttrCacheTable::recordToAttrCatEntry(attrCatRecord, &node->attrCatEntry);

    node->recId.block = ATTRCAT_BLOCK;
    node->recId.slot  = slot;
    node->next = nullptr;

    if (headAttr == nullptr) {
      headAttr = tailAttr = node;
    } else {
      tailAttr->next = node;
      tailAttr = node;
    }
  }

  AttrCacheTable::attrCache[ATTRCAT_RELID] = headAttr;
}

OpenRelTable::~OpenRelTable() {
  // free relation cache entries
  for (int i = 0; i < MAX_OPEN; i++) {
    if (RelCacheTable::relCache[i] != nullptr) {
      free(RelCacheTable::relCache[i]);
      RelCacheTable::relCache[i] = nullptr;
    }
  }

  // free attribute cache linked lists
  for (int i = 0; i < MAX_OPEN; i++) {
    AttrCacheEntry* cur = AttrCacheTable::attrCache[i];
    while (cur != nullptr) {
      AttrCacheEntry* nxt = cur->next;
      free(cur);
      cur = nxt;
    }
    AttrCacheTable::attrCache[i] = nullptr;
  }
}

//last part of stage 4
/* This function will open a relation having name `relName`.
Since we are currently only working with the relation and attribute catalog, we
will just hardcode it. In subsequent stages, we will loop through all the relations
and open the appropriate one.
*/
int OpenRelTable::getRelId(char relName[ATTR_SIZE]) {

  // if relname is RELCAT_RELNAME, return RELCAT_RELID
  // if relname is ATTRCAT_RELNAME, return ATTRCAT_RELID

  if (strcmp(relName, RELCAT_RELNAME) == 0) {
    return RELCAT_RELID;
  }

  // attribute catalog
  if (strcmp(relName, ATTRCAT_RELNAME) == 0) {
    return ATTRCAT_RELID;
  }
  return E_RELNOTOPEN;
}