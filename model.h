
// EECE 478
// Written by Jackie Chen
// Student Number: 60006087

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Model {
    
public:
    
    Model(void);
    void load_model(string);
    void reset_file_pointer(void);
    void find_num(void);
    void store_values(void);
    void render_model(void);
    void draw_triangle(float,float,float,float,float,float,float,float,float);
    void set_building(int);
    void set_building_name(string);
    void name_buildings(void);
    string get_building_name(void);
    int get_build_num(void);
    void set_build_num(int);
    void reset_model(void);
    void load_textures(void);
    void setModelParams(float,float,float,float,float,float,float,float,float,float,float,float);
    void sort_by_texture_index(void);

    // Initialize vectors to store the model parameters
    vector< vector<float> > vertices;
    vector< vector<float> > normals;
    vector< vector<float> > triangles;
    vector<string> textures;
    vector<string> buildings;
    vector<GLuint> textures_gl;
    vector<string> ppm_paths;
    
    // Set path of the models folder on your system
    string mainpath;
    
    ifstream in_file;
    string line;
    char* filepath;
    int num_textures;
    int num_vertices;
    int num_normals;
    int num_triangles;
    
    // Initialize modelname string to the first model in the folder
    string modelname;
    
    int build_num;
    
    float scale_x, scale_y, scale_z, rot_x, rot_y, rot_z, o_x, o_y, o_z, translate_x, translate_y, translate_z;
    float amplitude;
};
