// #include "OpenRelTable.h"
// #include <cstdlib>   // malloc, free
// #include <cstring>
// //stage 4 ex q
// #define STUDENTS_RELID 2
// #define RELCAT_SLOTNUM_FOR_STUDENTS 2

// #define ATTRCAT_SLOTNUM_START_FOR_STUDENTS 12
// #define STUDENTS_NO_ATTRS 4

// // Constructor: load RELCAT + ATTRCAT entries into caches
// OpenRelTable::OpenRelTable() {

//   // 1) initialize relCache and attrCache with nullptr
//   for (int i = 0; i < MAX_OPEN; ++i) {
//     RelCacheTable::relCache[i] = nullptr;
//     AttrCacheTable::attrCache[i] = nullptr;
//   }

//   RecBuffer relCatBlock(RELCAT_BLOCK);

//   Attribute relCatRecord[RELCAT_NO_ATTRS];
//   relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

//   RelCacheEntry relCacheEntry0;
//   RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry0.relCatEntry);
//   relCacheEntry0.recId.block = RELCAT_BLOCK;
//   relCacheEntry0.recId.slot  = RELCAT_SLOTNUM_FOR_RELCAT;

//   RelCacheTable::relCache[RELCAT_RELID] =
//       (RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[RELCAT_RELID]) = relCacheEntry0;

//   // ---------------------------
//   // ATTRCAT relation (relId = 1)
//   // ---------------------------
//   Attribute attrRelRecord[RELCAT_NO_ATTRS];
//   relCatBlock.getRecord(attrRelRecord, RELCAT_SLOTNUM_FOR_ATTRCAT);

//   RelCacheEntry relCacheEntry1;
//   RelCacheTable::recordToRelCatEntry(attrRelRecord, &relCacheEntry1.relCatEntry);
//   relCacheEntry1.recId.block = RELCAT_BLOCK;
//   relCacheEntry1.recId.slot  = RELCAT_SLOTNUM_FOR_ATTRCAT;

//   RelCacheTable::relCache[ATTRCAT_RELID] =(RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[ATTRCAT_RELID]) = relCacheEntry1;

//   // ---------------------------------
// // STUDENTS relation (relId = 2)
// // ---------------------------------
// //stage 4 ex
// Attribute studentsRelRecord[RELCAT_NO_ATTRS];
// relCatBlock.getRecord(studentsRelRecord, RELCAT_SLOTNUM_FOR_STUDENTS);

// RelCacheEntry relCacheEntry2;
// RelCacheTable::recordToRelCatEntry(
//     studentsRelRecord,
//     &relCacheEntry2.relCatEntry
// );

// relCacheEntry2.recId.block = RELCAT_BLOCK;
// relCacheEntry2.recId.slot  = RELCAT_SLOTNUM_FOR_STUDENTS;

// // VERY IMPORTANT for linearSearch
// relCacheEntry2.searchIndex.block = -1;
// relCacheEntry2.searchIndex.slot  = -1;

// RelCacheTable::relCache[STUDENTS_RELID] =
//     (RelCacheEntry*)malloc(sizeof(RelCacheEntry));

// *(RelCacheTable::relCache[STUDENTS_RELID]) = relCacheEntry2;
// //till here

//   /************ Setting up Attribute cache entries ************/
//   RecBuffer attrCatBlock(ATTRCAT_BLOCK);

//   Attribute attrCatRecord[ATTRCAT_NO_ATTRS];

//   // ---------------------------------------------------------
//   // Attributes of RELCAT (usually slots 0 to 5 in ATTRCAT)
//   // ---------------------------------------------------------
//   AttrCacheEntry* headRel = nullptr;
//   AttrCacheEntry* tailRel = nullptr;

//   for (int slot = 0; slot <= 5; slot++) {
//     attrCatBlock.getRecord(attrCatRecord, slot);

//     AttrCacheEntry* node = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//     AttrCacheTable::recordToAttrCatEntry(attrCatRecord, &node->attrCatEntry);

//     node->recId.block = ATTRCAT_BLOCK;
//     node->recId.slot  = slot;
//     node->next = nullptr;

//     if (headRel == nullptr) {
//       headRel = tailRel = node;
//     } else {
//       tailRel->next = node;
//       tailRel = node;
//     }
//   }

//   AttrCacheTable::attrCache[RELCAT_RELID] = headRel;

//   // ---------------------------------------------------------
//   // Attributes of ATTRCAT (usually slots 6 to 11 in ATTRCAT)
//   // ---------------------------------------------------------
//   AttrCacheEntry* headAttr = nullptr;
//   AttrCacheEntry* tailAttr = nullptr;

//   for (int slot = 6; slot <= 11; slot++) {
//     attrCatBlock.getRecord(attrCatRecord, slot);

