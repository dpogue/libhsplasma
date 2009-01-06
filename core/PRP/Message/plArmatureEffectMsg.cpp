#include "plArmatureEffectMsg.h"

plArmatureEffectStateMsg::plArmatureEffectStateMsg()
                        : fSurface(0), fAddSurface(false) { }

plArmatureEffectStateMsg::~plArmatureEffectStateMsg() { }

IMPLEMENT_CREATABLE(plArmatureEffectStateMsg, kArmatureEffectStateMsg, plMessage)

void plArmatureEffectStateMsg::read(hsStream* S, plResManager* mgr) {
    plMessage::read(S, mgr);
    fSurface = S->readByte();
    fAddSurface = S->readBool();
}

void plArmatureEffectStateMsg::write(hsStream* S, plResManager* mgr) {
    plMessage::write(S, mgr);
    S->writeByte(fSurface);
    S->writeBool(fAddSurface);
}

void plArmatureEffectStateMsg::IPrcWrite(pfPrcHelper* prc) {
    plMessage::IPrcWrite(prc);

    prc->startTag("ArmatureEffectState");
    prc->writeParam("Surface", fSurface);
    prc->writeParam("Add", fAddSurface);
    prc->endTag(true);
}

void plArmatureEffectStateMsg::IPrcParse(const pfPrcTag* tag, plResManager* mgr) {
    if (tag->getName() == "ArmatureEffectState") {
        fSurface = tag->getParam("Surface", "0").toInt();
        fAddSurface = tag->getParam("AddSurface", "false").toBool();
    } else {
        plMessage::IPrcParse(tag, mgr);
    }
}