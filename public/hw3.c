#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define Pi (4.0 * atan(1.0))
#define BOTTLE_NECK_DIAMETER 0.75

void assoc_bubble_sort(char BottleBarCodes[][9], double BottleOunces[], int SamplesInBatch);

double mean(double BottleOunces[], int SamplesInBatch);
void mode(double BottleOunces[], int SamplesInBatch);
double variance(double BottleOunces[], int SamplesInBatch, double Mean);
double median(double BottleOunces[], int SamplesInBatch);

int main(void)
{
	FILE *fp;
	char Date[12], Time[9];

	int SamplesInBatch = 0, i = 0;

	char CharSwap = 0;

	double Min = 0, Max = 0, Mean = 0, Variance = 0, SD = 0, Median = 0;
	double SamplesOutOfRange = 0, SamplesLessThan8 = 0, pi_r2 = 0;

	/* file name is up to 100 characters */
	char filename[101];

	int BatchNum = 0;

	printf("Please enter filename: ");
	scanf("%s", filename);

	if ((fp = fopen(filename, "r")) == NULL)
	{
		printf("The requested file '%s' was not found.\n", filename);
		return 1;
	}

	fscanf(fp, "%s", Date);
	fscanf(fp, "%s", Time);
	fscanf(fp, "%d-%d", &BatchNum, &SamplesInBatch);

	// an array of strings of length 8
	char BottleBarCodes[SamplesInBatch][9];
	double BottleOunces[SamplesInBatch];

	for (i = 0; i < SamplesInBatch; i++)
	{
		fscanf(fp, "%s%lf", BottleBarCodes[i], &BottleOunces[i]);
	}

	fclose(fp);

	for (i = 0; i < SamplesInBatch; i++)
	{
		// convert from height (in 0.0001 inches) to change to number of ounces
		pi_r2 = Pi * pow(BOTTLE_NECK_DIAMETER / 2, 2);
		BottleOunces[i] = (BottleOunces[i] * 0.554112554 * pi_r2 / 10000.0);
	}

	// sort the ounces from least to greatest and match the barcodes
	assoc_bubble_sort(BottleBarCodes, BottleOunces, SamplesInBatch);

	// BottleOunces is now sorted and the indexes still align with BottleBarCodes
	// therefore an index of 0 indicates the small volume and so forth
	Min = BottleOunces[0];
	Max = BottleOunces[SamplesInBatch - 1];

	Mean = mean(BottleOunces, SamplesInBatch);

	Variance = variance(BottleOunces, SamplesInBatch, Mean);

	SD = pow(Variance, 0.5);

	Median = median(BottleOunces, SamplesInBatch);

	for (i = 0; i < SamplesInBatch; i++)
	{
		if (BottleOunces[i] < Mean - 0.1 || BottleOunces[i] > Mean + 0.1)
		{
			SamplesOutOfRange++;
		}

		// need to add 8 because the ouces is just the deviation from 8
		if (BottleOunces[i] + 8 < 8)
		{
			SamplesLessThan8++;
		}
	}

	CharSwap = BottleBarCodes[0][4];
	BottleBarCodes[0][4] = '\0';

	printf("\nFiller Station ID\t\t: %s\n", BottleBarCodes[0]);

	BottleBarCodes[0][4] = CharSwap;

	printf("Date\t\t\t\t: %s\n", Date);
	printf("Time\t\t\t\t: %s\n", Time);
	printf("Batch Number\t\t\t: %d\n", BatchNum);
	printf("Number of Samples\t\t: %d\n\n", SamplesInBatch);

	printf("Bottle(s) outside of +/- 0.1 oz\t: %.0lf\n", SamplesOutOfRange);

	for (i = 0; i < SamplesInBatch; i++)
	{
		if (BottleOunces[i] < Mean - 0.1 || BottleOunces[i] > Mean + 0.1)
		{
			printf("\t%s - %lf\n", BottleBarCodes[i], BottleOunces[i]);
		}
	}

	printf("Bottle(s) under 8oz\t\t: %.0lf ", SamplesLessThan8);
	printf("(%.2lf%%)\n", SamplesLessThan8 * 100 / SamplesInBatch);

	printf("\nStatistical data:\n");
	printf("=============================================\n");
	printf("Maximum\t\t\t\t: % -.5lf oz\n", Max);
	printf("Minimum\t\t\t\t: % -.5lf oz\n", Min);
	printf("Mean\t\t\t\t: % -.5lf oz\n", Mean);
	printf("Median\t\t\t\t: % -.5lf oz\n", Median);

	// output for mode is handled within mode(2) itself
	mode(BottleOunces, SamplesInBatch);

	printf("Sample variance\t\t\t: % -.5lf oz\n", Variance);
	printf("Sample standard deviation\t: % -.5lf oz\n", SD);
	printf("Statistical upper bound\t\t: % -.5lf oz\n", Mean + (3 * SD));

	if (SamplesLessThan8 * 100 / SamplesInBatch > 10
	|| (Mean + 3 * SD) >  0.1
	|| (Mean - 3 * SD) < -0.1)
	{
		printf("\n\nRECALIBRATION NECESSARY\n");
	}

	return 0;
}


