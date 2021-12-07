using System;
using Kapy.Move;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using UnityEngine;

public class IngameInputMap : KapyInputMap
{
    public bool isZoom = false;
    public bool isPreAction = false;
    public int currentZoom = 0;

    public override void CustomEnable()
    {
        //movements
        IM.ONStickLeftTap += DirectionChange;
        IM.ONStickLeftHold += MoveChange;

        //actions
        IM.ONButtonATap += InGameButtonA;
        IM.ONButtonBTap += InGameButtonB;
        IM.ONButtonXTap += InGameButtonX;
        IM.ONButtonYTap += InGameButtonY;
        IM.ONButtonStartTap += InGameStartButton;

        //camera
        IM.ONStickRightTap += CameraSwitch;

        //TEST
        ONDirectionChange += testInputMap;
    }

    public override void CustomDisable()
    {
        IM.ONStickLeftTap -= DirectionChange;
        IM.ONStickLeftHold -= MoveChange;

        //actions
        IM.ONButtonATap -= InGameButtonA;
        IM.ONButtonBTap -= InGameButtonB;
        IM.ONButtonXTap -= InGameButtonX;
        IM.ONButtonYTap -= InGameButtonY;
        IM.ONButtonStartTap -= InGameStartButton;

        //camera
        IM.ONStickRightTap -= CameraSwitch;

        //TEST
        ONDirectionChange -= testInputMap;
    }

    public event DirectionDelegate ONDirectionChange;
    void DirectionChange(typeDirection _dir)
    {
        ONDirectionChange?.Invoke(_dir);
    }

    public event DirectionDelegate ONCameraSwitch;
    void CameraSwitch(typeDirection _dir)
    {
        ONCameraSwitch?.Invoke(_dir);
    }

    public event DirectionDelegate OnCameraZoom;
    void CameraZoom(typeDirection _dir)
    {
        OnCameraZoom?.Invoke(_dir);
    }

    public event DirectionDelegate ONMoveChange;
    void MoveChange(typeDirection _dir)
    {
        ONMoveChange?.Invoke(_dir);
    }

    public event VoidDelegate ONBaseAttack;
    public event VoidDelegate ONFirstSpell;
    void InGameButtonA()
    {
        if (IM.XBoxControllerInspector.triggerRight.isUsed)
        {
            //spells
            ONFirstSpell?.Invoke();
        }
        else
        {
            //base attack
            ONBaseAttack?.Invoke();
        }
    }

    public event VoidDelegate ONSecondSpell;
    void InGameButtonB()
    {
        if (IM.XBoxControllerInspector.triggerRight.isUsed)
        {
            //spells
            ONSecondSpell?.Invoke();
        }
        else
        {

        }
    }

    public event VoidDelegate OnThirdSpell;
    void InGameButtonX()
    {
        if (IM.XBoxControllerInspector.triggerRight.isUsed)
        {
            //spells
            OnThirdSpell?.Invoke();
        }
        else
        {

        }
    }

    public event VoidDelegate OnLastSpell;
    void InGameButtonY()
    {
        if (IM.XBoxControllerInspector.triggerRight.isUsed)
        {
            //spells
            OnLastSpell?.Invoke();
        }
        else
        {
            //inventory
            //change input map to menu map 
        }
    }

    public event VoidDelegate OnStart;
    void InGameStartButton()
    {
        OnStart?.Invoke();
        IM.ChangeInputMap<InGameMenuMap>();
    }

    void testInputMap(typeDirection _dir)
    {
        Debug.Log("TestInputMap " + _dir.ToString());
    }

    void Update()
    {

        if (IM.currentInputMap == this)
        {
            //Zoom Direction
            if (IM.XBoxControllerInspector.stickRight.isUsed)
            {
                isZoom = true;

                if (IM.XBoxControllerInspector.stickRight.currentDirection == typeDirection.Up)
                {
                    currentZoom = 1;
                }
                else if (IM.XBoxControllerInspector.stickRight.currentDirection == typeDirection.Down)
                {
                    currentZoom = -1;
                }
                else
                {
                    currentZoom = 0;
                }
            }

            //display grid + spell shortcut
            if (IM.XBoxControllerInspector.triggerRight.isUsed)
            {
                isPreAction = true;
            }
            else
            {
                isPreAction = false;
            }
        }
    }
}
