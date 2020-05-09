#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>

#include <imageprocessing.h>

//include socket
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

//include opencv
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

static int width = 480, height = 480;
static int sizeMaxImage = height*width;
static int bufferSize = 60000;

//Convertie un tableau à deux dimension en une image Mat
void bytesToImage_Left(vector<uchar> bytes)
{
    Mat image_l = imdecode(bytes,IMREAD_GRAYSCALE);
    imshow("test",image_l);
}

void bytesToImage_Right(vector<uchar> bytes)
{
    Mat image_r = imdecode(bytes,IMREAD_GRAYSCALE);
    imshow("test",image_r);
}

int main(int argc, char *argv[])
{
    //create socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1){
        cerr << "le socket ne fonctionne pas" << endl;
        return -1;
    }
    //bind socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);//transforme un mobre en array d'integer

    if(bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1){
        cerr << "bind impossible avec le port/ip fourni !" << endl;
        return -2;
    }
    //listen socket
    if(listen(listening, SOMAXCONN) == -1){
        cerr << "Can't listen" << endl;
        return -3;
    }
    //accept call
    sockaddr_in client;
    socklen_t clientsize = sizeof(client);
    char host[NI_MAXHOST];
    char server[NI_MAXSERV];
    int clientSocket = accept(listening, (sockaddr*) &client, &clientsize);
    if(clientSocket == -1) {
        cerr << "Problem with client connecting" << endl;
        return -4;
    }

    //close listening socket.
    close(listening);
    memset(host, 0, NI_MAXHOST);
    memset(server, 0, NI_MAXSERV);
    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, server, NI_MAXSERV, 0);
    if(result){
        cout << "host connecting on " << result << endl;
    }
    else{
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host <<" connected on " << ntohs(client.sin_port) << endl;
    }


    //while receiv data
    //ImageProcessing imgProcess;
    vector<uchar> bytes;
    char buf[bufferSize];     //buffer qui stocke les données reçu par le client
    int bytesRecv = 0;
    bool image_alternate = false;
    while(true){

        //clear buffer
        memset(buf, 0, bufferSize);

        //wait for a message
        bytesRecv = recv(clientSocket, buf, bufferSize, 0);
        if(bytesRecv == -1){
            cerr << "Connection issue" << endl;
            break;
        }
        if(bytesRecv == 0){
            cout << "client disconnected !" << endl;
            break;
        }

        for(int i = 0; i < bytesRecv; i++){
            bytes.push_back(static_cast<unsigned char>(buf[i]));
        }
        if(image_alternate){
           //image right
            bytesToImage_Right(bytes);
            bytes.clear();
            //on fait la carte de disparité
        }
        else{
            //image left
            bytesToImage_Left(bytes);
            bytes.clear();
        }
    }
    close(clientSocket);
    //close socket
    return 0;
}
