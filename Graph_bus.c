
//vertex_id lưu id được cấp phát cho vertex mới nhất được thêm vào; id được cấp phát bắt đầu từ 0; tương tự cho bus_id

//ERROR 
//CHÚ Ý LỖI LỚN: CÁC BTREE ĐƯỢC KHAI BÁO MÀ CHƯA ĐƯỢC MALLOC THÌ CHỈ LÀ CON TRỎ THÔI, KO CẦN FREE, NẾU FREE LÀ FREE DỮ LIỆU TRƯỚC ĐÓ LUÔN. lỖI NÀY TÌM MÁT 1 TIẾNG

//LỖI SAI MẤT GẦN NỬA NGÀY ĐỂ SỬA: TRONG CẤU TRÚC DIJKSTRA MÌNH KHAI BÁO THIẾU SỐ LƯỢNG CỦA BUS_NUM LÀ 100 MÀ THỰC TẾ LẠI CÓ HƠN, KHI MÌNH UDNGF BUS_ID LÀ HƠN 100 THÌ NÓ LẤN SANG CÁC VÙNG NHỚ CỦA CÁC BIẾN KHÁC TRONG CÙNG CẤU TRÚC DIJKSTRA !!!!!!!!! ĐÂY CÓ LẼ LÀ ĐẶC ĐIỂM CỦA STRUCT !NHỚ!!!!!!!!!!!!!!!!!!!!!

//xóa bằng máy sẽ làm cho kết thúc thay vì \n thì là  \n, rất khó xử lí, thôi làm chay vậy, hoặc là tìm một phương án khác

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jrb.h"
#include"jval.h"
#define INFINITIVE_VALUE 1000
#define bus_length 10
#define bus_num 130
#define ben_length 200
#define ben_num 1400
#define shortest_ben_num 100

typedef struct
{
  JRB edges;
  JRB vertices;
  JRB bus;
}Graph;

Graph createGraph();

void addVertex(Graph graph, char*name,int *vertex_id);
//thêm một node đỉnh vào trong cây JRB đỉnh, gồm khóa id và tên đỉnh name
int getVertexID(Graph graph, char*name);
char* getVertexName(Graph graph, int v_id);
char* getBusName(Graph graph, int b_id);

void addBus(Graph graph, char*name,int *bus_id);
int getBusID(Graph graph, char*name);

void addEdge(Graph graph, int id_vstart, int id_vfinish, int bus_id);//từ node đỉnh có id=v1, tạo 1 node con truyền vào đó id=v2 và giá trị đường đi từ đỉnh đến v2, tương tự với đỉnh id=v2

JRB getEdgeValue(Graph graph,  int id_vstart, int id_vfinish);

int in_degree   (Graph graph, int v_id, int* v_id_lilst);
int out_degree(Graph graph, int v_id, int* v_id_lilst);
void bus_pass(Graph graph, int v_id, int * bus_flag);

char* jrb_find_str_pre(JRB tree);
char* jrb_find_str_onePart(JRB tree);

typedef struct
{
  int visit;
  double cur_distance;
  int previous_vid[shortest_ben_num];
  int count_pre_vid;//chay tu 1
}DIJKSTRA_NODE;

void shortestPath(Graph graph, double *distance_s_f,  int id_vstart, int id_vfinish,DIJKSTRA_NODE *dijikstra_node,int  vertex_id);
//USE DIJICTRA ALGORITHM

void dropGraph(Graph graph);

void read(Graph graph, int * vertex_id, int *bus_id);

void menu();
int luachon();

int check_exist(Graph graph, char*name)
{
//chuẩn hóa lại dấu cách, ... của địa điểm nhập vào + viết hoa hay viết thường đều tra được********************************
  if(getVertexID(graph, name)!=INFINITIVE_VALUE)
    return 0; else return 1;
}

//BỘ KIỂU DỮ LIỆU CHO GET SHORTEST PATH BEGIN ***********************
typedef struct path_node
{
  int v_id;
  int bus[30];//nếu để ở đây là 100 sẽ gây ra vượt quá bộ nhớ
}pathNode;

typedef struct
{
  pathNode*nodePointer;
  int minChange;
}pathNodeArray;

typedef struct
{
  pathNodeArray*nodePointer;
}pathArray;
//BỘ KIỂU DỮ LIỆU CHO GET SHORTEST PATH END ***********************

//dùng truy vết ngược
void get_shortest_path(Graph graph, DIJKSTRA_NODE*dijikstra_node, int vid, pathNodeArray*path,  pathArray*list_path, int *listPathCount, int nodePathCount,int *min_change);

