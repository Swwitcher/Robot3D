using System.Collections;
using System.Net.Sockets;
using System.Net;
using System.Collections.Generic;
using UnityEngine;

public class Client : MonoBehaviour {

	public const int imageSidePx = 480; 
	public string serverIp = "127.0.0.1";
	public int port = 54000;
	public bool isConnected = false;
	private byte[] recv = new byte[100];
	public Camera leftCam;
	public Camera rightCam;

	private Client singleton;
	private Socket serverSocket;
	private System.Text.ASCIIEncoding encoded = new System.Text.ASCIIEncoding ();

	void Awake(){
		/*serverSocket = new Socket (AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		IPAddress remoteIPAdress = IPAddress.Parse (serverIp);
		IPEndPoint remoteEndPoint = new IPEndPoint (remoteIPAdress, port);
		singleton = this;
		serverSocket.Connect (remoteEndPoint); */

		//Tests
		byte[] tab = getCameraBytes(leftCam);
		for(int i=0; i< imageSidePx*imageSidePx; i++)
			print(System.Convert.ToInt64(tab[i]) + "\n");

		Application.Quit();

	}
	// void Update(){
		
		/*if (isConnected != serverSocket.Connected)
			isConnected = serverSocket.Connected;
		*/
		/* /!\ Convert CHAR to BYTE , NOT INT
		//Send image 1 to server
		serverSocket.Send (pixelMapToByteMap(getCamPixelTab(leftCam)));
		//Waiting for server answer
		while (serverSocket.Receive (recv) == 0);
		//Send image 2 to server
		serverSocket.Send (pixelMapToByteMap(getCamPixelTab(rightCam)));
		//Waiting for server answer (position of the person to follow
		while (serverSocket.Receive (recv) == 0);
		*/
		
	// }

	// void OnApplicationQuit(){
		/*serverSocket.Close ();
		serverSocket = null;  */
	// }

	Texture2D getImage(Camera cam){
		Rect view = cam.pixelRect;
		Texture2D texture = new Texture2D((int)view.width, (int)view.height, TextureFormat.RGB24, false);
		texture.ReadPixels(view, 0, 0, false);
		texture.Apply(false);
		return texture;
	}

	Color[] getColorMap(Texture2D texture){
		int i = 0;
		Color[] res = new Color[texture.width*texture.height];
		for(int y=0; y<texture.height; y++){
			for(int x=0; x<texture.width; x++){
				res[i] = texture.GetPixel(x, y);
				i++;
			}
		}
		return res;
	}

	byte[] colorToByteMap(Color[] map){
		byte[] res = new byte[map.Length];
		char c;
		int gray;
		byte b;
		for(int i=0; i<map.Length; i++){
			//Set to grayscale
			gray = (int)(map[i].r + map[i].g + map[i].b)/3;
			//Conversions 
			c = System.Convert.ToChar(gray);
			b = System.Convert.ToByte(c);
			//Assign
			res[i] = b;
		}
		return res;
	}

	byte[] getCameraBytes(Camera cam){
		Texture2D t = getImage(cam);
		Color[] cMap = getColorMap(t);
		return colorToByteMap(cMap);
	} 
}
