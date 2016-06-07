
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
