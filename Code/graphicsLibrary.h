// OBJ_Loader.h - A Single Header OBJ Model Loader

#pragma once

// Iostream - STD I/O Library
#include <bits/stdc++.h>

#define INF 1e9
#define kEpsilon 1e-8
#define vEpsilon 1e-3
#define cEpsilon 1e-2

using namespace std;

// Print progress to console while loading (large models)
#define OBJL_CONSOLE_OUTPUT

// Structure: Vector2
// Description: A 2D Vector that Holds Positional Data
struct Vector2
{
	// Default Constructor
	Vector2() {X = 0.0; Y = 0.0;}
	// Variable Set Constructor
	Vector2(double X_, double Y_) {X = X_; Y = Y_;}

	// Bool Equals Operator Overload
	bool operator == (const Vector2& other) const {return (this->X == other.X && this->Y == other.Y);}
	// Bool Not Equals Operator Overload
	bool operator != (const Vector2& other) const {return !(this->X == other.X && this->Y == other.Y);}
	// Addition Operator Overload
	Vector2 operator + (const Vector2& right) const {return Vector2(this->X + right.X, this->Y + right.Y);}
	// Subtraction Operator Overload
	Vector2 operator - (const Vector2& right) const {return Vector2(this->X - right.X, this->Y - right.Y);}
	// double Multiplication Operator Overload
	Vector2 operator * (const double& other) const {return Vector2(this->X *other, this->Y * other);}

	// Positional Variables
	double X;
	double Y;
};

// Structure: Vector3
// Description: A 3D Vector that Holds Positional Data
struct Vector3
{
	// Default Constructor
	Vector3() {X = 0.0; Y = 0.0; Z = 0.0;}
	// Variable Set Constructor
	Vector3 (double X_, double Y_, double Z_) {X = X_; Y = Y_; Z = Z_;}

	// Bool Equals Operator Overload
	bool operator == (const Vector3& other) const {return (this->X == other.X && this->Y == other.Y && this->Z == other.Z);}
	// Bool Not Equals Operator Overload
	bool operator != (const Vector3& other) const {return !(this->X == other.X && this->Y == other.Y && this->Z == other.Z);}
	// Addition Operator Overload
	Vector3 operator + (const Vector3& right) const {return Vector3(this->X + right.X, this->Y + right.Y, this->Z + right.Z);}
	// Subtraction Operator Overload
	Vector3 operator - (const Vector3& right) const {return Vector3(this->X - right.X, this->Y - right.Y, this->Z - right.Z);}
	// double Multiplication Operator Overload
	Vector3 operator * (const double& other) const {return Vector3(this->X * other, this->Y * other, this->Z * other);}
	// double Division Operator Overload
	Vector3 operator / (const double& other) const {return Vector3(this->X / other, this->Y / other, this->Z / other);}

	// Vector3 Magnitude Calculation
	double MagnitudeV3 () {return (sqrt(pow(this->X, 2) + pow(this->Y, 2) + pow(this->Z, 2)));}
	// Vector3 Magnitude Calculation
	void UnifyV3 () {*this = *this / this->MagnitudeV3();}

	// Positional Variables
	double X;
	double Y;
	double Z;
};

// Vector3 Multiplication Operator Overload
Vector3 operator * (const double& left, const Vector3& right) {return Vector3(right.X * left, right.Y * left, right.Z * left);}

// Namespace: OBJL
// Description: The namespace that holds everything that
// is needed and used for the OBJ Model Loader
namespace objl
{

	struct triangle
	{
		int index[3];
		Vector3 v[3];
		Vector3 n1[3];
		Vector3 n;
		Vector3 centroid;
		bool visible;
		triangle() {visible = true;}
	};

	/*
	// Structure: Vertex
	//
	// Description: Model Vertex object that holds
	//	a Position, Normal, and Texture Coordinate
	struct Vertex
	{
		// Position Vector
		Vector3 Position;

		// Normal Vector
		Vector3 Normal;

		// Texture Coordinate Vector
		Vector2 TextureCoordinate;
	};

	struct Material
	{
		Material()
		{
			name;
			Ns = 0.0;
			Ni = 0.0;
			d = 0.0;
			illum = 0;
		}

		// Material Name
		string name;
		// Ambient Color
		Vector3 Ka;
		// Diffuse Color
		Vector3 Kd;
		// Specular Color
		Vector3 Ks;
		// Specular Exponent
		double Ns;
		// Optical Density
		double Ni;
		// Dissolve
		double d;
		// Illumination
		int illum;
		// Ambient Texture Map
		string map_Ka;
		// Diffuse Texture Map
		string map_Kd;
		// Specular Texture Map
		string map_Ks;
		// Specular Hightlight Map
		string map_Ns;
		// Alpha Texture Map
		string map_d;
		// Bump Map
		string map_bump;
	};

	// Structure: Mesh
	//
	// Description: A Simple Mesh Object that holds
	//	a name, a vertex list, and an index list
	struct Mesh
	{
		// Default Constructor
		Mesh()
		{

		}
		// Variable Set Constructor
		Mesh(vector<Vertex>& _Vertices, vector<unsigned int>& _Indices)
		{
			Vertices = _Vertices;
			Indices = _Indices;
		}
		// Mesh Name
		string MeshName;
		// Vertex List
		vector<Vertex> Vertices;
		// Index List
		vector<unsigned int> Indices;

		// Material
		Material MeshMaterial;
	};
	*/
}

