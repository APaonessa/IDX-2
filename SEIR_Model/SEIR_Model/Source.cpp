#include <iostream>; 
#include <ctime>;
#include <math.h>;
#include <vector>;
#include <fstream>;
#include <string>; 
#include <ios>; 
#include "Agent.h";
#include "Facility.h";
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
using namespace std;

int randProb(vector<double> probs, boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > * RNGpoint)
{
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

int randProb(double probs[], int size, boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > * RNGpoint)
{
	boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > RNG = *RNGpoint;
	double randNum = RNG();

	double currProb = probs[0];
	for (int i = 0; i<(size - 1); i++) {
		if (randNum<currProb) {
			return i;				//return index of drawn probability
		}
		currProb = currProb + probs[i + 1];
	}

	return size - 1;
}


double randNum(boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > * RNGpoint)
{
	boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > RNG = *RNGpoint;
	return RNG();
}

double randNum(boost::variate_generator< boost::mt19937&, boost::random::triangle_distribution < > > * triangleRNGpoint)
{
	boost::variate_generator< boost::mt19937&, boost::random::triangle_distribution < > > RNG = *triangleRNGpoint;
	return RNG();
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

void inputInfo()
{
//Information for formatting the text file...
cout << "To run this program input a .txt file in this form...\n";
cout << "(Do not use commas to separate numbers)\n";
cout << "\n";
cout << "Line 1: Incubation time, infected time, reproductive rate\n";
cout << "\n";
cout << "Line 2: Length of time model runs for, time step for printing out data";
cout << "\n";
cout << "\n";
cout << "Line 3: Input 4 #'s for summary statistics"; 
cout << "\n";
cout << "\n"; 
cout << "Line 4: Number of simulation runs";
cout << "\n";
cout << "\n";
cout << "Example of txt file: \n";
cout << "2.0 14.0 1.5";
cout << "\n";
cout << "365 1";
cout << "\n";
cout << "7 14 30 60"; 
cout << "\n"; 
cout << "50";
cout << "\n";
cout << "\n";
}

void outputInfo(int inf_dist, double lower_inf, double upper_inf, int time_step, int inc_dist, double lower_inc, double upper_inc, double reproductive_rate, int endtime, int summary_statistic1, int summary_statistic2, int summary_statistic3, int summary_statistic4, int numOfRuns)
{
if (inc_dist == 0) {
	cout << "incubation time: ";
	cout << upper_inc;
	cout << "\n";
}
else if (inc_dist == 1) {
	cout << "incubation time: ";
	cout << lower_inc << "-" << upper_inc << " (uniform)";
	cout << "\n";
}
else if (inc_dist == 2) {
	cout << "incubation time: ";
	cout << lower_inc << "-" << upper_inc << " (triangular)";
	cout << "\n";
}

if (inf_dist == 0) {
	cout << "infected time: ";
	cout << upper_inf;
	cout << "\n";
}
else if (inf_dist == 1) {
	cout << "infected time: ";
	cout << lower_inf << "-" << upper_inf << " (uniform)";
	cout << "\n";
}
else if (inf_dist == 2) {
	cout << "infected time: ";
	cout << lower_inf << "-" << upper_inf << " (triangular)";
	cout << "\n";
}
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
cout << "number of simulation runs: ";
cout << numOfRuns;
cout << "\n";
}

void readTransferMatrix(double(*matrix)[30][30], string filename)	//num of facil must be less than 30
{
	fstream f;
	f.open(filename);
	int numOfFacil;
	f >> numOfFacil;		//first line of file is the number of facilities

	for (int i = 0; i < numOfFacil; i++) { // loop through the 1st dimension of the array
		for (int j = 0; j < numOfFacil; j++) { // loop through the 2nd dimension of the array
			f >> (*matrix)[i][j]; // read to each element of the array  
		}
	}
	f.close();

	//normalize
	for (int i = 0; i < numOfFacil; i++) {
		double sum = 0;
		for (int j = 0; j < numOfFacil; j++) {
			sum = sum + (*matrix)[i][j];
		}
		for (int j = 0; j < numOfFacil; j++) {
			(*matrix)[i][j] = (*matrix)[i][j] / sum;
		}
	}
	
}


int main()
{

	int n = 0;
	int runagain = 0;
	string inputInformation[13];
	char timestep_graph;
	int run = 0;

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
			string textfile;
			//Asks for filename from user
			//Could have a graphic interface? and open the file browser
			cout << "Input the name of the simulation info file you wish to use ";
			cin >> textfile;

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
				int time_step = stoi(inputInformation[4]);
				int endtime = stoi(inputInformation[3]);
				double upper_inc, lower_inc;
				int checkRange = inputInformation[0].find('-');
				int inc_dist, inf_dist;	//0 = single value, 1 = uniform, 2 = triangular
				if (checkRange == -1) {	//single value
					inc_dist = 0;
					lower_inc = stod(inputInformation[0]);
					upper_inc = stod(inputInformation[0]);
				}
				else {	//range of values
					lower_inc = stod(inputInformation[0].substr(0, checkRange));
					upper_inc = stod(inputInformation[0].substr(checkRange+1, inputInformation[0].size()-checkRange-1));

					cout << "\n";
					cout << "Uniform distribution (1) or triangular distribution (2) for incubation time?";
					cin >> inc_dist;
				}
				double upper_inf, lower_inf;
				checkRange = inputInformation[1].find('-');
				if (checkRange == -1) {	//single value
					inf_dist = 0;
					lower_inf = stod(inputInformation[1]);
					upper_inf = stod(inputInformation[1]);
				}
				else {	//range of values
					lower_inf = stod(inputInformation[1].substr(0, checkRange));
					upper_inf = stod(inputInformation[1].substr(checkRange + 1, inputInformation[1].size() - checkRange - 1));

					cout << "\n";
					cout << "Uniform distribution (1) or triangular distribution (2) for infected time?";
					cin >> inf_dist;

				}
				double reproductive_rate = stod(inputInformation[2]);
				int summary_statistic1 = stoi(inputInformation[5]);
				int summary_statistic2 = stoi(inputInformation[6]);
				int summary_statistic3 = stoi(inputInformation[7]);
				int summary_statistic4 = stoi(inputInformation[8]);
				int numOfRuns = stoi(inputInformation[9]);

				//prints out the data that is read in file as a check... 
				outputInfo(inf_dist, lower_inf, upper_inf, time_step, inc_dist, lower_inc, upper_inc, reproductive_rate, endtime, summary_statistic1, summary_statistic2, summary_statistic3, summary_statistic4, numOfRuns);

				string facilityfile;
				//Asks for filename from user
				cout << "Input the name of the facility file you wish to use ";
				cin >> facilityfile;

				//Lets the program recognize the given file 
				fstream ffile;

				//If the file exists (and in the right place) the file will open and write to the output the contents of file
				ffile.open(facilityfile);

				string facilInfo[31];
				if (ffile.good())
				{
					n = 0;
					while (!ffile.eof())
					{
						//cout << n;
						//runs through the file and populates the array with the data from the file
						string line;
						getline(ffile, line);
						facilInfo[n] = line;
						n++;
					}

					ffile.close();
				}
				int numFacil = stoi(facilInfo[0]);

				double transferMatrix[30][30];
				double(*matrixPoint)[30][30] = &transferMatrix;	//pointer to transferMatrix
				cout << "Enter the name of the transfer matrix file ";
				string filename;
				cin >> filename;
				readTransferMatrix(matrixPoint, filename);


				//Give the option to run with current data or start over and enter new data
				runagain = 0;
				string output_filename;

				//Asks for filename from user
				//Could have a graphic interface? and open the file browser
				cout << "Input the name of the file data will be outputted to: ";
				cin >> output_filename;

				//Lets the program recognize the given file 
				ofstream outputFile;
				outputFile.open(output_filename);


				boost::mt19937 generator(time(0));	//create RNG
				boost::random::uniform_real_distribution< > uniformDistribution(0.0, 1.0);
				boost::random::triangle_distribution< > triangleDistribution(0.0, 1.0);

				boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > >
					generateRandomNumbers(generator, uniformDistribution);
				boost::variate_generator< boost::mt19937&, boost::random::triangle_distribution < > >
					generateTriangleRandomNumbers(generator, triangleDistribution);

				boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > > * RNGpoint = &generateRandomNumbers;
				boost::variate_generator< boost::mt19937&, boost::random::triangle_distribution < > > * triangleRNGpoint = &generateTriangleRandomNumbers;
			
				vector<Facility*> facilities;
				while (run < numOfRuns) {
					//initialize facilities
					for (int i = 1; i <= numFacil; i++) {
						if (run == 0) {
							Facility * f = new Facility(i-1, facilInfo[i], triangleRNGpoint, RNGpoint, inc_dist, upper_inc, lower_inc, inf_dist, upper_inf, lower_inf);
							(*f).addS((*f).countSus());
							(*f).addE((*f).countExp());
							(*f).addI((*f).countInf());
							facilities.push_back(f);
						}
						else {
							Facility * f = facilities.at(i-1);
							(*f).updateS(0, (*f).countSus());
							(*f).updateE(0, (*f).countExp());
							(*f).updateI(0, (*f).countInf());
						}
					}


					
					for (int i = 1; i <= endtime; i = i + time_step)
					{
						for (int j = 0; j < numFacil; j++) {
							Facility * f = facilities.at(j);
							int sus = (*f).countSus();
							int exp = (*f).countExp();
							int inf = (*f).countInf();
							double beta;
							if (inf_dist == 0) {
								beta = reproductive_rate / (upper_inf * (sus + exp + inf));
							}
							else {
								beta = reproductive_rate / ((((upper_inf - lower_inf) / 2.0) + lower_inf) * (sus + exp + inf));
							}

							(*f).dynamics(beta, i);
							
							if (run == 0) {
								(*f).addS((*f).countSus());
								(*f).addE((*f).countExp());
								(*f).addI((*f).countInf());
							}
							else {
								(*f).updateS(i, (*f).countSus());
								(*f).updateE(i, (*f).countExp());
								(*f).updateI(i, (*f).countInf());
							}
						}

						//transfer
						vector<Agent*> transfers;
						for (int j = 0; j < numFacil; j++) {
							Facility * f = facilities.at(j);
							vector<Agent*> temp = (*f).removePatients(i);
							transfers.insert(transfers.end(), temp.begin(), temp.end()); 
						}
						for (Agent* a : transfers) {
							int destination = randProb(transferMatrix[(*a).getFacility()], numFacil, RNGpoint);
							(*facilities.at(destination)).addPatient(a, i);
						}
						transfers.clear();



					}
					run++;
					for (int j = 0; j < numFacil; j++) {	//reset
						Facility * f = facilities.at(j);
						(*f).reset();
					}
				}

				outputFile << "Day, Suceptible, Exposed, Infected, Total (by facility)\n";
				for (int i = 0; i <= (endtime/time_step); i++) {
					outputFile << i << ", ";
					for (int j = 0; j < numFacil; j++) {
						Facility f = *(facilities.at(j));
						outputFile << (double)f.getS(i) / numOfRuns << ", " << (double)f.getE(i) / numOfRuns << ", " << (double)f.getI(i) / numOfRuns << ", " << (f.getS(i) + f.getE(i) + f.getI(i)) / numOfRuns << ", ";
					}
					outputFile << "\n";
				}
				
				/*

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
				outputFile.close();*/
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