#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include"cJSON.h"

char *CreateTopo(int node_num,int link_num,char *ip_addr[],char *code[],char *host_ip[],char *det_ip[],int conn_status,int bandwidth[],int bw_usage[],int pkt_drop[],int delay[]){
	cJSON *topoJson = cJSON_CreateObject();
	cJSON *node_array = cJSON_CreateArray();
	cJSON *link_array = cJSON_CreateArray();
	 for(int i=0;i<node_num;i++){
		cJSON *nodeJson = cJSON_CreateObject();
		cJSON_AddItemToArray(node_array,nodeJson);
		cJSON_AddStringToObject(nodeJson,"ip",ip_addr[i]);
		cJSON_AddStringToObject(nodeJson,"code",code[i]);
	}
	
	for(int j=0;j<link_num;j++){
		cJSON *linkJson = cJSON_CreateObject();
		cJSON_AddItemToArray(link_array,linkJson);
		cJSON_AddStringToObject(linkJson,"host_ip",host_ip[j]);
		cJSON_AddStringToObject(linkJson,"forign_ip",det_ip[j]);
		cJSON_AddNumberToObject(linkJson,"conn_status",conn_status);
		cJSON_AddNumberToObject(linkJson,"comm_way",0);
		cJSON_AddNumberToObject(linkJson,"bandwidth",bandwidth[j]);
		cJSON_AddNumberToObject(linkJson,"bw_usage",bw_usage[j]);
		cJSON_AddNumberToObject(linkJson,"pkt_drop",pkt_drop[j]);
		cJSON_AddNumberToObject(linkJson,"delay",delay[j]);
	}
	
	cJSON_AddItemToObject(topoJson,"node",node_array);
	cJSON_AddItemToObject(topoJson,"link",link_array);
	char *t = cJSON_Print(topoJson);
	cJSON_Delete(topoJson);
	return t;
}

char *QUERYreply(int result,int subclass,int network_num,char *ip_addr[],int connectivity[],char *topo){
	char *sub[2] = {"reachability","topology"}; //subclass 0:sub[0] ; 1:sub[1]
	cJSON *queryJson = cJson_CreateObject();
	if(NULL == queryJson){
		return NULL;
	}1
	
	cJSON_AddStringToObject(queryJson,"cmd","QUERY-RESPONSE");
	cJSON_AddNumberToObject(queryJson,"result",result);
	cJSON_AddNumberToObject(queryJson,"cmd_id",235);
	cJSON_AddStringToObject(queryJson,"subclass",sub[subclass]);	
	if(subclass == 1){
	//return networks status
		cJSON *array = cJson_CreateArray();
		for(int i=0;i<network_num;i++){
		cJSON *subJson = cJSON_CreateObject();
		cJSON_AddItemToArray(array,subJson);
		cJSON_AddItemToObject(subJson,"destination",cJSON_CreateString(ip_addr[i]));
		cJSON_AddItemToObject(subJson,"connectivity",cJSON_CreateNumber(connect_stu[connect[i]]));
		}
		cJSON_AddItemToObject(queryJson,"network",array);

	}
	else{
	//return topology
		cJSON *topoJson = cJSON_Parse(topo);
		cJSON_AddItemToObject(queryJson,"topology",topoJson);
	}
	char *q = cJSON_Print(queryJson);
	cJSON_Delete(queryJson);
	return q;
}
char *BINDreply(int result,int binding_id){
	cJSON bindJson = cJson_CreateObject();
        cJSON_AddStringToObject(bindJson,"cmd","BIND-RESPONSE");
        cJSON_AddNumberToObject(bindJson,"result",result);
        cJSON_AddNumberToObject(bindJson,"cmd_id",235);
        cJSON_AddNumberToObject(bindJson,"binding_id",binding_id);
	
	char *b = cJSON_Print(bindJson);
        cJSON_Delete(bindJson);
        return t;

}

char *UNBINDreply(int result){
        cJSON unbindJson = cJson_CreateObject();
        cJSON_AddStringToObject(unbindJson,"cmd","UNBIND-RESPONSE");
        cJSON_AddNumberToObject(unbindJson,"result",result);
        cJSON_AddNumberToObject(unbindJson,"cmd_id",235);

        char *u = cJSON_Print(unbindJson);
        cJSON_Delete(unbindJson);
        return t;

}
char *AddVENTreply(int result,int vnet_id,char *topo){
        cJSON addventJson = cJson_CreateObject();
        cJSON_AddStringToObject(addventJson,"cmd","ADD-VENT-RESPONSE");
        cJSON_AddNumberToObject(addventJson,"vnet_id",vnet_id);
        cJSON_AddNumberToObject(addventJson,"cmd_id",235);
	cJSON_AddNumberToObject(addventJson,"result",result);

        char *a = cJSON_Print(addventJson);
        cJSON_Delete(addventJson);
        return t;

}

char *REMOVEVENTreply(int result,int vnet_id){
        cJSON removeventJson = cJson_CreateObject();
        cJSON_AddStringToObject(removeventJson,"cmd","REMOVE-VENT-RESPONSE");
        cJSON_AddNumberToObject(removeventJson,"vnet_id",vnet_id);
        cJSON_AddNumberToObject(removeventJson,"cmd_id",235);
	cJSON_AddNumberToObject(removeJson,"result",result);

        char *a = cJSON_Print(addventJson);
        cJSON_Delete(addventJson);
        return t;

}
void parseQUERY(char *Qmsg){

}

void parseBIND(char *Imsg){

}

int main(void){
	char *ip_addr[] = {"192.168.10.1","10.0.0.1"};
	char *det_ip[] = {"192.168.10.2"};
	char *host_ip[] = {"192.168.10.3"};
	char *code[] = {"293259WE","23r322B"};
	int bandwidth[] = {102400};
	int bw_usage[] = {2};
	int pkt_drop[] = {5};
	int delay[] = {100};
	char *topo = CreateTopo(2,1,ip_addr,code,host_ip,det_ip,0,bandwidth,bw_usage,pkt_drop,delay);
	printf("%s\n",topo);
	return 0;
}
