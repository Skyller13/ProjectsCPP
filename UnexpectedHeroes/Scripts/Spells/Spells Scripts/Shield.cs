using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shield : MonoBehaviour
{
    private int countDown;
    private Spell spell;
    private Material mat;

    private float increment;

    void Start()
    {
        spell = GetComponent<Spell>();

        countDown = spell.spellData.timeRemain;

        mat = transform.GetComponent<Material>();

        increment = 0f;

        //set parent and position
        Vector3 pos = spell.CurrentUnit.gameObject.transform.position;

        transform.position = new Vector3(pos.x, 1, pos.z);

        Toolbox.Instance.GetManager<SoundManager>().Play(spell.spellData.audioClipCode[0], gameObject);

        foreach (Transform child in spell.CurrentUnit.gameObject.transform)
        {
            if (child.name == transform.name)
            {
                Destroy(child.gameObject);
            }
        }

        transform.parent = spell.CurrentUnit.gameObject.transform;

        if (spell.CurrentUnit.typeUnit == Unit.TypeUnit.Player)
        {
            spell.CurrentUnit.GetComponent<PlayerController>().CallEndAction();
        }

        spell.CurrentUnit.CreateForceShield(spell.spellData.damage, countDown);

        Toolbox.Instance.GetManager<GameManager>().IncrementTurn();
    }

    void Update()
    {
        if (countDown == 0)
        {
            increment += Time.deltaTime;

           mat.SetFloat("Fresnel", increment);

            if (increment >= 2f)
            {
                Destroy(gameObject, 0.2f);
            }
        }
    }
}
