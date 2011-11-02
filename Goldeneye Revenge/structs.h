//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	structs.h - This file will handle all the structures for
//	the program such as vectors
//	
//////////////////////////////////////////////////////////////

struct Size
{
	public:
		float width;
		float height;
		float depth;

		Size()
		{
			//	Default constructor
			width = 0;
			height = 0;
			depth = 0;
		}

		Size(float _width, float _height, float _depth = 0)
		{
			width = _width;
			height = _height;
			depth = _depth;
		}

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator + so when you add a 
		//	size to another size, it will use this method.
		//
		//////////////////////////////////////////////////////////////
		Size operator +(Size size)
		{
			return Size(size.width + width, size.height + height, size.depth + depth);
		}

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator - so when you subtract a 
		//	size to another size, it will use this method.
		//
		//////////////////////////////////////////////////////////////
		Size operator -(Size size)
		{
			return Size(size.width - width, size.height - height, size.depth - depth);
		}

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator * so when you multiply a 
		//	size to a number, it will use this method.
		//
		//////////////////////////////////////////////////////////////
		Size operator *(float num)
		{
			return Size(num * width, num * height, depth * num);
		}

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator / so when you divide a 
		//	size from a number, it will use this method.
		//
		//////////////////////////////////////////////////////////////
		Size operator /(float num)
		{
			return Size(width / num, height / num, depth / num);
		}

		bool operator ==(Size a)
		{
			if (a.width == width && a.height == height)
			{
				return true;
			}

			return false;
		}
};

//////////////////////////////////////////////////////////////
//
//	This matrix stores a 1x4 matrix array
//
//////////////////////////////////////////////////////////////
struct Matrix1
{
	public:
		float c0, c1, c2, c3;

		Matrix1()
		{
			c0 = 0;
			c1 = 0;
			c2 = 0;
			c3 = 0;
		}

		Matrix1(float _c0, float _c1, float _c2, float _c3)
		{
			c0 = _c0;
			c1 = _c1;
			c2 = _c2;
			c3 = _c3;
		}
};

//////////////////////////////////////////////////////////////
//
//	This matrix stores a 4x4 matrix array
//
//////////////////////////////////////////////////////////////
struct Matrix4
{
	public:
		float mat[4][4];

		Matrix4()
		{
			for (int x = 0; x < 4; x++)
			{
				for (int y = 0; y < 4; y++)
				{
					mat[x][y] = 0;
				}
			}
		}

		Matrix4(Matrix1 r0)
		{
			Matrix4(r0, Matrix1(0, 0, 0, 0), Matrix1(0, 0, 0, 0), Matrix1(0, 0, 0, 0));
		}

		Matrix4(Matrix1 r0, Matrix1 r1)
		{
			Matrix4(r0, r1, Matrix1(0, 0, 0, 0), Matrix1(0, 0, 0, 0));
		}

		Matrix4(Matrix1 r0, Matrix1 r1, Matrix1 r2)
		{
			Matrix4(r0, r1, r2, Matrix1(0, 0, 0, 0));
		}

		Matrix4(float r0c0, float r0c1, float r0c2, float r0c3, 
				float r1c0, float r1c1, float r1c2, float r1c3, 
				float r2c0, float r2c1, float r2c2, float r2c3, 
				float r3c0, float r3c1, float r3c2, float r3c3)
		{
			mat[0][0] = r0c0;
			mat[0][1] = r0c1;
			mat[0][2] = r0c2;
			mat[0][3] = r0c3;

			mat[1][0] = r1c0;
			mat[1][1] = r1c1;
			mat[1][2] = r1c2;
			mat[1][3] = r1c3;

			mat[2][0] = r2c0;
			mat[2][1] = r2c1;
			mat[2][2] = r2c2;
			mat[2][3] = r2c3;

			mat[3][0] = r3c0;
			mat[3][1] = r3c1;
			mat[3][2] = r3c2;
			mat[3][3] = r3c3;
		}

		Matrix4(Matrix1 r1, Matrix1 r2, Matrix1 r3, Matrix1 r4)
		{
			mat[0][0] = r1.c0;
			mat[0][1] = r1.c1;
			mat[0][2] = r1.c2;
			mat[0][3] = r1.c3;

			mat[1][0] = r2.c0;
			mat[1][1] = r2.c1;
			mat[1][2] = r2.c2;
			mat[1][3] = r2.c3;

			mat[2][0] = r3.c0;
			mat[2][1] = r3.c1;
			mat[2][2] = r3.c2;
			mat[2][3] = r3.c3;

			mat[3][0] = r4.c0;
			mat[3][1] = r4.c1;
			mat[3][2] = r4.c2;
			mat[3][3] = r4.c3;
		}

