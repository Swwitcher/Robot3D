using System.Collections;
using System.Net.Sockets;
using System.Net;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

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
		Texture2D text = getText2D(leftCam);
		byte[] b = textToByte(text);
		print(b.Length);
	}
	/* void Update(){
		
		if (isConnected != serverSocket.Connected)
			isConnected = serverSocket.Connected;
		
		
	} */

	/*void OnApplicationQuit(){
		serverSocket.Close ();
		serverSocket = null;  
	}*/



// Fonction trouvé sur https://answers.unity.com/questions/576012/create-texture-from-current-camera-view.html, par cchameyr (Modifié pour prendre compte une camera mCamera)
private Texture2D getText2D(Camera mCamera)
     {
		 int mWidth = imageSidePx;
		 int mHeight = imageSidePx;
         Rect rect = new Rect(0, 0, mWidth, mHeight);
         RenderTexture renderTexture = new RenderTexture(mWidth, mHeight, 24);
         Texture2D screenShot = new Texture2D(mWidth, mHeight, TextureFormat.RGBA32, false);
 
         mCamera.targetTexture = renderTexture;
         mCamera.Render();
 
         RenderTexture.active = renderTexture;
         screenShot.ReadPixels(rect, 0, 0);
 
         mCamera.targetTexture = null;
         RenderTexture.active = null;
 
         Destroy(renderTexture);
         renderTexture = null;
         return screenShot;
     }

	private byte[] textToByte(Texture2D text){
		Color[] colormap = text.GetPixels();
		byte[] res = new byte[imageSidePx*imageSidePx];
		for(int i=0; i<imageSidePx; i++){
			res[i] = System.Convert.ToByte(colormap[i].grayscale * 100.0);
		}
		return res;
	}
}