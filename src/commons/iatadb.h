
#ifndef PROJECT_HEADER_FILE_H
#define PROJECT_HEADER_FILE_H

#include <time.h>
#include <stdlib.h>
#include <string.h>


#define MAX_FLIGHTS 304
#define MAX_AVAILABILITY 30
/**
 * this structure specify a iata airport code as a 3 char + ending code
 */
typedef struct iata_airport_code {
    char code[4];
    
} iata_airport_code;


typedef struct flight_code {
    char airline[3];
    int number;

} flight_code;

typedef struct filght_price {
    double net;
    double tax; 

} flight_price;


/*
 * this structure store the availability for a class (business, first class, economical...)
 */
typedef struct availability {
    char klass;
    size_t count;
    flight_price price;
} availability;


/**
 * a flight segement specify the departure location and time for a piece of flight. Same for arrival
 */
typedef struct flight_segment {
  iata_airport_code departure;
  iata_airport_code arrival;
  time_t time_departure;
  time_t time_arrival;
  char date[6];
  flight_code flightId;
  availability seats[3]; 
} flight_segment;


/**
 * this structure is the response to a findFlights function. It provides segements and availability classes
 */
typedef struct flight_availability {
    flight_segment seg[MAX_AVAILABILITY];
    size_t count_seg;
} flight_availability;


struct flight_segment listofFlights[MAX_FLIGHTS];



/**
 * Load data in the server
 * @param path of the database file
 */ 


int loadFlightData(char * input) {
    FILE *fp = fopen(input, "r");
    const char s[2] = ",";
    char *token;
    int i;
    int k= 0;

    if(fp != NULL)
    {
        char line[100];
	printf("Loading flight database...\n");
        while(fgets(line, sizeof(line), fp) != NULL)
        {
	  if (k > MAX_FLIGHTS-1) {
	    printf("...Finished!\n");
	    break;
	  }
	  
	  token = strtok(line, s);
            for(i=0;i<16;i++)
            {
	      if(i==0)
                {   
		  strcpy(listofFlights[k].flightId.airline,token);
		  token = strtok(NULL,s);
                } 
	      else if(i==1) {
		listofFlights[k].flightId.number = atoi(token);
                token = strtok(NULL,s);
	      }
	      else if(i==2) {
		strcpy(listofFlights[k].arrival.code ,token);
		token = strtok(NULL,s);
	      }
	      else if(i==3) {
		listofFlights[k].seats[0].klass = 'J';
		listofFlights[k].seats[0].count = atoi(token);
                token = strtok(NULL,s);
	      }
	      else if(i==4) {
		listofFlights[k].seats[1].klass = 'Y';
		listofFlights[k].seats[1].count = atoi(token);
                token = strtok(NULL,s);
	      }
	      else if(i==5) {
		listofFlights[k].seats[2].klass = 'M';
		listofFlights[k].seats[2].count = atoi(token);
                token = strtok(NULL,s);
	      }
	      else if(i==6) {		  
		strcpy(listofFlights[k].departure.code,token);
		token = strtok(NULL,s);
	      }
	      else if(i==7) {
		listofFlights[k].time_departure = atoi(token);
		token = strtok(NULL,s);
	      }
	      else if(i==8) {
		listofFlights[k].time_arrival = atoi(token);
		token = strtok(NULL,s);
	      } 
	      else if(i==9) {
		strcpy(listofFlights[k].date,token);
		token = strtok(NULL,s);
	      } 
	      else if(i==10) {
		listofFlights[k].seats[0].price.net = atof(token);
                token = strtok(NULL,s);
	      }
	      else if(i==11) {
		listofFlights[k].seats[1].price.net = atof(token);
                token = strtok(NULL,s);
	      }
	      else if(i==12) {
		listofFlights[k].seats[2].price.net= atof(token);
                token = strtok(NULL,s);
	      }       
	      else if(i==13) {
		listofFlights[k].seats[0].price.tax = atof(token);
                token = strtok(NULL,s);
	      }
	      else if(i==14) {
		listofFlights[k].seats[1].price.tax = atof(token);
                token = strtok(NULL,s);
	      }
	      else if(i==15) {
		listofFlights[k].seats[2].price.tax= atof(token);
                token = strtok(NULL,s);
	      }       
            }
	    k++;
        }
        fclose(fp);
    } else
      perror(input);  
}




