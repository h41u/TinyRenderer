#include <iostream>
#include <fstream>
#include "model.h"
#include <string>
#include <vector>
#include "geometry.h"
#include <sstream>
#include <math.h>

using namespace std;

 Model::Model(const char *filename)   {
    ifstream in_stream;
    string line;
    in_stream.open(filename, ifstream::in);
    if (in_stream.fail()) return  ; // TODO mettre des insultes
    while(!in_stream.eof()) {
      getline(in_stream,line) ; 
      istringstream iss(line.c_str());
      char trash ;
      if (!line.compare(0, 2, "v ")) {
	float x,y,z ; 
	Vec3f vec ; 
	iss >> trash >> x >> y >> z ; 
	//cout << "v\n" ; cout << x << "\n" ; cout << y << "\n" ; cout << z << "\n" ; 
	vec[0] = x ; 
	vec[1] = y ; 
	vec[2] = z ;
	vertices.push_back(vec) ; 
	
      }
else if (!line.compare(0, 3, "vn ")) {
	float x,y,z ; 
	vector<float> test ; 
	iss >> trash >> trash >> x >> y >> z ; 
	//cout << "vn\n" ; cout << x << "\n" ; cout << y << "\n" ; cout << z << "\n" ; 
	test.push_back(x) ; 
	test.push_back(y) ; 
	test.push_back(z) ; 
	vecNormalObj.push_back(test) ;
	//cout << vecNormalObj.size() << "\n" ; 
	//cout << vertices.size() << "\n" ; 
     }
      else if (!line.compare(0, 2, "f ")) {
	//cout << "bonjour\n" ; 
	vector<int> vec, vec2, vec3 ; 
	int numberUseless ,number, numberVt ; 
	iss >> trash ; 
	// recherche de pattern nombre/nombre/nombre et récupération du premier
	while(iss >> number >> trash >> numberVt >> trash >> numberUseless) {
	  number -- ; 
	  numberUseless -- ;
	  //cout << numberVt << "\n" ;  
	  numberVt -- ; 
	 // cout << numberVt << "\n" ;
	  vec3.push_back(numberVt) ; 
	  //cout<< "vec3 " << vec3[2] << "\n" ; 
	  vec2.push_back(numberUseless) ; 
	  vec.push_back(number) ; 

	}
	vertex.push_back(vec2) ; 
	faces.push_back(vec) ; 
	uv.push_back(vec3) ; 
      }
	else if (!line.compare(0,3,"vt ")) {
	float x,y,z ; 
	Vec3f test ; 
	iss >> trash >> trash >> x >> y >> z ; 
	test[0]=x ; 
	test[1]=y ; 
	test[2]=z ;  
	lineUv.push_back(test) ;
	/*vector<float> b = getUv(0) ; 	
	cout << b[0] << "\n" ; */	
	}
     
   }
   load_texture(filename, "_diffuse.tga", diffusemap);
   in_stream.close();
 }


 
int Model::nvertices() {
  return (int)vertices.size();
}
int Model::nfaces() {
  return (int)faces.size();
}
int Model::nnorm() {
  return (int)normal.size();
}
std::vector<int> Model::face(int idx) {
return faces[idx];
}
std::vector<int> Model::vertexIdx(int idx) {
return vertex[idx];
}
std::vector<int> Model::getLineUv(int idx) {
return uv[idx];
}
Vec3f Model::vert(int i) {
return vertices[i];
}
std::vector<float> Model::norm(int i) {
return normal[i];
}
std::vector<float> Model::vecNorm(int i) {
return vecNormalObj[i];
}
Vec3f Model::getUv(int i) {
return lineUv[i];
}
TGAColor Model::getTextures(int x, int y) {
return diffusemap.get(x,y);
}

void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) {
	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");
	if (dot!=std::string::npos) {
	texfile = texfile.substr(0,dot) + std::string(suffix);
	std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
	img.flip_vertically();
}
}
