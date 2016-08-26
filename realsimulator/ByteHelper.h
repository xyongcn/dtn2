class ByteHelper
{
public:
	static const bool endian_test()
	{
		int x = 1;
		char p = (char)x;
		if(p == 1)
			return false;//little endian
		else
			return true;//big endian

	}

	static const int byte_array_to_int(char* b)
	{
		int a= b[3] & 0xFF;

		a |= ((b[2] << 8) & 0xFF00);

		a|= ((b[1] << 16) & 0xFF0000);

		a |= ((b[0] << 24) & 0xFF000000);

		return a;

	}

	static const void int_to_byte_array(int value,char s[])
	{
		s[0]=value >> 24;
		s[1]=value >>16;
		s[2]=value >>8;
		s[3]=value;
	}
};