int main()
{
  Graph graph = createGraph();
  int vertex_id=-1;//chú ý vertex cho chạy từ 0
  int bus_id=-1;

  read(graph,&vertex_id,&bus_id);

  int id_vstart, id_vfinish;
  int v_id_lilst[vertex_id+1];
  int bus_flag[bus_id+1];
  DIJKSTRA_NODE dijikstra_node[vertex_id+1];
  //những thứ như mảng hay struc không thể khai báo trong switch
  int min_change=INFINITIVE_VALUE;
  double distance_s_f;
  int listPathCount=-1;
  pathArray *list_path=(pathArray*)malloc(sizeof(pathArray));

  int choice;
  do
    {
      menu();
      choice = luachon();

      if(choice!=6)
        {
          printf("Gợi ý nhập:\n");
          printf("Nhập vào kí tự '*' để được gợi ý từ có phần đầu yêu cầu\n");
          printf("Nhập vào kí tự '#' để được gợi ý từ chứa phần yêu cầu\n");
          printf("\n******************************\n\n");
        }

      switch(choice)
        {
        case 1:
          printf("Mời bạn nhập vào tên địa điểm muốn kiểm tra: ");
          char ben_find[ben_length];

          scanf("%[^\n]s",ben_find);
          while(getchar()!='\n');

          if(check_exist(graph,ben_find)==0) printf("\n\t---------Địa điểm vừa nhập có tồn tại---------\n");
          else printf("\n\t---------Địa điểm vừa nhập không tồn tại---------\n");
          printf("\n******************************\n\n");
          break;

        case 2:
          printf("Ban da chon chuc nang 2:\n\n");

         char place1[ben_length];
         do
           {
             printf("Mời bạn nhập vào địa điểm 1: ");
             scanf("%[^\n]s",place1);
             while(getchar()!='\n');

             if(place1[0]=='*')
               strcpy(place1,jrb_find_str_pre(graph.vertices));

             if(place1[0]=='#')
               strcpy(place1,jrb_find_str_onePart(graph.vertices));
           }while(place1[0]=='\0');

          if(check_exist(graph,place1)==1)
            {
              printf("\n\t---------Địa điểm 1 không tồn tại---------\n");
              printf("\n******************************\n\n");
              break;
            }

          char place2[ben_length];
          do
            {
              printf("Mời bạn nhập vào địa điểm 2: ");
              scanf("%[^\n]s",place2);
              while(getchar()!='\n');

              if(place2[0]=='*')
                strcpy(place2,jrb_find_str_pre(graph.vertices));

              if(place2[0]=='#')
                strcpy(place2,jrb_find_str_onePart(graph.vertices));
            }while(place2[0]=='\0');

          if(check_exist(graph,place2)==1)
            {
              printf("\n\t---------Địa điểm 2 không tồn tại---------\n");
              printf("\n******************************\n\n");
              break;
            }

            if((getEdgeValue(graph, getVertexID(graph, place1), getVertexID(graph, place2))!=NULL)||(getEdgeValue(graph,  getVertexID(graph, place2), getVertexID(graph, place1))!=NULL))
              printf("\n\t---------Tồn tại đường đi trực tiếp giữa hai địa điểm vưa nhập---------\n");
            else printf("\n\t---------Không tồn tại đường đi trực tiếp giữa hai địa điểm vừa nhập---------\n");

            printf("\n******************************\n\n");
          break;

        case 3:
          printf("Ban da chon chuc nang 3:\n\n");

          char place[ben_length];
          do
            {
              printf("Mời bạn nhập vào địa điểm: ");
              scanf("%[^\n]s",place);
              while(getchar()!='\n');
            }while(place[0]=='\0');

          if(place[0]=='*')
            strcpy(place,jrb_find_str_pre(graph.vertices));

          if(place[0]=='#')
            strcpy(place,jrb_find_str_onePart(graph.vertices));

          if(check_exist(graph,place)==1)
            {
              printf("\n\t---------Địa điểm nhập vào không tồn tại---------\n");
              printf("\n******************************\n\n");
              break;
            }

          int total;
          total=out_degree(graph, getVertexID(graph,place), v_id_lilst);
          printf("\n");
          printf("Các địa điểm có đường đi tới trực tiếp từ  \"%s\":\n\n",place);
          for(int i=1;i<=total;i++)
            printf("-%s\n",getVertexName(graph,v_id_lilst[i]));

          printf("\n");

          total=in_degree(graph, getVertexID(graph,place), v_id_lilst);
          printf("Các địa điểm có thể trực tiếp đi đến \"%s\" :\n\n",place);
          for(int i=1;i<=total;i++)
            printf("-%s\n",getVertexName(graph,v_id_lilst[i]));

          printf("\n******************************\n\n");
          break;

        case 4:
          printf("Ban da chon chuc nang 4:\n\n");

          char place_bus[ben_length];
          do
            {
              printf("Mời bạn nhập vào địa điểm: ");
              scanf("%[^\n]s",place_bus);
              while(getchar()!='\n');

              if(place_bus[0]=='*')
                strcpy(place_bus,jrb_find_str_pre(graph.vertices));

              if(place_bus[0]=='#')
                strcpy(place_bus,jrb_find_str_onePart(graph.vertices));
            }while(place_bus[0]=='\0');


          if(check_exist(graph,place_bus)==1)
            {
              printf("\n\t---------Địa điểm nhập không tồn tại---------\n");
              printf("\n******************************\n\n");
              break;
            }

          for(int i=0;i<=bus_id;i++) bus_flag[i]=-1;

          bus_pass(graph,getVertexID(graph,place_bus) , bus_flag);

          printf("\n");
          for(int i=0;i<=bus_id;i++)
            if(bus_flag[i]==1) printf("Tuyến  bus %s\n",getBusName(graph,i));

          printf("\n******************************\n\n");
          break;

        case 5:
          printf("Ban da chon chuc nam 5:\n\n");

          char place_start[ben_length];
          do
            {
              printf("Mời bạn nhập vào địa điểm xuất phát: ");
              scanf("%[^\n]s",place_start);
              while(getchar()!='\n');

              if(place_start[0]=='*')
                strcpy(place_start,jrb_find_str_pre(graph.vertices));

              if(place_start[0]=='#')
                strcpy(place_start,jrb_find_str_onePart(graph.vertices));
            }while(place_start[0]=='\0');

          if(check_exist(graph,place_start)==1)
            {
              printf("\n\t---------Điểm xuất phát không tồn tại---------\n");
              printf("\n******************************\n");
              break;
            }

          char place_finish[ben_length];
          do
            {
              printf("Mời bạn nhập vào địa điểm đến: ");
              scanf("%[^\n]s",place_finish);
              while(getchar()!='\n');

              if(place_finish[0]=='*')
                strcpy(place_finish,jrb_find_str_pre(graph.vertices));

              if(place_finish[0]=='#')
                strcpy(place_finish,jrb_find_str_onePart(graph.vertices));
            }while(place_finish[0]=='\0');

          if(check_exist(graph,place_finish)==1)
            {
              printf("\n\t---------Điểm đến không tồn tại---------\n");
              printf("\n******************************\n");
              break;
            }

          id_vstart = getVertexID(graph,place_start);
          id_vfinish= getVertexID(graph,place_finish);

          if(id_vstart==id_vfinish)
            {
              printf("\n\t---------Bạn đã ở nơi cần đến!---------\n");
              printf("\n******************************\n");
              break;
            }

          printf("\n******************************\n\n");

          distance_s_f=0;
          shortestPath(graph,&distance_s_f, id_vstart, id_vfinish, dijikstra_node,vertex_id);

          listPathCount=-1;
          min_change=INFINITIVE_VALUE;

          list_path->nodePointer=NULL;
          get_shortest_path(graph, dijikstra_node, id_vfinish,NULL,  list_path, &listPathCount, -1, &min_change);

          if(distance_s_f!=-1)
            {
              printf("\nKhoảng cách di chuyển tối ưu : %.2lf\n", distance_s_f);
              printf("\nSố đường đi có khoảng cách di chuyển tối ưu: %d\n",listPathCount+1);
      }
          printf("\nVới khoảng cách di chuyển tối ưu, số lần chuyển buyt tối ưu là : %d\n",min_change);

          int count_minMax=0;
          for(int i=0;i<=listPathCount;i++)
            if(list_path->nodePointer[i].minChange==min_change)
              count_minMax=count_minMax+1;

          printf("\n");
          printf("Số tuyến đường có khoảng cách di chuyển tối ưu cùng với số lần chuyển tuyến xe buyt là tối ưu: %d\n\n",count_minMax);
          for(int i=0;i<=listPathCount;i++)
            if(list_path->nodePointer[i].minChange==min_change)
              {
                printf("\t");
                for(int j=0;j<((int)distance_s_f+1)-1;j++)
                 {
                   printf("%s --(",getVertexName( graph, list_path->nodePointer[i].nodePointer[j].v_id));

                   for(int p=0;p<30;p++)
                      if(list_path->nodePointer[i].nodePointer[j+1].bus[p]!=-1)
                         printf("%s.",getBusName( graph, list_path->nodePointer[i].nodePointer[j+1].bus[p]));
                     
                   printf(")--> ");
                  }
                printf("%s",getVertexName( graph, list_path->nodePointer[i].nodePointer[(int)distance_s_f].v_id));
                printf("\n");
              }
          printf("\n******************************\n");
          break;

        default:
          printf("Cảm ơn vì đã sử dụng tôi!\nXin chào và hẹn gặp lại!\n");
          printf("\n******************************\n");
          break;
        }
    } while(choice != 6);
//NOTE: printf("%.2lf\n",getEdgeWeight(g, 6,3));//node lại ở đây weight dùng double - tức là %lf, dùng %d in ra sẽ bị sai
  dropGraph(graph);

  return 0;
}

