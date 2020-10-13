#include "MathLib.h"

#pragma region Vector2D

//Konstructor
Vector2D::Vector2D()
{

}
Vector2D::Vector2D(float x, float y)
{
	cord[0] = x;
	cord[1] = y;
}

Vector2D::~Vector2D()
{
}

//Functions
void Vector2D::Set(float x, float y)
{
	cord[0] = x;
	cord[1] = y;
}
void Vector2D::SetX(float x)
{
	cord[0] = x;
}
void Vector2D::SetY(float y)
{
	cord[1] = y;
}
float Vector2D::Norm()
{
	return (sqrt(pow(cord[0], 2) + pow(cord[1], 2)));
}

//Operators
float Vector2D::operator*(Vector2D rhs)
{
	return (this->GetX() * rhs.GetX() + this->GetY() * rhs.GetY());
}
void Vector2D::operator*(float rhs)
{
	this->cord[0] = this->cord[0] * rhs;
	this->cord[1] = this->cord[1] * rhs;
}
Vector2D Vector2D::operator+(Vector2D& rhs)
{
	Vector2D temp((this->GetX() + rhs.GetX()), (this->GetY() + rhs.GetY()));
	return temp;
}
Vector2D Vector2D::operator-(Vector2D& rhs)
{
	return Vector2D((this->GetX() - rhs.GetX()), (this->GetY() - rhs.GetY()));
}
void Vector2D::operator=(Vector2D& rhs)
{
	this->cord[0] = rhs.GetX();
	this->cord[1] = rhs.GetY();
}
bool Vector2D::operator==(Vector2D& rhs)
{
	if (this->GetX() == rhs.GetX() && this->GetY() == rhs.GetY())
	{
		return true;
	}
	return false;
}
bool Vector2D::operator!=(Vector2D& rhs)
{
	if (this->GetX() != rhs.GetX() || this->GetY() != rhs.GetY())
	{
		return true;
	}
	return false;
}
bool Vector2D::operator<(Vector2D& rhs)
{ // is this smaller then rhs
	if (this->GetX() < rhs.GetX() && this->GetY() < rhs.GetY())
	{
		return true;
	}
	return false;
}
bool Vector2D::operator<=(Vector2D& rhs)
{ // is this smaller then rhs
	if (this->GetX() <= rhs.GetX() && this->GetY() <= rhs.GetY())
	{
		return true;
	}
	return false;
}
bool Vector2D::operator>(Vector2D& rhs)
{ // is this bigger then rhs
	if (this->GetX() > rhs.GetX() && this->GetY() > rhs.GetY())
	{
		return true;
	}
	return false;
}
bool Vector2D::operator>=(Vector2D& rhs)
{ // is this bigger then rhs
	if (this->GetX() >= rhs.GetX() && this->GetY() >= rhs.GetY())
	{
		return true;
	}
	return false;
}
///Returns a refrence to the value of the x position in the Vector
float& Vector2D::operator[] (const int x)		//Set
{
	return cord[x];
}
///Returns a constant to the value of the x position in the Vector
float Vector2D::operator[] (const int x) const	//Get
{
	return cord[x];
}

#pragma endregion

#pragma region Matrix2D
//Konstructor
Matrix2D::Matrix2D()
{
	matrix[0][0] = 1.0f;
	matrix[0][1] = 0.0f;
	matrix[1][0] = 0.0f;
	matrix[1][1] = 1.0f;
}
Matrix2D::Matrix2D(float x1, float y1, float x2, float y2)
{
	matrix[0][0] = x1;
	matrix[0][1] = y1;
	matrix[1][0] = x2;
	matrix[1][1] = y2;
}
Matrix2D::Matrix2D(Vector2D x, Vector2D y)
{
	matrix[0][0] = x.GetX();
	matrix[0][1] = x.GetY();
	matrix[1][0] = y.GetX();
	matrix[1][1] = y.GetY();
}
Matrix2D::Matrix2D(const Matrix2D& rhs)
{
	matrix[0][0] = rhs.GetX1();
	matrix[0][1] = rhs.GetY1();
	matrix[1][0] = rhs.GetX2();
	matrix[1][1] = rhs.GetY2();
}
Matrix2D::Matrix2D(float degrees)
{
	matrix[0][0] = cos(degrees);
	matrix[0][1] = -sin(degrees);
	matrix[1][0] = sin(degrees);
	matrix[1][1] = cos(degrees);
}

Matrix2D::~Matrix2D()
{
}

//Functions
void Matrix2D::Set(float x1, float y1, float x2, float y2)
{
	matrix[0][0] = x1;
	matrix[0][1] = y1;
	matrix[1][0] = x2;
	matrix[1][1] = y2;
}
void Matrix2D::Set(Vector2D x, Vector2D y)
{
	matrix[0][0] = x.GetX();
	matrix[0][1] = x.GetY();
	matrix[1][0] = y.GetX();
	matrix[1][1] = y.GetY();
}
void Matrix2D::Set(float degrees)
{
	matrix[0][0] = cos(degrees);
	matrix[0][1] = -sin(degrees);
	matrix[1][0] = sin(degrees);
	matrix[1][1] = cos(degrees);
}
void Matrix2D::Rot(float degrees)
{
	matrix[0][0] = matrix[0][0] * cos(degrees);
	matrix[0][1] = matrix[0][1] * -sin(degrees);
	matrix[1][0] = matrix[1][0] * sin(degrees);
	matrix[1][1] = matrix[1][1] * cos(degrees);
}
void Matrix2D::Transponat()
{
	Matrix2D temp;
	temp = *this;
	this->matrix[0][1] = temp.matrix[1][0];
	this->matrix[1][0] = temp.matrix[0][1];
}
//void Matrix2D::Get and Print is Inline Functions in MathLib.h