		//	Convert from matrix4 to float array for GL
		operator GLfloat *()
		{
			static GLfloat m[16];

			int count = 0;
			for (int x = 0; x < 4; x++)
			{
				for (int y = 0; y < 4; y++)
				{
					m[count] = mat[x][y];
					count++;
				}
			}

			return m;
		}
};

//////////////////////////////////////////////////////////////
//
//	This vector stores the x and y co-ordinates
//
//////////////////////////////////////////////////////////////
struct Vector2
{
	public:
		
		float x, y;

		Vector2()
		{
			//	Default constructor
			x = 0;
			y = 0;
		}

		Vector2(float _x, float _y)
		{
			x = _x;
			y = _y;			
		}				

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator + so when you add a 
		//	vector to another vector, it will use this method.
		//
		//////////////////////////////////////////////////////////////
		Vector2 operator +(Vector2 vector)
		{
			return Vector2(vector.x + x, vector.y + y);
		}

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator - so when you subtract a 
		//	vector to another vector, it will use this method.
		//
		//////////////////////////////////////////////////////////////
		Vector2 operator -(Vector2 vector)
		{
			return Vector2(x - vector.x, y - vector.y);
		}

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator * so when you multiply a 
		//	vector to another vector, it will use this method.
		//
		//////////////////////////////////////////////////////////////
		Vector2 operator *(float num)
		{
			return Vector2(num * x, num * y);
		}

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator / so when you divide a 
		//	vector to another vector, it will use this method.
		//
		//////////////////////////////////////////////////////////////
		Vector2 operator /(float num)
		{
			return Vector2(x / num, y / num);
		}
};

//////////////////////////////////////////////////////////////
//
//	This is the vector structure which will be used in the 
//	camera class
//
//////////////////////////////////////////////////////////////
struct Vector3
{
	public:
		
		float x, y, z;

		Vector3()
		{
			//	Default constructor	
			x = 0;
			y = 0;
			z = 0;
		}

		Vector3(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}				

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator = so when you assign 
		//	a number to the vector, it sets everything to that
		//
		//////////////////////////////////////////////////////////////
		Vector3 operator =(int num)
		{
			return Vector3(num, num, num);
		}

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator + so when you add a 
		//	vector to another vector, it will use this method.
		//
		//////////////////////////////////////////////////////////////
		Vector3 operator +(Vector3 vector)
		{
			return Vector3(vector.x + x, vector.y + y, vector.z + z);
		}

		Vector3 operator +(int num)
		{
			return Vector3(num + x, num + y, num + z);
		}

		Vector3 operator +=(Vector3 vector)
		{
			return Vector3(x, y, z) + vector;
		}

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator - so when you subtract a 
		//	vector to another vector, it will use this method.
		//
		//////////////////////////////////////////////////////////////
		Vector3 operator -(Vector3 vector)
		{
			return Vector3(x - vector.x, y - vector.y, z - vector.z);
		}

		Vector3 operator -(int num)
		{
			return Vector3(x - num, y - num, z - num);
		}

		Vector3 operator -=(Vector3 vector)
		{
			return Vector3(x, y, z) - vector;
		}

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator * so when you multiply a 
		//	vector to another vector, it will use this method.
		//
		//////////////////////////////////////////////////////////////
		Vector3 operator *(float num)
		{
			return Vector3(num * x, num * y, num * z);
		}

		Vector3 operator *(Vector3 vector)
		{
			return Vector3(vector.x * x, vector.y * y, vector.z * z);
		}

		//////////////////////////////////////////////////////////////
		//
		//	This function handles the operator / so when you divide a 
		//	vector to another vector, it will use this method.
		//
		//////////////////////////////////////////////////////////////
		Vector3 operator /(float num)
		{
			return Vector3(x / num, y / num, z / num);
		}
};

//////////////////////////////////////////////////////////////
//
//	This vector is used for x, y, z and angles and will be used
//	in the camera class to store the rotation vector
//
//////////////////////////////////////////////////////////////
struct Vector4
{
	public:
		float x;
		float y;
		float z;
		float angle;

	Vector4()
	{
		x = 0;
		y = 0;
		z = 0;
		angle = 0;
	}

	Vector4(float _x, float _y, float _z, float _angle)
	{
		x = _x;
		y = _y;
		z = _z;
		angle = _angle;
	}

