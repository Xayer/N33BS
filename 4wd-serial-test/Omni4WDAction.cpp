#include "Omni4WDAction.h"

/*
	enum {STAT_UNKNOWN,
			STAT_STOP,
			STAT_ADVANCE,
			STAT_BACKOFF,
			STAT_LEFT,
			STAT_RIGHT,
			STAT_ROTATELEFT,
			STAT_ROTATERIGHT,
			STAT_UPPERLEFT,
			STAT_LOWERLEFT,
			STAT_LOWERRIGHT,
			STAT_UPPERRIGHT,
	};

	int (Omni4WD::*carAction[])(int speedMMPS)={
		&Omni4WD::setCarStop,
		&Omni4WD::setCarAdvance,
		&Omni4WD::setCarBackoff,
		&Omni4WD::setCarLeft,
		&Omni4WD::setCarRight,
		&Omni4WD::setCarUpperLeft,
		&Omni4WD::setCarLowerRight,
		&Omni4WD::setCarLowerLeft,
		&Omni4WD::setCarUpperRight,
		&Omni4WD::setCarRotateLeft,
		&Omni4WD::setCarRotateRight
	};

*/

/*
// failed??
// error: expected constructor, destructor, or type conversion before '=' token
//
const int (Omni4WD::*carAction[12])(int speedMMPS);	
carAction[&Omni4WD::STAT_UNKNOWN]=&Omni4WD::setcarStop;	// STAT_UNKNOWN
carAction[&Omni4WD::STAT_STOP]=&Omni4WD::setCarStop;
carAction[&Omni4WD::STAT_ADVANCE]=&Omni4WD::setCarAdvance;
carAction[&Omni4WD::STAT_BACKOFF]=&Omni4WD::setCarBackoff;
carAction[&Omni4WD::STAT_LEFT]=&Omni4WD::setCarLeft;
carAction[&Omni4WD::STAT_RIGHT]=&Omni4WD::setCarRight;
carAction[&Omni4WD::STAT_ROTATELEFT]=&Omni4WD::setCarRotateLeft;
carAction[&Omni4WD::STAT_ROTATERIGHT]=&Omni4WD::setCarRotateRight;
carAction[&Omni4WD::STAT_UPPERLEFT]=&Omni4WD::setCarUpperLeft;
carAction[&Omni4WD::STAT_LOWERLEFT]=&Omni4WD::setCarLowerLeft;
carAction[&Omni4WD::STAT_LOWERRIGHT]=&Omni4WD::setCarLowerRight;
carAction[&Omni4WD::STAT_UPPERRIGHT]=&Omni4WD::setCarUpperRight;
 */
/*
int Omni4WDAction::(Omni4WD::*carActions[])(int speedMMPS);={	// Error?? How to keep the sequence??
	&Omni4WD::setcarStop,	// STAT_UNKNOWN
	&Omni4WD::setCarStop,
	&Omni4WD::setCarAdvance,
	&Omni4WD::setCarBackoff,
	&Omni4WD::setCarLeft,
	&Omni4WD::setCarRight,
	&Omni4WD::setCarRotateLeft,
	&Omni4WD::setCarRotateRight,
	&Omni4WD::setCarUpperLeft,
	&Omni4WD::setCarLowerLeft,
	&Omni4WD::setCarLowerRight,
	&Omni4WD::setCarUpperRight
};

int Omni4WDAction::initCarActions() {	// Error: undefined reference to "Omni4WDAction::carActions"
	carActions[&Omni4WD::STAT_UNKNOWN]=&Omni4WD::setcarStop;
	carActions[&Omni4WD::STAT_STOP]=&Omni4WD::setCarStop;
	carActions[&Omni4WD::STAT_ADVANCE]=&Omni4WD::setCarAdvance;
	carActions[&Omni4WD::STAT_BACKOFF]=&Omni4WD::setCarBackoff;
	carActions[&Omni4WD::STAT_LEFT]=&Omni4WD::setCarLeft;
	carActions[&Omni4WD::STAT_RIGHT]=&Omni4WD::setCarRight;
	carActions[&Omni4WD::STAT_ROTATELEFT]=&Omni4WD::setCarRotateLeft;
	carActions[&Omni4WD::STAT_ROTATERIGHT]=&Omni4WD::setCarRotateRight;
	carActions[&Omni4WD::STAT_UPPERLEFT]=&Omni4WD::setCarUpperLeft;
	carActions[&Omni4WD::STAT_LOWERLEFT]=&Omni4WD::setCarLowerLeft;
	carActions[&Omni4WD::STAT_LOWERRIGHT]=&Omni4WD::setCarLowerRight;
	carActions[&Omni4WD::STAT_UPPERRIGHT]=&Omni4WD::setCarUpperRight;
	return 0;
}
 */

