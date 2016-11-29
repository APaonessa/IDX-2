#include "Facility.h"
#include <string>
#include <sstream>
#include <ctime>;


Facility::Facility(int facil, std::string in, boost::variate_generator< boost::mt19937&, boost::random::triangle_distribution < > > * triangle, boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > * uniform, int incdist, double upinc, double lowinc, int infdist, double upinf, double lowinf)
{
	facility = facil;
	std::string input[11];
	int i = 0;
	std::stringstream ssin(in);
	while (ssin.good() && i < 11) {
		ssin >> input[i];
		++i;
	}

	name = input[0];
	subfacil = stoi(input[1]);
	startS = stoi(input[2]);
	startE = stoi(input[3]);
	startI = stoi(input[4]);
	bed_size = stoi(input[5]);
	prev = stod(input[6]);
	inc = stod(input[7]);
	LOS_mean = stod(input[8]);
	LOS_dev = stod(input[9]);
	LOS_dist = stoi(input[10]);

	RNGpoint = uniform;
	triangleRNGpoint = triangle;

	genpoint = new boost::mt19937 (time(0));	//create RNG
	boost::random::lognormal_distribution< > lognormalDistribution(LOS_mean, LOS_dev);
	boost::random::normal_distribution< > normalDistribution(LOS_mean, LOS_dev);
	lognormRNGpoint = new boost::variate_generator< boost::mt19937&, boost::random::lognormal_distribution < > >(*genpoint, lognormalDistribution);
	normalRNGpoint = new boost::variate_generator< boost::mt19937&, boost::random::normal_distribution < > >(*genpoint, normalDistribution);

	inf_dist = infdist;
	inc_dist = incdist;
	lower_inf = lowinf;
	upper_inf = upinf;
	lower_inc = lowinc;
	upper_inc = upinc;


	//initialize agents
	for (int i = 0; i < startS; i++) {
		Agent * a = new Agent();
		(*a).setState(0);
		(*a).setEI(-1);
		(*a).setIS(-1);
		(*a).setDischarge(LOS());
		(*a).setFacility(facility);
		agents.push_back(a);
	}
	for (int i = 0; i < startE; i++) {
		Agent * a = new Agent();
		(*a).setState(1);
		if (inc_dist == 1 || inc_dist == 0) {		//uniform or single value
			(*a).setEI(ceil(randUniform()*(upper_inc - lower_inc)) + lower_inc);
		}
		else if (inc_dist == 2) {	//triangular
			(*a).setEI(ceil(randTriangle()*(upper_inc - lower_inc)) + lower_inc);
		}
		(*a).setIS(-1);
		(*a).setDischarge(LOS());
		(*a).setFacility(facility);
		agents.push_back(a);
	}
	for (int i = 0; i < startI; i++) {
		Agent * a = new Agent();
		(*a).setState(2);
		(*a).setEI(-1);
		if (inf_dist == 1 || inf_dist == 0) {		//uniform or single value
			(*a).setIS(ceil(randUniform()*(upper_inf - lower_inf)) + lower_inf);
		}
		else if (inf_dist == 2) {	//triangular
			(*a).setIS(ceil(randTriangle()*(upper_inf - lower_inf)) + lower_inf);
		}
		(*a).setDischarge(LOS());
		(*a).setFacility(facility);
		agents.push_back(a);
	} 

}

Facility::~Facility()
{
}

int Facility::countSus() {
	int count = 0;
	for (Agent* a : agents) {
		if ((*a).getState() == 0) {
			count++;
		}
	}
	return count;
}

int Facility::countExp() {
	int count = 0;
	for (Agent* a : agents) {
		if ((*a).getState() == 1) {
			count++;
		}
	}
	return count;
}

int Facility::countInf() {
	int count = 0;
	for (Agent* a : agents) {
		if ((*a).getState() == 2) {
			count++;
		}
	}
	return count;
}

double Facility::randUniform()
{
	boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > RNG = *RNGpoint;
	return RNG();
}

double Facility::randTriangle()
{
	boost::variate_generator< boost::mt19937&, boost::random::triangle_distribution < > > RNG = *triangleRNGpoint;
	return RNG();
}

void Facility::addS(int num)
{
	s_Data.push_back(num);
}

void Facility::addE(int num)
{
	e_Data.push_back(num);
}

void Facility::addI(int num)
{
	i_Data.push_back(num);
}

void Facility::updateS(int index, int num)
{
	s_Data[index] = s_Data[index] + num;
}

void Facility::updateE(int index, int num)
{
	e_Data[index] = e_Data[index] + num;
}

void Facility::updateI(int index, int num)
{
	i_Data[index] = i_Data[index] + num;
}

int Facility::getS(int index)
{
	return s_Data[index];
}

