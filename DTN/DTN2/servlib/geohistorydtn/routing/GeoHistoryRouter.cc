#include "GeoHistoryRouter.h"
#include "servlib/routing/RouteEntry.h"
#include "servlib/routing/RouteTable.h"
//#include "applib/dtn_api.h"

#include "sys/types.h"


namespace dtn
{

GeoHistoryRouter::GeoHistoryRouter()
:TableBasedRouter("GeoHistoryRouter", "geohistory"),
 Thread("GeoHistoryRouter", CREATE_JOINABLE)
{
	expiration=BundleConfig::EXPIRATION_TIME;
	baseArea=NULL;
	areamanager=AreaManager::Getinstance();
	geohistoryLog=GeohistoryLog::GetInstance();

}


//////////////////针对收到要发送的bundle，需要转发的bundle，邻居交互信息的bundle的处理
	void GeoHistoryRouter::handle_bundle_received(BundleReceivedEvent *event)
	{
		cout<< "GeohistoryRouter收到一个BundleReceivedEvent"<<endl;
		cout<<"!"<<event->bundleref_.object()->getBundleType()<<endl;
		cout<<"!"<<event->bundleref_.object()->source().str()<<endl;
		cout<<"!"<<event->bundleref_.object()->dest().str()<<endl;
		cout<<"!"<<event->source_<<endl;


		int length=event->bundleref_.object()->payload().length();
		u_char *buf=new u_char[length];
		event->bundleref_.object()->payload().read_data(0,length,buf);

		//cout<<buf<<endl;

		geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"GeohistoryRouter 收到一个 BundleReceivedEvent");
		//
		// * DTN中的时间，bundle的creation_ts为bundle创建的相对时间，DTNTime.TIMEVAL_CONVERSION就是相对的参数，应该是2000，00：00。
		// * 利用creation_ts和DTNTime.TIMEVAL_CONVERSION即可得到bundle创建时的真实时间
		// * bundle的expirionTime则是指bundle的有效时间
		// * bundle之间
		 //*
		if(event->bundleref_.object()->getBundleType()==Bundle::NEI_AREA_BUNDLE)
		{
			//自己想邻居发送的交换历史区域的bundle
			if(event->bundleref_.object()->source().str()==BundleDaemon::GetInstance()->local_eid().str())
			{
				cout<<"本节点往"<<event->bundleref_.object()->dest().str()<<"发送自己的区域移动信息";

			}
			else if(event->bundleref_.object()->dest().str()==BundleDaemon::GetInstance()->local_eid().str())
			{
				cout<<"邻居发来的邻居bundle"<<endl;
				const EndpointID source_eid=event->bundleref_.object()->source();
				Neighbour *nei=NeighbourManager::Getinstance()->checkNeighbour(source_eid);
				const BundlePayload &payload=event->bundleref_.object()->payload();
				//将payload保存到文件中，再更新NeighbourArea
				nei->getNeighbourArea()->Payload_update(source_eid.str(),payload);
			}
			else
			{
				printf("既不是自己发出的，也不是发给自己的邻居信息，源：%s,目的：%s"
										,event->bundleref_.object()->source().str().c_str(),event->bundleref_.object()->dest().str().c_str());
				geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"既不是自己发出的，也不是发给自己的邻居信息，源：%s,目的：%s"
						,event->bundleref_.object()->source().str().c_str(),event->bundleref_.object()->dest().str().c_str());
			}

		}
		//对其他类型bundle进行
		else if(event->bundleref_.object()->getBundleType()==Bundle::DATA_BUNDLE)
		{

		}
		else
		{
			printf("bundle_%d 的类型不明确，type：%d", event->bundleref_.object()->bundleid(),
					event->bundleref_.object()->getBundleType());
			geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"bundle_%d 的类型不明确，type：%d", event->bundleref_.object()->bundleid(),
					event->bundleref_.object()->getBundleType());
		}

		///////TableBasedRouter原本的
		std::cout<<"geohistory::handle_bundle_received"<<std::endl;
		bool should_route = true;

		Bundle* bundle = event->bundleref_.object();
		log_debug("handle bundle received: *%p", bundle);

		EndpointID remote_eid(EndpointID::NULL_EID());

		if (event->link_ != NULL) {
		   remote_eid = event->link_->remote_eid();
		}
		if (! reception_cache_.add_entry(bundle, remote_eid))
		{
		   log_info("ignoring duplicate bundle: *%p", bundle);
		   BundleDaemon::post_at_head(
					new BundleDeleteRequest(bundle, BundleProtocol::REASON_NO_ADDTL_INFO));
		   return;
		}
		Session* session = get_session_for_bundle(bundle);
		if (session != NULL)
		{
			should_route = add_bundle_to_session(bundle, session);
			if (! should_route) {
			   log_debug("session bundle %u is DOA", bundle->bundleid());
			   return; // don't route it
			}
		}
		if (bundle->session_flags() != 0) {
			should_route = handle_session_bundle(event);
		}
		if (should_route) {
		    route_bundle(bundle);
	    } else {
			BundleDaemon::post_at_head(
					new BundleDeleteRequest(bundle, BundleProtocol::REASON_NO_ADDTL_INFO));
	   }


	}