//Operatorer
void Matrix2D::operator=(Matrix2D& rhs)
{
	this->Set(rhs.GetX1(), rhs.GetY1(), rhs.GetX2(), rhs.GetY2());
}
void Matrix2D::operator*(float rhs)
{
	this->matrix[0][0] = this->GetX1() * rhs;
	this->matrix[0][1] = this->GetY1() * rhs;
	this->matrix[1][0] = this->GetX2() * rhs;
	this->matrix[1][1] = this->GetY2() * rhs;
}
Vector2D Matrix2D::operator*(Vector2D rhs)
{
	Vector2D temp(this->GetX1() * rhs.GetX() + this->GetY1() * rhs.GetY(),  //<- Xcord, 
		this->GetX2() * rhs.GetX() + this->GetY2() * rhs.GetY()); //<- Ycord
	return temp;
}
Matrix2D Matrix2D::operator*(Matrix2D rhs)
{
	Matrix2D temp(((this->matrix[0][0] * rhs.matrix[0][0]) + (this->matrix[0][1] * rhs.matrix[1][0])),
		((this->matrix[0][0] * rhs.matrix[0][1]) + (this->matrix[0][1] * rhs.matrix[1][1])), /* */ ((this->matrix[1][0] * rhs.matrix[0][0]) + (this->matrix[1][1] * rhs.matrix[1][0])), /* */ ((this->matrix[1][0] * rhs.matrix[0][1]) + (this->matrix[1][1] * rhs.matrix[1][1])));
	return temp;
}
#pragma endregion

#pragma region Matrix3D
//Konstructor
Matrix3D::Matrix3D()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; i < 3; i++)
			matrix[i][j] = 0.0f;
	matrix[0][0] = 1.0f;
	matrix[1][1] = 1.0f;
	matrix[2][2] = 1.0f;
}
Matrix3D::Matrix3D(float x1, float y1, float x2, float y2)
{
	matrix[0][0] = x1;
	matrix[0][1] = y1;
	matrix[1][0] = x2;
	matrix[1][1] = y2;
	matrix[0][2] = 0.0f;
	matrix[1][2] = 0.0f;
	matrix[2][0] = 0.0f;
	matrix[2][1] = 0.0f;
	matrix[2][2] = 1.0f;
}
Matrix3D::Matrix3D(Vector2D x, Vector2D y)
{
	matrix[0][0] = x.GetX();
	matrix[0][1] = x.GetY();
	matrix[1][0] = y.GetX();
	matrix[1][1] = y.GetY();
	matrix[0][2] = 0.0f;
	matrix[1][2] = 0.0f;
	matrix[2][0] = 0.0f;
	matrix[2][1] = 0.0f;
	matrix[2][2] = 1.0f;
}
Matrix3D::Matrix3D(const Matrix2D& rhs)
{
	matrix[0][0] = rhs.GetX1();
	matrix[0][1] = rhs.GetY1();
	matrix[1][0] = rhs.GetX2();
	matrix[1][1] = rhs.GetY2();
	matrix[0][2] = 0.0f;
	matrix[1][2] = 0.0f;
	matrix[2][0] = 0.0f;
	matrix[2][1] = 0.0f;
	matrix[2][2] = 1.0f;
}
Matrix3D::Matrix3D(float degrees)
{
	matrix[0][0] = cos(degrees);
	matrix[0][1] = -sin(degrees);
	matrix[1][0] = sin(degrees);
	matrix[1][1] = cos(degrees);
	matrix[0][2] = 0.0f;
	matrix[1][2] = 0.0f;
	matrix[2][0] = 0.0f;
	matrix[2][1] = 0.0f;
	matrix[2][2] = 1.0f;
}

Matrix3D::~Matrix3D() {}

//Funktions
void Matrix3D::Set(float x1, float y1, float x2, float y2)
{
	matrix[0][0] = x1;
	matrix[0][1] = y1;
	matrix[1][0] = x2;
	matrix[1][1] = y2;
}
void Matrix3D::Set(Vector2D x, Vector2D y)
{
	matrix[0][0] = x.GetX();
	matrix[0][1] = x.GetY();
	matrix[1][0] = y.GetX();
	matrix[1][1] = y.GetY();
}
void Matrix3D::Set(float degrees)
{
	matrix[0][0] = cos(degrees);
	matrix[0][1] = -sin(degrees);
	matrix[1][0] = sin(degrees);
	matrix[1][1] = cos(degrees);
	matrix[2][2] = degrees;
}
void Matrix3D::SetSize(float sizeX, float sizeY)
{
	matrix[2][0] = sizeX;
	matrix[2][1] = sizeY;
}
void Matrix3D::Rot(float degrees)
{
	matrix[0][0] = matrix[0][0] * cos(degrees);
	matrix[0][1] = matrix[0][1] * -sin(degrees);
	matrix[1][0] = matrix[1][0] * sin(degrees);
	matrix[1][1] = matrix[1][1] * cos(degrees);
}
void Matrix3D::Transponat()
{
	Matrix3D temp;
	temp = *this;
	this->matrix[0][1] = temp.matrix[1][0];
	this->matrix[1][0] = temp.matrix[0][1];
}
void Matrix3D::TranslationMatrix(Vector2D v)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; i < 2; i++)
			matrix[i][j] = 0.0f;
	matrix[0][0] = 1.0f;
	matrix[1][1] = 1.0f;
	matrix[2][2] = 1.0f;
	SetPosition(v);
}

void Matrix3D::SetPosition(Vector2D v)
{
	matrix[0][2] = v.GetX();
	matrix[1][2] = v.GetY();
}
Vector2D Matrix3D::GetPosition()
{
	Vector2D temp(this->matrix[0][2], this->matrix[1][2]);
	return temp;
}

//Operatorer
Vector2D Matrix3D::operator*(Vector2D& rhs)
{
	Vector2D temp(this->GetX1() * rhs.GetX() + this->GetY1() * rhs.GetY(),  //<- Xcord, 
		this->GetX2() * rhs.GetX() + this->GetY2() * rhs.GetY()); //<- Ycord
	return temp;
}

