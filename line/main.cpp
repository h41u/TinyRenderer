#include "tgaimage.h"
#include <cmath>
#include "model.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>
#include <limits>
#include "geometry.h" 
using namespace std ; 


const int width = 800 ; 
const int height = 800 ; 
const int depth = 800 ; 
int tab[width][height] ;  
Vec3f light = Vec3f(0,0,1) ; 
Vec3f center = Vec3f(0,0,0) ; 
Vec3f camera = Vec3f(-5,2,3) ; 
Model model("obj/african_head.obj") ; 


Vec3f m2v(Matrix m) {
return Vec3f(m[0][0]/m[3][0], m[1][0]/m[3][0], m[2][0]/m[3][0]);
}

Matrix v2m(Vec3f v) {
Matrix m(4, 1);
m[0][0] = v.x;
m[1][0] = v.y;
m[2][0] = v.z;
m[3][0] = 1.f;
return m;
}

Matrix viewPort( int x, int y , int w , int h ) {
 
  Matrix m = Matrix::identity(4) ; 
  m[0][3] = x + w/2.f ; 
  m[1][3] = y + h/2.f ; 
  m[2][3] = depth /2.f ; 
  m[0][0] = w/2.f ; 
  m[1][1] = h/2.f ; 
  m[2][2] = depth /2.f ; 
  
  return m ; 
}

Matrix lookAt ( Vec3f eye , Vec3f center , Vec3f up ) {
 
  Vec3f z = (eye-center).normalize() ; 
  Vec3f x = (up^z).normalize() ; 
  Vec3f y = z^x ; 
  Matrix result = Matrix::identity(4) ; 
  for ( int i = 0 ; i<3 ; i++ ) {
   result[0][i] = x[i] ;
   result[1][i] = y[i] ; 
   result[2][i] = z[i] ; 
   result[i][3] = -center[i] ;
  }
  return result ; 
}

