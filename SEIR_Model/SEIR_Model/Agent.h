#pragma once
class Agent
{
public:
	Agent();
	~Agent();
	int getState();
	void setState(int s);
	int getEI();
	void setEI(int day);
	int getIS();
	void setIS(int day);
	int getFacility();
	void setFacility(int facil);
	int getSubFacility();
	void setSubFacility(int subfacil);
	int LOS(int facility, int sub, int state);
private:
	int diseaseState;
	int EItransition;	//what day will they transition from E to I?
	int IStransition;	//what day will they transition from I to S?
	int facility;
	int subFacility;
};

