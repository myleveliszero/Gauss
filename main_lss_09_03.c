#include "lss_09_03.h"

code mymode;

int setup(const char* filename, double** A, double** B, double** X, int* n);

int parse_argv(int argc, char* argv[], const char** in_file, const char** out_file, int* pmode, int* tmode, int* hmode);

int write_to_file(const char* filename, double* X, int n, int success_code);

void freespace(double* A, double* B, double* X);

void hmode_instructions();

int setup(const char* filename, double** A, double** B, double** X,int* n)
{
	// Description;
	// reads a file if the file is not specified then the default file is lss_09_03_in.txt;
	// allocates memory for arrays A, B, X;
	// reads and writes data from the file to arrays A, B;
	// also defines the length of these arrays in variable n;


	// return codes;
	// code: -2;  content: the default input file lss_09_03_in.txt doesn't exist in current environment or can't open input file;
	// code: -3;  content: related with fscanf, not enough data(empty file) or incorrect type of data; 
	// code: -4;  content: the given "n" is negative or equal to zero;
	// code: -5;  content: memory was not allocated;
	// code: -6;  content: didn't scanned all data;
	// code:  2;  content: function "setup" is successfully ended;

	int err_code = -2;
	FILE* stream;
	int len = 0;

	if (filename != NULL) stream = fopen(filename, "r");
	else stream = fopen("lss_09_03_in.txt", "r");


	if (stream != NULL)
	{
		err_code = fscanf(stream, "%d", &len);

		if (err_code == 1)
		{
			if (len <= 0)
			{
				fclose(stream);
				return -4;
			}
				

			*n = len;
			double* a = (double*)malloc(sizeof(double) * (len*len));
			double* b = (double*)malloc(sizeof(double) * (len));
			double* x = (double*)malloc(sizeof(double) * (len));

		
			
			if (a != NULL && b != NULL && x != NULL)
			{
				// can enhence and probably better to return two different codes;
				// one is for not enough data and the second is for incorrect type of data(itod);
				// add extra variable which will tell to user where is the itod;
				for (size_t i = 0, j =0; i < len*len + len; i++)
				{
					if (i >= len * len)
					{
						err_code = fscanf(stream, "%lf", &b[j]);
						if (err_code == -1 || err_code == 0)
						{
							fclose(stream);
							return -3;
						}
						j++;
					}
					else
					{
						err_code = fscanf(stream, "%lf", &a[i]);
						if (err_code == -1 || err_code == 0)
						{	
							fclose(stream);
							return -3;
						}
					}

				}
				double dummy;
				err_code = fscanf(stream, "%lf", &dummy);
				if (err_code == 1)
				{
					fclose(stream);
					return -6;
				}
					

				*A = a; *B = b; *X = x;

				fclose(stream);
				return 2;

			}
			fclose(stream);
			return -5;
		}
		fclose(stream);
		return -3;
	}

	return err_code;
}


int parse_argv(int argc, char* argv[], const char** in_file, const char** out_file, int* pmode, int* tmode,int* hmode)
{	
	// Description;
	// Parses given const char* argv[], and extracts all given arguments; 
	// command line: name.exe [input_file] [output_file] [options]
 
	// return codes;
	// code: -7;  content: it's the case when input file passed after options or invalid data;
	// code: -8;  content: passed none-existing option;
	// code: -9;  content: first argument is option and the second is file;
	// code:  2;  content: function "parse_argv" is successfully ended;

	int err_code = 2;
	if (argc == 1) return err_code;
	
	for (size_t i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			if (argv[i][1] == 'p')
				*pmode = True;
			else if (argv[i][1] == 't')
				*tmode = True;
			else if (argv[i][1] == 'h') // if '?' also then, argv[i][1] == 'h' || argv[i][1] == '?'
				*hmode = True;
			else if (argv[i][1] == 'd')
				mymode.dmode = True;
			else if (argv[i][1] == 'e')
				mymode.emode = True;
			else return -8;

		}
		else if (i == 1 || i == 2)
		{
			if (i == 1)
				*in_file = argv[1];
			if (i == 2)
			{
				if (argv[1][0] != '-')
					*out_file = argv[2];
				else return -9;
			}
				
		}
		else return -7;
	}

	return err_code;
}

int write_to_file(const char* filename, double* X, int n,int success_code)
{
	FILE* stream;
	
	if (filename != NULL) stream = fopen(filename, "w");
	else stream = fopen("lss_09_03_out.txt", "w");

	if (stream != NULL)
	{
		if (success_code == 1)
		{
			fprintf(stream, "%d\n", 0);
			fclose(stream);
			return 2;
		}

		fprintf(stream, "%d\n", n);
		for (size_t i = 0; i < n; i++)
			fprintf(stream, "%1.9lf \n", X[i]);
		
		fclose(stream);
		return 2;
	}

	return -10;
}

