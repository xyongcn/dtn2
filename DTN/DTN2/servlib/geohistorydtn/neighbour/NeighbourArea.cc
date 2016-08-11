#include"NeighbourArea.h"
#include "servlib/bundling/BundlePayload.h"
#include "servlib/bundling/Bundle.h"
#include"servlib/naming/EndpointID.h"

namespace dtn
{
	const string NeighbourArea::tag="NeighbourArea";


	void NeighbourArea::init()
	{
		string filename;
		string temp=neighbourEID.str();
		for(int i=0;i<temp.size();)
		{
			if(temp[i-1]=='/')
			{
				if(temp[i]=='/')
				{
					int pointnum=0;
					for(int j=i+1;j<temp.size();++j)
					{
						string t;
						stringstream stream;
						stream << temp[j];
						t= stream.str();
						if(!strcmp(t.c_str(),"."))
							pointnum++;
						if(pointnum==4)
							break;
						filename.append(t);

					}
					break;
				}
			}
			++i;
		}
		string directories;
		string file;
		directories.append(NeighbourConfig::NEIGHBOURAREAFILEDIR);
		file.append(directories);
		file.append(filename);
		fstream neighbourAreaDir;
		neighbourAreaDir.open(directories.c_str(),ios::in);
		//cout<<directories<<endl;
		//cout<<file<<endl;

		if (!neighbourAreaDir)
		{
			string Dir;
			const char* c="/";
			char *dir=new char[30];
			strcpy(dir,directories.c_str());
			char *Dirpart=strtok(dir,c);
			Dir.append(Dirpart);
			Dirpart=strtok(NULL,c);
			while(Dirpart!=NULL)
			{
				Dir.append("/");
				Dir.append(Dirpart);
				fstream fdir(Dir.c_str());
				if(!fdir)
				{
					if(mkdir(Dir.c_str(),S_IRWXU|S_IRWXG|S_IRWXO)==0)
						cout<<Dir<<" Folder creation success"<<endl;//文件夹创建成功
					else
						cout<<Dir<<" Folder creation fail"<<endl;//can not make a dir;}
				}

				Dirpart=strtok(NULL,c);
			}
			neighbourAreaDir.close();
			return;

		}
		neighbourAreaDir.close();

		fstream neighbourAreaFile;
		neighbourAreaFile.open(file.c_str(),ios::in);
		if(neighbourAreaFile)
		{
			neighbourAreaFile.close();
			geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"从文件historyarea中读取历史的区域信息");
			updateArea(file);
		}
		else
		{
			neighbourAreaFile.close();
			return;
		}

	}

	/**
	 * 利用收到邻居的区域信息的bundle来初始化本地的邻居历史区域信息
	 * 将payload保存到文件中，再更新NeighbourArea
	 * @param eid
	 * @param payload
	 */
	void NeighbourArea::Payload_update(string eid,const BundlePayload *payload)
	{
		if(!areaMap.empty())
			areaMap.clear();

		//将文件保存到本地

		string filename;
		string temp=neighbourEID.str();
		for(int i=0;i<temp.size();)
		{
			if(temp[i-1]=='/')
			{
				if(temp[i]=='/')
				{
					int pointnum=0;
					for(int j=i+1;j<temp.size();++j)
					{
						string t;
						stringstream stream;
						stream << temp[j];
						t= stream.str();
						if(!strcmp(t.c_str(),"."))
							pointnum++;
						if(pointnum==4)
							break;
						filename.append(t);

					}
					break;
				}
			}
			++i;
		}
		string directories;
		string file;
		directories.append(NeighbourConfig::NEIGHBOURAREAFILEDIR);
		file.append(directories);
		file.append(filename);

		fstream neighAreaFile;
		neighAreaFile.open(file.c_str(),ios::trunc|ios::out|ios::binary);//删除原来的记录，重新写


		//将邻居的区域记录保存到本地
		//	GeohistoryLog.i(tag, String.format("将邻居（ %s）发来的payload里面的区域移动规律存储到文件中", eid));
		//payload.copy_to_file(neighAreaFile);
		//将payload读到一个字符流，再将字符流读到一个文件
		int length=payload->length();
		u_char *buf=new u_char[length];
		payload->read_data(0,length,buf);

		neighAreaFile.write((char *)buf,length);
		neighAreaFile.close();
		updateArea(file);

	}

	/**
	 * 该邻居的区域向量记录
	 */
	EndpointID neighbourEID;
	string neighbourEIDstr;

	/**
	 * 用来存放所有的Area的hashmap
	 */
	hash_map<string,Area> areaMap;

	NeighbourArea::NeighbourArea(EndpointID eid)
	{
		neighbourEID=eid;
		init();
	}

	/**
	 * 利用邻居区域移动频率的文件来更新频率文件
	 * @param payloadFile
	 * @throws IOException
	 * @throws FileNotFoundException
	 * @throws StreamCorruptedException
	 * @throws ClassNotFoundException
	 */
	void NeighbourArea::updateArea(string fileroute)
	{
		fstream payloadFile(fileroute.c_str());
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s,利用payloadFile来更新内存中邻居的区域移动规律",tag.c_str());
		//payloadFile.open(historyAreaFilePath.c_str(),ios::in);
		//从文件中读取历史的区域信息
		if (!payloadFile.is_open())
		{
			cout<<fileroute<<" Error opening file";
			return;
		}
		boost::archive::text_iarchive ia(payloadFile);
		Area area;
		try
		{
			while(!payloadFile.eof())
			{

				//将读取到的area保存起来
				ia >>area;
				char c[20];
				string s;
				sprintf(c,"%d",area.level);
				s.append(c);
				s.append("#");
				sprintf(c,"%d",area.id);
				s.append(c);
				areaMap[s]= area;
			}

		}
		catch(boost::archive::archive_exception &e)
		{}
		payloadFile.close();
	}


	/**
	 * 利用payload里面的缓存来更新邻居的移动规律
	 * @param buffer
	 * @throws StreamCorruptedException
	 * @throws FileNotFoundException
	 * @throws IOException
	 * @throws ClassNotFoundException
	 */
	////////////////////////暂时弃用
	/*void updateArea(u_char *buffer)
	{

		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s,利用payloadFile来更新内存中邻居的区域移动规律",tag);
		//payloadFile.open(historyAreaFilePath.c_str(),ios::in);
		//从文件中读取历史的区域信息
		if (!payloadFile.is_open())
		{
			cout<< "Error opening file";
			return;
		}
		boost::archive::text_iarchive ia(payloadFile);
		Area area;
		try
		{
			while(!payloadFile.eof())
			{

				//将读取到的area保存起来
				ia >>area;
				char c[20];
				string s;
				sprintf(c,"%d",area.level);
				s.append(c);
				s.append("#");
				sprintf(c,"%d",area.id);
				s.append(c);
				areaMap[s]= area;
			}

		}
		catch(boost::archive::archive_exception &e)
		{}
		payloadFile.close();
	}*/


	/**
	 * 作用 ：根据bundle的目的节点，查询当前邻居节点中离目的地最近的区域（主要是尽可能低的区域层次）
	 * @param bundle :需要对比目的节点的bundle
	 * @return :如果找到了同一层次的区域则返回该层次区域；如果该邻居区域信息为空，则为null；一般情况下只要有区域信息就能返回区域信息的。
	 */
	Area *checkBundleDestArea(Bundle bundle)
	{
		if(areaMap.empty())
			return NULL;

		string areastr;
		Area *result=NULL;
		char c[20];
		for(int i=1;i<=bundle.getAreaSize();++i)
		{
			sprintf(c,"%d",i);
			areastr.append(c);
			areastr.append("#");
			sprintf(c,"%d",bundle.getAreaId(i));
			areastr.append(c);
			hash_map<string,Area>::iterator it=areaMap.find(areastr);
			if(it!=areaMap.end())
			{
				result=&areaMap[areastr];
				return result;
			}
		}
		return result;
	}

	/**
	 * 根据area查询当前
	 * @param area
	 * @return
	 */
	Area *checkArea(Area area)
	{
		string areastr;
		Area *result=NULL;
		char c[20];
		sprintf(c,"%d",area.getAreaLevel());
		areastr.append(c);
		areastr.append("#");
		sprintf(c,"%d",area.getAreaId());
		areastr.append(c);
		hash_map<string,Area>::iterator it=areaMap.find(areastr);
		if(it!=areaMap.end())
		{
			result=&areaMap[areastr];
			return result;
		}
		return result;
	}
}
