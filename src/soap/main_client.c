#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//xml namespace mapping and gsoap glue code
#include "re213.nsmap"


int main(int argc, char** argv) {

	//create soap environement
	struct soap *soap = soap_new();

	/* Short example of a country request from user. */
	/************************************************/
	//fill the request with the name of the country
	char countryStr[256];
	struct country country;
	printf("Demonstrating a small example with countries!\n");
	printf("Enter a country: ");
	fgets(countryStr,255,stdin);
	strtok(countryStr,"\n");
	/* Here is a call to a remote procedure, so it starts with soap_call_ followed by the name of the function. */
	int soap_return_code=soap_call_re213__getCountry(soap, NULL, NULL, countryStr, &country);
	if (soap_return_code == SOAP_OK)
	  //ok! response properly filled
	  printf("Population: %d\nCapital: %s\nCurrency: %d\n", country.population,country.capital,country.currency);
	else {
	  printf("Possible incorrect or missing data from the sender : %d\n",soap_return_code );
	  soap_print_fault(soap, stderr);
	}
	printf("Example is finished :)\n");
	/************************************************/

	printf("----------------------------------------.\n"); 
	printf("Welcome to our TravelReservation Service!\n"); 
	printf("------------------------------------------\n");      
	/* Infinate loop where client handles user's input commands*/
	while(1) {

	}

	//cleanup
	soap_free(soap);
	return 1;
};

