#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define EARTH_RADIUS 6371.0 // in kilometers



void read_GPS_data(char* gpsString, float* latitude, float* longitude, float* time, float* speed) {
    
    // Split GPS string into parts
    char *token;
    token = strtok(gpsString, ",");
    int count = 0;
    char *NS , *EW;
    
    while (token != NULL) {
        count++;
        switch (count) {
        case 2:
            *time = atof(token);
            break;
        case 4:
            // Convert latitude from degrees and minutes to decimal degrees
            char degree1[3], minute1[10];
            strncpy(degree1, token, 2);
            strncpy(minute1, token + 2, 8);
            degree1[2] = '\0';
            minute1[8] = '\0';
            *latitude = atof(degree1) + atof(minute1) / 60;
            break;
        case 5:
            NS = token;
            break;
        case 6:
            // Convert longitude from degrees and minutes to decimal degrees
            char degree2[4], minute2[10];
            strncpy(degree2, token, 3);
            strncpy(minute2, token + 3, 8);
            degree2[3] = '\0';
            minute2[8] = '\0';
            *longitude = atof(degree2) + atof(minute2) / 60;
            break;
            
        case 7:
            EW = token;
            break;
        case 8:
            *speed = atof(token) * 0.514444;
            break;}
        token = strtok(NULL, ",");
        
    }
    //check the sign of latitude and longitude
    if (*NS == 'S'){*latitude= -*latitude;}
    if (*EW == 'W' ){*longitude= -*longitude;}
    float* data = malloc(4 * sizeof(float)); 
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
    
int gpsValidate(char *gpsString){
    char check[3];
    char checkcalcstr[3];
    char *AV;
    int i;
    int calculated_check;
    int checksumcheck , AVcheck;
    i=0;
    calculated_check=0;

    // check to ensure that the string starts with a $
    if(gpsString[i] == '$')
        i++;
    else
        return 0;

    //No NULL reached, 75 char largest possible NMEA message, no '*' reached
    while((gpsString[i] != 0) && (gpsString[i] != '*') && (i < 75)){
        calculated_check ^= gpsString[i];// calculate the checksum
        i++;
    }

    if(i >= 75){
        return 0;// the string was too long so return an error
    }

    if (gpsString[i] == '*'){
        check[0] = gpsString[i+1];    //put hex chars in check string
        check[1] = gpsString[i+2];
        check[2] = 0;
    }
    else
        return 0;// no checksum separator found there for invalid

    sprintf(checkcalcstr,"%02X",calculated_check);
    checksumcheck = ((checkcalcstr[0] == check[0]) && (checkcalcstr[1] == check[1])) ? 1 : 0 ;
    
    char *token;
    token = strtok(gpsString, ",");
    int count = 0;
    
    while (token != NULL) {
        count++;
        if (count == 3){ 
            AV = token;
        }
        token = strtok(NULL, ",");
    }
    AVcheck = (*AV=='A') ? 1 : 0;
    return(checksumcheck && AVcheck)? 1 : 0; 
    
} 
    
    
//...........................................................................................................................
