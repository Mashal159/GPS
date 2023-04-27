#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define EARTH_RADIUS 6371.0 // in kilometers



float read_GPS_data(char* gpsString) {
    
    // Split GPS string into parts
    char *token;
    token = strtok(gpsString, ",");
    int count = 0;
    float latitude, longitude;
    char *NS , *EW;
    while (token != NULL) {
        count++;
        if(count == 3) {
            // Convert latitude from degrees and minutes to decimal degrees
            char degree[3], minute[10];
            strncpy(degree, token, 2);
            strncpy(minute, token + 2, 8);
            degree[2] = '\0';
            minute[8] = '\0';
            latitude = atof(degree) + atof(minute) / 60;
        }
        else if(count == 4) {
            NS = token;
        }
        else if (count == 5) {
            // Convert longitude from degrees and minutes to decimal degrees
            char degree[4], minute[10];
            strncpy(degree, token, 3);
            strncpy(minute, token + 3, 8);
            degree[3] = '\0';
            minute[8] = '\0';
            longitude = atof(degree) + atof(minute) / 60;
        }
        else if(count == 6) {
            EW = token;
        }
        token = strtok(NULL, ",");
        
    }
    //check the sign of latitude and longitude
    if (*NS == 'S'){latitude= -latitude;}
    if (*EW == 'W' ){longitude= -longitude;}
    float *data; 
    data[0]=latitude;
    data[1]=longitude;
    return data;  //data[0] latitude value data[1] longitude value
}

double toRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

double distance(double lat1, double lon1, double lat2, double lon2) {
    //calculate distance between two points
    double dLat = toRadians(lat2 - lat1);
    double dLon = toRadians(lon2 - lon1);
    double a = sin(dLat / 2) * sin(dLat / 2) + cos(toRadians(lat1)) * cos(toRadians(lat2)) * sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;
}


double distance_inMeter(double KM_dis){
    return KM_dis*1000;
}
    
int GPS_validate(char *nmeastr){
    char check[3];
    char checkcalcstr[3];
    int i;
    int calculated_check;

    i=0;
    calculated_check=0;

    // check to ensure that the string starts with a $
    if(nmeastr[i] == '$')
        i++;
    else
        return 0;

    //No NULL reached, 75 char largest possible NMEA message, no '*' reached
    while((nmeastr[i] != 0) && (nmeastr[i] != '*') && (i < 75)){
        calculated_check ^= nmeastr[i];// calculate the checksum
        i++;
    }

    if(i >= 75){
        return 0;// the string was too long so return an error
    }

    if (nmeastr[i] == '*'){
        check[0] = nmeastr[i+1];    //put hex chars in check string
        check[1] = nmeastr[i+2];
        check[2] = 0;
    }
    else
        return 0;// no checksum separator found there for invalid

    sprintf(checkcalcstr,"%02X",calculated_check);
    return((checkcalcstr[0] == check[0])
        && (checkcalcstr[1] == check[1])) ? 1 : 0 ;
}    
    
    
//...........................................................................................................................