/////////////////////////处理link//////////
	void GeoHistoryRouter::handle_link_created(LinkCreatedEvent *event) {
		cout<<"geohistory:link_created"<<endl;
//		super.handle_link_created(event);
	    LinkRef link = event->link_;
	    ASSERT(link != NULL);
	    ASSERT(!link->isdeleted());

	    link->set_router_info(new DeferredList(logpath(), link));

	    EndpointID eid = link->remote_eid();
	    if (config_.add_nexthop_routes_ && eid != EndpointID::NULL_EID())
	    {
	        EndpointIDPattern eid_pattern(link->remote_eid());

	        // attempt to build a route pattern from link's remote_eid
	         if (!eid_pattern.append_service_wildcard())
	              // else assign remote_eid as-is
	             eid_pattern.assign(link->remote_eid());

	          // XXX/demmer this shouldn't call get_matching but instead
	          // there should be a RouteTable::lookup or contains() method
	          // to find the entry
	          RouteEntryVec ignored;
	        if (route_table_->get_matching(eid_pattern, link, &ignored) == 0)
	          {
	              RouteEntry *entry = new RouteEntry(eid_pattern, link);
	              entry->set_action(ForwardingInfo::FORWARD_ACTION);
	              //add_route(entry);//去除掉了源代码中的handle_changed_routes()
	              	  	  	  	  	  //即不需要将bundle,session重新路由选择
	              route_table_->add_entry(entry);
	           }
	     }
	}



	void GeoHistoryRouter::handle_contact_up(ContactUpEvent* event)
	{
		cout<<"geohistory:contact_up"<<endl;

		LinkRef link = event->contact_->link();
		ASSERT(link != NULL);
		ASSERT(!link->isdeleted());

		if (! link->isopen()) {
		      log_err("contact up(*%p): event delivered but link not open",
		                link.object());
		}

		add_nexthop_route(link);
		check_next_hop(link);

		RerouteTimerMap::iterator iter = reroute_timers_.find(link->name_str());
		 if (iter != reroute_timers_.end()) {
		      log_debug("link %s reopened, cancelling reroute timer", link->name());
		      RerouteTimer* t = iter->second;
		      reroute_timers_.erase(iter);
		      t->cancel();
		  }
		 /////////////////////////////////////////////////
		  //获取eid
		  EndpointID eid=link->remote_eid();

		  //a)	触发对已有邻居信息的保存
		  Neighbour *nei=NeighbourManager::Getinstance()->checkNeighbour(eid);
		  nei->addTimeCount();//添加当前邻居的计时器

		  //b)	触发邻居之间交换信息的bundle发送,发送本节点的历史区域信息
		  fstream fr;

		  AreaManager::Getinstance()->lockHistoryAreaMovingFile();//锁住文件的读写
		  fr.open(AreaManager::historyAreaFilePath.c_str());

		  if(fr)
		  {
			  SendBundleMsg *sendbundle=new SendBundleMsg(nei->getEid().str(), AreaManager::historyAreaFilePath, false, AreaInfo::defaultAreaId(), Bundle::NEI_AREA_BUNDLE);
			  AreaManager::Getinstance()->unlockHistoryAreaMovingFile();//解锁自身规律文件的锁
			  fr.close();
			  areaInfoQueue.push(sendbundle);
		  }
		  else
			  cout<<"don not have areahistory.txt!"<<endl;
	}

	void GeoHistoryRouter::handle_sendBundle(SendBundleMsg *msg)
	{
		printf("send message!\n");
		bool f=sendMessage(msg->dest_eid, msg->fileroute, msg->rctp,msg->areaid,msg->bundleType);
		if(f)
			cout<<"send successfully!"<<endl;
		else
			cout<<"send fail!"<<endl;
	}

	/**
	 * GeoHistoryDtn发送数据的
	* @param dest_eid
	* @param file
	* @param rctp
	* @param eventSource :标明该bundle是哪种类型，是APPbundle还是邻居间交换信息的bundle
	* @param areaid :用来表示目的节点的区域信息,不能为空
	* @return
	* @throws UnsupportedEncodingException
	* @throws DTNOpenFailException
	* @throws DTNAPIFailException
	*/
	 bool GeoHistoryRouter::sendMessage(string dest_eid,string fileroute,bool rctp,vector<int> areaid,int bundleType)
	 {


		 //判断bundle的类型
		if(bundleType==Bundle::DATA_BUNDLE)
		{
			if(areaid.empty() )//|| areaid.length!=4)
			{
				cout<<"DTN应用发送bundle时，目的节点的各层次区域信息不明确"<<endl;
				geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"DTN应用发送bundle时，目的节点的各层次区域信息不明确");
				return false;
			}
		}
		else if(bundleType!=Bundle::NEI_AREA_BUNDLE)
		{
			cout<<"DTN发送bundle时，event_source不明确，既不是应用的bundle也不是邻居的bundle"<<endl;
			geohistoryLog->LogAppend(geohistoryLog->INFO_LEVEL,"DTN发送bundle时，event_source不明确，既不是应用的bundle也不是邻居的bundle");
			return false;
		}

		//由APIServer的成员函数handle_send()修改

		BundleRef b("GeoHistoryRouter::handle_send");
	    b = new Bundle();

	      if(bundleType==Bundle::DATA_BUNDLE)
	    {
	    	b->setAreaSize(AreaLevel::CurrentAreaLevelNum);
	    	int i=1;
	    	for(vector<int>::iterator it=areaid.begin();it!=areaid.end();++it)
	    	{
	    		b->setAreaId(i,*it);
	    		++i;
	    	}
	    	if(i-1!=AreaLevel::CurrentAreaLevelNum)
	    		return false;
	    }

		b->setDeliverBundleNum(BundleConfig::DELIVERBUNDLENUM);
		b->setFloodBundleNum(BundleConfig::FLOODBUNDLENUM);
		b->setIsFlooding(0);
		b->setBundleType(bundleType);



		string source=BundleDaemon::GetInstance()->local_eid().str();
		b->mutable_source()->assign(source);
		b->mutable_dest()->assign(dest_eid);

	    b->mutable_replyto()->assign(source);
	    b->mutable_custodian()->assign(EndpointID::NULL_EID());

	    EndpointID::singleton_info_t info;

	    if (b->dest().known_scheme())
	    {
	        info = b->dest().is_singleton();

	         // all schemes must make a decision one way or the other
	        ASSERT(info != EndpointID::UNKNOWN);
	    }
	    else
	    {
	        info = EndpointID::is_singleton_default_;
	    }

	    switch (info)
	    {
	        case EndpointID::UNKNOWN:
	           return false;

	         case EndpointID::SINGLETON:
	            b->set_singleton_dest(true);
	            break;

	         case EndpointID::MULTINODE:
	             b->set_singleton_dest(false);
	             break;
	    }
	   if(sendMessage2(b,fileroute,source))
	    	return true;
	    else
	    	return false;
	 }
	 bool GeoHistoryRouter::sendMessage2(BundleRef b,string fileroute,string source)
	 {
	    dtn_bundle_priority_t priority = COS_NORMAL;

	    switch (priority)
	    {
	 	 #define COS(_cos) case _cos: b->set_priority(Bundle::_cos); break;
	         COS(COS_BULK);
	         COS(COS_NORMAL);
	         COS(COS_EXPEDITED);
	         COS(COS_RESERVED);
		 #undef COS
	     default:
	         return DTN_EINVAL;
	     };


	    const RegistrationTable* reg_table = BundleDaemon::instance()->reg_table();
	    RegistrationList unused;
	    int dopts=0;
	    if (b->source() == EndpointID::NULL_EID())
	    {
	        // Bundles with a null source EID are not allowed to request reports or
	        // custody transfer, and must not be fragmented.
	        if ((dopts & (DOPTS_CUSTODY | DOPTS_DELIVERY_RCPT |
	                           DOPTS_RECEIVE_RCPT | DOPTS_FORWARD_RCPT |
	                           DOPTS_CUSTODY_RCPT | DOPTS_DELETE_RCPT)) ||
	            ((dopts & DOPTS_DO_NOT_FRAGMENT)==0) )
	        {
	            log_err("bundle with null source EID requested report and/or "
	                    "custody transfer and/or allowed fragmentation");
	            return false;
	        }

	        b->set_do_not_fragment(true);
	    }

	    else if (reg_table->get_matching(b->source(), &unused) != 0)
	    {
	        // Local registration -- don't do anything
	    }
	    else if (b->source().subsume(BundleDaemon::instance()->local_eid()))
	    {
	        // Allow source EIDs that subsume the local eid
	    }
	    else
	    {
	        log_err("this node is not a member of the bundle's source EID (%s)",
	                b->source().str().c_str());
	        return false;
	    }

	    // delivery options
	    if (dopts & DOPTS_CUSTODY)
	        b->set_custody_requested(true);

	    if (dopts & DOPTS_DELIVERY_RCPT)
	        b->set_delivery_rcpt(true);

	    if (dopts & DOPTS_RECEIVE_RCPT)
	        b->set_receive_rcpt(true);

	    if (dopts & DOPTS_FORWARD_RCPT)
	        b->set_forward_rcpt(true);

	    if (dopts & DOPTS_CUSTODY_RCPT)
	        b->set_custody_rcpt(true);

	    if (dopts & DOPTS_DELETE_RCPT)
	        b->set_deletion_rcpt(true);

	    if (dopts & DOPTS_DO_NOT_FRAGMENT)
	        b->set_do_not_fragment(true);

	    b->set_expiration(expiration);


	    size_t payload_len;
	    char filename[30];
	    struct stat finfo;

	    sprintf(filename, "%.*s",fileroute.size(),fileroute.c_str());
	    if (stat(filename, &finfo) != 0)
	    {
	        return false;
	    }

	    int temp=finfo.st_size;
	    payload_len =temp;
	    b->mutable_payload()->set_length(payload_len);


	    if(sendMessage3(b,payload_len,filename,source))
	    	return true;
	    else
	    	return false;

	}

	bool GeoHistoryRouter::sendMessage3(BundleRef b,size_t payload_len,char *filename,
			 	 	 	 	 	 	 	 string source)
	{
        FILE* file;
        u_char *buffer=new u_char[payload_len];

        int r = 0;

        AreaManager::Getinstance()->lockHistoryAreaMovingFile();//锁住文件的读写
        if ((file = fopen(filename, "r")) == NULL)
        {
        	AreaManager::Getinstance()->unlockHistoryAreaMovingFile();//锁住文件的读写
            log_err("payload file %s can't be opened: %s",
                    filename, strerror(errno));
            return false;
        }
        r = fread(buffer, 1, payload_len, file);
        b->mutable_payload()->write_data(buffer, 0, payload_len);
      /*  while (left > 0)
       	{
        	printf("")
       	    r = fread(buffer, 1, (left>4096)?4096:left, file);
       	    printf("~~%d~~\n",r);
       	    printf("%s",buffer);

       	    if (r)
       	    {
       	         b->mutable_payload()->write_data(buffer, offset, r);
       	         left   -= r;
       	    }
       	    else
       	    {
       	        sleep(1); // pause before re-reading
       	     }
       	 }*/

       	 fclose(file);
         AreaManager::Getinstance()->unlockHistoryAreaMovingFile();//锁住文件的读写
       	 delete buffer;

       /*	 dtn_bundle_id_t id;
       	 memcpy(&id.source, source.c_str(), sizeof(dtn_endpoint_id_t));

       	 id.creation_ts.secs  = b->creation_ts().seconds_;
       	 id.creation_ts.seqno = b->creation_ts().seqno_;
       	 id.frag_offset = 0;
       	 id.orig_length = 0;*/

       	 log_info("DTN_SEND bundle *%p", b.object());

       	 // Sync the bundle payload to disk
        b->mutable_payload()->sync_payload();

	    BundleDaemon::post(
	        new BundleReceivedEvent(b.object(), EVENTSRC_APP));
         return true;
	 }


}
