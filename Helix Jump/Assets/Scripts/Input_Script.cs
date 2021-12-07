using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Input_Script : MonoBehaviour
{
    bool isPressed = false;
    Vector3 PressPos;
    Quaternion Rotation = new Quaternion();
    GameManager gm;

    void Start()
    {
        gm = FindObjectOfType<GameManager>();
        PressPos = new Vector3();
    }

    void Update()
    {
        //check mouse pressed
        if (Input.GetMouseButton(0)
            && gm.playerState
            && Time.timeScale > 0f)
        {
            if (!isPressed)
            {
                PressPos = Input.mousePosition;
                isPressed = true;
            }

            //modify tower rotation 
            if (isPressed)
            {
                Rotation = transform.rotation;

                if(Input.mousePosition.x < PressPos.x)
                {
                    float i = Rotation.eulerAngles.y + 0.5f;

                    Rotation = Quaternion.Euler(0, i, 0);

                    transform.rotation = Rotation;
                }
                
                if (Input.mousePosition.x > PressPos.x)
                {
                    float i = Rotation.eulerAngles.y - 0.5f;

                    Rotation = Quaternion.Euler(0, i, 0);

                    transform.rotation = Rotation;
                }
            }
        }
        else
        {
            //reset values
            PressPos = new Vector3();
            isPressed = false;
        }
    }
}
