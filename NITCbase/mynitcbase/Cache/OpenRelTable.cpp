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
    if(strcmp(relName, "Students") == 0)
        return 2;// Students in relcat is 2 
    return E_RELNOTOPEN;
}