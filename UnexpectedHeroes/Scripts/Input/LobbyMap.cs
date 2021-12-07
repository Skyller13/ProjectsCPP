using System.Collections;
using System.Collections.Generic;
using Kapy.Move;
using UnityEngine;

public class LobbyMap : KapyInputMap
{
    void CustomEnable()
    {
        //switch 
        IM.ONStickLeftTap += SwitchMenu;

        //actions
        IM.ONButtonATap += OnButtonATap;
    }

    void CustomDisable()
    {
        //switch 
        IM.ONStickLeftTap -= SwitchMenu;

        //actions
        IM.ONButtonATap -= OnButtonATap;
    }

    public event DirectionDelegate ONSwitchMenu;

    void SwitchMenu(typeDirection _dir)
    {
        ONSwitchMenu?.Invoke(_dir);
    }

    public event VoidDelegate ONSelect;
    void OnButtonATap()
    {
        ONSelect?.Invoke();
    }

    void Update()
    {
        
    }
}
