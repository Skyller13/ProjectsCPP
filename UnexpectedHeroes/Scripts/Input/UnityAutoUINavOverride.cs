using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class UnityAutoUINavOverride : StandaloneInputModule
{
    protected override AxisEventData GetAxisEventData(float x, float y, float moveDeadZone)
    {
        return base.GetAxisEventData(x, y, moveDeadZone);
    }
}