//     AttrCacheEntry* node = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//     AttrCacheTable::recordToAttrCatEntry(attrCatRecord, &node->attrCatEntry);

//     node->recId.block = ATTRCAT_BLOCK;
//     node->recId.slot  = slot;
//     node->next = nullptr;

//     if (headAttr == nullptr) {
//       headAttr = tailAttr = node;
//     } else {
//       tailAttr->next = node;
//       tailAttr = node;
//     }
//   }

//   AttrCacheTable::attrCache[ATTRCAT_RELID] = headAttr;
//   //stage 4 ex q frm here
//     /**** setting up Students relation in the Attribute Cache Table ****/
//   AttrCacheEntry* headStud = nullptr;
//   AttrCacheEntry* tailStud = nullptr;

//   for (int slot = ATTRCAT_SLOTNUM_START_FOR_STUDENTS;
//        slot < ATTRCAT_SLOTNUM_START_FOR_STUDENTS + STUDENTS_NO_ATTRS;
//        slot++) {

//     attrCatBlock.getRecord(attrCatRecord, slot);

//     AttrCacheEntry* node = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//     AttrCacheTable::recordToAttrCatEntry(attrCatRecord, &node->attrCatEntry);

//     node->recId.block = ATTRCAT_BLOCK;
//     node->recId.slot  = slot;
//     node->next = nullptr;

//     if (headStud == nullptr) headStud = tailStud = node;
//     else { tailStud->next = node; tailStud = node; }
//   }

//   AttrCacheTable::attrCache[STUDENTS_RELID] = headStud;
//   //to here
// }

// OpenRelTable::~OpenRelTable() {
//   // free relation cache entries
//   for (int i = 0; i < MAX_OPEN; i++) {
//     if (RelCacheTable::relCache[i] != nullptr) {
//       free(RelCacheTable::relCache[i]);
//       RelCacheTable::relCache[i] = nullptr;
//     }
//   }

//   // free attribute cache linked lists
//   for (int i = 0; i < MAX_OPEN; i++) {
//     AttrCacheEntry* cur = AttrCacheTable::attrCache[i];
//     while (cur != nullptr) {
//       AttrCacheEntry* nxt = cur->next;
//       free(cur);
//       cur = nxt;
//     }
//     AttrCacheTable::attrCache[i] = nullptr;
//   }
// }

// //last part of stage 4
// /* This function will open a relation having name `relName`.
// Since we are currently only working with the relation and attribute catalog, we
// will just hardcode it. In subsequent stages, we will loop through all the relations
// and open the appropriate one.
// */
// int OpenRelTable::getRelId(char relName[ATTR_SIZE]) {

//   // if relname is RELCAT_RELNAME, return RELCAT_RELID
//   // if relname is ATTRCAT_RELNAME, return ATTRCAT_RELID

//   if (strcmp(relName, RELCAT_RELNAME) == 0) {
//     return RELCAT_RELID;
//   }

//   // attribute catalog
//   if (strcmp(relName, ATTRCAT_RELNAME) == 0) {
//     return ATTRCAT_RELID;
//   }
//    if (strcmp(relName, "Students") == 0) {
//     return 2;   // Students relId
//   }
//   return E_RELNOTOPEN;
// }


