/*
 * Names: Graham Phenegar, Zak Dildine, Waleed Nasr, Lorenzo Parkins, Ted Hamacher
 * Emails: gphenega@purdue.edu, zdildine@purdue.edu, wnasr@purdue.edu, "Parkins, lparkin@purdue.edu, thamache@purdue.edu
 * Course: CNIT 31500 In Person / Online
 * Instructors: Dr. Eric Matson, Hyemin Hanna Kim
 * Assignment:  Team Project
 *
 * Program description:
 * Hangman game that uses the opencagedata api to get the latitute and longitude of the user's input. Then selected a
 * state based on that.
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#define _EARTHRAD 6371000 //in meters
#define _RADIANM M_PI/180
#define _LIVES 6
#define _BITS  64

//Getting the distance between two search results:
//This website gives the calculations based on latitude and longitude
// https://www.movable-type.co.uk/scripts/latlong.html
//Google's API can return the latitude and longitude of a search
//pre-store the latitude and longitude of each predefined city.

struct coordinates{
    double latitude;
    double longitude;
    char country[30];
};

struct MemoryStruct {
    char *memory;
    size_t size;
};

//a student struct
struct sScore{
    char country[_BITS];
    char attempt[_BITS];
    int score;
    struct sScore *nextNode;
};

//a struct for keeping updated data about the list, probably not as robust but im lazy
struct linkedList{
    struct sScore *start;
    struct sScore *end;
    int listSize;
};

double calculateDistance(struct coordinates, struct coordinates);
struct coordinates returnClosestCity(struct coordinates);
struct coordinates game(struct coordinates, struct linkedList *main);
char myinfo();
char optSelect();
void secondMenu();
struct coordinates getUserState();
void clear();
void hangman(int lives , const char update[]);
void printList(struct linkedList *list);


int showList();

int main() {
    char secondInput[30];

    struct linkedList *mainList;
    struct sScore *first;
    first = malloc(sizeof(struct sScore));

    strcpy(first->country, "None");
    first->nextNode = NULL;
    first->score = 0;
    strcpy(first->attempt, "None");


    mainList->start = first;
    mainList->end = first;
    mainList->listSize = 1;

    while(myinfo() == '1'){
        while(optSelect() == '1'){
            secondMenu();
            fflush(stdin);
            scanf("%s", &secondInput);
            clear();
            game(returnClosestCity(getUserState(secondInput)), mainList);
        }
    }

    struct sScore *oldStart = mainList->start;
    mainList->start = oldStart->nextNode;
    --mainList->listSize;

    printList(mainList);


    return 0;
}

int showList() {
    char input;

    printf("See Scores? 1 for yes other for no\n");
    scanf(" %c", &input);
    return input;
}

void printStudent(struct sScore *score){
    if (score==NULL){
        printf("INVALID STUDENT");
    }else{
        printf("Country: %s\n"
               "attempt: %s\n"
               "score: %d\n"
               ,score->country, score->attempt, score->score);
    }
}

//Handles going through each student and calling printStudent()
void printList(struct linkedList *list){
    struct sScore *counter;
    counter = list->start;

    if(counter == NULL){
        printf("List is Empty\n");
        return;
    }
    while(counter) {
        printStudent(counter);
        counter = counter->nextNode;
    }
}

struct coordinates returnClosestCity(struct coordinates city){
    int arraysize = 4;
    struct coordinates countries[arraysize];
    //hardcode
    countries[0].latitude = 36.778261, countries[0].longitude = -119.417932, strcpy(countries[0].country,"california");
    countries[1].latitude = 40.267194, countries[1].longitude = -86.134902, strcpy(countries[1].country,"indiana");
    countries[2].latitude = 39.045755, countries[2].longitude = -76.641271, strcpy(countries[2].country,"maryland");
    countries[3].latitude = 47.751074, countries[3].longitude = -120.740138, strcpy(countries[3].country,"washington");

    //fgf
    double distanceMin = calculateDistance(countries[0], city);
    double currentDist = 0;
    int currentCity = 0;
    int i=0;

    for (i = 0; i < arraysize; i++){
        if(i < arraysize){
            currentDist = calculateDistance(countries[i], city);
            //printf("Distance from %s to %s is: %f\n", countries[i].country, city.country, currentDist);
            if(currentDist < distanceMin){
                distanceMin = currentDist;
                currentCity = i;
            }
            printf("%f", i);
        }
    }

    //printf("Selected city: %s\n", countries[currentCity].country);
    return countries[currentCity];
}

double calculateDistance(struct coordinates target, struct coordinates city){
    double latitudeTargetRad = target.latitude * _RADIANM;
    double latitudeCityRad = city.latitude * _RADIANM;

    double latitudeDeltaRad = (city.latitude-target.latitude) * _RADIANM;
    double longitudeDeltaRad = (city.longitude-target.longitude) * _RADIANM;

    double haversine = pow(sin(latitudeDeltaRad/2.0),2.0) +
                       cos(latitudeTargetRad) * cos(latitudeCityRad) *
                       pow(sin(longitudeDeltaRad/2.0),2.0);

    double angularDistance = 2.0 * atan2(sqrt(haversine), sqrt(1.0-haversine));

    return _EARTHRAD * angularDistance;
}

struct coordinates game(struct coordinates city, struct linkedList *mainList){
    int i;
    int sizeOfWord = strlen(city.country);
    char letterInput;
    int lives = _LIVES;
    int subtractLives = 0;
    char guessedWord[sizeOfWord];

    struct sScore *newStart;
    newStart = malloc(sizeof(struct sScore));
    struct sScore *oldStart = mainList->start;


    for(i=0; i<sizeOfWord; i++){
        guessedWord[i]= *"_";
        guessedWord[i+1] = '\0';
    }

    while(1){
        hangman(lives, guessedWord);
        //checks if game is ended with no missing letters
        if (strchr((const char *) &guessedWord, '_') == NULL){
            //stores gamedata
            strcpy(newStart->attempt, guessedWord);
            newStart->score = lives;
            newStart->nextNode = NULL;
            strcpy(newStart->country, city.country);

            oldStart->nextNode = newStart;
            mainList->end = newStart;
            mainList->listSize += 1;

            printf("Game Over\n");
            break;
        }

        fflush(stdin);
        printf("Guess next letter:\n");
        scanf(" %c", &letterInput);


        //check if letter in word
        for(i=0; i<sizeOfWord; i++){
            if(city.country[i] == letterInput){
                guessedWord[i] = letterInput;
                subtractLives = 1;
            }
        }

        if(subtractLives == 0){
            --lives;
        }
        subtractLives = 0;

        if(lives == 0){
            hangman(lives, guessedWord);

            //stores gamedata
            strcpy(newStart->attempt, guessedWord);
            newStart->score = lives;
            newStart->nextNode = NULL;
            strcpy(newStart->country, city.country);

            oldStart->nextNode = newStart;
            mainList->end = newStart;
            mainList->listSize += 1;
            printf("You failed\n");
            break;
        }
    }

}

void hangman(int lives, const char updatedstring[]) {
    clear();
    int i;
    char hangmanString[236] = "***             |                                      ***\n"
                              "***             |                                      ***\n"
                              "***             |                                      ***\n";

//idk probs smarter way
    if(lives < 6){
        hangmanString[26] = 'O';
        if(lives < 5){
            hangmanString[84] = '/';
            if(lives < 4){
                hangmanString[85] = '|';
                if(lives < 3){
                    hangmanString[86] = '\\';
                    if(lives < 2){
                        hangmanString[143] = '/';
                        if(lives < 1){
                            hangmanString[145] = '\\';
                        }
                    }
                }
            }
        }
    }

    printf("**********************************************************\n");
    printf("************************Lives: %i**************************\n", lives);
    printf("***                                                    ***\n");
    printf("***             x=========+                            ***\n");
    printf("***             |         |                            ***\n");
    printf("%s", hangmanString);
    printf("***             |                                      ***\n");
    printf("***             |                                      ***\n");
    printf("***         --------------------------------           ***\n");
    printf("***                                                    ***\n");
    printf("***  Progress:%s\n", updatedstring);
    printf("**********************************************************\n");
}

/* https://stackoverflow.com/a/784455  got this bit from this guy <---*/
void reverse_string(char *str)
{
    char *start = str;
    char *end = start + strlen(str) - 1;
    char temp;

    while (end > start)
    {
        /* swap */
        temp = *start;
        *start = *end;
        *end = temp;
        /* move */
        ++start;
        --end;
    }
}