// static variable
Omni4WD* Omni4WDAction::_Omni=NULL;
Omni4WDAction* Omni4WDAction::_firstAction=NULL;
unsigned char Omni4WDAction::_nextID=0;	// next ID available
/*
Omni4WDAction::Omni4WDAction(Omni4WD* Omni) {
	setOmni4WD(Omni);
	reset();
}
 */
Omni4WDAction::Omni4WDAction(Omni4WD* Omni,unsigned char carStat,int speed,unsigned int duration, unsigned int uptime) {
	setOmni4WD(Omni);
	//setStat(STAT_QUEUING);
	
	reset();
	modify(carStat,speed,duration,uptime);
	setFirstAction(this);
}
Omni4WDAction::Omni4WDAction(unsigned char carStat,int speed,unsigned int duration, unsigned int uptime) {
	//setStat(STAT_QUEUING);
	reset();
	modify(carStat,speed,duration,uptime);
	setFirstAction(this);	// ??
}
Omni4WDAction::~Omni4WDAction()
{
}
Omni4WD* Omni4WDAction::getOmni4WD() {
	return _Omni;
}
Omni4WD* Omni4WDAction::setOmni4WD(Omni4WD* Omni) {
	if(Omni) _Omni=Omni;
	return getOmni4WD();
}
Omni4WDAction* Omni4WDAction::getFirstAction() {
	return _firstAction;
}
Omni4WDAction* Omni4WDAction::setFirstAction(Omni4WDAction* action) {
	if(action) {
		if(getFirstAction()==NULL) _firstAction=action;
		else getFirstAction()->add(action);
		return action;
	}
	return getFirstAction();
}
/*
Omni4WDAction* Omni4WDAction::setFirstAction(Omni4WDAction* action) {
	if(getFirstAction()==NULL && action) _firstAction=action;
	return getFirstAction();
}
 */
Omni4WDAction* Omni4WDAction::getNextAction() {
	return _nextAction;
}
Omni4WDAction* Omni4WDAction::setNextAction(Omni4WDAction* action) {
	_nextAction=action;
	return getNextAction();
}

unsigned char Omni4WDAction::init() {
	updatePrioAll();
	setID(getNextID());
	incNextID();
	setCarStat(Omni4WD::STAT_STOP);
	setCarSpeedMMPS(0);
	setUptime(0);
	setDuration(0);
	setPrio(0);
	setStat(STAT_UNKNOWN);
	return getID();
}
unsigned char Omni4WDAction::reset() {
	setNextAction(NULL);
	return init();
}

unsigned char Omni4WDAction::getNextID() {
	return _nextID;
}
unsigned char Omni4WDAction::setNextID(unsigned char id) {
	if(id<=MAXID) _nextID=id;
	else resetNextID();
	return getNextID();
}
unsigned char Omni4WDAction::incNextID() {
	return setNextID(getNextID()+1);
}
unsigned char Omni4WDAction::decNextID() {
	return setNextID(getNextID()-1);
}
unsigned char Omni4WDAction::resetNextID() {
	_nextID=0;
	return getNextID();
}
unsigned char Omni4WDAction::getID() const {
	return _id;
}
unsigned char Omni4WDAction::setID(unsigned char id) {
	if(id<=MAXID) _id=id;
	else id=0;
	return getID();
}
unsigned char Omni4WDAction::getCarStat() const {
	return _carStat;
}
unsigned char Omni4WDAction::setCarStat(unsigned char carStat) {
	if(Omni4WD::STAT_UNKNOWN<=_carStat && _carStat<=Omni4WD::STAT_UPPERRIGHT)
		_carStat=carStat;
	else _carStat=Omni4WD::STAT_UNKNOWN;
	return getCarStat();
}
int Omni4WDAction::getCarSpeedMMPS() const {
	return _speedMMPS;
}
int Omni4WDAction::setCarSpeedMMPS(int speedMMPS) {
	_speedMMPS=speedMMPS;
	if(_speedMMPS>MAXSPEED) _speedMMPS=MAXSPEED;
	else if(_speedMMPS<-MAXSPEED) _speedMMPS=-MAXSPEED;
	return getCarSpeedMMPS();
}
unsigned int Omni4WDAction::getUptime() const {
	return _uptime;
}
unsigned int Omni4WDAction::setUptime(unsigned int uptime) {
	return _uptime=uptime;
}
unsigned int Omni4WDAction::getDuration() const {
	return _duration;
}
unsigned int Omni4WDAction::setDuration(unsigned int duration) {
	return _duration=duration;
}
unsigned char Omni4WDAction::getPrio() const {
	return _prio;
}
unsigned char Omni4WDAction::setPrio(unsigned char prio) {
	if(prio<=MAXPRIO) _prio=prio;
	else _prio=MAXPRIO;
	return getPrio();
}
unsigned char Omni4WDAction::incPrio() {
	if(getPrio()<MAXPRIO) setPrio(getPrio()+1);
	else setPrio(MAXPRIO);
	return getPrio();
}
unsigned char Omni4WDAction::decPrio() {
	if(getPrio()>0) setPrio(getPrio()-1);
	else setPrio(0);
	return getPrio();
}
unsigned char Omni4WDAction::updatePrioAll() {
	unsigned char count=0;
	for(Omni4WDAction* action=getFirstAction();action!=NULL;action=action->getNextAction()) {
		if(action->getStat()==STAT_QUEUING || action->getStat()==STAT_HALTED)
			action->incPrio();
			++count;
	}
	return count;
}

