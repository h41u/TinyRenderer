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
//http://habrahabr.ru/post/248723/
using namespace std ; 


const int width = 800 ; 
const int height = 800 ; 
const int depth = 800 ; 
int tab[width][height] ; 
Vec3f light = Vec3f(0,0,1) ; 
Vec3f center = Vec3f(0,0,0) ; 
Vec3f camera = Vec3f(-5,0,3) ; 


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

void line(TGAImage &image,int color, int x0, int y0, int z0,int vn0, int x1, int y1, int z1, int vn1) {
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
		int intens = vn0*(1.-t) + vn1*t;
		if (steep) {
			if (tab[y][x] < z ) {
				image.set(y, x, TGAColor(intens,1));
				tab[y][x] = z ; 
			}
			
		} else {
			if(tab[x][y] < z) {
				image.set(x, y, TGAColor(intens,1));
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

void triangle2 (TGAImage &image,int color,Vec2f uv0, Vec3f d0,int vn0,Vec2f uv1, Vec3f d1, int vn1,Vec2f uv2, Vec3f d2, int vn2) {
	int x0,x1,x2,y0,y1,y2,z0,z1,z2 ; 
	x0 = d0.x ; 
	x1 = d1.x ; 
	x2 = d2.x ; 
	y0 = d0.y ; 
	y1 = d1.y ; 
	y2 = d2.y ; 
	z0 = d0.z ; 
	z1 = d1.z ; 
	z2 = d2.z ; 
	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
		std::swap(z0, z1);
		std::swap(vn0, vn1);
	}
	if (x0>x2) {
		std::swap(x0, x2);
		std::swap(y0, y2);
		std::swap(z0, z2);
		std::swap(vn0, vn2);
	}
	if (x1>x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
		std::swap(z1, z2);
		std::swap(vn1, vn2);
	}
	float t, t2 ; 
	for (int x=x0; x<=x1; x++) { 
		if((x-x0)==(x1-x0)) { t=1 ; }
		else {t = (x-x0)/(float)(x1-x0);}
		
		int y = y0*(1.-t) + y1*t;
		int z = z0*(1.-t) + z1*t;
		int vn = vn0*(1.-t) + vn1*t;
		
		if((x-x0)==(x2-x0)) { t2=1; }
		else {t2 = (x-x0)/(float)(x2-x0);}
		int ybis = y0*(1.-t2) + y2*t2;
		int zbis = z0*(1.-t2) + z2*t2;
		int vnbis = vn0*(1.-t2) + vn2*t2;
		
		line(image,color, x,y,z,vn,x,ybis,zbis,vnbis);
  	}
 	for (int x=x1; x<=x2; x++) { 
		if((x-x1)==(x2-x1)) { t=1 ; }
		else {t = (x-x1)/(float)(x2-x1);}
		int y = y1*(1.-t) + y2*t;
		int z = z1*(1.-t) + z2*t;
		int vn = vn1*(1.-t) + vn2*t;

		if((x-x0)==(x2-x0)) { t2=1; }
		else {t2 = (x-x0)/(float)(x2-x0);}
		int ybis = y0*(1.-t2) + y2*t2;
		int zbis = z0*(1.-t2) + z2*t2;
		int vnbis = vn0*(1.-t2) + vn2*t2;

		line(image,color, x,y,z,vn,x,ybis,zbis,vnbis);
 	}

}

/*void triangle (TGAImage &image,int color, int x0, int y0,int z0,int vn0, int x1, int y1, int z1,int vn1, int x2, int y2, int z2, int vn2) {
	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
		std::swap(z0, z1);
		std::swap(vn0, vn1);
	}
	if (x0>x2) {
		std::swap(x0, x2);
		std::swap(y0, y2);
		std::swap(z0, z2);
		std::swap(vn0, vn2);
	}
	if (x1>x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
		std::swap(z1, z2);
		std::swap(vn1, vn2);
	}
	float t, t2 ; 
	for (int x=x0; x<=x1; x++) { 
		if((x-x0)==(x1-x0)) { t=1 ; }
		else {t = (x-x0)/(float)(x1-x0);}
		
		int y = y0*(1.-t) + y1*t;
		int z = z0*(1.-t) + z1*t;
		int vn = vn0*(1.-t) + vn1*t;
		
		if((x-x0)==(x2-x0)) { t2=1; }
		else {t2 = (x-x0)/(float)(x2-x0);}
		int ybis = y0*(1.-t2) + y2*t2;
		int zbis = z0*(1.-t2) + z2*t2;
		int vnbis = vn0*(1.-t2) + vn2*t2;
		
		line(image,color, x,y,z,vn,x,ybis,zbis,vnbis);
  	}
 	for (int x=x1; x<=x2; x++) { 
		if((x-x1)==(x2-x1)) { t=1 ; }
		else {t = (x-x1)/(float)(x2-x1);}
		int y = y1*(1.-t) + y2*t;
		int z = z1*(1.-t) + z2*t;
		int vn = vn1*(1.-t) + vn2*t;

		if((x-x0)==(x2-x0)) { t2=1; }
		else {t2 = (x-x0)/(float)(x2-x0);}
		int ybis = y0*(1.-t2) + y2*t2;
		int zbis = z0*(1.-t2) + z2*t2;
		int vnbis = vn0*(1.-t2) + vn2*t2;

		line(image,color, x,y,z,vn,x,ybis,zbis,vnbis);
 	}

}*/

int main() {
  
	Matrix view = lookAt(camera,center,Vec3f(0,1,0)) ; 
	Matrix port = viewPort(width/8,height/8,width*3/4,height*3/4) ; 
	Matrix proj = Matrix::identity(4) ;
	proj[3][2] = -1.f/(camera-center).norm() ; 
	TGAImage image(800, 800, 1);
	for (int a=0 ; a<width ; a++ ) {
		for(int b=0 ; b<height ; b++) {
			tab[a][b] =  std::numeric_limits<int>::min();
		}
	}
	Model model("obj/african_head.obj") ; 
//	model.produitVectoriel() ; 
	for (int i=0; i<model.nfaces(); i++) {
	  vector<int> fa = model.face(i) ;  
	  vector<int> vex = model.vertexIdx(i) ; 
	  /*vector<float> vt = model.getUv(0) ; 
	  cout << vt[0] << "\n" ; */
	  vector<int> vt = model.getLineUv(i) ; 
	  //cout << vt[2] << "\n" ; 
	  vector<float> drawLine ; 
//	  vector<float> norm = model.norm(i) ; 
	  //float x,y,z,vns ;
	  float vns ; 
	  Vec3f b[3] ; 
	  Vec2f uv[3] ;
	  vector<Vec3f> test ;  
	  for ( int j=0 ; j<3 ; j++ ) { 
		Vec3f vec = model.vert(fa[j]) ;  
		uv[j] = model.getUv(vt[j]) ; 
		vector<float> vecNorm = model.vecNorm(vex[j]) ;	
		b[j] = port*proj*view*v2m(vec) ; 
		/*x = (vec[0]+1)*width ;
		x=x/2 ;  
		y = (vec[1]+1)*height ;
		y=y/2 ;  
		z = (vec[2]+1)*depth ;
		z=z/2 ;  
		drawLine.push_back(x) ; 
		drawLine.push_back(y) ; 
		drawLine.push_back(z) ;*/
		// scalaire avec lumière
		vns = vecNorm[0]*light[0] + vecNorm[1]*light[1] + vecNorm[2]*light[2] ; 
		if(vns < 0) {vns = 0;}
		if(vns > 1) {vns = 1;}
		vns = vns * 255 ;
		drawLine.push_back(vns) ;
	  }				
//	float pdtScalaire = norm[0]*light[0] + norm[1]*light[1] + norm[2]*light[2] ;
//	pdtScalaire = pdtScalaire*255 ; 
	/*triangle(image,pdtScalaire,drawLine[0],drawLine[1],drawLine[2],drawLine[3],drawLine[4],drawLine[5],drawLine[6],drawLine[7],drawLine[8],drawLine[9],drawLine[10],drawLine[11]) ; */
	triangle2(image,-564654654,uv[0],b[0],drawLine[0],uv[1],b[1],drawLine[1],uv[2],b[2],drawLine[2]) ;
	}

	image.flip_vertically();
	image.write_tga_file("dump.tga");
	
	return 0;
}
