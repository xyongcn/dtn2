# 在虚拟机上搭建DTN2环境
## 2017/04/20  高睿

### 1.下载内容
git clone https://github.com/xyongcn/dtn2.git

### 2.安装splite3
apt-get update  
apt-get install sqlite3  
apt-get install libsqlite3-dev  

### 3. 安装 libxml2(只有在Osm2Sqlit生成新的地图数据库时才需要，仅运行dtn2时不需要安装,可跳过该步)
在网上下载libxml2压缩文件，或从目录./Resource下解压libxml2-2.6.32.tar.gz，进入目录，执行  
./configure  
make  
make install  

安装后/usr/include/目录下出现libxml，libxml2文件夹即可。  

### 4.编译iaodv
- (1)在终端执行命令，uname -a    
    查看内核版本以及为内核为32位还是64位，确定内核版本后，去/usr/src找到对应的源码  
- (2)进入iaodv目录，即./iaodv/iaodv_sourceCode/iaodv下打开Makefile文件    
	添加ubuntu_KDIR := /usr/src/linux-headers-3.5.0-23-generic  
	(/usr/src/linux-headers-3.5.0-23-generic为ubuntu内核版本的源码，源码版本一定要与ubuntu  
    的内核版本一致)  
- (3)进入终端，进入./iaodv/iaodv_sourceCode/iaodv的目录下，执行    
	export ARCH=x86(内核为64位时，执行)  
	或export ARCH=i386(32位)  
执行	 	
make clean
make
- (4)执行完命令后，会看到该目录下的fbaodv_ubuntu.ko，这就是我们所需要的内核模块
- (5)将该内核模块fbaodv_ubuntu.ko放入到./iaodv目录下

### 5.安装tcl,db
- (1)下载Tcl8.5版本  
http://www.tcl.tk/software/tcltk/download.html  
或者从目录./Resource下获取tcl8.5.19-src.tar.gz  
- (2)解压，进入目录  
cd tcl8.5.18  
cd unix  
- (3)进行编译  
./configure --prefix=/opt/tcl  
make  
make install  

- (1)下载Berkeley DB 5.3版本  
http://www.oracle.com/technetwork/database/database-technologies/berkeleydb/downloads/index-082944.html  
或者从目录./Resource下获取db-5.3.28.NC.tar.gz  
- (2)解压，进入目录  
cd db-5.3.28  
cd build_unix  
- (3)进行编译  
../dist/configure --prefix=/opt/db --enable-cxx --enable-stl --enable-tcl   --with-tcl=/opt/tcl/lib  
make  
make install  

### 6.编译oasys(用原版的oasys，不要用编译过得，也就是./DTN下的oasys.tar.gz)
- (1)进入目录./DTN，将文件夹oasys(编译过)删除，解压oasys.tar.gz  
- (2)进入oasys文件夹  
- (3)生成编译文件  
./configure --with-tcl=/opt/tcl --with-db=/opt/db  
- (4)执行 make clean  
- (5)修改Rules.make  
在加入EXTRA_CFLAGS后加上-fpermissive  
- (6)修改tclcmd/tclreadline.c 将702行  
rl_attempted_completion_function = (CPPFunction *) TclReadlineCompletion;  
改为  
rl_attempted_completion_function = (rl_completion_func_t *) TclReadlineCompletion;  
（来源：http://www.ietf.org/mail-archive/web/dtn-users/current/msg01804.html  ）
- (7)开始编译  
make  

### 7.编译boost
apt-cache search boost  
sudo apt-get install libboost-all-dev  

### 8.编译DTN2
- (1)将DTN2文件夹放置在与oasys同样的位置  
- (2)生成编译文件  
./configure --disable-ecl --disable-edp --with-bpq  
- (3)make clean  
- (4)修改Rules.make  
添加  
EXTRA_CFLAGS	= -fpermissive -lboost_serialization  
EXTRA_CXXFLAGS  = -lboost_serialization  
EXTRA_LDFLAGS	= -lboost_serialization  
- (5)开始编译  
make  
- (6)创建程序运行过程中需要的临时目录  
mkdir /var/dtn  
mkdir /var/dtn/db  
mkdir /var/dtn/bundles  
- (7)将dtn.conf放置于/etc/文件夹下  
- (8)复制Tcl的库文件到系统目录  
cd /opt/tcl/lib  
cp -rf * /usr/lib/  
- (9)复制db的库文件到系统目录  
cd /opt/db/lib  
cp -rf * /usr/lib/  
- (10)首次执行程序，需要初始化数据库  
./daemon/dtnd --init  
- (11)执行程序  
在root权限下执行  ./daemon/dtnd  

