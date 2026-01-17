#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include <cstring>
#include <iostream>
#include <cstdio>


int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;

  // create objects for the relation catalog and attribute catalog
  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  // load the headers of both the blocks into relCatHeader and attrCatHeader.
  // (we will implement these functions later)
  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);

  for (int i = 0;i<relCatHeader.numEntries;i++) {

    Attribute relCatRecord[RELCAT_NO_ATTRS]; // will store the record from the relation catalog
    relCatBuffer.getRecord(relCatRecord, i);

    const char* relName = relCatRecord[RELCAT_REL_NAME_INDEX].sVal;
    printf("Relation: %s\n", relName);
    
    int attrBlockNum = ATTRCAT_BLOCK; //from here 

    while (attrBlockNum != -1) {
      RecBuffer attrCatBuffer(attrBlockNum);
      HeadInfo attrCatHeader;
      attrCatBuffer.getHeader(&attrCatHeader); // to here is part of ex1

      for (int j=0;j<attrCatHeader.numEntries;j++) {
        Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
        attrCatBuffer.getRecord(attrCatRecord, j);
//frm here
        const char *thisRel = attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal;

                // Only print attrs belonging to current relation
                if (strcmp(thisRel, relName) == 0) {
                  // ---- Q2: Update Students.Class -> Students.Batch in DBMS code ----
                  const char *attrName = attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal;

                  if (strcmp(relName, "Students") == 0 && strcmp(attrName, "Class") == 0) {
                    // overwrite attribute name safely (ATTR_SIZE bytes)
                    std::memset(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, 0, ATTR_SIZE);
                    std::strncpy(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, "Batch", ATTR_SIZE - 1);

                    // write updated record back to disk
                    attrCatBuffer.setRecord(attrCatRecord, j);
                  }

                  const char *finalAttrName = attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal;
                  const char *attrType =
                      (attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER) ? "NUM" : "STR";

                  printf("  %s: %s\n", finalAttrName, attrType);
                }
              }

              // hop to next ATTRCAT block using rblock
              attrBlockNum = attrCatHeader.rblock;
            }

            printf("\n");
          }

          return 0;
        }
        //to here is ex2

//         if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal, relName) == 0) {
//           const char* attrType =(attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER) ? "NUM" : "STR";
//           const char *attrName = attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal;

//           printf("  %s: %s\n",attrName,attrType);
//       }
//     }
//     attrBlockNum = attrCatHeader.rblock; //to move right *this is the exttra part
//     }
//     printf("\n");
//   }

//   return 0;
// }