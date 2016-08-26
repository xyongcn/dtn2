#include "GeoHistoryRouter.h"
#include "servlib/routing/RouteEntry.h"
#include "servlib/routing/RouteTable.h"
//#include "applib/dtn_api.h"

#include "sys/types.h"
#include "servlib/bundling/BundleTimestamp.h"

namespace dtn
{

GeoHistoryRouter::GeoHistoryRouter()
:TableBasedRouter("GeoHistoryRouter", "geohistory"),
 Thread("GeoHistoryRouter", CREATE_JOINABLE)
{
	routeAllBundle=new RouteAllBundleMsg();
	expiration=BundleConfig::EXPIRATION_TIME;
	baseArea=NULL;
	areamanager=AreaManager::Getinstance();
	geohistoryLog=GeohistoryLog::GetInstance();

}

/*int GeoHistoryRouter::route_bundle(Bundle* bundle)
{
	//如果是对指定邻居发送区域的频率信息的bundle，在这里处理即可
	if(bundle->getBundleType()==Bundle::NEI_AREA_BUNDLE)
	{
		return route_neighbourArea_bundle(bundle);
	}
	printf("route_bundle:send data bundle,bundle dst(%s),bundleid(%d),areaId(%d) ",
			bundle->dest().str().c_str(),bundle->bundleid(),bundle->getAreaId(bundle->getAreaSize()));

    RouteEntryVec matches;
    RouteEntryVec::iterator iter;
    map<Arearef,RouteEntryref> linkAreaMap;

	log_debug("route_bundle: checking bundle %d", bundle->bundleid());

	// check to see if forwarding is suppressed to all nodes
	if (bundle->fwdlog()->get_count(EndpointIDPattern::WILDCARD_EID(),
	                                    ForwardingInfo::SUPPRESSED) > 0)
    {
	    log_info("route_bundle: "
	                 "ignoring bundle %d since forwarding is suppressed",
	                 bundle->bundleid());
	    return 0;
	}
	//判断 bundle的关于区域的信息是否合法，GeoHistory相关信息不合法时退出发送
	if(!bundle->isGeoHistoryDtnValide())
	{
		printf("bundle_%d 的geohistory相关信息不合法",bundle->bundleid());
		return 0;
	}
	//判断是否遇到了目标节点，是否可以直接发送给目标节点
	if(canDirectDelivery(bundle))
	{
		return 1;
	}
	//查找合适的需要转发的bundle
	Area *bundleArea=new Area(bundle->getAreaSize(),bundle->getAreaId(bundle->getAreaSize()));
	//本节点到达目的地区域
	if(baseArea->id==bundleArea->id)
	{
		if(bundle->getIsFlooding()==0)
		{
			bundle->setIsFlooding(1);
		}
		printf("进入目标区域，执行洪泛转发:dst(%s),copyNum(%d),bundle_%d,区域:%d",
					bundle->dest().str().c_str(),bundle->getFloodBundleNum(),bundle->bundleid(),bundle->getAreaId(bundle->getAreaSize()));
		//进行洪泛转发
		get_matching_RouteEntryVec(bundle,matches, -1, linkAreaMap);

	    sort_routes(bundle, &matches);

	    log_debug("route_bundle bundle id %d: checking %zu route entry matches",
	              bundle->bundleid(), matches.size());

	    unsigned int count = 0;
	    for (iter = matches.begin(); iter != matches.end(); ++iter)
	    {
	        RouteEntry* route = *iter;
	        log_debug("checking route entry %p link %s (%p)",
	                  *iter, route->link()->name(), route->link().object());

	        if (! should_fwd(bundle, *iter))
	        {
	            continue;
	        }

			//更改bundle的副本数量
			string bundle_id=StringOfBundle(bundle);
			int num=bundle->getFloodBundleNum();
			map<string,int>::iterator it=Forward2PayloadNumMap.find(bundle_id);
			if(it!=Forward2PayloadNumMap.end())
			{
				num=Forward2PayloadNumMap[bundle_id]/2;
			}

			if(num<1)
			{
				printf("bundle(to:%s) don't have flood payload num(%d)",bundle->dest().str().c_str(),num);
				continue;
			}

			bundle->setFloodBundleNum(num);
			Forward2PayloadNumMap[bundle_id]= num;

	        DeferredList* dl = deferred_list(route->link());

	        if (dl == 0)
	           continue;

	        if (dl->list()->contains(bundle)) {
	              log_debug("route_bundle bundle %d: "
	                        "ignoring link *%p since already deferred",
	                        bundle->bundleid(), route->link().object());
	              continue;
	        }


	        check_next_hop(route->link());

	        if (!fwd_to_nexthop(bundle, *iter)) {
	              continue;
	        }

	        ++count;
	     }
	    log_debug("route_bundle bundle id %d: forwarded on %u links",
	              bundle->bundleid(), count);
	    return count;
	}
	else
	{
		//本节点进入目的区域后又离开了,这里不进行任何转发
		if(bundle->getIsFlooding()==1)
		{
			printf("进入目标区域又离开了，不进行任何转发。bundle_%d,目的区域：%d",
					bundle->bundleid(),bundle->getAreaId(bundle->getAreaSize()));
			//判断是否超时
			//先不管超时，bytewalla应该自己管理好了超时
//				if(bundle.creation_ts().seconds()+bundle.expiration_timer())
			return 0;
		}
		//本节点没有进入目的区域，执行普通转发
		else
		{
			 printf("没有进入目标区域，执行普通转发:dst(%s),copyNum(%d),bundle_%d,目的区域：%d",
						bundle->dest().str().c_str(),bundle->getDeliverBundleNum(),bundle->bundleid(),bundle->getAreaId(bundle->getAreaSize()));

			 //普通转发
			 //本节点离目的地最近的区域
			 Area *thisnodeArea=baseArea->checkBundleDestArea(bundle);
			 if(thisnodeArea==NULL)
			{
				printf("执行普通转发时，本节点没有找到与目标节点处在同一层的区域。bundle_%d,目的区域：%d",
							bundle->bundleid(),bundle->getAreaId(bundle->getAreaSize()));
				return 0;
			}
			 //获取符合要求的当前邻居节点
			 //要求邻居结点一定要比当前结点到达的区域层次更接近目的区域,且只有一个
			get_matching_RouteEntryVec(bundle,matches,thisnodeArea->getAreaLevel(),linkAreaMap);

			if(linkAreaMap.empty())
			{
			 	printf("没有找到合适的link");
			 	return 0;
			}
			map<Arearef,RouteEntryref>::iterator it_link=linkAreaMap.begin();
			list<Arearef> avaliableNodeList;
			for(;it_link!=linkAreaMap.end();++it_link)
			{
				Arearef temp_area=it_link->first;
				avaliableNodeList.push_back(temp_area);
			}
			//存储当前邻居结点到达bundle的最近区域，和当前结点到达bundle的最近区域,且当前结点区域层次一定比当前邻居的高
			avaliableNodeList.push_back(thisnodeArea);
			//存储当前邻居结点到达bundle的最近区域
			//历史邻居到达bundle的最近区域,且一定不等于当前邻居结点和当前结点到达bundle的最近区域
			avaliableNodeList=ChanceValueSort::getAllAvaliableNodeArea(&avaliableNodeList, bundle, thisnodeArea);

			//寻找合适的link转发
			int no=1;//标志着该节点的排名
			int count = 0;//原路由算法携带，返回向邻居发送bundle的个数
			for(list<Arearef>::iterator it_nl=avaliableNodeList.begin();it_nl!=avaliableNodeList.end();it_nl++)
			{
				RouteEntryref route=linkAreaMap[*it_nl];
				if(route!=NULL)
				{
			        log_debug("checking route entry %p link %s (%p)",
			                  *iter, route->link()->name(), route->link().object());

			        if (! should_fwd(bundle, *iter)) {
			            continue;
			        }

					//更改bundle的副本数量
					string bundle_id=StringOfBundle(bundle);
					int num=bundle->getDeliverBundleNum();
					map<string,int>::iterator it_f=Forward2PayloadNumMap.find(bundle_id);
					if(it_f!=Forward1PayloadNumMap.end())
					{
						num=Forward1PayloadNumMap[bundle_id]/2;
					}

					if(num<=1){
						printf("bundle(to:%s) don't have deliver num(%d)",bundle->dest().str().c_str(),num);
						continue;
					}

					//该节点排名小于副本数量，执行2分法发送
					if(no<=num)
					{
						bundle->setDeliverBundleNum(num/2);
						num=num-num/2;
					}
					//该节点排名大于副本数量，只发送1份
					else
					{
						--num;
						bundle->setDeliverBundleNum(1);
					}
					Forward1PayloadNumMap[bundle_id]=num;
			        DeferredList* dl = deferred_list(route->link());

			        if (dl == 0)
			          continue;

			        if (dl->list()->contains(bundle)) {
			            log_debug("route_bundle bundle %d: "
			                      "ignoring link *%p since already deferred",
			                      bundle->bundleid(), route->link().object());
			            continue;
			        }
			        check_next_hop(route->link());

			        if (!fwd_to_nexthop(bundle, *iter)) {
			            continue;
			        }

			        ++count;
				}
				++no;

			}
			printf("route_bundle bundle id %d: forwarded on %d links",
					bundle->bundleid(), count);
			return count;

		}
	}
}*/



/**
* 作用： 当sameAreaLevel为-1时，执行的洪泛转发所需要的link表；当sameAreaLevel为合法值时，将返回与目标尽可能接近目的地的邻居的link表。
* @param bundle ：需要转发的bundle
* @param entry_vec :需要返回的路由表项表
* @param sameAreaLevel ：本节点与目标所在的同一区域层次。如果为-1(RouteType.Flooding.sameAreaLevel)，则表示是执行洪泛的方法；
 * 							如果是-2(RouteType.Neighbour.sameAreaLevel)则表示对特定的邻居发送；
* 							如果是合法的值，则是查找尽可能接近邻居的路由表项;用路由方式来代替
 * @return the number of matching
*/
int GeoHistoryRouter::get_matching_RouteEntryVec(Bundle *bundle,RouteEntryVec entry_vec,
					int sameAreaLevel,map<Arearef,RouteEntryref> sameLevelAreaMap)

{
	route_table_->lock()->lock("GeoHistoryRouter::get_matching_RouteEntryVec");
	//尽可能底层的LEVEL，AreaLevel
	int areaLevel=sameAreaLevel;
	int count = 0;

    RouteEntryVec::const_iterator iter;
    for (iter = route_table_->route_table()->begin();
         iter != route_table_->route_table()->end(); ++iter)
    {
        RouteEntry* entry = *iter;
		//如果为本节点将跳过
		if (entry->dest_pattern().match(BundleDaemon::GetInstance()->local_eid()))
			continue;
		//判断该bundle是否从该link收到过或者发送过，决定是否需要转发
		EndpointID *prevhop;
		bool prevhop_result=reception_cache_.lookup(bundle,prevhop);
		if(prevhop_result)
		{
			if (prevhop->equals(entry->link()->remote_eid())
					&& !prevhop->equals(EndpointID::NULL_EID()))
			{
				printf("bundle在这个link(remote:%s)收到过或者发送过",prevhop->str().c_str());
				continue;
			}
			else
			{
				printf("link(%s)上发送bundle(%d)并不重复",prevhop->str().c_str(),
						bundle->bundleid());
			}
		}
		//如果是flood的转发的方式，那么需要获取所有的link
		if(sameAreaLevel==RouteType::Flooding)
		{
			printf("洪泛式路由方式查找路由表中的Link,bundle dst:%s",bundle->dest().str().c_str());
			//原有算法，将link加到路由项中
			if(	entry->link()->state() ==Link::OPEN ||
					entry->link()->state() == Link::AVAILABLE )
			{
				RouteEntryVec::const_iterator p;
				for(p=entry_vec.begin();p!=entry_vec.end();++p)
				{
					if(entry == *p)
						break;
				}
				if(p== entry_vec.end())
				{
					entry_vec.push_back(entry);
					++count;
				}
				else
				{
					printf("entry already in matches... ignoring");
				}
			}
			//如果只是转发给特定的邻居，那么只要获取邻居的link就行了
			else if(sameAreaLevel==RouteType::Neighbour)
			{
				printf("向特定邻居发送信息的路由方式查找路由表中的Link,bundle dst:%s",bundle->dest().str().c_str());
				//检查是否是目的邻居
				if(entry->dest_pattern().match(bundle->dest()))
				{
					//将link加到路由项中
					if(	entry->link()->state() ==Link::OPEN ||
							entry->link()->state() == Link::AVAILABLE )
					{
						RouteEntryVec::const_iterator p;
						for(p=entry_vec.begin();p!=entry_vec.end();++p)
						{
							if(entry == *p)
								break;
						}
						if(p== entry_vec.end())
						{
							entry_vec.push_back(entry);
							++count;
						}
						else
						{
							printf("entry already in matches... ignoring");
						}
					}
				}
			}
			else if(sameAreaLevel>0)
			{
				printf("普通转发方式查找路由表中Link,bundle dst:%s,sameAreaLevel:%s",
											bundle->dest().str().c_str(),sameAreaLevel);
				Neighbour *neighbour=NeighbourManager::Getinstance()->getNeighbour(entry->dest_pattern());
				if(neighbour==NULL)
				{
					printf("没有邻居%s的记录",neighbour->getEid().str().c_str());
					continue;
				}
				NeighbourArea *neiArea=neighbour->getNeighbourArea();
				if(neiArea->areaMap.empty())
				{
					printf("没有邻居%s的历史区域移动规律",neighbour->getEid().str().c_str());
					continue;
				}
				Area *area=neiArea->checkBundleDestArea(bundle);
				if(area==NULL)
				{
					printf("邻居%s没有去过目标区域",neighbour->getEid().str().c_str());
					continue;
				}
				//本节点离目标节点更接近，因此否决该link
				if(area->getAreaLevel()>areaLevel)
				{
					printf("本节点到过的区域比邻居%s更接近目标区域",neighbour->getEid().str().c_str());
					continue;
				}

				//该邻居比本节点更接近目的地，调整区域等级和已有的link表
				else if(area->getAreaLevel()<areaLevel)
				{
					entry_vec.clear();
					sameLevelAreaMap.clear();
					areaLevel=area->getAreaLevel();
				}
				//该邻居与本节点一样接近目的地，默认操作
				else
				{
					printf("本节点和比邻居%s一样接近目标区域",neighbour->getEid().str().c_str());
					continue;
				}
				//将link加到路由项中
				if(	entry->link()->state() ==Link::OPEN ||
						entry->link()->state() == Link::AVAILABLE )
				{
					RouteEntryVec::const_iterator p;
					for(p=entry_vec.begin();p!=entry_vec.end();++p)
					{
						if(entry == *p)
							break;
					}
					if(p== entry_vec.end())
					{
						entry_vec.push_back(entry);
						++count;

						//加入到Map里面
						sameLevelAreaMap[area]=entry;
					}
					else
					{
						printf("entry already in matches... ignoring");
					}
				}

			}
			else
			{
				printf("不合法的请求查找路由表中Link bundle dst:%s,sameAreaLevel：%d",bundle->dest().str().c_str(),sameAreaLevel);
			}
		}
    }
	route_table_->lock()->unlock();
	return count;
}


bool GeoHistoryRouter::canDirectDelivery(Bundle *bundle)
{
    RouteEntryVec::const_iterator iter;
    for (iter = route_table_->route_table()->begin();
         iter != route_table_->route_table()->end(); ++iter)
    {
        RouteEntry* entry = *iter;
        if (entry->dest_pattern().match(bundle->dest()))
        {
			if(	entry->link()->state() ==Link::OPEN ||
					entry->link()->state() == Link::AVAILABLE )
			{
				printf("遇到了目的节点，直接将bundle发送给目的节点%s",entry->dest_pattern().str().c_str());

				DeferredList* dl = deferred_list(entry->link());

				if (dl == 0)
					continue;

				if (dl->list()->contains(bundle)) {
					log_debug("route_bundle bundle %d: "
	                      "ignoring link *%p since already deferred",
	                      bundle->bundleid(), entry->link().object());
					continue;
				}

				// because there may be bundles that already have deferred
				// transmission on the link, we first call check_next_hop to
				// get them into the queue before trying to route the new
				// arrival, otherwise it might leapfrog the other deferred
				// bundles
				check_next_hop(entry->link());

				if (!fwd_to_nexthop(bundle, *iter)) {
					continue;
				}
				printf("直接转发给目的节点完成:dst(%s),copyNum(%d),bundle_%d,目的区域：%d",
					bundle->dest().str().c_str(),bundle->getDeliverBundleNum(),bundle->bundleid(),bundle->getAreaId(bundle->getAreaSize()));
				//可以删除bundle了
				bundle->geoRouterTransmmited=true;
				return true;
			}
        }
    }
	return false;
}

int  GeoHistoryRouter::route_neighbourArea_bundle(Bundle *bundle)
{
	if(bundle->getBundleType()!=Bundle::NEI_AREA_BUNDLE)
	{
		printf("bundle send to %s is not the NEI_AREA_BUNDLE,and it's excute in route_neighbourArea_bundle function");
		return -1;
	}

	RouteEntryVec matches;
    RouteEntryVec::iterator iter;

	printf("向指定邻居发送各个Area的频率信息");
	map<Arearef,RouteEntryref> map_nouse;
	//进行特定路由转发
	get_matching_RouteEntryVec(bundle, matches, RouteType::Neighbour,map_nouse);

    sort_routes(bundle, &matches);

    unsigned int count = 0;
    for (iter = matches.begin(); iter != matches.end(); ++iter)
    {
        RouteEntry* route = *iter;
        log_debug("checking route entry %p link %s (%p)",
                  *iter, route->link()->name(), route->link().object());

        if (! should_fwd(bundle, *iter)) {
            continue;
        }

        DeferredList* dl = deferred_list(route->link());

        if (dl == 0)
          continue;

        if (dl->list()->contains(bundle)) {
            log_debug("route_bundle bundle %d: "
                      "ignoring link *%p since already deferred",
                      bundle->bundleid(), route->link().object());
            continue;
        }

        // because there may be bundles that already have deferred
        // transmission on the link, we first call check_next_hop to
        // get them into the queue before trying to route the new
        // arrival, otherwise it might leapfrog the other deferred
        // bundles
        check_next_hop(route->link());

        if (!fwd_to_nexthop(bundle, *iter)) {
            continue;
        }

        ++count;
    }

    log_debug("route_bundle bundle id %d: forwarded on %u links",
              bundle->bundleid(), count);
    return count;
}


void GeoHistoryRouter::handle_routeAllBundle()
{
	 oasys::ScopeLock l(pending_bundles_->lock(),
	                       "TableBasedRouter::reroute_all_bundles");

	 log_debug("reroute_all_bundles... %zu bundles on pending list",
	              pending_bundles_->size());

	  // XXX/demmer this should cancel previous scheduled transmissions
	  // if any decisions have changed

	  BundleList::iterator iter;
	  for (iter = pending_bundles_->begin();
	        iter != pending_bundles_->end();
	        ++iter)
	  {
		  Bundle *b=*iter;
			//对所有本路由算法要发送的bundle进行重新route
		  if(b->getBundleType()!=Bundle::DATA_BUNDLE)
		  {
				continue;
		  }
	      route_bundle(*iter);
	  }

}


//////////////////针对收到要发送的bundle，需要转发的bundle，邻居交互信息的bundle的处理
	void GeoHistoryRouter::handle_bundle_received(BundleReceivedEvent *event)
	{
		cout<< "GeohistoryRouter收到一个BundleReceivedEvent"<<endl;
		cout<<"!"<<event->bundleref_.object()->getBundleType()<<endl;
		cout<<"!"<<event->bundleref_.object()->source().str()<<endl;
		cout<<"!"<<event->bundleref_.object()->dest().str()<<endl;
		cout<<"!"<<event->source_<<endl;

		//int length=event->bundleref_.object()->payload().length();
		//u_char *buf=new u_char[length];
		//event->bundleref_.object()->payload().read_data(0,length,buf);

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
				areaInfoQueue.push(routeAllBundle);

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


	/**
	 * 提取bundle的唯一标识字符串
	 * @param bundle
	 * @return
	 */
	string  GeoHistoryRouter::StringOfBundle(Bundle *bundle)
	{
		//bundle转关键字
		string bundlestr;
		char c[20];

		sprintf(c,"%d",bundle->bundleid());
		bundlestr.append(c);
		bundlestr.append("#");

		bundlestr.append(bundle->dest().str());
		bundlestr.append("#");

		bundlestr.append(bundle->source().str());
		bundlestr.append("#");

		sprintf(c,"%d",bundle->creation_ts().seconds_);
		bundlestr.append(c);
		bundlestr.append("#");

		sprintf(c,"%d",bundle->creation_ts().seqno_);
		bundlestr.append(c);
		bundlestr.append("#");

		sprintf(c,"%b",bundle->is_fragment());
		bundlestr.append(c);
		bundlestr.append("#");

		sprintf(c,"%d",bundle->frag_offset());
		bundlestr.append(c);
		bundlestr.append("#");

		sprintf(c,"%b",bundle->orig_length());
		bundlestr.append(c);

		return bundlestr;
	}

}
