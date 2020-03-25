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
		serverSocket = new Socket (AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		IPAddress remoteIPAdress = IPAddress.Parse (serverIp);
		IPEndPoint remoteEndPoint = new IPEndPoint (remoteIPAdress, port);
		singleton = this;
		serverSocket.Connect (remoteEndPoint);

		//Test//

		serverSocket.Send (pixelMapToByteMap(getCamPixelTab(leftCam)));
	
		//End//
	}

	void Update(){
		if (isConnected != serverSocket.Connected)
			isConnected = serverSocket.Connected;
		
		/*
		//Send image 1 to server
		serverSocket.Send (pixelMapToByteMap(getCamPixelTab(leftCam)));
		//Waiting for server answer
		while (serverSocket.Receive (recv) == 0);
		//Send image 2 to server
		serverSocket.Send (pixelMapToByteMap(getCamPixelTab(rightCam)));
		//Waiting for server answer (position of the person to follow
		while (serverSocket.Receive (recv) == 0);
		*/
		
	}

	void OnApplicationQuit(){
		serverSocket.Close ();
		serverSocket = null; 
	}


	Color[] getCamPixelTab(Camera cam){
		Texture2D texture = new Texture2D (imageSidePx, imageSidePx);
		RenderTexture rt = new RenderTexture (imageSidePx, imageSidePx, 16);
		cam.targetTexture = rt;
		Rect src = new Rect (0, 0, imageSidePx, imageSidePx);
		cam.Render ();
		RenderTexture.active = rt;
		texture.ReadPixels (src, 0, 0);
		Color[] result = texture.GetPixels();
		return result;
	}

	int colorToGray(Color pixel){
		return (int)((pixel.r + pixel.g + pixel.b) / 3); 
	}

	byte[] pixelMapToByteMap(Color[] map){
		byte[] result = new byte[imageSidePx*imageSidePx];
		for (int i = 0; i < imageSidePx * imageSidePx; i++)
			result [i] = System.Convert.ToByte(colorToGray (map [i]));
		return result;
	}


}
