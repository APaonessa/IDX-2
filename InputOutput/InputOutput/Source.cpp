#include <iostream>; 
#include <fstream>
#include <string>; 
#include <ios>; 
using namespace std;

int main()
{
	int n = 0; 
	int runagain = 0;
	//Information for formatting the text file...
	cout << "To run this program input a .txt file in this form...\n";
	cout << "\n";
	cout << "Line 1: # of Susceptible, # of Exposed, # of Infected\n";
	cout << "(Do not use commas to separate numbers)\n"; 
	cout << "Line 2: the time for transmission\n"; 
	cout << "\n"; 
	//cout << "Line 3: time step you want data displayed at"; 
	string myArray[5];

	char timestep_graph; 

	while (runagain > -1)
	{
		//executes loop if the input fails (e.g., no characters were read)
		while (cout << "Would you like to run again? (yes = 1/no = 0) >>>" && !(cin >> runagain)) {
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
					myfile >> myArray[n];
					n++;
				}
				myfile.close();

				//switches the data from being in string form to being an integer
					int susceptible = stoi(myArray[0]);
					int exposed = stoi(myArray[1]);
					int infected = stoi(myArray[2]);
					int time_step = stoi(myArray[3]);
					
					//prints out the data that is read in file as a check... 
					cout << "susceptible: ";
					cout << susceptible;
					cout << "\n";
					cout << "exposed: "; 
					cout << exposed;
					cout << "\n";
					cout << "infected: ";
					cout << infected;
					cout << "\n";
					cout << "time step: ";
					cout <<	time_step; 
					cout << "\n";
				//Give the option to run with current data or start over and enter new data
				runagain = 0;
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
}
