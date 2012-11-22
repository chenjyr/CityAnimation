
// EECE 478
// Written by Jackie Chen
// Student Number: 60006087

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "model.h"
#include "PPM.h"

#define MODELFOLDER "/Models/"

using namespace std;

Model::Model (void){
}

// Function which loads the file input and stores the vertices, normals, and triangles in vectors
void Model::load_model(string model) {

    this->modelname = model;
    
    // Get path of the .model
    string fpath = "/Models/" + this->modelname + "/" + this->modelname + ".model";
    this->filepath = &(getcwd(NULL,0) + fpath)[0];
    
  //  cout << "FILEPATH: " << filepath << endl;
    
    // Open the .model file and temporary output file
    this->in_file.open(this->filepath);    

    // If file opened successfully, load the model
    if (this->in_file.is_open()) {
        this->find_num();
        this->store_values();
    } else {
        cout << "Error Opening File!" << endl;
        exit(1);
    }
    
    // Close file after finished storing parameters
    in_file.close();
    
    this->load_textures();
    this->sort_by_texture_index();
}

void Model::sort_by_texture_index(void) {
    
    vector< vector<float> > temp_triangles;
    
    for (int j = 0; j < this->textures_gl.size(); j++ ) {
        for (int i = 0; i < this->triangles.size(); i++) {
            if (this->triangles[i][4] == j) {
                temp_triangles.push_back(this->triangles[i]);
            }
        }
    }
    
    if (temp_triangles.size() != this->triangles.size()) {
        cout << "ERROR!!!!!!!!!!!" << endl;
    }
    
    this->triangles = temp_triangles;
   
}