unsigned char Omni4WDAction::getStat() const {
	return _stat;
}
unsigned char Omni4WDAction::setStat(unsigned char stat) {
	if(STAT_UNKNOWN<=stat && stat<=STAT_HALTED) _stat=stat;
	else _stat=STAT_UNKNOWN;
	return getStat();
}
unsigned char Omni4WDAction::markQueuing() {
	return setStat(STAT_QUEUING);
}
unsigned char Omni4WDAction::markActing() {
	return setStat(STAT_ACTING);
}
unsigned char Omni4WDAction::markDone() {
	return setStat(STAT_DONE);
}
unsigned char Omni4WDAction::markDel() {
	return setStat(STAT_UNKNOWN);
}

Omni4WDAction* Omni4WDAction::findActing() {
	for(Omni4WDAction* action=getFirstAction();action!=NULL;action=action->getNextAction()) {
		if(action->getStat()==STAT_ACTING)
			return action;
	}
	return NULL;
}
Omni4WDAction* Omni4WDAction::find1stPrio() {
	Omni4WDAction* exec=getFirstAction();
	// find the highest priority one
	for(Omni4WDAction* action=exec->getNextAction();action!=NULL;action=action->getNextAction()) { 
		if(exec->getStat()!=STAT_QUEUING || action->getStat()==STAT_QUEUING && action->getPrio()>exec->getPrio())
			exec=action;
	}
	if(exec->getStat()==STAT_QUEUING) {
		return exec;
	}
	return NULL;
}
Omni4WDAction* Omni4WDAction::findNExec() {
	Omni4WDAction* exec=find1stPrio();
	if(exec) exec->exec();
	return exec;
}
Omni4WDAction* Omni4WDAction::findNReuse(unsigned char carStat,int speedMMPS,unsigned int duration,unsigned int uptime) {
	for(Omni4WDAction* action=getFirstAction();action!=NULL;action=action->getNextAction()) {
		unsigned char stat=action->getStat();
		if(action == getFirstAction()) continue;
		if(stat!=STAT_QUEUING && stat!=STAT_ACTING && stat!=STAT_HALTED) {
			action->init();
			action->modify(carStat,speedMMPS,duration,uptime);
			return action;
		}
	}
	return NULL;	// Failed
}

// Arduino doesn't support malloc()!!!
Omni4WDAction* Omni4WDAction::add(Omni4WDAction* action) {
	for(Omni4WDAction* act=getFirstAction();act!=NULL;act=act->getNextAction()) {
		if(act==action) break;
		if(act->getNextAction()==NULL) return act->setNextAction(action);
	}
	return NULL;
}
/*
Omni4WDAction* Omni4WDAction::add(Omni4WDAction& action) {
	return add(&action);
}
 */
unsigned char Omni4WDAction::modify(unsigned char carStat,int speedMMPS,unsigned int duration,unsigned int uptime) {
	setCarStat(carStat);
	setCarSpeedMMPS(speedMMPS);
	setUptime(uptime);
	setDuration(duration);
	return setStat(STAT_QUEUING);
}
unsigned char Omni4WDAction::del() {
	return setStat(STAT_UNKNOWN);
}
unsigned char Omni4WDAction::delAll() {
	for(Omni4WDAction* action=getFirstAction();action!=NULL;action=action->getNextAction())
		action->del();
	return 0;
}
unsigned char Omni4WDAction::halt() {
	unsigned char stat=getStat();
	if(stat==STAT_QUEUING || stat==STAT_ACTING)
		return setStat(STAT_HALTED);
	else return stat;
}
unsigned char Omni4WDAction::haltAll() {
	unsigned char count=0;	// How many actions to be halted
	for(Omni4WDAction* action=getFirstAction();action!=NULL;action=action->getNextAction()) { 
		if(action->halt()==STAT_HALTED) ++count;
	}
	return count;
}
unsigned char Omni4WDAction::active() {
	if(getStat()==STAT_HALTED) setStat(STAT_QUEUING);
	return getStat();
}
unsigned char Omni4WDAction::activeAll() {
	unsigned char count=0;	// How many actions to be activated
	for(Omni4WDAction* action=getFirstAction();action!=NULL;action=action->getNextAction()) { 
		if(action->getStat()==STAT_HALTED && action->active()) ++count;
	}
	return count;
}
unsigned char Omni4WDAction::countActive() const {
	unsigned char count=0;
	for(Omni4WDAction* action=getFirstAction();action!=NULL;action=action->getNextAction()) {
		if(action->getStat()==STAT_QUEUING || action->getStat()==STAT_ACTING) ++count;
	}
	return count;
}
unsigned char Omni4WDAction::countAll() const {
	unsigned char count=0;
	for(Omni4WDAction* action=getFirstAction();action!=NULL;action=action->getNextAction())
		++count;		
	return count;
}

