using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MenuAide : MonoBehaviour
{
    GetMenu affichage;

    // Start is called before the first frame update
    void Start()
    {
        affichage = FindObjectOfType<GetMenu>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void OpenOptions()
    {
        affichage.Menu();
    }
}
