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

using namespace std ; 
int tab[800][800] ; 
 

void line(TGAImage &image,int color, int x0, int y0, int z0, int x1, int y1, int z1) {
	/*
	x0 = 13; y0 = 20; x1 = 80; y1 = 40;
	for (float t=0.; t<1.; t+=.01) { // on sais pas comment choisir le pas
		int x = x0*(1.-t) + x1*t;
		int y = y0*(1.-t) + y1*t;
		image.set(x, y, TGAColor(255, 1));
	}
	image.clear();

	// deux problemes: 1) pente 2) x0>x1
	x0 = 20; y0 = 13; x1 = 40; y1 = 80;
	x1 = 13; y1 = 20; x0 = 80; y0 = 40;
	for (int x=x0; x<=x1; x++) { 
		float t = (x-x0)/(float)(x1-x0);
		int y = y0*(1.-t) + y1*t;
		image.set(x, y, TGAColor(255, 1));
	}

	image.clear();

	x1 = 13; y1 = 20; x0 = 80; y0 = 40;
	x0 = 20; y0 = 13; x1 = 40; y1 = 80;
	bool steep = false;
	if (std::abs(x0-x1)<std::abs(y0-y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	for (int x=x0; x<=x1; x++) { 
		float t = (x-x0)/(float)(x1-x0); // supercher
		int y = y0*(1.-t) + y1*t;        // cher
		if (steep) {
			image.set(y, x, TGAColor(255, 1));
		} else {
			image.set(x, y, TGAColor(255, 1));
		}
	}

	image.clear();
	*/

	/*
	bool steep = false;
	if (std::abs(x0-x1)<std::abs(y0-y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1-x0;
	int dy = y1-y0;
	float derror = std::abs(dy/float(dx));
	float error = 0;
	int y = y0;
	for (int x=x0; x<=x1; x++) {
		if (steep) {
			image.set(y, x, TGAColor(255, 1));
		} else {
			image.set(x, y, TGAColor(255, 1));
		}
		error += derror;
		if (error>.5) {
			y += (y1>y0?1:-1);
			error -= 1.;
		}
	}

	*/
	bool steep = false;
	if (std::abs(x0-x1)<std::abs(y0-y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
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
		if (steep) {
			if (tab[y][x] < z ) {
				image.set(y, x, TGAColor(color,1));
				tab[y][x] = z ; 
			}
			
		} else {
			if(tab[x][y] < z) {
				image.set(x, y, TGAColor(color,1));
				tab[x][y] = z ; 
			}
			//image.set(x, y, TGAColor(color,1));
		}
		errordx += derrordx;

		if (errordx>dx) {
			y += (y1>y0?1:-1);
			errordx -= 2*dx;
		}
	}


}
void triangle (TGAImage &image,int color, int x0, int y0,int z0, int x1, int y1, int z1, int x2, int y2, int z2) {
	//cout << "en cours" ; 	
	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
		std::swap(z0, z1);
	}
	if (x0>x2) {
		std::swap(x0, x2);
		std::swap(y0, y2);
		std::swap(z0, z2);
	}
	if (x1>x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
		std::swap(z1, z2);
	}
	float t, t2 ; 
	for (int x=x0; x<=x1; x++) { 
		if((x-x0)==(x1-x0)) { t=1 ; }
		else {t = (x-x0)/(float)(x1-x0);}
		int y = y0*(1.-t) + y1*t;
		int z = z0*(1.-t) + z1*t;
		
		if((x-x0)==(x2-x0)) { t2=1; }
		else {t2 = (x-x0)/(float)(x2-x0);}
		int ybis = y0*(1.-t2) + y2*t2;
		int zbis = z0*(1.-t2) + z2*t2;
		
		line(image,color, x,y,z,x,ybis,zbis);
  	}
 	for (int x=x1; x<=x2; x++) { 
		if((x-x1)==(x2-x1)) { t=1 ; }
		else {t = (x-x1)/(float)(x2-x1);}
		int y = y1*(1.-t) + y2*t;
		int z = z1*(1.-t) + z2*t;
		if((x-x0)==(x2-x0)) { t2=1; }
		else {t2 = (x-x0)/(float)(x2-x0);}
		int ybis = y0*(1.-t2) + y2*t2;
		int zbis = z0*(1.-t2) + z2*t2;
		line(image,color, x,y,z,x,ybis,zbis);
 	}

}

int main() {
	TGAImage image(800, 800, 1);
	/*TGAImage image(100, 100, 1);
	triangle(image,0,0,100,100,50,90) ; */
	//line(image, 13, 20, 80, 40);
	/*srand(time(NULL)) ; 
	cout << "non" ; 
	for(int i = 0 ; i<2000 ; i=i+200 ) {
		cout << "oui" ;
		for(int j = 0 ; j<1000 ; j=j+100 ) {
			cout << "eoui" ; 
			int x0 = (rand()%100)+1 ; 
			int y0 = (rand()%100)+1 ; 
			int x1 = (rand()%100)+1 ; 
			int y1 = (rand()%100)+1 ; 
			int x2 = (rand()%100)+1 ; 
			int y2 = (rand()%100)+1 ; 
			triangle(image,x0+i,y0+j,x1+i,y1+j,x2+i,y2+j) ;
			line(image,x0+100+i,y0+j,x1+i+100,y1+j) ; 
			line(image,x0+100+i,y0+j,x2+i+100,y2+j) ;
			line(image,x1+100+i,y1+j,x2+i+100,y2+j) ;	
		}
	}*/
	
	for (int a=0 ; a<800 ; a++ ) {
		for(int b=0 ; b<800 ; b++) {
			tab[a][b] =  std::numeric_limits<int>::min();
		}
	}
	Model model("obj/african_head.obj") ; 
	model.produitVectoriel() ; 
	//cout << (int)model.nnorm() << "\n" ; 
	for (int i=0; i<model.nfaces(); i++) {
	  vector<int> fa = model.face(i) ;  
	  vector<float> drawLine ; 
	  vector<float> norm = model.norm(i) ; 
	  for ( int j=0 ; j<3 ; j++ ) {
		vector<float> vec = model.vert(fa[j]) ;  
		float x = (vec[0]+1)*800 ;
		x=x/2 ;  
		float y = (vec[1]+1)*800 ;
		y=y/2 ;  
		float z = (vec[2]+1)*800 ;
		z=z/2 ;  
		drawLine.push_back(x) ; 
		drawLine.push_back(y) ; 
		drawLine.push_back(z) ;
	  }
	/*(image,drawLine[0],drawLine[1],drawLine[2],drawLine[3]) ; 
	line(image,drawLine[0],drawLine[1],drawLine[4],drawLine[5]) ; 
	line(image,drawLine[2],drawLine[3],drawLine[4],drawLine[5]) ;*/
	float pdtScalaire = norm[2]*1 ;
	pdtScalaire = pdtScalaire*255 ; 
	//cout << pdtScalaire << "\n" ;  
	triangle(image,pdtScalaire,drawLine[0],drawLine[1],drawLine[2],drawLine[3],drawLine[4],drawLine[5],drawLine[6],drawLine[7],drawLine[8]) ; 
	} 
	image.flip_vertically();
	image.write_tga_file("dump.tga");
	
	return 0;
}
