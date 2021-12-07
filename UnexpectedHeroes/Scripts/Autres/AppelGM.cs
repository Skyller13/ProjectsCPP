using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AppelGM : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Toolbox.Instance.GetManager<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