### 9.编译
./MapInterface  
./RealSimulator  
./Osm2Sqlite(如果没有安装libxml2，将无法编译，但不影响dtn2运行)  
./ModifyVector  
./NodeName/FindDtnNodeName  
./NodeName/ModifyDtnNodeName  
./NodeName/ModifyNodeTrack  
./ConfigEnvironment/ModifyDTN2_DA  
./ConfigEnvironment/ModifyIaodv.shCom_radius  
./ConfigEnvironment/ModifyIaodv.shMesh_dev  
./CustomMap/WriteArea  
./CustomMap/WriteNode  
./CustomMap/WriteWay  
(1)进入到对应目录后  
(2)执行make clean  
(3)执行make即可  

### 10.适应双eth网卡环境
执行./dtn-control.sh -configEnvironment eth1;(eth1为你想要dtn2进行网络通讯的特定的网卡名)  

# 脚本./dtn-control.sh详细介绍
- 1./dtn-control.sh  脚本帮助
- 2./dtn-control.sh -help  脚本帮助
- 3./dtn-control.sh -iaodv  如果已加载fbaodv_ubuntu.ko模块，则删除该模块，重新加载，如果没有删除，则直接加载fbaodv_ubuntu.ko模块
- 4./dtn-control.sh -dtnNodeName  从文件/etc/dtn.conf中获取dtn2的id
- 5./dtn-control.sh -realSimulator  在后台运行RealSimulator，程序打印的信息在./RealSimulator/RealLog.txt中显示
- 6./dtn-control.sh -mapInterface  在后台运行MapInterface，程序打印的信息在./ MapInterface/MapInLog.txt中显示
- 7./dtn-control.sh -osm2Sqlite  执行Osm2Sqlite程序，生成新的地图数据库sumo.db
- 8./dtn-control.sh -dtn2  在后台运行dtn2，dtn2打印的信息在文件./DTN/DTN2/dtn2.log中显示
- 9./dtn-control.sh -dtn  在前台打开dtn2
- 10./dtn-control.sh -killdtn2   杀死在后台运行的dtn2程序
- 11./dtn-control.sh -killrealSimulator   杀死在后台运行的RealSimulator程序
- 12./dtn-control.sh -killmapInterface   杀死在后台运行的MapInterface程序
- 13./dtn-control.sh -watch  每隔5秒显示结点之间的距离，该距离日志由iaodv提供，该日志为/proc/fbaodv/dis_history
- 14./dtn-control.sh -clearData   删除dtn2对一个特定的轨迹学习的内容，该学习内容在目录/DTN/DTN2/logDocuments下，包括areamoving.txt(每到达一区域的记录的日志)，bundleLog.txt(发送，转发，接收的bundle，包括data_bundle和neigh_bundle)，dataBundleLog.txt(只记录data_bundle的发送转发接收)，historyarea.txt(序列化的移动规律表)，GeohistoryLog.txt(dtn2的日志)，和目录geoHistory_dtn(该目录包含邻居相遇表和邻居的移动规律表)
- 15./dtn-control.sh -clearLog   删除后台运行时dtn2,RealSimulator,MapInterface打印的文件
也就是dtn2.log,MapInLog.txt,RealLog.txt
- 16./dtn-control.sh -sendBundle   利用./DTN/DTN2/apps/dtnsend/dtnsend发送bundle，
格式举例：
./dtn-control.sh -sendBundle "./dtnsend -s dtn://192.168.11.122.gao.com -d 
dtn://192.168.11.125.gao.com -t m -p hello -g 121412"
其中-g 后面跟的是目的区域id
- 17./dtn-control.sh -configEnvironment 配置网络环境，也就是希望dtn2和iaodv利用哪一个网卡进行通信
分为三种情况
- - (1) ./dtn-control.sh -configEnvironment    默认利用所有的网卡
- - (2) ./dtn-control.sh -configEnvironment eth1   dtn2和iaodv都只利用网卡eth1
- - (3) ./dtn-control.sh -configEnvironment eth1 eth2 dtn2和iaodv都只利用两块或两块以上的网卡(这种情况我并没有尝试过，不确信具体的效果)

    该命令修改两处配置文件
    一是/etc/dtn.conf，为了使dtn2利用特定的网卡
