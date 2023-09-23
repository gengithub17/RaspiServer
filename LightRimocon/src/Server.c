#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h> //コンパイル時に -pthreadオプションをつける
#include <cjson/cJSON.h> //コンパイル時に -lcjsonオプションをつける

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEVICE_DRIVER "/dev/lrimo"
#define LOGFILE "/home/pi/BackApp/applog"
#define SERVER_PORT 11111
#define MAX_REQUEST_SIZE 1024
#define POST_BODY_SIZE 20

pthread_mutex_t FILE_ACCESS;
unsigned char LightMode = 0; //証明のon/off状態 //最後に書き込んだ値(あくまでも予測) //FILE_ACCESS所有時のみ変更可

int put_log(char *message){
	time_t now;
	struct tm *local_time;
	struct tm thread_local_time;
	time(&now);
	local_time = localtime_r(&now, &thread_local_time); //各スレッドから呼び出す時は構造体定義してから //localtime関数はグローバル変数使うのでスレッドセーフでない
	char now_print[20];
	memset(now_print, 0, sizeof(char)*20);
	sprintf(now_print, "%02d-%02d-%02d %02d:%02d:%02d", local_time->tm_year%100,local_time->tm_mon+1,local_time->tm_mday,local_time->tm_hour,local_time->tm_min,local_time->tm_sec);

	FILE *fp_log = fopen(LOGFILE, "a");
	fprintf(fp_log,"%s RimoconServer : %s\n", now_print, message);
	fclose(fp_log);
	return 0;
}

int put_signal(char *signal, char *executer){
	char message[50];
	sprintf(message, "signal %s from %s", signal, executer);
	return put_log(message);
}

void *button_read(void *arg){
	FILE *dd;
	if((dd = fopen(DEVICE_DRIVER, "r+")) == NULL){
		put_log("device driver open error at button_read");
		return NULL;
	}
	unsigned char read_val;
	unsigned char write_val;
	char signal_name[][4] = {"off", "on"};
	int pushing = 0;

	while(fread(&read_val, sizeof(char), 1, dd) == 1){
		if(!pushing && read_val==1){ //スイッチの押下を検知
			pushing = 1;
		}
		if(pushing && read_val==0){ //スイッチの開放を検知
			if(pthread_mutex_trylock(&FILE_ACCESS)==0){
				write_val = (LightMode+1)%2;
				if(fwrite(&write_val, sizeof(char), 1, dd)==1){
					LightMode = write_val;
					put_signal(signal_name[write_val], "Button");
				}else{
					put_log("device driver write error at button_read");
				}
				pthread_mutex_unlock(&FILE_ACCESS);
			}else{
				put_log("failed to get device driver write access at button_read");
			}
			pushing = 0;
		}
	}
	fclose(dd);
	put_log("exit button_read");
	return NULL;
}

int http_responce(int sock, int status, const char *status_message, const char *body){
	char responce[MAX_REQUEST_SIZE];
	
	if(sprintf(responce, "HTTP/1.1 %d %s\r\n\r\n", status, status_message)==EOF){
		put_log("http responce key error");
		return -1;
	}
	if(write(sock, responce, strlen(responce))<=0){
		put_log("http responce send error");
		return -1;
	}
	return 0;
}

int http_communication(int sock, char *body, int body_len){
	char send_buf=0;
	char recv_buf[MAX_REQUEST_SIZE];

	memset(recv_buf, 0, sizeof(char)*MAX_REQUEST_SIZE);

	if(read(sock, recv_buf, sizeof(char)*MAX_REQUEST_SIZE)<=0){
		put_log("failed to read request header");
		return -1;
	}else if(strstr(recv_buf, "POST")==NULL){
		http_responce(sock, 404, "Not Found", NULL);
		return 1;
	}else {
		memset(body, 0, sizeof(char)*body_len);
		if(read(sock, body, sizeof(char)*body_len)<=0){
			put_log("failed to read request body");
			return -1;
		}
	}
	return 0;
}

int read_jsondata(const char *json_str){
	cJSON *root = cJSON_Parse(json_str);
	if(root == NULL){
		put_log("failed to parse JSON data");
		return -1;
	}
	cJSON *value = cJSON_GetObjectItem(root, "signal");
	if(value == NULL || !cJSON_IsNumber(value)){
		put_log("JSON value error");
		cJSON_Delete(root);
		return -1;
	}
	return value->valueint;
}

void *socket_server(void *arg){
	int w_addr, c_sock;
	struct sockaddr_in a_addr;
	struct sockaddr_in client;
	socklen_t client_size = sizeof(client);

	w_addr = socket(AF_INET, SOCK_STREAM, 0);
	if(w_addr == -1){
		put_log("socket error");
		return NULL;
	}

	/*構造体を全て0にセット*/
	memset(&a_addr, 0, sizeof(struct sockaddr_in));

	/*サーバーのipとポート設定*/
	a_addr.sin_family = AF_INET;
	a_addr.sin_port = htons((unsigned short)SERVER_PORT);
	a_addr.sin_addr.s_addr = INADDR_ANY; //通信相手は任意

	/*ソケットに情報設定*/
	if(bind(w_addr, (const struct sockaddr *)&a_addr, sizeof(a_addr)) == -1){
		put_log("bind error");
		return NULL;
	}

	/*ソケットを接続待ちに設定*/
	if(listen(w_addr, 2) == -1){
		put_log("listen error");
		close(w_addr);
		return NULL;
	}

	char post_json[POST_BODY_SIZE];
	char signal[4]; //"on" or "off"
	unsigned char sig;
	char failure[2]; //クライアントに返すステータス 0/1
	while(1){
		strcpy(failure, "0");
		// waiting connection...
		printf("wating connection...\n");
		if((c_sock = accept(w_addr, (struct sockaddr *)&client, &client_size))==-1){
			put_log( "accept error");
			close(w_addr);
		}
		printf("accept ok\n");
		// connected
		if(http_communication(c_sock, post_json, POST_BODY_SIZE)!=0){
			continue;
		}

		memset(signal, 0, 4);
		switch(read_jsondata(post_json)){
			case 0:
				strcpy(signal, "off");
				sig = 0;
				break;
			case 1:
				strcpy(signal, "on");
				sig = 1;
				break;
			default:
				sig = -1;
		}
		if(sig == -1) continue;

		if(pthread_mutex_trylock(&FILE_ACCESS)==0){
			FILE *dd;
			if((dd = fopen(DEVICE_DRIVER, "w")) == NULL){
				put_log("device driver open error at socket_server");
			}else{
				if(fwrite(&sig, sizeof(unsigned char), 1, dd)==1){
					LightMode = sig;
					put_signal(signal, "App");
				}else{
					put_log( "device driver write error at socket_server");
					strcpy(failure, "1");
				}
				fclose(dd);
			}
			pthread_mutex_unlock(&FILE_ACCESS);
		}else{
			put_log("failed to get device driver write access at socket_server");
			strcpy(failure, "1");
		}

		http_responce(c_sock, 200, "OK", failure);
		close(c_sock);
	}
	close(w_addr);

	put_log("exit socket_server");
	return NULL;
}

int main(){
	pthread_mutex_init(&FILE_ACCESS, NULL);

	pthread_t button_thread, socket_thread;
	pthread_create(&button_thread, NULL, button_read, NULL);
	pthread_create(&socket_thread, NULL, socket_server, NULL);

	pthread_join(button_thread, NULL);
	pthread_join(socket_thread, NULL);

	pthread_mutex_destroy(&FILE_ACCESS);
	
	return 0;
}