#pragma endregion

#pragma region Vector4D

///Konstructor 4 float
Vector4D::Vector4D(float x, float y, float z, float w)
{
	cord[0] = x; cord[1] = y; cord[2] = z; cord[3] = w;
}
///Dekonstructor
Vector4D::~Vector4D()
{

}
///Set 4 float
void Vector4D::Set(float x, float y, float z, float w)
{
	cord[0] = x; cord[1] = y; cord[2] = z; cord[3] = w;
}
///Set X-Value
void Vector4D::SetX(float x)
{
	cord[0] = x;
}
///Set Y-Value
void Vector4D::SetY(float y)
{
	cord[1] = y;
}
///Set Z-Value
void Vector4D::SetZ(float z)
{
	cord[2] = z;
}
///Set W-Value
void Vector4D::SetW(float w)
{
	cord[3] = w;
}

///Normalizer, gets a vector with length of only 1.
Vector4D Vector4D::Normalize()
{
	float length = 1.0f / this->Length();
	Vector4D temp((this->cord[0] * length), (this->cord[1] * length), (this->cord[2] * length), 1.0f);
	return temp;
}
/// IndexBasedMultiplication.
Vector4D Vector4D::IBMulti(Vector4D a, Vector4D b)
{
	return Vector4D((a[0] * b[0]), (a[1] * b[1]), (a[2] * b[2]), 1);
}
Vector4D Vector4D::cross(Vector4D lhs, Vector4D rhs)
{
	Vector4D temp((lhs.cord[1] * rhs.cord[2] - lhs.cord[2] * rhs.cord[1]), (lhs.cord[2] * rhs.cord[0] - lhs.cord[0] * rhs.cord[2]), (lhs.cord[0] * rhs.cord[1] - lhs.cord[1] * rhs.cord[0]), 1.0f);
	return temp;
}
///Gets Length of the vector
float Vector4D::Length()
{
	return sqrt((this->cord[0] * this->cord[0]) + (this->cord[1] * this->cord[1]) + (this->cord[2] * this->cord[2]));
}

///Scaling 
Vector4D Vector4D::operator*(float rhs)			//Scaling
{
	Vector4D temp;
	temp.cord[0] = this->cord[0] * rhs;
	temp.cord[1] = this->cord[1] * rhs;
	temp.cord[2] = this->cord[2] * rhs;
	return temp;
}
///Dot Multi
float Vector4D::operator*(Vector4D rhs)		//Dot Multi
{
	return ((this->cord[0] * rhs.cord[0]) + (this->cord[1] * rhs.cord[1]) + (this->cord[2] * rhs.cord[2]));
}
///Cross Multi
Vector4D Vector4D::operator&&(Vector4D rhs)	//Cross Multi  "&&"  may change
{
	Vector4D temp((this->cord[1] * rhs.cord[2] - this->cord[2] * rhs.cord[1]), (this->cord[2] * rhs.cord[0] - this->cord[0] * rhs.cord[2]), (this->cord[0] * rhs.cord[1] - this->cord[1] * rhs.cord[0]));
	return temp;
}
///Addition
Vector4D Vector4D::operator+(Vector4D& rhs)	//Addition
{
	Vector4D temp((this->cord[0] + rhs.cord[0]), (this->cord[1] + rhs.cord[1]), (this->cord[2] + rhs.cord[2]));
	return temp;
}
///Subtraction
Vector4D Vector4D::operator-(Vector4D& rhs)	//Subtraction
{
	Vector4D temp((this->cord[0] - rhs.cord[0]), (this->cord[1] - rhs.cord[1]), (this->cord[2] - rhs.cord[2]));
	return temp;
}
void Vector4D::operator+=(Vector4D rhs)
{
	this->cord[0] = this->cord[0] + rhs.cord[0];
	this->cord[1] = this->cord[1] + rhs.cord[1];
	this->cord[2] = this->cord[2] + rhs.cord[2];
}
void Vector4D::operator-=(Vector4D rhs)
{
	this->cord[0] = this->cord[0] - rhs.cord[0];
	this->cord[1] = this->cord[1] - rhs.cord[1];
	this->cord[2] = this->cord[2] - rhs.cord[2];
}
///Returns a refrence to the value of the x position in the Vector
float& Vector4D::operator[] (const int x)		//Set
{
	return cord[x];
}
///Returns a constant to the value of the x position in the Vector
float Vector4D::operator[] (const int x) const	//Get
{
	return cord[x];
}

#pragma endregion

#pragma region Matrix4D

//Matrix4D::Matrix4D()
//{
//	matrix[0][0] = 1; matrix[0][1] = 0; matrix[0][2] = 0; matrix[0][3] = 0;								// x1, y1, z1, w1
//	matrix[1][0] = 0; matrix[1][1] = 1; matrix[1][2] = 0; matrix[1][3] = 0;								// x2, y2, z2, w2
//	matrix[2][0] = 0; matrix[2][1] = 0; matrix[2][2] = 1; matrix[2][3] = 0;								// x3, y3, z3, w3
//	matrix[3][0] = 0; matrix[3][1] = 0; matrix[3][2] = 0; matrix[3][3] = 1;								// x4, y4, z4, w4
//	GetArr();
//}

