class Vector2;
class Vector3;
class Vector4;
class Matrix4;

 
class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);

	float & x();
	float & y();

	float x() const;
	float y() const;

	const float * elements() const;

	Vector2 operator+(const Vector2 & v) const;
	Vector2 operator-(const Vector2 & v) const;
	Vector2 operator-() const;
	float operator*(const Vector2 & v) const;
	float operator^(const Vector2 & v) const;
	Vector2 operator*(float s) const;
	Vector2 operator/(float s) const;
	
	Vector2 & operator+=(const Vector2 & v);
	Vector2 & operator-=(const Vector2 & v);
	Vector2 & operator*=(float s);
	Vector2 & operator/=(float s);

	float length() const;
	Vector2 normalize() const;

	Vector2 rotate(float rad) const;

private:
	float e[2];
};


class Vector3
{
friend class Matrix4;
public:
	Vector3();
	Vector3(float x, float y, float z);

	float & x();
	float & y();
	float & z();

	float x() const;
	float y() const;
	float z() const;

	const float * elements() const;

	Vector3 operator+(const Vector3 & v) const;
	Vector3 operator-(const Vector3 & v) const;
	Vector3 operator-() const;
	float operator*(const Vector3 & v) const;
	Vector3 operator^(const Vector3 & v) const;
	Vector3 operator*(float s) const;
	Vector3 operator/(float s) const;
	
	Vector3 & operator+=(const Vector3 & v);
	Vector3 & operator-=(const Vector3 & v);
	Vector3 & operator*=(float s);
	Vector3 & operator/=(float s);

	float length() const;
	Vector3 normalize() const;

	Vector3 rotateX(float rad) const;
	Vector3 rotateY(float rad) const;
	Vector3 rotateZ(float rad) const;
	Vector3 rotate(const Vector3 & eulerxyz) const;

private:
	float e[3];
};


class Vector4
{
friend class Matrix4;
public:
	Vector4();
	Vector4(float x, float y);
	Vector4(float x, float y, float z);
	Vector4(float x, float y, float z, float w);

	float & x();
	float & y();
	float & z();
	float & w();

	float x() const;
	float y() const;
	float z() const;
	float w() const;

	const float * elements() const;

	Vector4 operator+(const Vector4 & v) const;
	Vector4 operator-(const Vector4 & v) const;
	Vector4 operator-() const;
	float operator*(const Vector4 & v) const;
	Vector4 operator^(const Vector4 & v) const;
	Vector4 operator*(float s) const;
	Vector4 operator/(float s) const;
	
	Vector4 & operator+=(const Vector4 & v);
	Vector4 & operator-=(const Vector4 & v);
	Vector4 & operator*=(float s);
	Vector4 & operator/=(float s);

	float length() const;
	Vector4 normalize() const;

	Vector4 rotateX(float rad) const;
	Vector4 rotateY(float rad) const;
	Vector4 rotateZ(float rad) const;
	Vector4 rotate(const Vector4 & rot) const;

private:
	float e[4];
};


class Matrix4
{
public:
	Matrix4();

	const float * elements() const;

	Matrix4 operator*(const Matrix4 & m);
	
	Vector4 operator*(const Vector4 & v);

	static Matrix4 translationMatrix(const Vector3 & v);
	
	static Matrix4 lookAtMatrix(const Vector3 & position, const Vector3 & target, const Vector3 & up);

	static Matrix4 perspectiveMatrix(float fovy, float aspect, float znear, float zfar);

private:
	float e[16];
};
