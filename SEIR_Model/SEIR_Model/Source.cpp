#include <iostream>
#include <ctime>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>; 
#include <ios>; 
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
cout << "\n";
cout << "Example of txt file: \n";
cout << "1000 0 1";
cout << "\n";
cout << "2.0 14.0 1.5";
cout << "\n";
cout << "365 1";
cout << "\n";
//cout << "Line 3: time step you want data displayed at";
}

void outputInfo(int susceptible, int infected, int exposed, double infected_time, int time_step, double incubation_time, double reproductive_rate, int endtime)
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
}

int main()
{
	int n = 0;
	int runagain = 0;
	string inputInformation[8];
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

				//prints out the data that is read in file as a check... 
				outputInfo(susceptible, infected, exposed, infected_time, time_step, incubation_time, reproductive_rate, endtime); 

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
				//fstream myfile;

				string outputFileName = output_textfile; //input

				ofstream outputFile;
				outputFile.open(outputFileName);

				vector<int> sData;
				vector<int> eData;
				vector<int> iData;

				sData.push_back(susceptible);
				eData.push_back(exposed);
				iData.push_back(infected);
				outputFile << "Day, Suceptible, Exposed, Infected\n"; 
				outputFile << "0, " << sData[0] << ", " << eData[0] << ", " << iData[0] << " \n";

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

					outputFile <<  i + 1 << ", " << sData[i] << ", " << eData[i] << ", " << iData[i] << " \n";
				}
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