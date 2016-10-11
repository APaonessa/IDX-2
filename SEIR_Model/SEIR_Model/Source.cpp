#include <iostream>; 
#include <ctime>;
#include <math.h>;
#include <vector>;
#include <fstream>;
#include <string>; 
#include <ios>; 
#include "Agent.h";
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
using namespace std;

int countInfected(vector<Agent*> agents) {
	int count = 0;
	for (Agent* a : agents) {
		if ((*a).getState() == 2) {
			count++;
		}
	}
	return count;
}

int countExposed(vector<Agent*> agents) {
	int count = 0;
	for (Agent* a : agents) {
		if ((*a).getState() == 1) {
			count++;
		}
	}
	return count;
}

int countSus(vector<Agent*> agents) {
	int count = 0;
	for (Agent* a : agents) {
		if ((*a).getState() == 0) {
			count++;
		}
	}
	return count;
}

int randProb(vector<double> probs, boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > * RNGpoint)
{
	//double randNum = (double)rand() / RAND_MAX;	//random number between 0 and 1
	boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > RNG = *RNGpoint;
	double randNum = RNG();

	double currProb = probs[0];
		for(int i = 0; i<(probs.size()-1); i++) {
			if (randNum<currProb) {
				return i;				//return index of drawn probability
			}
			currProb = currProb + probs[i + 1];
		}

	return probs.size() - 1;
}

double summary_statistics(vector<int>& inputdata, int amt_time, int previous_time)
{
	double total = 0;  
	double avg = 0; 
	for (int i = (previous_time-1); i < (amt_time) ; i++)
	{
		
		total = inputdata[i] + total;
		
	}
	avg = total / (amt_time - (previous_time-1));
	return avg; 
}


//combines susToExp, expToInf, and infToSus
void transmission(vector<Agent*> agents, double beta, double incubationTime, double infectedTime, int timeStep, boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > * RNGpoint)
{
	double probability = 1 - pow(1 - beta, countInfected(agents));
	vector<double> probs = { probability, (1 - probability) };

	for (Agent* a : agents)
	{
		if ((*a).getState() == 0) {
			if (randProb(probs, RNGpoint) == 0) {
				(*a).setState(1);
				(*a).setTimeExp(timeStep);
			}
		}
		else if ((*a).getState() == 1) {
			if (timeStep - (*a).getTimeExp() >= incubationTime) {
				(*a).setState(2);
				(*a).setTimeInf(timeStep);
			}
		}
		else if ((*a).getState() == 2) {
			if (timeStep - (*a).getTimeInf() >= infectedTime) {
				(*a).setState(0);
			}
		}
	}



}

void inputInfo()
{
//Information for formatting the text file...
cout << "To run this program input a .txt file in this form...\n";
cout << "(Do not use commas to separate numbers)\n";
cout << "\n";
cout << "Line 1: # of Susceptible, # of Exposed, # of Infected\n";
cout << "Line 2: Incubation time, infected time, reproductive rate\n";
cout << "\n";
cout << "Line 3: Length of time model runs for, time step for printing out data";
cout << "\n";
cout << "\n";
cout << "Line 4: Input 4 #'s for summary statistics"; 
cout << "\n";
cout << "\n"; 
cout << "Example of txt file: \n";
cout << "1000 0 1";
cout << "\n";
cout << "2.0 14.0 1.5";
cout << "\n";
cout << "365 1";
cout << "\n";
cout << "7 14 30 60"; 
cout << "\n"; 
cout << "\n"; 
}

