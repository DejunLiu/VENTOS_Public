/****************************************************************************/
/// @file    vLog.h
/// @author  Mani Amoozadeh <maniam@ucdavis.edu>
/// @author  second author name
/// @date    May 2016
///
/****************************************************************************/
// VENTOS, Vehicular Network Open Simulator; see http:?
// Copyright (C) 2013-2015
/****************************************************************************/
//
// This file is part of VENTOS.
// VENTOS is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#ifndef LOGRECORDER_H
#define LOGRECORDER_H

#include <BaseApplLayer.h>
#include <omnetpp.h>
#include "boost/format.hpp"
#include <mutex>

namespace VENTOS {

#define   WARNING_LOG   0b00000001
#define   INFO_LOG      0b00000010
#define   ERROR_LOG     0b00000100
#define   DEBUG_LOG     0b00001000
#define   EVENT_LOG     0b00010000   // event log
#define   ALL_LOG       0b11111111
#define   NO_LOG        0b00000000


class vLog : public BaseApplLayer
{
public:
    virtual ~vLog();
    virtual void initialize(int stage);
    virtual void finish();
    virtual void handleMessage(omnetpp::cMessage *msg);

    // overloading the << operator
    template<typename T>
    vLog& operator << (const T& inv)
    {
        std::lock_guard<std::mutex> lock(lock_log);

        if( logRecordCMD || omnetpp::cSimulation::getActiveEnvir()->isGUI() )
        {
            if( (systemLogLevel & lastLogLevel) != 0 )
            {
                auto it = vLogStreams.find(lastCategory);
                if(it == vLogStreams.end())
                    throw omnetpp::cRuntimeError("can't find the category in vLogStreams map!");

                // use the stream associated with this category
                *(it->second) << inv;
            }
        }

        return *this;
    }

    vLog& setLog(uint8_t logLevel = INFO_LOG, std::string cat = "std::cout", std::string subcat = "");
    void flush();

private:
    typedef BaseApplLayer super;

    std::mutex lock_log;
    uint8_t systemLogLevel = 0;
    bool logRecordCMD = false;

    std::map<std::string /*category name*/, std::ostream *> vLogStreams;
    uint8_t lastLogLevel = INFO_LOG;
    std::string lastCategory = "std::cout";
};

}

#endif