- - (1)discovery announce tcp0 disc0 tcp interval=5   默认利用所有的网卡
- - (2)discovery announce tcp0 disc0 tcp interval=5 cl_addr=192.168.11.122(指定网卡的ip)
dtn2只利用网卡eth1,eth1的ip为192.168.11.122
- - (3)discovery announce tcp0 disc0 tcp interval=5 cl_addr=192.168.1.122
  discovery announce tcp0 disc0 tcp interval=5 cl_addr=192.168.11.122
dtn2利用网卡eth0和eth1。eth0的ip为192.168.1.122, eth1的ip为192.168.11.122

    二是修改./iaodv/aodv.sh，为了使iaodv在特定网卡中运行
- - (1)MESH_DEV="mesh_dev="     默认利用所有的网卡
- - (2)MESH_DEV="mesh_dev=eth1"   iaodv只利用网卡eth1
- - (3)MESH_DEV="mesh_dev=eth0/eth1"     iaodv利用网卡eth0和eth1

- 18./dtn-control.sh -renameDevices  重新命名dtn2的名字
格式举例：
./dtn-control.sh -renameDevices dtn://192.168.11.122.gao.com
- 19./dtn-control.sh -modifyNodeTrack   指定结点的轨迹，也就是修改文件
./RealSimulator/nodeInfo.txt中的nodeNo，使得结点可以按照track.txt文件中指定的轨迹序号移动
格式举例:
./dtn-control.sh -modifyNodeTrack 1
- 20./dtn-control.sh -modifyCommDis    指定结点之间的通信范围，也就是修改文件
./iaodv/aodv.sh 中的COM_RADIUS
格式举例:
./dtn-control.sh -modifyCommDis 500
- 21./dtn-control.sh -modifyVector   指定dtn2中的频率向量的类型。dtn2默认使用分钟和小时两种尺度，也可以修改成自己想要的尺度。也就是修改./DTN/DTN2/mydtn.conf文件。修改后，必须要清除dtn2学习的日志和规律，也就是要执行./dtn-control.sh -clearData
格式举例:
./dtn-control.sh -modifyVector minute hour

# 运行DTN2说明
### 1.打开iaodv
./dtn-control.sh -iaodv  
如果已经加载了aodv的内核模块，则先删除该内核模块，再加载./iaodv/fbaodv_ubuntu.ko模块。如果没有加载过，则直接加载  
### 2.打开RealSimulator
- (1)在后台执行RealSimulator  
如果后台已经有RealSimulator在运行，则执行  
./dtn-control.sh -killrealSimulator  
./dtn-control.sh -realSimulator   
如果没有，则直接执行./dtn-control.sh -realSimulator  
- (2) 在前台执行RealSimulator  
打开一个终端，切换成root权限，进入./RealSimulator下目录，执行./RealSimulator  
就可看到RealSimulator打印的信息  
### 3.打开MapInterface
- (1)在后台执行MapInterface  
如果后台已经有MapInterface在运行，则执行  
./dtn-control.sh -killmapInterface  
./dtn-control.sh -mapInterface  
如果没有，则直接执行./dtn-control.sh -mapInterface  
- (2) 在前台执行MapInterface  
打开一个终端，切换成root权限，进入. /MapInterface下目录，执行./ MapInterface  
就可看到MapInterface打印的信息   

### 4.打开dtn2
如果后台运行着dtn2，则执行  
./dtn-control.sh -killdtn2  
如果想要后台运行dtn2，则执行  
./dtn-control.sh -dtn2  
如果想要前台运行dtn2，则执行  
./dtn-control.sh -dtn  

