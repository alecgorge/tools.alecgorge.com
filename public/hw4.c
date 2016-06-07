#include <stdio.h>
#include <math.h>
	
#define R_CONST 0.082054

#define DBL_PRINT(...) printf(__VA_ARGS__); fprintf(output, __VA_ARGS__)

// for data input
void get_double(char *prompt, double *stor_loc);
void get_char(char *prompt, char *stor_loc);
void get_string(char *prompt, char *stor_loc);

int verify(char *conf_message);

// for calculations
double f(double v, double p, double a, double b, double t);
double fprime(double v, double p, double a, double b);
void newton_raphson(double results[], double guess, double abs_pressure,
	double temperature, double a, double b, double max_iterations,
	double converg_criteria);
double ideal_gas_law(double abs_pressure, double temperature);

// globals, for output
FILE *output = NULL;

int main(void) {
	double temperature, init_pressure, final_pressure, num_increments,
		   a, b, max_iterations, converg_criteria;
	char gas_type, output_filename[101];

	int i = 0;
	double abs_pressure, press_single_incr, results[2], ideal;

	// get inputs
	get_double("Temperature (K)", &temperature);
	get_double("initial pressure (atm)", &init_pressure);
	get_double("final pressure (atm)", &final_pressure);
	get_double("number of pressure increments", &num_increments);

	get_char("gas type", &gas_type);
	if(gas_type == 'c') {
		a = 3.59200;
		b = 0.04267;
	}
	else if(gas_type == 'o') {
		a = 1.36000;
		b = 0.03183;
	}
	else {
		get_double("a (for a gas, if not carbon dioxide or oxygen)", &a);
		get_double("b (for a gas, if not carbon dioxide or oxygen)", &b);
	}

	get_double("maximum number of iterations", &max_iterations);
	get_double("convergence criteria", &converg_criteria);

	for(i = 0; i < 5; i++) {
		get_string("Output file name", output_filename);

		output = fopen(output_filename, "r");
		if(output != NULL) {
			printf("Silly goose! '%s' already exists!\n", output_filename);
			fclose(output);
		}
		else {
			output = fopen(output_filename, "w");
			break;
		}

		// the user has inputted 5 files that exist
		if(i == 4) {
			printf("Unable to make \"new\" output file after 5 attempts! Exiting!\n");
			return 1;
		}
	}

	// output known data
	DBL_PRINT("\n\ninitial pressure (atm) = %lf\n", init_pressure);
	DBL_PRINT("final pressure (atm) = %lf\n", final_pressure);
	DBL_PRINT("number of pressure increments = %lf\n", num_increments);
	DBL_PRINT("\na = %lf\n", a);
	DBL_PRINT("b = %lf\n\n", b);
	DBL_PRINT("gas type = %c\n", gas_type);
	DBL_PRINT("maximum number of iterations = %lf\n", max_iterations);
	DBL_PRINT("convergence criteria = %lf\n", converg_criteria);
	DBL_PRINT("\n\n");

	DBL_PRINT("Temperature (K)\t\tPressure (atm)\t\tMolal Volume,");
	DBL_PRINT("\t\tNumber Iterations\t\tMolal Volume,\n");
	DBL_PRINT("\t\t\t\t\t\t L/mol (vdW)\t\t\t\t\t\t L/mol (IGL)\n");

	press_single_incr = (final_pressure - init_pressure) / num_increments;

	for(abs_pressure = init_pressure
	  ; abs_pressure <= final_pressure
	  ; abs_pressure += press_single_incr) {
	  	ideal = ideal_gas_law(abs_pressure, temperature);

		newton_raphson(results, ideal, abs_pressure, temperature, a, b,
					   max_iterations, converg_criteria);

		DBL_PRINT("  %lf\t\t  %10lf\t\t %10lf\t\t  %10lf\t\t\t%10lf\n", temperature,
			abs_pressure, results[0], results[1], ideal);
	}

	fclose(output);

	return 0;
}

double f(double v, double p, double a, double b, double t) {
	return (p + a / pow(v, 2)) * (v - b) - R_CONST * t;
}

double fprime(double v, double p, double a, double b) {
	return p + (a / pow(v, 2)) - ((2 * a * (-1 * b + v)) / pow(v, 3));
}

double ideal_gas_law(double p, double t) {
	return (R_CONST * t) / p;
}

void newton_raphson(double results[], double guess, double abs_pressure,
	double temperature, double a, double b, double max_iterations,
	double converg_criteria) {
	int i = 0;
	double last = guess - f(guess, abs_pressure, a, b, temperature)
						/ fprime(guess, abs_pressure, a, b);
	double this = 0.0;

	for(i = 1; i < max_iterations; i++) {
		this = last - f(last, abs_pressure, a, b, temperature)
			        / fprime(last, abs_pressure, a, b);

		if(fabs((this - last) / this) <= converg_criteria) {
			results[0] = this;
			results[1] = i + 1;
			return;
		}

		last = this;
	}

	results[0] = 0.0;
	results[1] = max_iterations;
}

int verify(char *conf_message) {
	char conf;

	printf("%s", conf_message);
	scanf("%c", &conf); getchar();

	if(conf == 'Y' || conf == 'y' || conf == 0) return 1;
	else if(conf == 'N' || conf == 'n') return 0;
	else {
		printf("Please enter 'Y' or 'y' for yes or 'N' or 'n' for no, not '%c'\n", conf);
		return verify(conf_message);
	}
}

void get_double(char *prompt, double *stor_loc) {
	char conf_message[101];

	printf("%s: ", prompt);
	scanf("%lf", stor_loc); getchar();

	sprintf(conf_message, "Is '%lf' correct? (Y)es or (N)o: ", *stor_loc);
	
	if(!verify(conf_message)) get_double(prompt, stor_loc);
}

void get_char(char *prompt, char *stor_loc) {
	char conf_message[101];

	printf("%s: ", prompt);
	scanf("%c", stor_loc); getchar();

	sprintf(conf_message, "Is '%c' correct? (Y)es or (N)o: ", *stor_loc);
	
	if(!verify(conf_message)) get_char(prompt, stor_loc);
}

void get_string(char *prompt, char *stor_loc) {
	char conf_message[202];

	printf("%s: ", prompt);
	scanf("%s", stor_loc); getchar();

	sprintf(conf_message, "Is '%s' correct? (Y)es or (N)o: ", stor_loc);
	
	if(!verify(conf_message)) get_string(prompt, stor_loc);
}

