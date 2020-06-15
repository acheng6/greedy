#include <iostream>
#include "Color.h"
#include "Bridges.h"
#include <string>
#include <fstream>
#include <array>
#include "ColorGrid.h"

using namespace std;
using namespace bridges;
using std::string;

int * j ; //pointer to our array (universal)
int length; //will determine length for 2-d output
int width; //will determine width for 2-d output
int max_number; //holds the max number
int min_number; // hold the minimum number
int min_;   //the absolute value that will be added
//to the negative numbers to scale correctly

/**
 This method is used for scaling the picture to the correct size. We take inside the parameters
 our array that was scaled, our max_number to use to scale the picture
 and our width and length to build it correctly.
 We return the pointer to our array.
 */
int * scaling(int *j, int max_number, int width, int length){
    for(int i = 0 ; i <(width*length); i++) {
        
        j[i] = (((double)j[i]/max_number)* 255);
    }
    return j;
}
/**
 This is the primary greedyAlgorithm where it takes in a value representing
 our left-most point and then travel through to our right-side point.
 
 */

void greedyAlgorithm(int userInput) {
    //userinput is out starting point and is our
    //starting point for the leftmost part of the array
    //first find the number that corresponds to that array element.
    //Will set that arrayElement that is chosen to -1 so when
    //found in main, it will draw a REDline.
    int i = userInput* width;
    for(int x = 0; x < width; x++) {
        int value1 ;
        int value3 ;
        if((i-width+1)<0)
        {
            value1 = 99999;
        }
        else{
            value1 = j[i-width+1];
        }
        
        if((i+width+1)>(length*width)){
            value3 = 99999;
        }
        else{
            value3 = j[i+width+1];
        }
        int value2 = j[i+1];
        
        j[i] = -1;
        if(value1 < value2 && value1<value3) {
            j[i-width+1] = -1;
            i = i-width+1;
        }
        else if (value2 <value1 && value2< value3){
            j[i+1] = -1;
            i = i+1;
        }
        else if (value3<value1 && value3 <value2){
            j[i + width+1] = -1;
            i = i + width+1;
        }
        else{
            j[i + 1] = -1;
            i = (i + 1);
        }
    }
}
/**
 This method is used first and when called it will take in the
 string value of the file name and then take the first line
 and make it the length && width.
 From there we look for the getline method where we take in
 our file and take every line and break it down and set each point into
 the array which will hold the values.
 */

void Drawer(std::string txt) {
    //read in data from a file
    std::fstream myfile;
    myfile.open(txt);
    string example;
    int location = -1;
    if(myfile.good()){
        getline(myfile, example);
        location = example.find_first_of(" ");
    }
    int size = 0;
    int current_number = 0;
    
    if(myfile.is_open()) {
        width = stoi(example.substr(0, location));
        length = stoi(example.substr(location+1));
        j = new int [width * length];
        //cout << "test" <<endl;
        int counter = 0;
        while(getline(myfile, example)){
            istringstream iss(example);
            while(iss>>current_number) {
                
                cout<<current_number<<endl;
                j[counter] = current_number;
                counter++;
                
            }
            
        }
        
        // This for loop is used to find the max_number in the array to help
        // with our scaling.
        
        int temp = j[0];
        for(int i = 0 ; i <counter; i++) {
            if(temp<j[i]){
                temp = j[i];
            }
            max_number = temp;
        }
        //For loop to find the min_number
        
        for(int i = 0 ; i <counter; i++) {
            if(temp>j[i]){
                temp = j[i];
            }
            min_number = temp;
        }
        // This helps to set the offset to
        //be able to move negative values into
        //positive values but taking the absolute
        //value of the min_number and seting it to >= 0
        if(min_number<0){
            min_number = abs(min_number);
            for(int i = 0 ; i <length*width; i++) {
                j[i] += min_number;
            }
            max_number += min_number;
            min_number = 0;
        }
        //after all is done, and we have the correct array
        //we use our scaling method to create the image.
        scaling(j, max_number, width, length);
        //then we input a number and that will move the
        //greedy algorithm accordingly to the right side of the
        //map as optimally as it can be.
        greedyAlgorithm(200); //hard-coded the value 200
        
        myfile.close(); //close file
        delete [] j; //deallocated array
    }
}

int main(int argc, char *argv[])
{
    
    Drawer("/Users/amandacheng/Downloads/colorado844x480.dat.txt");
    //calling Drawer method with string representation of file path
    
    Bridges *bridges = new Bridges(5, "acheng6", "1365455548776");
    //open bridges to be able to view the file
    ColorGrid *cg  = new ColorGrid(length, width, Color("white"));
    //creating the color grid with the length and width (global variable)
    
    /**
     Iterate through the grid and set the color accordingly to RGB values.
     If value = -1 then we use Red for the color to track the most optimal location.
     */
     cout << length << endl;
     cout << width << endl;
     cout << length*width << endl;
     int col =  0;
     int row  = 0;
     int temp = 0;

     for(int i = 0; i < length; i++){
         //row = length
         row = i;
         for(int m = 0; m < width; m++){
             col = m;
             temp = j[(i*width)+m];
             if(temp == -1) {
                 cg -> set(row, col, Color(255, 0 , 0 ));
             } else {
                 cg->set(row, col, Color(temp, temp, temp));
             }
         }
     }
     
    bridges->setDataStructure(cg);
    bridges->visualize();
    
}

