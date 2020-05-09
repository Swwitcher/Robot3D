using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Threading;
using UnityEngine;

public class player : MonoBehaviour
{

    public float speed = 1f;
    // Update is called once per frame
    void Update()
    {
        transform.Translate(speed * Input.GetAxis("Horizontal")*Time.deltaTime, 0f, speed*Input.GetAxis("Vertical")*Time.deltaTime);
    }
}
