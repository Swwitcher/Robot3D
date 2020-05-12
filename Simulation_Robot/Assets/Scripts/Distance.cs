using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using UnityEngine;

public class Distance : MonoBehaviour
{
    public Transform target;
    int i = 0;
    int x = 0;
    List<string> lines = new List<string>();
    string createText, readText;
    // Update is called once per frame
    void Update()
    {
        float dist = (Mathf.Sqrt(Mathf.Pow((target.position.x - transform.position.x), 2f) + Mathf.Pow((target.position.z - transform.position.z), 2f) - 0.75f))/2.75f;
        if (i % 10 == 0)
            lines.Add(dist.ToString());
        i++;
        File.WriteAllLines(Application.dataPath + "res.txt", lines);


    }
}