Matrix4D::Matrix4D(float mat[16])
{
	arr[0] = mat[0];
	arr[1] = mat[1];
	arr[2] = mat[2];
	arr[3] = mat[3];
	arr[4] = mat[4];
	arr[5] = mat[5];
	arr[6] = mat[6];
	arr[7] = mat[7];
	arr[8] = mat[8];
	arr[9] = mat[9];
	arr[10] = mat[10];
	arr[11] = mat[11];
	arr[12] = mat[12];
	arr[13] = mat[13];
	arr[14] = mat[14];
	arr[15] = mat[15];

	MakeToMatrix();
}
///Konstructor 16 float
Matrix4D::Matrix4D(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3, float x4, float y4, float z4, float w4) // [0][1] first row second kolomn, [1][0] second row first kolomn.
{
	matrix[0][0] = x1; matrix[0][1] = y1; matrix[0][2] = z1; matrix[0][3] = w1;								// x1, y1, z1, w1
	matrix[1][0] = x2; matrix[1][1] = y2; matrix[1][2] = z2; matrix[1][3] = w2;								// x2, y2, z2, w2
	matrix[2][0] = x3; matrix[2][1] = y3; matrix[2][2] = z3; matrix[2][3] = w3;								// x3, y3, z3, w3
	matrix[3][0] = x4; matrix[3][1] = y4; matrix[3][2] = z4; matrix[3][3] = w4;								// x4, y4, z4, w4
}
///Konstructor 8 Vector2D
Matrix4D::Matrix4D(Vector2D a, Vector2D b, Vector2D c, Vector2D d, Vector2D e, Vector2D f, Vector2D g, Vector2D h)			// a, b, c, d: e, f, g, h;
{
	matrix[0][0] = a.GetX(); matrix[0][1] = b.GetX(); matrix[0][2] = c.GetX(); matrix[0][3] = d.GetX();		// aX, bX, cX, dX
	matrix[1][0] = a.GetY(); matrix[1][1] = b.GetY(); matrix[1][2] = c.GetY(); matrix[1][3] = d.GetY();		// aY, bY, cY, dY
	matrix[2][0] = e.GetX(); matrix[2][1] = f.GetX(); matrix[2][2] = g.GetX(); matrix[2][3] = h.GetX();		// eX, fX, gX, hX
	matrix[3][0] = e.GetY(); matrix[3][1] = f.GetY(); matrix[3][2] = g.GetY(); matrix[3][3] = h.GetY();		// eY, fY, gY, hY
}
///Konstructor 4 Matrix2D
Matrix4D::Matrix4D(const Matrix2D& a, const Matrix2D& b, const Matrix2D& c, const Matrix2D& d)				// a, b: c, d;
{
	matrix[0][0] = a.GetX1(); matrix[0][1] = a.GetY1(); matrix[0][2] = b.GetX1(); matrix[0][3] = b.GetY1();	// aX1, aY1, bX1, bY1
	matrix[1][0] = a.GetX2(); matrix[1][1] = b.GetY2(); matrix[1][2] = b.GetX2(); matrix[1][3] = b.GetY2();	// aX2, aY2, bX2, bY2
	matrix[2][0] = c.GetX1(); matrix[2][1] = c.GetY1(); matrix[2][2] = d.GetX1(); matrix[2][3] = d.GetY1();	// cX1, cY1, dX1, dY1
	matrix[3][0] = c.GetX2(); matrix[3][1] = c.GetY2(); matrix[3][2] = d.GetX2(); matrix[3][3] = d.GetY2();	// cX2, cY2, dX2, dY2
}
///Konstructor 1 Matrix3D
Matrix4D::Matrix4D(const Matrix3D& a)
{
	matrix[0][0] = a.GetX1(); matrix[0][1] = a.GetY1(); matrix[0][2] = a.GetZ1(); matrix[0][3] = 0;	// aX1, aY1, bX1, bY1
	matrix[1][0] = a.GetX2(); matrix[1][1] = a.GetY2(); matrix[1][2] = a.GetZ2(); matrix[1][3] = 0;	// aX2, aY2, bX2, bY2
	matrix[2][0] = a.GetX3(); matrix[2][1] = a.GetY3();	matrix[2][2] = a.GetZ3(); matrix[2][3] = 0;	// cX1, cY1, dX1, dY1
	matrix[3][0] = 0; matrix[3][1] = 0;	matrix[3][2] = 0; matrix[3][3] = 0;	// cX2, cY2, dX2, dY2
}
///Konstructor Rotation Matrix around Vector
Matrix4D::Matrix4D(Vector4D Vec, float degrees) //Rotation
{
	float Cos = cos(degrees);
	float Sin = sin(degrees);
	float x = Vec.GetX(), y = Vec.GetY(), z = Vec.GetZ();

	Matrix4D
	(
		x*x*(1 - Cos) + Cos, y*x*(1 - Cos) - z * Sin, z*x*(1 - Cos) + y * Sin, 0,
		x*y*(1 - Cos) + z * Sin, y*y*(1 - Cos) + Cos, z*y*(1 - Cos) - x * Sin, 0,
		x*z*(1 - Cos) - y * Sin, y*z*(1 - Cos) + x * Sin, z*z*(1 - Cos) + Cos, 0
	);
}
Matrix4D::Matrix4D(Vector4D R, Vector4D U, Vector4D D)
{
	Matrix4D
	(
		R.GetX(), R.GetY(), R.GetZ(), 0,
		U.GetX(), U.GetY(), U.GetZ(), 0,
		D.GetX(), D.GetY(), D.GetZ(), 0,
		0, 0, 0, 1
	);
	this->GetArr();
}
Matrix4D::Matrix4D(Vector4D P)
{
	Matrix4D
	(
		1, 0, 0, -P.GetX(),
		0, 1, 0, -P.GetY(),
		0, 0, 1, -P.GetZ(),
		0, 0, 0, 1
	);
	this->GetArr();
}
///Dekostructor
Matrix4D::~Matrix4D() {}

