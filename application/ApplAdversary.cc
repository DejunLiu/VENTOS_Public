
#include "ApplAdversary.h"

const simsignalwrap_t ApplAdversary::mobilityStateChangedSignal = simsignalwrap_t(MIXIM_SIGNAL_MOBILITY_CHANGE_NAME);

Define_Module(ApplAdversary);

void ApplAdversary::initialize(int stage)
{
	BaseApplLayer::initialize(stage);

	if (stage==0)
	{
        // get the ptr of the current module
        nodePtr = FindModule<>::findHost(this);
        if(nodePtr == NULL)
            error("can not get a pointer to the module.");

		myMac = FindModule<WaveAppToMac1609_4Interface*>::findSubModule(getParentModule());
		assert(myMac);

		TraCI = FindModule<TraCI_Extend*>::findGlobalModule();

        findHost()->subscribe(mobilityStateChangedSignal, this);

        // vehicle id in omnet++
		myId = getParentModule()->getIndex();

		myFullId = getParentModule()->getFullName();

		AttackT = par("AttackT").doubleValue();
		falsificationAttack = par("falsificationAttack").boolValue();
		replayAttck = par("replayAttck").boolValue();
		jammingAttck = par("jammingAttck").boolValue();
	}
}


void ApplAdversary::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj)
{
    Enter_Method_Silent();

    if (signalID == mobilityStateChangedSignal)
    {
        ApplAdversary::handlePositionUpdate(obj);
    }
}


void ApplAdversary::handleLowerMsg(cMessage* msg)
{
    // Attack time has not arrived yet!
    if(simTime().dbl() < AttackT)
        return;

    // make sure msg is of type WaveShortMessage
    WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsm);

    if ( string(wsm->getName()) == "beaconVehicle" )
    {
        BeaconVehicle* wsm = dynamic_cast<BeaconVehicle*>(msg);
        ASSERT(wsm);

        EV << "######### received a beacon!" << endl;

        if(falsificationAttack)
        {
            DoFalsificationAttack(wsm);
        }
        else if(replayAttck)
        {
            DoReplayAttack(wsm);
        }
        else if(jammingAttck)
        {
            DoJammingAttack(wsm);
        }
    }
    else if( string(wsm->getName()) == "platoonMsg" )
    {
        PlatoonMsg* wsm = dynamic_cast<PlatoonMsg*>(msg);
        ASSERT(wsm);

        // ignore it!
    }
}


void ApplAdversary::handleSelfMsg(cMessage* msg)
{

}


// adversary get a msg, modifies the acceleration and re-send it
void ApplAdversary::DoFalsificationAttack(BeaconVehicle* wsm)
{
    // duplicate the received beacon
    BeaconVehicle* FalseMsg = wsm->dup();

    // alter the acceleration field
    FalseMsg->setAccel(6.);

    // send it
    sendDelayedDown(FalseMsg, 0.);

    EV << "## Altered msg is sent." << endl;
}


// adversary get a msg and re-send it with a delay (without altering the content)
void ApplAdversary::DoReplayAttack(BeaconVehicle * wsm)
{
    // duplicate the received beacon
    BeaconVehicle* FalseMsg = wsm->dup();

    // send it with delay
    double delay = 10;
    sendDelayedDown(FalseMsg, delay);

    EV << "## Altered msg is sent with delay of " << delay << endl;
}


void ApplAdversary::DoJammingAttack(BeaconVehicle * wsm)
{


}


void ApplAdversary::handlePositionUpdate(cObject* obj)
{
    ChannelMobilityPtrType const mobility = check_and_cast<ChannelMobilityPtrType>(obj);
    curPosition = mobility->getCurrentPosition();
}


void ApplAdversary::finish()
{
	findHost()->unsubscribe(mobilityStateChangedSignal, this);
}


ApplAdversary::~ApplAdversary()
{

}

