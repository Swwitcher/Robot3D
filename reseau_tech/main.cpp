#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>

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
/*
void disparityCard(Mat left, Mat right){
    //Mat dispCard, res;
    if(!left.empty() && !right.empty()){
        //calibrateWithParameter(i);
        //Ptr<StereoSGBM> steRes = StereoSGBM::create(0,64,10);
        //steRes->setMode(StereoSGBM::MODE_SGBM_3WAY);
        //steRes->compute(srcL,srcR,dispCard);
        //normalize(dispCard,res,0,255,CV_MINMAX,CV_8U);
        //imshow("carte de disparité",res);
        //waitKey(0);
    }
}
*/

static int width, height = 480;

//fonction trouvé sur https://answers.opencv.org/question/33596/convert-mat-to-byte-in-c/?answer=33603

void bytesToMat(char* bytes,int width,int height)
{
    Mat image = Mat(height,width,CV_8UC3,bytes).clone(); // make a copy
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
        cout << "host connecteing on " << result << endl;
    }
    else{
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host <<" connected on " << ntohs(client.sin_port) << endl;
    }

    //while receiv
    char buf[4096];
    int bytesRecv = 1;
    int error = 0;
    bool imageL = true;
    int sizeMaxImage = height*width*8;
    int totalSize;
    int n = 0;
    unsigned char *image = new unsigned char[height*width];

    while(true){
        //clear buffer
        memset(buf, 0, 4096);
        //wait for a message
        bytesRecv = recv(clientSocket, buf, 4096, 0);

        if(bytesRecv == -1){
            cerr << "Connection issue" << endl;
            break;
        }
        if(bytesRecv == 0){
            cout << "client disconnected !" << endl;
            break;
        }

        //cout << "received a message : " << string(buf, 0, bytesRecv) << endl;
        //cout << " : " << bytesRecv << endl;
        totalSize += bytesRecv;
        if(totalSize > sizeMaxImage){
            //on save l'image

            totalSize = 0;
            n = 0;
            char* msg = "coucou";
            send(clientSocket,msg,strlen(msg),0);
            break;
        }

        if(string(buf, 0, bytesRecv) == "quit"){
            char* msg = "Tchou";
            send(clientSocket,msg,strlen(msg),0);
            cout << "client disconnected !" << endl;
            break;
        }
        else{
            char* msg = "Roger !";
            send(clientSocket,msg,strlen(msg),0);
            //message d'erreur
//            for(int i = 0; i < bytesRecv; i++){
//                image[i+(n*4096)] = buf[i];
//            }
//            n++;
        }

    }
    close(clientSocket);
    //close socket
    return 0;
}