/**
 * Look for a flight in the system
 * @param departure iata code for departure airport
 * @param arrival iata code for arrival airport
 * @param date date of the departure. This function will look 3h after the specified date
 * @param availability provides at most 30 availability responses.
 */ 

void findFlights(const char departure[3], const char arrival[3], const char date[5],
                 flight_availability *availability) {
  int k, i = 0;
  for(k=0;k<MAX_FLIGHTS;k++) {
    if(!strcmp(listofFlights[k].arrival.code,departure)
       && !strcmp(listofFlights[k].departure.code,arrival)
       && !strcmp(listofFlights[k].date,date))
      {
	strcpy(availability->seg[i].arrival.code,listofFlights[k].arrival.code );
	strcpy(availability->seg[i].departure.code,listofFlights[k].departure.code);
	availability->seg[i].time_arrival = listofFlights[k].time_arrival;
	availability->seg[i].time_departure = listofFlights[k].time_departure;
	strcpy(availability->seg[i].date, listofFlights[k].date);
	strcpy(availability->seg[i].flightId.airline,listofFlights[k].flightId.airline);
	availability->seg[i].flightId.number = listofFlights[k].flightId.number;
	availability->seg[i].seats[0].klass = listofFlights[k].seats[0].klass;
	availability->seg[i].seats[0].count = listofFlights[k].seats[0].count;
	availability->seg[i].seats[1].klass = listofFlights[k].seats[1].klass;
	availability->seg[i].seats[1].count = listofFlights[k].seats[1].count;
	availability->seg[i].seats[2].klass = listofFlights[k].seats[2].klass;
	availability->seg[i].seats[2].count = listofFlights[k].seats[2].count;

	availability->seg[i].seats[0].price.net = listofFlights[k].seats[0].price.net;
	availability->seg[i].seats[0].price.tax = listofFlights[k].seats[0].price.tax;
	availability->seg[i].seats[1].price.net = listofFlights[k].seats[1].price.net;
	availability->seg[i].seats[1].price.tax = listofFlights[k].seats[1].price.tax;
	availability->seg[i].seats[2].price.net = listofFlights[k].seats[2].price.net;
	availability->seg[i].seats[2].price.tax = listofFlights[k].seats[2].price.tax;
	i++;
	if(i>=MAX_AVAILABILITY) perror("Error: Data overflow.\n");
      }
  }
  availability->count_seg = i;
};


/**
 * 
 * Price the flight in the system
 * @param code for the airline
 * @param flightNum the char containing the flight number
 * @param time the time of departure
 * @param klass the confort class of the flight
 * @return the prices of the flight 
 */
flight_price priceSegment(char *airline, int flightNumber, const time_t time_departure,const char * date, const char klass) {
   int k;
  for(k=0;k<MAX_FLIGHTS;k++) {
    if(!strcmp(listofFlights[k].flightId.airline,airline)
       && listofFlights[k].flightId.number==flightNumber	
       && listofFlights[k].time_departure==time_departure
       && !strcmp(listofFlights[k].date,date))
      {
	switch (klass) {
	case 'J':	
	  return listofFlights[k].seats[0].price;
	
	case 'Y':	
		return listofFlights[k].seats[1].price;
	case 'M':	
		return listofFlights[k].seats[2].price;
	default :
		printf("Invalid class type!\n");
	
	}
      }
  }
}

#endif //PROJECT_HEADER_FILE_H
