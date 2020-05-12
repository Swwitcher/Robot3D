using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class Switch_camera : MonoBehaviour
{
    public Camera c1, c2;
    // Start is called before the first frame update
    void Start()
    {
        c1.enabled = true;
        c2.enabled = false;
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyUp("space")){
            c1.enabled = c1.enabled?false:true;
            c2.enabled = c2.enabled?false:true;
        }
    }
}