// Namespace: Math
// Description: The namespace that holds all of the math functions need for OBJL
namespace math
{
	// Vector3 max
	Vector3 max (Vector3 us, Vector3 them)
	{
		if (us.X < them.X)
			{us.X = them.X;us.Y = them.Y;us.Z = them.Z;}
		return us;
	}

	// Vector3 min
	Vector3 min (Vector3 us, Vector3 them)
	{
		if (us.X > them.X)
         {us.X = them.X;us.Y = them.Y;us.Z = them.Z;}
		return us;
	}

	// Vector3 Magnitude Calculation
	double MagnitudeV3(const Vector3 in) {return (sqrt(pow(in.X, 2) + pow(in.Y, 2) + pow(in.Z, 2)));}
	// Vector3 Cross Product
	Vector3 CrossV3(const Vector3 a, const Vector3 b) {return Vector3(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);}
	// Vector3 DotProduct
	double DotV3(const Vector3 a, const Vector3 b) {return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);}
	// Angle between 2 Vector3 Objects
	double AngleBetweenV3(const Vector3 a, const Vector3 b) {return acos(DotV3(a, b) / (MagnitudeV3(a) * MagnitudeV3(b)));}
}


// Namespace: Algorithm
// Description: The namespace that holds all of the algorithms needed for OBJL
namespace algorithm
{	
	bool inTriangle (Vector3 P, Vector3 v0, Vector3 v1, Vector3 v2)
	{
		// Starting vars
		// compute plane's normal
		Vector3 v0v1 = v1 - v0;
		Vector3 v0v2 = v2 - v0;
		// no need to normalize
		Vector3 N = math::CrossV3(v0v1,v0v2); // N
		Vector3 C; // vector perpendicular to triangle's plane

		// edge 0
		Vector3 edge0 = v1 - v0;
		Vector3 vp0 = P - v0;
		C = math::CrossV3(edge0,vp0);
		if (math::DotV3(N,C) < 0){return false;} // P is on the right side

		// edge 1
		Vector3 edge1 = v2 - v1;
		Vector3 vp1 = P - v1;
		C = math::CrossV3(edge1,vp1);
		if ((math::DotV3(N,C)) < 0){return false;} // P is on the right side

		// edge 2
		Vector3 edge2 = v0 - v2;
		Vector3 vp2 = P - v2;
		C = math::CrossV3(edge2,vp2);
		if ((math::DotV3(N,C)) < 0){return false;} // P is on the right side;

		return true; // this ray hits the triangle
	}

	// Check to see if a Vector3 Point is within a 3 Vector3 Triangle
	bool rayTriangleIntersect(const Vector3 &orig, const Vector3 &dir,const Vector3 &v0, const Vector3 &v1, const Vector3 &v2,double &t,double &u,double& v)
	{
			
		// compute plane's normal
		Vector3 v0v1 = v1 - v0;
		Vector3 v0v2 = v2 - v0;
		// no need to normalize
		Vector3 N = math::CrossV3(v0v1,v0v2); // N
		double denom = math::DotV3(N,N);

		// Step 1: finding P

		// check if ray and plane are parallel ?
		double NdotRayDirection = math::DotV3(N,dir);
		if (fabs(NdotRayDirection) < kEpsilon) // almost 0
			return false; // they are parallel so they don't intersect !

		// compute d parameter using equation 2
		double d = -math::DotV3(N,v0);

		// compute t (equation 3)
		t = (math::DotV3(N,orig) + d) / NdotRayDirection;
		t=-t;
		// check if the triangle is in behind the ray
		if (t < 0) 
			return false; // the triangle is behind

		// compute the intersection point using equation 1
		Vector3 P = orig + t * dir;

		// Step 2: inside-outside test
		Vector3 C; // vector perpendicular to triangle's plane

		// edge 0
		Vector3 edge0 = v1 - v0;
		Vector3 vp0 = P - v0;
		C = math::CrossV3(edge0,vp0);
		if (math::DotV3(N,C) < 0)
			return false; // P is on the right side

		// edge 1
		Vector3 edge1 = v2 - v1;
		Vector3 vp1 = P - v1;
		C = math::CrossV3(edge1,vp1);
		if ((u = math::DotV3(N,C)) < 0)
			return false; // P is on the right side

		// edge 2
		Vector3 edge2 = v0 - v2;
		Vector3 vp2 = P - v2;
		C = math::CrossV3(edge2,vp2);
		if ((v = math::DotV3(N,C)) < 0)
			return false; // P is on the right side;
		
		u /= denom;
		v /= denom;

		return true; // this ray hits the triangle
	} 

