
// EECE 478
// Written by Jackie Chen
// Student Number: 60006087

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define SHUTTLE 2
#define MODELFOLDER "/Models/"

#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "model.h"
#include "PPM.h"
#include "city.h"

using namespace std;

// Function to parse and load the city
void City::loadcity(string city) {
    
    this->ShuttlePoint = 0;
    this->cityname = city;

    // Get path of the .model
    string fpath = MODELFOLDER + this->cityname + ".city";
    this->filepath = &(getcwd(NULL,0) + fpath)[0];
    
    //  cout << "FILEPATH: " << filepath << endl;
    
    // Open the .model file and temporary output file
    this->in_file.open(this->filepath);    
    
    // If file opened successfully, load the model
    if (this->in_file.is_open()) {
        
        this->findNumBuildings();
        this->findBuildingParams();
        this->loadCity();
        
    } else {
        cout << "Error Opening File!" << endl;
        exit(1);
    }

}

// Function to find # of buildings for vector sizing
void City::findNumBuildings(void) {
    
    int count = 0;
    
    while (getline(this->in_file,this->line)) {
        if (count == 2) {
            this->num_buildings = atoi(this->line.c_str());
            break;
        }
        count++;
    }
}

// Function to find the parameters and store them for each building in their respective vectors
void City::findBuildingParams(void) {
    
    
    this->building.resize(this->num_buildings);
    this->scale_x.resize(this->num_buildings);
    this->scale_y.resize(this->num_buildings);
    this->scale_z.resize(this->num_buildings);
    this->rot_x.resize(this->num_buildings);
    this->rot_y.resize(this->num_buildings);
    this->rot_z.resize(this->num_buildings);
    this->o_x.resize(this->num_buildings);
    this->o_y.resize(this->num_buildings);
    this->o_z.resize(this->num_buildings);
    this->translate_x.resize(this->num_buildings);
    this->translate_y.resize(this->num_buildings);
    this->translate_z.resize(this->num_buildings);

    int count, buildingNum = -1, firstpos, numchars;
    string temp_string;
    float temp_float;
    bool found;
    
    while (getline(this->in_file,this->line)) {
        
        count = 0;
        temp_string = "";
        temp_float = 0.0f;
        found = false;
        
        for (int i = 0; i < this->line.length(); i++) {
            
            switch(count) {
                case 0: 
                    if (this->line.at(i) != ' ') {
                        temp_string += this->line.at(i);
                    } else {
                        this->building[buildingNum] = temp_string;
                        count++;
                    }
                    break;
                case 1: 
                    if (found == false && this->line.at(i) != ' ') {
                        found = true;
                        firstpos = i;
                    } else if (found == true && this->line.at(i) == ' '){
                        numchars = i - firstpos;
                        this->scale_x[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;
                    }
                    break;
                case 2: 
                    if (found == false && this->line.at(i) != ' ') {
                        found = true;
                        firstpos = i;
                    } else if (found == true && this->line.at(i) == ' '){
                        numchars = i - firstpos;
                        this->scale_y[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;
                    }
                    break;
                case 3: 
                    if (found == false && this->line.at(i) != ' ') {
                        found = true;
                        firstpos = i;
                    } else if (found == true && this->line.at(i) == ' '){
                        numchars = i - firstpos;
                        this->scale_z[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;
                    }
                    break;
                case 4: 
                    if (found == false && this->line.at(i) != ' ') {
                        found = true;
                        firstpos = i;
                    } else if (found == true && this->line.at(i) == ' '){
                        numchars = i - firstpos;
                        this->rot_x[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;
                    }
                    break;
                case 5: 
                    if (found == false && this->line.at(i) != ' ') {
                        found = true;
                        firstpos = i;
                    } else if (found == true && this->line.at(i) == ' '){
                        numchars = i - firstpos;
                        this->rot_y[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;
                    }
                    break;
                case 6: 
                    if (found == false && this->line.at(i) != ' ') {
                        found = true;
                        firstpos = i;
                    } else if (found == true && this->line.at(i) == ' '){
                        numchars = i - firstpos;
                        this->rot_z[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;
                    }
                    break;
                case 7: 
                    if (found == false && this->line.at(i) != ' ') {
                        found = true;
                        firstpos = i;
                    } else if (found == true && this->line.at(i) == ' '){
                        numchars = i - firstpos;
                        this->o_x[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;
                    }
                    break;
                case 8: 
                    if (found == false && this->line.at(i) != ' ') {
                        found = true;
                        firstpos = i;
                    } else if (found == true && this->line.at(i) == ' '){
                        numchars = i - firstpos;
                        this->o_y[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;
                    }
                    break;
                case 9: 
                    if (found == false && this->line.at(i) != ' ') {
                        found = true;
                        firstpos = i;
                    } else if (found == true && this->line.at(i) == ' '){
                        numchars = i - firstpos;
                        this->o_z[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;
                    }
                    if (found == true && i == (this->line.length() - 1)) {
                        numchars = i + 1 - firstpos;
                        this->translate_z[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;   
                    }
                    break;
                case 10: 
                    if (found == false && this->line.at(i) != ' ') {
                        found = true;
                        firstpos = i;
                    } else if (found == true && this->line.at(i) == ' '){
                        numchars = i - firstpos;
                        this->translate_x[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;
                    }
                    break;
                case 11: 
                    if (found == false && this->line.at(i) != ' ') {
                        found = true;
                        firstpos = i;
                    } else if (found == true && this->line.at(i) == ' '){
                        numchars = i - firstpos;
                        this->translate_y[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;
                    }
                    break;
                case 12: 
                    if (found == false && this->line.at(i) != ' ') {
                        found = true;
                        firstpos = i;
                    } else if (found == true && this->line.at(i) == ' '){
                        numchars = i - firstpos;
                        this->translate_z[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;
                    }
                    if (found == true && i == (this->line.length() - 1)) {
                        numchars = i + 1 - firstpos;
                        this->translate_z[buildingNum] = atof((this->line.substr(firstpos,numchars)).c_str());
                        found = false;
                        count++;   
                    }
                    break;
                
            }
        }
        buildingNum++;
    }
 
}

// Function to initialize and load all of the models in the city
void City::loadCity(void) {
    
    for (int i = 0; i < this->num_buildings; i++) {
        
        this->models.push_back(new Model());
        
        this->models[i]->load_model(building[i]);
        
        this->models[i]->setModelParams(this->scale_x[i],this->scale_y[i],this->scale_z[i],this->rot_x[i],this->rot_y[i],this->rot_z[i],this->o_x[i],this->o_y[i],this->o_z[i],this->translate_x[i],this->translate_y[i],this->translate_z[i]);
    }
}

// Function to render the city (each model of the city)
void City::renderCity(void) {
    
   
    for (int i = 0; i < this->num_buildings; i++) {
        
       if (i < 2) {
            //give illusion of infinite plane
            for (int j = -2; j <= 2; j++) {
                for (int k = -2; k <= 2; k++) {
                    this->models[i]->setModelParams(this->scale_x[i],this->scale_y[i],this->scale_z[i],this->rot_x[i],this->rot_y[i],this->rot_z[i],this->o_x[i],this->o_y[i],this->o_z[i],this->translate_x[i]+((int)((this->xPos+(1440.009*j))/1440.009)*1440.009),this->translate_y[i],this->translate_z[i]+((int)((this->zPos+(1698.769*k))/1698.769)*1698.769));   
                    this->models[i]->render_model();
                }
            }
        }
        else 
            this->models[i]->render_model();

    }
}
                         

string City::getCityName(void) {
    return this->cityname;
}

void City::animateShuttle(void) {
    
    /*
     Shuttle:
        Facing directions (from initial point of reference): 
        0, -90, -90 (forward)
        0, 90, 90   (backward)
        -90, 0 0    (left) 
        -90, 180, 0 (right)
     */
    
    switch (this->ShuttlePoint) {
        case 0: // RISE UP (FORWARD)
            this->models[SHUTTLE]->translate_z -= this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_z <= 600) {
                this->models[SHUTTLE]->translate_z += 25;
                this->ShuttlePoint = 1;
            }
            break;
        case 1: // GO STRAIGHT (LEFT)
            this->models[SHUTTLE]->translate_y += this->ShuttleSpeed/2.0f;
            if (this->models[SHUTTLE]->translate_y >= 250) {
                this->ShuttlePoint = 2;
            }
            break;
        case 2:
            this->models[SHUTTLE]->translate_z -= this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_z <= 330) {
                this->models[SHUTTLE]->rot_x = -90;
                this->models[SHUTTLE]->rot_y = 0;
                this->models[SHUTTLE]->rot_z = 0;
                this->models[SHUTTLE]->translate_z += 25;
                this->ShuttlePoint = 3;
            }
            break;
        case 3: // TURN RIGHT (FORWARD)
            this->models[SHUTTLE]->translate_x -= this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_x <= 13) {
                this->models[SHUTTLE]->rot_x = 0;
                this->models[SHUTTLE]->rot_y = -90;
                this->models[SHUTTLE]->rot_z = -90;
                this->models[SHUTTLE]->translate_x -= 25;
                this->models[SHUTTLE]->translate_z -= 35;
                this->ShuttlePoint = 4;
            }
            break;
        case 4: // TURN RIGHT (RIGHT)
            this->models[SHUTTLE]->translate_z -= this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_z <= 0) {
                this->models[SHUTTLE]->rot_x = -90;
                this->models[SHUTTLE]->rot_y = 180;
                this->models[SHUTTLE]->rot_z = 0;
                this->models[SHUTTLE]->translate_x += 25;
                this->models[SHUTTLE]->translate_z -= 10;
                this->ShuttlePoint = 5;
            }
            break;
            
        case 5: // TURN RIGHT (BACKWARD)
            this->models[SHUTTLE]->translate_x += this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_x >= 1175) {
                this->models[SHUTTLE]->rot_x = 0;
                this->models[SHUTTLE]->rot_y = 90;
                this->models[SHUTTLE]->rot_z = 90;
                this->models[SHUTTLE]->translate_x += 30;
                this->models[SHUTTLE]->translate_z -= 10;
                this->ShuttlePoint = 6;
            }
            break;
        case 6: // TURN LEFT (RIGHT)
            this->models[SHUTTLE]->translate_z += this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_z >= 300) {
                this->models[SHUTTLE]->rot_x = -90;
                this->models[SHUTTLE]->rot_y = 180;
                this->models[SHUTTLE]->rot_z = 0;
                this->models[SHUTTLE]->translate_z += 0;
                this->ShuttlePoint = 7;
            }
            break;
        case 7: // TURN RIGHT (BACKWARD)
            this->models[SHUTTLE]->translate_x += this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_x >= 1440) {
                this->models[SHUTTLE]->rot_x = 0;
                this->models[SHUTTLE]->rot_y = 90;
                this->models[SHUTTLE]->rot_z = 90;
                this->models[SHUTTLE]->translate_x += 25;
                this->models[SHUTTLE]->translate_z -= 10;
                this->ShuttlePoint = 8;
            }
            break;
        case 8: // TURN RIGHT (LEFT)
            this->models[SHUTTLE]->translate_z += this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_z >= 800) {
                this->models[SHUTTLE]->rot_x = -90;
                this->models[SHUTTLE]->rot_y = 0;
                this->models[SHUTTLE]->rot_z = 0;
                this->models[SHUTTLE]->translate_x -= 20;
                this->models[SHUTTLE]->translate_z += 10;
                this->ShuttlePoint = 9;
            }
            break;
        case 9: // TURN LEFT (BACKWARD) AND RISE UP
            this->models[SHUTTLE]->translate_x -= this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_x <= 685) {
                this->models[SHUTTLE]->rot_x = 0;
                this->models[SHUTTLE]->rot_y = 90;
                this->models[SHUTTLE]->rot_z = 90;
                this->models[SHUTTLE]->translate_x += 45;
                this->models[SHUTTLE]->translate_z -= 0;
                this->ShuttlePoint = 10;
            }
            break;
            
        case 10: // GO STRAIGHT (BACKWARD)
            this->models[SHUTTLE]->translate_y += this->ShuttleSpeed/2.0f;
            if (this->models[SHUTTLE]->translate_y >= 400) {
                this->ShuttlePoint = 11;
            }
            break;
            
        case 11: // LOWER DOWN (BACKWARD)
            this->models[SHUTTLE]->translate_z += this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_z >= 1000) {
                this->ShuttlePoint = 12;
            }
            break;
            
        case 12: // GO STRAIGHT (BACKWARD)
            this->models[SHUTTLE]->translate_y -= this->ShuttleSpeed/2.0f;
            if (this->models[SHUTTLE]->translate_y <= 250.0f) {
                this->ShuttlePoint = 13;
            }
            break;

            
        case 13: // TURN LEFT (RIGHT)
            this->models[SHUTTLE]->translate_z += this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_z >= 1170) {
                this->models[SHUTTLE]->rot_x = -90;
                this->models[SHUTTLE]->rot_y = 180;
                this->models[SHUTTLE]->rot_z = 0;
                this->models[SHUTTLE]->translate_x += 0;
                this->models[SHUTTLE]->translate_z -= 20;
                this->ShuttlePoint = 15;
            }
            break;
            
            
        case 15: // TURN RIGHT (BACKWARD)
            this->models[SHUTTLE]->translate_x += this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_x >= 1430) {
                this->models[SHUTTLE]->rot_x = 0;
                this->models[SHUTTLE]->rot_y = 90;
                this->models[SHUTTLE]->rot_z = 90;
                this->models[SHUTTLE]->translate_x += 25;
                this->models[SHUTTLE]->translate_z -= 0;
                this->ShuttlePoint = 16;
            }
            break;
        case 16: // TURN RIGHT (LEFT)
            this->models[SHUTTLE]->translate_z += this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_z >= 1715) {
                this->models[SHUTTLE]->rot_x = -90;
                this->models[SHUTTLE]->rot_y = 0;
                this->models[SHUTTLE]->rot_z = 0;
                this->models[SHUTTLE]->translate_x += 0;
                this->models[SHUTTLE]->translate_z -= 0;
                this->ShuttlePoint = 17;
            }
            break;
        case 17: // TURN RIGHT (FORWARD)
            this->models[SHUTTLE]->translate_x -= this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_x <= -2) {
                this->models[SHUTTLE]->rot_x = 0;
                this->models[SHUTTLE]->rot_y = -90;
                this->models[SHUTTLE]->rot_z = -90;
                this->models[SHUTTLE]->translate_x -= 20;
                this->models[SHUTTLE]->translate_z -= 0;
                this->ShuttlePoint = 18;
            }
            break;
            break;
        case 18: // TURN RIGHT (RIGHT)
            this->models[SHUTTLE]->translate_z -= this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_z <= 1180) {
                this->models[SHUTTLE]->rot_x = -90;
                this->models[SHUTTLE]->rot_y = 180;
                this->models[SHUTTLE]->rot_z = 0;
                this->models[SHUTTLE]->translate_x += 20;
                this->models[SHUTTLE]->translate_z -= 20;
                this->ShuttlePoint = 19;
            }
            break;
        case 19: // TURN LEFT (FORWARD) and LOWER DOWN
            this->models[SHUTTLE]->translate_x += this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_x >= 700) {
                this->models[SHUTTLE]->rot_x = 0;
                this->models[SHUTTLE]->rot_y = -90;
                this->models[SHUTTLE]->rot_z = -90;
                this->models[SHUTTLE]->translate_x -= 35;
                this->models[SHUTTLE]->translate_z -= 0;
                this->ShuttlePoint = 20;
            }
            break;
        case 20: // GO STRAIGHT
            this->models[SHUTTLE]->translate_y -= this->ShuttleSpeed/2.0f;
            if (this->models[SHUTTLE]->translate_y <= 20) {
                this->ShuttlePoint = 21;
            }
            break;
            
        case 21: // RESET
            this->models[SHUTTLE]->translate_z -= this->ShuttleSpeed;
            if (this->models[SHUTTLE]->translate_z <= 865) {
                this->models[SHUTTLE]->rot_x = 0;
                this->models[SHUTTLE]->rot_y = -90;
                this->models[SHUTTLE]->rot_z = -90;
                this->models[SHUTTLE]->translate_x += 0;
                this->models[SHUTTLE]->translate_z -= 0;
                this->ShuttlePoint = 22;
            }
            break;
        default:
            this->models[SHUTTLE]->translate_x = 700;
            this->models[SHUTTLE]->translate_y = 20;
            this->models[SHUTTLE]->translate_z = 865;
            this->models[SHUTTLE]->rot_x = 0;
            this->models[SHUTTLE]->rot_y = -90;
            this->models[SHUTTLE]->rot_z = -90;
            this->ShuttlePoint = 0;
            break;
    }
}

void City::introductoryShuttle (float speed) {
    this->models[SHUTTLE]->translate_z -= speed;
}

void City::setShuttleSpeed (float speed) {
    this->ShuttleSpeed = speed;
}

