using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Threading;
using UnityEngine;

public class Personnage: MonoBehaviour
{

    public float speed = 1f;
    // Update is called once per frame
    void Update()
    {
        if(Input.GetKey(KeyCode.Z))
            transform.Translate(0f, 0f, speed*Time.deltaTime);
        if(Input.GetKey(KeyCode.Q))
            transform.Translate(-speed*Time.deltaTime, 0f, 0f);
        if(Input.GetKey(KeyCode.S))
            transform.Translate(0f, 0f, -speed*Time.deltaTime*0.5f);
        if(Input.GetKey(KeyCode.D))
            transform.Translate(speed*Time.deltaTime*0.5f, 0f, 0f);
    }
}
