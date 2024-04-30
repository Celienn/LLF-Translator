#include "serverUDP.h"

using namespace std;
int main(int argc,char **argv, char **env){
    Socket serverudp(SOCK_DGRAM,0);
    if (serverudp.Bind() == -1 ){
        cout << "failed to connect" << endl;
    }else{
        cout << "linked to socket" << endl;
    }
    inet_addr("127.0.0.1");


    while(1){
        cout << "in boucle" << endl;

        if (serverudp.Receive() == -1){
            cout << "Receive() failed" << endl;
        }else{
            cout <<  serverudp.getbuffer() << endl;
        }

        if (serverudp.Send() == -1){
            cout << "Sendto()" << endl;
        }else{
            cout << "callback sent" << endl;
        }

    }
    


    return 1;
}