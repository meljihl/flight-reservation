#include <stdio.h>
#include <string.h>
//#include <api.h>

//xml namespace mapping and gsoap glue code
#include <stdsoap2.h>
#include "re213.nsmap"
#include "api.h"
#include "commons/iatadb.h"
#include "time.h"
#include <pthread.h>


char wsdl_path[512];

// function to change the net price of the flights every 5 minutes 
void *threadproc(void *arg)
{
    while(1)
    {
      int k;
      srand(time(NULL));  
      sleep(3);
      for(k=0;k<MAX_FLIGHTS;k++) {
        listofFlights[k].seats[0].price.net +=rand()%20;
        listofFlights[k].seats[1].price.net +=rand()%20;
        listofFlights[k].seats[2].price.net +=rand()%20;
	
      }
    }

    return 0;
}


int http_get(struct soap *soap) {


    FILE *fd = NULL;
    char *s = strchr(soap->path, '?');
    if (!s || strcmp(s, "?wsdl"))
        return SOAP_GET_METHOD;
    fd = fopen(wsdl_path, "rb"); // open WSDL file to copy
    if (!fd)
        return 404; // return HTTP not found error
    soap->http_content = "text/xml"; // HTTP header with text/xml content
    soap_response(soap, SOAP_FILE);
    for (; ;) {
        size_t r = fread(soap->tmpbuf, 1, sizeof(soap->tmpbuf), fd);
        if (!r)
            break;
        if (soap_send_raw(soap, soap->tmpbuf, r))
            break; // can't send, but little we can do about that
    }
    fclose(fd);
    soap_end_send(soap);
    return SOAP_OK;
}





int main(int argc, char **argv) {


    int m, s; /* master and slave sockets */
    struct soap *soap = soap_new();
    soap_set_imode(soap, SOAP_XML_STRICT);
    soap->fget = http_get;
    int i = 0;
    if (argc < 4) {
        printf("usage: port_number wsdl_path data_file");
        exit(-1);
    } else {
      loadFlightData(argv[3]);
      strcpy(wsdl_path, argv[2]);
      m = soap_bind(soap, NULL, atoi(argv[1]), 100); /* bind to the port supplied as command-line argument */
        if (m < 0) {
            soap_print_fault(soap, stderr);
            exit(-1);
        }
        fprintf(stderr, "Socket connection successful: master socket = %d\n",
                m);
	pthread_t tid;
	int err = pthread_create(&tid, NULL, &threadproc, NULL);
	if(err != 0)
	printf("\ncan't create thread :[%s]", strerror(err));
      

	for (; ;) {	     
	  s = soap_accept(soap);
	  fprintf(stderr, "Socket connection successful: slave socket = %d\n",s);
	  if (s < 0) {
	    soap_print_fault(soap, stderr);
	    exit(1);
	  }
	  soap_serve(soap);
	  
	  soap_end(soap);
	    
	}
	pthread_join(tid, NULL);

    }
    

    soap_done(soap);
    free(soap);

    return 0;
}


//here the implementation of the function is really simple C, except that we need to add a soap* as the first arg.
int re213__getCountry(
       struct soap *soap,
        char *name,
        struct country *country) {

printf("%s----\n",name);
    if (strcmp(name, "Italy") == 0) {
        country->population = 60808000;
        country->capital="Rome";
        country->currency=re213__currency__EUR;
        country->name="Italia";
    }
    else if (strcmp(name, "France")==0) {
        country->population = 66917469;
        country->capital="Paris";
        country->currency=re213__currency__EUR;
        country->name="France";
    }
    else if (strcmp(name, "England")==0) {
        country->population = 53012456;
        country->capital="London";
        country->currency=re213__currency__GBP;
        country->name="England";

    }
    else{
      
	return SOAP_ERR;
    }
    return SOAP_OK;
}

