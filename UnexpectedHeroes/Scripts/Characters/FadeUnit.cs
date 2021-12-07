using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FadeUnit : MonoBehaviour
{
    private Unit selfUnit;
    private float speed;
    private bool stopBool;

    private Material fadeMat;
    private Color baseColor = Color.yellow;
    private Material[] mats;

    void Start()
    {
        stopBool = false;
        speed = 0.01f;

        selfUnit = GetComponent<Unit>();

        fadeMat = Resources.Load("Materials/fadeMatAdd", typeof(Material)) as Material;

        Renderer rend = selfUnit.GetComponentInChildren<Renderer>();

        baseColor = rend.material.GetColor("_BaseColor");

        mats = rend.materials;

        for (int i = 0; i < mats.Length; i++)
        {
            mats[i] = new Material(fadeMat);
        }

        selfUnit.GetComponentInChildren<Renderer>().materials = mats;
    }

    public bool IsFaded()
    {
        if (baseColor.a <= 0)
        {
            baseColor.a = 0;
            stopBool = true;
            return true;
        }

        return false;
    }

    void Update()
    {
        if (!stopBool)
        {
            baseColor.a -= speed;

            foreach (var VARIABLE in mats)
            {
                VARIABLE.SetColor("_BaseColor", baseColor);
            }

            IsFaded();
        }
        else
        {
            //can destroy unit
        }
    }
}