//stage 3 exercise q from here
#include "OpenRelTable.h"
#include <cstdlib>
#include <cstring>
OpenRelTable::OpenRelTable()
{
    /* initialize caches */
    for (int i = 0; i < MAX_OPEN; i++)
    {
        RelCacheTable::relCache[i] = nullptr;
        AttrCacheTable::attrCache[i] = nullptr;
    }

    /************ RELATION CACHE ************/

    RecBuffer relCatBlock(RELCAT_BLOCK);
    Attribute relCatRecord[RELCAT_NO_ATTRS];
    
    /* ---- RELCAT ---- */
    relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

    RelCacheEntry relCatEntry;
    RelCacheTable::recordToRelCatEntry(
        relCatRecord,
        &relCatEntry.relCatEntry
    );

    relCatEntry.recId.block = RELCAT_BLOCK;
    relCatEntry.recId.slot  = RELCAT_SLOTNUM_FOR_RELCAT;

    RelCacheTable::relCache[RELCAT_RELID] =
        (RelCacheEntry*) malloc(sizeof(RelCacheEntry));

    *(RelCacheTable::relCache[RELCAT_RELID]) = relCatEntry;

    /* ---- ATTRCAT ---- */
    relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_ATTRCAT);

    RelCacheEntry attrCatRelEntry;
    RelCacheTable::recordToRelCatEntry(
        relCatRecord,
        &attrCatRelEntry.relCatEntry
    );

    attrCatRelEntry.recId.block = RELCAT_BLOCK;
    attrCatRelEntry.recId.slot  = RELCAT_SLOTNUM_FOR_ATTRCAT;

    RelCacheTable::relCache[ATTRCAT_RELID] =
        (RelCacheEntry*) malloc(sizeof(RelCacheEntry));

    *(RelCacheTable::relCache[ATTRCAT_RELID]) = attrCatRelEntry;
    /* ---- STUDENTS -----*/
    int studentSlot = -1;
    RelCacheEntry relEntry;

    for(int i=2; i < 16; i++)
    {
         if(relCatBlock.getRecord(relCatRecord, i) != SUCCESS)
         break;
        if(strcmp(relCatRecord[RELCAT_REL_NAME_INDEX].sVal, "Students") == 0)
        {
            studentSlot = i;
            break;
        }
    }
    if(studentSlot != -1)
    {
        relCatBlock.getRecord(relCatRecord, studentSlot);
        RelCacheTable::recordToRelCatEntry(relCatRecord, &relEntry.relCatEntry);
        relEntry.recId.block = RELCAT_BLOCK;
        relEntry.recId.slot = studentSlot;
         RelCacheTable::relCache[2] =
        (RelCacheEntry*)malloc(sizeof(RelCacheEntry));
    *(RelCacheTable::relCache[2]) = relEntry;
    }
    /************ ATTRIBUTE CACHE ************/

    RecBuffer attrCatBlock(ATTRCAT_BLOCK);
    Attribute attrCatRecord[ATTRCAT_NO_ATTRS];

    /* ---- Attributes of RELCAT ---- */
    AttrCacheEntry *head = nullptr;
    AttrCacheEntry *prev = nullptr;

    for (int i = 0; i <= 5; i++)
    {
        attrCatBlock.getRecord(attrCatRecord, i);

        AttrCacheEntry *curr =
            (AttrCacheEntry*) malloc(sizeof(AttrCacheEntry));

        AttrCacheTable::recordToAttrCatEntry(
            attrCatRecord,
            &curr->attrCatEntry
        );

        curr->recId.block = ATTRCAT_BLOCK;
        curr->recId.slot  = i;
        curr->next        = nullptr;

        if (!head)
            head = curr;
        else
            prev->next = curr;

        prev = curr;
    }

    AttrCacheTable::attrCache[RELCAT_RELID] = head;

    /* ---- Attributes of ATTRCAT ---- */
    head = nullptr;
    prev = nullptr;

    for (int i = 6;i <= 11; i++)
    {
        attrCatBlock.getRecord(attrCatRecord, i);

        AttrCacheEntry *curr =
            (AttrCacheEntry*) malloc(sizeof(AttrCacheEntry));

        AttrCacheTable::recordToAttrCatEntry(
            attrCatRecord,
            &curr->attrCatEntry
        );

        curr->recId.block = ATTRCAT_BLOCK;
        curr->recId.slot  = i;
        curr->next        = nullptr;

        if (!head)
            head = curr;
        else
            prev->next = curr;

        prev = curr;
    }

    AttrCacheTable::attrCache[ATTRCAT_RELID] = head;
    head = prev = nullptr;
    for(int i=0; i < 16; i++)
    {
        if(attrCatBlock.getRecord(attrCatRecord, i) != SUCCESS)
        break;
    if(strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal, "Students") == 0)
     {
        AttrCacheEntry* curr =  (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
        AttrCacheTable::recordToAttrCatEntry(attrCatRecord, &curr->attrCatEntry);

        curr->recId.block = ATTRCAT_BLOCK;
        curr->recId.slot = i;
        curr->next = nullptr;

        if(!head)
        head = prev = curr;
       else
        {
            prev->next = curr;
            prev = curr;
        }
     }
    }
AttrCacheTable::attrCache[2] = head;

}

OpenRelTable::~OpenRelTable()
{
    for (int i = 0; i < MAX_OPEN; i++)
    {
        if (RelCacheTable::relCache[i])
        {
            free(RelCacheTable::relCache[i]);
            RelCacheTable::relCache[i] = nullptr;
        }

        AttrCacheEntry *curr = AttrCacheTable::attrCache[i];
        while (curr)
        {
            AttrCacheEntry *next = curr->next;
            free(curr);
            curr = next;
        }
        AttrCacheTable::attrCache[i] = nullptr;
    }
}

int OpenRelTable::getRelId(char relName[ATTR_SIZE]) {

    if (strcmp(relName, RELCAT_RELNAME) == 0)
        return RELCAT_RELID;

    if (strcmp(relName, ATTRCAT_RELNAME) == 0)
        return ATTRCAT_RELID;
    return E_RELNOTOPEN;
}
//to here