Graph createGraph()
{
Graph graph;
graph.edges=make_jrb();//graph.edges chỉ đến rood của cây JRB cạnh
graph.vertices=make_jrb();//graph.vertices chỉ đến rood của cây JRB đỉnh
 graph.bus=make_jrb();
  return graph;
}

void addVertex(Graph graph, char*name,int *vertex_id)
{
  if(jrb_find_str(graph.vertices,name)==NULL)//nếu vertex chưa tồn tại
    {
      *vertex_id=*vertex_id+1;//set new vetex's id
      jrb_insert_str(graph.vertices,strdup(name),new_jval_i(*vertex_id));
      //do nếu truyền name thẳng vào mà name là một con trỏ coppy con trỏ trỏ vào vùng dữ liệu trước đó, vùng dữ liệu này mà thay đổi thì sẽ gây thay đổi không mong muốn trong vertex
      JRB newEdgesTree=make_jrb();
      jrb_insert_int(graph.edges,*vertex_id, new_jval_v(newEdgesTree));
    }
  return;
}

//extern Jval new_jval_s(char *);
//bắt buộc lưu vào trong jrb phải dùng kiểu jval 
//new_jval_s(...); tạo ra một biến kiểu jval với dạng 1 con trỏ char* trỏ đến vùng nhớ của ...
//hàm strdup(...) tạo ra một vùng nhớ vừa đủ để coppy ... vào vùng nhớ đó

int getVertexID(Graph graph, char*name)
{
 JRB vertex=jrb_find_str(graph.vertices,name);
 if(vertex==NULL) return INFINITIVE_VALUE;
 else return jval_i(vertex->val);
}

char* getVertexName(Graph graph, int v_id)
{
  JRB pointer;
  jrb_traverse(pointer,graph.vertices)
    {
      if(jval_i(pointer->val)==v_id)
        return jval_s(pointer->key);
    }
}

void addBus(Graph graph, char*name,int *bus_id)
{
  if(jrb_find_str(graph.bus,name)==NULL)
    {
      *bus_id=*bus_id+1;
      jrb_insert_str(graph.bus,strdup(name),new_jval_i(*bus_id));
    }
  return;
}

int getBusID(Graph graph, char*name)
{
 JRB bus=jrb_find_str(graph.bus,name);
 if(bus==NULL) return INFINITIVE_VALUE;
 else return jval_i(bus->val);
}

char* getBusName(Graph graph, int b_id)
{
  JRB pointer;
  jrb_traverse(pointer,graph.bus)
    {
      if(jval_i(pointer->val)==b_id)
        return jval_s(pointer->key);
    }
}

