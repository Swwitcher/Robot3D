using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class distance : MonoBehaviour
{
    public Transform target;
    int i = 0;
    // Update is called once per frame
    void Update()
    {
        float dist = Mathf.Sqrt(Mathf.Pow((target.position.x - transform.position.x), 2f) + Mathf.Pow((target.position.z - transform.position.z), 2f) - 1); //-1 car il y a 0.5 de rayon pour le robot et le personnage
        if (i % 50 == 0)
            print(dist);
        i++;
    }
}
