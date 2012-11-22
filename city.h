
// EECE 478
// Written by Jackie Chen
// Student Number: 60006087


#ifndef GLUT_city_h
#define GLUT_city_h

class City {
public:
    void loadcity(string);
    string getCityName(void);
    void findNumBuildings(void);
    void findBuildingParams(void);
    void loadCity(void);
    void renderCity(void);
    void animateShuttle(void);
    void introductoryShuttle(float);
    void setShuttleSpeed(float);
    
    string cityname;
    string line;
    char* filepath;
    ifstream in_file;
    int num_buildings;
    vector<string> building;
    vector<float> scale_x;
    vector<float> scale_y;
    vector<float> scale_z;
    vector<float> rot_x;
    vector<float> rot_y;
    vector<float> rot_z;
    vector<float> o_x;
    vector<float> o_y;
    vector<float> o_z;
    vector<float> translate_x;
    vector<float> translate_y;
    vector<float> translate_z;
    vector<Model*> models;
    int xPos, yPos, zPos;
    int ShuttlePoint;
    float ShuttleSpeed;
};

#endif
