#include "common.h"

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
 * Analytical expressions for approximating modified Bessel function of the second kind at
 * integer order 0. This is needed later for obtaining the same function of order 1 via recurrence
 * relations. This expression was derived by Martin (2017).
 *
 * @param x  Input: Input
 * @return approximation result.
 */
double K0(double x){
  double cosh = 0.5 * (pow(_E_, x) + pow(_E_, -x)); // Hyperbolic cosine expressed as exponentials.
  double term1 = 1.0 / ((1.0+0.72763*pow(x,2.)) * pow(1.0+0.08250*pow(x,2.),5./4.) * cosh);
  double term2 = 0.11593 - 0.05064*pow(x,2.) - 0.00470*pow(x,2.)*sqrt(1+0.28723*pow(x,2.)) - 0.5 * log(pow(x,2.)/(1.+pow(x,2.))) * (1.+1.57580*pow(x,2.)+0.78684*pow(x,4.)+0.04032*pow(x,6.));
  return term1*term2;
}

/**
 * Analytical expressions for approximating modified Bessel function of the second kind at
 * integer order 2. This is needed later for obtaining the same function of order 1 via recurrence
 * relations, as well as computing the Maxwell-Boltzmann integral results. This expression was 
 * derived by Caruso & Silveira (2020).
 *
 * Note that their work also included analytical approximations for K0 and K1. However, the 
 * fractional errors on those expressions are larger than the tolerance of our work.
 *
 * @param x  Input: Input
 * @return approximation result.
 */
double K2(double x){
  return pow(_E_, -x) * (784./1615. +
                         2./pow(x,2.) +
                         3232./(1615.*x) -
                         448.*x/4845. +
                         5416744.*pow(x,2.)/190855665. -
                         93376.*pow(x,3.)/14549535. +
                         27424*pow(x,4.)/31177575. -
                         512.*pow(x,5.)/8083075. +
                         4.*pow(x,6.)/2204475.);
}

/**
 * Analytical expressions for the  modified Bessel function of the second kind at integer order 1. 
 * This is needed to compute the Maxwell-Boltzmann integral results. Besides using approximated 
 * results for K0 and K2 as inputs, the recurrence relation used here to obtain K1 is exact and 
 * follows from the properties of modified Bessel functions
 *
 * $K_{n+1}(x) = K_{n-1}(x) + \frac{2n}{x}K_n(x)$\, ,
 *
 * where we use n=1 and rearranged to obtain an expression for K1 in terms of K2 and K0.
 *
 * @param x  Input: Input
 * @return result.
 */
double K1(double x){
  return x/2. * (K2(x)-K0(x));
}

/**
 * Dimensionless Maxwell-Boltzmann density integral.
 *
 * @param x  Input: Dimensionless inverse fluid temperature x=m/Td.
 * @return analytic result in polynomial and modified bessel function (2nd kind) terms.
 */
double rhohat(double x){
  //double K1 = gsl_sf_bessel_Kn(1, x); // OBSOLETE: Unusable by python wrapper
  //double K2 = gsl_sf_bessel_Kn(2, x); // OBSOLETE: Unusable by python wrapper  
  return pow(x, 2.)/2*K2(x) + pow(x, 3.)/6*K1(x);
}

/**
 * Dimensionless Maxwell-Boltzmann pressure integral.
 *
 * @param x  Input: Dimensionless inverse fluid temperature x=m/Td.
 * @return analytic result in polynomial and modified bessel function (2nd kind) terms.
 */
double phat(double x){  
  //double K2 = gsl_sf_bessel_Kn(2, x); // OBSOLETE: Unusable by python wrapper  
  return pow(x, 2.)/2*K2(x);
}


/** 
 * A very makeshift secant method non-linear function root finder. May be improved in the future.
 *
 * @param point1   Input: Solution guess 1.
 * @param point2   Input: Solution guess 2.
 * @param params   Input: Array of doubles required by target entropy function. 
 * @param *entrop  Input: Entropy function to find root of.
 * @return roots x of the target entropy function. 
 */ 
double secantMethod(double point1, double point2, double* params, double (*entrop)(double, double*)){
  double res, f0, f1, f2;
  int iteration = 0;
    
  do {
    f0 = entrop(point1, params);
    f1 = entrop(point2, params);

    res = point2 - ((f1 * (point2 - point1)) / (f1 - f0));
    f2 = entrop(res, params);

    point1 = point2;
    point2 = res;

    iteration++;
    
    if (iteration >= _SECANT_MAX_){
      printf("Maximum iterations exceeded in solving x(a).\n");
      break;
    }

  } while (fabs(f2) > _SECANT_TOL_);
  
  return res;
}

/**
 * Non-linear function derived from entropy conservation for a fluid with one step (WZDR).
 * Root is the solution to x(a).
 * 
 * @param x   Input: Dimensionless inverse fluid temperature x=m/Td.
 * @param params   Input: Array of doubles containing necessary quantities as follows: 
 *     @param a   Input: Current scale factor.
 *     @param at  Input: Fluid transition scale factor.
 *     @param rg  Input: Dimensionless step size.
 * @return Evaluated result at a particular test x.
 */
double entrop_one_step(double x, double* params){
  double LHS, RHS;
  double a = params[0];
  double at = params[1];
  double rg = params[2];

  LHS = pow(x*at/a,3.);
  RHS = 1. + rg/4*(3*rhohat(x) + phat(x));
  return LHS - RHS;
}

/**
 * Non-linear function derived from entropy conservation for a fluid with two steps.
 * Root is the solution to x1(a).
 * 
 * @param x1   Input: Dimensionless inverse fluid temperature x1=m1/Td.
 * @param params   Input: Array of doubles containing necessary quantities as follows: 
 *     @param a    Input: Current scale factor.
 *     @param at1  Input: First fluid transition scale factor.
 *     @param at2  Input: Second fluid transition scale factor.
 *     @param rg   Input: Dimensionless step size.
 * @return Evaluated result at a particular test x1.
 */
double entrop_two_step(double x1, double* params){
  double LHS, RHS;
  double a = params[0];
  double at1 = params[1];
  double at2 = params[2];
  double rg = params[3];
  double rg2 = params[4];
  double x2 = x1*at1/at2;

  LHS = pow(x1*at1/a,3.);
  //RHS = 1. + rg/4*(3*(rhohat(x1)+rhohat(x2)) + (phat(x1)+phat(x2)));
  RHS = 1. + 1./4.*(rg*(3*rhohat(x1)+phat(x1)) + rg2*(3*rhohat(x2)+phat(x2)));
  return LHS - RHS;
}

/**
 * Numerically solves x as a function of the scale factor via a secant method root finder.
 *
 * @param params   Input: Array of doubles containing necessary quantities as follows: 
 *     @param a    Input: Current scale factor.
 *     @param at1  Input: First fluid transition scale factor.
 *     @(optional)param at2  Input: Second fluid transition scale factor.
 *     @param rg   Input: Dimensionless step size. 
 * @return root x of the non-linear function. 
 */
double solve_x_of_a(double* params, int paramSize){
  double a = params[0];
  double at = params[1];
  
  // Initial guesses for x (or x1)
  double guess1 = a/at * 1.1;
  double guess2 = a/at * 0.9;
  
  // Case for single step fluid.
  if (paramSize == 3){    
    return secantMethod(guess1, guess2, params, entrop_one_step);    
  } else {
    // Case for solving x1 for two stepped fluid.
    return secantMethod(guess1, guess2, params, entrop_two_step); 
  }  
}

/* End stepped fluid modification */