///Set 16 float
void Matrix4D::Set(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3, float x4, float y4, float z4, float w4) // [0][1] first row second kolomn, [1][0] second row first kolomn. sets zero on none value!!
{
	matrix[0][0] = x1; matrix[0][1] = y1; matrix[0][2] = z1; matrix[0][3] = w1;		// x1, y1, z1, w1
	matrix[1][0] = x2; matrix[1][1] = y2; matrix[1][2] = z2; matrix[1][3] = w2;		// x2, y2, z2, w2
	matrix[2][0] = x3; matrix[2][1] = y3; matrix[2][2] = z3; matrix[2][3] = w3;		// x3, y3, z3, w3
	matrix[3][0] = x4; matrix[3][1] = y4; matrix[3][2] = z4; matrix[3][3] = w4;		// x4, y4, z4, w4
}
///Set 8 Vector2D
void Matrix4D::Set(Vector2D a, Vector2D b, Vector2D c, Vector2D d, Vector2D e, Vector2D f, Vector2D g, Vector2D h)			// a, b, c, d: e, f, g, h;
{
	matrix[0][0] = a.GetX(); matrix[0][1] = b.GetX(); matrix[0][2] = c.GetX(); matrix[0][3] = d.GetX();		// aX, bX, cX, dX
	matrix[1][0] = a.GetY(); matrix[1][1] = b.GetY(); matrix[1][2] = c.GetY(); matrix[1][3] = d.GetY();		// aY, bY, cY, dY
	matrix[2][0] = e.GetX(); matrix[2][1] = f.GetX(); matrix[2][2] = g.GetX(); matrix[2][3] = h.GetX();		// eX, fX, gX, hX
	matrix[3][0] = e.GetY(); matrix[3][1] = f.GetY(); matrix[3][2] = g.GetY(); matrix[3][3] = h.GetY();		// eY, fY, gY, hY
}
///Set 4 Matrix2D
void Matrix4D::Set(const Matrix2D& a, const Matrix2D& b, const Matrix2D& c, const Matrix2D& d)				// a, b: c, d;
{

	matrix[0][0] = a.GetX1(); matrix[0][1] = a.GetY1(); matrix[0][2] = b.GetX1(); matrix[0][3] = b.GetY1();	// aX1, aY1, bX1, bY1
	matrix[1][0] = a.GetX2(); matrix[1][1] = b.GetY2(); matrix[1][2] = b.GetX2(); matrix[1][3] = b.GetY2();	// aX2, aY2, bX2, bY2
	matrix[2][0] = c.GetX1(); matrix[2][1] = c.GetY1(); matrix[2][2] = d.GetX1(); matrix[2][3] = d.GetY1();	// cX1, cY1, dX1, dY1
	matrix[3][0] = c.GetX2(); matrix[3][1] = c.GetY2(); matrix[3][2] = d.GetX2(); matrix[3][3] = d.GetY2();	// cX2, cY2, dX2, dY2
}
void Matrix4D::SetI()
{
	matrix[0][0] = 1; matrix[0][1] = 0; matrix[0][2] = 0; matrix[0][3] = 0;								// x1, y1, z1, w1
	matrix[1][0] = 0; matrix[1][1] = 1; matrix[1][2] = 0; matrix[1][3] = 0;								// x2, y2, z2, w2
	matrix[2][0] = 0; matrix[2][1] = 0; matrix[2][2] = 1; matrix[2][3] = 0;								// x3, y3, z3, w3
	matrix[3][0] = 0; matrix[3][1] = 0; matrix[3][2] = 0; matrix[3][3] = 1;								// x4, y4, z4, w4
	GetArr();
}