void addEdge(Graph graph, int id_vstart, int id_vfinish, int bus_id)
{
  //check
  //if(id_vstart==0) printf("Begin:%d end:%d - %d\n",id_vstart,id_vfinish, bus_id);
  JRB startVertex=jrb_find_int(graph.edges,id_vstart);
  if(startVertex==NULL)
    {
      printf("vertex start didn't exist\n");
      return;
    }

  JRB finishVertex=jrb_find_int(graph.edges,id_vfinish);
  if(finishVertex==NULL)
    {
      printf("vertex finish didn't exist\n");
      return;
    }

  JRB rootEdgeTreeStartVertex=(JRB)jval_v(startVertex->val);
  JRB vfinishNode=jrb_find_int(rootEdgeTreeStartVertex,id_vfinish);

  if(vfinishNode==NULL)
    {
      JRB bus=make_jrb();
      jrb_insert_int(bus,bus_id,new_jval_i(1));
      jrb_insert_int(rootEdgeTreeStartVertex,id_vfinish,new_jval_v(bus));
    }
  else
    {
      JRB bus=(JRB)jval_v(vfinishNode->val);
      if(jrb_find_int(bus,bus_id)==NULL)
        jrb_insert_int(bus,bus_id,new_jval_i(1));
      //check
      /*if(id_vfinish==0)
        {
          JRB pointer;
          jrb_traverse(pointer, bus)
            {printf("%d\n",jval_i(pointer->key));}
            }*/
    }

  return;
}

JRB getEdgeValue(Graph graph,  int id_vstart, int id_vfinish)
{
  // if(id_vfinish==160)
  //if(id_vstart==getVertexID(graph,"Tran Dang Ninh"))
  //  printf("%s  -------- check error 2\n",getVertexName(graph,id_vstart));
  JRB startVertex=jrb_find_int(graph.edges,id_vstart);
  if(startVertex==NULL) return NULL;

  JRB rootEdgeTree=(JRB)jval_v(startVertex->val);

  JRB node_vfinish=jrb_find_int(rootEdgeTree,id_vfinish);
  if(node_vfinish==NULL)
    { return NULL;}

  return (JRB) jval_v(node_vfinish->val);
}

//find all vertex have path to vertex X
int in_degree(Graph graph, int v_id, int* v_id_lilst)
{
  JRB pointer;
  JRB vertex_treeEdge;
  int total = 0;

  jrb_traverse(pointer, graph.edges)
    {
      vertex_treeEdge= (JRB) jval_v(pointer->val);
      if (jrb_find_int(vertex_treeEdge, v_id)!=NULL)
        {
          total++;
          v_id_lilst[total] = jval_i(pointer->key);
        }
    }
  return total;
}

//find all vertex that X have path to
int out_degree(Graph graph, int v_id, int* v_id_lilst)
{
  JRB pointer;
  JRB vertex=jrb_find_int(graph.edges,v_id);
  JRB vertex_treeEdge= (JRB) jval_v(vertex->val);
  int total = 0;

  jrb_traverse(pointer, vertex_treeEdge)
    {
      total++;
      v_id_lilst[total] = jval_i(pointer->key);
    }
  return total;
}

void bus_pass(Graph graph, int v_id, int * bus_flag)
{
  JRB pointer;
  JRB vertex_treeEdge;

  //bus go to place
  jrb_traverse(pointer, graph.edges)
    {
      vertex_treeEdge= (JRB) jval_v(pointer->val);

      if (jrb_find_int(vertex_treeEdge, v_id)!=NULL)
        {
          JRB bus_tree=(JRB)jval_v(jrb_find_int(vertex_treeEdge, v_id)->val);
          JRB bus_pointer;
          jrb_traverse(bus_pointer, bus_tree)
           {
             //check
              //printf("%d\n",jval_i(bus_pointer->key));
              bus_flag[jval_i(bus_pointer->key)] =1 ;
           }
        }
    }

  //bus start from place
  JRB vertex=jrb_find_int(graph.edges,v_id);
  vertex_treeEdge= (JRB) jval_v(vertex->val);
  jrb_traverse(pointer, vertex_treeEdge)
    {
      JRB bus_tree=(JRB)jval_v(pointer->val);
      JRB bus_pointer;
      jrb_traverse(bus_pointer, bus_tree)
        {bus_flag[jval_i(bus_pointer->key)] =1 ;}
    }
  return;
}

 
//TÌM KEY CÓ PHẦN ĐẦU GIỐNG INPUT BEGIN****************************
char* jrb_find_str_pre(JRB tree)
{
  printf("Mời bạn nhập vào key (chương trình sẽ đưa ra tối đai 15 gợi ý): ");
  char key[ben_length];
  scanf("%[^\n]s",key);
  while(getchar()!='\n');

  char**preSimilar=NULL;
  int length=strlen(key);
  char *str_tmp;
  JRB pointer;
  int count=0;

  jrb_traverse(pointer,tree)
    {
      str_tmp=strdup(jval_s(pointer->key));
      str_tmp[length]='\0';
      if(cmp_koPhanBietChuHoaChuThuong(key,str_tmp)==0)
        {
          count=count+1;
          if(preSimilar==NULL)
            preSimilar=(char**)realloc(NULL,count*sizeof(char*));
          else
            preSimilar=(char**)realloc(preSimilar,count*sizeof(char*));
          preSimilar[count-1]=strdup(jval_s(pointer->key));
          if(count>=15) break;
        }
    }

  printf("\n");
  for(int i=0;i<count;i++)
    printf("%d) %s\n", i+1, preSimilar[i]);

  printf("\n");
  printf("Bạn có muốn chọn key bên dưới không? [Y/N]: ");
  char choice[257];
  scanf("%[^\n]s",choice);
  while(getchar()!='\n');

  while((strcmp(choice,"Y")!=0)&&(strcmp(choice,"N")!=0))
    {
      printf("Bạn đã nhập sai!\n");
      printf("Mời bạn nhập lại: ");
      scanf("%[^\n]s",choice);
      while(getchar()!='\n');
    }

  if(strcmp(choice,"Y")==0)
    {
      printf("\n");
      printf("Mời bạn nhập vào số của key: ");

      scanf("%[^\n]s",choice);
      while(getchar()!='\n');

      int check_num=0;
      for(int i=0;i<(int)strlen(choice);i++)
        if((choice[i]>'9')||(choice[i]<'0'))
          {
            check_num=1;
            break;
          }

      while( (check_num==1)||(atoi(choice)>15))
        {
          printf("Bạn đã nhập sai!\n");
          printf("Mời bạn nhập lại: ");
          scanf("%[^\n]s",choice);
          while(getchar()!='\n');

          check_num=0;
          for(int i=0;i<(int)strlen(choice);i++)
            if((choice[i]>'9')||(choice[i]<'0'))
              {
                check_num=1;
                break;
              }
        }

      char *choice_key_tmp=strdup(preSimilar[atoi(choice)-1]);
      printf("\n");

      for(int i=0;i<count;i++) free(preSimilar[i]);
      free(preSimilar);
      free(str_tmp);

      return choice_key_tmp;
    }

  printf("\n");
  for(int i=0;i<count;i++) free(preSimilar[i]);
  free(preSimilar);
  free(str_tmp);

  char null1[1];
  null1[0]='\0';
  char*null=null1;
  return null;
}
//TÌM KEY CÓ PHẦN ĐẦU GIỐNG INPUT END****************************

