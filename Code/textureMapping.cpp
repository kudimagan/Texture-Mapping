#include "graphicsLibrary.h"

using namespace objl;

#define scale 37.795275590551
#define DIV 40
#define MULT 0.4
int width;
int height;
double scale1;

map <int, Vector3> vertices;
map <int, triangle> objMap;
map < int, vector <int> > vertexTriangle;
int vertexCount;
int triangleCount;
Vector3 mean;
double ka, kd, ks, spep;	// material reflective properties
double d0, d1, d2;			// attenuation
double Ia, Ip;				// ambient light and light source
int matr[1000][1000];

Vector3 calculateMean()
{
	Vector3 mean(0,0,0);
	for (int it = 1; it <= vertexCount; it++)
	{
       mean = mean + vertices[it];
	}
	return (mean*(1.0/vertexCount));
}
double maxX,maxY,maxZ,minX,minY,minZ;

void shiftScale (double minX,double minY)
{
	Vector3 mean(-minX,-minY,-1000);
	for (int it = 1; it <= vertexCount; ++it)
		vertices[it] = vertices[it] + mean;
}

void OBJParse (string filename)
{
	ifstream file(filename);
	string curline;
	int counter = 1;
	double minX, maxX, minY, maxY, minZ, maxZ;
	bool flag = false;bool flag1 = false;

	while (getline(file, curline))
	{
		if (algorithm::firstToken(curline) == "v")
		{
			vector<string> spos;
			Vector3 vpos;
			algorithm::split(algorithm::tail(curline), spos, " ");
			vpos.X = stod(spos[0]);//*scale*scale1;
			vpos.Y = stod(spos[1]);//*scale*scale1;
			vpos.Z = stod(spos[2]);//*scale*scale1;
			vertices[counter++] = vpos;
			vertexCount++;
			flag1 = true;
			if(flag==false)
			 {
			 	flag = true;
			 	minX = maxX = vpos.X;
			 	minY = maxY = vpos.Y;
			 	minZ = maxZ = vpos.Z;

			 }else
			 {
			 	minX = min(minX,vpos.X);
			 	maxX = max(maxX,vpos.X);
			 	minY = min(minY,vpos.Y);
			 	maxY = max(maxY,vpos.Y);
			 	minZ = min(minZ,vpos.Z);
			 	maxZ = max(maxZ,vpos.Z);
			 }
		}

		else if (flag1)
			break;
	}

	shiftScale(minX,minY);
	mean = calculateMean();

	do
	{
		if (algorithm::firstToken(curline) == "f")
		{
			vector<string> spos;
			Vector3 vpos;
			int v1, v2, v3;
			algorithm::split(algorithm::tail(curline), spos, " ");
			v1 = stoi(spos[0]);
			v2 = stoi(spos[1]);
			v3 = stoi(spos[2]);
			triangle t;
			t.v[0] = vertices[v1];
			t.v[1] = vertices[v2];
			t.v[2] = vertices[v3];
			t.n = math::CrossV3(t.v[1]-t.v[0], t.v[2]-t.v[0]);
			t.n.UnifyV3();
			t.centroid = (t.v[0] + t.v[1] + t.v[2]) / 3;
			t.index[0] = v1;
			t.index[1] = v2;
			t.index[2] = v3;
			vertexTriangle[v1].push_back(triangleCount);
			vertexTriangle[v2].push_back(triangleCount);
			vertexTriangle[v3].push_back(triangleCount);
			objMap[triangleCount++] = t;
		}
	} while (getline(file, curline));

	//cout << "Vertices: " << vertexCount << "\n";
	//cout << "Faces: " << triangleCount << "\n";
}

void getNormals()
{
	for (int it = 1; it <= vertexCount; it++)
	{
		int temp = vertexTriangle[it].size();
		Vector3 nn;
		for (int it1 = 0; it1 < temp; it1++)
			nn = nn + objMap[vertexTriangle[it][it1]].n;
		nn.UnifyV3();
		//cout<<nn.X<<" "<<nn.Y<<" "<<nn.Z<<endl;
		for (int it1 = 0; it1 < temp; it1++)
		{
			for (int q = 0; q < 3; q++)
			{
				if (objMap[vertexTriangle[it][it1]].index[q] == it)
				{
					objMap[vertexTriangle[it][it1]].n1[q] = nn;
					break;
				}
			}
		}
	}
}