char *filterText(char *data, char *from, char *to){
    char *dataC = data;

    char totalText[strlen(strstr(dataC, from))];
    strcpy(totalText, strstr(dataC, from));
    reverse_string(totalText);


    char subText[strlen(strstr(totalText, to))];
    strcpy(subText, strstr(totalText, to));
    reverse_string(subText);

    char *point = subText;

    return point;
}

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

struct coordinates getUserState(char input[30]){
    char linkToSend[300];
    struct coordinates returnCORD;
    char firstPart[]= "https://api.opencagedata.com/geocode/v1/xml?q=";
    char lastPart[]= "&countrycode=us&key=ca0ccb1249ea40c2b4526c933fb35f66&limit=1&no_annotations=1";

    strcpy(linkToSend, firstPart);
    strcat(linkToSend, input);
    strcat(linkToSend, lastPart);

    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);   //will be grown as needed by the realloc above
    chunk.size = 0;     //no data at this point

    curl_global_init(CURL_GLOBAL_ALL);

    // init the curl session
    curl_handle = curl_easy_init();

    // specify URL to get
    curl_easy_setopt(curl_handle, CURLOPT_URL, linkToSend);

    // send all data to this function
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    // we pass our 'chunk' struct to the callback function
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    // some servers don't like requests that are made without a user-agent
    //   field, so we provide one
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    // get it!
    res = curl_easy_perform(curl_handle);

   //  check for errors
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }
    else {
        char *lat, *subO, *lon, *eptrOne, *eptrTwo;
        char latitude[100], longitude[100];
        int i;

        subO = filterText(chunk.memory, "<geometry>", ">yrtemoeg/<");
        char formatedData[strlen(subO)];
        strcpy(formatedData, subO);

        lat = filterText(formatedData, "<lat>", ">tal/<");
        strcpy(latitude, lat);

        lon = filterText(formatedData, "<lng>",">gnl/<");
        strcpy(longitude, lon);

        for(i =0 ; i < 5; i++){
            latitude[i] = ' ';
            longitude[i] = ' ';
        }

        returnCORD.longitude = strtod(longitude, &eptrOne);
        returnCORD.latitude = strtod(latitude, &eptrTwo);
    }

    // cleanup curl stuff
    curl_easy_cleanup(curl_handle);

    free(chunk.memory);

    // we're done with libcurl, so clean it up
    curl_global_cleanup();
    return returnCORD;
}