// Function to store the vertices and normals in a 2D array
void Model::store_values(void) {
    
    bool tex = false, v = false, n = false, t = false, st_only = true, num_found = false;
    int firstpos = -1, numchars = -1, linenum = 0, bnum;
    float tempn;
    string temptex = "";

    // Resize the vectors based on the number of vertices,normals,triangles,textures
    this->vertices.resize(this->num_vertices);
    for (int i = 0; i < this->num_vertices; ++i)
        this->vertices[i].resize(3);
    this->normals.resize(this->num_normals);
    for (int i = 0; i < this->num_normals; ++i)
        this->normals[i].resize(3);
    this->triangles.resize(this->num_triangles);
    for (int i = 0; i < this->num_triangles; ++i)
        this->triangles[i].resize(11);
    this->textures.resize(this->num_textures);
    
    // Iterate through the file
    while (getline(this->in_file,this->line)) {
        
        st_only = true; // To keep track of if line has any parameters
        num_found = false; // To keep track of if a number has been found
        
        // If end of section found
        if (this->line.find("</") != string::npos) {
            tex = false;
            v = false;
            n = false;
            t = false;
            num_found = false;
        }
        
        // If currently in vertices section
        else if (v == true && this->line.length() > 0) {
            bnum = 0;
            
            // Iterate through line
            for (int i = 0; i < this->line.length(); i++) {

                // If number has not been found and character is not a space or tab, then number found
                if (num_found == false && (this->line.at(i) != ' ' && this->line.at(i) != '\t')) {
                    st_only = false;
                    num_found = true;
                    firstpos = i;
                }
                // If number has been found and character is a space or tab, then store number
                else if (num_found == true && (this->line.at(i) == ' ' || this->line.at(i) == '\t')) {
                    numchars = i-firstpos;
                    tempn = atof((this->line.substr(firstpos,numchars)).c_str());
                    this->vertices[linenum][bnum++] = tempn;
                    num_found = false;
                }
                
                // If a number has been found and end of line found, store number
                if (num_found == true && i == (this->line.length() - 1)) {
                    numchars = i + 1 -firstpos;
                    tempn = atof((this->line.substr(firstpos,numchars)).c_str());
                    this->vertices[linenum][bnum++] = tempn;
                    num_found = false;
                }
            }
            // If line has paremeters, increment first dimension of vector
            if (st_only == false) 
                linenum++;
        }
        
        // If currently in the normals section
        else if (n == true && this->line.length() > 0) {
            bnum = 0;
            
            // Iterate through line
            for (int i = 0; i < this->line.length(); i++) {
                
                // If number has not been found and character is not a space or tab, then number found
                if (num_found == false && (this->line.at(i) != ' ' && this->line.at(i) != '\t')) {
                    st_only = false;
                    num_found = true;
                    firstpos = i;
                }
                // If number has been found and character is a space or tab, then store number
                else if (num_found == true && (this->line.at(i) == ' ' || this->line.at(i) == '\t')) {
                    numchars = i-firstpos;
                    tempn = atof((this->line.substr(firstpos,numchars)).c_str());
                    this->normals[linenum][bnum++] = tempn;
                    num_found = false;
                }
                
                // If a number has been found and end of line found, store number
                if (num_found == true && i == (this->line.length() - 1)) {
                    numchars = i+ 1 -firstpos;
                    tempn = atof((this->line.substr(firstpos,numchars)).c_str());
                    this->normals[linenum][bnum++] = tempn;
                    num_found = false;
                }
            }
            // If line has paremeters, increment first dimension of vector
            if (st_only == false) 
                linenum++;
        }
        
        // If currently in the triangles section
        else if (t == true && this->line.length() > 0) {
            bnum = 0;
            
            // Iterate through line
            for (int i = 0; i < this->line.length(); i++) {
                

                // If number has not been found and character is not a space or tab, then number found
                if (num_found == false && (this->line.at(i) != ' ' && this->line.at(i) != '\t')) {
                    st_only = false;
                    num_found = true;
                    firstpos = i;
                }
                // If number has been found and character is a space or tab, then store number
                else if (num_found == true && (this->line.at(i) == ' ' || this->line.at(i) == '\t')) {
                    numchars = i-firstpos;
                    tempn = atof((this->line.substr(firstpos,numchars)).c_str());
                    this->triangles[linenum][bnum++] = tempn;
                    num_found = false;
                }
                
                // If a number has been found and end of line found, store number
                if (num_found == true && i == (this->line.length() - 1)) {
                    numchars = i + 1 - firstpos;
                    tempn = atof((this->line.substr(firstpos,numchars)).c_str());
                    this->triangles[linenum][bnum++] = tempn;
                    num_found = false;
                }
            }
            // If line has paremeters, increment first dimension of vector
            if (st_only == false) 
                linenum++;
        }
        
        // If currently in the textures section
        else if (tex == true && this->line.length() > 0) {
            // Iterate through line
            for (int i = 0; i < this->line.length(); i++) {

                // If character has been found, store characters in a temporary string
                if (this->line.at(i) != ' ' && this->line.at(i) != '\t') {
                    temptex += this->line[i];
                    num_found = true;
                }
                
                // If space found after characters, store temporary string into texture array
                else if (num_found == true && (this->line.at(i) == ' ' || this->line.at(i) == '\t')) {
                    this->textures[linenum] = temptex;

                }
                
                if (i == this->line.length() - 1) {
                    this->textures[linenum] = temptex;
                }
            }
            // If line has paremeters, increment vector position
            if (num_found == true) {
                linenum++;
                temptex = "";
            }
        }

        // If line has "triangles", triangles section found (if no '/')
        else if (this->line.find("triangles") != string::npos) {
            t = true;
            linenum = 0;
        }
        // If line has "normals", normals section found (if no'/')
        else if (this->line.find("normals") != string::npos) {
            n = true;
            linenum = 0;
        }
        // if line has "vertices", vertices section found (if no '/')
        else if (this->line.find("vertices") != string::npos) {
            v = true;
            linenum = 0;
        }
        // If line has "textures", textures section found (if no '/')
        else if (this->line.find("textures") != string::npos) {
            tex = true;
            linenum = 0;
        }
    }
  /*  for (int i = 0; i < vertices.size(); i++) {
        cout << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << endl;
    }
    */
   /* for (int i = 0; i < textures.size(); i++) {
        cout << textures[i] << endl;
    }*/
    
  /*  for (int i = 0 ; i < normals.size(); i++) {
        for (int j = 0; j < normals[i].size(); j++) {
            cout << normals[i][j] << " ";
        }
        cout << endl;
    }
   */
 /*   float xx = 0, zz = 0;
    float x2 = 1000, z2 = 1000;
    for (int i = 0; i < this->vertices.size(); i++) {
        if (vertices[i][1] > xx) xx = vertices[i][1];
        if (vertices[i][0] > zz) zz = vertices[i][0];
        if (vertices[i][1] < x2) x2 = vertices[i][1];
        if (vertices[i][0] < z2) z2 = vertices[i][0];


    }
    
    cout << "X:" << xx*0.05 << " Z:" << zz*0.05 << endl;
    cout << "X2:" << x2*0.05 << " Z2:" << z2*0.05 << endl;
   */
    
    cout << this->modelname << " loaded!" << endl;
}


