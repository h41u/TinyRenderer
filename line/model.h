#define __MODEL_H__
#include <vector>
#include "geometry.h" 
#include "tgaimage.h"
class Model {
  

public : 
    Model(const char *filename);
    std::vector<std::vector<int> > faces;
    std::vector<std::vector<int> > vertex;
    std::vector<std::vector<int> > uv;
    std::vector<Vec3f> vertices; 
    std::vector<std::vector<float> > vecNormalObj ; 
    std::vector<Vec3f> lineUv ;  
    std::vector<std::vector<float> > normal ; 
    int nvertices() ;
    int nfaces() ;
    int nnorm() ; 
    std::vector<int> face(int idx) ;
    std::vector<int> vertexIdx(int idx) ;
    std::vector<int> getLineUv(int idx) ;
    Vec3f vert(int i) ;
    std::vector<float> norm(int i) ;
    std::vector<float> vecNorm(int i) ;
    Vec3f getUv(int i) ;
    void produitVectoriel() ; 
    void load_texture(std::string filename, const char *suffix, TGAImage &img);
    TGAImage diffusemap;
    TGAColor getTextures(int x, int y);
};  
