#ifndef NETWORK_H
#define NETWORK_H
#include "Camera.h"
#include <cstring>
#include <SDL_net.h>
class Network
{
    public:

        SDLNet_SocketSet server;
        TCPsocket connection;
        char tmp[1400];
        Network(const char *ip);
        ~Network();
        void Send(Camera *cam);
        void Recieve(Camera *p);
		void SendText(string *str);

    protected:
    private:
};

#endif // NETWORK_H