///Makes a Rotation Matrix i the X-axel
Matrix4D Matrix4D::RotX(float degrees)
{
	float c = cos(degrees), s = sin(degrees);

	return Matrix4D
	(
		1, 0, 0, 0,
		0, c, -s, 0,
		0, s, c, 0,
		0, 0, 0, 1
	);
}
///Makes a Rotation Matrix i the Y-axel
Matrix4D Matrix4D::RotY(float degrees)
{
	float c = cos(degrees), s = sin(degrees);

	return Matrix4D
	(
		c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1
	);

}
///Makes a Rotation Matrix i the Z-axel
Matrix4D Matrix4D::RotZ(float degrees)
{
	float c = cos(degrees), s = sin(degrees);

	return Matrix4D
	(
		c, -s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}
///Makes a Rotation Matrix around a Vector4D
Matrix4D Matrix4D::RotVec(Vector4D Vec, float degrees)
{
	float Cos = cos(degrees);
	float Sin = sin(degrees);
	float x = Vec.GetX(), y = Vec.GetY(), z = Vec.GetZ();

	return Matrix4D
	(
		x*x*(1 - Cos) + Cos, y*x*(1 - Cos) - z * Sin, z*x*(1 - Cos) + y * Sin, 0,
		x*y*(1 - Cos) + z * Sin, y*y*(1 - Cos) + Cos, z*y*(1 - Cos) - x * Sin, 0,
		x*z*(1 - Cos) - y * Sin, y*z*(1 - Cos) + x * Sin, z*z*(1 - Cos) + Cos, 0
	);
}

///Transposes the current Matrix, NOT A NEW ONE
void Matrix4D::TransposeThis()
{
	Matrix4D temp = Matrix4D();
	temp = *this;

	this->matrix[0][1] = temp.matrix[1][0]; // 1
	this->matrix[1][0] = temp.matrix[0][1]; // 2

	this->matrix[0][2] = temp.matrix[2][0]; // 3
	this->matrix[2][0] = temp.matrix[0][2]; // 4

	this->matrix[1][2] = temp.matrix[2][1]; // 5
	this->matrix[2][1] = temp.matrix[1][2]; // 6

	this->matrix[0][3] = temp.matrix[3][0]; // 7
	this->matrix[3][0] = temp.matrix[0][3]; // 8

	this->matrix[1][3] = temp.matrix[3][1]; // 9
	this->matrix[3][1] = temp.matrix[1][3]; // 10

	this->matrix[2][3] = temp.matrix[3][2]; // 11
	this->matrix[3][2] = temp.matrix[2][3]; // 12

	temp.~Matrix4D();
}

Matrix4D Matrix4D::Transpose(Matrix4D mat)
{
	float* arr = mat.GetPointer();
	float returnMatrix[16];
	returnMatrix[0] = arr[0];
	returnMatrix[4] = arr[1];
	returnMatrix[8] = arr[2];
	returnMatrix[12] = arr[3];

	returnMatrix[1] = arr[4];
	returnMatrix[5] = arr[5];
	returnMatrix[9] = arr[6];
	returnMatrix[13] = arr[7];

	returnMatrix[2] = arr[8];
	returnMatrix[6] = arr[9];
	returnMatrix[10] = arr[10];
	returnMatrix[14] = arr[11];
	
	returnMatrix[3] = arr[12];
	returnMatrix[7] = arr[13];
	returnMatrix[11] = arr[14];
	returnMatrix[15] = arr[15];

	return Matrix4D(returnMatrix);
}
///inverses the current Matrix, NOT A NEW ONE
void Matrix4D::InversThis()
{
	GetArr();
	float inv[16], det;
	int i;

	inv[0] = arr[5] * arr[10] * arr[15] -
		arr[5] * arr[11] * arr[14] -
		arr[9] * arr[6] * arr[15] +
		arr[9] * arr[7] * arr[14] +
		arr[13] * arr[6] * arr[11] -
		arr[13] * arr[7] * arr[10];

	inv[4] = -arr[4] * arr[10] * arr[15] +
		arr[4] * arr[11] * arr[14] +
		arr[8] * arr[6] * arr[15] -
		arr[8] * arr[7] * arr[14] -
		arr[12] * arr[6] * arr[11] +
		arr[12] * arr[7] * arr[10];

	inv[8] = arr[4] * arr[9] * arr[15] -
		arr[4] * arr[11] * arr[13] -
		arr[8] * arr[5] * arr[15] +
		arr[8] * arr[7] * arr[13] +
		arr[12] * arr[5] * arr[11] -
		arr[12] * arr[7] * arr[9];

	inv[12] = -arr[4] * arr[9] * arr[14] +
		arr[4] * arr[10] * arr[13] +
		arr[8] * arr[5] * arr[14] -
		arr[8] * arr[6] * arr[13] -
		arr[12] * arr[5] * arr[10] +
		arr[12] * arr[6] * arr[9];

	inv[1] = -arr[1] * arr[10] * arr[15] +
		arr[1] * arr[11] * arr[14] +
		arr[9] * arr[2] * arr[15] -
		arr[9] * arr[3] * arr[14] -
		arr[13] * arr[2] * arr[11] +
		arr[13] * arr[3] * arr[10];

	inv[5] = arr[0] * arr[10] * arr[15] -
		arr[0] * arr[11] * arr[14] -
		arr[8] * arr[2] * arr[15] +
		arr[8] * arr[3] * arr[14] +
		arr[12] * arr[2] * arr[11] -
		arr[12] * arr[3] * arr[10];

	inv[9] = -arr[0] * arr[9] * arr[15] +
		arr[0] * arr[11] * arr[13] +
		arr[8] * arr[1] * arr[15] -
		arr[8] * arr[3] * arr[13] -
		arr[12] * arr[1] * arr[11] +
		arr[12] * arr[3] * arr[9];

	inv[13] = arr[0] * arr[9] * arr[14] -
		arr[0] * arr[10] * arr[13] -
		arr[8] * arr[1] * arr[14] +
		arr[8] * arr[2] * arr[13] +
		arr[12] * arr[1] * arr[10] -
		arr[12] * arr[2] * arr[9];

	inv[2] = arr[1] * arr[6] * arr[15] -
		arr[1] * arr[7] * arr[14] -
		arr[5] * arr[2] * arr[15] +
		arr[5] * arr[3] * arr[14] +
		arr[13] * arr[2] * arr[7] -
		arr[13] * arr[3] * arr[6];

	inv[6] = -arr[0] * arr[6] * arr[15] +
		arr[0] * arr[7] * arr[14] +
		arr[4] * arr[2] * arr[15] -
		arr[4] * arr[3] * arr[14] -
		arr[12] * arr[2] * arr[7] +
		arr[12] * arr[3] * arr[6];

	inv[10] = arr[0] * arr[5] * arr[15] -
		arr[0] * arr[7] * arr[13] -
		arr[4] * arr[1] * arr[15] +
		arr[4] * arr[3] * arr[13] +
		arr[12] * arr[1] * arr[7] -
		arr[12] * arr[3] * arr[5];

	inv[14] = -arr[0] * arr[5] * arr[14] +
		arr[0] * arr[6] * arr[13] +
		arr[4] * arr[1] * arr[14] -
		arr[4] * arr[2] * arr[13] -
		arr[12] * arr[1] * arr[6] +
		arr[12] * arr[2] * arr[5];

	inv[3] = -arr[1] * arr[6] * arr[11] +
		arr[1] * arr[7] * arr[10] +
		arr[5] * arr[2] * arr[11] -
		arr[5] * arr[3] * arr[10] -
		arr[9] * arr[2] * arr[7] +
		arr[9] * arr[3] * arr[6];

	inv[7] = arr[0] * arr[6] * arr[11] -
		arr[0] * arr[7] * arr[10] -
		arr[4] * arr[2] * arr[11] +
		arr[4] * arr[3] * arr[10] +
		arr[8] * arr[2] * arr[7] -
		arr[8] * arr[3] * arr[6];

	inv[11] = -arr[0] * arr[5] * arr[11] +
		arr[0] * arr[7] * arr[9] +
		arr[4] * arr[1] * arr[11] -
		arr[4] * arr[3] * arr[9] -
		arr[8] * arr[1] * arr[7] +
		arr[8] * arr[3] * arr[5];

	inv[15] = arr[0] * arr[5] * arr[10] -
		arr[0] * arr[6] * arr[9] -
		arr[4] * arr[1] * arr[10] +
		arr[4] * arr[2] * arr[9] +
		arr[8] * arr[1] * arr[6] -
		arr[8] * arr[2] * arr[5];

	det = arr[0] * inv[0] + arr[1] * inv[4] + arr[2] * inv[8] + arr[3] * inv[12];

	if (det == 0)
	{
		printf("Invalid invers: identity returned\n");
		Set(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		return;
	}

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		arr[i] = inv[i] * det;
	MakeToMatrix();
}
///Prints a easy readout
void Matrix4D::Print()
{
	printf(" | %f : %f : %f : %f | \n | %f : %f : %f : %f | \n | %f : %f : %f : %f | \n | %f : %f : %f : %f | \n",
		this->matrix[0][0], this->matrix[0][1], this->matrix[0][2], this->matrix[0][3],
		this->matrix[1][0], this->matrix[1][1], this->matrix[1][2], this->matrix[1][3],
		this->matrix[2][0], this->matrix[2][1], this->matrix[2][2], this->matrix[2][3],
		this->matrix[3][0], this->matrix[3][1], this->matrix[3][2], this->matrix[3][3]
	);
}
///Converts to a arr[16]
void Matrix4D::GetArr()
{
	int index = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			arr[index] = this->matrix[i][j];
			index++;
		}
}
float* Matrix4D::GetPointer()
{
	GetArr();
	return arr;
}

Matrix4D Matrix4D::GetPositionMatrix(Vector4D vec)
{
	Matrix4D temp(1, 0, 0, vec.GetX(), 0, 1, 0, vec.GetY(), 0, 0, 1, vec.GetZ(), 0, 0, 0, 1);
	return temp;
}
Matrix4D Matrix4D::GetScaleMatrix(Vector4D vec)
{
	return Matrix4D(
		vec[0], 0, 0, 0,
		0, vec[1], 0, 0,
		0, 0, vec[2], 0,
		0, 0,	0	, 1);
}
Matrix4D Matrix4D::GetRotationFromQuaternian(Vector4D vec)
{
	float xx = vec[0] * vec[0];
	float xy = vec[0] * vec[1];
	float xz = vec[0] * vec[2];
	float xw = vec[0] * vec[3];

	float yy = vec[1] * vec[1];
	float yz = vec[1] * vec[2];
	float yw = vec[1] * vec[3];

	float zz = vec[2] * vec[2];
	float zw = vec[2] * vec[3];

	return Matrix4D(
		1 - 2 * (yy + zz), 2 * (xy - zw), 2 * (xz + yw), 0,
		2 * (xy + zw), 1 - 2 * (xx + zz), 2 * (yz - xw), 0,
		2 * (xz - yw), 2 * (yz + xw), 1 - 2 * (xx + yy), 0,
			0		 ,		0		,		   0	   , 1);
}

///Converts arr to matrix
void Matrix4D::MakeToMatrix()
{
	this->Set(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7], arr[8], arr[9], arr[10], arr[11], arr[12], arr[13], arr[14], arr[15]);
}

