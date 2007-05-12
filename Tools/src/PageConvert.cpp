#include <stdio.h>
#include <string.h>
#include "PubUtilLib/plResMgr/plResManager.h"

const char* PlasmaVerNames[] = { "Unknown", "Prime", "PotS", "EOA", "Live" };

const char* getSuffix(PlasmaVer pv) {
    switch (pv) {
    case pvPrime:    return "prime";
    case pvPots:     return "pots";
    case pvLive:     return "live";
    case pvEoa:      return "eoa";
    default:         return "err";
    }
}

void doHelp() {
    printf("PRP Page Converter 1.0\nBy Michael Hansen\n\n");
    printf("Usage:  PageConvert [-to???] [-help] filename [filename] [...]\n\n");
    printf("  -toprime  Converts to Prime format (63.11) (Default)\n");
    printf("  -topots   Converts to Path of the Shell format (63.12)\n");
    printf("  -tolive   Converts to Uru Live format (69.3)\n");
    printf("  -toeoa    Converts to Myst V: End of Ages format\n");
    printf("  -help     Displays this screen\n\n");
}

int main(int argc, char** argv) {
    plResManager rm;

    PlasmaVer toVer = pvPrime;
    int files = 0;

    if (argc == 1) {
        doHelp();
        return 0;
    }

    for (int i=1; i<argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') argv[i]++;
            if (strcmp(argv[i], "-toeoa") == 0)
                toVer = pvEoa;
            else if (strcmp(argv[i], "-toprime") == 0)
                toVer = pvPrime;
            else if (strcmp(argv[i], "-topots") == 0)
                toVer = pvPots;
            else if (strcmp(argv[i], "-tolive") == 0)
                toVer = pvLive;
            else if (strcmp(argv[i], "-help") == 0) {
                doHelp();
                return 0;
            } else {
                fprintf(stderr, "Error: Unrecognized option %s\n", argv[i]);
                return 1;
            }
        } else {
            plPageInfo* page;
            try {
                page = rm.ReadPage(argv[i]);
            } catch (const char* e) {
                fprintf(stderr, "%s\n", e);
                return 1;
            } catch (...) {
                fprintf(stderr, "Undefined error!\n");
                return 1;
            }
            printf("PageID: %s (%08X)\n", page->getLocation().toString(),
                                          page->getLocation().pageID.unparse());
            printf("Page Flags: %d\n", page->getLocation().flags);
            printf("Age Name: %s\n", page->getAge());
            printf("Page Name: %s\n", page->getPage());
            printf("Plasma Version: %s\n", PlasmaVerNames[rm.getVer()]);
            printf("Keyring: %d keys\n", rm.keys.countKeys(page->getLocation().pageID));
            printf("Objects Read: %d\n", page->nObjects);
            
            rm.setVer(toVer, true);
            char fn[256];
            sprintf(fn, "%s.%s", argv[i], getSuffix(toVer));
            rm.WritePage(fn, page);
            printf("Objects Written: %d\n\n", page->nObjects);
            files++;
        }
    }

    printf("Done!  Successfully transcoded %d files!\n", files);
    return 0;
}
