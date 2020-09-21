#include <iostream>
#include <fstream>
#include <string> 
#define pi 3.14
#define SIZE 4000
using namespace std;
int LoadData(double EMGdata[], string filename);
void BandPassFilter(double EMGdata[], double fl, double fh, double dt, int size);
void ApplySmoothing(double EMGdata[], int size);
int ZeroCrossing(double EMGdata[], int size);


int main()
{
	//declaring and initilizing variables
	int fl = 55, fh = 65;
	double dt = 1.0 / 4000.0;
	string filename;
	//printing the following sentences
	cout << "EMG Signal Processing" << endl;
	cout << "Please enter the file name where the EMG data is stored" << endl;
	//the name of file stored in the filename
	cin >> filename;
	//declaring array
	double EMGdata[SIZE];
	//calling functions
	int size = LoadData(EMGdata, filename);
	BandPassFilter(EMGdata, fl, fh, dt, size);
	ApplySmoothing(EMGdata, size);
	int num = ZeroCrossing(EMGdata, size);
	//printing size
	cout << "The number of data points is:" << size << endl;
	//printing output
	cout << "The number of zero crossing is equal to: " << num << endl;
	system("pause");
	return(0);
}
// function that load the data into an array for further processing
int LoadData(double EMGdata[], string filename)
{
	//declare and initilization 
	double time;
	int size = 0;
	//opening file
	ifstream infile;
	//checking if the files open or not
	infile.open(filename, ios::in);
	if (infile.fail())
	{
		//print message if the file is not opening
		cerr << "Error opening the file";
		system("pause");
		exit(1);
	}
	//do while loop until the END OF FILE
	while (!infile.eof())
	{
		//	Reading data from file into time and then into EMGdata[size]
		infile >> time >> EMGdata[size];
		size++;
	}
	infile.close();
	//returning size to the data
	return size;
}
//Function that filters data and remove low and high frequencies from a signal
void BandPassFilter(double EMGdata[], double fl, double fh, double dt, int size)
{
	//declaring arrays
	double yh[SIZE], yl[SIZE], output[SIZE];
	//declaring variables and assigning equations for them
	double R1 = 1 / (2 * pi *fl), a1 = R1 / (R1 + dt), R2 = 1 / (2 * pi *fh), a2 = dt / (R2 + dt);
	//Assiging EMGdata
	yh[0] = EMGdata[0];
	yl[0] = EMGdata[0];
	//loop for high pass filter equation
	for (int i = 1; i < size; i++)
	{
		yh[i] = a1 * (yh[i - 1] + EMGdata[i] - EMGdata[i - 1]);
	}
	//loop for low pass filter equation
	for (int i = 1; i < size; i++)
	{
		yl[i] = yl[i - 1] + a2 * (EMGdata[i] - yl[i - 1]);
	}
	//loop for band pass filter equation
	for (int x = 0; x < size; x++)
	{
		output[x] = yh[x] - yl[x];
		EMGdata[x] = output[x];
	}

}
// function that gives us the average of a number and the three numbers that are following it
void ApplySmoothing(double EMGdata[], int size)
{
	//loop for Apply Smoothing equation
	for (int i = size - 1; i > 3; i--)
	{
		EMGdata[i] = (EMGdata[i] + EMGdata[i - 1] + EMGdata[i - 2] + EMGdata[i - 3]) / 4;
	}
}
// function that showes us how many time the line was crossed
int ZeroCrossing(double EMGdata[], int size)
{
	int num = 0;
	{
		for (int i = 1; i < size; i++)
			// if two sets of arrays next to each other were multiplied and the answer is negative then the zero line was crossed because we had a positive number and a negative number.
			if (EMGdata[i] * EMGdata[i - 1] < 0)
				num++;
	}
	return num;
}
