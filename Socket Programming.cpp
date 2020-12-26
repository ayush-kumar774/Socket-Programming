#include <iostream>
#include <winsock.h>

using namespace std;
#define PORT 9909

// Port number 0 to 256 are reserved for system purposes.

struct sockaddr_in srv;
// This struct contains all the details about the socket (The prototype is given below)
// struct sockaddr_in
// {
//         short sin_family ; -> socket internet family (which is the first argument of socket function).
//         u_short sin_port; -> socket port number
//         struct in_addr sin_addr ; -> Machine address IP adrress
//         char sin_zero[8] ; -> It has nothing to do. All we need to initialize it with zero
// }

fd_set fr, fw, fe;

// FD_SETSIZE = 64 
// typedef struct fd_set {
/*      u_int fd_count;  /* how many are SET? */ // -> It contains the number of socket descriptor which are
//                       going to set in the fd_set and we will set those socket descriptor in the array which is below
//      SOCKET fd_array[FD_SETSIZE]; // an array of SOCKETs
//} fd_set ;

int nMaxFd;

int main()
{
    int nRet = 0;
    // Initialize the WSA variables -> It enables the API which are responsible for socket programming in windows
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
    {
        cout << "WSA failed to initialize." << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "WSA initialized." << endl;
    }

    // Initialize the socket
    //socket(adress family , type , protocol)
    // if protocol is zero then it will use underlink protocol
    // protocol IPPROTO_TCP -> to use TCP/IP protocol
    // AF_INET -> internetwork : UDP, TCP , etc.
    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket < 0)
    {
        cout << "The socket is not opened." << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "Socket is opened successfully." << endl;
        cout << "The Socket ID is " << nSocket << endl;
    }

    // Initialize the environment for sockaddr structure
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = INADDR_ANY; // We are assigning the address of our local machine
    // This INADDR_ANY macro will simply fetch our(system) IP address 
    // If we want to give our IP address manually we simply need to write
    // srv.sin_addr.s_addr = inet_addr("127.0.0.5") ;

    memset(&(srv.sin_zero), 0, 8);

    // Day 2 
    // About the Blocking vs Non Blocking Socket
    // optval = 0 means blocking and != 0 means non blocking

    // We don't need this piece of code because by default every socket is blocking socket.
    // We will need this code only when we are willing to use non-blocking socket.

    // START
        // u_long optval = 0 ;
        // nRet = ioctlsocket(nSocket, FIONBIO, &optval) ;
        // // ioctlsocket() -> In windows this function is used for setting the socket.
        // // it's argument are the socket, FIONBIO -> set/clear non-blocking i/o , unsigned long type integer 
        // // where 0 means blocking and 1 means non blocking socket
        // if (nRet != 0)
        // {
        //     cout << "ioctlsocket call failed" << endl ;
        //     exit(EXIT_FAILURE) ;
        // }
        // else 
        // {
        //     cout << "ioctlsocket call passed" << endl ;
        // }

    // END


    // Bind the socket to the local port
    nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
    if (nRet < 0)
    {
        cout << "Failed to bind to local port." << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "Binding successful." << endl;
    }
    // Listen the request from the client (queues the request)

    nRet = listen(nSocket, 5); // listen(socket, backlog)
    // backlog -> how many request at a time we can listen to 
    // Suppose 10 request comes to the server 5 will be in active queue (listening mode) and rest 5 will be in waiting queue.
    if (nRet < 0)
    {
        cout << "Failed to start listen to local port" << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "Started listening to local port." << endl;
    }

    /* int n = 100;
     for (int nIndex = 0; nIndex < 64; nIndex++)
     {
         FD_SET(n, &fr);
         n++;
     }
     cout << "FD count " << fr.fd_count << endl;
     for (int nIndex = 0; nIndex < 64; nIndex++)
     {
         cout << fr.fd_array[nIndex] << endl ;

     }*/ // In this program if you change the condition to like any number which is greater
     // than 64 but still you will get the count 64


    nMaxFd = nSocket;
    struct timeval tv;
    tv.tv_sec = 1; // we will be waiting for one second.
    tv.tv_usec = 0; // It is for micro second

    while (1)
    {
        // Here we are setting the socket descriptor
        FD_ZERO(&fr);
        FD_ZERO(&fw);
        FD_ZERO(&fe);

        FD_SET(nSocket, &fr);
        FD_SET(nSocket, &fw);
        FD_SET(nSocket, &fe);

        // cout << "Before select call: " << fr.fd_count << endl;
        // The count for fe will be the same as fr.

        // Keep waiting for new requests and proceed as per the request

        nRet = select(nMaxFd + 1, &fr, &fw, &fe, &tv);
        // select(SocketDescriptor, reading descriptor, writing descriptor, exceptional descriptor, time in which our descriptors should be ready) ;
        // cout << "After select call: " << fr.fd_count << endl;

        if (nRet > 0)
        {
            // When someon connects or communicates with a messafe over
            // a dedicated connection

        }
        else if (nRet == 0)
        {
            // No connection or any communication request made or you can
            // say that none of the socket descriptors are ready.
            cout << "Nothing on the port " << PORT << endl;
        }
        else
        {
            // It failed and your code shouild show some useful message.
        }
    }
    return 0;
}