void line(TGAImage &image, int x0, int y0, int z0,int vn0,int textx0, int texty0, int x1, int y1, int z1, int vn1, int textx1, int texty1) {
	bool steep = false;
	if (std::abs(x0-x1)<std::abs(y0-y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
		std::swap(z0, z1);
		std::swap(vn0, vn1);
		std::swap(textx0, textx1);
		std::swap(texty0, texty1);
	}
	int dx = x1-x0;
	int dy = y1-y0;
	int derrordx = 2*std::abs(dy);
	int errordx = 0;
	int y = y0; 
	for (int x=x0; x<=x1; x++) {

		float t; 
		if((x-x0)==(x1-x0)) { t=1 ; }
		else {t = (x-x0)/(float)(x1-x0);}

		int z = z0*(1.-t) + z1*t;
		//int intens = vn0*(1.-t) + vn1*t;
		int textx = textx0*(1.-t) + textx1*t;
		int texty = texty0*(1.-t) + texty1*t;
		TGAColor color = model.getTextures(textx,texty) ; 
		if (steep) {
			if (tab[y][x] < z ) {
				image.set(y, x,TGAColor(color.r,color.g,color.b,1));
				tab[y][x] = z ; 
			}
			
		} else {
			if(tab[x][y] < z) {
				image.set(x, y,TGAColor(color.r,color.g,color.b,1));
				tab[x][y] = z ; 
			}
		}
		errordx += derrordx;

		if (errordx>dx) {
			y += (y1>y0?1:-1);
			errordx -= 2*dx;
		}
	}


}

void triangle (TGAImage &image,Vec3f uv0, Vec3f d0,int vn0,Vec3f uv1, Vec3f d1, int vn1,Vec3f uv2, Vec3f d2, int vn2) {
	
	int x0,x1,x2,y0,y1,y2,z0,z1,z2,uvx0,uvx1,uvy0,uvy1,uvx2,uvy2 ; 
	x0 = d0.x ; x1 = d1.x ; x2 = d2.x ; 
	y0 = d0.y ; y1 = d1.y ; y2 = d2.y ; 
	z0 = d0.z ; z1 = d1.z ; z2 = d2.z ; 
	uvx0 = uv0.x ; uvy0 = uv0.y ; 
	uvx1 = uv1.x ; uvy1 = uv1.y ; 
	uvx2 = uv2.x ; uvy2 = uv2.y ; 

	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
		std::swap(z0, z1);
		std::swap(vn0, vn1);
		std::swap(uvx0, uvx1);
		std::swap(uvy0, uvy1);
	}
	if (x0>x2) {
		std::swap(x0, x2);
		std::swap(y0, y2);
		std::swap(z0, z2);
		std::swap(vn0, vn2);
		std::swap(uvx0, uvx2);
		std::swap(uvy0, uvy2);
	}
	if (x1>x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
		std::swap(z1, z2);
		std::swap(vn1, vn2);
		std::swap(uvx1, uvx2);
		std::swap(uvy1, uvy2);
	}
	float t, t2 ; 
	for (int x=x0; x<=x1; x++) { 
		if((x-x0)==(x1-x0)) { t=1 ; }
		else {t = (x-x0)/(float)(x1-x0);}
		
		int y = y0*(1.-t) + y1*t;
		int z = z0*(1.-t) + z1*t;
		int vn = vn0*(1.-t) + vn1*t;
		int textx = uvx0*(1.-t) + uvx1*t;
		int texty = uvy0*(1.-t) + uvy1*t;
		
		if((x-x0)==(x2-x0)) { t2=1; }
		else {t2 = (x-x0)/(float)(x2-x0);}
		int ybis = y0*(1.-t2) + y2*t2;
		int zbis = z0*(1.-t2) + z2*t2;
		int vnbis = vn0*(1.-t2) + vn2*t2;
		int textxbis = uvx0*(1.-t2) + uvx2*t2;
		int textybis = uvy0*(1.-t2) + uvy2*t2;
		
		line(image,x,y,z,vn,textx,texty,x,ybis,zbis,vnbis,textxbis,textybis);
  	}
 	for (int x=x1; x<=x2; x++) { 
		if((x-x1)==(x2-x1)) { t=1 ; }
		else {t = (x-x1)/(float)(x2-x1);}
		int y = y1*(1.-t) + y2*t;
		int z = z1*(1.-t) + z2*t;
		int vn = vn1*(1.-t) + vn2*t;
		int textx = uvx1*(1.-t) + uvx2*t;
		int texty = uvy1*(1.-t) + uvy2*t;

		if((x-x0)==(x2-x0)) { t2=1; }
		else {t2 = (x-x0)/(float)(x2-x0);}
		int ybis = y0*(1.-t2) + y2*t2;
		int zbis = z0*(1.-t2) + z2*t2;
		int vnbis = vn0*(1.-t2) + vn2*t2;
		int textxbis = uvx0*(1.-t2) + uvx2*t2;
		int textybis = uvy0*(1.-t2) + uvy2*t2;

		line(image,x,y,z,vn,textx,texty,x,ybis,zbis,vnbis,textxbis,textybis);
 	}

}

int main() {
	Matrix view = lookAt(camera,center,Vec3f(0,1,0)) ; 
	Matrix port = viewPort(width/8,height/8,width*3/4,height*3/4) ; 
	Matrix proj = Matrix::identity(4) ;
	proj[3][2] = -1.f/(camera-center).norm() ; 
	TGAImage image(800, 800, TGAImage::RGB);
	for (int a=0 ; a<width ; a++ ) {
		for(int b=0 ; b<height ; b++) {
			tab[a][b] =  std::numeric_limits<int>::min();
		}
	}

	for (int i=0; i<model.nfaces(); i++) {
	  vector<int> fa = model.face(i) ;  
	  vector<int> vex = model.vertexIdx(i) ; 
	  vector<int> vt = model.getLineUv(i) ; 
	  vector<float> intens ; 
	  float vns ; 
	  Vec3f coord[3] ; 
	  Vec3f uv[3] ;  
	  for ( int j=0 ; j<3 ; j++ ) { 
		Vec3f vec = model.vert(fa[j]) ;  
		Vec3f uvtest = model.getUv(vt[j])*1024 ; 
		uv[j] = uvtest ; 
		vector<float> vecNorm = model.vecNorm(vex[j]) ;	
		coord[j] = port*proj*view*v2m(vec) ; 
		vns = vecNorm[0]*light[0] + vecNorm[1]*light[1] + vecNorm[2]*light[2] ; 
		if(vns < 0) {vns = 0;}
		if(vns > 1) {vns = 1;}
		vns = vns * 255 ;
		intens.push_back(vns) ;
	  }
	  triangle(image,uv[0],coord[0],intens[0],uv[1],coord[1],intens[1],uv[2],coord[2],intens[2]) ;
	}

	image.flip_vertically();
	image.write_tga_file("dump.tga");
	
	return 0;
}
