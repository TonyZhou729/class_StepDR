#include "common.h"

/* Stepped fluid modification */
#include "gsl/gsl_sf_bessel.h" // For usage of the modified bessel function of the second kind from gsl math library.
/* End stepped fluid modification */

void class_protect_sprintf(char* dest, char* tpl,...) {
  va_list args;
  va_start(args,tpl);
  vsnprintf(dest, 2048,tpl,args);
  va_end(args);
}

void class_protect_fprintf(FILE* stream, char* tpl,...) {
  va_list args;
  char dest[6000];
  va_start(args,tpl);
  vsnprintf(dest, 2048,tpl,args);
  va_end(args);
  fprintf(stream,"%s",dest);
}

void* class_protect_memcpy(void* dest, void* from, size_t sz) {
  return memcpy(dest, from,sz);
}

int get_number_of_titles(char * titlestring){
  int i;
  int number_of_titles=0;

  for (i=0; i<strlen(titlestring); i++){
    if (titlestring[i] == '\t')
      number_of_titles++;
  }
  return number_of_titles;
}

/**
 * Finds wether or not a file exists.
 *
 * @param fname  Input: File name
 * @return boolean
 */

int file_exists(const char *fname){
  FILE *file = fopen(fname, "r");
  if (file != NULL){
    fclose(file);
    return _TRUE_;
  }

  return _FALSE_;

}

/**
 * Finds whether two doubles are equal or which one is bigger
 *
 * @param a Input: first number
 * @param b Input: second number
 * @return -1, 1 or 0
 */

int compare_doubles(const void *a,
                    const void *b){
  double *x = (double *) a;
  double *y = (double *) b;
  if (*x < *y)
    return -1;
  else if
    (*x > *y) return 1;
  return 0;
}

/**
 * This function detects if a string begins with a character,
 * ignoring whitespaces during its search
 *
 * returns the result, NOT the _SUCCESS_ or _FAILURE_ codes.
 * (This is done such that it can be used inside of an if statement)
 *
 * @param thestring  Input: string to test
 * @param beginchar  Input: the character by which the string begins or not
 * @return boolean
 */

int string_begins_with(char* thestring, char beginchar){

  /** Define temporary variables */
  int int_temp=0;
  int strlength = strlen((thestring));
  int result = _FALSE_;

  /** Check through the beginning of the string to see if the beginchar is met */
  for(int_temp=0;int_temp<strlength;++int_temp){
    /* Skip over whitespaces (very important) */
    if(thestring[int_temp]==' ' || thestring[int_temp]=='\t'){continue;}
    /* If the beginchar is met, everything is good */
    else if(thestring[int_temp]==beginchar){result=_TRUE_;}
    /* If something else is met, cancel */
    else{break;}
  }

  return result;
}

/**
 * Get version number
 *
 * @param version  Output: The string to write the version number into
 * @return the error status
 */

int class_version(
                  char * version
                  ) {

  sprintf(version,"%s",_VERSION_);
  return _SUCCESS_;
}

/* Stepped fluid modification */

/**
 * Dimensionless Maxwell-Boltzmann density integral.
 *
 * @param x  Input: Dimensionless inverse fluid temperature x=m/Td.
 * @return analytic result in polynomial and modified bessel function (2nd kind) terms.
 */
double rhohat(double x){
  double K1 = gsl_sf_bessel_Kn(1, x);
  double K2 = gsl_sf_bessel_Kn(2, x);
  return pow(x, 2.)/2*K2 + pow(x, 3.)/6*K1;
}

/**
 * Dimensionless Maxwell-Boltzmann pressure integral.
 *
 * @param x  Input: Dimensionless inverse fluid temperature x=m/Td.
 * @return analytic result in polynomial and modified bessel function (2nd kind) terms.
 */
double phat(double x){  
  double K2 = gsl_sf_bessel_Kn(2, x);
  return pow(x, 2.)/2*K2;
}

/** 
 * A very makeshift secant method non-linear function root finder. May be improved in the future.
 *
 * @param x0  Input: Solution guess 1.
 * @param x1  Input: Solution guess 2.
 * @param a   Input: Current scale factor.
 * @param at  Input: Fluid step scale factor.
 * @param rg  Input: Dimensionless step size.
 * @return root x of the function. 
 */
double _secant_method(double x0, double x1, double a, double at, double rg){
  double x2, f0, f1, f2;
  int iteration = 0;
    
  do {
    f0 = _f_(x0, a, at, rg);
    f1 = _f_(x1, a, at, rg);

    x2 = x1 - ((f1 * (x1 - x0)) / (f1 - f0));
    f2 = _f_(x2, a, at, rg);

    x0 = x1;
    x1 = x2;

    iteration++;
    
    if (iteration >= _SECANT_MAX_){
      printf("Maximum iterations exceeded in solving x(a).\n");
      break;
    }

  } while (fabs(f2) > _SECANT_TOL_);
  
  return x2;
}

/**
 * Non-linear function derived from entropy conservation, root of which is the solution to x(a).
 *
 * @param x   Input: Dimensionless inverse fluid temperature x=m/Td.
 * @param a   Input: Scale factor of interest.
 * @param at  Input: Fluid step scale factor.
 * @param rg  Input: Dimensionless step size.
 * @return Evaluated result at a particular test x.
 */
double _f_(double x, double a, double at, double rg){
  double LHS, RHS;
  LHS = pow(x*at/a,3.);
  RHS = 1. + rg/4*(3*rhohat(x) + phat(x));
  return LHS - RHS;
}

/**
 * Numerically solves x as a function of the scale factor via a secant method root finder.
 *
 * @param x   Input: Dimensionless inverse fluid temperature x=m/Td.
 * @param a   Input: Scale factor of interest.
 * @param at  Input: Fluid step scale factor.
 * @param rg  Input: Dimensionless step size.
 * @return root x of the non-linear function. 
 */
double solve_x_of_a(double a, double at, double rg){
  double guess1 = a/at * 1.1;
  double guess2 = a/at * 0.9;
  return _secant_method(guess1, guess2, a, at, rg);
}

/* End stepped fluid modification */












