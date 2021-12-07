using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameMenuMap : KapyInputMap
{

    public override void CustomEnable()
    {
        IM.ONButtonBTap += MenuQuit;
    }

    public override void CustomDisable()
    {
        IM.ONButtonBTap -= MenuQuit;
    }

    void Update()
    {
        
    }

    public event VoidDelegate ONMenuQuit;
    void MenuQuit()
    {
        ONMenuQuit?.Invoke();
    }

    public void ExitInGameMenuMap()
    {
        IM.ChangeInputMap<IngameInputMap>();
    }
}