int Facility::getE(int index)
{
	return e_Data[index];
}

int Facility::getI(int index)
{
	return i_Data[index];
}


void Facility::dynamics(double beta, int timeStep)
{
	double probability = 1 - pow(1 - beta, countInf());
	std::vector<double> probs = { probability, (1 - probability) };

	for (Agent* a : agents)
	{
		if ((*a).getState() == 0) {	//sus to exp
			if (randProb(probs) == 0) {
				(*a).setState(1);
			}
		}
		else if ((*a).getState() == 1) {	//exp to inf
			if ((*a).getEI() == -1) {
				if (inc_dist == 1 || inc_dist == 0) {		//uniform or single value
					(*a).setEI(timeStep + ceil(randUniform()*(upper_inc - lower_inc)) + lower_inc);
				}
				else if (inc_dist == 2) {		//triangular
					(*a).setEI(timeStep + ceil(randTriangle()*(upper_inc - lower_inc)) + lower_inc);
				}
			}
			else if (timeStep == (*a).getEI()) {
				(*a).setState(2);
				(*a).setEI(-1);
			}
		}
		else if ((*a).getState() == 2) {	//inf to sus
			if ((*a).getIS() == -1) {
				if (inf_dist == 1 || inf_dist == 0) {		//uniform or single value
					(*a).setIS(timeStep + ceil(randUniform()*(upper_inf - lower_inf)) + lower_inf);
				}
				else if (inf_dist == 2) {		//triangular
					(*a).setIS(timeStep + ceil(randTriangle()*(upper_inf - lower_inf)) + lower_inf);
				}
			}
			else if (timeStep == (*a).getIS()) {
				(*a).setState(0);
				(*a).setIS(-1);
			}
		}
	}

}

int Facility::randProb(std::vector<double> probs)
{
	double currProb = probs[0];
	double rand = randUniform();
	for (int i = 0; i<(probs.size() - 1); i++) {
		if (rand<currProb) {
			return i;				//return index of drawn probability
		}
		currProb = currProb + probs[i + 1];
	}
	return probs.size() - 1;
}

void Facility::reset() {
	agents.clear();
	//reinitialize agents
	for (int i = 0; i < startS; i++) {
		Agent * a = new Agent();
		(*a).setState(0);
		(*a).setEI(-1);
		(*a).setIS(-1);
		(*a).setDischarge(LOS());
		(*a).setFacility(facility);
		agents.push_back(a);
	}
	for (int i = 0; i < startE; i++) {
		Agent * a = new Agent();
		(*a).setState(1);
		if (inc_dist == 1 || inc_dist == 0) {		//uniform or single value
			(*a).setEI(ceil(randUniform()*(upper_inc - lower_inc)) + lower_inc);
		}
		else if (inc_dist == 2) {	//triangular
			(*a).setEI(ceil(randTriangle()*(upper_inc - lower_inc)) + lower_inc);
		}
		(*a).setIS(-1);
		(*a).setDischarge(LOS());
		(*a).setFacility(facility);
		agents.push_back(a);
	}
	for (int i = 0; i < startI; i++) {
		Agent * a = new Agent();
		(*a).setState(2);
		(*a).setEI(-1);
		if (inf_dist == 1 || inf_dist == 0) {		//uniform or single value
			(*a).setIS(ceil(randUniform()*(upper_inf - lower_inf)) + lower_inf);
		}
		else if (inf_dist == 2) {	//triangular
			(*a).setIS(ceil(randTriangle()*(upper_inf - lower_inf)) + lower_inf);
		}
		(*a).setDischarge(LOS());
		(*a).setFacility(facility);
		agents.push_back(a);
	}
}

double Facility::LOS() {
	if (LOS_dist == 0) {	//normal
		boost::variate_generator< boost::mt19937&, boost::random::normal_distribution < > > normRNG = *normalRNGpoint;
		return normRNG();
	}
	else if (LOS_dist == 1) {		//lognormal DOESNT WORK
		boost::variate_generator< boost::mt19937&, boost::random::lognormal_distribution < > > logRNG = *lognormRNGpoint;
		return logRNG();
	}
}

std::vector<Agent*> Facility::removePatients(int timestep) {
	std::vector<Agent*> removed;
	for (int i = agents.size() - 1; i >= 0; i--) {
		if (timestep >= (*agents.at(i)).getDischarge()) {
			removed.push_back(agents.at(i));
			agents.erase(agents.begin() + i);
		}
	}
	return removed;
}

void Facility::addPatient(Agent* a, int timestep) {
	(*a).setFacility(facility);
	(*a).setSubFacility(subfacil);
	(*a).setDischarge(LOS() + timestep);
	agents.push_back(a);
}