	//////////////////////////////////////////////////////////////
	//
	//	This function handles the operator = so when you assign 
	//	a number to the vector, it sets everything to that
	//
	//////////////////////////////////////////////////////////////
	Vector4 operator =(int num)
	{
		return Vector4(num, num, num, num);
	}

	//////////////////////////////////////////////////////////////
	//
	//	This function handles the operator + so when you add a 
	//	vector to another vector, it will use this method.
	//
	//////////////////////////////////////////////////////////////
	Vector4 operator +(Vector4 vector)
	{
		return Vector4(vector.x + x, vector.y + y, vector.z + z, vector.angle);
	}

	//////////////////////////////////////////////////////////////
	//
	//	This function handles the operator - so when you subtract a 
	//	vector to another vector, it will use this method.
	//
	//////////////////////////////////////////////////////////////
	Vector4 operator -(Vector4 vector)
	{
		return Vector4(x - vector.x, y - vector.y, z - vector.z, vector.angle);
	}

	//////////////////////////////////////////////////////////////
	//
	//	This function handles the operator * so when you multiply a 
	//	vector to another vector, it will use this method.
	//
	//////////////////////////////////////////////////////////////
	Vector4 operator *(float num)
	{
		return Vector4(num * x, num * y, num * z, angle * num);
	}

	//////////////////////////////////////////////////////////////
	//
	//	This function handles the operator / so when you divide a 
	//	vector to another vector, it will use this method.
	//
	//////////////////////////////////////////////////////////////
	Vector4 operator /(float num)
	{
		return Vector4(x / num, y / num, z / num, angle);
	}
};

//////////////////////////////////////////////////////////////
//
//	This is our position structure 
//
//////////////////////////////////////////////////////////////
struct Position
{
	float x;
	float y;
	float z;
	float speed;
};

//////////////////////////////////////////////////////////////
//
//	This is our person properties structure 
//
//////////////////////////////////////////////////////////////
struct PlayerInfo
{
	float health;
	float armour;
};

//////////////////////////////////////////////////////////////
//
//	This is our weapon info structure 
//
//////////////////////////////////////////////////////////////
struct WeaponInfo
{
	int totalAmmo;
	int currentAmmo;
	int maxAmmoPerClip;
	int fireMode;
	float damage;
};

//////////////////////////////////////////////////////////////
//
//	This is our setting structure 
//
//////////////////////////////////////////////////////////////
struct Setting
{	
	public:		
		string groupName;
		string name;
		string data;

		Setting(string _groupName, string _name, string _data)
		{
			groupName = _groupName;
			name = _name;
			data = _data;
		}

		Setting()	
		{
			groupName = "";
			name = "";
			data = "";
		}		
};

//////////////////////////////////////////////////////////////
//
//	This structure is going to be for our camera class to move
//	in a certain direction.
//
//////////////////////////////////////////////////////////////
struct Waypoint
{
	Vector3 position;
	Vector3 view;
	Vector3 up;
	float speed;

	Waypoint()
	{
		position = 0;
		view = 0;
		speed = 0;
		up = 0;
	}

	Waypoint(Vector3 _position, Vector3 _view, float _speed, Vector3 _up)
	{
		position = _position;
		view = _view;
		speed = _speed;		
		up  = _up;
	}
};

//////////////////////////////////////////////////////////////
//
//	This structure is for rects
//
//////////////////////////////////////////////////////////////
struct Rect
{
	public:
		float x1, y1, x2, y2;

		Rect()
		{
			x1 = y1 = x2 = y2 = 0;
		}

		Rect(float _x1, float _y1, float _x2, float _y2)
		{
			x1 = _x1;
			y1 = _y1;
			x2 = _x2;
			y2 = _y2;
		}
};

//////////////////////////////////////////////////////////////
//
//	This structure is for colour
//
//////////////////////////////////////////////////////////////
struct Colour
{
	int red, blue, green, alpha;

	Colour()
	{
		red = 0;
		blue = 0;
		green = 0;
		alpha = 0;
	}

	Colour(int r, int g, int b, int a = 255)
	{
		red = r;
		green = g;
		blue = b;
		alpha = a;
	}

	Colour(float r, float g, float b, float a = 1.0f)
	{
		red = (1 / 255) * r;
		green = (1 / 255) * g;
		blue = (1 / 255) * b;
		alpha = (1 / 255) * a;
	}

	//	Format 0x######
	Colour(int colour)
	{
		red = (colour >> 16) & 0xFF;
		green = (colour >> 8) & 0xFF;
		blue = (colour & 0x0000FF);	
		alpha = 255;
	}
};