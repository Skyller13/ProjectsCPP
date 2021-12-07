using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CameraController : MonoBehaviour
{
    public enum AngleCam
    {
        Zero = 0,
        Nonante = 1,
        CentHuitante = 2,
        DeuxCentSeptante = 3
    }

    [SerializeField] AngleCam angle;
    float distance;
    float timer; //between actions
    float timerLerpMAX = 1f;
    float timerLerp =0f;
    bool wasMoved = false;
    bool hasMoved = false;
    Vector3 initialPos;
    Vector3 departure;
    Vector3 destination;
    public Transform target;
    public Transform myCam;
    public Transform currentView;

    Vector3 oldPos; // camera lerp old pos
    Vector3 lastPos; // camera lerp last pos

    float angleBetween = 0f;

    private void Start()
    {
        timer = 0;

        transform.position = target.position;
        initialPos = myCam.position;

        departure = initialPos;
        destination = initialPos;
        oldPos = myCam.forward;
        lastPos = myCam.forward;
        currentView = myCam;
    }

    private void Update()
    {
        timerLerp += Time.deltaTime / timerLerpMAX;

        if(timerLerp >= 1)
        {
            timerLerp = 1;
        }

        if (wasMoved)
        {
            timer += Time.deltaTime;
        }

        if (timer > 2f)
        {
            timer = 0;
            wasMoved = false;
            hasMoved = true;
        }

        if (angle == AngleCam.Zero)
        {
            currentView = transform.GetChild(1);
        }
        else if (angle == AngleCam.Nonante)
        {
            currentView = transform.GetChild(2);
        }
        else if (angle == AngleCam.CentHuitante)
        {
            currentView = transform.GetChild(3);
        }
        else if (angle == AngleCam.DeuxCentSeptante)
        {
            currentView = transform.GetChild(4);
        }

        Vector3 zoom = myCam.forward.normalized * distance;

        if (Input.GetAxisRaw("VerticalPad") > 0)
        {
            distance += 10;
        }
        else if (Input.GetAxisRaw("VerticalPad") < 0)
        {
            distance -= 10;
        }

        if (distance > 150)
        {
            distance = 150;
        }
        else if (distance < 0)
        {
            distance = 0;
        }
               
        if (Input.GetAxisRaw("HorizontalPad") != 0
            && !wasMoved
            && !hasMoved)
        {
            float way = Input.GetAxisRaw("HorizontalPad");

            if (way < 0)
            {
                departure = initialPos;
                destination.x = -departure.z;
                destination.z = departure.x;

                oldPos = myCam.forward;
                lastPos.x = -oldPos.z;
                lastPos.z = oldPos.x;

                angleBetween = Vector3.Angle(departure, destination);

                angle--;
                if (angle < AngleCam.Zero)
                {
                    angle = AngleCam.DeuxCentSeptante;
                }

                wasMoved = true;
                timerLerp = 0f;
            }
            else
            {
                if (!wasMoved)
                {
                    departure = initialPos;

                    destination.x = departure.z;
                    destination.z = -departure.x;

                    oldPos = myCam.forward;
                    lastPos.x = oldPos.z;
                    lastPos.z = -oldPos.x;

                    angleBetween = -Vector3.Angle(departure, destination);

                    angle++;
                    if (angle > AngleCam.DeuxCentSeptante)
                    {
                        angle = AngleCam.Zero;
                    }

                    wasMoved = true;
                    timerLerp = 0f;
                }
            }
        }

        initialPos = Quaternion.Euler(0f, angleBetween * timerLerp, 0f) * departure;
        myCam.forward = Quaternion.Euler(0f, angleBetween * timerLerp, 0f) * oldPos;
        myCam.position = initialPos + zoom;
        myCam.position += target.position;

        if (hasMoved)
        {
            initialPos = currentView.position;
            myCam.forward = currentView.forward;
            myCam.position = initialPos + zoom;
            myCam.rotation = currentView.rotation;

            departure = initialPos;
            angleBetween = 0.0f;
            oldPos = myCam.forward;

            hasMoved = false;
        }
    }
}
