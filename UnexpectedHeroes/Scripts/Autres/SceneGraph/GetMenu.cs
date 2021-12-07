using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GetMenu : MonoBehaviour
{
    Canvas canvas;
    bool opt;
    // Start is called before the first frame update
    void Start()
    {
        canvas = GetComponent<Canvas>();
    }

    // Update is called once per frame
    void Update()
    {
        if (opt)
        {
            canvas.enabled = true;
        }
        else
        {
            canvas.enabled = false;
        }
    }
    public void Menu()
    {
        opt = !opt;
    }
}