Matrix4D Matrix4D::lookAt(Vector4D cameraPosition, Vector4D cameraTarget, Vector4D cameraUp)
{
	Vector4D f = (cameraTarget - cameraPosition).Normalize();
	Vector4D u = cameraUp.Normalize();
	Vector4D s = Vector4D::cross(f, u).Normalize();
	u = Vector4D::cross(s, f);

	Matrix4D Result;
	Result.matrix[0][0] = s.GetX();
	Result.matrix[1][0] = s.GetY();
	Result.matrix[2][0] = s.GetZ();
	Result.matrix[0][1] = u.GetX();
	Result.matrix[1][1] = u.GetY();
	Result.matrix[2][1] = u.GetZ();
	Result.matrix[0][2] = -f.GetX();
	Result.matrix[1][2] = -f.GetY();
	Result.matrix[2][2] = -f.GetZ();
	Result.matrix[3][0] = -(s * cameraPosition);
	Result.matrix[3][1] = -(u * cameraPosition);
	Result.matrix[3][2] =  (f * cameraPosition);
	Result.TransposeThis();
	return Result;
}

Matrix4D Matrix4D::PerspectiveProjection(float FOV, float aspect, float near, float far)
{
	return Matrix4D(
		1.0f / (aspect * tan(FOV / 2)), 0, 0, 0,
		0, 1.0f / (tan(FOV / 2)), 0, 0,
		0, 0, ((-near - far) / (near - far)), -1,
		0, 0, ((2 * far * near) / (far - near)), 0
	);
}


