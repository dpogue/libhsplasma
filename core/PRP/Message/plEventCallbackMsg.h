/* This file is part of HSPlasma.
 *
 * HSPlasma is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HSPlasma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HSPlasma.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _PLEVENTCALLBACKMSG_H
#define _PLEVENTCALLBACKMSG_H

#include "plMessage.h"

class PLASMA_DLL plEventCallbackMsg : public plMessage {
    CREATABLE(plEventCallbackMsg, kEventCallbackMsg, plMessage)

protected:
    float fEventTime;
    CallbackEvent fEvent;
    short fIndex, fRepeats, fUser;

public:
    plEventCallbackMsg();

    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);

protected:
    virtual void IPrcWrite(pfPrcHelper* prc);
    virtual void IPrcParse(const pfPrcTag* tag, plResManager* mgr);

public:
    float getEventTime() const { return fEventTime; }
    CallbackEvent getEvent() const { return fEvent; }
    short getIndex() const { return fIndex; }
    short getRepeats() const { return fRepeats; }
    short getUser() const { return fUser; }

    void setEventTime(float time) { fEventTime = time; }
    void setEvent(CallbackEvent evt) { fEvent = evt; }
    void setIndex(short idx) { fIndex = idx; }
    void setRepeats(short repeats) { fRepeats = repeats; }
    void setUser(short user) { fUser = user; }
};


class PLASMA_DLL plEventCallbackSetupMsg : public plMessage {
    CREATABLE(plEventCallbackSetupMsg, kEventCallbackSetupMsg, plMessage)

public:
    class PLASMA_DLL plAnimCallbackSetup {
    public:
        plString fMarker;
        plKey fReceiver;
        short fUser;

        plAnimCallbackSetup();
    };

protected:
    hsTArray<plAnimCallbackSetup> fCallbacks;

public:
    plEventCallbackSetupMsg();

    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);

protected:
    virtual void IPrcWrite(pfPrcHelper* prc);
    virtual void IPrcParse(const pfPrcTag* tag, plResManager* mgr);
};

#endif
