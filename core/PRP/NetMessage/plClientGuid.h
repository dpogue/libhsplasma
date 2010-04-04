#ifndef _PLCLIENTGUID_H
#define _PLCLIENTGUID_H

#include "PRP/plCreatable.h"
#include "Sys/plUuid.h"

DllClass plClientGuid : public plCreatable {
    CREATABLE(plClientGuid, kClientGuid, plCreatable)

public:
    enum Flags {
        kAcctUuid = 0x1,
        kPlayerID = 0x2,
        kTempPlayerID = 0x4,
        kCCRLevel = 0x8,
        kProtectedLogin = 0x10,
        kBuildType = 0x20,
        kPlayerName = 0x40,
        kSrcAddr = 0x80,
        kSrcPort = 0x100,
        kReserved = 0x200,
        kClientKey = 0x400,
    };

private:
    unsigned short fFlags;
    plUuid fAcctUuid;
    unsigned int fPlayerID;
    plString fPlayerName;
    unsigned char fCCRLevel, fProtectedLogin, fBuildType;
    unsigned int fSrcAddr;
    unsigned short fSrcPort;
    unsigned short fReserved;
    plString fClientKey;

public:
    plClientGuid();

    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);

protected:
    virtual void IPrcWrite(pfPrcHelper* prc);
    virtual void IPrcParse(const pfPrcTag* tag, plResManager* mgr);

public:
    unsigned short getFlags() const { return fFlags; }
    plUuid getAcctUuid() const { return fAcctUuid; }
    unsigned int getPlayerID() const { return fPlayerID; }
    plString getPlayerName() const { return fPlayerName; }
    unsigned char getCCRLevel() const { return fCCRLevel; }
    unsigned char getProtectedLogin() const { return fProtectedLogin; }
    unsigned char getBuildType() const { return fBuildType; }
    unsigned int getSrcAddr() const { return fSrcAddr; }
    unsigned short getSrcPort() const { return fSrcPort; }
    unsigned short getReserved() const { return fReserved; }
    plString getClientKey() const { return fClientKey; }

    void setFlags(unsigned short flags) { fFlags = flags; }
    void setAcctUuid(const plUuid& acctUuid) { fAcctUuid = acctUuid; }
    void setPlayerID(unsigned int playerID) { fPlayerID = playerID; }
    void setPlayerName(const plString& playerName) { fPlayerName = playerName; }
    void setCCRLevel(unsigned char cCRLevel) { fCCRLevel = cCRLevel; }
    void setProtectedLogin(unsigned char protectedLogin) { fProtectedLogin = protectedLogin; }
    void setBuildType(unsigned char buildType) { fBuildType = buildType; }
    void setSrcAddr(unsigned int srcAddr) { fSrcAddr = srcAddr; }
    void setSrcPort(unsigned short srcPort) { fSrcPort = srcPort; }
    void setReserved(unsigned short reserved) { fReserved = reserved; }
    void setClientKey(const plString& clientKey) { fClientKey = clientKey; }
};

#endif
