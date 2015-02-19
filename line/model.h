#define __MODEL_H__
#include <vector>

class Model {
  

public : 
    Model(const char *filename);
    std::vector<std::vector<int> > faces;
    std::vector<std::vector<float> > vertices; 
    std::vector<std::vector<float> > vecNormalObj ; 
    std::vector<std::vector<float> > normal ; 
    int nvertices() ;
    int nfaces() ;
    int nnorm() ; 
    std::vector<int> face(int idx) ;
    std::vector<float> vert(int i) ;
    std::vector<float> norm(int i) ;
    void produitVectoriel() ; 

};  
