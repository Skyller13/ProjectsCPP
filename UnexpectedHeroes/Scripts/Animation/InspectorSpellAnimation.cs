using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InspectorSpellAnimation : MonoBehaviour
{
    public bool IsAlive = true;
    [SerializeField] Animator anim;

    private void Start()
    {
        anim = GetComponent<Animator>();
    }

    public void EndSpell()
    {
        anim.SetTrigger("End");
    }

    public void KillSpell()
    {
        IsAlive = false;
    }
}
