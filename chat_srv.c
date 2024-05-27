#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <errno.h>
#include <time.h>

#define MAX_CLIENT 5
#define MAX_EVENTS	10

void errProc(const char*);

int main(int argc, char** argv)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);// 현재 날짜 시간 출력
	

	int listenSd, connectSd;
	struct sockaddr_in srvAddr, clntAddr;
	int clntAddrLen, readLen;
	char rBuff[BUFSIZ], rBuff1[BUFSIZ];
	int i;

	int epfd, ready, readfd;
	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];
    
    //추가한 배열
    int client[MAX_CLIENT];
    //초기화, -1은 사용되지 않고 있다는 의미
    memset(client, -1, sizeof(int) * MAX_CLIENT);

	if(argc != 2)
	{
		printf("Usage: %s [Port Number]\n", argv[0]);
		return -1;
	}

	printf("Server start...\n"); //시작

	epfd = epoll_create(1);
	if(epfd == -1) errProc("epoll_create"); 

	listenSd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(listenSd == -1 ) errProc("socket");
	
	memset(&srvAddr, 0, sizeof(srvAddr));
	srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(atoi(argv[1]));

	if(bind(listenSd, (struct sockaddr *) &srvAddr, sizeof(srvAddr)) == -1)
		errProc("bind");

	if(listen(listenSd, 5) < 0) errProc("listen");
	
	ev.events = EPOLLIN;
	ev.data.fd = listenSd;
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, listenSd, &ev) == -1)
		errProc("epoll_ctl");

	clntAddrLen = sizeof(clntAddr);
	while(1) {
		printf("Monitoring ... \n");
		ready = epoll_wait(epfd, events, MAX_EVENTS, -1);
		if(ready == -1) {
			if(errno == EINTR) continue;
			else errProc("epoll_wait");			
		}
		
		for(i=0; i<ready; i++)	{
			if(events[i].data.fd == listenSd) {// accept a client 
				connectSd = accept(listenSd, (struct sockaddr *) &clntAddr, &clntAddrLen);
				if(connectSd == -1)	{
					fprintf(stderr,"Accept Error");
					continue;
				}
                else{
                    int j, k;
                    /*
                    client 배열에 등록
                    비어있는 가장 작은 값에 할당
                    저장하면 for밖으로 나감 
					*/
                    for(j = 0; j < MAX_CLIENT; ++j){
                        if(client[j] == -1){
                            client[j] = connectSd;
                            break;
                        }
                    }
                    
					sprintf(rBuff1, "채팅 서버에 연결 되었습니다.\n");
					write(client[j], rBuff1, strlen(rBuff1));
					
                    //다른 클라이언트들에게 연결알림
                    for(k = 0; k < MAX_CLIENT; ++k){
                        //방금 연결된 클라이언트 j와 사용하지 않는 client는 제외
                        if(k == j || client[k] == -1){
                            continue;
                        }
                        else{
                            //연결
							sprintf(rBuff1, "사용자%d님이 입장하였습니다.\n", j+1);
							write(client[k], rBuff1, strlen(rBuff1));
                        }
                    }
                }
				fprintf(stderr,"A client is connected...\n");
                
				ev.data.fd = connectSd;
                if(epoll_ctl(epfd, EPOLL_CTL_ADD, connectSd, &ev) == -1)
                                    errProc("epoll_ctl");
			}
			else {
				readfd = events[i].data.fd;
				readLen = read(readfd, rBuff, sizeof(rBuff)-1);
				if(readLen == 0) 
				{
					fprintf(stderr,"A client is disconnected...\n");
                    
                    //연결 끊김
		    int k;
		    for(int j = 0; j< MAX_CLIENT; ++j){
			   if(client[j] == readfd){
					 k = j+1;
					 client[j] = -1;
					 }
			 } 
                    for(int j = 0; j < MAX_CLIENT; ++j){
                        
                      
                        if(client[j] != -1){
							sprintf(rBuff1, "사용자%d님이 퇴장하였습니다.\n", k);
							write(client[j], rBuff1, strlen(rBuff1));
                        }
                    }
                    
					if(epoll_ctl(epfd, EPOLL_CTL_DEL, readfd, &ev) == -1)
						errProc("epoll_ctl");	
					close(readfd);
					continue;
				}
				rBuff[readLen] = '\0';
				printf("Client(%d): %s\n", events[i].data.fd,rBuff);
                
		int k;
		for(int j = 0; j < MAX_CLIENT; ++j){
			if(client[j] == readfd){
				k = j+1;
			}
		}
                for(int j = 0; j < MAX_CLIENT; ++j){
                    if(client[j] == readfd || client[j] == -1){ //보낸대상,안쓰는거
                        continue;
                    }
                    else{
						sprintf(rBuff1, "[%d.%d.%d %d:%d:%d] 사용자%d : %s\n",
							tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
							tm.tm_hour, tm.tm_min, tm.tm_sec, k, rBuff);
						write(client[j], rBuff1, strlen(rBuff1));
                    }
                }
			}
		}	
	}
	close(listenSd);
	close(epfd);
	return 0;
}

void errProc(const char * str)
{
	fprintf(stderr,"%s: %s", str, strerror(errno));
	exit(1);
}