# 换轨迹和地图说明
### 1.换轨迹
- (1)首先根据./Documents/20150618sumo帮助文档，生成适用于当前地图下的轨迹文件track.txt  
- (2)将./RealSimulator目录下的track.txt文件删去，再将新生成的track.txt文件换上  
- (3)执行./dtn-control.sh -modifyNodeTrack 修改想要结点在哪一个轨迹上运行的轨迹号  
- (4)执行./dtn-control.sh   -clearData清除dtn2学习过的日志和规律，因为结点的移动轨迹变化，之前学习的移动轨迹规律不再适用，必须清除  
- (5)如果对通信距离有新的要求，则执行./dtn-control.sh -modifyCommDis，如果没有需要则不执行  
- (6)如果后台已有RealSimulator运行，则执行./dtn-control.sh   -killrealSimulator关闭运行旧的轨迹文件的RealSimulator  
- (7)每次更新完轨迹后，都需要让dtn2单独运行一段时间，为了学习新的移动规律。当学习了新的移动规律后，再进行转发。  

### 2.换地图
- (1)首先生成三个文件，分别是node.xml，way.xml，area.xml  
- -	a.生成node.xml文件  
	进入到./CustomMap/WriteNode目录下，打开test.node.xml文件，根据该				文件的格式进行修改。该文件主要记录地图中所有的点的id和该点的经纬度，			因此只需要修改id,x,y这三项，其余项无需改变。点的id是一个整数。  
	该文件的格式完全仿照sumo生成轨迹时的点文件格式，因此同样可以利				用./CustomMap/utm2gps，将点的相对坐标转换为大地坐标，也就是经纬度
	修改好test.node.xml文件后，执行./WriteNode，便可得到node.xml文件
- - b. 生成way.xml文件  
	进入到./CustomMap/WriteWay目录下，打开test.edg.xml文件，根据该				文件的格式进行修改。该文件主要记录地图中所有边的id和由哪些点组成的，			因此只需要修改id,from,to这三项，其余项无需改变。边的id是一个整数，通			常是由两个点的id组合而成，比如点1，点2，边为12或21。如此命名只是				为了方便记忆，不如此命名也可以，只要是一个整数就行。组成一条边的两个			点的顺序也没有要求。  
	修改test.edg.xml文件后，执行./WriteWay，便可得到way.xml文件
- - c. 生成area.xml文件  
	进入到./CustomMap/WriteArea目录下，打开test.area.xml文件，根据该				文件的格式进行修改。  
	该文件主要记录地图中所有区域的情况，id指区域id,way指组成这个区域的				所有边的id，relation指该区域下一层的子区域的id，node指该区域的中心点			id(无用，可以随便填)，level指区域的层数，最顶层为1，依次类推，按照osm			规定，层数不能超过12。  
	一定要注意的是，一个区域是由多条边组成，当录入边id时，也就是添加多				个way项时，一定要按照顺时针或逆时针的顺序进行录入，否则会出错误。
	区域id的命名规则通常是，从区域最左上角的点开始，顺时针方向，将所有				组成该区域的点id组合起来。该命名方式通常是方便记忆，不遵守也可以。  
	修改好test.area.xml文件后，执行./WriteArea，便可得到area.xml文件    
  

- (2)将生成的node.xml，way.xml，area.xml放入到./Osm2Sqlite/assets目录下，再回		到./Osm2Sqlite目录下，执行./Osm2Sqlite或回到./目录下执行./dtn-control.sh -osm2Sqlite
	如此，在./Osm2Sqlite目录下生成新的sumo.db数据库文件。  
- (3)进入到./MapInterface目录下，删除原有的sumo.db文件，将新的sumo.db文件替换上  
- (4)如果后台有运行MapInterface，则执行./dtn-control.sh -killmapInterface关闭该进程  
- (5)其余的行为和换轨迹的重复，参考换轨迹的步骤  


# 验脚本，应用脚本，运行脚本说明
### changeMap.sh中有4个实验的准备工作
执行./ changeMap.sh会有说明，分别是实验2-1,3层12子区域，4层32子区域-1,4层	32子区域-2这四个实验  
	该脚本用来处理实验的准备工作，包括，换地图，换轨迹，清除dtn2原有的规律数据，	修改轨迹，修改通信距离，以及将之前保存下来的该实验的dtn2的规律数据放入到dtn2	中，如此，dtn2运行时，可以直接转发数据，不必要再进行移动规律的学习。  
	
	需要注意的是，我是按照dtn结点的名字去修改轨迹号和存放dtn2的规律，因此当结点名字与我的dtn名字不同时，需要修改该脚本
	我的dtn结点的名字分别是  
	dtn://192.168.11.122.gao.com  
	dtn://192.168.11.124.gao.com  
	dtn://192.168.11.125.gao.com  


