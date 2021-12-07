using System;
using System.Collections;
using System.Collections.Generic;
using Kapy.Move;
using UnityEngine;

public class TargetInputMap : KapyInputMap
{
    public override void CustomEnable()
    {
        IM.ONButtonATap += TargetButtonA;
        IM.ONButtonBTap += TargetButtonB;
        IM.ONButtonXTap += TargetButtonX;
        IM.ONButtonYTap += TargetButtonY;
        IM.ONStickLeftTap += SwitchTarget;
    }

    public override void CustomDisable()
    {
        IM.ONButtonATap -= TargetButtonA;
        IM.ONButtonBTap -= TargetButtonB;
        IM.ONButtonXTap -= TargetButtonX;
        IM.ONButtonYTap -= TargetButtonY;
        IM.ONStickLeftTap -= SwitchTarget;
    }

    public event VoidDelegate ONSelect;
    void TargetButtonA()
    {
        ONSelect?.Invoke();
    }

    public event VoidDelegate ONBack;
    void TargetButtonB()
    {
        ONBack?.Invoke();
    }

    public event VoidDelegate OnAccept;
    void TargetButtonX()
    {
        OnAccept?.Invoke();
    }

    public event VoidDelegate ONCancel;
    void TargetButtonY()
    {
        ONCancel?.Invoke();
    }

    public event DirectionDelegate ONSwitch;
    void SwitchTarget(typeDirection _dir)
    {
        ONSwitch?.Invoke(_dir);
    }

    void Update()
    {
        
    }
}
