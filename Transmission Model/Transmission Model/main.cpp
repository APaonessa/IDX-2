#include <iostream>
#include <ctime>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;

int susToExp(double beta, int infected, int susceptible)
{
	int count = 0; //counts number of people that need to transition from S to E
	double probability = 1 - pow(1 - beta, infected);
	int randNum;

	//an iteration for each susceptible person
	for (int i = 0; i < susceptible; i++)
	{
		randNum = rand() % 1000;
		if (randNum < probability * 1000.0)
		{
			count++;
		}
	}
	return count;
}

int expToInf(double incubationTime, int timeStep, int exposed)
{
	int count = 0; //counts number of people that need to transition from E to I
	double probability = timeStep / incubationTime;
	int randNum;

	//an iteration for each exposed person
	for (int i = 0; i < exposed; i++)
	{
		randNum = rand() % 1000;
		if (randNum <= probability * 1000.0)
		{
			count++;
		}
	}
	return count;
}

int infToSus(double infectedTime, int timeStep, int infected)
{
	int count = 0; //counts number of people that need to transition from I to S
	double probability = timeStep / infectedTime;
	int randNum;

	//an iteration for each infected person
	for (int i = 0; i < infected; i++)
	{
		randNum = rand() % 1000;
		if (randNum < probability * 1000.0)
		{
			count++;
		}
	}
	return count;
}


int main()
{
	int susceptible = 99;	//input
	int exposed = 0;	//input
	int infected = 1;	//input
	int endTime = 365;	//input
	int timeStep = 1;	//input
	double incubationTime = 2.0; //input
	double infectedTime = 14.0;	//input
	double reproductiveRate = 1.5; //input
	string outputFileName = "output.txt"; //input

	ofstream outputFile;
	outputFile.open(outputFileName);

	vector<int> sData;
	vector<int> eData;
	vector<int> iData;

	sData.push_back(susceptible);
	eData.push_back(exposed);
	iData.push_back(infected);

	outputFile << "0, " << sData[0] << ", " << eData[0] << ", " << iData[0] << ", \n";

	srand(time(0));	//seed random number generator

	for (int i = 0; i < endTime; i = i + timeStep)
	{
		double beta = reproductiveRate / (infectedTime * (susceptible + exposed + infected));
		int difference1 = susToExp(beta, infected, susceptible);
		int difference2 = expToInf(incubationTime, timeStep, exposed);
		int difference3 = infToSus(infectedTime, timeStep, infected);

		susceptible = susceptible + difference3 - difference1;
		exposed = exposed + difference1 - difference2;
		infected = infected + difference2 - difference3;

		sData.push_back(susceptible);
		eData.push_back(exposed);
		iData.push_back(infected);

		outputFile << i + 1 << ", " << sData[i] << ", " << eData[i] << ", " << iData[i] << ", \n";
	}

	outputFile.close();

	return 0;
}