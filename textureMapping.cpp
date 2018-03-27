#include "graphics_library.h"
#include <bits/stdc++.h>
using namespace std;
using namespace objl;
struct triangle{
  
  int index[3];
  Vector3 v[3];
  Vector3 n1[3];
  Vector3 n;
  Vector3 centroid;
  bool visible;
  triangle()
  {
    visible = true;
  }

};
Mesh objMesh;
map< int, Vector3 > vertices;
map< int, Vector3 > normals;
vector< int > triangles;
map< int, triangle > objMap;
map< int, vector< int > > vertexTriangle;
int vertexcount;
int triangleCount;
Vector3 mean;
double ka,kd,ks;
double intensity,Ia;
void OBJParse(string filename)
{
   ifstream file(filename);
   string curline;
   int counter = 1;
   double minX,maxX,minY,maxY,minZ,maxZ;
   while(getline(file,curline))
   {
    if(algorithm::firstToken(curline) == "v")
    {
		  
      std::vector<std::string> spos;
		  Vector3 vpos;
		  algorithm::split(algorithm::tail(curline), spos, " ");
      vpos.X = std::stod(spos[0])*1000+150;
		  vpos.Y = std::stod(spos[1])*1000;
		  vpos.Z = std::stod(spos[2])*1000-400;
		  vertices[counter++] = vpos;
		  vertexcount++;

	  }else if(algorithm::firstToken(curline) == "f")
	  {
		 
      std::vector<std::string> spos;
		  Vector3 vpos;
		  int v1,v2,v3;
		  algorithm::split(algorithm::tail(curline), spos, " ");
      v1 = std::stoi(spos[0]);
		  v2 = std::stoi(spos[1]);
		  v3 = std::stoi(spos[2]);
		  triangles.emplace_back(v1);
		  triangles.emplace_back(v2);
		  triangles.emplace_back(v3);
      triangle t;
      t.v[0] = vertices[v1];
      t.v[1] = vertices[v2];
      t.v[2] = vertices[v3];
      t.n = math::CrossV3(t.v[1]-t.v[0],t.v[2]-t.v[0]);
      t.n = t.n*(1.0/math::MagnitudeV3(t.n));
      t.centroid = (t.v[0] + t.v[1] + t.v[2])*(1.0/3);
      t.index[0] = v1;
      t.index[1] = v2;
      t.index[2] = v3;
      vertexTriangle[v1].push_back(triangleCount);
      vertexTriangle[v2].push_back(triangleCount);
      vertexTriangle[v3].push_back(triangleCount);
      objMap[triangleCount++] = t;
    
    }

  }
}
void calculateMean()
{
	mean.X = 0;
	mean.Y = 0;
	mean.Z = 0;
	for(int it=1;it<=vertexcount;it++)
	{
        mean = mean + vertices[it];
	}
	mean = mean*(1.0/vertexcount);
}
void getNormals()
{
  int it,it1;
  for(it=1;it<=vertexcount;it++)
  {
      int temp = vertexTriangle[it].size();
      Vector3 nn;
      nn.X = 0;
      nn.Y = 0;
      nn.Z = 0;
      for(it1=0;it1<temp;it1++)
      {
             nn = nn + objMap[vertexTriangle[it][it1]].n;
      }
      nn = nn * (1.0/temp);
      cout<<nn.X<<" "<<nn.Y<<" "<<nn.Z<<endl;
      for(it1=0;it1<temp;it1++)
      {
        int q=0;
        for(q=0;q<3;q++)
        {
           if(objMap[vertexTriangle[it][it1]].index[q] == it)
           {
              objMap[vertexTriangle[it][it1]].n1[q] = nn;
              break;
           }
        }
      }
  }
}
colourHSV trace(Vector3 point,Vector3 dir,colourHSV lightColorHSV,Vector3 lightloc,colourRGB image2[],int width1,int height1)
{
   colourHSV surface;
   surface.h = lightColorHSV.h;
   surface.s = lightColorHSV.s;
   surface.v = 0;
   int k;
   double minZ = INF;
   Vector3 minPoint(-INF,-INF,-INF);
   int it;
   int minTri = -1;
  double t,u,v;
   for(it=0;it<triangleCount;it++)
   {   

       if(algorithm::rayTriangleIntersect(point,dir,objMap[it].v[0],objMap[it].v[1],objMap[it].v[2],t,u,v))
       {
            Vector3 pp = objMap[it].v[0]*(1-u-v) + objMap[it].v[1]*u + objMap[it].v[2]*v;
            if(pp.Z < minZ)
            {
            	 minZ = pp.Z;
            	 minPoint = pp;
            	 minTri = it;
            }
       }
    }
    if(minZ == INF){return surface;}
    else {
        
        Vector3 temp = lightloc - minPoint;
        //Vector3 dir = minPoint-mean;
        Vector3 dir = objMap[minTri].n1[0]*(1-u-v)+objMap[minTri].n1[1]*u+objMap[minTri].n1[2]*v;
        double d = math::MagnitudeV3(dir);
        double u1,v1;
        dir = dir*(1.0/d);
        if(math::DotV3(minPoint-mean,dir)<0)dir = dir *(-1);        
        double a1 = minPoint.X - mean.X;
        double a2 = minPoint.Y - mean.Y;
        double a3 = minPoint.Z - mean.Z;
        double A = dir.X*dir.X + dir.Y*dir.Y + dir.Z*dir.Z;
        double B = 2*(a1*dir.X+a2*dir.Y+a3*dir.Z);
        double C = a1*a1+a2*a2+a3*a3 - 1000*1000;
        double tt1 = (-B + sqrt(B*B - 4*A*C))/(2*A);
        double tt2 = (-B - sqrt(B*B - 4*A*C))/(2*A);
        Vector3 endPt;
        if(fabs(tt1)<fabs(tt2))
        {
               endPt = minPoint + dir * ( tt1);
        }else endPt = minPoint + dir * (tt2);
        double t1,t2,t3,t4,t5,t6;
        u1 = asin((endPt.X-mean.X)/1000)/PI + 0.5;
        v1 = asin((endPt.Y-mean.Y)/1000)/PI + 0.5;
        int u2 = (int)(u1*width1);
        int v2 = (int)(v1*height1);
        surface = convertRGB2HSV(image2[u2*width1+v2]);
        
    }
    return surface;
}
void backFaceCulling(Vector3 viewer)
{
   for(int it=0;it<triangleCount;it++)
   {
      Vector3 v1 = objMap[it].centroid - viewer;
      double doo = math::DotV3(v1,objMap[it].n);
      if(doo > 1e-6)
      {
         objMap[it].visible = 0;
      }
   }
}
void render(string filename)
{
    unsigned width = 200,height = 200;
    colourRGB *image = new colourRGB[width * height];

    colourHSV *image1 = new colourHSV[width*height],*pixel = image1;
    char buffer[1024];
    strcpy(buffer,filename.c_str());
    int i,j,k;
    Vector3 viewer(50,50,1000);
    backFaceCulling(viewer);
    ifstream ifs("cube.pgm", std::ios::in);
    char buff;
    char buff2[1024];
    ifs>>buff2;
    int width1,height1;
    ifs>>width1;
    ifs>>height1;
    ifs>>buff2;
    colourRGB *image2 = new colourRGB[width1*height1];
    unsigned char ch1;
    int it=0;
    while(!ifs.eof())
    {
       ifs>>ch1;
       int tempi = (unsigned int)ch1;
       image2[it++] = colourRGB(tempi/255,tempi/255,tempi/255);

    }
    ifs.close();
    for(j=0;j<height;j++)
    for(i=0;i<width;i++,pixel++)
    {
        Vector3 framePt(i,height-1-j,0);
        Vector3 lightLoc(300,300,300);
        Vector3 dir = framePt - viewer;
        *pixel = trace(viewer,dir,convertRGB2HSV(colourRGB(0,0,1)),lightLoc,image2,width1,height1);
        //fill colour at pixel
    }


    for(unsigned int i = 0;i<width*height;i++)
    {
    	image[i] = convertHSV2RGB(image1[i]);
    }
    ofstream ofs(buffer, std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i) {
        ofs << (unsigned char)(std::min(1.0, image[i].r) * 255) <<
               (unsigned char)(std::min(1.0, image[i].g) * 255) <<
               (unsigned char)(std::min(1.0, image[i].b) * 255);
    }
    ofs.close();
    delete [] image;
}
int main()
{
    ka = 1;
    kd = 0.6;
    ks = 0.5;
    intensity = 1;
    Ia = 10;
    OBJParse("bunny.obj");
    calculateMean();
    getNormals();
    render("bunny6.ppm");
    return 0;
}
