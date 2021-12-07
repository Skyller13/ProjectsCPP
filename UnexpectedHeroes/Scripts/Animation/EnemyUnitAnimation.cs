using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyUnitAnimation : BaseUnitAnimation
{
    protected override void CustomUpdate()
    {
        base.CustomUpdate();



        if (unit.stateUnit == Unit.StateUnit.DEATH)
        {
            if (unit.GetComponent<FadeUnit>() == null)
            {
                unit.gameObject.AddComponent<FadeUnit>();
            }
            anim.SetBool("IsAlive", false);
        }
    }
}
