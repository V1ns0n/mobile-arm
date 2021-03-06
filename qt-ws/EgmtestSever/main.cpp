#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netdb.h>
#include<stdarg.h>
#include<string.h>

#include "egm.pb.h" // generated by Google protoc.exe

#define SERVER_PORT 6510
#define BUFFER_SIZE 1400
static unsigned int sequenceNumber = 0;
using namespace std;
using namespace abb::egm;

// 返回自系统开机以来的毫秒数（tick）
unsigned long GetTickCount()
{
    struct timespec ts;

     clock_gettime(CLOCK_MONOTONIC, &ts);

     return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

// Create a simple sensor message
void CreateSensorMessage(EgmSensor* pSensorMessage,double pose[])
{
    EgmHeader* header = new EgmHeader();
    header->set_mtype(EgmHeader_MessageType_MSGTYPE_CORRECTION);
    header->set_seqno(sequenceNumber++);
    header->set_tm(GetTickCount());

    pSensorMessage->set_allocated_header(header);

    EgmCartesian *pc = new EgmCartesian();
    pc->set_x(pose[0]);
    pc->set_y(pose[1]);
    pc->set_z(pose[2]);

    EgmQuaternion *pq = new EgmQuaternion();
    pq->set_u0(1.0);
    pq->set_u1(0.0);
    pq->set_u2(0.0);
    pq->set_u3(0.0);

    EgmPose *pcartesian = new EgmPose();
    pcartesian->set_allocated_orient(pq);
    pcartesian->set_allocated_pos(pc);

    EgmPlanned *planned = new EgmPlanned();
    planned->set_allocated_cartesian(pcartesian);

    pSensorMessage->set_allocated_planned(planned);
}

// Display inbound robot message
void DisplayRobotMessage(EgmRobot *pRobotMessage)
{
    if (pRobotMessage->has_header() && pRobotMessage->header().has_seqno() && pRobotMessage->header().has_tm() && pRobotMessage->header().has_mtype())
    {
        printf("SeqNo=%d Tm=%u Type=%d\n", pRobotMessage->header().seqno(), pRobotMessage->header().tm(), pRobotMessage->header().mtype());
    }
    else
    {
        printf("No header\n");
    }
}

int main()
{
 /* 创建UDP套接口 */
 struct sockaddr_in server_addr;
 bzero(&server_addr, sizeof(server_addr));
 server_addr.sin_family = AF_INET;
 server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
 server_addr.sin_port = htons(SERVER_PORT);

 /* 创建socket */
 int server_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
 if(server_socket_fd == -1)
 {
  perror("Create Socket Failed:");
  exit(1);
 }

 /* 绑定套接口 */
 if(-1 == (bind(server_socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))))
 {
  perror("Server Bind Failed:");
  exit(1);
 }

 /* 数据传输 */
  /* 定义一个地址，用于捕获客户端地址 */
  struct sockaddr_in client_addr;
  socklen_t client_addr_length = sizeof(client_addr);

  /* 接收数据 */
  char protoMessage[BUFFER_SIZE];
  bzero(protoMessage, BUFFER_SIZE);
  int n=recvfrom(server_socket_fd, protoMessage, BUFFER_SIZE,0,(struct sockaddr*)&client_addr, &client_addr_length);
  if( n== -1)
  {
   perror("Receive Data Failed:");
   exit(1);
  }

  // deserialize inbound message
   EgmRobot *pRobotMessage = new EgmRobot();
   pRobotMessage->ParseFromArray(protoMessage, n);
   DisplayRobotMessage(pRobotMessage);
   delete pRobotMessage;

  //test code
  /*EgmSensor *pRobotMessage = new EgmSensor();
  pRobotMessage->ParseFromArray(protoMessage, n);
  DisplayRobotMessage(pRobotMessage);
  delete pRobotMessage;*/
   int count=60;
   double pose[3]={100,0,0};
 //while(count>0)
 //{
       string messageBuffer;
       // create and send a sensor message
      EgmSensor *pSensorMessage = new EgmSensor();
      CreateSensorMessage(pSensorMessage,pose);
      pSensorMessage->SerializeToString(&messageBuffer);

      //send a message to robot
       if(sendto(server_socket_fd, messageBuffer.c_str(), messageBuffer.length(),0,(struct sockaddr*)&client_addr, sizeof(client_addr)) == -1)
       {
        perror("Send Data Failed:");
        exit(1);
       }
       delete pSensorMessage;
       count--;
      // for(int i=0;i<3;i++)
          // pose[i]+=0.1;
     sleep(1);
     while(1){}
 //}

 close(server_socket_fd);
 return 0;
}