// Function to find the number of vertices, normals, and triangles in the model
void Model::find_num(void) {
    
    size_t found_textures, found_v, found_n, found_t;
    int firstpos = -1, numchar = -1;
    
    // Iterate through file
    while (getline(in_file,line)) {
        
        // Find line which contains the # of vertices, normals, and triangles
        if (line.find("/") == string::npos) {
            found_textures = line.find("<textures");
            found_v = this->line.find("<vertices");
            found_n = this->line.find("<normals");
            found_t = this->line.find("<triangles");
        }
    
        // If textures line found, determine # of textures
        if (found_textures != string::npos) {
            for (int i = (int)found_textures; i < this->line.length(); i++) {
                if (firstpos == -1) {
                    if (this->line.at(i) >= 48 && this->line.at(i) <= 57) 
                        firstpos = i;
                } else if (firstpos >= 0 && (this->line.at(i) < 48 || this->line.at(i) > 57)) 
                    numchar = i-firstpos;
            }
            
            this->num_textures = atoi(this->line.substr(firstpos,numchar).c_str());
            found_textures = -1;
            firstpos = -1;
            numchar = -1;
            
        }
        
        // If vertices line found, determine # of vertices
        else if ((found_v != string::npos)) {
            for (int i = (int)found_v; i < this->line.length(); i++) {
                if (firstpos == -1) {
                    if (this->line.at(i) >= 48 && this->line.at(i) <= 57) 
                        firstpos = i;
                } else if (firstpos >= 0 && (this->line.at(i) < 48 || this->line.at(i) > 57)) 
                    numchar = i-firstpos;
            }
            
            this->num_vertices = atoi(this->line.substr(firstpos,numchar).c_str());
            found_v = -1;
            firstpos = -1;
            numchar = -1;
        }
        
        // If normals line found, determine # of normals
        else if (found_n != string::npos) {
            for (int i = (int)found_n; i < this->line.length(); i++) {
                if (firstpos == -1) {
                    if (this->line.at(i) >= 48 && this->line.at(i) <= 57) 
                        firstpos = i;
                } else if (firstpos >= 0 && (this->line.at(i) < 48 || this->line.at(i) > 57)) 
                    numchar = i-firstpos;
            }
                        
            this->num_normals = atoi(this->line.substr(firstpos,numchar).c_str());
            found_n = -1;
            firstpos = -1;
            numchar = -1;
        }
        
        // If triangles line found, determine # of triangles
        else if (found_t != string::npos) {
            for (int i = (int)found_t; i < this->line.length(); i++) {
                if (firstpos == -1) {
                    if (this->line.at(i) >= 48 && this->line.at(i) <= 57) 
                        firstpos = i;
                } else if (firstpos >= 0 && (this->line.at(i) < 48 || this->line.at(i) > 57)) 
                    numchar = i-firstpos;
            }
            
            
            this->num_triangles = atoi(this->line.substr(firstpos,numchar).c_str());
            found_t = -1;
            firstpos = -1;
            numchar = -1;
        }
    }
    // Reset file pointer to prepare to loop through file again to store parameters
    this->reset_file_pointer();
    

}

// Function to reset the file pointer for reading
void Model::reset_file_pointer(void) {
    this->in_file.clear();
    this->in_file.seekg(0,ios_base::beg);
}

