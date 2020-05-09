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
static int bufferSize = 4800;

//Convertie un tableau à deux dimension en une image Mat
void bytesToImage_Left(vector<char> bytes)
{
    Mat image_l = imdecode(bytes,IMREAD_GRAYSCALE);
//    imshow("test",image);
}

void bytesToImage_Right(vector<char> bytes)
{
    Mat image_r = imdecode(bytes,IMREAD_GRAYSCALE);
//    imshow("test",image);
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
    ImageProcessing imgProcess;

    char buf[bufferSize];     //buffer qui stocke les données reçu par le client
    int bytesRecv = 0;  //nb d'octet reçu par le serveur
    int totalSize = 0;  //nb total d'octet reçu
    //on créer un vector pour stocker chaque octet de l'image
    vector<char> image_data;

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

        //cout << "Début réception des données : " << string(buf, 0, bytesRecv) << endl;

        totalSize += bytesRecv;

        //si on reçoit un message du client pour arreter le serveur
        if(string(buf, 0, bytesRecv) == "quit"){
            char* msg = "Conection terminated !";
            send(clientSocket,msg,strlen(msg),0);
            cout << "Client disconnected !" << endl;
            return 0;
        }
        else if(totalSize >= sizeMaxImage){
            //on save l'image
            cout << "Image received ! " << totalSize << endl;
            bytesToImage_Left(image_data);
            totalSize = 0;
            char* msg = "All packet receive !";
            send(clientSocket,msg,strlen(msg),0);
        }
        else{
            cout << "Début réception des données : " << string(buf, 0, bytesRecv) << endl;
            cout << "Taille reçue = " << bytesRecv << endl;
            for(int i = 0; i < bytesRecv; i++){
                image_data.push_back(buf[i]);
            }

            char* msg = "Image packet received !";
            send(clientSocket,msg,strlen(msg),0);
        }
    }
    close(clientSocket);
    //close socket
    return 0;
}
