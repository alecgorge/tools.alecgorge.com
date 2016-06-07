import sys
import math

R = 0.082054

def verify(r):
	print("Is '%s' correct? (Y)es or (N)o: " % r, end = "")
	char = str(input()).strip()
	if char == 'Y' or char == 'y':
		return True
	if char == 'N' or char == 'n':
		return False

	print("Please enter 'Y' or 'y' for yes or 'N' or 'n' for no, not '%c'" % char)
	return verify(r)

def get_inp(prompt, _type = 'lf'):
	print(prompt + ": ", end = "")
	r = input()

	if not verify(r):
		r = get_inp(prompt, _type)

	if _type == 'c':
		return r[0]
	if _type == 's':
		return r.strip()
	return float(r)

def f(v, p, a, b, t):
	return (p + a / v**2) * (v - b) - R * t

def fprime(v, p, a, b):
	return p + (a / v**2) - ((2 * a * (-1 * b + v)) / v**3)

def ideal_gas_law(p, t):
	return (R * t) / p

def double_print(txt, end = "\n"):
	print(txt, end = end)
	output.write(txt + end)

def newton_raphson(guess, abs_pressure, temperature, a, b, max_iterations,
	 converg_criteria):
	n = f(guess, abs_pressure, a, b, temperature)
	d = fprime(guess, abs_pressure, a, b)
	last = guess - n / d
	this = 0.0;

	for i in range(1, max_iterations):
		n = f(last, abs_pressure, a, b, temperature)
		d = fprime(last, abs_pressure, a, b)
		this = last - n / d

		if math.fabs((this - last) / this) <= converg_criteria:
			return this, i+1

		last = this

	return 0.0, max_iterations

temperature = get_inp("Temperature (K)")
init_pressure = round(get_inp("initial pressure (atm)"))
final_pressure = round(get_inp("final pressure (atm)"))
num_increments = round(get_inp("number of pressure increments"))

gas_type = get_inp("gas type", "c")
if gas_type == 'c':
	a = 3.59200
	b = 0.04267
elif gas_type == 'o':
	a = 1.36000
	b = 0.03183
else:
	a = get_inp("a (for a gas, if not carbon dioxide or oxygen)")
	b = get_inp("b (for a gas, if not carbon dioxide or oxygen)")

max_iterations = round(get_inp("maximum number of iterations"))
converg_criteria = get_inp("convergence criteria")
output_filename = ""
output = False

for i in range(5):
	output_filename = get_inp("Output file name", "s");

	try:
		output = open(output_filename, "x")
		break
	except:
		print("Silly goose! '%s' already exists!" % output_filename)

	# the user has inputted 5 files that exist
	if i == 4:
		print("Unable to make \"new\" output file after 5 attempts! Exiting!")
		sys.exit(1)

# output known data
double_print("\n\ninitial pressure (atm) = %lf" % init_pressure)
double_print("final pressure (atm) = %lf" % final_pressure)
double_print("number of pressure increments = %lf" % num_increments)
double_print("\na = %lf" % a)
double_print("b = %lf\n" % b)
double_print("gas type = %c" % gas_type)
double_print("maximum number of iterations = %lf" % max_iterations)
double_print("convergence criteria = %lf" % converg_criteria)
double_print("\n")

double_print("Temperature (K)\t\tPressure (atm)\t\tMolal Volume,", end = "")
double_print("\t\tNumber Iterations\t\tMolal Volume,")
double_print("\t\t\t\t\t\t L/mol (vdW)\t\t\t\t\t\t L/mol (IGL)")

press_single_incr = round((final_pressure - init_pressure) / num_increments)
for abs_pressure in range(init_pressure, final_pressure+press_single_incr, press_single_incr):
	ideal = ideal_gas_law(abs_pressure, temperature)

	(guess, iterations) = newton_raphson(ideal, abs_pressure, temperature, a, b,  max_iterations, converg_criteria)

	double_print("  %lf\t\t  %10lf\t\t %10lf\t\t  %10lf\t\t\t%10lf" % (temperature, abs_pressure, guess, iterations, ideal))

output.close()

