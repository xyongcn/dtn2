#include "NeighbourManager.h"
namespace dtn{
	NeighbourManager *NeighbourManager::instance=NULL;
	const string NeighbourManager::tag="NeighbourManager";
	string NeighbourManager::historyNeighbourFileName="./logDocuments/geoHistory_dtn/historyNeighbour";
	string NeighbourManager::historyNeighbourFileDirectory="./logDocuments/geoHistory_dtn/";


	NeighbourManager::NeighbourManager()
	{
		geohistoryLog=GeohistoryLog::GetInstance();
		//neighbourlist=new HashMap<String, Neighbour>(NeighbourConfig::NEIGHOURNUM);
	}


	void NeighbourManager::init()
	{
		//从文件中读取邻居
		fstream file;

		file.open(historyNeighbourFileName.c_str(),ios::in | ios::binary);
		boost::archive::text_iarchive ia(file);
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"从文件%s中读取历史的邻居信息",historyNeighbourFileName.c_str());

		Neighbour neighbour;
		try
		{
			while(!file.eof())
			{
				ia >> neighbour;
				string s=neighbour.neighbourEid.str();
				if(s.empty())
					continue;
				neighbourlist[s]=neighbour;
				neighbour.addAreaFVector2Manager();

			}
		}catch(boost::archive::archive_exception &e)
		{}
		file.close();
	}

	/**
	 * 将历史的邻居记录保存到文件中，以便下一次访问
	 */
	void NeighbourManager::saveHistoryNeighbour()
	{
		string directories;
		string file;
		directories.append(historyNeighbourFileDirectory);
		file.append(historyNeighbourFileName);
		fstream neighbourAreaDir(directories.c_str());

		if (!neighbourAreaDir)
		{
			if(mkdir(directories.c_str(),S_IRWXU|S_IRWXG|S_IRWXO)==0)
				cout<<"Folder creation success"<<endl;//文件夹创建成功
			else
				cout<<"Folder creation fail"<<endl;//can not make a dir;
			return;
		}

		fstream neighbourAreaFile;
		neighbourAreaFile.open(file.c_str(),ios::out | ios::binary | ios::trunc);
		boost::archive::text_oarchive oa(neighbourAreaFile);
		for(hash_map<string,Neighbour>::iterator it=neighbourlist.begin();
			it!=neighbourlist.end();++it)
		{
			Neighbour n=it->second;
			oa << n;
		}
		neighbourAreaFile.close();
	}



	/**
	 * 关闭邻居管理器所要进行的必要步骤和操作，包括清空已有的邻居表，保存历史的邻居信息
	 */
	void NeighbourManager::shutdown()
	{
		//退出前，打印本节点的邻居的相遇频率，邻居的区域频率向量
		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"NeighbourManager退出前打印所有的邻居的相遇频率以及邻居的区域频率向量");
		for(hash_map<string,Neighbour>::iterator it=neighbourlist.begin();
				it!=neighbourlist.end();++it)
		{
			Neighbour n=it->second;
			geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s",n.toString().c_str());
			n.printAreaInfo();
		}
		saveHistoryNeighbour();//保存所有的邻居对象到文件

		neighbourlist.clear();//清空邻居列表
	}

	/**
	 * 遇到新邻居后进行check，如果没有该邻居将其建立
	 */
	Neighbour * NeighbourManager::checkNeighbour(EndpointID eid)
	{
		//if(eid==null)
			//return null;
		hash_map<string,Neighbour>::iterator it=neighbourlist.find(eid.str());
		if(it==neighbourlist.end())
		{
			Neighbour nei(eid);
			neighbourlist[eid.str()]=nei;
		}
		hash_map<string,Neighbour>::iterator it2=neighbourlist.find(eid.str());
		Neighbour *n=&(it2->second);
		//更改频率向量
		n->checkVectorChange();
		return n;
	}

	/**
	 * 找到该eid的Neichbour
	 */
	Neighbour * NeighbourManager::getNeighbour(EndpointID eid)
	{
		//if(eid==null)
			//return null;
		string eid_s=eid.str();
		eid_s=eid_s.substr(0,eid_s.length()-2);

		hash_map<string,Neighbour>::iterator it=neighbourlist.find(eid_s);
		if(it==neighbourlist.end())
			return NULL;
		else
		{
			Neighbour *n=&(it->second);
			return n;
		}
	}

	list<Neighbour *> NeighbourManager::getAllNeighbour()
	{
		list<Neighbour *> allneighbour;
		for(hash_map<string,Neighbour>::iterator it=neighbourlist.begin();
				it!=neighbourlist.end();++it)
		{
			Neighbour *n=&(it->second);
			allneighbour.push_back(n);
		}
		return allneighbour;
	}

	void NeighbourManager::printAllNeighbour()
	{
		for(hash_map<string,Neighbour>::iterator it=neighbourlist.begin();
				it!=neighbourlist.end();++it)
		{
			Neighbour *n=&(it->second);
			cout<<n->toString()<<endl;
			//geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"%s",n->toString().c_str());
		}
	}

	/**
	 * 将邻居发来的区域频率信息的bundle的payload内容保存下来
	 * @param payload：bundle的内容
	 * @param eid：发来bundle的邻居的eid
	 */
	void NeighbourManager::saveNeighbourAreaPayload(BundlePayload payload,string eid)
	{
		string directories;
		string file;
		directories.append(NeighbourConfig::NEIGHBOURAREAFILEDIR);
		file.append(directories);
		file.append(eid);

		fstream Dir(directories.c_str());
		if (!Dir)
		{
			if(mkdir(directories.c_str(),S_IRWXU|S_IRWXG|S_IRWXO)==0)
				cout<<"Folder creation success"<<endl;//文件夹创建成功
			else
				cout<<"Folder creation fail"<<endl;//can not make a dir;
			return;
		}

		fstream fr;
		fr.open(file.c_str(),ios::trunc | ios::out | ios::binary);
		//将邻居的区域记录保存到本地，利用payload里面原有的方法
		//GeohistoryLog.i(tag, String.format("将邻居（ %s）发来的payload里面的区域移动规律存储到文件中", eid));
		//payload.copy_to_file(neighAreaFile);
		int length=payload.length();
		u_char *buf=new u_char[length];
		payload.read_data(0,length,buf);

		fr.write((char *)buf,length);
		fr.close();
	}


}
