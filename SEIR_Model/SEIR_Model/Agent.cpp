#include "Agent.h"



Agent::Agent()
{
}

Agent::~Agent()
{
}

int Agent::getState() {
	return diseaseState;
}

void Agent::setState(int s) {
	diseaseState = s;
}

int Agent::getTimeExp() {
	return timeExp;
}

void Agent::setTimeExp(int day) {
	timeExp = day;
}

int Agent::getTimeInf() {
	return timeInf;
}

void Agent::setTimeInf(int day) {
	timeInf = day;
}