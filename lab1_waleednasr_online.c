/* Lab 1 Graded - CIT315
 * Name: Waleed Nasr
 * Email: wnasr@purdue.edu
 * Lab time: Online
 *
 * General Documentation: Make machine go BRR and calculate the volume, gallons, and cost of filling a swiming pool with
 * water
 */

#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI acos(-1.0)
#endif

#ifndef M_LEVEL
#define M_LEVEL 0.5
#endif

//function used to take three arguments and handle the input verification and checking. Modular enough so that it fits
//all program needs.
float returnCleanOutput(char msg[], float min, float max){
    float input;
    int flag = 1;

    while (flag == 1){
        printf("%s (%.1f -> %.1f feet):", msg, min, max);

        //using the output of scanf to determine proper input...
        if (scanf("%f", &input) == 0) {
            printf("This is not a number, please try again...\n");
        }else if(input < min || input > max){
            printf("Please input a value between %f and %f...\n", min, max);
        }else{
            flag =0;
        }
        fflush(stdin);
    }

    return input;
}

//Calculates the first part of the swimming pool, the deep area...
float returnVolumeDeep(float depth, float width, float length){
    //printf("%f", depth*width*length);
    return (depth*width*length);
}

//Calculates the second part of the swimming pool, the transition.
float returnVolumeTransition(float depthLong, float depthShort, float width, float length){
    //printf("%f",((depthLong + depthShort) * (length/2.0)) * width);
    return (((depthLong + depthShort) * (length/2.0)) * width);
}

//Calculates the third part of the swimming pool, the shallow area
float returnVolumeShallow(float depth, float width, float length){
    //printf("%f",depth*width*length);
    return (depth*width*length);
}

//Calculates the fourth part of the swimming pool, the walk-in
float returnVolumeStepin(float depth, float width, float length){
    //printf("%f",((depth*length)/2.0) * width);
    return (((depth*length)/2.0) * width);
}

//Calculates the hot tub.
float returnHotTub(float width, float height){
    float radius = width/2.0;
    return (radius * radius * M_PI * height);
}

int main() {
    //declaration of all variables.
    int flag = 1;
    int again;

    //dimensions variables
    float width;
    float deepLength;
    float walkInLength;
    float transitionLength;
    float shallowLength;
    float largeDepth;
    float smallDepth;
    float hotWidth;
    float hotHeight;
    float totalLength;

    //calculation variables
    float poolVolume;
    float poolGallons;
    float tubVolume;
    float tubGallons;
    float totalCost;

    while (flag == 1){
        //Assigning a value to each variable using the returnCleanOutput() function...
        smallDepth = returnCleanOutput("Please enter the Depth of the shallow end of the pool",0.0, 5.0);
        largeDepth = returnCleanOutput("Please enter the Depth of the deep end of the pool",6.0, 15.0);
        width = returnCleanOutput("Please enter the Width of the pool",15.0, 30.0);
        totalLength = returnCleanOutput("Please enter Length of the pool",35.0, 70.0);
        walkInLength = returnCleanOutput("Please enter Length of the walk-in",5.0, ((1.0/3.0)*totalLength));
        shallowLength = returnCleanOutput("Please enter Length of the shallow end",10.0, ((1.0/2.0)*totalLength));
        deepLength = returnCleanOutput("Please enter Length of the deep end",12.0, ((1.0/2.0)*totalLength));
        hotWidth = returnCleanOutput("Please enter Width of the hot tub",8.0, 14.0);
        hotHeight = returnCleanOutput("Please enter Depth of the hot tub",3.0, 5.0) - M_LEVEL;
        transitionLength = totalLength - shallowLength - deepLength - walkInLength;

        //Calculating the total volume of the swimming pool...
        poolVolume = returnVolumeDeep(largeDepth, width, deepLength) +
                returnVolumeTransition(largeDepth, smallDepth, width, transitionLength) +
                returnVolumeShallow(smallDepth, width, shallowLength) +
                returnVolumeStepin(smallDepth, width, walkInLength);

        //Subtracting the total with the 6 inch difference
        poolVolume -= totalLength*width*M_LEVEL;

        //Calculating the hot tub volume
        tubVolume = returnHotTub(hotWidth, hotHeight);

        //Calculating the gallons
        poolGallons = poolVolume * 7.481;
        tubGallons = tubVolume * 7.481;

        //Calculating total cost
        totalCost = (poolGallons + tubGallons) * 0.28;

        //Presenting all the info
        printf("Pool Dimensions (feet)\n"
               "Depth of the shallow end: %.1f\n"
               "Depth of the deep end: %.1f\n"
               "Width of the pool: %.1f\n"
               "Length of the pool: %.1f\n"
               "Length of the walk-in: %.1f\n"
               "Length of the shallow end: %.1f\n"
               "Length of the deep end: %.1f\n"
               "Width of the hot tub: %.1f\n"
               "Depth of the hot tub: %.1f\n",
               smallDepth, largeDepth, width, totalLength, walkInLength, shallowLength, deepLength, hotWidth,hotHeight);

        printf("------------------------------\nPool Volume\n"
               "Total volume of pool: %.1f\n"
               "Gallons of water to fill: %.1f\n"
               "Hot Tub Volume\n"
               "Total volume of hot tub: %.1f\n"
               "Gallons of water to fill: %.1f\n"
               "Total gallons for both: %.1f\n"
               "Total cost for both: $%.2f\n",
               poolVolume, poolGallons, tubVolume, tubGallons, poolGallons+tubGallons, totalCost);

        //Logic to handle reset
        printf("Try again? (1=y / 0=n): ");

        if (scanf("%i", &again) == 1) {
            if (again == 1){
                continue;
            }else{
                flag = 0;
            }
        }
        fflush(stdin);

    }


    return 0;
}