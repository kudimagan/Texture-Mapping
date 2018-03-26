#include "graphics_library.h"
using namespace std;
using namespace objl;
struct triangle{
  
  int index[3];
  Vector3 v[3];
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
int vertexcount;
int triangleCount;
Vector3 mean;
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
      vpos.X = std::stod(spos[0])*1000+100;
		  vpos.Y = std::stod(spos[1])*1000;
		  vpos.Z = std::stod(spos[2])*1000-200;
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
      t.centroid = (t.v[0] + t.v[1] + t.v[2])*(1.0/3);
      t.index[0] = v1;
      t.index[1] = v2;
      t.index[2] = v3;
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
colourHSV trace(Vector3 point,Vector3 dir,colourHSV lightColorHSV)
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
   for(it=0;it<triangleCount;it++)
   {   
       double t,u,v;
       if(algorithm::rayTriangleIntersect(point,dir,objMap[it].v[0],objMap[it].v[1],objMap[it].v[2],t,u,v))
       {
            Vector3 pp = objMap[it].v[0]*(1-u-v) + objMap[it].v[1]*u + objMap[it].v[2]*v;
            if(pp.Z < minZ)
            {
            	 minZ = pp.Z;
            	 minPoint = pp;
            	 minTri = it/3;
            }
       }
    }
    if(minZ == INF){return surface;}
    else {surface.v=1;}
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
    unsigned width = 200,height = 100;
    colourRGB *image = new colourRGB[width * height];
    colourHSV *image1 = new colourHSV[width*height],*pixel = image1;
    char buffer[1024];
    strcpy(buffer,filename.c_str());
    int i,j,k;
    Vector3 viewer(50,50,1000);
    backFaceCulling(viewer);
    for(j=0;j<height;j++)
    for(i=0;i<width;i++,pixel++)
    {
        Vector3 framePt(i,height-1-j,0);
        Vector3 lightLoc(900,900,900);
        Vector3 dir = framePt - viewer;
        *pixel = trace(viewer,dir,convertRGB2HSV(colourRGB(0,0,1)));
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
    OBJParse("mug.obj");
    calculateMean();
    render("mug.ppm");
    return 0;
}
