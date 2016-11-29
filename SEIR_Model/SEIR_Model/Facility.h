#pragma once
#include <vector>
#include "Agent.h"
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <memory>

class Facility
{
public:
	Facility(int facil, std::string input, boost::variate_generator< boost::mt19937&, boost::random::triangle_distribution < > > * triangle, boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > * uniform, int incdist, double upinc, double lowinc, int infdist, double upinf, double lowinf);
	~Facility();
	int countSus();
	int countExp();
	int countInf();
	void addS(int num);
	void addE(int num);
	void addI(int num);
	void updateS(int index, int num);
	void updateE(int index, int num);
	void updateI(int index, int num);
	int getS(int index);
	int getE(int index);
	int getI(int index);
	void dynamics(double beta, int timeStep);
	void reset();
	double LOS();
	std::vector<Agent*> removePatients(int timestep);
	void addPatient(Agent* a, int timestep);

private:
	std::string name;
	std::vector<Agent*> agents;
	int facility;
	int subfacil;
	int bed_size;
	double prev;
	double inc;
	double LOS_mean;
	double LOS_dev;
	int LOS_dist;
	boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > *RNGpoint;
	boost::variate_generator< boost::mt19937&, boost::random::triangle_distribution < > > * triangleRNGpoint;
	boost::variate_generator< boost::mt19937&, boost::random::lognormal_distribution < > > * lognormRNGpoint;
	boost::variate_generator< boost::mt19937&, boost::random::normal_distribution < > > * normalRNGpoint;
	boost::mt19937 * genpoint;
	int inc_dist;
	int inf_dist;
	double upper_inc;
	double lower_inc;
	double upper_inf;
	double lower_inf;
	double randUniform();
	double randTriangle();
	int startS;
	int startI;
	int startE;
	std::vector<int> s_Data;
	std::vector<int> e_Data;
	std::vector<int> i_Data;
	int randProb(std::vector<double> probs);
};

