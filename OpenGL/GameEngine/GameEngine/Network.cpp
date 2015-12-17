#include "Network.h"

Network::Network(const char *ipchar)
{
    SDLNet_Init();
    IPaddress ip;
    if(SDLNet_ResolveHost(&ip,ipchar,1234)==-1)
    {
        std::cout<<"There was an error"<<std::endl;
    }

    connection = SDLNet_TCP_Open(&ip);
    if(connection==NULL)
    {
        std::cout<<"There was an error(bad ip)"<<std::endl;
    }

    server = SDLNet_AllocSocketSet(1);

    SDLNet_TCP_AddSocket(server,connection);
}

Network::~Network()
{
    SDLNet_TCP_Send(connection,"2 \n",4);
    SDLNet_TCP_Close(connection);
    SDLNet_FreeSocketSet(server);
    SDLNet_Quit();
}

void Network ::Send(Camera *p)
{
    if(p->isReady())
    {
        sprintf(tmp,"1 %d \n",p->getId());

        int size=0;
        int len =strlen(tmp)+1;
        while(size<len)
        {
            size+=SDLNet_TCP_Send(connection,tmp+size,len-size);
        }
    }
}




/*void Network ::Recieve(Camera *p)
{
    while(SDLNet_CheckSockets(socket,0)>0 &&
           SDLNet_SocketReady(connection) )

    {
            int offset=0;
            do
            {
                offset+=SDLNet_TCP_Recv(connection,tmp+offset,1400);
                if(offset<=0)
                    return;
            }while(tmp[strlen(tmp)-1]!='\n');

            int type,id;
            sscanf(tmp,"%d %d",&type,&id);
            if(type==0 )
            {
               // std::cout<<"Type:"<<type<<std::endl;
                p->setId(id);
            }

            else if(type==1)
            {

            }

            else if(type==2)
            {

            }



                }
}
*/