colourRGB trace (Vector3 point, Vector3 dir, colourRGB lightColorRGB, Vector3 lightloc, int mode, colourRGB * image2 = NULL, int width1 = 0, int height1 = 0)
{
	colourHSV surface;
	colourRGB sRGB;
	surface = convertRGB2HSV(lightColorRGB * Ip);
	surface.v = 0;

	double f, att, ang, minZ = INF;
	Vector3 minPoint(-INF,-INF,-INF);
	int minTri = -1;
	double t, u, v;

	// Z-buffer
	for (int it = 0; it < triangleCount; it++)
	{	 
		if (objMap[it].visible)
		{
			if (algorithm::rayTriangleIntersect(point, dir, objMap[it].v[0], objMap[it].v[1], objMap[it].v[2], t, u, v))
			{
				Vector3 pp = objMap[it].v[0] * (1-u-v) + objMap[it].v[1] * u + objMap[it].v[2] * v;
				if (pp.Z < minZ)
				{
					minZ = pp.Z;
					minPoint = pp;
					minTri = it;
				}
			}
		}
	}

	if ((minZ == INF) || (minTri == -1))
	{
		sRGB = convertHSV2RGB (surface);
		//sRGB = sRGB + colourRGB(Ia, Ia, Ia) * ka;
		return sRGB;
	}

	else 
	{	
		if (mode == 1)
		{
			Vector3 director = minPoint - mean;
			director.UnifyV3();
			double ut = sqrt(director.X * director.X + director.Z * director.Z);
			double theta = acos(director.X / ut);
			double phi = acos(ut);
			int theta1;
			int phi1;
			if (director.Z < 0)
				theta = 2 * M_PI - theta;
			if (director.Y < 0)
				phi = 2 * M_PI - phi;
			theta /= (2 * M_PI / DIV);
			phi /= (2 * M_PI / DIV);
		    if(theta-int(theta)>=0.5)
                theta1 = int(theta)+1;
            else theta1 = int(theta);
            if(phi1-int(phi1)>=0.5)
            	phi1 = int(phi)+1;
            else phi1 = int(phi);
			surface.v =  (theta1 + phi1 + 1) % 2;
		}
		else if (mode == 2)
		{
			Vector3 mean1 = mean;
			mean1.Y = minPoint.Y; 
			Vector3 director = minPoint - mean1;
			double r = sqrt(director.X * director.X + director.Z * director.Z);
			double theta = acos(director.X / r);
			if (director.Z < 0)
				theta = 2 * M_PI - theta;
			theta /= (2 * M_PI / DIV);
			r /= DIV;
			surface.v = ((int)theta + (int)(director.Y / (MULT * DIV))) % 2;
		}
		else if (mode == 3)
			surface.v = ((int)(minPoint.X / (MULT * DIV)) + (int)(minPoint.Y / (MULT * DIV))) % 2;

		Vector3 lcap = lightloc - minPoint;
		f = math::MagnitudeV3(lcap);
		lcap = lcap / f;
		Vector3 ncap = objMap[minTri].n1[0]*(1-u-v)+objMap[minTri].n1[1]*u+objMap[minTri].n1[2]*v;
		ncap.UnifyV3();
		ang = math::DotV3 (ncap, lcap);
		Vector3 rcap = 2 * (ang * ncap) - lcap;
		Vector3 vcap = dir;
		vcap.UnifyV3();
		att = 1.0 / (d0 + d1 * f + d2 * f*f);

		//surface.v = surface.v * (att * (kd * ang + ks * pow(math::DotV3(rcap, vcap), spep)));
	}

	// Ambient light
	sRGB = convertHSV2RGB (surface);
	sRGB = sRGB + colourRGB(Ia, Ia, Ia) * ka;

	return sRGB;
}

//removing directly invisible faces
void backFaceCulling (Vector3 viewer)
{
	for (int it = 0; it < triangleCount; it++)
	{
		Vector3 v1 = objMap[it].centroid - viewer;
		v1.UnifyV3();
		double doo = math::DotV3(v1, objMap[it].n);
		if (doo > vEpsilon)
			objMap[it].visible = 0;
	}
}

//renders image with texture
//mode = 0 => No texture
//mode = 1 => Spherical Texture
//mode = 2 => Cylindrical Texture
//mode = 3 => Planar Texture
void render (string filename, int mode = 0)
{
	colourRGB * image = new colourRGB[width * height];
	colourRGB * pixel = image;
	
	//Back Face Culling
	Vector3 viewer(width/2,height/2, 1000);
	backFaceCulling(viewer);

	// Processing texture
	cout << "Rendering Image:" << filename << "\nProgress...\n";
	for (int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++, pixel++)
		{
			Vector3 framePt(i, height-1-j, 0);
			Vector3 lightLoc(width/2,height/2,300);
			Vector3 dir = framePt - viewer;
			//*pixel = trace (viewer, dir, colourRGB(0,0,1), lightLoc, image2, width1, height1);
			*pixel = trace (viewer, dir, colourRGB(1,1,1), lightLoc, mode);
			if((*pixel).r || (*pixel).g || (*pixel).b)
			{
				matr[i][height-1-j] = 1;
			}else matr[i][height-1-j] = 0;
			//fill colour at pixel
			progressBar (((j+1) * height + (i + 1)) * 1.0 / (width * height));
		}
	}
	for(int j = 0; j < height;j++)
		{for(int i = 0 ; i < width ; i++)
		{
			 cout<<matr[i][j]<<" ";
		}
		cout<<endl;
	}
	pixel = image;
	double maxim = colorNormalise(image,width,height);
	cout << "\nImage file opened for writing.\n";
	ofstream ofs (filename.c_str(), ios::out);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	
	for (unsigned i = 0; i < width * height; ++i) 
		ofs << (unsigned char)((image[i].r / maxim) * 255) 
			<< (unsigned char)((image[i].g / maxim) * 255) 
			<< (unsigned char)((image[i].b / maxim) * 255);
	
	ofs.close();
	delete [] image;
}

int main(int argc,char** argv)
{
	ka = 1;
	kd = 0.5;
	ks = 0.5;
	spep = 1;
	d0 = 1;
	d1 = 0;
	d2 = 0;
	Ia = 0.2;
	Ip = 0.6;
	width = atoi(argv[1]);
	height = atoi(argv[2]);
	scale1 = atoi(argv[6]);
	int mode = atoi(argv[4]);
	char buffer[1024] = "../Objects/";
	strcat(buffer,argv[3]);

	OBJParse(buffer);
	cout << "Object file parsed.\n";
	getNormals();
	cout << "Face normals calculated.\n";
	char buffer1[1024] = "../Images/";
	strcat(buffer1,argv[5]);
	render(buffer1, mode);
	cout << "Image rendered.\n";
	return 0;
}
