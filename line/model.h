#define __MODEL_H__
#include <vector>
#include "geometry.h" 
class Model {
  

public : 
    Model(const char *filename);
    std::vector<std::vector<int> > faces;
    std::vector<std::vector<int> > vertex;
    std::vector<Vec3f> vertices; 
    std::vector<std::vector<float> > vecNormalObj ; 
    std::vector<std::vector<float> > normal ; 
    int nvertices() ;
    int nfaces() ;
    int nnorm() ; 
    std::vector<int> face(int idx) ;
    std::vector<int> vertexIdx(int idx) ;
    Vec3f vert(int i) ;
    std::vector<float> norm(int i) ;
    std::vector<float> vecNorm(int i) ;
    void produitVectoriel() ; 

};  
