#ifndef _PLKEY_H
#define _PLKEY_H

#include "../../CoreLib/hsStream.h"
#include "../../DynLib/PageID.h"
#include "../../CoreLib/hsRefCnt.h"

class plKey : public hsRefCnt {
public:
    char flags;
    PageID pageID;
    short pageType;
    char extra1;
    short objType;
    int objID;
    char* objName;
    int someID, clientID;
    char eoaExtra2;
    class hsKeyedObject* objPtr;

    bool exists;
    
    unsigned long fileOff;
    unsigned long objSize;

public:
    plKey();
    plKey(plKey * init);
    ~plKey();

    bool operator==(plKey &other);

    /*
    void read(hsStream *S);
    void write(hsStream *S);
    
    void readRef(hsStream *S);
    void writeRef(hsStream *S);
    */
};

#endif
