#pragma once
class Agent
{
public:
	Agent();
	~Agent();
	int getState();
	void setState(int s);
	int getTimeExp();
	void setTimeExp(int day);
	int getTimeInf();
	void setTimeInf(int day);
private:
	int diseaseState;
	int timeExp;	//what day were they exposed?
	int timeInf;	//what day do they become infectious?
};

