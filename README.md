# dtn2 test
DTN routing algorithm for Galileo by gaorui<br>
<br>
##dtn2 test one
###实验描述：
![](https://github.com/xyongcn/dtn2/raw/master/dtn-test/test1/test1.jpg)
>通信距离500m<br>
>三个静止节点，1节点与2节点相距300m,2节点与3节点相距300m<br>
>1,2节点相连通，2,3节点相连通，1,3节点不连通<br>
###实验结果：
>同时打开三个节点时，1,2节点交换移动表，且连接不再中断；2,3节点交换移动表，连接也不再中断<br>
<br>
##dtn2 test two
###实验描述：
![](https://github.com/xyongcn/dtn2/raw/master/dtn-test/test2/test2.jpg)
>通信距离500m<br>
>2,3节点静止，1节点按长宽都为200m的正方形轨迹不停的运动，平均30s每圈
>2节点与3节点相距400m,3个节点全连通<br>
###实验结果：
>同时打开三个节点时，3个节点两两交换移动表，且连接都不会再中断，1节点一直在2,3节点间摆渡<br>
<br>
##dtn2 test three
###实验描述：
![](https://github.com/xyongcn/dtn2/raw/master/dtn-test/test3/test3.jpg)
>通信距离500m<br>
>2,3节点静止，1节点按长为600m，宽为400m的长方形轨迹不停的运动，平均90s每圈
>2节点与3节点相距1200m,不连通，2与1节点连通时，1节点与3节点不连通;3与1节点连通时，1节点与2节点不连通<br>
###实验结果：
同时打开三个节点时, 1节点一直在2,3节点间摆渡,因此当1,2节点连通时，交换邻居表，不连通时，连接中断，并循环这个过程；2,3节点同种情况<br>
<br>
##dtn2 test four
###实验描述：
![](https://github.com/xyongcn/dtn2/raw/master/dtn-test/test4/test4.jpg)
>通信距离500m<br>
>1,3节点按长为400m，宽为600m的长方形轨迹不停的运动，平均50s每圈；2节点按长为600m，宽为600m的正方形轨迹不停的运动，平均60s每圈；
>2节点与3节点不连通，2与1节点连通时，1节点与3节点不连通;3与1节点连通时，1节点与2节点不连通<br>
###实验结果：
同时打开三个节点时, 1节点一直在2,3节点间摆渡,2,3节点也按自己的轨迹运行。当1,2节点连通时，交换邻居表，不连通时，连接中断，并循环这个过程；2,3节点同种情况<br>
<br>

##iaodv test one
![](https://github.com/xyongcn/dtn2/raw/master/dtn-test/aodv_test/aodv_test.jpg)
>通信距离500m<br>
>2,3节点静止，1节点按长为420m，宽为420m的正方形轨迹不停的运动，平均90s每圈
>2节点与3节点连通；2与1节点连通时，1节点与3节点不连通;3与1节点连通时，1节点与2节点不连通<br>
>由于2,3节点连通，因此当1,2连通，1,3不连通时，1节点ping3节点，可以通过1,2,3三跳ping通三节点
<br>