	// Split a String into a string array at a given token
	inline void split (const string &in, vector<string> &out, string token)
	{
		out.clear();

		string temp;

		for (int i = 0; i < int(in.size()); i++)
		{
			string test = in.substr(i, token.size());

			if (test == token)
			{
				if (!temp.empty())
				{
					out.push_back(temp);
					temp.clear();
					i += (int)token.size() - 1;
				}

				else
					out.push_back("");
			}

			else if (i + token.size() >= in.size())
			{
				temp += in.substr(i, token.size());
				out.push_back(temp);
				break;
			}

			else
				temp += in[i];
		}
	}

	// Get tail of string after first token and possibly following spaces
	inline string tail(const string &in)
	{
		size_t token_start = in.find_first_not_of(" \t");
		size_t space_start = in.find_first_of(" \t", token_start);
		size_t tail_start = in.find_first_not_of(" \t", space_start);
		size_t tail_end = in.find_last_not_of(" \t");

		if (tail_start != string::npos && tail_end != string::npos)
			return in.substr(tail_start, tail_end - tail_start + 1);
		else if (tail_start != string::npos)
			return in.substr(tail_start);

		return "";
	}

	// Get first token of string
	inline string firstToken(const string &in)
	{
		if (!in.empty())
		{
			size_t token_start = in.find_first_not_of(" \t");
			size_t token_end = in.find_first_of(" \t", token_start);
			if (token_start != string::npos && token_end != string::npos)
				return in.substr(token_start, token_end - token_start);
			else if (token_start != string::npos)
				return in.substr(token_start);
		}

		return "";
	}

	// Get element at given index position
	template <class T>
	inline const T & getElement(const vector<T> &elements, string &index)
	{
		int idx = stoi(index);
		if (idx < 0)
			idx = int(elements.size()) + idx;
		else
			idx--;
		return elements[idx];
	}
}

struct colourHSV
{	
	double h,s,v;

	colourHSV(double r = 0, double g = 0,double b = 0) {h = r; s = g; v = b;}
};

struct colourRGB
{
	double r, g, b;

	colourRGB (double rr = 0, double gg = 0, double bb = 0) {r = rr; g = gg; b = bb;}
	colourRGB operator * (const colourRGB &t) const {return colourRGB(r*t.r, g*t.g, b*t.b);}
	colourRGB operator + (const colourRGB &t) const {return colourRGB(r+t.r, g+t.g, b+t.b);}
	colourRGB operator * (const double& f) const {return colourRGB(r*f, g*f, b*f);}
};

colourHSV convertRGB2HSV (colourRGB in)
{
	colourHSV out;
	double min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max > in.b ? max : in.b;

	out.v = max;								// v
	delta = max - min;
	if (delta < 0.00001)
	{
		out.s = 0.0;
		out.h = 0; 	// undefined, maybe nan?
		return out;
	}

	if (max > 0.0)  // NOTE: if Max is == 0, this divide would cause a crash
		out.s = (delta / max);					// s
	else 
	{
		// if max is 0, then r = g = b = 0				
		// s = 0, h is undefined
		out.s = 0.0;
		out.h = NAN;							// its now undefined
		return out;
	}
	if (in.r >= max)							// > is bogus, just keeps compiler happy
		out.h = (in.g - in.b) / delta;			// between yellow & magenta
	else if (in.g >= max)
		out.h = 2.0 + (in.b - in.r) / delta;	// between cyan & yellow
	else
		out.h = 4.0 + (in.r - in.g) / delta;	// between magenta & cyan

	out.h *= 60.0;								// degrees

	if (out.h < 0.0)
		out.h += 360.0;

	return out;
}

colourRGB convertHSV2RGB(colourHSV in)
{
	double hh, p, q, t, ff;
	long i;
	colourRGB out;

	if (in.s <= 0.0) 	// < is bogus, just shuts up warnings
	{		 
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}

	hh = in.h;
	if (hh >= 360.0) 
		hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch(i) 
	{
		case 0:
			out.r = in.v;
			out.g = t;
			out.b = p;
			break;
		case 1:
			out.r = q;
			out.g = in.v;
			out.b = p;
			break;
		case 2:
			out.r = p;
			out.g = in.v;
			out.b = t;
			break;
		case 3:
			out.r = p;
			out.g = q;
			out.b = in.v;
			break;
		case 4:
			out.r = t;
			out.g = p;
			out.b = in.v;
			break;
		case 5:
		default:
			out.r = in.v;
			out.g = p;
			out.b = q;
			break;
	}

	return out;	 
}

void progressBar (double progress)
{
    int barWidth = 70;
    cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i <= barWidth; ++i)
    {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(progress * 100.0) << " %\r";
    cout.flush();
}

/*
struct rgbInt
{
	int r, g, b;

	colourRGB convertTo01() {return colourRGB (r * 1.0/255, g * 1.0/255, b * 1.0/255);}
};
*/