//TÌM KEY CÓ INPUT BÊN TRONG BEGIN****************************
char* jrb_find_str_onePart(JRB tree)
{
  char**key=NULL;
  int count_key=0;

  printf("Mời bạn nhập vào các key - nhập '#S' để kết thúc (chương trình sẽ đưa ra tối đai 15 gợi ý): ");

  printf("\n\n");
  char key_tmp[ben_length];
  do
    {
      printf("Key %d: ",count_key+1);
      scanf("%[^\n]s",key_tmp);
      while(getchar()!='\n');
      if(strcmp(key_tmp,"#S")!=0)
        {
          count_key=count_key+1;
          if(key==NULL)
            key=(char**)realloc(NULL,count_key*sizeof(char*));
          else
            key=(char**)realloc(key,count_key*sizeof(char*));

          for(int p=0;p<(int)strlen(key_tmp);p++)
            if(('A'<=key_tmp[p])&&(key_tmp[p]<='Z')) key_tmp[p]=key_tmp[p]+32;

          key[count_key-1]=strdup(key_tmp);
        }
    }while(strcmp(key_tmp,"#S")!=0);

  char**preSimilar=NULL;
  char *str_tmp;
  JRB pointer;
  int count=0;

  jrb_traverse(pointer,tree)
    {
      str_tmp=strdup(jval_s(pointer->key));

      for(int p=0;p<(int)strlen(str_tmp);p++)
        if(('A'<=str_tmp[p])&&(str_tmp[p]<='Z')) str_tmp[p]=str_tmp[p]+32;

      int flag=0;
      for(int i=0;i<count_key;i++)
        {
          if(strstr(str_tmp,key[i])==NULL)
            {
              flag=1;
              break;
            }
        }

      if(flag!=1)
        {
          count=count+1;
          if(preSimilar==NULL)
            preSimilar=(char**)realloc(NULL,count*sizeof(char*));
          else
            preSimilar=(char**)realloc(preSimilar,count*sizeof(char*));
          preSimilar[count-1]=strdup(jval_s(pointer->key));
          if(count>=15) break;
        }
    }

  printf("\n");
  for(int i=0;i<count;i++)
    printf("%d) %s\n", i+1, preSimilar[i]);

  printf("\n");
  printf("Bạn có muốn chọn key bên dưới không? [Y/N]: ");
  char choice[257];
  scanf("%[^\n]s",choice);
  while(getchar()!='\n');

  while((strcmp(choice,"Y")!=0)&&(strcmp(choice,"N")!=0))
    {
      printf("Bạn đã nhập sai!\n");
      printf("Mời bạn nhập lại: ");
      scanf("%[^\n]s",choice);
      while(getchar()!='\n');
    }

  if(strcmp(choice,"Y")==0)
    {
      printf("\n");
      printf("Mời bạn nhập vào số của key: ");

      scanf("%[^\n]s",choice);
      while(getchar()!='\n');

      int check_num=0;
      for(int i=0;i<(int)strlen(choice);i++)
        if((choice[i]>'9')||(choice[i]<'0'))
          {
            check_num=1;
            break;
          }

      while( (check_num==1)||(atoi(choice)>15))
        {
          printf("Bạn đã nhập sai!\n");
          printf("Mời bạn nhập lại: ");
          scanf("%[^\n]s",choice);
          while(getchar()!='\n');

          check_num=0;
          for(int i=0;i<(int)strlen(choice);i++)
            if((choice[i]>'9')||(choice[i]<'0'))
              {
                check_num=1;
                break;
              }
        }

      char *choice_key_tmp=strdup(preSimilar[atoi(choice)-1]);
      printf("\n");

      for(int i=0;i<count;i++) free(preSimilar[i]);
      free(preSimilar);
      free(str_tmp);
      for(int i=0;i<count_key;i++) free(key[i]);
      free(key);

      return choice_key_tmp;
    }

  printf("\n");
  for(int i=0;i<count;i++) free(preSimilar[i]);
  free(preSimilar);
  free(str_tmp);
  for(int i=0;i<count_key;i++) free(key[i]);
  free(key);

  char null1[1];
  null1[0]='\0';
  char*null=null1;
  return null;
}
//TÌM KEY CÓ INPUT BÊN TRONG END ****************************