void outputInfo(int susceptible, int infected, int exposed, double infected_time, int time_step, double incubation_time, double reproductive_rate, int endtime, int summary_statistic1, int summary_statistic2, int summary_statistic3, int summary_statistic4)
{
cout << "susceptible: ";
cout << susceptible;
cout << "\n";
cout << "exposed: ";
cout << exposed;
cout << "\n";
cout << "infected: ";
cout << infected;
cout << "\n";
cout << "incubation time: ";
cout << incubation_time;
cout << "\n";
cout << "infected time: ";
cout << infected_time;
cout << "\n";
cout << "reproductive rate: ";
cout << reproductive_rate;
cout << "\n";
cout << "length of simulation: ";
cout << endtime;
cout << "\n";
cout << "time step: ";
cout << time_step;
cout << "\n";
cout << "summary statistic 1: "; 
cout <<  summary_statistic1; 
cout << "\n"; 
cout << "summary statistic 2: "; 
cout << summary_statistic2; 
cout << "\n"; 
cout << "summary statistic 3: "; 
cout << summary_statistic3; 
cout << "\n"; 
cout << "summary statistic 4: "; 
cout << summary_statistic4; 
cout << "\n"; 
}

int main()
{
	
	int n = 0;
	int runagain = 0;
	string inputInformation[12];
	char timestep_graph;

	inputInfo(); 


	while (runagain > -1)
	{
		//executes loop if the input fails (e.g., no characters were read)
		while (cout << "Would you like to run again? (yes = 1/no = 0) >>>" && !(cin >> runagain))
		{
			cin.clear(); //clear bad input flag
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
			cout << "Invalid input; please re-enter and integer choice.\n";
		}
		if (runagain > 0 && runagain < 2)
		{
			string filename;
			//Asks for filename from user
			//Could have a graphic interface? and open the file browser
			cout << "Input the name of the .txt file you wish to use ";
			cin >> filename;

			//Concatonates the filename with the .txt ending to open the file 
			string textfile = filename + ".txt";
			string line;
			//Lets the program recognize the given file 
			fstream myfile;

			//If the file exists (and in the right place) the file will open and write to the output the contents of file
			myfile.open(textfile);

			if (myfile.good())
			{
				while (!myfile.eof())
				{
					//runs through the file and populates the array with the data from the file
					myfile >> inputInformation[n];
					n++;
				}
				myfile.close();

				//switches the data from being in string form to being an integer
				int susceptible = stoi(inputInformation[0]);
				int exposed = stoi(inputInformation[1]);
				int infected = stoi(inputInformation[2]);
				int time_step = stoi(inputInformation[7]);
				int endtime = stoi(inputInformation[6]);
				double incubation_time = stod(inputInformation[3]);
				double infected_time = stod(inputInformation[4]);
				double reproductive_rate = stod(inputInformation[5]);
				int summary_statistic1 = stoi(inputInformation[8]); 
				int summary_statistic2 = stoi(inputInformation[9]); 
				int summary_statistic3 = stoi(inputInformation[10]); 
				int summary_statistic4 = stoi(inputInformation[11]); 

				//prints out the data that is read in file as a check... 
				outputInfo(susceptible, infected, exposed, infected_time, time_step, incubation_time, reproductive_rate, endtime, summary_statistic1, summary_statistic2, summary_statistic3, summary_statistic4); 

				//Give the option to run with current data or start over and enter new data
				runagain = 0;
				string output_filename;

				//Asks for filename from user
				//Could have a graphic interface? and open the file browser
				cout << "Input the name of the file data will be outputted to: ";
				cin >> output_filename;

				//Concatonates the filename with the .txt ending to open the file 
				string output_textfile = output_filename + ".txt";
				string line;
				//Lets the program recognize the given file 

				string outputFileName = output_textfile; //input

				ofstream outputFile;
				outputFile.open(outputFileName);

				//stores summary of SEI at every time interval
				vector<int> sData;
				vector<int> eData;
				vector<int> iData;

				vector<Agent*> agents;

				//initialize agents
				for (int i = 0; i < susceptible; i++) {
					Agent * a = new Agent();
					(*a).setState(0);
					agents.push_back(a);
				}
				for (int i = 0; i < exposed; i++) {
					Agent * a = new Agent();
					(*a).setState(1);
					(*a).setTimeExp(-1*(i % (int)incubation_time));	//approximately even distribution of stage of incubation
					agents.push_back(a);
				}
				for (int i = 0; i < infected; i++) {
					Agent * a = new Agent();
					(*a).setState(2);
					(*a).setTimeInf(-1*(i % (int)infected_time));		//approximately even distribution of stage of infection
					agents.push_back(a);
				}

				sData.push_back(susceptible);
				eData.push_back(exposed);
				iData.push_back(infected);
				outputFile << "Day, Suceptible, Exposed, Infected, Total\n"; 
				outputFile << "0, " << sData[0] << ", " << eData[0] << ", " << iData[0] << ", " << sData[0]+eData[0]+iData[0] << "\n";

				boost::mt19937 generator(time(0));	//create RNG
				boost::random::uniform_real_distribution< > uniformDistribution(0.0, 1.0);
				boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > >
					generateRandomNumbers(generator, uniformDistribution);
				
				boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > * RNGpoint = &generateRandomNumbers;
				
				
				//srand(time(NULL));	//seed random number generator
				
				for (int i = 1; i <= endtime; i = i + time_step)
				{
					int sus = countSus(agents);
					int exp = countExposed(agents);
					int inf = countInfected(agents);
					double beta = reproductive_rate / (infected_time * (sus + exp + inf));
					transmission(agents, beta, incubation_time, infected_time, i, RNGpoint);

					sData.push_back(countSus(agents));
					eData.push_back(countExposed(agents));
					iData.push_back(countInfected(agents));

					outputFile << i << ", " << sData[i] << ", " << eData[i] << ", " << iData[i] << ", " << sData[i] + eData[i] + iData[i] << "\n";
				}
				double summary_statistic_1_s = summary_statistics(sData, summary_statistic1, 1);
				double summary_statistic_1_e = summary_statistics(eData, summary_statistic1, 1);
				double summary_statistic_1_i = summary_statistics(iData, summary_statistic1, 1);
				double summary_statistic_2_s = summary_statistics(sData, summary_statistic2, summary_statistic1); 
				double summary_statistic_2_e = summary_statistics(eData, summary_statistic2, summary_statistic1);
				double summary_statistic_2_i = summary_statistics(iData, summary_statistic2, summary_statistic1);
				double summary_statistic_3_s = summary_statistics(sData, summary_statistic3, summary_statistic2);	
				double summary_statistic_3_e = summary_statistics(eData, summary_statistic3, summary_statistic2);
				double summary_statistic_3_i = summary_statistics(iData, summary_statistic3, summary_statistic2);
				double summary_statistic_4_s = summary_statistics(sData, summary_statistic4, summary_statistic3); 
				double summary_statistic_4_e = summary_statistics(eData, summary_statistic4, summary_statistic3);
				double summary_statistic_4_i = summary_statistics(iData, summary_statistic4, summary_statistic3);


				cout << "\n"; 
				cout << "Here is a summary of the outputed data: \n"; 
				cout << "Susceptible, Exposed, Infected \n"; 
				cout << "At 7 days: "; 
				cout << summary_statistic_1_s << ", " << summary_statistic_1_e << ", " << summary_statistic_1_i;
				cout << "\n"; 
				cout << "At 14 days: "; 
				cout << summary_statistic_2_s << ", " << summary_statistic_2_e << ", " << summary_statistic_2_i; 
				cout << "\n"; 
				cout << "At 1 month: ";
				cout << summary_statistic_3_s << ", " << summary_statistic_3_e << ", " << summary_statistic_3_i; 
				cout << "\n"; 
				cout << "At 2 months: "; 
				cout << summary_statistic_4_s << ", " << summary_statistic_4_e << ", " << summary_statistic_4_i; 
				cout << "\n";  
				outputFile.close();
			}
			//If file does not exist the program will output this and then end
			else cout << "Unable to open file.  ";
		}
		else
		{
			exit(1);
			return 0;
		} 
	}
	return 0;
}