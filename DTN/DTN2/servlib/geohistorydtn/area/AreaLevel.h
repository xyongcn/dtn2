

class AreaLevel {

public :
	//osm中区域层数为12
//	static const int ZEROLEVEL=0;
	static const int FIRSTLEVEL=1;
	static const int SECONDLEVEL=2;
	static const int THIRDLEVEL=3;
	static const int FOURLEVEL=4;
	static const int FIVELEVEL=5;
	static const int SIXLEVEL=6;
	static const int SEVENLEVEL=7;
	static const int EIGHTLEVEL=8;
	static const int NINELEVEL=9;
	static const int TENLEVEL=10;
	static const int ELEVENLEVEL=11;
	static const int TWELVELEVEL=12;

	//获取相匹配的level
	static bool isLevelValid(int l)
	{
		for(int i=MINLEVEL;i<=MAXLEVEL;i++)
		{
			if(i==l)
				return true;
		}
		return false;
	}

	static int nextLevel(int l)
	{
		for(int i=MINLEVEL;i<=MAXLEVEL;i++)
		{
			if(i==l)
			{
				if(i<MAXLEVEL)
				{
					return (i+1);
				}
				else
					break;
			}
		}
		return -1;
	}
	const static int CurrentAreaLevelNum=3;
	//最顶层
	const static int MINLEVEL=FIRSTLEVEL;
	//最底层
	 const static int MAXLEVEL=TWELVELEVEL;
};