void Omni4WDAction::debugger() const {
	for(Omni4WDAction* action=getFirstAction();action!=NULL;action=action->getNextAction()) {
		Serial.print("ID: ");
		Serial.print(action->getID(),DEC);
		Serial.print("  Car: ");
		Serial.print(action->getCarStat(),DEC);
		Serial.print("  SP: ");
		Serial.print(action->getCarSpeedMMPS(),DEC);
		Serial.print("  Pr: ");
		Serial.print(action->getPrio(),DEC);
		Serial.print("  Stat: ");
		Serial.println(action->getStat(),DEC);
	}
}

void Omni4WDAction::delAction(bool isAll)
{
	for(Omni4WDAction* pAct = getFirstAction();pAct != NULL; )
	{
		if(pAct->getNextAction() == NULL) {return;}
		if(!isAll)
		{
			if(pAct->getNextAction()->getStat() != STAT_DONE) 
			{
				pAct = pAct->getNextAction();continue;
			}
		}
		Omni4WDAction* temp = pAct->getNextAction();
		pAct->setNextAction(pAct->getNextAction()->getNextAction());
		delete temp;
	}
}

bool Omni4WDAction::isNull()
{
	if(getFirstAction()->getNextAction() == NULL)
	return true;
	else return false;
}

unsigned char Omni4WDAction::Kill(bool b_SlowStop)
{
	if(b_SlowStop)
	{
		getOmni4WD()->setCarSlow2Stop(getUptime());
	}
	getOmni4WD()->setCarStop(0);
	getOmni4WD()->PIDRegulate();
	Serial.println("Killed");
	return setStat(STAT_DONE);
}

unsigned char Omni4WDAction::exec() {
	if(_endTime < millis())
	{
		Serial.println("Time out ,Kill");
		return Kill(true);
	}
	getOmni4WD()->PIDRegulate();
	return getStat();
}

unsigned char Omni4WDAction::Start()
{
	if(getStat()!=STAT_QUEUING) return getStat();
	switch(getCarStat()) {
		case Omni4WD::STAT_UNKNOWN:
		case Omni4WD::STAT_STOP:
			getOmni4WD()->setCarStop(0); break;
		case Omni4WD::STAT_ADVANCE:
			getOmni4WD()->setCarAdvance(0); break;
		case Omni4WD::STAT_BACKOFF:
			getOmni4WD()->setCarBackoff(0); break;
		case Omni4WD::STAT_LEFT:
			getOmni4WD()->setCarLeft(0); break;
		case Omni4WD::STAT_RIGHT:
			getOmni4WD()->setCarRight(0); break;
		case Omni4WD::STAT_ROTATELEFT:
			getOmni4WD()->setCarRotateLeft(0); break;
		case Omni4WD::STAT_ROTATERIGHT:
			getOmni4WD()->setCarRotateRight(0); break;
		case Omni4WD::STAT_UPPERLEFT:
			getOmni4WD()->setCarUpperLeft(0); break;
		case Omni4WD::STAT_LOWERLEFT:
			getOmni4WD()->setCarLowerLeft(0); break;
		case Omni4WD::STAT_LOWERRIGHT:
			getOmni4WD()->setCarLowerRight(0); break;
		case Omni4WD::STAT_UPPERRIGHT:
			getOmni4WD()->setCarUpperRight(0); break;
		default:
			getOmni4WD()->setCarStop(0); break;
	}
	if(getDuration()==0) return setStat(STAT_DONE);
	_startTime = millis();
	_endTime = _startTime + (unsigned long)getDuration();
	getOmni4WD()->setCarSpeedMMPS(getCarSpeedMMPS(),getUptime());
	setStat(STAT_ACTING);
}