/*Hoàn toàn dùng theo thuật toán dijkstra*/
void shortestPath(Graph graph, double *distance_s_f,  int id_vstart, int id_vfinish,DIJKSTRA_NODE *dijikstra_node,int  vertex_id)
{
  int  new_choice_vid;
  int new_choice_vid_tmp;

  double min;

  for(int i=0;i<vertex_id+1;i++)
{
  dijikstra_node[i].count_pre_vid=0;
  dijikstra_node[i].visit =0;
  dijikstra_node[i].cur_distance = 0; 
  for(int i=0;i<vertex_id+1;i++) dijikstra_node[i].count_pre_vid=0;
}
  dijikstra_node[id_vstart].cur_distance = 0;
  dijikstra_node[id_vstart].count_pre_vid=0;
  new_choice_vid=id_vstart;
  dijikstra_node[id_vstart].visit=1;

  min= INFINITIVE_VALUE;

  for (int i=0; i<=vertex_id; i++)
    if(i!=id_vstart)
      {
        if(getEdgeValue(graph,id_vstart,i)==NULL)
          {
            dijikstra_node[i].cur_distance = INFINITIVE_VALUE;
            dijikstra_node[i].count_pre_vid=0;
          }
        else
          {
            dijikstra_node[i].cur_distance=1;
            dijikstra_node[i].count_pre_vid=1;
            dijikstra_node[i].previous_vid[dijikstra_node[i].count_pre_vid-1]= id_vstart;
          }

        if(dijikstra_node[i].cur_distance<min)
          {
            min=dijikstra_node[i].cur_distance;
            new_choice_vid_tmp=i;
          }

        dijikstra_node[i].visit=0;
      }

 //voi dijikstra thì trong thuật toán dù có 2 cái bé nhất như nhau thì có chọn cai nào cũng ra được đường đi ngắn nhất mà thôi, nếu ko đi được thì là do ban đầu đồ thị đã không đi được rồi


  if(min==INFINITIVE_VALUE)//không tính toán tiếp được - no path
    {printf("error1\n");*distance_s_f=  -1; return;}

  new_choice_vid=new_choice_vid_tmp;
  dijikstra_node[new_choice_vid].visit=1;

//printf("new choice: %d\n",new_choice_vid);
  do
    {
      min= INFINITIVE_VALUE;

      for (int i=0; i<=vertex_id; i++)
        {
          if(dijikstra_node[i].visit==0)
            {
              if(getEdgeValue(graph,new_choice_vid,i)!=NULL)
                {

                  double new_distance = dijikstra_node[new_choice_vid].cur_distance+1;

                  if(dijikstra_node[i].cur_distance==new_distance)//nếu mà khoảng cách từ điểm đang được chọn đến điểm i = khoảng cách ngắn nhất hiện tại của i thì thêm nó vào trong xâu những điểm ngay trước điểm i - là những điểm ở cuối của những con đường tạo nên một trong những con đường ngắn nhất đến điểm i ở thời điểm hiện tại 
                    {
                      int flag_check_duplicate=0;
                      for(int check=1;check<=dijikstra_node[i].count_pre_vid;check++)
                        {if(dijikstra_node[i].previous_vid[check-1]==new_choice_vid) flag_check_duplicate=1;}

                      if(flag_check_duplicate==0)
                        {
                          dijikstra_node[i].count_pre_vid=dijikstra_node[i].count_pre_vid+1;
                          dijikstra_node[i].previous_vid[dijikstra_node[i].count_pre_vid-1]=new_choice_vid;
                        }
                    }

                  if(dijikstra_node[i].cur_distance>new_distance)//nếu mà khoảng cách từ điểm được chọn đến điểm i < khoảng cách ngắn nhất hiện tại của điểm i thì ta phải cập nhật lại toàn bộ, các con đường tạo nên khoảng cách ngắn nhất cũ đều được reset lại, không dùng nữa
                    {
                      dijikstra_node[i].count_pre_vid=1;
                      dijikstra_node[i].cur_distance=new_distance;
                      dijikstra_node[i].previous_vid[dijikstra_node[i].count_pre_vid-1]=new_choice_vid;
                    }
                }

              if(dijikstra_node[i].cur_distance<min)
                {
                  min=dijikstra_node[i].cur_distance;
                  new_choice_vid_tmp=i;
                }
            }
        }
      if(min==INFINITIVE_VALUE)//không tính toán tiếp được - no path
        {printf("error2\n");*distance_s_f=  -1; return;}

      new_choice_vid=new_choice_vid_tmp;
      dijikstra_node[new_choice_vid].visit=1;
      
//printf("New choice: %s\n", getVertexName( graph,new_choice_vid));

    }while(dijikstra_node[id_vfinish].cur_distance!=min);
  *distance_s_f=min;
  return ;
}

