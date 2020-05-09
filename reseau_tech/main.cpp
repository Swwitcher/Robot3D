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
static int bufferSize = 65000;

//Convertie un tableau à deux dimension en une image Mat
Mat bytesToImage_Left(vector<uchar> bytes)
{
    Mat image_l = imdecode(bytes,IMREAD_ANYCOLOR);
    if(image_l.empty())
        cout << "l'image est vide !!" <<endl;
    cout << image_l.size << endl;
    //imshow("test",image_l);
    //waitKey(0);

    return image_l;
}

Mat bytesToImage_Right(vector<uchar> bytes)
{
    Mat image_r = imdecode(bytes,IMREAD_ANYCOLOR);
    if(image_r.empty())
        cout << "l'image est vide !!" <<endl;
    cout << image_r.size << endl;
    //imshow("test",image_r);
    //waitKey(0);

    return image_r;
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
    vector<uchar> bytes;
    char buf[bufferSize];     //buffer qui stocke les données reçu par le client
    int bytesRecv = 0;
    bool image_alternate = false;
    Mat image_l;
    Mat image_r;

    int gs_value = 0;
    bool gs_value_on = false;

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

        for(int i = 0; i < bytesRecv; i++)
            bytes.push_back(static_cast<unsigned char>(buf[i]));

        if(bytes.empty())
            cout << "c'est vide !!";
        else
            cout << "bytes size = " << bytes.size() << endl;

        if(image_alternate){
           //image right
            image_r = bytesToImage_Right(bytes).clone();
            bytes.clear();
            //on fait la carte de disparité
            Mat disp = imgProcess.dispMap(image_l, image_r).clone();
            if(!gs_value_on){
                gs_value = imgProcess.object_gs_value(disp);
                gs_value_on = true;
            }
            int res;

            if((res = imgProcess.left_or_right(disp)) == 0)
                res = imgProcess.forward_or_backward(gs_value,disp);
            char msg[100];
            sprintf(msg,"%d", res);
            send(clientSocket,msg,strlen(msg),0);

            image_alternate = false;
        }
        else{
            //image left
            image_l = bytesToImage_Left(bytes).clone();
            bytes.clear();
            char msg[100];
            sprintf(msg, "%d", 0);
            image_alternate = true;
            send(clientSocket,msg,strlen(msg),0);
        }
    }
    close(clientSocket);
    //close socket
    return 0;
}
