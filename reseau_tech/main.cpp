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
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

static int width = 480, height = 480;
static int sizeMaxImage = height*width;
static int bufferSize = 4800;

//fonction trouvé sur https://answers.opencv.org/question/33596/convert-mat-to-byte-in-c/?answer=33603
//Convertie un tableau à deux dimension en une image Mat
void bytesToMat(char** bytes,int width,int height)
{
    Mat image(width, height, CV_8UC1);
    memcpy(image.data, bytes, sizeMaxImage*sizeof(char));
    imshow("test",image);
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
    //ImageProcessing imgProcess = new ImageProcessing();

    char buf[bufferSize];     //buffer qui stocke les données reçu par le client
    int bytesRecv = 0;  //nb d'octet reçu par le serveur

    int totalSize = 0;  //nb total d'octet reçu
    int cpt_row = 1;    //cpt de ligne

    Mat image(480,480, CV_8UC3, Scalar(0,0,0));
    if(image.empty()){
        cout << "erreur lors de la création de l'image" << endl;
    }

    //on créer un tablau en 2 dimensions pour stocker chaque octet de l'image
    char **image2D = new char*[height];
    for(int i = 0; i < height;i++ ){
        image2D[i] = new char[width];
    }


    while(true){
        char b[480*480];
        recv(clientSocket, buf, 480*480, 0);
        cv::Mat m = cv::imdecode(b, IMREAD_GRAYSCALE);
        cv::imshow("heho", m);
        /*//clear buffer
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
            //bytesToMat(image,width,height);
            totalSize = 0;
            cpt_row == 0;
            char* msg = "All packet receive !";
            send(clientSocket,msg,strlen(msg),0);
        }
        else{
            cout << "Début réception des données : " << string(buf, 0, bytesRecv) << endl;
            cout << "Taille reçue = " << bytesRecv << endl;
            for(int i = 0; i < bytesRecv; i++){
                if(i%width == 0 && i != 0)
                    cpt_row++;
                image2D[cpt_row][i] = buf[i];
            }
            char* msg = "Image packet received !";
            send(clientSocket,msg,strlen(msg),0);
        }*/
    }
    close(clientSocket);
    //close socket
    return 0;
}
