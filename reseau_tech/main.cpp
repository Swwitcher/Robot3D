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

/**
 * @brief convert a byte vector into a Mat
 * @param bytes the vector to consider (Bytes need to encode a JPG image)
 * @return a Mat (That you can use as an image)
 */
Mat bytesToImage(vector<uchar> bytes)
{
    Mat image_l = imdecode(bytes,IMREAD_ANYCOLOR);
    if(image_l.empty())
        cout << "Empty image !!" <<endl;
    cout << image_l.size << endl;

    return image_l;
}


int main(int argc, char *argv[])
{
    //create socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1){
        cerr << "Socket is not working !" << endl;
        return -1;
    }
    //bind socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    if(bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1){
        cerr << "Cannot bind with this ip / port !" << endl;
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
    char buf[bufferSize];     //store recieved data
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
            cout << "Empty";
        else
            cout << "bytes size = " << bytes.size() << endl;

        if(image_alternate){
           //right image
            image_r = bytesToImage(bytes).clone();
            bytes.clear();
            //Disparity map generation
            Mat disp = imgProcess.dispMap(image_l, image_r).clone();
            //imshow("Disp", disp);
            //waitKey(1);
            if(!gs_value_on){
                gs_value = imgProcess.obj_pix_nb(disp);
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
            //Left image
            image_l = bytesToImage(bytes).clone();
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
