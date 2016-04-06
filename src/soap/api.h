#ifndef _PROJECT_API_H_
#define _PROJECT_API_H_
#include <stdsoap2.h>

enum currency { re213__currency__GBP = 0, re213__currency__EUR = 1, re213__currency__PLN = 2 };

struct country
{
    char *name;	/* required element of type xsd:string */
    int population;	/* required element of type xsd:int */
    char *capital;	/* required element of type xsd:string */
    enum currency currency;	/* required element of type ns1:currency */
};


//here we have the prototype of our function. Nothing special except the re213__ in front of it. It is done to prevent
//name clashes.

//gsoap re213 service name: re213
//gsoap re213 service style: rpc
//gsoap re213 service port: http://localhost:8080/
//gsoap re213 service namespace: urn:re213
int  re213__getCountry(
        char* name,
        struct country *country);


//int re213__getStudentBySSN(char *SS, char* studentName);




//int re213__getFlight(char* name,struct flight *re213_flight);

#endif //_PROJECT_API_H_