### showApp.sh是运行一个仿照吴竞邦的应用的app
该脚本的主要作用是，源结点将一个负载为json格式的字符串bundle发给目的结点，目的结点一直监听，当目的结点接受到该bundle时，再发送回一个json格式字符串的bundle和一个分在为一个文件的bundle，源结点同样在一直监听，收到这两个bundle后在分别处理。  
三个结点运行的脚本内容不一样，源结点和目的结点的脚本一样，都是运行showApp.sh
，该脚本是将dtn2是在后台运行，并打开监听程序。而中间结点运行showApp_mid.sh
在前台打开dtn2，不执行监听程序  

当在三个结点上执行该应用时，  
- (1).源结点需要修改
- - a./ion/cmd.json  中file，也就是目的结点转发给源结点的文件的路径，saddr源结				点id，daddr目的结点id，darea目的结点所在的区域id,sarea源结点所在的区域id,
- - b.还需要修改json_start_cmd.sh中的SOURCE_ADDR也就是源结点的id
DESTINATION_ADDR，目的结点的id,和AREA，源结点所在的区域id。以及APPCONFIG，	也就是cmd.json的路径
- - c.修改json_cmd_listener.sh中的THIS_ADDR，也就是源结点的名字
- (2).目的结点
修改json_cmd_listener.sh中的THIS_ADDR，也就是目的结点的名字

### opendtn2.sh和opendtn2InBackground.sh是两个运行所有程序的脚本
步骤都为：删除iaodv内核模块，加载iaodv内核模块，杀死RealSimulator后台进程，打开RealSimulator，杀死MapInterface后台进程，打开MapInterface，杀死dtn2后台进程，打开dtn2
两者的区别仅在于opendtn2.sh是在前台打开dtn2，opendtn2InBackground.sh是在后台打开dtn2

# 文件内容简单介绍
- 1.ConfigEnvironment
- - (1)ModifyDTN2_DA    配置dtn2的网络环境，指定特定的网卡
- - (2)ModifyIaodv.shCom_radius    修改结点之间通信范围，也就是修改./iaodv/aodv.sh中	的Com_radius
- - (3)ModifyIaodv.shMesh_dev    配置iaodv的网络环境，指定特定的网卡
- 2.Osm2Sqlite    利用node.xml，way.xml，area.xml三个文件生成地图数据库sumo.db
- 3.CustomMap    
- - (1)WriteArea   生成area.xml
- - (2) WriteNode   生成node.xml
- - (3) WriteWay  生成way.xml
- - (4)gpsTImeConvert    将sumo生成的轨迹文件gps.txt处理变成我们所需的轨迹文件							track.txt
- - (5)utm2gps   将点的相对坐标变换为大地坐标
- 4.MapInterface    查询一个轨迹点经纬度所在的所有层区域
- 5.dtn-test   实验准备数据和实验结果，包括3层8区域的5组实验，3层12区域的1组实	验，和4层32区域的3组实验
- 6.ion   模仿吴竞邦实验的应用所需的脚本
- 7.DTN  dtn2和oasys
- 8.ModifyVector    修改dtn2所需要的频率向量的时间尺度
- 9.RealSimulator    模拟结点在地图上移动
- 10.Documents    包含各种文档素材说明
- 11.NodeName    
- - (1)FindDtnNodeName		查找dtn的名字
- - (2)ModifyDtnNodeName		修改dtn的名字
- - (3)ModifyNodeTrack		修改dtn结点的轨迹
- 12.dtn-control.sh    总控制脚本
- 13.showApp.sh/showApp_mid.sh   打开应用
- 14.changeMap.sh   替换4个实验的准备数据
- 15.dtn.conf    dtn配置文件备份
- 16.opendtn2.sh/opendtn2InBackground.sh    打开实验