/* this is a function that displays our info, assignment info, and how to enter or exit the program*/
char myinfo() {
    char input;
    printf("                ____                 __                   \n"
           "               / __ \\__  ___________/ /_  _____           \n"
           "              / /_/ / / / / ___/ __  / / / / _ \\          \n"
           "             / ____/ /_/ / /  / /_/ / /_/ /  __/          \n"
           "            /_/    \\__,_/_/   \\__,_/\\__,_/\\___/           \n"
           "                                                          \n"
           "       Name: Graham Phenegar, Zak Dildine, Waleed Nasr      \n"
           "               Lorenzo Parkins, Ted Hamacher \n"
           "                 **** CNIT 31500 Project ****               \n"
           "                    The final project                 \n"
           "           --------------------------------------         \n"
           "Press 0 then enter to exit, or press 1 then enter to start \n"
           ">>> ");
    scanf(" %c", &input);
    return input;

}

/* this is a function to display options on which calculation the user  would like to carry out*/
char optSelect() {
    char input;
    printf(" **********************************************************\n");
    printf(" **********************************************************\n");
    printf(" ***                                                    ***\n");
    printf(" ***   This is a simple game of hangman                 ***\n");
    printf(" ***                                                    ***\n");
    printf(" ***   press 1 for hangman                              ***\n");
    printf(" ***   press 2 to terminate                             ***\n");
    printf(" ***                                                    ***\n");
    printf(" **********************************************************\n");
    printf(" **********************************************************\n>>> ");
    scanf(" %c", &input);
    return input;
}
void secondMenu(){

    printf("     ,__                                                  _, \n");
    printf("  \\~\\|  ~~---___              ,                          | \\ \n");
    printf("   |      / |   ~~~~~~~|~~~~~| ~~---,                  _/,  > \n");
    printf("  /~-_--__| |          |     \\     / ~\\~~/          /~| ||,' \n");
    printf("  |       /  \\         |------|   {    / /~)     __-  ',|_\\, \n");
    printf(" /       |    |~~~~~~~~|      \\    \\   | | '~\\  |_____,|~,-' \n");
    printf(" |~~--__ |    |        |____  |~~~~~|--| |__ /_-'     {,~ \n");
    printf(" |   |  ~~~|~~|        |    ~~\\     /  `-' |`~ |~_____{/ \n");
    printf(" |   |     |  '---------,      \\----|   |  |  ,' ~/~\\,|` \n");
    printf(" ',  \\     |    |       |~~~~~~~|    \\  | ,'~~\\  /    | \n");
    printf("  |   \\    |    |       |       |     \\_-~    /`~___--\\ \n");
    printf("  ',   \\  ,-----|-------+-------'_____/__----~~/      / \n");
    printf("   '_   '\\|     |      |~~~|     |    |      _/-,~~-,/ \n");
    printf("     \\    |     |      |   |_    |    /~~|~~\\    \\,/ \n");
    printf("      ~~~-'     |      |     `~~~\\___|   |   |    / \n");
    printf("          '-,_  | _____|          |  /   | ,-'---~\\ \n");
    printf("              `~'~  \\             |  `--,~~~~-~~,  \\ \n");
    printf("                     \\/~\\      /~~~`---`         |  \\ \n");
    printf("                         \\    /                   \\  | \n");
    printf("                          \\  |                     '\\' \n");
    printf("                           `~' \n");
    printf("Please enter a City or State below.\n");
}
/* this is a function to display options to either go back or to quit*/
void optSelectAlt1() {
    printf(" **********************************************************\n");
    printf(" **********************************************************\n");
    printf(" ***                                                    ***\n");
    printf(" ***   Would you like to go back to the main menu?      ***\n");
    printf(" ***                                                    ***\n");
    printf(" ***                press 1 to go back                  ***\n");
    printf(" ***                press 2 to terminate                ***\n");
    printf(" ***                                                    ***\n");
    printf(" **********************************************************\n");
    printf(" **********************************************************\n");
}

/* this function clears the terminal. I don't fully understand it yet, but
   i do know that it is an ANSI escape sequence code that moves the cursor
   to the top left corner of the screen and then clears the screen
   starting from the point of the cursor. I found out how I could do this at
   https://www.xspdf.com/help/50482672.html
   and i found a better explanation of it at
   https://stackoverflow.com/questions/55672661/what-this-character-sequence-033h-033j-does-in-c */
void clear() {
    printf("\033[H\033[J");
}