/*
	Ý tường: với bộ cấu trúc để lưu tuyến đường && realloc ta lọc và lưu riêng rẽ các tuyến đường có khoảng cách tối ưu tìm được
	Với các bến có được ta xem xét xem cách đi xe buýt nào là tối ưu - số lần chuyển bến là ít nhất:
Ta xét từ điểm bắt đầu đến điếm kết thúc, giữa hai điểm sẽ có n tuyến xe buýt để nối hai địa điểm đó. 
	Có 2 trường hợp: với X là một điểm thuộc tuyến đường
	TH1) Tuyến xe buýt dùng để đi đến X có thể dùng để đi đến điểm tiếp theo sau X => ta loại bỏ những tuyến xe buýt không thể nối tiếp còn lại
	TH2) Không tồn tại tuyến xe buýt nối tiếp => chúng ta bắt buộc phải chuyển bến, lúc này bất cứ xe buýt nào dùng được đều có thể dùng 
*/
void get_shortest_path(Graph graph, DIJKSTRA_NODE*dijikstra_node, int vid, pathNodeArray*path,  pathArray*list_path, int *listPathCount, int nodePathCount,int *min_change)
{
/*
	path: dãy các điểm thuộc tuyến đường
	nodePathCount: số điểm đã truy vết được của tuyến đường hiện tại - được lưu trong path 
	pathArray: lưu lại các tuyến đường 
	listPathCount: số tuyến đường đã được lưu lại
	min_change: số lần chuyển xe buýt ít nhất
*/

  nodePathCount=nodePathCount+1;
  if(path==NULL)
    {
      path=(pathNodeArray*)malloc(sizeof(pathNodeArray));
      path->nodePointer= (pathNode*)realloc(NULL,(nodePathCount+1)/*vì ta để dãy bắt đầu từ 0*/*sizeof(pathNode));//Tương tự như với char thì path->nodePointer là một chuỗi các con trỏ trỏ đến những phần tử pathNode
    }
  else path->nodePointer= (pathNode*)realloc(path->nodePointer,(nodePathCount+1)*sizeof(pathNode));
  path->nodePointer[nodePathCount].v_id=vid;

  if(dijikstra_node[vid].count_pre_vid==0)
/*đã đến điểm xuất phát - bắt đầu quá trình lưu lại tuyến và xét số làn chuyển buyt là ít nhất có thể
*/
    {
      /*cấp phát bộ nhớ cho việc lưu trữ tuyến đường mới*/
      *listPathCount=*listPathCount+1;
      if(list_path->nodePointer==NULL)
        list_path->nodePointer=(pathNodeArray*)realloc(NULL,(*listPathCount+1)*sizeof(pathNodeArray));
      else list_path->nodePointer=(pathNodeArray*)realloc(list_path->nodePointer,(*listPathCount+1)*sizeof(pathNodeArray));//vì ta để dãy bắt đầu từ 0

      list_path->nodePointer[*listPathCount].nodePointer=
        (pathNode*)realloc(NULL,(nodePathCount+1)*sizeof(pathNode));

      int change=0;

      for(int i=0;i<=nodePathCount;i++)
        {
          /*bắt đầu quá trình đưa tuyến đường mới vào mảng các tuyến đường - song song với việc thiết lập xe buýt đi qua giữa các điểm sao là ít nhất*/
          list_path->nodePointer[*listPathCount].nodePointer[i]=path->nodePointer[nodePathCount-i];
          for(int p=0;p<30;p++) list_path->nodePointer[*listPathCount].nodePointer[i].bus[p]=-1;

          if(i>0)//bắt đầu đi đến điểm thứ hai
            {
              pathNode* prePosition=&(list_path->nodePointer[*listPathCount].nodePointer[i-1]);
              pathNode* nowPosition=&(list_path->nodePointer[*listPathCount].nodePointer[i]);

              JRB vertex_treeEdge= (JRB) jval_v(jrb_find_int(graph.edges, prePosition->v_id) ->val);
              JRB bus_tree=(JRB)jval_v(jrb_find_int(vertex_treeEdge, nowPosition->v_id)->val);

              JRB bus_pointer;
          
              int bus_count=-1;int flag=0;


              jrb_traverse(bus_pointer, bus_tree)
                {
                  int id_bus=jval_i(bus_pointer->key);
                  bus_count=bus_count+1;
                  nowPosition->bus[bus_count]=id_bus;

                  for(int p=0;p<30;p++) if(prePosition->bus[p]==nowPosition->bus[bus_count]) flag=1;
                }

              if(flag==0)//không tồn tại có chuyến xe buýt nối tiếp, phải đổi xe buýt
                {
                  change=change+1;
                  if(change>*min_change) return;
                }
              else if(flag==1)// => tồn tại xe buyt nối tiếp
                {

                  for(int p=0;p<30;p++)
                    if(prePosition->bus[p]>-1)
                      {
                        int check_del=1;
                        for(int k=0;k<30;k++)
                          if(prePosition->bus[p]==nowPosition->bus[k]) check_del=0;
                        if(check_del==1) prePosition->bus[p]=-1;
                      }

                  for(int p=0;p<30;p++)
                    if(nowPosition->bus[p]>-1)
                      {
                        int check_del=1;
                        for(int k=0;k<30;k++)
                          if(nowPosition->bus[p]==prePosition->bus[k]) check_del=0;
                        if(check_del==1) nowPosition->bus[p]=-1;

                      }
                }


              /*duyệt lại lần nữa - điểm dừng đang xét hiện tại có số buyt dùng là tối ưu nhất -> ta phải xét quy ngược để làm tốt các điểm dừng trước đó trong việc sử dụng buyt*/ 
              for(int preCheck=i;preCheck>1;preCheck--)
                {flag=0;
                  prePosition=&(list_path->nodePointer[*listPathCount].nodePointer[preCheck-1]);
                  nowPosition=&(list_path->nodePointer[*listPathCount].nodePointer[preCheck]);
                  for(int p=0;p<30;p++)
                    if(prePosition->bus[p]>-1)
                      for(int k=0;k<30;k++)
                        if(prePosition->bus[p]==nowPosition->bus[k]) flag=1;

                  if(flag==1)// => tồn tại xe buyt nối tiếp
                      for(int p=0;p<30;p++)
                        if(prePosition->bus[p]>-1)
                          {
                            int check_del=1;
                            for(int k=0;k<30;k++)
                              if(prePosition->bus[p]==nowPosition->bus[k]) check_del=0;
                            if(check_del==1) prePosition->bus[p]=-1;
                          }
                }
            }
        }

      list_path->nodePointer[*listPathCount].minChange=change;

      if(change<*min_change)
        {*min_change=change;}

      return;
    }

  for(int i=1;i<=dijikstra_node[vid].count_pre_vid;i++)
    get_shortest_path(graph,dijikstra_node, dijikstra_node[vid].previous_vid[i-1], path, list_path, listPathCount, nodePathCount, min_change);

  //cho nay chua free path***********************************
  return;
}