void freespace(double* A, double* B, double* X)
{
	free(A); A = NULL;
	free(B); B = NULL;
	free(X); X = NULL;
}


void hmode_instructions()
{
	printf("\nUsage: lss [input_file_name][output_file_name][options]\n");
	printf("Where options include: \n");
	printf("	-d			print debug messages [default OFF]\n");
	printf("	-e			print errors [default OFF]\n");
	printf("	-p			print matrix [default OFF]\n");
	printf("	-t			print execution time [default OFF]\n");
	printf("	-h, -?			print this and exit\n\n\n");
	printf("Default input_file_name value is lss_09_03_in.txt, default output_file_name value is lss_09_03_out.txt.\n\n");
}


int main(int argc, char* argv[])
{
	clock_t overallstart = clock();
	int err_code = 0;
	
	// 
	int hmode, tmode, pmode;

	const char* input_file = NULL,
			  * output_file = NULL;

	// Parsing command line(cl), extracting the flags and in,out files if they are in the cl
	err_code = parse_argv(argc, argv, &input_file, &output_file, &pmode,&tmode,&hmode);

	if (hmode == True) hmode_instructions();

	if (err_code != 2)
	{
		if (err_code == -7)
			fprintf(stderr, "Invalid argument\n");
		else if (err_code == -8)
			fprintf(stderr, "Invalid argument\n");
		else if (err_code == -9)
			fprintf(stderr, "Invalid argument\n");

		return err_code;
	}

	// A,B,X,n will be used for execution of function "int lss_09_03"
	double* A = NULL,
		  * B = NULL,
		  * X = NULL;
	int n = 0;
	
	
	
	clock_t setup_start = clock();
	err_code = setup(input_file, &A, &B, &X, &n);
	clock_t setup_end = clock();
	if (mymode.dmode == True)
	{
		printf("\n------****** DEBUG MODE [ON] ******------\n");
		printf("Loading file: %lf sec \n", (double)(setup_end - setup_start) / (double)(CLOCKS_PER_SEC));
	}
	if (pmode == True)
	{
		printf("\nPrint Mode [ON] BEGIN\n");
		print_mv(A, B, n);
	}
	// Checking return code of function "setup"
	if (err_code != 2)
	{
		if (mymode.emode == True)
		{
			if (err_code == -2)
			{	
				if (input_file != NULL)
					fprintf(stderr, "Invalid file %s\n", input_file);
				else fprintf(stderr, "the default lss_09_03_in.txt doesn't exist\n");
			}
			else if (err_code == -3)
			{
				// could be n is incorrect to
				if (input_file != NULL)
					fprintf(stderr, "Invalid file %s\n", input_file);
				else fprintf(stderr, "Invalid file lss_09_03_in.txt\n");
			}
			else if (err_code == -4)
			{
				if (input_file != NULL)
					fprintf(stderr, "Invalid 'n' in the file %s\n", input_file);
				else fprintf(stderr, "Invalid 'n' in the file lss_09_03_in.txt\n");
			}
			else if (err_code == -5)
				fprintf(stderr, "memory was not allocated for array A or B or X\n");
			else if (err_code == -6)
			{
				if (input_file != NULL)
					fprintf(stderr, "Invalid file %s\n", input_file);
				else fprintf(stderr, "Invalid file lss_09_03_in.txt\n");
			}
		}

		if (err_code != -5)
			freespace(A, B, X);

		return err_code;
	}
	// End of checking return code of "setup"

	double* tmp = (double*)malloc(lss_memsize_09_03(n * n));
	if (tmp == NULL) return -5;

	clock_t start = clock();
	int success_code = lss_09_03(n, A, B, X, tmp);
	clock_t end = clock();
	free(tmp), tmp = NULL; 

	if (mymode.dmode == True)
	{
		printf("The array X: \n");
		print_arr(X, n, VECTOR);
	}

	if (pmode == True)
	{
		printf("Print Mode [ON] END\n");
		print_mode(X, n);;
	}

	if (tmode == True) printf("Execution time: %lf sec\n", (double)(end - start) / (double)(CLOCKS_PER_SEC));

	if (mymode.dmode == True) printf("Writing array X(answer) to the ouput file\n");
	err_code = write_to_file(output_file, X, n, success_code);

	if (err_code != 2)
	{
		if (mymode.emode == True)
			fprintf(stderr, "Can't write to a file");

		return err_code;
	}


	freespace(A, B, X);

	clock_t overallend = clock();
	if (mymode.dmode == True)
	{
		printf("Overall execution time: %lf sec\n", (double)(overallend - overallstart) / (double)(CLOCKS_PER_SEC));
		printf("\n------****** End of DEBUG ******------\n");
	}

	return success_code;
}