void assoc_bubble_sort(char BottleBarCodes[][9], double BottleOunces[], int SamplesInBatch)
{
	int i, j;
	float OuncesSwap;
	char BarCodeSwap[9];

	// perform a bubble sort
	for (i = 0; i < SamplesInBatch; i++)
	{
		for (j = 0; j < SamplesInBatch - 1; j++)
		{
			if (BottleOunces[j] > BottleOunces[j+1])
			{
				// perform a swap of the bar codes
				strcpy(BarCodeSwap, BottleBarCodes[j]);
				strcpy(BottleBarCodes[j], BottleBarCodes[j + 1]);
				strcpy(BottleBarCodes[j + 1], BarCodeSwap);

				// perform a swap of the ounces
				OuncesSwap = BottleOunces[j];
				BottleOunces[j]= BottleOunces[j + 1];
				BottleOunces[j + 1] = OuncesSwap;
			}
		}
	}
}


double mean(double BottleOunces[], int SamplesInBatch)
{
	double Sum = 0;
	int i;
	
	for (i = 0; i < SamplesInBatch; i++)
	{
		Sum = Sum + BottleOunces[i];
	}

	return Sum / SamplesInBatch;
}

void mode(double BottleOunces[], int SamplesInBatch)
{
	int i = 0, Count = 1, ModeLength = 0;
	int BottleOuncesFrequency[SamplesInBatch], MaxBottleOuncesFrequency = 1;

	// initalize the frequency array to 0's
	for (i = 0; i < SamplesInBatch; i++)
	{
		BottleOuncesFrequency[i] = 0;
	}

	// build an array of the number of the same values in a row
	for(i = 0; i < SamplesInBatch; i += Count) {
		Count = 1;

		while(BottleOunces[i] == BottleOunces[i + Count])
		{
			Count++;
		}

		BottleOuncesFrequency[i] = Count;
	}

	// find the largest frequency
	for (i = 0; i < SamplesInBatch; i++)
	{
		if (BottleOuncesFrequency[i] > MaxBottleOuncesFrequency)
		{
			MaxBottleOuncesFrequency = BottleOuncesFrequency[i];
		}  
	}

	// find how many times this max occurs 
	for (i = 0; i < SamplesInBatch; i++)
	{
		if (BottleOuncesFrequency[i] == MaxBottleOuncesFrequency)
		{
			ModeLength++;
		}
	}

	printf("Found %d mode(s) with %d instances: ", ModeLength, MaxBottleOuncesFrequency);

	// print out all of the ounces that are part of the max
	for (i = 0; i < SamplesInBatch; i++)
	{
		if (BottleOuncesFrequency[i] == MaxBottleOuncesFrequency)
		{
			printf("% -5lf, ", BottleOunces[i]);
		}
	}

	printf("\n");
}

double variance(double BottleOunces[], int SamplesInBatch, double Mean)
{
	int i = 0;
	double Sum = 0;

	for(i = 0; i < SamplesInBatch; i++)
	{
		Sum = Sum + pow(BottleOunces[i] - Mean, 2);
	}

	return Sum / (SamplesInBatch - 1);
}

double median(double BottleOunces[], int SamplesInBatch)
{
	double Med;

	// integer division will happen here, so for odd number of samples,
	// this is the same as floor(SamplesInBatch / 2.0);
	int MiddleIndex = SamplesInBatch / 2;

	// if there are an even number of bottles in the sample
	if ((SamplesInBatch % 2) == 0)
	{
		Med = (BottleOunces[MiddleIndex] + BottleOunces[MiddleIndex - 1]) / 2;
	}
	else
	{
		Med = BottleOunces[MiddleIndex];
	}

	return Med;
}