// Function to render the model after loading from the .model file
void Model::render_model(void) {
    
    glPushMatrix();
    glTranslatef(this->translate_x,this->translate_y,this->translate_z);
    glScalef(this->scale_x,this->scale_y,this->scale_z);
    glRotatef(this->rot_z,0,0,1);
    glRotatef(this->rot_y,0,1,0);
    glRotatef(this->rot_x,1,0,0);
    glTranslatef(this->o_x,this->o_y,this->o_z);

    glColor3f(1.0f,1.0f,1.0f);

    int tex_index = 0;
    int start = 1;
    
    if (this->modelname == "Shuttle") 
        glColor3f(1.0f,0.8f,0.0f);
    
    // Loop through number of triangles
    for (int i = 0; i < this->num_triangles; i++) {
            
        // Bind texture and glBegin only if texture has changed
        if (((int)this->triangles[i][4] != tex_index) || start++ == 1) {
            glBindTexture(GL_TEXTURE_2D, this->textures_gl[this->triangles[i][4]]);
            glBegin(GL_TRIANGLES);
            tex_index = this->triangles[i][4];
        }
        
        glNormal3f(this->normals[this->triangles[i][3]][0],this->normals[this->triangles[i][3]][1],this->normals[this->triangles[i][3]][2]);

        // Draw each and every triangle as specified        
        glTexCoord2f(this->triangles[i][5],this->triangles[i][6]);
        glVertex3f(this->vertices[this->triangles[i][0]][0],this->vertices[this->triangles[i][0]][1],this->vertices[this->triangles[i][0]][2]);

        glTexCoord2f(this->triangles[i][7],this->triangles[i][8]);
        glVertex3f(this->vertices[this->triangles[i][1]][0],this->vertices[this->triangles[i][1]][1],this->vertices[this->triangles[i][1]][2]);

        glTexCoord2f(this->triangles[i][9],this->triangles[i][10]);
        glVertex3f(this->vertices[this->triangles[i][2]][0],this->vertices[this->triangles[i][2]][1],this->vertices[this->triangles[i][2]][2]); 
        
        // glEnd if next iteration is a new texture, or last triangle
        if ((i == this->triangles.size() - 1) || ((int)this->triangles[i+1][4] != tex_index)) {
            glEnd();
        }
    }
    
    glPopMatrix();
}

// Get a specified building name
string Model::get_building_name(void) {
    return this->modelname;
}


void Model::load_textures(void)
{
    int width, height;
    unsigned char *ppm_texture;
    bool tex_found = false;
    int tex_index = 0;
    string ppm_path;
    
    this->textures_gl.resize(this->textures.size());
    this->ppm_paths.resize(this->textures.size());
    
    for (int i = 0; i < this->textures.size() ; i++) {
        
        tex_found = false;
        
        string temppath = MODELFOLDER + this->modelname + "/" + this->textures[i];
        ppm_path = getcwd(NULL,0) + temppath;

        for (int j = 0; j < textures.size(); j++) {
            if (ppm_path == this->ppm_paths[j]) {
                tex_found = true;
                tex_index = j;
            }
            else if (j == i) {
                this->ppm_paths[j] = ppm_path;
            }
        }
        
        if (tex_found == false) {
            
            ppm_texture = PPM::Read(ppm_path, width, height);
            
            glGenTextures(1,&this->textures_gl[i]);
            
            glBindTexture(GL_TEXTURE_2D,this->textures_gl[i]);
            
            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
            
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR );

            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, ppm_texture );

            free(ppm_texture);
            
        } else if (tex_found == true) {
            this->textures_gl[i] = this->textures_gl[tex_index];
        }
        
        
        
    }
    return;
}

void Model::setModelParams(float sx, float sy, float sz, float rx, float ry , float rz, float ox, float oy, float oz, float tx, float ty, float tz) {
              
    this->scale_x = sx;
    this->scale_y = sy;
    this->scale_z = sz;
    this->rot_x = rx;
    this->rot_y = ry;
    this->rot_z = rz;
    this->o_x = ox;
    this->o_y = oy;
    this->o_z = oz;
    this->translate_x = tx;
    this->translate_y = ty;
    this->translate_z = tz;
}


