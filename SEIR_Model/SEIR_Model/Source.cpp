#include <iostream>; 
#include <ctime>;
#include <math.h>;
#include <vector>;
#include <fstream>;
#include <string>; 
#include <ios>; 
using namespace std;

int randProb(int div, double probs[])
{
	double randNum = (double)rand() / RAND_MAX;	//random number between 0 and 1
	
	cout << "randNum = ";
	cout << randNum;
	cout << "\n";
	
	double currProb = probs[0];
		for(int i = 0; i<div-1; i++) {
			if (randNum<currProb) {
				return i;				//return index of drawn probability
			}
			currProb = currProb + probs[i + 1];
		}

	return div - 1;
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
	srand(time(NULL));	//seed rand()
	double input[4] = { 0.2, 0.3, 0.1, 0.4 };
	for (int i = 0; i < 10; i++) {
		cout << randProb(4, input);
		cout << "\n\n";
	}






	/*
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

				vector<int> sData;
				vector<int> eData;
				vector<int> iData;

				sData.push_back(susceptible);
				eData.push_back(exposed);
				iData.push_back(infected);
				outputFile << "Day, Suceptible, Exposed, Infected, Total\n"; 
				outputFile << "0, " << sData[0] << ", " << eData[0] << ", " << iData[0] << ", " << sData[0]+eData[0]+iData[0] << "\n";

				srand(time(0));	//seed random number generator
				
				for (int i = 0; i < endtime; i = i + time_step)
				{
					double beta = reproductive_rate / (infected_time * (susceptible + exposed + infected));
					int difference1 = susToExp(beta, infected, susceptible);
					int difference2 = expToInf(incubation_time, time_step, exposed);
					int difference3 = infToSus(infected_time, time_step, infected);

					susceptible = susceptible + difference3 - difference1;
					exposed = exposed + difference1 - difference2;
					infected = infected + difference2 - difference3;

					sData.push_back(susceptible);
					eData.push_back(exposed);
					iData.push_back(infected);

					outputFile << i + 1 << ", " << sData[i] << ", " << eData[i] << ", " << iData[i] << ", " << sData[i] + eData[i] + iData[i] << "\n";
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
	}*/
	return 0;
}