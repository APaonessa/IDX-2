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

int Agent::getEI() {
	return EItransition;
}

void Agent::setEI(int day) {
	EItransition = day;
}

int Agent::getIS() {
	return IStransition;
}

void Agent::setIS(int day) {
	IStransition = day;
}