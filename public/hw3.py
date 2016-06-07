import sys
import math
from collections import Counter

def mode(Bottles):
	c = Counter(bottle['ounces'] for bottle in Bottles)

	MaxCount = 0
	MostComOz = c.most_common()
	for Inst in MostComOz:
		if Inst[0] == MostComOz[0][0]:
			MaxCount = MaxCount + 1

	Output = "Found %d mode(s) with %d instances: " % (MaxCount, MostComOz[0][1])
	print(Output, end = '')

	for Inst in MostComOz:
		if Inst[1] == MostComOz[0][1]:
			print("% -5lf, " % Inst[0], end = '')

	print("")

	return

def variance(Bottles, Mean):
	Sum = 0.0

	for bottle in Bottles:
		Sum = Sum + (bottle['ounces'] - Mean) ** 2

	return Sum / (len(Bottles) - 1)

def median(Bottles):
	length = len(Bottles)
	mid = Bottles[length // 2]

	if length % 2 == 0:
		return (mid['ounces'] + Bottles[length // 2 - 1]['ounces']) / 2.0

	return mid['ounces']

def deviationToOunces(deviation):
	pi_r2 = math.pi * ((0.75 / 2) ** 2)

	return float(deviation) * 0.554112554 * pi_r2 / 10000.0

Date = ""
Time = ""

SamplesInBatch = 0
BatchNum = 0

Min = 0.0
Max = 0.0
Mean = 0.0
Variance = 0.0
SD = 0.0
Median = 0.0

SamplesOutOfRange = 0.0
SamplesLessThan8 = 0.0

Bottles = []


print("Please enter filename: ")
filename = input().strip()

# error trap filename
try:
	fp = open(filename)
except IOError:
	print("The requested file '%s' was not found." % filename)
	sys.exit(1)

Date = fp.readline().strip()
Time = fp.readline().strip()
(BatchNum, SamplesInBatch) = fp.readline().strip().split("-")
SamplesInBatch = int(SamplesInBatch)

for line in fp.readlines():
	line = line.strip().split()

	for j in range(len(line)):
		if j % 2 == 0:
			Bottles.append({
				'code': line[j],
				'ounces': deviationToOunces(line[j+1])
			})

Bottles = sorted(Bottles, key=lambda bottle: bottle['ounces'])

Min = Bottles[0]['ounces']
Max = Bottles[-1]['ounces']

Mean = sum(bottle['ounces'] for bottle in Bottles) / float(len(Bottles))

Variance = variance(Bottles, Mean)

SD = Variance ** .5

Median = median(Bottles)

for bottle in Bottles:
	o = bottle['ounces']

	if o < Mean - 0.1 or o > Mean + 0.1:
		SamplesOutOfRange = SamplesOutOfRange + 1
	if o + 8 < 8:
		SamplesLessThan8 = SamplesLessThan8 + 1

print("Filler Station ID\t\t: %s" % Bottles[0]['code'][0:4])
print("Date\t\t\t\t: %s" % Date)
print("Time\t\t\t\t: %s" % Time)
print("Batch Number\t\t\t: %s" % BatchNum)
print("Number of Samples\t\t: %d\n" % SamplesInBatch)

print("Bottle(s) outside of +/- 0.1 oz\t: %.0lf" % SamplesOutOfRange)

for bottle in Bottles:
	if bottle['ounces'] < Mean - 0.1 or bottle['ounces'] > Mean + 0.1:
		print("\t%s - %f" % bottle['code'], bottle['ounces'])

percent = SamplesLessThan8 * 100 / SamplesInBatch
print("Bottle(s) under 8oz\t\t: %.0lf (%.2lf%%)" % (SamplesLessThan8, percent))

print("\nStatistical data:")
print("=============================================")
print("Maximum\t\t\t\t: % -.5lf oz" % Max)
print("Minimum\t\t\t\t: % -.5lf oz" % Min)
print("Mean\t\t\t\t: % -.5lf oz" % Mean)
print("Median\t\t\t\t: % -.5lf oz" % Median)

# output for mode is handled within mode(2) itself
mode(Bottles)

print("Sample variance\t\t\t: % -.5lf oz" % Variance)
print("Sample standard deviation\t: % -.5lf oz" % SD)
print("Statistical upper bound\t\t: % -.5lf oz" % (Mean + (3 * SD)))

MeanDiv = ((Mean + 3 * SD) > 0.1) or ((Mean - 3 * SD) < -0.1)
if (SamplesLessThan8 * 100 / SamplesInBatch > 10) or MeanDiv:
	print("\n\nRECALIBRATION NECESSARY")

fp.close()