void dropGraph(Graph graph)
{
  jrb_free_tree(graph.edges);
  jrb_free_tree(graph.vertices);
  jrb_free_tree(graph.bus);
  return;
}

//READ FILE DATA *****************************************
//CHINHR LAIJ PHAN DOC SAO CHO KHONG DOC CHU MA CHUYEN THANH SO LUON, NHANH HON ***********************
void read(Graph graph, int * vertex_id, int *bus_id)
{
  FILE* file= fopen("bus.txt","r");
  if (file == NULL)
    {
      printf("File input could not open\n");
      return;
	}

  char* ben_chieu_di[50];
  for (int i = 0;i<50;i++)
    ben_chieu_di[i] = (char*)malloc(ben_length*sizeof(char));

  char* ben_chieu_ve[50];
  for (int i = 0;i<50;i++)
    ben_chieu_ve[i] = (char*)malloc(ben_length*sizeof(char));

  fseek(file,0,SEEK_SET);
  char checkEOF=' ';
  while (!feof(file))
	{
      char bus_name[bus_length];
      fscanf(file,"%[^\n]s",bus_name);
      //printf("*********Bus******************\n");
      //printf("-*%s*-\n",bus_name);
      addBus(graph,bus_name,bus_id);

      while (fgetc(file)=='\n');
      fseek(file,-1,SEEK_CUR);

      int ben_count_di=-1;
      char check='\0';
      do{
        ben_count_di=ben_count_di+1;
        fscanf(file,"%[^-.]s",ben_chieu_di[ben_count_di]);
        //printf("-%d:%s-\n",ben_count_di,ben_chieu_di[ben_count_di]);
        addVertex(graph, ben_chieu_di[ben_count_di],vertex_id);
        check=fgetc(file);
      }while(check!='.');

      while (fgetc(file)=='\n');
      fseek(file,-1,SEEK_CUR);

      for(int i=0;i<ben_count_di;i++)
        addEdge(graph, getVertexID(graph,ben_chieu_di[i] ), getVertexID(graph,ben_chieu_di[i+1] ),*bus_id);

      int ben_count_ve=-1;
      check='\0';
      do{
        ben_count_ve=ben_count_ve+1;
        fscanf(file,"%[^-.]s",ben_chieu_ve[ben_count_ve]);
        //printf("-%d:%s-\n",ben_count_ve,ben_chieu_ve[ben_count_ve]);
        addVertex(graph, ben_chieu_ve[ben_count_ve],vertex_id);
        check=fgetc(file);
      }while(check!='.');

      for(int i=0;i<ben_count_ve;i++)
        addEdge(graph, getVertexID(graph,ben_chieu_ve[i] ), getVertexID(graph,ben_chieu_ve[i+1] ), *bus_id);

      while(fgetc(file)!='\n');
      while((checkEOF=fgetc(file))=='\n');

      if(checkEOF!=EOF)
        fseek(file,-1,SEEK_CUR);
    }

  for (int i = 0;i<50;i++) free(ben_chieu_di[i]);
  for (int i = 0;i<50;i++) free(ben_chieu_ve[i]);
  fclose(file);
}

//MENU ******************************************************
void menu()
{
  printf("\nMenu\n\n");
  printf("******************************\n");
  printf("Nhấn phím 1: Kiểm tra một tên địa điểm tại  Hà Nội \n");
  printf("Nhấn phím 2: Kiểm tra xem hai địa điểm có nối trực tiếp với nhau hay không\n");
  printf("Nhấn phím 3: Liệt kê các địa điểm nối với một địa điểm cho trước\n");
  printf("Nhấn phím 4: Các tuyến xe buýt đi qua một địa điểm\n");
  printf("Nhấn phím 5: Đường đi ngắn nhất\n");
  printf("Nhấn phím 6: Thoát \n");
  printf("******************************\n\n");
  return;
}

int luachon()
{
  float choice;
  printf("Mời bạn nhập vào lựa chọn của mình : ");
  char choice_tmp[257];
  scanf("%[^\n]s",choice_tmp);//mình fscanf đã đọc luôn dấu xuống dòng ở cuối dòng rồi, nhưng mà vì nếu mà người dùng nhập nhiều từ, nghĩa là có dấu cách trống, thì mình phải loại bỏ trong buffer 
  while(getchar()!='\n');

  while( strlen(choice_tmp)>1 || '6'<choice_tmp[0] || choice_tmp[0]<'1')
  {
      printf("Bạn đã nhập sai!\n");
      printf("Mời bạn nhập lại: ");
      scanf("%[^\n]s",choice_tmp);
      while(getchar()!='\n');
    }

  choice=(float)atof(choice_tmp);

  printf("\n******************************\n\n");

  return choice;
}
