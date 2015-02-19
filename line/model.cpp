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
	vector<float> vec ; 
	iss >> trash >> x >> y >> z ; 
	vec.push_back(x) ; 
	vec.push_back(y) ; 
	vec.push_back(z) ; 
	vertices.push_back(vec) ; 
      }
      else if (!line.compare(0, 2, "f ")) {
	vector<int> vec ; 
	int numberUseless ,number ; 
	iss >> trash ; 
	// recherche de pattern nombre/nombre/nombre et récupération du premier
	while(iss >> number >> trash >> numberUseless >> trash >> numberUseless) {
	  number -- ; 
	  vec.push_back(number) ; 
	}
	faces.push_back(vec) ; 
      }
     
     else if (!line.compare(0, 2, "vn ")) {
	float x,y,z ; 
	vector<float> vec ; 
	iss >> trash >> x >> y >> z ; 
	vec.push_back(x) ; 
	vec.push_back(y) ; 
	vec.push_back(z) ; 
	vecNormalObj.push_back(vec) ;
     }
   }
   in_stream.close();
 }

void Model::produitVectoriel() { 
	for (int i=0; i<nfaces(); i++) {
	  vector<int> fa = face(i) ;
	  vector<float> stock ; 
	  vector<float> avant ; 
	  for ( int j=0 ; j<3 ; j++ ) {
		vector<float> vec = vert(fa[j]) ;  
		float a = (vec[0]+1)*800 ;
		a=a/2 ;  
		float b = (vec[1]+1)*800 ;
		b=b/2 ;  
		float c = (vec[2]+1)*800 ;
		c=c/2 ;  
		stock.push_back(a) ; 
		stock.push_back(b) ;
		stock.push_back(c) ;  
	  }
	avant.push_back(stock[3]-stock[0]) ; 
	avant.push_back(stock[4]-stock[1]) ;
	avant.push_back(stock[5]-stock[2]) ;  
	avant.push_back(stock[6]-stock[0]) ; 
	avant.push_back(stock[7]-stock[1]) ;
	avant.push_back(stock[8]-stock[2]) ; 
	vector<float> vec2 ; 
	vec2.push_back((avant[1]*avant[5])-(avant[2]*avant[4])) ; 
	vec2.push_back((avant[2]*avant[3])-(avant[0]*avant[5])) ;
	vec2.push_back((avant[0]*avant[4])-(avant[1]*avant[3])) ;
	float x = vec2[0] ; 
	float y = vec2[1] ; 
	float z = vec2[2] ; 
	float taille = sqrt( (x*x)+(y*y)+(z*z) ) ; 
	vector<float> vec3 ; 
	vec3.push_back(x/taille) ; 
	vec3.push_back(y/taille) ; 
	vec3.push_back(z/taille) ; 
	normal.push_back(vec3) ;
	}
	
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
std::vector<float> Model::vert(int i) {
return vertices[i];
}
std::vector<float> Model::norm(int i) {
return normal[i];
}