///Divide by float
Matrix4D Matrix4D::operator/(float rhs)
{
	Matrix4D Temp(
		this->matrix[0][0] / rhs, this->matrix[0][1] / rhs, this->matrix[0][2] / rhs, this->matrix[0][3] / rhs,
		this->matrix[1][0] / rhs, this->matrix[1][1] / rhs, this->matrix[1][2] / rhs, this->matrix[1][3] / rhs,
		this->matrix[2][0] / rhs, this->matrix[2][1] / rhs, this->matrix[2][2] / rhs, this->matrix[2][3] / rhs,
		this->matrix[3][0] / rhs, this->matrix[3][1] / rhs, this->matrix[3][2] / rhs, this->matrix[3][3] / rhs);
	return Temp;
}
///multiply by float
Matrix4D Matrix4D::operator*(float rhs)
{
	Matrix4D Temp(
		this->matrix[0][0] * rhs, this->matrix[0][1] * rhs, this->matrix[0][2] * rhs, this->matrix[0][3] * rhs,
		this->matrix[1][0] * rhs, this->matrix[1][1] * rhs, this->matrix[1][2] * rhs, this->matrix[1][3] * rhs,
		this->matrix[2][0] * rhs, this->matrix[2][1] * rhs, this->matrix[2][2] * rhs, this->matrix[2][3] * rhs,
		this->matrix[3][0] * rhs, this->matrix[3][1] * rhs, this->matrix[3][2] * rhs, this->matrix[3][3] * rhs);
	return Temp;
}
///multiply by Vector4D
Vector4D Matrix4D::operator*(Vector4D rhs)
{
	return Vector4D
	(
		this->matrix[0][0] * rhs.GetX() + this->matrix[0][1] * rhs.GetY() + this->matrix[0][2] * rhs.GetZ(),
		this->matrix[1][0] * rhs.GetX() + this->matrix[1][1] * rhs.GetY() + this->matrix[1][2] * rhs.GetZ(),
		this->matrix[2][0] * rhs.GetX() + this->matrix[2][1] * rhs.GetY() + this->matrix[2][2] * rhs.GetZ(),
		this->matrix[3][0] * rhs.GetX() + this->matrix[3][1] * rhs.GetY() + this->matrix[3][2] * rhs.GetZ()
	);
}
///multiply by Matrix4D
Matrix4D Matrix4D::operator*(Matrix4D rhs)
{
	return Matrix4D
	(
		/* x1 */ this->matrix[0][0] * rhs.matrix[0][0] + this->matrix[0][1] * rhs.matrix[1][0] + this->matrix[0][2] * rhs.matrix[2][0] + this->matrix[0][3] * rhs.matrix[3][0],
		/* y1 */ this->matrix[0][0] * rhs.matrix[0][1] + this->matrix[0][1] * rhs.matrix[1][1] + this->matrix[0][2] * rhs.matrix[2][1] + this->matrix[0][3] * rhs.matrix[3][1], 
		/* z1 */ this->matrix[0][0] * rhs.matrix[0][2] + this->matrix[0][1] * rhs.matrix[1][2] + this->matrix[0][2] * rhs.matrix[2][2] + this->matrix[0][3] * rhs.matrix[3][2],
		/* w1 */ this->matrix[0][0] * rhs.matrix[0][3] + this->matrix[0][1] * rhs.matrix[1][3] + this->matrix[0][2] * rhs.matrix[2][3] + this->matrix[0][3] * rhs.matrix[3][3],
		
		/* x2 */ this->matrix[1][0] * rhs.matrix[0][0] + this->matrix[1][1] * rhs.matrix[1][0] + this->matrix[1][2] * rhs.matrix[2][0] + this->matrix[1][3] * rhs.matrix[3][0],
		/* y2 */ this->matrix[1][0] * rhs.matrix[0][1] + this->matrix[1][1] * rhs.matrix[1][1] + this->matrix[1][2] * rhs.matrix[2][1] + this->matrix[1][3] * rhs.matrix[3][1], 
		/* z2 */ this->matrix[1][0] * rhs.matrix[0][2] + this->matrix[1][1] * rhs.matrix[1][2] + this->matrix[1][2] * rhs.matrix[2][2] + this->matrix[1][3] * rhs.matrix[3][2], 
		/* w2 */ this->matrix[1][0] * rhs.matrix[0][3] + this->matrix[1][1] * rhs.matrix[1][3] + this->matrix[1][2] * rhs.matrix[2][3] + this->matrix[1][3] * rhs.matrix[3][3],
		
		/* x3 */ this->matrix[2][0] * rhs.matrix[0][0] + this->matrix[2][1] * rhs.matrix[1][0] + this->matrix[2][2] * rhs.matrix[2][0] + this->matrix[2][3] * rhs.matrix[3][0],
		/* y3 */ this->matrix[2][0] * rhs.matrix[0][1] + this->matrix[2][1] * rhs.matrix[1][1] + this->matrix[2][2] * rhs.matrix[2][1] + this->matrix[2][3] * rhs.matrix[3][1],
		/* z3 */ this->matrix[2][0] * rhs.matrix[0][2] + this->matrix[2][1] * rhs.matrix[1][2] + this->matrix[2][2] * rhs.matrix[2][2] + this->matrix[2][3] * rhs.matrix[3][2], 
		/* w3 */ this->matrix[2][0] * rhs.matrix[0][3] + this->matrix[2][1] * rhs.matrix[1][3] + this->matrix[2][2] * rhs.matrix[2][3] + this->matrix[2][3] * rhs.matrix[3][3],
		
		/* x4 */ this->matrix[3][0] * rhs.matrix[0][0] + this->matrix[3][1] * rhs.matrix[1][0] + this->matrix[3][2] * rhs.matrix[2][0] + this->matrix[3][3] * rhs.matrix[3][0],
		/* y4 */ this->matrix[3][0] * rhs.matrix[0][1] + this->matrix[3][1] * rhs.matrix[1][1] + this->matrix[3][2] * rhs.matrix[2][1] + this->matrix[3][3] * rhs.matrix[3][1], 
		/* z4 */ this->matrix[3][0] * rhs.matrix[0][2] + this->matrix[3][1] * rhs.matrix[1][2] + this->matrix[3][2] * rhs.matrix[2][2] + this->matrix[3][3] * rhs.matrix[3][2], 
		/* w4 */ this->matrix[3][0] * rhs.matrix[0][3] + this->matrix[3][1] * rhs.matrix[1][3] + this->matrix[3][2] * rhs.matrix[2][3] + this->matrix[3][3] * rhs.matrix[3][3]
	);
}
void Matrix4D::operator+=(Matrix4D rhs)
{
	Matrix4D Temp(
		this->matrix[0][0] + rhs.matrix[0][0], this->matrix[0][1] + rhs.matrix[0][1], this->matrix[0][2] + rhs.matrix[0][2], this->matrix[0][3] + rhs.matrix[0][3],
		this->matrix[1][0] + rhs.matrix[1][0], this->matrix[1][1] + rhs.matrix[1][1], this->matrix[1][2] + rhs.matrix[1][2], this->matrix[1][3] + rhs.matrix[1][3],
		this->matrix[2][0] + rhs.matrix[2][0], this->matrix[2][1] + rhs.matrix[2][1], this->matrix[2][2] + rhs.matrix[2][2], this->matrix[2][3] + rhs.matrix[2][3],
		this->matrix[3][0] + rhs.matrix[3][0], this->matrix[3][1] + rhs.matrix[3][1], this->matrix[3][2] + rhs.matrix[3][2], this->matrix[3][3]);
	
}
#pragma endregion